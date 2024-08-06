////#########################################################################################################################################################################################################################################
// Copyright (C) Juin 2020 Stitch pour Aquayoup
// AI generique npc par classe : PRETRE Ver 2022-07-31
// Il est possible d'influencer le temp entre 2 cast avec `BaseAttackTime` & `RangeAttackTime` 
// Necessite dans Creature_Template :
// Minimun  : UPDATE `creature_template` SET `ScriptName` = 'Stitch_npc_ai_pretre',`AIName` = '' WHERE (entry = 15100003);
// Optionel : UPDATE `creature_template` SET `HealthModifier` = 2, `ManaModifier` = 3, `ArmorModifier` = 1, `DamageModifier` = 2,`BaseAttackTime` = 2000, `RangeAttackTime` = 2000 WHERE(entry = 15100003);
// Optionel : Utilisez pickpocketloot de creature_template pour passer certains parametres (Solution choisit afin de rester compatible avec tout les cores). Si pickpocketloot = 1 (branche1 forcé), pickpocketloot = 2 (branche2 forcé), etc
//###########################################################################################################################################################################################################################################
// # npc de Test Stitch_npc_ai_pretre  .npc 15100003
// REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `HealthModifier`, `HealthModifierExtra`, `ManaModifier`, `ManaModifierExtra`, `ArmorModifier`, `DamageModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES
// (15100003, 0, 0, 0, 0, 0, 7669, 0, 0, 0, 'npc_ai_Pretre', '', '', '', 0, 90, 90, 0, 0, 2102, 0, 1.01, 1.01, 0.7, 0, 0, 2000, 2000, 1, 1, 2, 0, 2048, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 2, 1, 3, 1, 2, 2, 1, 0, 144, 1, 0, 0, 'Stitch_npc_ai_pretre', 20173);
// REPLACE INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`, `ItemID2`, `ItemID3`, `VerifiedBuild`) VALUES
// (15100003, 1, 15979, 0, 0, 0),
// (15100003, 2, 15979, 0, 0, 0),
// (15100003, 3, 15979, 0, 0, 0);
//###########################################################################################################################################################################################################################################

#include "CreatureTextMgr.h"

//################################################################################################
//StitchAI AI Pretre
//################################################################################################

class Stitch_npc_ai_pretre : public CreatureScript
{
public: Stitch_npc_ai_pretre() : CreatureScript("Stitch_npc_ai_pretre") { }

		struct Stitch_npc_ai_pretreAI : public ScriptedAI
		{
			Stitch_npc_ai_pretreAI(Creature* creature) : ScriptedAI(creature) { }

			uint32 BrancheSpe = 1;													// Choix de la Spécialisation : Ombre=1, Discipline=2
			uint32 NbrDeSpe = 2;													// Nombre de Spécialisations
			uint32 ForceBranche;
			uint32 Random;
			uint32 DistanceDeCast = 40;												// Distance max a laquelle un npc attaquera , au dela il quite le combat
			uint32 ResteADistance = 10;												// Distance max a laquelle un npc s'approchera
			uint32 Dist;															// Distance entre le npc et sa cible
			uint32 Mana;
			uint32 MaxMana = me->GetMaxPower(POWER_MANA);
			uint32 MessageAlagro = 0;
			uint32 Spell_ContreAttaque = 0;
			Unit* victim = me->GetVictim();

			// Definitions des variables Cooldown et le 1er lancement
			uint32 Cooldown_Spell1 = 2500;
			uint32 Cooldown_Spell2 = 1500;
			uint32 Cooldown_Spell3 = 500;
			uint32 Cooldown_Spell_Heal = 3000;										// Cooldown pour la fréquence du heal
			uint32 Cooldown_RegenMana = 3000;										// Cooldown pour le regen du mana
			uint32 Cooldown_ResteADistance = 1000;									// Test si en contact pour s'eloigner
			uint32 Cooldown_Spell_Bouclier = 6000;									// Cooldown pour mot de pouvoir : bouclier
			uint32 Cooldown_Npc_Emotes = urand(5000, 8000);
			uint32 Cooldown_Spell_ContreAttaque = 4000;
			uint32 Cooldown_Spell_ContreAttaque_defaut = 8000;

			// Spells Divers
			uint32 Buf_all = 21562;													// Mot de pouvoir : Robustesse 21562
			uint32 Buf_branche1 = 15473;											// Forme d'Ombre 15473
			uint32 Buf_branche2 = 81700;											// Archange 81700 (soin +20% 15s)
			uint32 Spell_Heal_Caster = 2061;  										// Soins Rapides (light) 2061
			uint32 Spell_Heal_Heal = 139;  											// Rénovation 139 (rend pv a l'instant + 12s)
			uint32 Etreinte_Vampirique = 15286;										// Etreinte vampirique
			uint32 Nova_Sacree = 132157;											// Nova sacrée dmg/heal 12m
			uint32 Mot_de_pouvoir_Bouclier = 17;									// Mot de pouvoir : Bouclier
			uint32 Mot_de_pouvoir_Bouclier_effet = 11835;
			uint32 Soins_Rapides = 300265;											// Soins Rapides 300265/2061
			uint32 Toucher_Vampirique = 300231;
			uint32 Peste_devorante = 138490;
			uint32 Mot_de_lombre_Douleur = 34941;

			// Spells Ombre
			uint32 Spell_branche1_agro = 0;
			uint32 Spell_branche1_1 = 0;
			uint32 Spell_branche1_2 = 0;
			uint32 Spell_branche1_3 = 0;
			uint32 branche1_agro[4] = { 15487, 15487, 172884, 64044 };				// Ombrefiel 172884, Horreur psychique 64044 (fear 5s), Silence 15487 5s
			uint32 branche1_1[4] = { 183324, 183324, 183324, 60440 };				// Fouet mental 183324, Incandescence mentale 60440
			uint32 branche1_2[2] = { 92713, 145550 };								// Attaque mentale 92713, Pointe mentale 145550
			uint32 branche1_3[3] = { 300231, 138490, 34941 };						// Toucher vampirique 300231,  Peste dévorante 138490, Mot de l’ombre:Douleur 34941

			// Spells Discipline
			uint32 Spell_branche2_agro = 0;	//    
			uint32 Spell_branche2_1 = 0;
			uint32 Spell_branche2_2 = 0;
			uint32 Spell_branche2_3 = 0;
			uint32 branche2_agro[6] = { 15487, 15487, 15487, 88625, 88625, 64044 };	// Horreur psychique 64044 (fear 5s), Silence 15487 5s , Mot sacré : Châtier 88625 (stun 3s) , 
			uint32 branche2_1[2] = { 168380, 168380 };								// Châtiment 168380  
			uint32 branche2_2[3] = { 18165, 18165 ,165721 };						// Flammes sacrées 18165, Pénitence 165721
			uint32 branche2_3[3] = { 34941, 34941, 34941 };							// Mot de l’ombre:Douleur 34941

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
				me->SetSheath(SHEATH_STATE_UNARMED);								//Arme rangée
			}

			void Init_AI()
			{

				// Message a l'agro forcé par spell(8)
				if (me->m_spells[7] == 1) { MessageAlagro = 1; }

				// Spell contre attaque si PV bas
				if (me->m_spells[6] != 0) { Spell_ContreAttaque = me->m_spells[6]; }

				// ################################################################################################################################################
				// Forcer le choix de la Spécialisation par creature_template->pickpocketloot
				// ################################################################################################################################################
				// Forcer le choix de la Spécialisation par creature_template->pickpocketloot
				ForceBranche = me->GetCreatureTemplate()->pickpocketLootId;							// creature_template->pickpocketloot
				if (ForceBranche == 1) { BrancheSpe = 1; }											// branche1 forcé
				else if (ForceBranche == 2) { BrancheSpe = 2; }										// branche2 forcé 
				else
				{
					// Sinon Choix de la Spécialisation Aléatoire
					BrancheSpe = urand(1, NbrDeSpe + 1);
				}

				if ((BrancheSpe > NbrDeSpe) || (BrancheSpe == 0)) { BrancheSpe = 2; }

				// ################################################################################################################################################
				// Tirages aléatoires des spells
				// ################################################################################################################################################
				// Spell a lancer a l'agro ------------------------------------------------------------------------------------------------------------------------

				me->CastSpell(me, Buf_all, true);																	// Buf_all sur lui meme

				// Reste a distance variable suivant ci le mob est a l'extérieur ou a l'Intérieur 
				if (me->GetMap()->IsOutdoors(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()))
				{
					ResteADistance = urand(10, 13);
				}
				else
				{
					ResteADistance = 10;
				}

				switch (BrancheSpe)
				{
				case 1: // Si Ombre -------------------------------------------------------------------------------------------------------------------------------
					me->CastSpell(me, Buf_branche1, true);															// Buf1 sur lui meme
										
					me->LoadEquipment(1, true);																		// creature_equip_template 1

					// Tirages aléatoires des spells Ombre
					Spell_branche1_agro = branche1_agro[urand(0, 3)];
					Spell_branche1_1 = branche1_1[urand(0, 3)];
					Spell_branche1_2 = branche1_2[urand(0, 1)];
					Spell_branche1_3 = branche1_3[urand(0, 2)];
					break;

				case 2: // Si Discipline --------------------------------------------------------------------------------------------------------------------------
					me->CastSpell(me, Buf_branche2, true);															// Buf2 sur lui meme

					me->LoadEquipment(2, true);																		// creature_equip_template 2

					// Tirages aléatoires des spells Discipline 
					Spell_branche2_agro = branche2_agro[urand(0, 5)];
					Spell_branche2_1 = branche2_1[urand(0, 1)];
					Spell_branche2_2 = branche2_2[urand(0, 2)];
					Spell_branche2_3 = branche2_3[urand(0, 2)];
					break;

				}
				// ################################################################################################################################################
				// Divers
				// ################################################################################################################################################
				me->SetSheath(SHEATH_STATE_MELEE);																	// S'equipe de l'arme au contact
				me->SetReactState(REACT_AGGRESSIVE);
				// ################################################################################################################################################
			}

			void JustRespawned() override
			{
				me->GetMotionMaster()->MoveTargetedHome();								// Retour home pour rafraichir client
				//me->SetSpeedRate(MOVE_RUN, 1.01f);
				me->SetReactState(REACT_AGGRESSIVE);

				Init_AI();
				me->SetSheath(SHEATH_STATE_UNARMED);								//Arme rangée
			}
			void EnterCombat(Unit* /*who*/) override
			{
				ResteADistance = 12 + urand(0, 5);
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
				me->RemoveAura(104015);	// Retire afaissé / Stun
				me->RemoveAura(153964);	// Retire agenouillé, avec evade
				me->RemoveAura(42648);	// Retire Dort allongé + zzz
				me->RemoveAura(18795);	// Retire Dort allongé + zzz 
				me->RemoveAura(43905);	// Retire Ivre
				me->HandleEmoteCommand(0);
				me->SetByteValue(UNIT_FIELD_BYTES_1, 0, 0);
				me->SetByteValue(UNIT_FIELD_BYTES_2, 0, 0);

			}
			void EnterEvadeMode(EvadeReason /*why*/) override
			{
				Start_Agro = 0;
				RetireBugDeCombat();
				me->AddUnitState(UNIT_STATE_EVADE);
				//me->SetSpeedRate(MOVE_RUN, 1.5f);										// Vitesse de déplacement
				me->GetMotionMaster()->MoveTargetedHome();								// Retour home
				me->RemoveAllControlled();												// renvois pet
			}
			void JustReachedHome() override
			{
				me->RemoveAura(Buf_branche1);
				me->RemoveAura(Buf_branche2);
				me->RemoveAura(Buf_all);

				me->SetSheath(SHEATH_STATE_UNARMED);								//Arme rangée
				me->SetReactState(REACT_AGGRESSIVE);
				//me->SetSpeedRate(MOVE_RUN, 1.01f);				
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
						case 1: // Si Ombre -------------------------------------------------------------------------------------------------------------------------------
							me->CastSpell(me, Buf_all, true);																// Buf_all sur lui meme
							me->CastSpell(me, Buf_branche1, true);															// Buf1 sur lui meme
							
							Random = urand(1, 2);
							if (Random == 1) { me->CastSpell(victim, Spell_branche1_agro, true); }							// 1/2 Chance de lancer le sort d'agro
							break;

						case 2: // Si Discipline --------------------------------------------------------------------------------------------------------------------------
							me->CastSpell(me, Buf_all, true);																// Buf_all sur lui meme
							me->CastSpell(me, Buf_branche2, true);															// Buf2 sur lui meme
							
							Random = urand(1, 2);
							if (Random == 1) { me->CastSpell(victim, Spell_branche2_agro, true); }							// 1/2 Chance de lancer le sort d'agro
							break;

							// ############################################################################################################################################
						}
					}

					// ####################################################################################################################################################
					// Combat suivant la Spécialisation
					switch (BrancheSpe)
					{
					case 1: // Spécialisation Ombre #######################################################################################################################
							// Regen mana en combat -----------------------------------------------------------------------------------------------------------------------
						if (Cooldown_RegenMana <= diff)
						{
							me->SetPower(POWER_MANA, Mana + (MaxMana / 10));
							if (Mana > MaxMana) { me->SetPower(POWER_MANA, MaxMana); }
							Cooldown_RegenMana = urand(3000, 3500);
						}
						else Cooldown_RegenMana -= diff;

						Heal_En_Combat_Caster(diff);
						Mouvement_Caster(diff);

						// Combat -----------------------------------------------------------------------------------------------------------------------------------------

						// Spell3 sur la cible  (Sort secondaire tres lent , généralement utilisé comme Dot)
						if (Cooldown_Spell3 <= diff && (!victim->HasAura(Spell_branche1_3) && Spell_branche1_3 != Toucher_Vampirique && Spell_branche1_3 != Peste_devorante && Spell_branche1_3 != Mot_de_lombre_Douleur))
						{
							//DoCastVictim(Spell_branche1_3);
							me->CastSpell(victim, Spell_branche1_3, true);
							Cooldown_Spell3 = urand(8000, 10000);
						}
						else Cooldown_Spell3 -= diff;

						// Spell2 sur la cible chaque (Sort secondaire plus lent)
						if (Cooldown_Spell2 <= diff && !me->HasUnitState(UNIT_STATE_CASTING))
						{
							//DoCastVictim(Spell_branche1_2);
							me->CastSpell(victim, Spell_branche1_2, false);
							Cooldown_Spell2 = 5000;
						}
						else Cooldown_Spell2 -= diff;

						// Spell1 sur la cible chaque (Sort Régulié)
						if (Cooldown_Spell1 <= diff)
						{
							if (!me->HasUnitState(UNIT_STATE_MOVE) )
							{
								//me->StopMoving();
								DoCastVictim(Spell_branche1_1);
								//me->CastSpell(victim, Spell_branche1_1, false);
								Cooldown_Spell1 = 3500;
							}
						}
						else Cooldown_Spell1 -= diff;

						break;

					case 2: // Spécialisation Discipline ##################################################################################################################
							// Regen mana en combat -----------------------------------------------------------------------------------------------------------------------
						if (Cooldown_RegenMana <= diff)
						{
							me->SetPower(POWER_MANA, Mana + (MaxMana / 10));
							if (Mana > MaxMana) { me->SetPower(POWER_MANA, MaxMana); }
							Cooldown_RegenMana = urand(2000, 2500);
						}
						else Cooldown_RegenMana -= diff;

						Heal_En_Combat_Heal(diff);
						
						// Combat -----------------------------------------------------------------------------------------------------------------------------------------

						// Spell1 sur la cible chaque (Sort Régulié)
						if (Cooldown_Spell1 <= diff && !me->HasUnitState(UNIT_STATE_CASTING) )
						{
							DoCastVictim(Spell_branche2_1);
							Cooldown_Spell1 = 3500;
						}
						else Cooldown_Spell1 -= diff;


						// Spell2 sur la cible chaque (Sort secondaire plus lent)
						if (Cooldown_Spell2 <= diff /*&& !me->HasUnitState(UNIT_STATE_CASTING)*/)
						{
							DoCastVictim(Spell_branche2_2);
							Cooldown_Spell2 = 8000;
						}
						else Cooldown_Spell2 -= diff;


						// Spell3 sur la cible  (Sort secondaire tres lent , généralement utilisé comme Dot)
						if (Cooldown_Spell3 <= diff && (!victim->HasAura(Spell_branche2_3) && Spell_branche2_3 != Mot_de_lombre_Douleur))
						{
							DoCastVictim(Spell_branche2_3);
							Cooldown_Spell3 = urand(8000, 10000);
						}
						else Cooldown_Spell3 -= diff;

						Mouvement_Caster(diff);
						break;
					}

					// ####################################################################################################################################################
					ContreAttaque(diff);
					
				}
				// ########################################################################################################################################################
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
					EnterEvadeMode(EVADE_REASON_SEQUENCE_BREAK);						// Quite le combat si la cible > 30m (Caster & Mélée) ou > 40m de home
				}
			}
			void Mouvement_Caster(uint32 diff)
			{
				if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING) || me->HasAura(Mot_de_pouvoir_Bouclier_effet) || AuraFigé())
					return;
				
				Mana = me->GetPower(POWER_MANA);
				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);


				if (Cooldown_ResteADistance <= diff)
				{
					// Mouvement aléatoire si cible < 6m & Mana > 5% --------------------------------------------------------------------------------------------------

					if ((Dist <6) && (Mana > MaxMana / 20) )
					{
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
						Cooldown_ResteADistance = 2000;
						Cooldown_Spell1 = 1500;															// Pour ne pas avoir de temp mort apres un eloignement
					}
				}
				else Cooldown_ResteADistance -= diff;

				// Speed normal si distance > 10m ------------------------------------------------------------------------------------------------------------------
				if (AuraLenteur() == false && Dist> 10 && me->GetSpeedRate(MOVE_RUN) == 1.2f)
				{
					me->SetSpeedRate(MOVE_RUN, 1.0f); // Uniquement si non ralenti par un spell joueur
				}

				// Mouvement OFF si Mana > 5% & distance >= 6m & <= 10/15m ---------------------------------------------------------------------------------------------
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

				// mot de pouvoir:bouclier sur lui meme ----------------------------------------------------------------------------------------------------------------
				if (Cooldown_Spell_Bouclier <= diff)
				{
					if (me->GetHealth() < (me->GetMaxHealth()*0.40))									// Si PV < 40%
					{
						me->CastSpell(me, Mot_de_pouvoir_Bouclier, true);								// Utilise Mot de pouvoir : Bouclier chaque 90s 
						Cooldown_Spell_Bouclier = 90000;
					}
				}
				else Cooldown_Spell_Bouclier -= diff;
			}
			void Heal_En_Combat_Heal(uint32 diff)
			{
				if (!UpdateVictim())
					return;

				// mot de pouvoir:bouclier sur lui meme ----------------------------------------------------------------------------------------------------------------
				if (Cooldown_Spell_Bouclier <= diff)
				{
					if (me->GetHealth() < (me->GetMaxHealth()*0.40))									// Si PV < 40%
					{
						me->CastSpell(me, Mot_de_pouvoir_Bouclier, true);								// Utilise Mot de pouvoir : Bouclier chaque 90s 
						Cooldown_Spell_Bouclier = 90000;
					}
				}
				else Cooldown_Spell_Bouclier -= diff;



				// Heal sur lui & Friend
				if (Cooldown_Spell_Heal <= diff)
				{
					Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, DistanceDeCast, true);								// pour heal friend

					// heal sur lui meme ------------------------------------------------------------------------------------------------------------------------------
					if (me->GetHealth() < (me->GetMaxHealth()*0.70) && !me->HasAura(Spell_Heal_Heal))						// Si PV < 70%
					{
						me->CastSpell(me, Spell_Heal_Heal, true);															// Rénovation
						Cooldown_Spell_Heal = 3000;
					}
					else if (me->GetHealth() < (me->GetMaxHealth()*0.50) && !me->HasUnitState(UNIT_STATE_MOVE))				// Si PV < 50%
					{
						DoCast(me, Soins_Rapides);																			// Soins rapides 300265 
						Cooldown_Spell_Heal = 3000;
					}
					// heal sur Friend ---------------------------------------------------------------------------------------------------------------------------------
					else if (target = DoSelectLowestHpFriendly(DistanceDeCast) )												// Distance de l'allié = 30m
					{
						if (me->IsFriendlyTo(target) && (me != target))
						{
							if (target->GetHealth() < (target->GetMaxHealth()*0.60) && !target->HasAura(Spell_Heal_Heal))		// Si PV < 60%
							{
								me->CastSpell(target, Spell_Heal_Heal, true);													// Rénovation
								Cooldown_Spell_Heal = 4000;
							}
							else if (target->GetHealth() < (target->GetMaxHealth()*0.40) && !me->HasUnitState(UNIT_STATE_MOVE))	// Si PV < 40%
							{
								DoCast(target, Soins_Rapides);																	// Soins rapides 2061 
								Cooldown_Spell_Heal = 4000;
							}
						}

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
			return new Stitch_npc_ai_pretreAI(creature);
		}
};

//#########################################################################################################################################################################
void AddSC_npc_ai_pretre()
{
	new Stitch_npc_ai_pretre;
}















