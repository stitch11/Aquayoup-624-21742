////#########################################################################################################################################################################################################################################
// Copyright (C) Juin 2020 Stitch pour Aquayoup
// AI generique npc par classe : MAGE Ver 2022-07-31
// Il est possible d'influencer le temp entre 2 cast avec `BaseAttackTime` & `RangeAttackTime` 
// Necessite dans Creature_Template :
// Minimun  : UPDATE `creature_template` SET `ScriptName` = 'Stitch_npc_ai_mage',`AIName` = '' WHERE (entry = 15100005);
// Optionel : UPDATE `creature_template` SET `HealthModifier` = 2, `ManaModifier` = 3, `ArmorModifier` = 1, `DamageModifier` = 2,`BaseAttackTime` = 2000, `RangeAttackTime` = 2000 WHERE(entry = 15100005);
// Optionel : Utilisez pickpocketloot de creature_template pour passer certains parametres (Solution choisit afin de rester compatible avec tout les cores). Si pickpocketloot = 1 (branche1 forc�), pickpocketloot = 2 (branche2 forc�), etc
//###########################################################################################################################################################################################################################################
// # npc de Test Stitch_npc_ai_Mage  .npc 15100005
// REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `HealthModifier`, `HealthModifierExtra`, `ManaModifier`, `ManaModifierExtra`, `ArmorModifier`, `DamageModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES
// (15100005, 0, 0, 0, 0, 0, 28759, 0, 0, 0, 'npc_ai_Mage', '', '', '', 0, 90, 90, 0, 0, 2102, 0, 1.01, 1.01, 1, 0, 0, 2000, 2000, 1, 1, 2, 0, 2048, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 2, 1, 3, 1, 2, 2, 1, 0, 144, 1, 0, 0, 'Stitch_npc_ai_mage', 20173);
// REPLACE INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`, `ItemID2`, `ItemID3`, `VerifiedBuild`) VALUES
// (15100005, 1, 854, 0, 0, 0),
// (15100005, 2, 854, 0, 0, 0),
// (15100005, 3, 854, 0, 0, 0);
//###########################################################################################################################################################################################################################################

#include "CreatureTextMgr.h"

//################################################################################################
//StitchAI AI Mage
//################################################################################################

class Stitch_npc_ai_mage : public CreatureScript
{
public: Stitch_npc_ai_mage() : CreatureScript("Stitch_npc_ai_mage") { }

		struct Stitch_npc_ai_mageAI : public ScriptedAI
		{
			Stitch_npc_ai_mageAI(Creature* creature) : ScriptedAI(creature) { }

			uint32 BrancheSpe = 1;													// Choix de la Sp�cialisation : Arcane = 1, Feu = 2, Givre = 3
			uint32 NbrDeSpe = 3;													// Nombre de Sp�cialisations 
			uint32 ForceBranche;
			uint32 Random;
			uint32 DistanceDeCast = 40;												// Distance max a laquelle un npc attaquera , au dela il quite le combat
			uint32 ResteADistance = 15;												// Distance max a laquelle un npc s'approchera
			uint32 Dist;															// Distance entre le npc et sa cible
			uint32 Mana;
			uint32 MaxMana = me->GetMaxPower(POWER_MANA);
			uint32 Start_Agro = 0;
			Unit* victim = me->GetVictim();
			uint32 MessageAlagro = 0;
			uint32 Spell_ContreAttaque = 0;

			// Definitions des variables Cooldown et le 1er lancement
			uint32 Cooldown_Spell1 = 500;
			uint32 Cooldown_Spell2 = 2000;
			uint32 Cooldown_Spell3 = 3500;
			uint32 Cooldown_Spell4 = 5500;
			uint32 Cooldown_Spell_Heal = 5000;											// Cooldown pour la fr�quence du heal
			uint32 Cooldown_RegenMana = 3000;											// Cooldown pour le regen du mana
			uint32 Cooldown_ResteADistance = 2000;										// Test si en contact pour s'eloigner
			uint32 Cooldown_Npc_Emotes = urand(5000, 8000);
			uint32 Cooldown_Spell_ContreAttaque = 4000;
			uint32 Cooldown_Spell_ContreAttaque_defaut = 8000;

			// Spells Divers
			uint32 Buf_all = 165743;													// Armure de givre 165743, Armure du mage 6118
			uint32 Buf_branche1 = 0;
			uint32 Buf_branche1_liste[2] = { 1459, 36749 };								// Illumination des Arcanes 1459, Puissance arcanique 36749 (Puissance des sorts +25/1h)
			uint32 Buf_branche2 = 0;
			uint32 Buf_branche2_liste[2] = { 79849, 140468 };							// Armure de la fournaise 79849 (s10/lvl si touch�, crotique +5%), Lueur de la flamme 140468 (dmg -20%, cumulable)
			uint32 Buf_branche3 = 0;
			uint32 Buf_branche3_liste[2] = { 111264, 1459 };							// Garde glaciale 111264 (3 m�l�es 30s), Illumination des Arcanes 1459
			uint32 Spell_Heal_Caster = 12051;											// Evocation 12051

			// Spells Arcane
			uint32 Spell_branche1_agro = 0;
			uint32 Spell_branche1_1 = 0;
			uint32 Spell_branche1_2 = 0;
			uint32 Spell_branche1_3 = 0;
			uint32 Spell_branche1_4 = 0;
			uint32 branche1_agro[3] = { 30449, 102051, 31589 };							// Vol de sort 30449, Givregueule 102051 (silence 8s), Lenteur 31589
			uint32 branche1_1[2] = { 145437, 145437 };									// Barrage des Arcanes 145437
			uint32 branche1_2[3] = { 145434, 145434,128192 };							// D�flagration des Arcanes 145434,Lance d'esprit 128192
			uint32 branche1_3[2] = { 191293, 191293 };									// Projectiles des Arcanes 191293 

			// Spells Feu
			uint32 Spell_branche2_agro = 0;
			uint32 Spell_branche2_1 = 0;
			uint32 Spell_branche2_2 = 0;
			uint32 Spell_branche2_3 = 0;
			uint32 Spell_branche2_4 = 31661;											// Souffle du dragon 31661 (Recharge 20s,12m)
			uint32 branche2_agro[4] = { 56858, 30449, 102051 , 2139 };					// Choc de flammes 56858, Vol de sort 30449, Givregueule 102051 (silence 8s), Contresort 2139
			uint32 branche2_1[2] = { 9053, 9053 };										// Boule de feu 9053 
			uint32 branche2_2[2] = { 38391, 38391 };									// Br�lure 38391 
			uint32 branche2_3[2] = { 14145, 14145 };									// Trait de feu 14145

			// Spells Givre
			uint32 Spell_branche3_agro = 0;
			uint32 Spell_branche3_1 = 0;
			uint32 Spell_branche3_2 = 0;
			uint32 Spell_branche3_3 = 0;
			uint32 branche3_agro[4] = { 140485, 84714, 2139, 124935 };					// Pi�ge � runes gel� 140485 (gele 20s,20m), Sol gel� 84714, Contresort 2139, Torrent 124935
			uint32 branche3_1[4] = { 71318, 71318, 71318,34425 };						// Eclair de givre 71318,Trait d'eau 34425	
			uint32 branche3_2[2] = { 49906, 49906 };									// Javelot de glace 49906
			uint32 branche3_3[2] = { 120, 122 };										// C�ne de froid 120 (Recharge 12s,12m) , Nova de givre 122
			uint32 cone_de_froid = 120;

			// Emotes
			uint32 Npc_Emotes[22] = { 1,3,7,11,15,16,19,21,22,23,24,53,66,71,70,153,254,274,381,401,462,482 };

			void InitializeAI() override
			{
				// Buf au lancement du serveur (Pet, armure de givre, etc)
				uint32 Tmp = me->m_spells[3];
				if (Tmp != 0)
				{
					me->CastSpell(me, Tmp, true);
				}
				me->SetSheath(SHEATH_STATE_UNARMED);								//Arme rang�e
			}

			void Init_AI()
			{
				// ################################################################################################################################################
				// Forcer le choix de la Sp�cialisation par creature_template->pickpocketloot
				// ################################################################################################################################################
				// Forcer le choix de la Sp�cialisation par creature_template->pickpocketloot
				ForceBranche = me->GetCreatureTemplate()->pickpocketLootId;											// creature_template->pickpocketloot
				if (ForceBranche == 1) { BrancheSpe = 1; }															// branche1 forc�
				else if (ForceBranche == 2) { BrancheSpe = 2; }														// branche2 forc� 
				else if (ForceBranche == 3) { BrancheSpe = 3; }														// branche3 forc�
				else
				{
					// Sinon Choix de la Sp�cialisation Al�atoire
					BrancheSpe = urand(1, NbrDeSpe);
				}

				if ((BrancheSpe > NbrDeSpe) || (BrancheSpe == 0)) { BrancheSpe = 2; }

				// Reste a distance variable suivant ci le mob est a l'ext�rieur ou a l'Int�rieur
				if (me->GetMap()->IsOutdoors(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()))
				{
					ResteADistance = urand(14, 16);
				}
				else
				{
					ResteADistance = 7;
				}


				// ################################################################################################################################################
				// Tirages al�atoires des spells
				// ################################################################################################################################################

				// Message a l'agro forc� par spell(8)
				if (me->m_spells[7] == 1) { MessageAlagro = 1; }

				// Spell contre attaque si PV bas
				if (me->m_spells[6] != 0) { Spell_ContreAttaque = me->m_spells[6]; }

				// Spell a lancer a l'agro ------------------------------------------------------------------------------------------------------------------------
				me->CastSpell(me, Buf_all, true);																	// Buf_all sur lui meme
				
				switch (BrancheSpe)
				{
				case 1: // Si Arcane --------------------------------------------------------------------------------------------------------------------------
					Buf_branche1 = Buf_branche1_liste[urand(0, 1)];
					me->CastSpell(me, Buf_branche1, true);															// Buf1 sur lui meme

					me->LoadEquipment(1, true);																		// creature_equip_template 1
					Bonus_Armure(125);																				// Bonus d'armure +25%

					// Tirages al�atoires des spells Arcane 
					Spell_branche1_agro = branche1_agro[urand(0, 2)];
					Spell_branche1_1 = branche1_1[urand(0, 1)];
					Spell_branche1_2 = branche1_2[urand(0, 2)];
					Spell_branche1_3 = branche1_3[urand(0, 1)];
					break;

				case 2: // Si Feu -------------------------------------------------------------------------------------------------------------------------
					Buf_branche2 = Buf_branche2_liste[urand(0, 1)];
					me->CastSpell(me, Buf_branche2, true);															// Buf2 sur lui meme

					me->LoadEquipment(2, true);																		// creature_equip_template 2
					Bonus_Armure(125);																				// Bonus d'armure +25%

					// Tirages al�atoires des spells Feu 
					Spell_branche2_agro = branche2_agro[urand(0, 3)];
					Spell_branche2_1 = branche2_1[urand(0, 1)];
					Spell_branche2_2 = branche2_2[urand(0, 1)];
					Spell_branche2_3 = branche2_3[urand(0, 1)];
					break;

				case 3: // Si Givre -------------------------------------------------------------------------------------------------------------------------
					Buf_branche3 = Buf_branche3_liste[urand(0, 1)];
					me->CastSpell(me, Buf_branche3, true);															// Buf3 sur lui meme

					me->LoadEquipment(3, true);																		// creature_equip_template 3
					Bonus_Armure(125);																				// Bonus d'armure +25%

					// Tirages al�atoires des spells Givre 
					Spell_branche3_agro = branche3_agro[urand(0, 3)];
					Spell_branche3_1 = branche3_1[urand(0, 3)];
					Spell_branche3_2 = branche3_2[urand(0, 1)];
					Spell_branche3_3 = branche3_3[urand(0, 1)];
					break;
				}
				// ################################################################################################################################################
				// Divers
				// ################################################################################################################################################
				me->SetSheath(SHEATH_STATE_MELEE);																		// S'equipe de l'arme au contact
				me->SetReactState(REACT_AGGRESSIVE);
				// ################################################################################################################################################
			}

			void JustRespawned() override
			{
				me->GetMotionMaster()->MoveTargetedHome();								// Retour home pour rafraichir client
				//me->SetSpeedRate(MOVE_RUN, 1.01f);
				me->SetReactState(REACT_AGGRESSIVE);

				Init_AI();
				me->SetSheath(SHEATH_STATE_UNARMED);								//Arme rang�e
			}
			void EnterCombat(Unit* /*who*/) override
			{
				ResteADistance = 10 + urand(0, 5);
				Init_AI();

				if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == WAYPOINT_MOTION_TYPE)
				{
					me->StopMoving();
					me->GetMotionMaster()->MoveIdle();
				}
				me->SetSheath(SHEATH_STATE_MELEE);									//Arme sortie

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
				me->GetMotionMaster()->MoveTargetedHome();								// Retour home
				me->RemoveAllControlled();												// renvois pet
				Bonus_Armure(100);														// Retire bonus d'armure
			}
			void JustReachedHome() override
			{
				me->RemoveAura(Buf_branche1);
				me->RemoveAura(Buf_branche2);
				me->RemoveAura(Buf_branche3);
				me->RemoveAura(Buf_all);

				me->SetSheath(SHEATH_STATE_UNARMED);								//Arme rang�e
				me->SetReactState(REACT_AGGRESSIVE);
				Bonus_Armure(100);														// Retire bonus d'armure
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
					Cooldown_Npc_Emotes = urand(8000, 15000);
				}
				else
					Cooldown_Npc_Emotes -= diff;

				// ################################################################################################################################################
				// En Combat ######################################################################################################################################
				// ################################################################################################################################################
				if (UpdateVictim() )
				{
					Mana = me->GetPower(POWER_MANA);
					Unit* victim = me->GetVictim();
					Dist = me->GetDistance(victim);

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

						// ################################################################################################################################################
						// Spell a lancer a l'agro 
						// ################################################################################################################################################
						switch (BrancheSpe)
						{
						case 1: // Si Arcane --------------------------------------------------------------------------------------------------------------------------
							me->CastSpell(me, Buf_all, true);														// Buf_all sur lui meme
							me->CastSpell(me, Buf_branche1, true);													// Buf sur lui meme

							Random = urand(1, 2);
							if (Random == 1 && UpdateVictim()) { DoCastVictim(Spell_branche1_agro); }				// 1/2 Chance de lancer le sort d'agro
							break;

						case 2: // Si Feu -------------------------------------------------------------------------------------------------------------------------
							me->CastSpell(me, Buf_all, true);														// Buf_all sur lui meme
							me->CastSpell(me, Buf_branche2, true);													// Buf sur lui meme

							Random = urand(1, 2);
							if (Random == 1 && UpdateVictim()) { DoCastVictim(Spell_branche2_agro); }				// 1/2 Chance de lancer le sort d'agro
							//if (Random == 1) { me->CastSpell(victim, Spell_branche2_agro, true); }		// 1/2 Chance de lancer le sort d'agro
							break;

						case 3: // Si Givre -------------------------------------------------------------------------------------------------------------------------
							me->CastSpell(me, Buf_all, true);														// Buf_all sur lui meme
							me->CastSpell(me, Buf_branche3, true);													// Buf sur lui meme

							Random = urand(1, 2);
							if (Random == 1 && UpdateVictim()) { DoCastVictim(Spell_branche3_agro); }				// 1/2 Chance de lancer le sort d'agro
							break;

							// ################################################################################################################################################
						}
					}


					// ############################################################################################################################################
					// Combat suivant la Sp�cialisation
					if (me->HasUnitState(UNIT_STATE_CASTING))
					{
						me->ClearUnitState(UNIT_STATE_MOVING);
					}


					switch (BrancheSpe)
					{
					case 1: // Sp�cialisation Arcane ##############################################################################################################
							// Regen mana en combat -------------------------------------------------------------------------------------------------------------------
						if (Cooldown_RegenMana <= diff)
						{
							me->SetPower(POWER_MANA, Mana + (MaxMana / 12));
							if (Mana > MaxMana) { me->SetPower(POWER_MANA, MaxMana); }
							Cooldown_RegenMana = urand(2500, 3000);
						}
						else Cooldown_RegenMana -= diff;

						// Combat -------------------------------------------------------------------------------------------------------------------------------------
						// Spell3 sur la cible 
						if (Cooldown_Spell3 <= diff)
						{
							DoCastVictim(Spell_branche1_3);
							Cooldown_Spell3 = 10000;
						}
						else Cooldown_Spell3 -= diff;

						// Spell2 sur la cible 
						if (Cooldown_Spell2 <= diff)
						{
							DoCastVictim(Spell_branche1_2);
							Cooldown_Spell2 = 4000;
						}
						else Cooldown_Spell2 -= diff;

						// Spell1 sur la cible 
						if (Cooldown_Spell1 <= diff && !me->HasUnitState(UNIT_STATE_MOVE))
						{
								DoCastVictim(Spell_branche1_1);
								Cooldown_Spell1 = 3000;
						}
						else Cooldown_Spell1 -= diff;
						break;

					case 2: // Sp�cialisation Feu #############################################################################################################
							// Regen mana en combat -------------------------------------------------------------------------------------------------------------------
						if (Cooldown_RegenMana <= diff)
						{
							me->SetPower(POWER_MANA, Mana + (MaxMana / 12));
							if (Mana > MaxMana) { me->SetPower(POWER_MANA, MaxMana); }
							Cooldown_RegenMana = urand(2000, 2500);
						}
						else Cooldown_RegenMana -= diff;

						// Combat -------------------------------------------------------------------------------------------------------------------------------------
						// Spell1 sur la cible chaque (Sort R�guli�)
						if (Cooldown_Spell1 <= diff && !me->HasUnitState(UNIT_STATE_MOVE))
						{
							DoCastVictim(Spell_branche2_1);
							Cooldown_Spell1 = 3500;
						}
						else Cooldown_Spell1 -= diff;

						// Spell2 sur la cible 
						if (Cooldown_Spell2 <= diff)
						{
							DoCastVictim(Spell_branche2_2);
							Cooldown_Spell2 = urand(2500, 3500);
						}
						else Cooldown_Spell2 -= diff;

						// Spell3 sur la cible 
						if (Cooldown_Spell3 <= diff)
						{
							DoCastVictim(Spell_branche2_3);
							Cooldown_Spell3 = urand(6000, 8000);
						}
						else Cooldown_Spell3 -= diff;

						// Spell4 sur la cible 
						if (Cooldown_Spell4 <= diff && Dist <= 10)
						{
							DoCastVictim(Spell_branche2_4);
							Cooldown_Spell4 = urand(12000, 14000);
						}
						else Cooldown_Spell4 -= diff;
						break;

					case 3: // Sp�cialisation Givre #############################################################################################################
							// Regen mana en combat ------------------------------------------------------------------------------------------------------------------------
						if (Cooldown_RegenMana <= diff)
						{
							me->SetPower(POWER_MANA, Mana + (MaxMana / 15));
							if (Mana > MaxMana) { me->SetPower(POWER_MANA, MaxMana); }
							Cooldown_RegenMana = urand(2000, 2500);
						}
						else Cooldown_RegenMana -= diff;

						// Combat --------------------------------------------------------------------------------------------------------------------------------------

						// Spell3 sur la cible  (Sort secondaire tres lent , g�n�ralement utilis� comme Dot)


						if (Cooldown_Spell3 <= diff && Dist <= 10  )
						{
							if ( Spell_branche3_3 == cone_de_froid && me->isInFront(victim, Dist) || Spell_branche3_3 != cone_de_froid )
							{
								DoCastVictim(Spell_branche3_3, true);
								Cooldown_Spell3 = urand(15000, 20000);
							}
						}
						else Cooldown_Spell3 -= diff;

						// Spell2 sur la cible chaque (Sort secondaire plus lent)
						if (Cooldown_Spell2 <= diff)
						{
							DoCastVictim(Spell_branche3_2, true);
							Cooldown_Spell2 = urand(2500, 3500);
						}
						else Cooldown_Spell2 -= diff;

						// Spell1 sur la cible chaque (Sort R�guli�)
						if (Cooldown_Spell1 <= diff && !me->HasUnitState(UNIT_STATE_MOVE) )
						{
							DoCastVictim(Spell_branche3_1/*, true*/);
							Cooldown_Spell1 = 3500;
						}
						else Cooldown_Spell1 -= diff;

						break;

					}

					// ################################################################################################################################################

					Heal_En_Combat_Caster(diff);
					ContreAttaque(diff);
					Mouvement_Caster(diff);
				}
				// ################################################################################################################################################
				Mouvement_All();
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

					if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE) && me->IsInCombat())
					{
						me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
					}

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
					RetireBugDeCombat();
					me->AddUnitState(UNIT_STATE_EVADE);
					EnterEvadeMode(EVADE_REASON_SEQUENCE_BREAK);						// Quite le combat si la cible > 30m (Caster & M�l�e) ou > 40m de home
				}
			}
			void Mouvement_Caster(uint32 diff)
			{
				if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING) || AuraFig�() )
					return;

				Mana = me->GetPower(POWER_MANA);
				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				if (Cooldown_ResteADistance <= diff)
				{
					// Mouvement al�atoire si cible < 6m & Mana > 5% --------------------------------------------------------------------------------------------------

					if ((Dist <6) && (Mana > MaxMana / 20))
					{
						me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);						// UNROOT
						me->SetWalk(false);																// Ne pas marcher

						if (AuraLenteur() == false && !Interieur())
						{
							me->SetSpeedRate(MOVE_RUN, 1.1f); // Uniquement si non ralenti par un spell joueur
						}

						float x = 0.0f, y = 0.0f, z = 0.0f;
						uint32 mapid = 0;

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
						mapid = victim->GetMapId();
						me->GetMotionMaster()->MovePoint(mapid, x, y, z);
						Cooldown_ResteADistance = 2000;
					}
				}
				else Cooldown_ResteADistance -= diff;

				// Speed normal si distance > 10m ------------------------------------------------------------------------------------------------------------------

				// Mouvement OFF si Mana > 5% & distance >= 6m & <= 10m ---------------------------------------------------------------------------------------------
				if ((Mana > MaxMana / 20) && (Dist >= ResteADistance - 4) && (Dist <= ResteADistance))
				{
					if (me->isMoving())																	// Sinon bug d'animation
					{
						AttackStart(victim);
						AttackStartCaster(victim, ResteADistance);										// Distance de combat
						void DoRangedAttackIfReady();													// Combat a distance
						me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);							// ROOT
					}
				}

				// Mouvement ON si distance > 15m ------------------------------------------------------------------------------------------------------------------
				if (Dist > ResteADistance)
				{
					me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);						// UNROOT
					AttackStartCaster(victim, ResteADistance);										// Distance de cast
					void DoRangedAttackIfReady();													// Combat a distance
					if (AuraLenteur() == false)
					{
						me->SetSpeedRate(MOVE_RUN, 1.0f); // Uniquement si non ralenti par un spell joueur
					}
				}

				// Mouvement ON si Mana < 5%  ----------------------------------------------------------------------------------------------------------------------
				if (Mana < MaxMana / 20)
				{
					me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);							// UNROOT
					if (AuraLenteur() == false)
					{
						me->SetSpeedRate(MOVE_RUN, 1.0f); // Uniquement si non ralenti par un spell joueur
					}
				}

			}
			void Heal_En_Combat_Caster(uint32 diff)
			{
				if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_MOVE))
					return;

				if (Cooldown_Spell_Heal <= diff)
				{
					// heal sur lui meme-------------------------------------------------------------------------------------------------------------------------------
					if ((me->GetHealth() < (me->GetMaxHealth()*0.40)))								// Si PV < 40%
					{
						DoCastVictim(Spell_Heal_Caster);
						Cooldown_Spell_Heal = 45000;
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
			void Bonus_Armure(int val) // 
			{
				// +- Bonus d'armure 100% = pas de bonus/malus   ex : Bonus_Armure(115); // Bonus d'armure +15%
				me->SetModifierValue(UNIT_MOD_ARMOR, BASE_VALUE, me->GetArmor() * (val / 100));
				me->SetCanModifyStats(true);
				me->UpdateAllStats();
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
					) return true;
				else return false;
			}
			bool Interieur()
			{
				if (me->GetMap()->IsOutdoors(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()))
					return false;
				else return true;
			}
		};
		
		CreatureAI* GetAI(Creature* creature) const override
		{
			return new Stitch_npc_ai_mageAI(creature);
		}
};

//##################################################################################################################################################################
void AddSC_npc_ai_mage()
{
	new Stitch_npc_ai_mage;
}















