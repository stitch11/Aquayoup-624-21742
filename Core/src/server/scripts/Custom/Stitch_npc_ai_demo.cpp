////#########################################################################################################################################################################################################################################
// Copyright (C) Juin 2020 Stitch pour Aquayoup
// AI generique npc par classe : DEMO Ver 2020-10-24
// Il est possible d'influencer le temp entre 2 cast avec `BaseAttackTime` & `RangeAttackTime` 
// Necessite dans Creature_Template :
// Minimun  : UPDATE `creature_template` SET `ScriptName` = 'Stitch_npc_ai_demo',`AIName` = '' WHERE (entry = 15100004);
// Optionel : UPDATE `creature_template` SET `HealthModifier` = 2, `ManaModifier` = 3, `ArmorModifier` = 1, `DamageModifier` = 2,`BaseAttackTime` = 2000, `RangeAttackTime` = 2000 WHERE(entry = 15100004);
// Optionel : Utilisez pickpocketloot de creature_template pour passer certains parametres (Solution choisit afin de rester compatible avec tout les cores). Si pickpocketloot = 1 (branche1 forcé), pickpocketloot = 2 (branche2 forcé), etc
//###########################################################################################################################################################################################################################################
// # npc de Test Stitch_npc_ai_demo  .npc 15100004
// REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `HealthModifier`, `HealthModifierExtra`, `ManaModifier`, `ManaModifierExtra`, `ArmorModifier`, `DamageModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES
// (15100004, 0, 0, 0, 0, 0, 7669, 0, 0, 0, 'npc_ai_Pretre', '', '', '', 0, 90, 90, 0, 0, 2102, 0, 1.01, 1.01, 0.7, 0, 0, 2000, 2000, 1, 1, 2, 0, 2048, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 2, 1, 3, 1, 2, 2, 1, 0, 144, 1, 0, 0, 'Stitch_npc_ai_demo', 20173);
// REPLACE INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`, `ItemID2`, `ItemID3`, `VerifiedBuild`) VALUES
// (15100004, 1, 15979, 0, 0, 0),
// (15100004, 2, 15979, 0, 0, 0),
// (15100004, 3, 15979, 0, 0, 0);
//###########################################################################################################################################################################################################################################



//################################################################################################
//StitchAI AI Demo
//################################################################################################

class Stitch_npc_ai_demo : public CreatureScript
{
public: Stitch_npc_ai_demo() : CreatureScript("Stitch_npc_ai_demo") { }

		struct Stitch_npc_ai_demoAI : public ScriptedAI
		{
			Stitch_npc_ai_demoAI(Creature* creature) : ScriptedAI(creature) { }

			uint32 BrancheSpe = 1;													// Choix de la Spécialisation : Affliction = 1, Demonologie = 2 , Destruction = 3
			uint32 NbrDeSpe = 3;													// Nombre de Spécialisations 
			uint32 ForceBranche;
			uint32 Random;
			uint32 DistanceDeCast = 30;												// Distance max a laquelle un npc attaquera , au dela il quite le combat
			uint32 ResteADistance = 15;												// Distance a laquelle un npc cast : ROOT +-5m
			uint32 Dist;															// Distance entre le npc et sa cible 
			uint32 Mana;
			uint32 MaxMana = me->GetMaxPower(POWER_MANA);

			Unit* victim = me->GetVictim();

			// Definitions des variables Cooldown et le 1er lancement
			uint32 Cooldown_Spell1 = 1500;
			uint32 Cooldown_Spell2 = 1000;
			uint32 Cooldown_Spell3 = 500;
			uint32 Cooldown_Spell4 = 750;
			uint32 Cooldown_Spell_Heal = 3000;											// Cooldown pour la fréquence du heal
			uint32 Cooldown_RegenMana = 3000;											// Cooldown pour le regen du mana
			uint32 Cooldown_ResteADistance = 2000;										// Test si en contact pour s'eloigner
			uint32 Cooldown_Npc_Emotes = urand(5000, 8000);

			// Spells Divers
			uint32 Buf_all = 79954;														// Armure démoniaque 79934 , Gangrarmure 79954 
			uint32 Buf_branche1;	
			uint32 Buf_branche1_liste[2] = { 104773, 104773 };							// Résolution interminable 104773 degat -40% 8s , Rune de puissance 116011 PS +40% 10s
			uint32 Buf_branche2 ;
			uint32 Buf_branche2_liste[2] = { 104773, 104773 };							// Résolution interminable 104773 degat -40% 8s , Rune de puissance 116011 PS +40% 10s
			uint32 Buf_branche3 ;
			uint32 Buf_branche3_liste[2] = { 104773, 104773 };							// Résolution interminable 104773 degat -40% 8s , Rune de puissance 116011 PS +40% 10s 
			uint32 Spell_Heal_Caster = 689;  											// Drain de vie 689
			uint32 Armure_Demoniaque = 79934;											// Armure_Demoniaque
			uint32 Gangrarmure = 79954;													// Gangrarmure

			// Spells Affliction
			uint32 Spell_branche1_agro;
			uint32 Spell_branche1_1;
			uint32 Spell_branche1_2;
			uint32 Spell_branche1_3;
			uint32 Spell_branche1_4;
			uint32 branche1_agro[2] = { 30108, 30108 };									// Affliction instable 30108
			uint32 branche1_1[2] = { 686, 686 };										// Trait de l'ombre 686
			uint32 branche1_2[2] = { 980, 980 };										// Agonie 980 
			uint32 branche1_3[2] = { 172, 172 };										// corruption 172
			uint32 branche1_4[2] = { 48181, 48181 };									// Hanter 48181

			// Spells Demonologie
			uint32 Spell_branche2_agro;	//    
			uint32 Spell_branche2_1;
			uint32 Spell_branche2_2;
			uint32 Spell_branche2_3;
			uint32 branche2_agro[2] = { 6353, 105174 };									// Feu de l'âme 6353, Main de Gul’dan 105174
			uint32 branche2_1[2] = { 686, 686 };										// Trait de l'ombre 686
			uint32 branche2_2[2] = { 980, 980 };										// Agonie 980 , corruption 172
			uint32 branche2_3[2] = { 70388, 70388 };									// Graine de Corruption 70388
			uint32 Pet_Demo;
			uint32 Pet_Demo_Liste[5] = { 30146, 30146, 30146, 30146, 30146 };			// Gangregarde 30146, Succube 712, Chasseur corrompu 691, Marcheur du Vide 697, Diablotin 688

			// Spells Destruction
			uint32 Spell_branche3_agro;
			uint32 Spell_branche3_1;
			uint32 Spell_branche3_2;
			uint32 Spell_branche3_3;
			uint32 branche3_agro[2] = { 116858, 116858 };								// Trait du chaos 116858
			uint32 branche3_1[2] = { 29722, 29722 };									// Incinérer 29722
			uint32 branche3_2[2] = { 17962, 17962 };									// Conflagration 17962
			uint32 branche3_3[2] = { 348, 348 };										// Immolation 348   
			
			// Emotes
			uint32 Npc_Emotes[22] = { 1,3,7,11,15,16,19,21,22,23,24,53,66,71,70,153,254,274,381,401,462,482 };

			uint32 Start_Agro = 0;

			void Init_AI()
			{
				// ################################################################################################################################################
				// Forcer le choix de la Spécialisation par creature_template->pickpocketloot
				// ################################################################################################################################################
				// Forcer le choix de la Spécialisation par creature_template->pickpocketloot
				ForceBranche = me->GetCreatureTemplate()->pickpocketLootId;							// creature_template->pickpocketloot
				if (ForceBranche == 1) { BrancheSpe = 1; }											// branche1 forcé
				else if (ForceBranche == 2) { BrancheSpe = 2; }										// branche2 forcé 
				else if (ForceBranche == 3) { BrancheSpe = 3; }										// branche3 forcé
				else
				{
					// Sinon Choix de la Spécialisation Aléatoire
					BrancheSpe = urand(1, NbrDeSpe);
				}

				if ((BrancheSpe > NbrDeSpe) || (BrancheSpe == 0)) { BrancheSpe = 1; }

				me->RemoveAllControlled();															// renvois pet

				// ################################################################################################################################################
				// Tirages aléatoires des spells
				// ################################################################################################################################################
				// Spell a lancer a l'agro ------------------------------------------------------------------------------------------------------------------------

				switch (BrancheSpe)
				{
				case 1: // Si Affliction --------------------------------------------------------------------------------------------------------------------------
					Buf_branche1 = Buf_branche1_liste[urand(0, 1)];
					me->CastSpell(me, Buf_branche1, true);															// Buf1 sur lui meme
					me->CastSpell(me, Gangrarmure, true);															// Gangrarmure, Armure_Demoniaque

					me->LoadEquipment(1, true);																		// creature_equip_template 1

					// Tirages aléatoires des spells Affliction 
					Spell_branche1_agro = branche1_agro[urand(0, 1)];
					Spell_branche1_1 = branche1_1[urand(0, 1)];
					Spell_branche1_2 = branche1_2[urand(0, 1)];
					Spell_branche1_3 = branche1_3[urand(0, 1)];
					Spell_branche1_4 = branche1_4[urand(0, 1)];
					break;

				case 2: // Si Demonologie -------------------------------------------------------------------------------------------------------------------------
					Buf_branche2 = Buf_branche2_liste[urand(0, 1)];
					me->CastSpell(me, Buf_branche2, true);															// Buf2 sur lui meme
					me->CastSpell(me, Armure_Demoniaque, true);														// Gangrarmure, Armure_Demoniaque

					me->LoadEquipment(2, true);																		// creature_equip_template 2

					// Tirages aléatoires des spells Demonologie 
					Spell_branche2_agro = branche2_agro[urand(0, 1)];
					Spell_branche2_1 = branche2_1[urand(0, 1)];
					Spell_branche2_2 = branche2_2[urand(0, 1)];
					Spell_branche2_3 = branche2_3[urand(0, 1)];

					// Tirages aléatoires du pet
					 Pet_Demo = Pet_Demo_Liste[urand(0, 4)];
					 me->CastSpell(me, Pet_Demo, true);
					break;

				case 3: // Si Destruction -------------------------------------------------------------------------------------------------------------------------
					Buf_branche3 = Buf_branche3_liste[urand(0, 1)];
					me->CastSpell(me, Buf_branche3, true);															// Buf3 sur lui meme
					me->CastSpell(me, Gangrarmure, true);															// Gangrarmure, Armure_Demoniaque

					me->LoadEquipment(3, true);																		// creature_equip_template 3

					// Tirages aléatoires des spells Affliction 
					Spell_branche3_agro = branche3_agro[urand(0, 1)];
					Spell_branche3_1 = branche3_1[urand(0, 1)];
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
				me->SetSpeedRate(MOVE_RUN, 1.01f);
				me->SetReactState(REACT_AGGRESSIVE);

				Init_AI();
			}
			void EnterCombat(Unit* /*who*/) override
			{
				ResteADistance = 10 + urand(0, 5);
				Init_AI();
			}
			void EnterEvadeMode(EvadeReason /*why*/) override
			{
				Start_Agro = 0;
				RetireBugDeCombat();
				me->AddUnitState(UNIT_STATE_EVADE);
				me->SetSpeedRate(MOVE_RUN, 1.5f);										// Vitesse de déplacement
				me->GetMotionMaster()->MoveTargetedHome();								// Retour home
				me->RemoveAllControlled();												// renvois pet
			}
			void JustReachedHome() override
			{
				me->RemoveAura(Buf_branche1);
				me->RemoveAura(Buf_branche2);
				me->RemoveAura(Buf_branche3);
				me->RemoveAura(Armure_Demoniaque);
				me->RemoveAura(Gangrarmure);

				me->SetReactState(REACT_AGGRESSIVE);
				me->SetSpeedRate(MOVE_RUN, 1.01f);										// Vitesse par defaut définit a 1.01f puisque le patch modification par type,famille test si 1.0f
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
				if (UpdateVictim() /*&& !me->HasUnitState(UNIT_STATE_MOVE) || me->HasUnitState(UNIT_STATE_CASTING)*/)
				{
					Mana = me->GetPower(POWER_MANA);
					Unit* victim = me->GetVictim();
					Dist = me->GetDistance(victim);

					if (Start_Agro == 0)
					{
						Start_Agro = 1;

						// ################################################################################################################################################
						// Spell a lancer a l'agro 
						// ################################################################################################################################################
						switch (BrancheSpe)
						{
						case 1: // Si Affliction --------------------------------------------------------------------------------------------------------------------------
							me->CastSpell(me, Armure_Demoniaque, true);														// Buf sur lui meme
							Random = urand(1, 2);
							if (Random == 1) { me->CastSpell(victim, Spell_branche1_agro, true); }		// 1/2 Chance de lancer le sort d'agro
							break;

						case 2: // Si Demonologie -------------------------------------------------------------------------------------------------------------------------
							me->CastSpell(me, Armure_Demoniaque, true);														// Buf sur lui meme
							Random = urand(1, 2);
							if (Random == 1) { me->CastSpell(victim, Spell_branche2_agro, true); }		// 1/2 Chance de lancer le sort d'agro

							// Tirages aléatoires du pet
							Pet_Demo = Pet_Demo_Liste[urand(0, 4)];
							me->CastSpell(me, Pet_Demo, true);
							break;

						case 3: // Si Destruction -------------------------------------------------------------------------------------------------------------------------
							me->CastSpell(me, Gangrarmure, true);															// Buf sur lui meme
							Random = urand(1, 2);
							if (Random == 1) { me->CastSpell(victim, Spell_branche3_agro, true); }		// 1/2 Chance de lancer le sort d'agro
							break;

							// ################################################################################################################################################
						}
					}


					// ############################################################################################################################################
					// Combat suivant la Spécialisation
				switch (BrancheSpe)
				{
				case 1: // Spécialisation Affliction ##############################################################################################################
						// Regen mana en combat -------------------------------------------------------------------------------------------------------------------
					if (Cooldown_RegenMana <= diff)
					{
						me->SetPower(POWER_MANA, Mana + (MaxMana / 12));
						if (Mana > MaxMana) { me->SetPower(POWER_MANA, MaxMana); }
						Cooldown_RegenMana = urand(2500, 3000);
					}
					else Cooldown_RegenMana -= diff;

					// Combat -------------------------------------------------------------------------------------------------------------------------------------
					// Spell4 sur la cible  (Sort secondaire tres lent , généralement utilisé comme Dot)
					if (Cooldown_Spell4 <= diff)
					{
						DoCastAOE(Spell_branche1_4, true);
						Cooldown_Spell4 = urand(8000, 10000);
					}
					else Cooldown_Spell4 -= diff;

					// Spell3 sur la cible  (Sort secondaire tres lent , généralement utilisé comme Dot)
					if (Cooldown_Spell3 <= diff)
					{
						DoCastVictim(Spell_branche1_3);
						Cooldown_Spell3 = urand(10000, 12000);
					}
					else Cooldown_Spell3 -= diff;

					// Spell2 sur la cible chaque (Sort secondaire plus lent)
					if (Cooldown_Spell2 <= diff /*&& !me->HasUnitState(UNIT_STATE_CASTING)*/)
					{
						DoCastVictim(Spell_branche1_2);
						Cooldown_Spell2 = urand(12000, 14000);
					}
					else Cooldown_Spell2 -= diff;

					// Spell1 sur la cible chaque (Sort Régulié)
					if (Cooldown_Spell1 <= diff)
					{
						DoCastVictim(Spell_branche1_1);
						Cooldown_Spell1 = 3500;
					}
					else Cooldown_Spell1 -= diff;
					break;
				case 2: // Spécialisation Demonologie #############################################################################################################
						// Regen mana en combat -------------------------------------------------------------------------------------------------------------------
					if (Cooldown_RegenMana <= diff)
					{
						me->SetPower(POWER_MANA, Mana + (MaxMana / 12));
						if (Mana > MaxMana) { me->SetPower(POWER_MANA, MaxMana); }
						Cooldown_RegenMana = urand(2500, 3000);
					}
					else Cooldown_RegenMana -= diff;

					// Combat -------------------------------------------------------------------------------------------------------------------------------------
					// Spell1 sur la cible chaque (Sort Régulié)
					if (Cooldown_Spell1 <= diff)
					{
						DoCastVictim(Spell_branche2_1);
						Cooldown_Spell1 = 3500;
					}
					else Cooldown_Spell1 -= diff;

					// Spell2 sur la cible chaque (Sort secondaire plus lent)
					if (Cooldown_Spell2 <= diff)
					{
						DoCastVictim(Spell_branche2_2);
						Cooldown_Spell2 = urand(14000, 18000);
					}
					else Cooldown_Spell2 -= diff;

					// Spell3 sur la cible  (Sort secondaire tres lent , généralement utilisé comme Dot)
					if (Cooldown_Spell3 <= diff)
					{
						DoCastVictim(Spell_branche2_3);
						Cooldown_Spell3 = urand(12000, 14000);
					}
					else Cooldown_Spell3 -= diff;
					break;
				case 3: // Spécialisation Destruction #############################################################################################################
						// Regen mana en combat ------------------------------------------------------------------------------------------------------------------------
					if (Cooldown_RegenMana <= diff)
					{
						me->SetPower(POWER_MANA, Mana + (MaxMana / 20));
						if (Mana > MaxMana) { me->SetPower(POWER_MANA, MaxMana); }
						Cooldown_RegenMana = urand(2000, 2500);
					}
					else Cooldown_RegenMana -= diff;

					// Combat --------------------------------------------------------------------------------------------------------------------------------------
					// Spell3 sur la cible  (Sort secondaire tres lent , généralement utilisé comme Dot)
					if (Cooldown_Spell3 <= diff)
					{
						DoCastVictim(Spell_branche3_3);
						Cooldown_Spell3 = urand(10000, 12000);
					}
					else Cooldown_Spell3 -= diff;

					// Spell2 sur la cible chaque (Sort secondaire plus lent)
					if (Cooldown_Spell2 <= diff)
					{
						DoCastVictim(Spell_branche3_2, true);
						Cooldown_Spell2 = 6500;
					}
					else Cooldown_Spell2 -= diff;

					// Spell1 sur la cible chaque (Sort Régulié)
					if (Cooldown_Spell1 <= diff)
					{
						DoCastVictim(Spell_branche3_1);
						Cooldown_Spell1 = 3000;
					}
					else Cooldown_Spell1 -= diff;
					break;

				}

				// ################################################################################################################################################
				Heal_En_Combat_Caster(diff);
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
				if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
					return;

				Mana = me->GetPower(POWER_MANA);
				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				if (Cooldown_ResteADistance <= diff)
				{
					// Mouvement aléatoire si cible < 6m & Mana > 5% --------------------------------------------------------------------------------------------------

					if ((Dist <6) && (Mana > MaxMana / 20))
					{
						me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);						// UNROOT
						me->SetSpeedRate(MOVE_RUN, 1.1f);

						float x, y, z, mapid;
						x = (me->GetPositionX() + urand(0, ResteADistance * 2) - ResteADistance);
						y = (me->GetPositionY() + urand(0, ResteADistance * 2) - ResteADistance);
						z = me->GetPositionZ();
						mapid = victim->GetMapId();
						me->GetMotionMaster()->MovePoint(mapid, x, y, z);
						Cooldown_ResteADistance = 4000;
					}
				}
				else Cooldown_ResteADistance -= diff;

				// Speed normal si distance > 10m ------------------------------------------------------------------------------------------------------------------
				if (Dist> 10 && me->GetSpeedRate(MOVE_RUN) == 1.1f)
				{
					me->SetSpeedRate(MOVE_RUN, 1.01f);
				}

				// Mouvement OFF si Mana > 5% & distance >= 6m & <= 10m ---------------------------------------------------------------------------------------------
				if ((Mana > MaxMana / 20) && (Dist >= ResteADistance - 4) && (Dist <= ResteADistance))
				{
					AttackStart(victim);
					me->GetMotionMaster()->MoveChase(victim, ResteADistance);							// Pour suivre la cible
					void DoRangedAttackIfReady();														// Combat a distance
					me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);								// ROOT
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
				if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_MOVE))
					return;

				if (Cooldown_Spell_Heal <= diff)
				{
					// heal sur lui meme-------------------------------------------------------------------------------------------------------------------------------
					if ((me->GetHealth() < (me->GetMaxHealth()*0.50)))								// Si PV < 50%
					{
						DoCastVictim(Spell_Heal_Caster);
						Cooldown_Spell_Heal = 3500;
					}
				}
				else Cooldown_Spell_Heal -= diff;
			}



		};
		
		CreatureAI* GetAI(Creature* creature) const override
		{
			return new Stitch_npc_ai_demoAI(creature);
		}
};

//##################################################################################################################################################################
void AddSC_npc_ai_demo()
{
	new Stitch_npc_ai_demo;
}















