////#########################################################################################################################################################################################################################################
// Copyright (C) Juin 2020 Stitch pour Aquayoup
// AI generique npc par classe : Chasseur Ver 2022-07-31
// Equiper l'arc ou le fusil en ItemID2 & ItemID3
// Il est possible d'influencer le temp entre 2 cast avec `BaseAttackTime` & `RangeAttackTime` 
// Necessite dans Creature_Template :
// Minimun  : UPDATE `creature_template` SET `ScriptName` = 'Stitch_npc_ai_chasseur',`AIName` = '' WHERE (entry = 15100004);
// Optionel : UPDATE `creature_template` SET `HealthModifier` = 2, `ManaModifier` = 3, `ArmorModifier` = 1, `DamageModifier` = 2,`BaseAttackTime` = 2000, `RangeAttackTime` = 2000 WHERE(entry = 15100004);
// Optionel : Utilisez pickpocketloot de creature_template pour passer certains parametres (Solution choisit afin de rester compatible avec tout les cores). Si pickpocketloot = 1 (branche1 forcé), pickpocketloot = 2 (branche2 forcé), etc
//###########################################################################################################################################################################################################################################
// # npc de Test Stitch_npc_ai_chasseur  .npc 15100008
// REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `HealthModifier`, `HealthModifierExtra`, `ManaModifier`, `ManaModifierExtra`, `ArmorModifier`, `DamageModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES
//(15100008, 0, 0, 0, 0, 0, 30686, 0, 0, 0, 'npc_ai_chasseur', '', '', '', 0, 90, 90, 0, 0, 2102, 0, 1.01, 1.01, 1, 0, 0, 2000, 2000, 1, 1, 2, 0, 2048, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 2, 1, 1, 1, 2, 2, 1, 0, 144, 1, 0, 0, 'Stitch_npc_ai_chasseur', 20173);
// REPLACE INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`, `ItemID2`, `ItemID3`, `VerifiedBuild`) VALUES
// (15100008, 1, 1899, 0, 17686, 0),
// (15100008, 2, 1899, 0, 36619, 0),
// (15100008, 3, 1899, 0, 20910, 0);
//###########################################################################################################################################################################################################################################

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellAuraEffects.h"
#include "SpellScript.h"
#include "CreatureTextMgr.h"

//################################################################################################
//StitchAI AI Chasseur
//################################################################################################

class Stitch_npc_ai_chasseur : public CreatureScript
{
public: Stitch_npc_ai_chasseur() : CreatureScript("Stitch_npc_ai_chasseur") { }

		struct Stitch_npc_ai_chasseurAI : public ScriptedAI
		{
			Stitch_npc_ai_chasseurAI(Creature* creature) : ScriptedAI(creature) { }

			uint32 BrancheSpe = 1;													// Choix de la Spécialisation : Survie = 1, Bete = 2 
			uint32 NbrDeSpe = 2;													// Nombre de Spécialisations 
			uint32 ForceBranche;
			uint32 Random;
			uint32 DistanceDeCast = 40;												// Distance max a laquelle un npc attaquera , au dela il quite le combat
			uint32 ResteADistance = 12;												// Distance max a laquelle un npc s'approchera
			uint32 Dist;															// Distance entre le npc et sa cible
			uint32 Mana;
			uint32 MaxMana = 100;
			uint32 Start_Agro = 0;
			uint32 MessageAlagro = 0;
			uint32 Spell_ContreAttaque = 0;
			Unit* victim = me->GetVictim();

			// Definitions des variables Cooldown et le 1er lancement
			uint32 Cooldown_Spell1 = 1000;
			uint32 Cooldown_Spell2 = 2500;
			uint32 Cooldown_Spell3 = 4000;
			uint32 Cooldown_Spell4 = 5500;
			uint32 Cooldown_Spell_Heal = 6000;											// Cooldown pour la fréquence du heal
			uint32 Cooldown_RegenMana = 3000;											// Cooldown pour le regen du mana
			uint32 Cooldown_ResteADistance = 500;										// Test si en contact pour s'eloigner
			uint32 Cooldown_Npc_Emotes = urand(5000, 8000);
			uint32 Cooldown_Spell_ContreAttaque = 4000;
			uint32 Cooldown_Spell_ContreAttaque_defaut = 8000;

			// Spells Divers
			uint32 Buf_all = 31519;														// Aura de précision (60s) 31519
			uint32 Buf_branche1 = 0;
			uint32 Buf_branche1_liste[2] = { 109212, 109212 };							// Engagement spirituel 109212
			uint32 Buf_branche2 = 0;
			uint32 Buf_branche2_liste[2] = { 109260, 109260 };							// Faucon de fer 109260
			uint32 Spell_Heal_Caster = 300247;  										// Piqure du moustique 300247


			// Spells Survie
			uint32 Spell_branche1_agro = 0;
			uint32 Spell_branche1_1 = 0;
			uint32 Spell_branche1_2 = 0;
			uint32 Spell_branche1_3 = 0;
			uint32 Spell_branche1_4 = 0;
			uint32 branche1_agro[5] = { 145663, 145663, 82941, 82941, 145663 };			// Piège explosif a distance 82939, Piège de glace a distance 82941, Marque du chasseur 145663
			uint32 branche1_1[2] = { 171943, 171943 };									// Tir des arcanes 171943 
			uint32 branche1_2[3] = { 300241, 35511, 35511 };							// Tir explosif 300241, Morsure de serpent 35511 15s 
			uint32 branche1_3[3] = { 145654, 145654, 48098 };							// Tir du cobra 145654 , Flèches multiples 48098 3s
			uint32 branche1_4[2] = { 80003, 5116 };										// Flèche noire 80003 18s, Trait de choc 5116 6s

			// Spells Bete
			uint32 Spell_branche2_agro = 0;	//    
			uint32 Spell_branche2_1 = 0;
			uint32 Spell_branche2_2 = 0;
			uint32 Spell_branche2_3 = 0;
			uint32 branche2_agro[6] = { 13813, 13809, 19386, 19577, 19386, 19577 };				// Piège explosif 13813, Piège de glace 13809, Piqûre de wyverne 19386, Intimidation 19577
			uint32 branche2_1[2] = { 171943, 171943 };											// Tir des arcanes 171943 
			uint32 branche2_2[2] = { 80015, 80015 };											// Tir assuré 80015
			uint32 branche2_3[2] = { 19574, 34026 };											// Courroux bestial 19574 60s, Ordre de tuer 34026 30s
			uint32 Pet_Chasseur;
			uint32 Pet_Chasseur_Liste[7] = { 3612, 7488, 7906, 8274, 7909, 32730, 3621 };		// Tigre 3612 (698), Loup 7488 (3862), Lion 7906 (5438), Sanglier 8274 (4535), Gorille 7909 (5442), Ravageur 32730 (19461), Panthere noire 3621 (756)


			// Emotes
			uint32 Npc_Emotes[22] = { 1,3,7,11,15,16,19,21,22,23,24,53,66,71,70,153,254,274,381,401,462,482 };
			

			void Init_AI() 
			{
				// ################################################################################################################################################
				// Forcer le choix de la Spécialisation par creature_template->pickpocketloot
				// ################################################################################################################################################
				ForceBranche = me->GetCreatureTemplate()->pickpocketLootId;											// creature_template->pickpocketloot
				if (ForceBranche == 1) { BrancheSpe = 1; }															// branche1 forcé
				else if (ForceBranche == 2) { BrancheSpe = 2; }														// branche2 forcé 

				else
				{
					// Sinon Choix de la Spécialisation Aléatoire
					BrancheSpe = urand(1, NbrDeSpe + 1);															// Plus de chance d'etre Survie
				}
				if ((BrancheSpe > NbrDeSpe) || (BrancheSpe == 0)) { BrancheSpe = 2; }

				// Reste a distance variable suivant ci le mob est a l'extérieur ou a l'Intérieur
				if (me->GetMap()->IsOutdoors(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()))
				{
					ResteADistance = urand(12, 14);
				}
				else
				{
					ResteADistance = 10;
				}

				// ################################################################################################################################################
				// Tirages aléatoires des spells
				// ################################################################################################################################################

				// Message a l'agro forcé par spell(8)
				if (me->m_spells[7] == 1) { MessageAlagro = 1; }

				// Spell contre attaque si PV bas
				if (me->m_spells[6] != 0) { Spell_ContreAttaque = me->m_spells[6]; }

				switch (BrancheSpe)
				{
				case 1: // Si Branche 1 ---------------------------------------------------------------------------------------------------------------------------
					Buf_branche1 = Buf_branche1_liste[urand(0, 1)];
					me->CastSpell(me, Buf_branche1, true);															// Buf1 sur lui meme

					me->LoadEquipment(1, true);																		// creature_equip_template 1
					Bonus_Armure(125);																				// Bonus d'armure +25%

					Spell_branche1_agro = branche1_agro[urand(0, 4)];
					Spell_branche1_1 = branche1_1[urand(0, 1)];
					Spell_branche1_2 = branche1_2[urand(0, 2)];
					Spell_branche1_3 = branche1_3[urand(0, 2)];
					Spell_branche1_4 = branche1_4[urand(0, 1)];
					break;

				case 2: // Si Si Branche 2 ------------------------------------------------------------------------------------------------------------------------
					Buf_branche2 = Buf_branche2_liste[urand(0, 1)];
					me->CastSpell(me, Buf_branche2, true);															// Buf2 sur lui meme

					me->LoadEquipment(2, true);																		// creature_equip_template 2
					Bonus_Armure(125);																				// Bonus d'armure +25%

					Spell_branche2_agro = branche2_agro[urand(0, 5)];
					Spell_branche2_1 = branche2_1[urand(0, 1)];
					Spell_branche2_2 = branche2_2[urand(0, 1)];
					Spell_branche2_3 = branche2_3[urand(0, 1)];

					// Tirages aléatoires du pet
					Pet_Chasseur = Pet_Chasseur_Liste[urand(0, 6)];
					break;
				}
				// ################################################################################################################################################
				// Divers
				// ################################################################################################################################################
				me->SetSheath(SHEATH_STATE_RANGED);																	// S'equipe de l'arme a distance
				me->LoadEquipment(1, true);													// creature_equip_template 1

				me->SetReactState(REACT_AGGRESSIVE);

				// ################################################################################################################################################
			}

			void JustRespawned() override
			{
				me->GetMotionMaster()->MoveTargetedHome();								// Retour home pour rafraichir client
				//me->SetSpeedRate(MOVE_RUN, 1.01f);
				me->SetReactState(REACT_AGGRESSIVE);
				me->SetSheath(SHEATH_STATE_RANGED);										// S'equipe de l'arme a distance
				
				Init_AI();
				Def_Power();
			}
			void EnterCombat(Unit* /*who*/) override
			{
				Init_AI();
				Def_Power();


				if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == WAYPOINT_MOTION_TYPE)
				{
					me->StopMoving();
					me->GetMotionMaster()->MoveIdle();
				}

			}
			void EnterEvadeMode(EvadeReason /*why*/) override
			{
				RetireBugDeCombat();
				me->AddUnitState(UNIT_STATE_EVADE);
				me->GetMotionMaster()->MoveTargetedHome();								// Retour home
				me->RemoveAllControlled();												// renvois pet
				Def_Power();
				Start_Agro = 0;
				Bonus_Armure(100);														// Retire bonus d'armure
			}
			void JustReachedHome() override
			{
				me->RemoveAura(Buf_branche1);
				me->RemoveAura(Buf_branche2);

				me->SetReactState(REACT_AGGRESSIVE);
				Bonus_Armure(100);														// Retire bonus d'armure
				Def_Power();
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
				if (UpdateVictim())
				{ 
				Mana = me->GetPower(POWER_FOCUS);
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

					me->CastSpell(me, Buf_all, true);																	// Buf_all sur lui meme

					switch (BrancheSpe)
					{
					case 1: // Si Survie ------------------------------------------------------------------------------------------------------------------------------
						me->CastSpell(me, Buf_branche1, true);															// Buf1 sur lui meme
						Random = urand(1, 2);
						if (Random == 1 && UpdateVictim()) { me->CastSpell(victim, Spell_branche1_agro, true); }		// 1/2 Chance de lancer le sort d'agro

						break;
					case 2: // Si Bete --------------------------------------------------------------------------------------------------------------------------------
						me->CastSpell(me, Buf_branche2, true);															// Buf1 sur lui meme
						Random = urand(1, 2);
						if (Random == 1 && UpdateVictim()) { me->CastSpell(victim, Spell_branche2_agro, true); }		// 1/2 Chance de lancer le sort d'agro

						// Tirages aléatoires du pet
						me->CastSpell(me, Pet_Chasseur, true);
						break;
						// ################################################################################################################################################

						DoCastAOE(95826, true);
					}
				}

					// ################################################################################################################################################
					// Spell a lancer a l'agro 
					// ################################################################################################################################################
				
					


				// ################################################################################################################################################
				// Combat suivant la Spécialisation
					switch (BrancheSpe)
				{
				case 1: // Spécialisation Survie ------------------------------------------------------------------------------------------------------------------
						// Regen mana en combat -------------------------------------------------------------------------------------------------------------------
					if (Cooldown_RegenMana <= diff)
					{
						me->SetPower(POWER_FOCUS, Mana + (MaxMana / 2.5));
						if (Mana > MaxMana || Mana == 0) { me->SetPower(POWER_FOCUS, MaxMana); }
						Cooldown_RegenMana = urand(1500, 2000);

						me->SetSheath(SHEATH_STATE_RANGED);																	// S'equipe de l'arme a distance
						me->SetReactState(REACT_AGGRESSIVE);
					}
					else Cooldown_RegenMana -= diff;

					// Combat -------------------------------------------------------------------------------------------------------------------------------------

					if (!me->isInFront(victim, Dist))																		// Si la cible n'est pas de face
						break;

						// Spell4 sur la cible  (Sort secondaire tres lent , généralement utilisé comme Dot) : Flèche noire 3674 18s, Morsure de serpent 31975 15s
					if (Cooldown_Spell4 <= diff)
					{
						me->CastSpell(victim, Spell_branche1_4, true);
						Cooldown_Spell4 = urand(15000, 18000);
					}
					else Cooldown_Spell4 -= diff;

					// Spell3 sur la cible  (Sort secondaire tres lent , généralement utilisé comme Dot) : Tir du cobra 77767, Flèches multiples 2643 3s
					if (Cooldown_Spell3 <= diff)
					{
						me->CastSpell(victim, Spell_branche1_3, true);
						Cooldown_Spell3 = urand(8000, 8000);
					}
					else Cooldown_Spell3 -= diff;

					// Spell2 sur la cible chaque (Sort secondaire plus lent) : Tir explosif 53301 6s, Trait de choc
					if (Cooldown_Spell2 <= diff)
					{
						me->CastSpell(victim, Spell_branche1_2, true);
						Cooldown_Spell2 = urand(6000, 7000);
					}
					else Cooldown_Spell2 -= diff;

					// Spell1 sur la cible chaque (Sort Régulié) : Tir des arcanes
					if (Cooldown_Spell1 <= diff)
					{
						me->CastSpell(victim, Spell_branche1_1, true);
						Cooldown_Spell1 = 2000;
					}
					else Cooldown_Spell1 -= diff;

					break;

				case 2: // Spécialisation Bete --------------------------------------------------------------------------------------------------------------------
						// Regen mana en combat ------------------------------------------------------------------------------------------------------------------------
					if (Cooldown_RegenMana <= diff)
					{
						me->SetPower(POWER_FOCUS, Mana + (MaxMana / 2.5));
						if (Mana > MaxMana) { me->SetPower(POWER_FOCUS, MaxMana); }
						Cooldown_RegenMana = urand(2500, 3000);

						me->SetSheath(SHEATH_STATE_RANGED);																	// S'equipe de l'arme a distance
						me->SetReactState(REACT_AGGRESSIVE);
					}
					else Cooldown_RegenMana -= diff;

					if (!me->isInFront(victim, Dist))																		// Si la cible n'est pas de face
						break;

					// Combat --------------------------------------------------------------------------------------------------------------------------------------
					// Spell1 sur la cible chaque (Sort Régulié) : Tir des arcanes
					if (Cooldown_Spell1 <= diff)
					{
						me->CastSpell(victim, Spell_branche2_1, true);
						Cooldown_Spell1 = 2000;
					}
					else Cooldown_Spell1 -= diff;

					// Spell2 sur la cible chaque (Sort secondaire plus lent) : Tir assuré
					if (Cooldown_Spell2 <= diff)
					{
						me->CastSpell(victim, Spell_branche2_2, true);
						Cooldown_Spell2 = urand(5000, 6000);
					}
					else Cooldown_Spell2 -= diff;

					// Spell3 sur la cible  (Sort secondaire tres lent , généralement utilisé comme Dot) : Courroux bestial 19574 60s, Ordre de tuer 34026 30s
					if (Cooldown_Spell3 <= diff && !victim->HasAura(Spell_branche2_3) )
					{
						me->CastSpell(victim, Spell_branche2_3, true);
						Cooldown_Spell3 = urand(40000, 60000);
					}
					else Cooldown_Spell3 -= diff;

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
				if (me->IsAlive() && !me->IsInCombat() && !me->isMoving() && (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > 1))
				{
					EnterEvadeMode(EVADE_REASON_SEQUENCE_BREAK);
				}

				if (!UpdateVictim())
					return;

				Dist = me->GetDistance(me->GetVictim());
				if ((Dist > DistanceDeCast) || (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > 40))
				{
					RetireBugDeCombat();
					me->AddUnitState(UNIT_STATE_EVADE);
					EnterEvadeMode(EVADE_REASON_SEQUENCE_BREAK);						// Quite le combat si la cible > 30m (Caster & Mélée) ou > 40m de home
				}
			}
			void Mouvement_Caster(uint32 diff)
			{
				if (!UpdateVictim() || AuraFigé()/*|| me->HasUnitState(UNIT_STATE_CASTING)*/)
					return;

				Mana = me->GetPower(POWER_FOCUS);
				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				if (Cooldown_ResteADistance <= diff)
				{
					// Mouvement aléatoire si cible < 6m  ----------------------------------------------------------------------------------------------------------

					if (Dist <6)
					{
						me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);						// UNROOT
						
						if (AuraLenteur() == false && !Interieur())
						{
							me->SetSpeedRate(MOVE_RUN, 1.2f); // Uniquement si non ralenti par un spell 
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
						Cooldown_ResteADistance = 3000;
					}
				}
				else Cooldown_ResteADistance -= diff;

				// Speed normal si distance > 10m ------------------------------------------------------------------------------------------------------------------
				if (Dist> 10)
				{
					if (me->GetSpeedRate(MOVE_RUN) == 1.2f && AuraLenteur() == false)
					{
						me->SetSpeedRate(MOVE_RUN, 1.0f);
					}
				}

				// Mouvement OFF si Mana > 5% & distance >= 11m & <= 15m ---------------------------------------------------------------------------------------------
				if ( (Dist >= ResteADistance - 4) && (Dist <= ResteADistance) )
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
					me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);								// UNROOT
					//me->GetMotionMaster()->MoveChase(victim, ResteADistance);								// Distance de combat
					AttackStartCaster(victim, ResteADistance);												// Distance de cast
					void DoRangedAttackIfReady();															// Combat a distance
					if (me->GetSpeedRate(MOVE_RUN) == 1.2f && AuraLenteur() == false)
					{
						me->SetSpeedRate(MOVE_RUN, 1.0f);
					}
				}


			}


			void Heal_En_Combat_Caster(uint32 diff)
			{
				if (!UpdateVictim() /*|| me->HasUnitState(UNIT_STATE_MOVE)*/)
					return;

				if (Cooldown_Spell_Heal <= diff)
				{
					// heal sur lui meme-------------------------------------------------------------------------------------------------------------------------------
					if ((me->GetHealth() < (me->GetMaxHealth()*0.6)))								// Si PV < 60%
					{
						DoCastVictim(Spell_Heal_Caster);
						Cooldown_Spell_Heal = 6000;
					}
				}
				else Cooldown_Spell_Heal -= diff;

			}
			void Def_Power()
			{
				me->SetMaxPower(POWER_MANA, 100);
				me->setPowerType(POWER_FOCUS);											// Utilise de la focalisation
				me->SetMaxPower(POWER_FOCUS, 100);
				me->SetPower(POWER_FOCUS, 99);
			}
			void ContreAttaque(uint32 diff)
			{
				if (!UpdateVictim())
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
					) return true;
				else return false;
			}
			bool AuraFigé()
			{
				if (me->HasAura(122)		// Nova de givre
					|| me->HasAura(3600)	// Totem de lien terrestre
					|| me->HasAura(6474)	// Totem de lien terrestre passif
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
			return new Stitch_npc_ai_chasseurAI(creature);
		}
};

//##################################################################################################################################################################
void AddSC_npc_ai_chasseur()
{
	new Stitch_npc_ai_chasseur;
}















