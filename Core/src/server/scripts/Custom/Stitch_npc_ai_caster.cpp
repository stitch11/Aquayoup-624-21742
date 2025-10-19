//#########################################################################################################################################################################################################################################
// Copyright (C) Juillet 2020 Stitch pour https:\\Aquayoup.123.fr
// AI generique npc par classe : Caster Ver 2024-09-01 (caster simple, combat a distance)
//
// ScriptName = Stitch_npc_ai_caster : npc d'exemple : 15100013
// spell1 : Attaque principale
// spell2 : Dot
// spell3 : spell lanc� a l'agro
// spell4 : spell lanc� a l'�vade ou respawn
// spell5 : Buf
// spell6 : Heal(lui m�me uniquement)
// 
// Si spell[1] = 0 :
// alors affectation al�atoire de tous les spells(pr�d�fini dans le core), sinon utilisera les spells d�finis dans creature_template spell[1a5]
// Ou bien, si spell[1] = 0 : Il est possible de forcer le choix en �ditant le pickpocketloot de creature_template avec le num�ro de la Classe :
// 
// Ou bien si spell1 = 0
// pickpocketloot = 0 : Random
// pickpocketloot = 1 : Mage feu
// pickpocketloot = 2 : Mage Givre
// pickpocketloot = 3 : Demo,
// pickpocketloot = 4 : Druide
// pickpocketloot = 5 : Pretre
// pickpocketloot = 6 : DK Chaos
// pickpocketloot = 7 : Le mob ne cherchera plus a garder une certaine distance de sa cible et restera a sa place

// Il est possible d'influencer le temp entre 2 cast avec `BaseAttackTime` & `RangeAttackTime` 
// Necessite dans Creature_Template :
// UPDATE `creature_template` SET `spell1` = ATTAQUE , `spell2` =  DOT , `spell3` =  AGRO, `spell4` = EVADE , `spell5` = BUF , `spell6` =  HEAL , `ScriptName` = ' Stitch_npc_ai_caster',`AIName` = '' WHERE (entry = 2237);
// REPLACE INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`, `ItemID2`, `ItemID3`, `VerifiedBuild`) VALUES
// (15100013, 1, 854, 0, 0, 0);	## Baton
//###########################################################################################################################################################################################################################################

#include "CreatureTextMgr.h"


//################################################################################################
//StitchAI AI Caster
//################################################################################################

class Stitch_npc_ai_caster : public CreatureScript
{
public: Stitch_npc_ai_caster() : CreatureScript("Stitch_npc_ai_caster") { }

		struct Stitch_npc_ai_casterAI : public ScriptedAI
		{
			Stitch_npc_ai_casterAI(Creature* creature) : ScriptedAI(creature) { }

			uint32 Npc_Type = me->GetCreatureTemplate()->type;
			uint32 Npc_Family = me->GetCreatureTemplate()->family;
			uint32 Npc_Model = me->GetDisplayId();
			uint32 Random;
			uint32 ForceClasse;
			uint32 DistanceDeCast = 40;												// Distance max a laquelle un npc attaquera , au dela il quite le combat,Est aussi utilis� pour la distance max de son home (DistanceDeCast+20)
			uint32 ResteADistance = 15;												// Distance max a laquelle un npc s'approchera
			uint32 Dist;															// Distance entre le npc et sa cible
			Unit* victim = me->GetVictim();										 
			uint32 Mana;
			uint32 MaxMana = me->GetMaxPower(POWER_MANA);
			uint32 ForceBranche;
			uint32 MessageAlagro = 0;
			uint32 Spell_ContreAttaque = 0;
			uint32 Visuel_Teleportation = 87459;
			uint32 Bond_aleatoire_25m = 300267;
			uint32 Spell_Canalise_hc = me->m_spells[7];								// Sort canalis� hors combat, doit etre fixe et en home

			// Definitions des variables Cooldown et le 1er lancement
			uint32 Cooldown_Spell1 = 1000;
			uint32 Cooldown_Spell1_defaut = urand(3000, 3750);
			uint32 Cooldown_Spell2 = 4000;
			uint32 Cooldown_Spell2_defaut = urand(7000,10000);
			uint32 Cooldown_ResteADistance = 3000;									// Test si en contact
			uint32 Cooldown_ResteADistance_Defaut = 3000;
			uint32 Cooldown_ResteADistance_Teleportation = 3000;
			uint32 Cooldown_ResteADistance_Defaut_Teleportation = urand(5000, 7000);
			uint32 Cooldown_Anti_Bug_Figer = 2000;
			uint32 Cooldown_Npc_Emotes = urand(5000, 8000);
			uint32 Cooldown_Spell_Heal = 5000;
			uint32 Cooldown_Spell_Heal_defaut = 6000;
			uint32 Cooldown_RegenMana = 5000;
			uint32 Cooldown_RegenMana_defaut = 3500;
			uint32 Cooldown_bond_aleatoire_25m = 3000;
			uint32 Cooldown_bond_aleatoire_25m_Defaut = urand(6000, 8000);
			uint32 Cooldown_Spell_Canalise_hc = 1000;
			uint32 Cooldown_Spell_Canalise_hc_defaut = 3000;						// Sort canalis� hors combat
			uint32 Cooldown_Demande_Assistance = 3000;
			uint32 Demande_Assistance_effectu� = 0;
			uint32 auto_peur5s = 8225;
			uint8 me_rank = me->GetCreatureTemplate()->rank;

			// Spells
			uint32 Buf_1 = 0;													
			uint32 Spell_agro = 0;
			uint32 Spell_1 = 0;
			uint32 Spell_2 = 0;
			uint32 Spell_respawn_evade = 0;
			uint32 Spell_Heal = 0;	
			uint32 Cooldown_Spell_ContreAttaque = 4000;
			uint32 Cooldown_Spell_ContreAttaque_defaut = 8000;

			// Mage Feu
			uint32 Liste_Spell_agro_1[4] = { 31589, 2120, 11366, 2136 };		// Lenteur 31589, Choc de flammes 2120, Double explosions pyrotechniques 11366, Trait de feu 2136
			uint32 Liste_Spell_2_1[2] = { 108853, 38391 };						// D�flagration infernale 108853, Br�lure 38391
			// Mage Givre
			uint32 Liste_Spell_agro_2[3] = { 30449, 44572, 2139 };				// Vol de sort 30449, Cong�lation 44572, Contresort 2139
			uint32 Liste_Spell_2_2[2] = { 120, 30455 };							// C�ne de froid 120, Javelot de glace 30455
			// Demo
			uint32 Liste_Spell_2_3[4] = { 172, 44267, 980, 30108 };				// Corruption 172, Immolation 44267, Agonie 980, Affliction instable 30108
			// Druide
			uint32 Liste_Spell_agro_4[3] = { 78674, 102355, 33844 };			// Eruption stellaire 78674, Essaim de lucioles 102355, Sarments 33844
			uint32 Liste_Spell_1_4[2] = { 119577, 5176 };						// Col�re (verte) 119577, Col�re (jaune) 5176
			uint32 Liste_Spell_2_4[2] = { 8921, 8921 };							// Eclat lunaire 8921, Feu stellaire 2912
			uint32 Liste_Spell_Heal_4[3] = { 5185, 5185, 8936 };				// Toucher gu�risseur 5185, Croissance sauvage 48438, R�cup�ration 774, R�tablissement 8936
			// Pretre
			uint32 Liste_Spell_Heal_5[2] = { 2061, 2060 };						// Soins rapides 2061, Soins 2060
			// DK Chaos
			uint32 Liste_Spell_agro_6[4] = { 45524, 47528, 77575, 47476 };		// Cha�nes de glace 45524, Gel de l'esprit 47528, Pouss�e de fi�vre 77575, Strangulation 47476
			uint32 Liste_Spell_1[3] = { 300051, 300052, 300050 };				// Javelot de givre 300051, Javelot de peste 300052, Javelot de sang 300050
			uint32 Liste_Spell_respawn_evade_6[3] = { 57330, 3114, 48792 };		// Cor de l�hiver 57330, Passage de givre 3114, Robustesse glaciale 48792

			// Emotes
			uint32 Npc_Emotes[22] = { 1,3,7,11,15,16,19,21,22,23,24,53,66,71,70,153,254,274,381,401,462,482 };


			uint32 Start_Agro = 0;

			void InitializeAI() override 
			{
				// Buf au lancement du serveur (Pet, armure de givre, etc)
				uint32 Tmp = me->m_spells[3];
				if (Tmp != 0)
				{
					me->CastSpell(me, Tmp, true);
				}
				Spell_canalis�_hc_home();
				Arme_rang�();
			}

			void Init_AI()
			{

				// ################################################################################################################################################
				// Tirages al�atoires des spells
				// ################################################################################################################################################
				// Si m_spells[0] != 0 alors affectation al�atoire de tous les spells (pr�d�finis dans le core) , sinon utilisera les spell d�fini dans vreature_template spells[1 a 5]
				// m_spells[0] : Attaque principale						- Correspond a spell1 de creature_template
				// m_spells[1] : Dot									- Correspond a spell2 de creature_template
				// m_spells[2] : spell lanc� a l'agro					- Correspond a spell3 de creature_template
				// m_spells[3] : spell lanc� a l'�vade ou respawn		- Correspond a spell4 de creature_template
				// m_spells[4] : Buf									- Correspond a spell5 de creature_template
				// m_spells[5] : Heal(lui meme uniquement)				- Correspond a spell6 de creature_template

				if (me->m_spells[0] == 0) 
				{ 
					// Forcer le choix de la classe par creature_template->pickpocketloot
					ForceBranche = me->GetCreatureTemplate()->pickpocketLootId;
					if (ForceBranche == 1 || ForceBranche == 12) { ForceClasse = 1; }										// Mage feu forc�
					else if (ForceBranche == 2 || ForceBranche == 13) { ForceClasse = 2; }									// Mage Givre forc� 
					else if (ForceBranche == 3 || ForceBranche == 14) { ForceClasse = 3; }									// Demo forc�
					else if (ForceBranche == 4 || ForceBranche == 15) { ForceClasse = 4; }									// Druide forc�
					else if (ForceBranche == 5 || ForceBranche == 16) { ForceClasse = 5; }									// Pretre forc�
					else if (ForceBranche == 6 || ForceBranche == 17) { ForceClasse = 6; }									// DK Chaos forc�
					else { ForceClasse = urand(1, 6); }

					// Tirage al�atoire d'une pseudo classe
					switch (ForceClasse)
					{
					case 1: // Mage feu
					case 12:
						Buf_1 = 165743;													// Armure de givre	
						Spell_agro = Liste_Spell_agro_1[urand(0, 3)];
						Spell_1 = 9053;													// Boule de feu 
						Spell_2 = Liste_Spell_2_1[urand(0, 1)];
						Spell_respawn_evade = 19726;									// Aura de r�sistance
						Spell_Heal = 12051;												// Evocation
						Cooldown_Spell2_defaut = 6000;
						Cooldown_Spell_Heal_defaut = 30000;
						Cooldown_ResteADistance_Defaut = 4000;
						Cooldown_RegenMana_defaut = 4000;
						break;
					case 2: // Mage Givre
					case 13:
						Buf_1 = 165743;													// Armure de givre	
						Spell_agro = Liste_Spell_agro_2[urand(0, 2)];
						Spell_1 = 116;													// Eclair de givre
						Spell_2 = Liste_Spell_2_2[urand(0, 1)];
						Spell_respawn_evade = 19726;									// Aura de r�sistance
						Spell_Heal = 12051;												// Evocation
						Cooldown_Spell2_defaut = 7000;
						Cooldown_Spell_Heal_defaut = 30000;
						Cooldown_ResteADistance_Defaut = 4000;
						Cooldown_RegenMana_defaut = 2750;
						break;
					case 3: // Demo
					case 14:
						Buf_1 = 79934;  												// Armure d�moniaque
						Spell_agro = 44267;												// Immolation
						Spell_1 = 686;													// Trait de l'ombre
						Spell_2 = Liste_Spell_2_3[urand(0, 3)];
						Spell_respawn_evade = 0;										// Sombre intention (puissance des sorts + 10 % )
						Spell_Heal = 0;													// Drain de vie 689
						Cooldown_Spell2_defaut = 10000;
						Cooldown_Spell_Heal_defaut = 6000;
						Cooldown_ResteADistance_Defaut = 5000;
						Cooldown_RegenMana_defaut = 2500;
						break;
					case 4: // Druide
					case 15:
						Buf_1 = 1126;													// Marque du fauve
						Spell_agro = Liste_Spell_agro_4[urand(0, 2)];
						Spell_1 = Liste_Spell_1_4[urand(0, 1)];
						Spell_2 = Liste_Spell_2_4[urand(0, 1)]; 							
						Spell_respawn_evade = 22812; 									// Ecorce 22812
						Spell_Heal = Liste_Spell_Heal_4[urand(0, 2)];
						Cooldown_Spell2_defaut = 20000;
						Cooldown_Spell_Heal_defaut = 6000;
						Cooldown_ResteADistance_Defaut = 3500;
						Cooldown_RegenMana_defaut = 1000;
						break;
					case 5: // Pretre
					case 16:
						Buf_1 = 21562;													// Mot de pouvoir� : Robustesse 21562
						Spell_agro = 8092;												// Attaque mentale 8092
						Spell_1 = 585;													// Ch�timent 585
						Spell_2 = 589;													// Mot de l�ombre�: Douleur 589
						Spell_respawn_evade = 528;										// Dissipation de la magie 528, Suppression de la douleur 33206
						Spell_Heal = Liste_Spell_Heal_5[urand(0, 1)];
						Cooldown_Spell2_defaut = 15000;
						Cooldown_Spell_Heal_defaut = 5000;
						Cooldown_ResteADistance_Defaut = 4000;
						Cooldown_RegenMana_defaut = 2000;
						break;
					case 6: // DK Chaos
					case 17:
						Buf_1 = 300049;													// Pr�sence du Chaos 30049
						Spell_agro = Liste_Spell_agro_6[urand(0, 3)];
						Spell_1 = Liste_Spell_1[urand(0, 2)];							// Javelot de givre 300051, Javelot de peste 300052, Javelot de sang 300050
						Spell_2 = 300061;												// Choc impie 300061
						Spell_respawn_evade = Liste_Spell_respawn_evade_6[urand(0, 2)];	
						Spell_Heal = 48982;												// Connexion runique 48982
						Cooldown_Spell2_defaut = 10000;
						Cooldown_Spell_Heal_defaut = 30000;
						Cooldown_ResteADistance_Defaut = 5000;
						Cooldown_RegenMana_defaut = 5000;
						break;
					}

					// Pas de DOT ou Heal si level <15 sinon trop dur
					if (me->getLevel() < 15)
					{
						Spell_2 = 0;
						Spell_Heal = 0;
					}
				} else 
				{ 
					Spell_1 = me->m_spells[0]; 
					Spell_2 = me->m_spells[1]; 
					Spell_agro = me->m_spells[2];
					Spell_respawn_evade = me->m_spells[3];
					Buf_1 = me->m_spells[4];
					Spell_Heal = me->m_spells[5];
				}

				// Message a l'agro forc� par spell(8)
				if (me->m_spells[7] == 1) { MessageAlagro = 1; }

				// Spell contre attaque si PV bas
				if (me->m_spells[6] != 0) { Spell_ContreAttaque = me->m_spells[6]; }


				// Spell a lancer a l'agro ------------------------------------------------------------------------------------------------------------------------
					Random = urand(1, 2);
					if (Random == 1 && Buf_1 != 0) { me->CastSpell(me, Buf_1, true); }

				// ################################################################################################################################################
				// Divers
				// ################################################################################################################################################
				me->SetSheath(SHEATH_STATE_MELEE);												// S'equipe de l'arme au contact
				me->SetReactState(REACT_AGGRESSIVE);

				// Reste a distance variable suivant ci le mob est a l'ext�rieur ou a l'Int�rieur
				if (me->GetMap()->IsOutdoors(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()))
				{
					ResteADistance = urand(14,16);
				}
				else
				{
					ResteADistance = 7;
				}

				// Reste a distance faible forc� 
				if (ForceBranche == 8)
				{
					if (Interieur())
						ResteADistance =4;
					else
						ResteADistance = 7;

				}

				Cooldown_Spell_Canalise_hc_defaut = me->GetCurrentSpellCastTime(Spell_Canalise_hc)+urand(500,1500);
				if (Cooldown_Spell_Canalise_hc_defaut < 2000)
					Cooldown_Spell_Canalise_hc_defaut = 2000;

				// ################################################################################################################################################


			}
			void JustRespawned() override
			{
				me->GetMotionMaster()->MoveTargetedHome();								// Retour home pour rafraichir client
				//me->SetSpeedRate(MOVE_RUN, 1.01f);
				me->SetReactState(REACT_AGGRESSIVE);
				Spell_canalis�_hc_home();

				Init_AI();
				Arme_rang�();
				Random = urand(1, 2);
				if (Random == 1 && Spell_respawn_evade != 0) { me->CastSpell(me, Spell_respawn_evade, true); }		// 1/2 Chance de lancer le sort au respawn ou evade
			}

			void EnterCombat(Unit* /*who*/) override
			{
				Init_AI();
				me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);								// ROOT
				me->SetSheath(SHEATH_STATE_MELEE);									//Arme sortie

				if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == WAYPOINT_MOTION_TYPE)
				{
					me->StopMoving();
					me->GetMotionMaster()->MoveIdle();
				}

				//Retire certaines Aura, emotes & Bytes a l'agro
				me->RemoveAura(80264);	// Retire Marche a pas de loup
				me->RemoveAura(152891);	// Retire Avance a pas de loup invisible
				me->RemoveAura(104015);	// Retire afaiss� / Stun
				me->RemoveAura(153964);	// Retire agenouill�, avec evade
				me->RemoveAura(42648);	// Retire Dort allong� + zzz
				me->RemoveAura(18795);	// Retire Dort allong� + zzz 
				me->RemoveAura(43905);	// Retire Ivre
				me->RemoveAura(101090);	// Retire Danse
				me->HandleEmoteCommand(0);
				me->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0); // Autre fa�on de retirer des �motes pour les cas particuliers
				me->SetByteValue(UNIT_FIELD_BYTES_1, 0, 0);
				me->SetByteValue(UNIT_FIELD_BYTES_2, 0, 0);

			}
			void EnterEvadeMode(EvadeReason /*why*/) override
			{
				Start_Agro = 0;
				Demande_Assistance_effectu� = 0;
				RetireBugDeCombat();
				me->AddUnitState(UNIT_STATE_EVADE);
				//me->SetSpeedRate(MOVE_RUN, 1.5f);													// Vitesse de d�placement
				me->GetMotionMaster()->MoveTargetedHome();											// Retour home
				me->RemoveAllControlled();															// renvois pet

				me->RemoveAura(Buf_1);

				Random = urand(1, 3);
				if (Random == 1 && Spell_respawn_evade != 0) { me->CastSpell(me, Spell_respawn_evade, true); }		// 1/3 Chance de lancer le sort au respawn ou evade
			}
			void JustReachedHome() override
			{
				me->SetReactState(REACT_AGGRESSIVE);
				//me->SetSpeedRate(MOVE_RUN, 1.01f);										// Vitesse par defaut d�finit a 1.01f puisque le patch modification par type,famille test si 1.0f
				Arme_rang�();
			}
			void UpdateAI(uint32 diff) override
			{
				if (me->HasUnitState(UNIT_STATE_CONFUSED) || me->HasUnitState(UNIT_STATE_STUNNED) || me->HasUnitState(UNIT_STATE_DISTRACTED) || me->HasUnitState(UNIT_STATE_CANNOT_TURN) || me->HasUnitState(UNIT_STATE_CONTROLLED) || me->HasUnitState(UNIT_STATE_POSSESSED) || me->HasUnitState(UNIT_STATE_CONFUSED_MOVE))
					return;
				// ################################################################################################################################################
				// Emotes hors combat & mouvement & humanoides ou mort-vivants ####################################################################################
				// ################################################################################################################################################

				if ((Cooldown_Npc_Emotes <= diff) && (!me->isMoving()) && (!me->IsInCombat()) && (Npc_Type == CREATURE_TYPE_HUMANOID || Npc_Type == CREATURE_TYPE_UNDEAD))
				{
					uint32 Npc_Play_Emotes = Npc_Emotes[urand(0, 21)];
					me->HandleEmoteCommand(Npc_Play_Emotes);
					Cooldown_Npc_Emotes = urand(10000, 20000);
				}
				else
					Cooldown_Npc_Emotes -= diff;

				// ################################################################################################################################################
				// En Combat ######################################################################################################################################
				// ################################################################################################################################################
				if (UpdateVictim() /*&& !me->HasUnitState(UNIT_STATE_MOVE) || me->HasUnitState(UNIT_STATE_CASTING)*/)
				{
					Mana = me->GetPower(POWER_MANA);
					Unit* victim = me->GetVictim();
					Dist = me->GetDistance(victim);

					// Ce heal s'il a un sort de heal -------------------------------------------------------------------------------------------------------------
						Heal_En_Combat_Caster(diff);


					if (Start_Agro == 0)
					{
						Start_Agro = 1;
						
							me->StopMoving();

							if (me->HasAura(Spell_Canalise_hc))
								me->RemoveAura(Spell_Canalise_hc);

							// Message a l'agro , ci le mob a plusieurs lignes (creature_text groupid>0) il y a de forte chance que ce soit pour un dialogue
							// et non un simple message a l'agro. Donc on l'ignore.
							Random = urand(1, 5);
							if (sCreatureTextMgr->TextExist(me->GetEntry(), 0) && Random == 1 || MessageAlagro == 1)
							{
								Talk(0);
							}
							else if (sCreatureTextMgr->TextExist(me->GetEntry(), 1) && Random == 1 || MessageAlagro == 1)
							{
								Talk(1);
							}

						// ########################################################################################################################################
						// Spell a lancer a l'agro 
						// ########################################################################################################################################

							Random = urand(1, 2);
							if (Random == 1 && Spell_agro != 0) { DoCast(victim, Spell_agro); }	// 1/2 Chance de lancer le sort d'agro

							if (Spell_respawn_evade != 0) { me->CastSpell(me, Spell_respawn_evade, true); }
							// ####################################################################################################################################
					}


					// ############################################################################################################################################
					// Combat 
					// Regen mana en combat -----------------------------------------------------------------------------------------------------------------------
					if (Cooldown_RegenMana <= diff)
					{
						me->SetPower(POWER_MANA, Mana + (MaxMana / 100));
						if (Mana > MaxMana) { me->SetPower(POWER_MANA, MaxMana); }

						Cooldown_RegenMana = Cooldown_RegenMana_defaut;
					}
					else Cooldown_RegenMana -= diff;

						// Spell1 sur la cible  -------------------------------------------------------------------------------------------------------------------
						if (Cooldown_Spell1 <= diff && Spell_1 !=0 )
						{
							me->CastSpell(victim, Spell_1, true);
							//DoCast(victim, Spell_1);
							//DoMeleeAttackIfReady();
							Cooldown_Spell1 = Cooldown_Spell1_defaut; 

						}
						else Cooldown_Spell1 -= diff;

						// Spell2 sur la cible  -------------------------------------------------------------------------------------------------------------------
						if (Cooldown_Spell2 <= diff && Spell_2 != 0)
						{
							if (!victim->HasAura(Spell_2))
							{
								me->CastSpell(victim, Spell_2, true);
								Cooldown_Spell2 = Cooldown_Spell2_defaut;
							}
						}
						else Cooldown_Spell2 -= diff;

						ContreAttaque(diff);
						Mouvement_Caster(diff);
						Demande_Assistance(diff);

					// ############################################################################################################################################
				}
				// ################################################################################################################################################
				Mouvement_All();

				if (Cooldown_Spell_Canalise_hc <= diff)
				{

					Spell_canalis�_hc_home();
					Cooldown_Spell_Canalise_hc = Cooldown_Spell_Canalise_hc_defaut;
				}
				else Cooldown_Spell_Canalise_hc -= diff;

			}

			void RetireBugDeCombat()
			{
				me->CombatStop(true);
				me->RemoveAllControlled();												// renvois pet
				me->DeleteThreatList();
				me->LoadCreaturesAddon();
				me->SetLootRecipient(NULL);
				me->ResetPlayerDamageReq();
				me->SetLastDamagedTime(0);
				me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);				// UNROOT
				me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);					// Retire flag "en combat"
				me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);				// Retire flag "non attaquable"
			}
			void Mouvement_All()
			{
				// Anti bug de combat : si s'�loigne trop de son home : DistanceDeCast+20
				if (me->IsAlive() && (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > DistanceDeCast+20) )
				{
					RetireBugDeCombat();

					EnterEvadeMode(EVADE_REASON_SEQUENCE_BREAK);

					if (me->IsInCombat() && me->getAttackers().empty())
					{
						EnterEvadeMode(EVADE_REASON_NO_HOSTILES);
					}
				}


				// Si la cible est trop loin >DistanceDeCast
				if (!UpdateVictim())
					return;

				Dist = me->GetDistance(me->GetVictim());
				if ((Dist > DistanceDeCast))
				{
					if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE) && me->IsInCombat())
					{
						me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
					}

					RetireBugDeCombat();
					me->AddUnitState(UNIT_STATE_EVADE);
					EnterEvadeMode(EVADE_REASON_SEQUENCE_BREAK);						
				}
			}
			void Mouvement_Caster(uint32 diff)
			{
				if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
					return;

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);
				ForceBranche = me->GetCreatureTemplate()->pickpocketLootId;

					// Bond al�atoire si cible < 6m & mana > 10%  ---------------------------------------------------------------------------------------------
					if (Cooldown_bond_aleatoire_25m <= diff && !AuraFig�())
					{
						if (Dist <6 && (Mana > MaxMana / 10) && (ForceBranche == 10))
						{
							DoCast(me, Bond_aleatoire_25m);
							Cooldown_bond_aleatoire_25m = Cooldown_bond_aleatoire_25m_Defaut;
							Cooldown_ResteADistance = Cooldown_ResteADistance_Defaut;
						}
					}
					else Cooldown_bond_aleatoire_25m -= diff;


				// Teleportation al�atoire si cible < 6m & mana > 10%  ---------------------------------------------------------------------------------------------
				if (Cooldown_ResteADistance_Teleportation <= diff)
				{
					if (Dist <6 && (Mana > MaxMana / 10) && (ForceBranche == 9))
					{
						Teleport_Au_Tour_Aleatoire();
						Cooldown_ResteADistance_Teleportation = Cooldown_ResteADistance_Defaut_Teleportation;
						Cooldown_ResteADistance = Cooldown_ResteADistance_Defaut;
					}
				}
				else Cooldown_ResteADistance_Teleportation -= diff;

				// ------------------------------------------------------------------------------------------------------------------------------------------------
				if (Cooldown_ResteADistance <= diff && !AuraFig�())
				{
					// Mouvement al�atoire si cible < 6m & mana > 10%  
					if (Dist <=5 && (Mana > MaxMana / 10) && (ForceBranche != 7 && ForceBranche <12 ) )
					{
						
						me->SetWalk(false);																// Ne pas marcher

						if(!AuraLenteur() && !Interieur())
						{
							me->SetSpeedRate(MOVE_RUN, 1.1f); // Uniquement si non ralenti par un spell 
						}

						float x = 0.0f, y = 0.0f, z = 0.0f;
						uint32 mapid = 0;

						me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);						// UNROOT

						x = (victim->GetPositionX() + urand(0, ResteADistance * 2) - ResteADistance);
						y = (victim->GetPositionY() + urand(0, ResteADistance * 2) - ResteADistance);
						if (me->GetMap()->IsOutdoors(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()) && !me->IsUnderWater())
						{
							z = victim->GetMap()->GetHeight(me->GetPhaseMask(), x, y, MAX_HEIGHT, true);
						}
						else 
						{
							z = victim->GetPositionZ();	// Sinon bug en interieur
						}
						mapid = me->GetMapId();
						me->GetMotionMaster()->MovePoint(mapid, x, y, z);
						Cooldown_ResteADistance = Cooldown_ResteADistance_Defaut;
					}
				}
				else 
					Cooldown_ResteADistance -= diff;
					



				// Mouvement OFF si distance >= 8m & <= 15m 
				if ((Dist > 5) && (Dist <= ResteADistance))
				{
					//if (!me->isMoving())																// Sinon bug d'animation
					if (!me->HasUnitState(UNIT_STATE_MOVE))
					{
						AttackStart(victim);
						AttackStartCaster(victim, ResteADistance);										// Distance de combat
						void DoRangedAttackIfReady();													// Combat a distance
						me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);							// ROOT
					}
				}



				// Mouvement ON si distance > 20m 
				if (Dist > ResteADistance)
				{
					me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);							// UNROOT
					AttackStartCaster(victim, ResteADistance);											// Distance de cast
					void DoRangedAttackIfReady();														// Combat a distance
					if (!AuraLenteur())
					{
						me->SetSpeedRate(MOVE_RUN, 1.0f); // Uniquement si non ralenti par un spell 
					}
				}

				// Mouvement OFF si distance =< 20m  & fixe
				if ((Dist <= ResteADistance) && (ForceBranche == 7))
				{
					me->StopMoving();
					me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);								// ROOT
				}


				// Mouvement ON si Mana < 10%  
				if (Mana < MaxMana / 10)
				{
					me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);							// UNROOT
					AttackStartCaster(victim, 1);														// Distance de cast
					DoMeleeAttackIfReady();																// Combat en m�l�e
					if (!AuraLenteur())
					{
						me->SetSpeedRate(MOVE_RUN, 1.0f); // Uniquement si non ralenti par un spell 
					}
				}
			}

			void Tourne_Au_Tour_En_Combat()
			{
				if (!UpdateVictim())
					return;

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				float x = 0.0f, y = 0.0f, z = 0.0f;
				uint32 mapid = 0;

				x = (victim->GetPositionX() + urand(0, 4) - 2);
				y = (victim->GetPositionY() + urand(0, 4) - 2);
				z = victim->GetMap()->GetHeight(me->GetPhaseMask(), x, y, MAX_HEIGHT, true);
				mapid = victim->GetMapId();
				//me->GetClosePoint(x, y, z, me->GetObjectSize() / 3, 3);
				me->GetMotionMaster()->MovePoint(mapid, x, y, z);
			}
			void Avance_3m_En_Combat()
			{
				if (!UpdateVictim())
					return;

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				float x = 0.0f, y = 0.0f, z = 0.0f;
				uint32 mapid = 0;

				x = (victim->GetPositionX());
				y = (victim->GetPositionY());
				z = victim->GetMap()->GetHeight(me->GetPhaseMask(), x, y, MAX_HEIGHT, true);
				mapid = victim->GetMapId();
				me->GetClosePoint(x, y, z, me->GetObjectSize() / 3, 3);
				me->GetMotionMaster()->MovePoint(mapid, x, y, z);
			}
			void Teleport_Au_Tour_Aleatoire()
			{
				if (!UpdateVictim())
					return;

				if (me->HasUnitState(UNIT_STATE_CONFUSED) || me->HasUnitState(UNIT_STATE_STUNNED) || me->HasUnitState(UNIT_STATE_DISTRACTED))
					return;

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				uint32 mapid = victim->GetMapId();
				float x = (victim->GetPositionX() + urand(0, ResteADistance * 2.0f) - ResteADistance);
				float y = (victim->GetPositionY() + urand(0, ResteADistance * 2.0f) - ResteADistance);
				float z = victim->GetMap()->GetHeight(me->GetPhaseMask(), x, y, MAX_HEIGHT, true);

				me->CastSpell(me, Visuel_Teleportation, true);
				me->NearTeleportTo(x, y, z, mapid);
				me->CastSpell(me, Visuel_Teleportation, true);
			}

			void Bonus_Degat_Arme_Done(int val) // 
			{
				// +- Bonus en % de degat des armes inflig�es a victim
				me->HandleStatModifier(UNIT_MOD_ATTACK_POWER, TOTAL_PCT, val, true);
				me->HandleStatModifier(UNIT_MOD_DAMAGE_OFFHAND, TOTAL_PCT, val, true);
				me->HandleStatModifier(UNIT_MOD_DAMAGE_RANGED, TOTAL_PCT, val, true);
			}
			void Bonus_Armure(int val) // 
			{
				// +- Bonus d'armure 100% = pas de bonus/malus   ex : Bonus_Armure(115); // Bonus d'armure +15%
				me->SetModifierValue(UNIT_MOD_ARMOR, BASE_VALUE, me->GetArmor() * (val / 100));
				me->SetCanModifyStats(true);
				me->UpdateAllStats();
			}
			void Heal_En_Combat_Caster(uint32 diff)
			{
				//if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_MOVE))
					//return;

				if (Cooldown_Spell_Heal <= diff && Spell_Heal != 0)
				{
					// heal sur lui meme-------------------------------------------------------------------------------------------------------------------------------
					if ((me->GetHealth() < (me->GetMaxHealth()*0.60)))								// Si PV < 60%
					{
						//DoCast(me, Spell_Heal);
						DoCastAOE(Spell_Heal);
						Cooldown_Spell_Heal = Cooldown_Spell_Heal_defaut;
					}
				}
				else Cooldown_Spell_Heal -= diff;


			}
			void ContreAttaque(uint32 diff)
			{
				if (!UpdateVictim())
					return;
				Unit* victim = me->GetVictim();
				if (victim->HasAura(Spell_ContreAttaque) || Spell_ContreAttaque == 0)
					return;

				// Contre attaque sur la cible (2 chance sur 3) -------------------------------------------------------------------------------------------------
				if (Cooldown_Spell_ContreAttaque <= diff && Spell_ContreAttaque != 0)
				{
					if ((me->GetHealth() < (me->GetMaxHealth()*0.40)))									// Si PV =< 40%
					{
						Random = urand(1, 3);
						if (Random != 1)
						{
							DoCastVictim(Spell_ContreAttaque);
						}
					}
					Cooldown_Spell_ContreAttaque = Cooldown_Spell_ContreAttaque_defaut;
				}
				else Cooldown_Spell_ContreAttaque -= diff;
			}
			bool AuraLenteur()
			{
				if (me->HasAura(137573)		// vitesse (+70%/4s) , annule tous les effets affectant le d�placement
					|| me->HasAura(31224)	// Cape d'ombre    
					|| me->HasAura(1856)	// Disparition
					) return false;

				if (me->HasAura(116)		// Eclair_de_givre 116 
					|| me->HasAura(71318)	// Eclair_de_givre 71318
					|| me->HasAura(31589)	// Lenteur 31589
					|| me->HasAura(6136) 	// Armure_de_givre 6136
					|| me->HasAura(8056) 	// Horion_de_givre 8056
					|| me->HasAura(12548) 	// Horion_de_givre 12548
					|| me->HasAura(9080) 	// Brise_genou 9080
					|| me->HasAura(1715) 	// Brise_genou 1715
					|| me->HasAura(69917) 	// Fievre_de_givre 69917
					|| me->HasAura(67719) 	// Fievre_de_givre 67719
					|| me->HasAura(45477) 	// Toucher_de_glace 45477
					|| me->HasAura(300051) 	// Javelot_de_givre 300051
					|| me->HasAura(300237) 	// Javelot_de_givre 300237
					|| me->HasAura(60192) 	// Gel_de_zone 60192
					|| me->HasAura(116095) 	// Handicap 116095
					|| me->HasAura(300197) 	// Toucher_de_glace 300197
					|| me->HasAura(20170)	// Sceau de justice 20170
					|| me->HasAura(6343)	// Coup de tonnerre
					|| me->HasAura(8147)	// Coup de tonnerre
					) return true;
				else return false;
			}
			bool AuraFig�()
			{
				if (me->HasAura(122)		// Nova de givre
					|| me->HasAura(3600)	// Totem de lien terrestre
					|| me->HasAura(6474)	// Totem de lien terrestre passif
					|| me->HasAura(33844)	// Sarments 4s
					|| me->HasAura(22127)	// Sarments 6s
					|| me->HasAura(31409)	// Sarment multiple
					|| me->HasAura(160402)	// Emprise terrestre (4s, 30m, comme Sarment mais avec des rocher )
					|| me->HasAura(45524)	// Cha�nes de glace
					|| me->HasAura(853)		// Marteau de la justice
					|| me->HasAura(339)		// Sarment du Totem de poigne de terre
					|| me->HasAura(64695)	// Sarment du Totem de poigne de terre
					|| me->HasAura(125467)	// Auto ROOT
					|| me->HasAura(31736)	// Quete 9720
					) return true;
				else return false;
			}
			bool Interieur()
			{
				if (me->GetMap()->IsOutdoors(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()))
					return false;
				else return true;
			}
			void Spell_canalis�_hc_home()
			{
				// Sort canalis� hors combat, doit etre fixe et en home 
				if (Spell_Canalise_hc > 1 && !me->IsInCombat() && !me->HasAura(Spell_Canalise_hc))
				{
					//me->CastSpell(me, Spell_Canalise_hc, true);
					DoCast(me, Spell_Canalise_hc);
				}
			}
			void Arme_rang�()
			{
				// Certains Modelid posent probleme et seront donc ignor�s
				if (Npc_Model == 6824 || Npc_Model == 6825 || Npc_Model == 6821 || Npc_Model == 5773 || Npc_Model == 937 || Npc_Model == 16861)
					return;

				// Uniquement pour les humanoides, mort-vivants + family 0
				if ((Npc_Type != CREATURE_TYPE_HUMANOID && Npc_Type != CREATURE_TYPE_UNDEAD) || Npc_Family != 0 )
					return;

				me->SetSheath(SHEATH_STATE_UNARMED);								//Arme rang�e
			}

			void Demande_Assistance(uint32 diff)
			{
				if (Demande_Assistance_effectu� == 1 || AuraLenteur() == true || !UpdateVictim() || me_rank !=0)
					return;

				if (Cooldown_Demande_Assistance <= diff)
				{
					if ((me->GetHealth() < (me->GetMaxHealth()*0.20)))
					{
						if (urand(1,3) == 1)
						{
							me->CastSpell(me, auto_peur5s, true);
							Demande_Assistance_effectu� = 1;
							Cooldown_ResteADistance = Cooldown_ResteADistance_Defaut;
							Cooldown_Spell1 = Cooldown_Spell1_defaut;
							Cooldown_Spell2 = Cooldown_Spell2_defaut;
						}
						Cooldown_Demande_Assistance = 3000;
					}
				}
				else Cooldown_Demande_Assistance -= diff;
			}


		};


		

		CreatureAI* GetAI(Creature* creature) const override
		{
			return new Stitch_npc_ai_casterAI(creature);
		}
};

//#################################################################################################################################################################
void AddSC_npc_ai_caster()
{
	new Stitch_npc_ai_caster();
}















