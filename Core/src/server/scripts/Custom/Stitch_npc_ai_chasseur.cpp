////#########################################################################################################################################################################################################################################
// Copyright (C) Juin 2020 Stitch pour Aquayoup
// AI generique npc par classe : Chasseur Ver 2020-10-12
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
			uint32 DistanceDeCast = 30;												// Distance max a laquelle un npc attaquera , au dela il quite le combat
			uint32 ResteADistance = 15;												// Distance max a laquelle un npc s'approchera
			uint32 Dist;															// Distance entre le npc et sa cible
			uint32 Mana;
			uint32 MaxMana = 100;

			Unit* victim = me->GetVictim();

			// Definitions des variables Cooldown et le 1er lancement
			uint32 Cooldown_Spell1 = 1000;
			uint32 Cooldown_Spell2 = 2500;
			uint32 Cooldown_Spell3 = 4000;
			uint32 Cooldown_Spell4 = 5500;
			uint32 Cooldown_Spell_Heal = 6000;											// Cooldown pour la fréquence du heal
			uint32 Cooldown_RegenMana = 3000;											// Cooldown pour le regen du mana
			uint32 Cooldown_ResteADistance = 3000;										// Test si en contact pour s'eloigner
			uint32 Cooldown_Npc_Emotes = urand(5000, 8000);

			// Spells Divers
			uint32 Buf_all = 31519;														// Aura de précision (60s) 31519
			uint32 Buf_branche1;
			uint32 Buf_branche1_liste[2] = { 109212, 109212 };							// Engagement spirituel 109212
			uint32 Buf_branche2;
			uint32 Buf_branche2_liste[2] = { 109260, 109260 };							// Faucon de fer 109260
			uint32 Spell_Heal_Caster = 19801;  											// Piqure du moustique 19801


																						// Spells Survie
			uint32 Spell_branche1_agro;
			uint32 Spell_branche1_1;
			uint32 Spell_branche1_2;
			uint32 Spell_branche1_3;
			uint32 Spell_branche1_4;
			uint32 branche1_agro[5] = { 82939, 82939, 82941, 82941, 145663 };			// Piège explosif a distance 82939, Piège de glace a distance 82941, Marque du chasseur 145663
			uint32 branche1_1[2] = { 3044, 3044 };										// Tir des arcanes 3044 
			uint32 branche1_2[3] = { 53301, 5116, 5116 };								// Tir explosif 53301 6s, Trait de choc 5116 6s
			uint32 branche1_3[3] = { 77767, 77767, 2643 };								// Tir du cobra 77767 , Flèches multiples 2643 3s
			uint32 branche1_4[2] = { 3674, 31975 };										// Flèche noire 3674 18s, Morsure de serpent 31975 15s 

																						// Spells Bete
			uint32 Spell_branche2_agro;	//    
			uint32 Spell_branche2_1;
			uint32 Spell_branche2_2;
			uint32 Spell_branche2_3;
			uint32 branche2_agro[6] = { 13813, 13809, 19386, 19577, 19386, 19577 };				// Piège explosif 13813, Piège de glace 13809, Piqûre de wyverne 19386, Intimidation 19577
			uint32 branche2_1[2] = { 3044, 3044 };												// Tir des arcanes 3044 
			uint32 branche2_2[2] = { 56641, 56641 };											// Tir assuré 56641
			uint32 branche2_3[2] = { 19574, 34026 };											// Courroux bestial 19574 60s, Ordre de tuer 34026 30s
			uint32 Pet_Chasseur;
			uint32 Pet_Chasseur_Liste[7] = { 3612, 7488, 7906, 8274, 7909, 32730, 3621 };		// Tigre 3612, Loup 7488, Lion 7906, Sanglier 8274, Gorille 7909, Ravageur 32730, Pantere noire 3621


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


				// ################################################################################################################################################
				// Tirages aléatoires des spells
				// ################################################################################################################################################
				switch (BrancheSpe)
				{
				case 1: // Si Branche 1 ---------------------------------------------------------------------------------------------------------------------------
					Buf_branche1 = Buf_branche1_liste[urand(0, 1)];
					me->CastSpell(me, Buf_branche1, true);															// Buf1 sur lui meme

					me->LoadEquipment(1, true);																		// creature_equip_template 1

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

					uint32 Cooldown_Spell3 = 30000;

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
				me->SetReactState(REACT_AGGRESSIVE);
				// ################################################################################################################################################
			}

			void JustRespawned() override
			{
				me->GetMotionMaster()->MoveTargetedHome();								// Retour home pour rafraichir client
				me->SetSpeedRate(MOVE_RUN, 1.01f);
				me->SetReactState(REACT_AGGRESSIVE);
				me->SetSheath(SHEATH_STATE_RANGED);										// S'equipe de l'arme a distance
				
				Init_AI();
				Def_Power();
			}
			void EnterCombat(Unit* /*who*/) override
			{
				Init_AI();
				Def_Power();

				// ################################################################################################################################################
				// Spell a lancer a l'agro 
				// ################################################################################################################################################
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

				}

			}
			void EnterEvadeMode(EvadeReason /*why*/) override
			{
				RetireBugDeCombat();
				me->AddUnitState(UNIT_STATE_EVADE);
				me->SetSpeedRate(MOVE_RUN, 1.5f);										// Vitesse de déplacement
				me->GetMotionMaster()->MoveTargetedHome();								// Retour home
				me->RemoveAllControlled();												// renvois pet
				Def_Power();
			}
			void JustReachedHome() override
			{
				me->RemoveAura(Buf_branche1);
				me->RemoveAura(Buf_branche2);

				me->SetReactState(REACT_AGGRESSIVE);
				me->SetSpeedRate(MOVE_RUN, 1.01f);										// Vitesse par defaut définit a 1.01f puisque le patch modification par type,famille test si 1.0f
				Def_Power();
			}
			void UpdateAI(uint32 diff) override
			{
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
				if (UpdateVictim() /*|| me->isPossessed() || me->IsCharmed() || me->HasAuraType(SPELL_AURA_MOD_FEAR)*/)
				{ 
				Mana = me->GetPower(POWER_FOCUS);
				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);
				// if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC)) { me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC); }
				// if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC)) { me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC); }
				// if (!me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT)) { me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT); }

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

						// Spell4 sur la cible  (Sort secondaire tres lent , généralement utilisé comme Dot) : Flèche noire 3674 18s, Morsure de serpent 31975 15s
					if (Cooldown_Spell4 <= diff)
					{
						DoCastAOE(Spell_branche1_4, true);
						Cooldown_Spell4 = urand(15000, 18000);
					}
					else Cooldown_Spell4 -= diff;

					// Spell3 sur la cible  (Sort secondaire tres lent , généralement utilisé comme Dot) : Tir du cobra 77767, Flèches multiples 2643 3s
					if (Cooldown_Spell3 <= diff)
					{
						DoCastVictim(Spell_branche1_3);
						Cooldown_Spell3 = urand(8000, 8000);
					}
					else Cooldown_Spell3 -= diff;

					// Spell2 sur la cible chaque (Sort secondaire plus lent) : Tir explosif 53301 6s, Trait de choc
					if (Cooldown_Spell2 <= diff /*&& !me->HasUnitState(UNIT_STATE_CASTING)*/)
					{
						DoSpellAttackIfReady(Spell_branche1_2);
						Cooldown_Spell2 = urand(6000, 7000);
					}
					else Cooldown_Spell2 -= diff;

					// Spell1 sur la cible chaque (Sort Régulié) : Tir des arcanes
					if (Cooldown_Spell1 <= diff)
					{
						DoSpellAttackIfReady(Spell_branche1_1);
						Cooldown_Spell1 = 1500;
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

					// Combat --------------------------------------------------------------------------------------------------------------------------------------
					// Spell1 sur la cible chaque (Sort Régulié) : Tir des arcanes
					if (Cooldown_Spell1 <= diff)
					{
						DoCastVictim(Spell_branche2_1);
						Cooldown_Spell1 = 2000;
					}
					else Cooldown_Spell1 -= diff;

					// Spell2 sur la cible chaque (Sort secondaire plus lent) : Tir assuré
					if (Cooldown_Spell2 <= diff)
					{
						DoCastVictim(Spell_branche2_2);
						Cooldown_Spell2 = urand(5000, 6000);
					}
					else Cooldown_Spell2 -= diff;

					// Spell3 sur la cible  (Sort secondaire tres lent , généralement utilisé comme Dot) : Courroux bestial 19574 60s, Ordre de tuer 34026 30s
					if (Cooldown_Spell3 <= diff)
					{
						DoCastVictim(Spell_branche2_3);
						Cooldown_Spell3 = urand(45000, 45000);
					}
					else Cooldown_Spell3 -= diff;

					break;
				}

				// ################################################################################################################################################
				Heal_En_Combat_Caster(diff);
				Mouvement_Caster(diff);
				Mouvement_All();
				}
				// ################################################################################################################################################
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
				if (!UpdateVictim() /*|| me->HasUnitState(UNIT_STATE_CASTING)*/)
					return;

				Mana = me->GetPower(POWER_FOCUS);
				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				if (Cooldown_ResteADistance <= diff)
				{
					// Mouvement aléatoire si cible < 6m & Mana > 5% --------------------------------------------------------------------------------------------------

					if ((Dist <6) && (Mana > MaxMana / 20))
					{
						me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);						// UNROOT
						me->SetSpeedRate(MOVE_RUN, 1.1f);

						float x, y, z;
						x = (me->GetPositionX() + urand(0, ResteADistance * 2) - ResteADistance);
						y = (me->GetPositionY() + urand(0, ResteADistance * 2) - ResteADistance);
						z = me->GetPositionZ();
						me->GetMotionMaster()->MovePoint(0xFFFFFE, x, y, z);
						Cooldown_ResteADistance = urand(5000, 8000);
					}
				}
				else Cooldown_ResteADistance -= diff;

				// Speed normal si distance > 10m ------------------------------------------------------------------------------------------------------------------
				if (Dist> 10 && me->GetSpeedRate(MOVE_RUN) == 1.1f)
				{
					me->SetSpeedRate(MOVE_RUN, 1.01f);
				}

				// Mouvement OFF si Mana > 5% & distance >= 10m & <= 15m ---------------------------------------------------------------------------------------------
				if ((Mana > MaxMana / 20) && (Dist >= ResteADistance - 5) && (Dist <= ResteADistance))
				{
					me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);							// ROOT
					AttackStartCaster(victim, ResteADistance);										// Distance de combat
					void DoRangedAttackIfReady();													// Combat a distance
				}

				// Mouvement ON si distance > 15m ------------------------------------------------------------------------------------------------------------------
				if (Dist > ResteADistance)
				{
					me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);						// UNROOT
					//AttackStart(victim);															// Combat au corp a corp
					AttackStartCaster(victim, ResteADistance);									// Distance de cast
					void DoRangedAttackIfReady();													// Combat a distance
					//DoMeleeAttackIfReady();															// Combat en mélée
				}

				// Mouvement ON si Mana < 5%  ----------------------------------------------------------------------------------------------------------------------
				if (Mana < MaxMana / 20)
				{
					me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);							// UNROOT
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















