////#########################################################################################################################################################################################################################################
// Copyright (C) Juillet 2020 Stitch pour https:\\Aquayoup.123.fr
// AI generique npc par classe : Lancier Ver 2022-07-31 (caster simple, combat a distance si 8-30m, sinon combat au corp a corp 0-8m)
//
// ScriptName = Stitch_npc_ai_lancier : npc d'exemple : 15100014
// le "lancer une arme" , "Tir a l'arc" ou "Tir au fusil" est automatique de 8 a 40m et est tributaire de pickpocketloot
// Si pickpocketloot = 0 : Lancier , si pickpocketloot = 1 : Archer , si pickpocketloot = 2 : Fusilier
// spell1 : Attaque principale au corp a corp
// spell2 : Attaque Dot au corp a corp
// spell3 : spell lanc� a l'agro
// spell4 : spell lanc� a l'�vade ou respawn
// spell5 : Buf
// spell6 : Heal(tir sur cible)
//
// Si spell1 = 0 : tirage al�atoire des spells

// Il est possible d'influencer le temp entre 2 cast avec `BaseAttackTime` & `RangeAttackTime` 
// Necessite dans Creature_Template :
// UPDATE `creature_template` SET `spell1` = ATTAQUE, `spell2` = DOT, `spell3` = AGRO, `spell4` = EVADE, `spell5` = BUF, `ScriptName` = ' Stitch_npc_ai_lancier', `AIName` = '' WHERE(entry = 2237);
// REPLACE INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`, `ItemID2`, `ItemID3`, `VerifiedBuild`) VALUES
// (15100014, 1, 44654, 0,116459, 0);	## Lance + arc
//###########################################################################################################################################################################################################################################

#include "CreatureTextMgr.h"

//################################################################################################
//StitchAI AI Lancier
//################################################################################################

class Stitch_npc_ai_lancier : public CreatureScript
{
public: Stitch_npc_ai_lancier() : CreatureScript("Stitch_npc_ai_lancier") { }

		struct Stitch_npc_ai_lancierAI : public ScriptedAI
		{
			Stitch_npc_ai_lancierAI(Creature* creature) : ScriptedAI(creature) { }

			uint32 Random;
			uint32 Random_AI = 0;
			uint32 DistanceDeCast = 40;												// Distance max a laquelle un npc attaquera , au dela il quite le combat
			uint32 ResteADistance = 15;												// Distance max a laquelle un npc s'approchera
			uint32 Dist;															// Distance entre le npc et sa cible
			Unit* victim = me->GetVictim();										 
			uint32 Mana;
			uint32 MaxMana = me->GetMaxPower(POWER_MANA);
			uint32 ForceBranche = me->GetCreatureTemplate()->pickpocketLootId;
			uint32 MessageAlagro = 0;
			uint32 Spell_ContreAttaque = 0;
			uint32 Spell_Canalise_hc = me->m_spells[7];								// Pour spell canalis� hors combat
			float x = 0.0f, y = 0.0f, z = 0.0f;
			uint32 mapid = 0;

			// Definitions des variables Cooldown et le 1er lancement
			uint32 Cooldown_Spell1 = 1000;
			uint32 Cooldown_Spell2 = 3000;
			uint32 Cooldown_ResteADistance = 2000;									// Test si en contact
			uint32 Cooldown_ResteADistance_Defaut = 4000;
			uint32 Cooldown_Anti_Bug_Figer = 2000;
			uint32 Cooldown_Npc_Emotes = urand(5000, 8000);
			uint32 Cooldown_RegenMana = 5000;
			uint32 Cooldown_RegenMana_defaut = 3500;
			uint32 Cooldown_Spell_Heal = 5000;
			uint32 Cooldown_Spell_Heal_defaut = 6000;
			uint32 Cooldown_Spell_ContreAttaque = 4000;
			uint32 Cooldown_Spell_ContreAttaque_defaut = 8000;
			uint32 Cooldown_Principal_B = 2000;										// Tempo pour s"eloigner
			uint32 Cooldown_Principal_B_Defaut = 3000;								
			uint32 Cooldown_Principal_C = 250;										// Tempo pour arreter le mouvement
			uint32 Cooldown_Principal_C_Defaut = 1500;
			uint32 Cooldown_Spell_Canalise_hc = 3000;
			uint32 Cooldown_Spell_Canalise_hc_defaut = 3000;						// Sort canalis� hors combat

			// Spells
			uint32 Buf_1 = 0;
			uint32 liste_Buf[4] = { 6673, 1160, 97462, 8599 };						// Cri de guerre 6673, Cri d�moralisant 1160 (8s 10m Soi-m�me), Cri de ralliement 97462 (+10% pv), Enrager 8599

			uint32 Spell_agro = 0;
			uint32 liste_agro[2] = { 133308, 79444 };								// Lancer le filet 133308 (25m 5s), Empaler 79444 (Jette une lance 60m) 

			uint32 Spell_1;
			uint32 liste_spell_1[4] = { 29426, 126799, 118326, 172851 };			// Frappe h�ro�que 29426, Frappe tranchante 126799, Attaque vicieuse 118326, Encha�nement 172851

			uint32 Spell_2 = 0;
			uint32 liste_spell_2[4] = { 127171, 118532, 772, 62317 };				// Fendoir vicieux 15/lvl + 2/lvl/1s cumulable 5 fois 127171, Entaille infect�e 118532, Pourfendre 772, D�vaster 62317	

			uint32 Lancer_une_Arme = 42332;											// 8-40m Empaler 79444, Lancer une arme 42332 
			uint32 Tir_Arc = 95826;
			uint32 Tir_Fusil = 6660;
			uint32 Tir_1 = 42332;
			uint32 Spell_respawn_evade = 22812; 									// Ecorce 22812;
			uint32 Spell_Heal = 0;													// Tir dur cible (pour drain)

			// Emotes
			uint32 Npc_Emotes[22] = { 1,3,7,11,15,16,19,21,22,23,24,53,66,71,70,153,254,274,381,401,462,482 };

			uint32 Start_Agro = 0;

			void InitializeAI() override
			{
				// Forcer le choix par creature_template->pickpocketloot . Lancier = 0 , Archer = 1 , Fusilier = 2, Lancier (Distant) = 3 , Archer = 4  (Distant) , Fusilier= 5 (Distant) 
				//ForceBranche = me->GetCreatureTemplate()->pickpocketLootId;
				if (ForceBranche == 1 || ForceBranche == 4 || ForceBranche == 7) { Tir_1 = Tir_Arc; }									// Tir a l'arc
				else if (ForceBranche == 2 || ForceBranche == 5 || ForceBranche == 8) { Tir_1 = Tir_Fusil; }							// Tir au fusil
				else if (ForceBranche == 0 || ForceBranche == 3 || ForceBranche == 6) { Tir_1 = Lancer_une_Arme; }						// Lance une arme		

				if (Tir_1 != Lancer_une_Arme) { me->SetSheath(SHEATH_STATE_RANGED); }				// S'�quipe d'arc ou fusil
				else
				{
					me->SetSheath(SHEATH_STATE_MELEE);												// S'equipe de l'arme au contact
				}

				// Buf au lancement du serveur (Pet, armure de givre, etc)
				uint32 Tmp = me->m_spells[3];
				if (Tmp != 0)
				{
					me->CastSpell(me, Tmp, true);
				}
			}

			void Init_AI()
			{

				// Message a l'agro forc� par spell(8)
				if (me->m_spells[7] == 1) { MessageAlagro = 1; }

				// Spell contre attaque si PV bas
				if (me->m_spells[6] != 0) { Spell_ContreAttaque = me->m_spells[6]; }

				// ################################################################################################################################################
				// D�finition des spells
				// ################################################################################################################################################
				// spell1 : Attaque principale
				// spell2 : Dot
				// spell3 : spell lanc� a l'agro
				// spell4 : spell lanc� a l'�vade ou respawn
				// spell5 : Buf
				// spell6 : Heal(tir sur cible)

				// Si aucun spell d�fini dans creature_template->spell[1] : tirage al�atoire des sorts
				if (me->m_spells[0] == 0) 
				{ 
						Buf_1 = liste_Buf[urand(0, 3)];
						Spell_agro = liste_agro[urand(0, 1)];
						Spell_1 = liste_spell_1[urand(0, 3)];
						Spell_2 = liste_spell_2[urand(0, 3)];
						Cooldown_ResteADistance_Defaut = 4000;
						Cooldown_RegenMana_defaut = 4000;
				}
				else 
				{ 
					Spell_1 = me->m_spells[0];
					if (me->m_spells[1] != 0) { Spell_2 = me->m_spells[1]; } else Spell_2 = 0;
					if (me->m_spells[2] != 0) { Spell_agro = me->m_spells[2]; } else Spell_agro = 0;
					if (me->m_spells[3] != 0) { Spell_respawn_evade = me->m_spells[3]; } else Spell_respawn_evade = 0;
					if (me->m_spells[4] != 0) { Buf_1 = me->m_spells[4]; } else Buf_1 = 0;
					if (me->m_spells[5] != 0) { Spell_Heal = me->m_spells[5]; } else Spell_Heal = 0;

				}


				Random_AI = urand(0,1);		// Caster_Puis_Contact ou Caster

				// Reste a distance variable suivant ci le mob est a l'ext�rieur ou a l'Int�rieur
				if (me->GetMap()->IsOutdoors(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()))
				{
					ResteADistance = urand(14, 16);
				}
				else
				{
					ResteADistance = 7;
				}

				Cooldown_Spell_Canalise_hc_defaut = me->GetCurrentSpellCastTime(Spell_Canalise_hc);
				if (Cooldown_Spell_Canalise_hc_defaut < 3000)
					Cooldown_Spell_Canalise_hc_defaut = urand(5000,15000);

				Cooldown_Spell_Canalise_hc_defaut = Cooldown_Spell_Canalise_hc_defaut + urand(1000, 2000);

				// ################################################################################################################################################
				// Divers
				// ################################################################################################################################################
				if (Tir_1 != Lancer_une_Arme) { me->SetSheath(SHEATH_STATE_RANGED); }				// S'�quipe d'arc ou fusil
				else
				{
					me->SetSheath(SHEATH_STATE_MELEE);												// S'equipe de l'arme au contact
				}

				me->SetReactState(REACT_AGGRESSIVE);
				//me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);								// ROOT
				// ################################################################################################################################################
			}
			void JustRespawned() override
			{
				me->GetMotionMaster()->MoveTargetedHome();								// Retour home pour rafraichir client
				//me->SetSpeedRate(MOVE_RUN, 1.01f);
				me->SetReactState(REACT_AGGRESSIVE);

				Init_AI();
				Random = urand(1, 2);
				if (Random == 1 && Spell_respawn_evade != 0) { me->CastSpell(me, Spell_respawn_evade, true); }		// 1/2 Chance de lancer le sort au respawn ou evade
			}

			void EnterCombat(Unit* /*who*/) override
			{
				Init_AI();
				me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);								// ROOT

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
				RetireBugDeCombat();
				me->AddUnitState(UNIT_STATE_EVADE);
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

				if (Tir_1 != Lancer_une_Arme) { me->SetSheath(SHEATH_STATE_RANGED); }				// S'�quipe d'arc ou fusil
				else
				{
					me->SetSheath(SHEATH_STATE_MELEE);												// S'equipe de l'arme au contact
				}
			}
			void UpdateAI(uint32 diff) override
			{
				if (me->HasUnitState(UNIT_STATE_CONFUSED) || me->HasUnitState(UNIT_STATE_STUNNED) || me->HasUnitState(UNIT_STATE_DISTRACTED) || me->HasUnitState(UNIT_STATE_CANNOT_TURN) || me->HasUnitState(UNIT_STATE_CONTROLLED) || me->HasUnitState(UNIT_STATE_POSSESSED) || me->HasUnitState(UNIT_STATE_CONFUSED_MOVE))
					return;
				// ################################################################################################################################################
				// Emotes hors combat & mouvement #################################################################################################################
				// ################################################################################################################################################
				if ((Cooldown_Npc_Emotes <= diff) && (!me->isMoving()) && (!me->IsInCombat()))
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

					// Ce heal s'il a un sort de Heal(tir sur cible) -----------------------------------------------------------------
					Heal_En_Combat_Caster(diff);

					me->SetSheath(SHEATH_STATE_RANGED);

					//----------------------------------------------------------------------------------------------------------------

					if (Start_Agro == 0)
					{
						Start_Agro = 1;
						
						// Message a l'agro , ci le mob a plusieurs lignes (creature_text groupid>0) il y a de forte chance que ce soit pour un dialogue
						// et non un simple message a l'agro. Donc on l'ignore.
						Random = urand(1, 5);
						if ((sCreatureTextMgr->TextExist(me->GetEntry(), 0)) && (!sCreatureTextMgr->TextExist(me->GetEntry(), 1) && Random == 1) || MessageAlagro == 1)
						{
							Talk(0);
						}

						// Spell a lancer a l'agro ------------------------------------------------------------------------------------------------------------------------
						Random = urand(1, 3);
						if (Random != 1 && Buf_1 != 0) { me->CastSpell(me, Buf_1, true); }				// 2/3 de ce buf

						Random = urand(1, 3);
						if (Random != 1 && Spell_agro != 0 && Dist > 8) { DoCastAOE(Spell_agro,true); }	// 2/3 Chance de lancer le sort d'agro

						if (Spell_respawn_evade != 0) { me->CastSpell(me, Spell_respawn_evade, true); }

					}

					// ############################################################################################################################################
					// Regen mana en combat -----------------------------------------------------------------------------------------------------------------------
					if (Cooldown_RegenMana <= diff)
					{
						me->SetPower(POWER_MANA, Mana + (MaxMana / 100));
						if (Mana > MaxMana) { me->SetPower(POWER_MANA, MaxMana); }

						Cooldown_RegenMana = Cooldown_RegenMana_defaut;
					}
					else Cooldown_RegenMana -= diff;

					// Combat au contat ---------------------------------------------------------------------------------------------------------------------------
					if (Dist < 6)
					{
						me->SetSheath(SHEATH_STATE_MELEE);												// S'equipe de l'arme au contact
						// Spell1 sur la cible  
						if (Cooldown_Spell1 <= diff && Spell_1 !=0)
						{
							me->StopMoving();
							//Bonus_Degat_Arme_Done(100);
							DoCast(victim, Spell_1);
							DoMeleeAttackIfReady();																// Combat en m�l�e
							//Bonus_Degat_Arme_Done(-100);
							Cooldown_Spell1 = urand(3000,3500);
						}
						else Cooldown_Spell1 -= diff;


						// Spell2 sur la cible  
						if (Cooldown_Spell2 <= diff && Spell_2 != 0)
						{
							me->CastSpell(victim, Spell_2, true);
							Cooldown_Spell2 = urand(5000, 7000);

						}
						else Cooldown_Spell2 -= diff;
					}

					// Combat a distance --------------------------------------------------------------------------------------------------------------------------
					if (Dist >= 6)
					{

						// Spell1 sur la cible  
						if (Cooldown_Spell1 <= diff )	//Uniquement si Spell_2 non defini et avec la cadence de Spell_1
						{
							if (Tir_1 != Lancer_une_Arme) { me->SetSheath(SHEATH_STATE_RANGED); }				// S'�quipe d'arc ou fusil
							else
							{
								me->SetSheath(SHEATH_STATE_MELEE);												// S'equipe de l'arme au contact
							}


							me->StopMoving();
							if (Spell_2 != 0)
							{
								me->CastSpell(victim, Spell_2, true);
							}
							else
								DoCast(victim, Tir_1);


							Cooldown_Spell1 = urand(2500,3500);
						}
						else Cooldown_Spell1 -= diff;
					}

					ContreAttaque(diff);

					// Forcer le choix par creature_template->pickpocketloot . 
					//Lancier = 0, Archer = 1, Fusilier = 2, Lancier (Distant) = 3, Archer = 4 (Distant), Fusilier= 5 (Distant), Arme (Random) = 6 , Tir a l'arc = 7  (Random) , Tir au fusil= 8 (Random) 
					if (ForceBranche < 3 || (ForceBranche >=6 && Random_AI ==0) )
					{ 
						Mouvement_Caster_Puis_Contact(diff); 
					}
					else
						Mouvement_Caster(diff);

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
				// Anti bug de combat
				if (me->IsAlive() && (me->GetDistance(me->GetHomePosition()) >  50))
				{
					RetireBugDeCombat();

					EnterEvadeMode(EVADE_REASON_SEQUENCE_BREAK);

					if (me->IsInCombat() && me->getAttackers().empty())
					{
						EnterEvadeMode(EVADE_REASON_NO_HOSTILES);
					}
				}





				if (!UpdateVictim())
					return;

				Dist = me->GetDistance(me->GetVictim());
				if ((Dist > DistanceDeCast) || (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > 40))
				{
					if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE) && me->IsInCombat())
					{
						me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
					}

					RetireBugDeCombat();
					me->AddUnitState(UNIT_STATE_EVADE);
					EnterEvadeMode(EVADE_REASON_SEQUENCE_BREAK);						// Quite le combat si la cible > 30m (Caster & M�l�e) ou > 40m de home
				}
			}

			// ###### Reste a distance mais va au contact si la cible ce raproche ##################################################################################
			void Mouvement_Caster_Puis_Contact(uint32 diff)
			{
				if (!UpdateVictim() || AuraFig�() /*|| me->HasUnitState(UNIT_STATE_CASTING)*/)
					return;

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);



				if (Cooldown_ResteADistance <= diff)
				{
				// Mouvement ON si distance < 8m ------------------------------------------------------------------------------------------------------------------
				if ( Dist < 8)
				{
					me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);							// UNROOT
					AttackStartCaster(victim, 1);														// Distance de cast
					DoMeleeAttackIfReady();																// Combat en m�l�e
				}

				// Mouvement ON si distance > 20m -----------------------------------------------------------------------------------------------------------------
				if (Dist > ResteADistance + 5)
				{
					me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);							// UNROOT
					AttackStartCaster(victim, ResteADistance);											// Distance de cast
					void DoRangedAttackIfReady();														// Combat a distance
				}

				// Mouvement OFF si distance >= 8m & <= 20m -------------------------------------------------------------------------------------------------------
				if ((Dist >= 8) && (Dist <= ResteADistance + 5))
				{
						AttackStart(victim);
						AttackStartCaster(victim, ResteADistance);										// Distance de combat
						void DoRangedAttackIfReady();													// Combat a distance
						me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);							// ROOT
				}
				Cooldown_ResteADistance = 2000;
				}
				else Cooldown_ResteADistance -= diff;

			}
			// ###### Caster , reste a distance ####################################################################################################################
			void Mouvement_Caster(uint32 diff)
			{
				if (!UpdateVictim() /*|| me->HasUnitState(UNIT_STATE_CASTING)*/ || AuraFig�() )
					return;

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				if (Cooldown_Principal_B <= diff)
				{
					// Mouvement al�atoire si cible <= 5m  ---------------------------------------------------------------------------------------------------------

					if (Dist <= 5)
					{
						me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);						// UNROOT
						me->SetWalk(false);																// Ne pas marcher

						if (AuraLenteur() == false)
						{
							me->SetSpeedRate(MOVE_RUN, 1.1f); // Uniquement si non ralenti par un spell joueur
						}

						x = (victim->GetPositionX() + irand(0, ResteADistance * 2) - ResteADistance);
						y = (victim->GetPositionY() + irand(0, ResteADistance * 2) - ResteADistance);
						if (me->GetMap()->IsOutdoors(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()) && !me->IsUnderWater())
						{
							z = victim->GetMap()->GetHeight(victim->GetPhaseMask(), x, y, MAX_HEIGHT, true);
						}
						else
						{
							z = victim->GetPositionZ() + 2;	// Sinon bug en interieur
						}
						mapid = victim->GetMapId();
						me->GetMotionMaster()->MovePoint(mapid, x, y, z);
						Cooldown_Principal_B = Cooldown_Principal_B_Defaut;
						Cooldown_Spell1 = 2750;
						Cooldown_Spell2 = 2750;
					}
				}
				else Cooldown_Principal_B -= diff;


				if (Cooldown_Principal_C <= diff)
				{
					// Mouvement OFF si distance >5m & <= 15m -------------------------------------------------------------------------------------------------------
					if ((Dist > 5) && (Dist <= ResteADistance))
					{
						me->StopMoving();
						AttackStart(victim);
						AttackStartCaster(victim, ResteADistance);										// Distance de combat
						void DoRangedAttackIfReady();													// Combat a distance
						me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);							// ROOT

					}

					// Mouvement ON si distance > 20m -----------------------------------------------------------------------------------------------------------------
					if (Dist > ResteADistance)
					{
						me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);							// UNROOT
						AttackStartCaster(victim, ResteADistance);											// Distance de cast
						void DoRangedAttackIfReady();														// Combat a distance
					}
					Cooldown_Principal_C = Cooldown_Principal_C_Defaut;
				}
				else Cooldown_Principal_C -= diff;
			}

			

			void Heal_En_Combat_Caster(uint32 diff)
			{
				//if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_MOVE))
				//return;

				// Heal(lui meme) -------------------------------------------------------------------------------------------------------------------------------
				if (Cooldown_Spell_Heal <= diff && Spell_Heal != 0 )
				{
					if ((me->GetHealth() < (me->GetMaxHealth()*0.50)) && !me->HasAura(Spell_Heal))		// Si PV =< 50%
					{
						me->CastSpell(me, Spell_Heal, true);
					}
					Cooldown_Spell_Heal = Cooldown_Spell_Heal_defaut;
				}
				else Cooldown_Spell_Heal -= diff;

			}
			void Bonus_Degat_Arme_Done(int val) // 
			{
				// +- Bonus en % de degat des armes inflig�es a victim
				me->HandleStatModifier(UNIT_MOD_ATTACK_POWER, TOTAL_PCT, val, true);
				me->HandleStatModifier(UNIT_MOD_DAMAGE_OFFHAND, TOTAL_PCT, val, true);
				me->HandleStatModifier(UNIT_MOD_DAMAGE_RANGED, TOTAL_PCT, val, true);
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
					) return true;
				else return false;
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

		};




		CreatureAI* GetAI(Creature* creature) const override
		{
			return new Stitch_npc_ai_lancierAI(creature);
		}
};

//#################################################################################################################################################################
void AddSC_npc_ai_lancier()
{
	new Stitch_npc_ai_lancier();
}















