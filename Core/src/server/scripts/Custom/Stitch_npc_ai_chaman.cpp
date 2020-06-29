////#########################################################################################################################################################################################################################################
// Copyright (C) Juin 2020 Stitch pour Aquayoup
// AI generique npc par classe : CHAMAN V1.0
// Il est possible d'influencer le temp entre 2 cast avec `BaseAttackTime` & `RangeAttackTime` 
// Necessite dans Creature_Template :
// Minimun  : UPDATE `creature_template` SET `ScriptName` = 'Stitch_npc_ai_chaman',`AIName` = '' WHERE (entry = 15100002);
// Optionel : UPDATE `creature_template` SET `HealthModifier` = 2, `ManaModifier` = 3, `ArmorModifier` = 1, `DamageModifier` = 2,`BaseAttackTime` = 2000, `RangeAttackTime` = 2000 WHERE(entry = 15100002);
// Optionel : Utilisez pickpocketloot de creature_template pour passer certains parametres (Solution choisit afin de rester compatible avec tout les cores). Si pickpocketloot = 1 (branche1 forcé), pickpocketloot = 2 (branche2 forcé), etc
//###########################################################################################################################################################################################################################################
// # npc de Test Stitch_npc_ai_chaman  .npc 15100002
// REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `HealthModifier`, `HealthModifierExtra`, `ManaModifier`, `ManaModifierExtra`, `ArmorModifier`, `DamageModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES
// (15100002, 0, 0, 0, 0, 0, 59382, 0, 0, 0, 'Stitch_npc_ai_Chaman', '', '', '', 0, 90, 90, 0, 0, 2102, 0, 1.01, 1.01, 0.7, 0, 0, 2000, 2000, 1, 1, 2, 0, 2048, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 2, 1, 3, 1, 2, 2, 1, 0, 144, 1, 0, 0, 'Stitch_npc_ai_chaman', 20173);
// REPLACE INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`, `ItemID2`, `ItemID3`, `VerifiedBuild`) VALUES
// (15100002, 1, 854, 0, 0, 0),				// baton			- Restauration
// (15100002, 2, 2209, 63165, 0, 0),		// dague+bouclier	- Elementaire
// (15100002, 3, 2209, 2209, 0, 0);		    // dague * 2		- Amelioration
//###########################################################################################################################################################################################################################################



//################################################################################################
//StitchAI AI Chaman
//################################################################################################

class Stitch_npc_ai_chaman : public CreatureScript
{
public: Stitch_npc_ai_chaman() : CreatureScript("Stitch_npc_ai_chaman") { }

		struct Stitch_npc_ai_chamanAI : public ScriptedAI
		{
			Stitch_npc_ai_chamanAI(Creature* creature) : ScriptedAI(creature) { }
			uint32 BrancheSpe = 1;													// Choix de la Spécialisation : Restauration=1, Elementaire=2, Amelioration=3
			uint32 NbrDeSpe = 3;													// Nombre de Spécialisations
			uint32 Random;
			uint32 DistanceDeCast = 30;												// Distance max a laquelle un npc attaquera , au dela il quite le combat
			uint32 ResteADistance = 15;												// Distance max a laquelle un npc s'approchera
			uint32 Dist;															// Distance entre le npc et sa cible
			uint32 Mana;
			uint32 MaxMana = me->GetMaxPower(POWER_MANA);

			Unit* victim = me->GetVictim();										 
			uint32 Buf_branche1 = 974;												// Bouclier de foudre 324 - Bouclier de terre 974 - Bouclier d’eau 52127
			uint32 Buf_branche2 = 52127;											// Bouclier de foudre 324 - Bouclier de terre 974 - Bouclier d’eau 52127
			uint32 Buf_branche3 = 324;												// Bouclier de foudre 324 - Bouclier de terre 974 - Bouclier d’eau 52127
			
			// Definitions des variables Cooldown et le 1er lancement
			uint32 Cooldown_Spell1 = 500;
			uint32 Cooldown_Spell2 = 2500;
			uint32 Cooldown_Spell3 = 3500;
			uint32 Cooldown_Spell_Heal = 3000;
			uint32 Cooldown_RegenMana = 3000;
			uint32 Cooldown_ResteADistance = 1000;									// Test si en contact pour  Restauration, Elementaire et s'eloigner, bouger en combat pour Amelioration
			uint32 Cooldown_Charge = 8000;

			// Spells Divers
			uint32 Spell_Heal_Caster = 8004;  										// Afflux de soins 
			uint32 Spell_Heal_Heal = 77472;  										// Vague de soins 
			uint32 Rappel_totemique = 36936;										// Rappel totemique : retire les totems invoqués
			uint32 Totem_de_magma = 8190;											// Totem de magma
			uint32 Horion_de_terre = 8042;											// Horion de terre 
			uint32 Horion_de_givre = 8056;											// Horion de givre

			// Spells Restauration
			uint32 Spell_branche1_agro;
			uint32 Spell_branche1_1;
			uint32 Spell_branche1_2;
			uint32 Spell_branche1_3;
			uint32 branche1_agro[4] = { 8050, 8056, 3599, 73393 };					// Horion de flammes 8050 - Horion de givre 8056 - Totem incendiaire 3599 - Totem de flot de mana 73393
			uint32 branche1_1[2] = { 403, 403 };									// Eclair 403
			uint32 branche1_2[2] = { 8056, 8042 };									// Horion de givre 8056 - Horion de terre 8042 (stun 2s)
			uint32 branche1_3[2] = { 8050, 57994 };									// Horion de flammes 8050 30s - Cisaille de vent 57994
			
			// Definitions des Spells Elementaire
			uint32 Spell_branche2_agro;
			uint32 Spell_branche2_1;
			uint32 Spell_branche2_2;
			uint32 Spell_branche2_3;
			uint32 branche2_agro[5] = { 8050, 8056, 3599, 2484, 78222 };			// Horion de flammes 8050 - Horion de givre 8056 - Totem incendiaire 3599 - Totem de lien terrestre 2484 - Totem de peau de pierre 78222
			uint32 branche2_1[4] = { 403, 403, 403, 421 };							// Eclair 403 - Chaîne d'éclairs 421 
			uint32 branche2_2[3] = { 51505, 51505 ,8056 };							// Explosion de lave 51505 - Horion de givre 8056 8s
			uint32 branche2_3[4] = { 8050, 8050, 8042, 57994 };						// Horion de flammes 8050 30s - Horion de terre 8042 (stun 2s) - Cisaille de vent 57994

			// Spells Amelioration
			uint32 Spell_branche3_agro;
			uint32 Spell_branche3_1;
			uint32 Spell_branche3_2;
			uint32 Spell_branche3_3;
			uint32 branche3_agro[4] = { 370, 370, 2484, 3599 };						// Purge 370 (retire 1 buf), Totem de lien terrestre 2484, Totem incendiaire 3599
			uint32 branche3_1[2] = { 300200, 300200 };								// Frappe primordiale 300200
			uint32 branche3_2[3] = { 17364, 60103, 8056 };							// Horion de givre 8056 8s       - Frappe-tempête 17364 - Fouet de lave 60103
			uint32 branche3_3[2] = { 8050, 8042 };									// Horion de flammes 8050 - Cisaille de vent 57994 -  Horion de givre 8056 8s - Horion de terre 8042 (stun 2s)




			void JustRespawned() override
				{
					me->GetMotionMaster()->MoveTargetedHome();								// Retour home
					me->SetSpeedRate(MOVE_RUN, 1.01f);
				}
			void EnterCombat(Unit* /*who*/) override
			{
				if (!UpdateVictim())
					return;
			
				Unit* victim = me->GetVictim();

				// Forcer le choix de la Spécialisation par creature_template->pickpocketloot
				uint32 ForceBranche = me->GetCreatureTemplate()->pickpocketLootId;					// creature_template->pickpocketloot
				if (ForceBranche == 1) { BrancheSpe = 1; }											// branche1 forcé
				else if (ForceBranche == 2) { BrancheSpe = 2; }										// branche2 forcé
				else if (ForceBranche == 3) { BrancheSpe = 3; }										// branche3 forcé 
				else
				{
					// Sinon Choix de la Spécialisation Aléatoire
					BrancheSpe = urand(1, NbrDeSpe + 2);
					if (BrancheSpe > NbrDeSpe) { BrancheSpe = 2; }									// plus de chance d'etre Elementaire
				}

				me->SetReactState(REACT_AGGRESSIVE);


					switch(BrancheSpe)
					{
					case 1: // Si Spécialisation Restauration -----------------------------------------------------------------------------------------------------
						me->CastSpell(me, Buf_branche1, true);								// Buf1 sur lui meme
						me->LoadEquipment(1, true);											// creature_equip_template 1
						me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);				// ROOT

						// Tirages aléatoires des spells Restauration 
						Spell_branche1_agro = branche1_agro[urand(0,3)];
						Spell_branche1_1 = branche1_1[urand(0, 1)];
						Spell_branche1_2 = branche1_2[urand(0, 1)];
						Spell_branche1_3 = branche1_3[urand(0, 1)];

						// Totem
						me->CastSpell(me, 5394, true); 										// Totem guérisseur 5394 (12s)

						Random = urand(1, 2); 
						if (Random == 1) { DoCastVictim(Spell_branche1_agro); }				// 1/2 Chance de lancer le sort d'agro
						//Bonus_Armure(115);													// Bonus d'armure +15%
					break;

					case 2: // Si Spécialisation Elementaire  --------------------------------------------------------------------------------------------------------
						me->CastSpell(me, Buf_branche2, true);								// Buf2 sur lui meme
						me->LoadEquipment(2, true);											// creature_equip_template 2
						me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);				// ROOT

						// Tirages aléatoires des spells Elementaire 
						Spell_branche2_agro = branche2_agro[urand(0, 4)];
						Spell_branche2_1 = branche2_1[urand(0, 3)];
						Spell_branche2_2 = branche2_2[urand(0, 2)];
						Spell_branche2_3 = branche2_3[urand(0, 3)];

						// Totem
						me->CastSpell(me, 3599, true);										// Totem incendiaire 3599

						Random = urand(1, 3);
						if (Random == 1) { DoCastVictim(Spell_branche2_agro); }				// 1/3 Chance de lancer le sort d'agro
						//Bonus_Armure(115);													// Bonus d'armure +15%
					break;

					case 3: // Si Spécialisation Amelioration  --------------------------------------------------------------------------------------------------------
						me->CastSpell(me, Buf_branche3, true);								// Buf3 sur lui meme
						me->LoadEquipment(3, true);											// creature_equip_template 3

						// Tirages aléatoires des spells Amelioration 
						Spell_branche3_agro = branche3_agro[urand(0, 3)];
						Spell_branche3_1 = branche3_1[urand(0, 1)];
						Spell_branche3_2 = branche3_2[urand(0, 2)];
						Spell_branche3_3 = branche3_3[urand(0, 1)];

						// Totem
						me->CastSpell(me, 78222, true); 									// Totem de peau de pierre 78222

						Random = urand(1, 3);
						if (Random == 1) { DoCastVictim(Spell_branche3_agro); }				// 1/3 Chance de lancer le sort d'agro

						Bonus_Armure(115);													// Bonus d'armure +15%
						break;
						
					}

			}
			void JustReachedHome() override
			{
				me->CastSpell(me, Rappel_totemique, true);									// Rappel totémique 36936
				me->RemoveAura(Buf_branche1);
				me->RemoveAura(Buf_branche2);
				me->RemoveAura(Buf_branche3);

				Bonus_Armure(100);															// Retire bonus d'armure

				RetireBugDeCombat();
				me->SetReactState(REACT_AGGRESSIVE);
				me->SetSpeedRate(MOVE_RUN, 1.01f);											// Vitesse par defaut définit a 1.01f puisque le patch modification par type,famille test si 1.0f
			}
			void EnterEvadeMode(EvadeReason /*why*/) override
			{
				RetireBugDeCombat();
				me->AddUnitState(UNIT_STATE_EVADE);
				me->SetSpeedRate(MOVE_RUN, 1.5f);											// Vitesse de déplacement
				me->GetMotionMaster()->MoveTargetedHome();									// Retour home
			}
			void UpdateAI(uint32 diff) override
			{
				if (!UpdateVictim()/* || me->isPossessed() || me->IsCharmed() || me->HasAuraType(SPELL_AURA_MOD_FEAR)*/)
					return;

				Mana = me->GetPower(POWER_MANA);
				Dist = me->GetDistance(me->GetVictim());

				// Combat suivant la Spécialisation
				switch (BrancheSpe)
				{
				case 1: // Spécialisation Restauration #########################################################################################################################

					Combat_Restauration(diff);
					Heal_En_Combat_Heal(diff);
					Mouvement_Caster(diff);
					Mouvement_All();
					break;
					
				case 2 : // Spécialisation Elementaire #########################################################################################################################

					Combat_Elementaire(diff);
					Heal_En_Combat_Caster(diff);
					Mouvement_Caster(diff);
					Mouvement_All();
					break;

				case 3: // Spécialisation Amelioration #########################################################################################################################

					Combat_Amelioration(diff);
					Heal_En_Combat_Caster(diff);
					Mouvement_Contact(diff);
					Mouvement_All();
					break;

				}
			}
			
			void RetireBugDeCombat()
			{
				me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);				// UNROOT
				me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);					// Retire flag "en combat"
				me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);				// Retire flag "non attaquable"
								
				me->DeleteThreatList();
				me->CombatStop(true);
				me->LoadCreaturesAddon();
				me->SetLootRecipient(NULL);
				me->ResetPlayerDamageReq();
				me->SetLastDamagedTime(0);
			}

			void Mouvement_All()
			{
				if (!UpdateVictim())
					return;

				Dist = me->GetDistance(me->GetVictim());
				if ((Dist > DistanceDeCast) | (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > 40))
				{
					RetireBugDeCombat();
					me->AddUnitState(UNIT_STATE_EVADE);
					EnterEvadeMode(EVADE_REASON_SEQUENCE_BREAK);						// Quite le combat si la cible > 30m (Caster & Mélée) ou > 40m de home
				}
			}
			void Mouvement_Caster(uint32 diff)
			{
				if (!UpdateVictim() | (me->HasUnitState(UNIT_STATE_CASTING)))
					return;

				Mana = me->GetPower(POWER_MANA);
				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				// Mouvement aléatoire si cible < 6m & Mana > 5% : bouge -------------------------------------------------------------------------------------------
				if (Cooldown_ResteADistance <= diff)
				{
					if ((Dist < 6) && (Mana > MaxMana / 20))
					{
						me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);					// UNROOT
						me->SetSpeedRate(MOVE_RUN, 1.1f);

						float x, y, z;
						me->GetClosePoint(x, y, z, me->GetObjectSize() / 3, ResteADistance + 5);	// Bouge de ResteADistance + 5m
						me->GetMotionMaster()->MovePoint(0xFFFFFE, x, y, z);

						Cooldown_ResteADistance = 5000;
					}
				}
				else Cooldown_ResteADistance -= diff;

				// Mouvement OFF si Mana > 5% & distance > 5m & <= 20m ---------------------------------------------------------------------------------------------
				if ((Mana > MaxMana / 20) && (Dist > 5) && (Dist <= ResteADistance))
				{
					AttackStartCaster(me->GetVictim(), ResteADistance);								// Distance de cast

					me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);							// ROOT
					me->SetSpeedRate(MOVE_RUN, 1.01f);
				}

				// Mouvement ON si distance > 20m ------------------------------------------------------------------------------------------------------------------
				if (Dist > 20)
				{
					AttackStartCaster(me->GetVictim(), ResteADistance);								// Distance de cast

					me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);						// UNROOT
					me->SetSpeedRate(MOVE_RUN, 1.01f);
				}

				// Mouvement ON si Mana < 5%  ----------------------------------------------------------------------------------------------------------------------
				if (Mana < MaxMana / 20)
				{
					me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);						// UNROOT
					me->SetSpeedRate(MOVE_RUN, 1.01f);
				}

			}
			void Mouvement_Contact(uint32 diff)
			{
				if (!UpdateVictim())
					return;

				Dist = me->GetDistance(me->GetVictim());

				// Si la cible est entre 8 & 30m : Horion de terre ou Horion de givre ------------------------------------------------------------------------------
				if (Cooldown_Charge <= diff)
				{
					Random = urand(1, 8);
					if ((Dist >= 8) && (Dist <= DistanceDeCast))
					{
						if (Random = 1)
						{
							DoSpellAttackIfReady(Horion_de_terre);									// Horion de terre 8042 (stun 2s) - 1 chance sur 8
						}
						else if (Random = 2)
						{
							DoSpellAttackIfReady(Horion_de_givre);									// Horion de givre 8056 8s - 1 chance sur 8
						}
						Cooldown_Charge = urand(15000, 20000);
						Cooldown_ResteADistance = urand(2000, 4000);								// Sinon bugue Totem de magma 8190
					}
				}
				else Cooldown_Charge -= diff;

				// Si la cible < 8m : Totem de magma ou Avance ou Tourne au tour -----------------------------------------------------------------------------------
				if (Dist < 8 && (Cooldown_ResteADistance <= diff))
				{
					Random = urand(1, 5);
					if (Random == 3)
					{
						DoCast(me, Totem_de_magma);													// 1 chance sur 5 : Totem de magma 8190
					}
					else if (Random < 3)
					{
						Avance_3m_En_Combat();
					}
					else
					{
						Tourne_Au_Tour_En_Combat();													// 2 chance sur 5 tourne au tour de sa victime
					}
					Cooldown_ResteADistance = urand(6000, 8000);
				}
				else Cooldown_ResteADistance -= diff;
			}
			void Tourne_Au_Tour_En_Combat()
			{
				if (!UpdateVictim())
					return;
				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(me->GetVictim());

				float x, y, z;
				x = (victim->GetPositionX() + urand(0, 4) - 2);
				y = (victim->GetPositionY() + urand(0, 4) - 2);
				z = victim->GetPositionZ();
				me->GetMotionMaster()->MovePoint(0, x, y, z);

			}
			void Avance_3m_En_Combat()
			{
				if (!UpdateVictim())
					return;

				float x, y, z;
				me->GetClosePoint(x, y, z, me->GetObjectSize() / 3, 3);
				me->GetMotionMaster()->MovePoint(0, x, y, z);
			}

			void Combat_Restauration(uint32 diff)
			{
				if (!UpdateVictim())
					return;

				if (Dist <= DistanceDeCast)
				{
					// Regen mana en combat ------------------------------------------------------------------------------------------------------------------------
					if (Cooldown_RegenMana <= diff)
					{
						me->SetPower(POWER_MANA, Mana + (MaxMana / 10));
						if (Mana > MaxMana) { me->SetPower(POWER_MANA, MaxMana); }
						Cooldown_RegenMana = 1000;
					}
					else Cooldown_RegenMana -= diff;

					// Combat --------------------------------------------------------------------------------------------------------------------------------------
					// Spell1 sur la cible chaque (Sort Régulié)
					if (Cooldown_Spell1 <= diff)
					{
						DoCastVictim(Spell_branche1_1);
						Cooldown_Spell1 = 4000;
					}
					else Cooldown_Spell1 -= diff;

					// Spell2 sur la cible chaque (Sort secondaire plus lent)
					if (Cooldown_Spell2 <= diff)
					{
						DoCastVictim(Spell_branche1_2);
						Cooldown_Spell2 = 8000;
					}
					else Cooldown_Spell2 -= diff;

					// Spell3 sur la cible  (Sort secondaire tres lent , généralement utilisé comme Dot)
					if (Cooldown_Spell3 <= diff)
					{
						DoCastVictim(Spell_branche1_3);
						Cooldown_Spell3 = urand(10000, 12000);
					}
					else Cooldown_Spell3 -= diff;
				}
			}
			void Combat_Elementaire(uint32 diff)
			{
				if (!UpdateVictim())
					return;

				// Regen mana en combat ------------------------------------------------------------------------------------------------------------------------
				if (Cooldown_RegenMana <= diff)
				{
					me->SetPower(POWER_MANA, Mana + (MaxMana / 5));
					if (Mana > MaxMana) { me->SetPower(POWER_MANA, MaxMana); }
					Cooldown_RegenMana = 1500;
				}
				else Cooldown_RegenMana -= diff;

				// Combat --------------------------------------------------------------------------------------------------------------------------------------
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
						Cooldown_Spell2 = 8000;
					}
					else Cooldown_Spell2 -= diff;

					// Spell3 sur la cible  (Sort secondaire tres lent , généralement utilisé comme Dot)
					if (Cooldown_Spell3 <= diff)
					{
						DoCastVictim(Spell_branche2_3);
						Cooldown_Spell3 = urand(10000, 12000);
					}
					else Cooldown_Spell3 -= diff;
				}
			void Combat_Amelioration(uint32 diff)
			{
				if (!UpdateVictim())
					return;

				// Regen mana en combat ----------------------------------------------------------------------------------------------------------------------------
				if (Cooldown_RegenMana <= diff)
				{
					me->SetPower(POWER_MANA, Mana + (MaxMana / 20));
					if (Mana > MaxMana) { me->SetPower(POWER_MANA, MaxMana); }
					Cooldown_RegenMana = 1000;
				}
				else Cooldown_RegenMana -= diff;

				
				// Combat ------------------------------------------------------------------------------------------------------------------------------------------
				if (Dist < 6)
				{
					// Spell1 sur la cible chaque (Sort Régulié)
					if (Cooldown_Spell1 <= diff)
					{
						DoCastVictim(Spell_branche3_1);
						Cooldown_Spell1 = 3500;
					}
					else Cooldown_Spell1 -= diff;

					// Spell2 sur la cible chaque (Sort secondaire plus lent)
					if (Cooldown_Spell2 <= diff)
					{
						Bonus_Degat_Arme_Done(-50);										// Reduction de degat pour ce sort trop puissant
						DoCastVictim(Spell_branche3_2);
						Bonus_Degat_Arme_Done(50);										// On le re initialise 

						Cooldown_Spell2 = urand(5000, 7000);
					}
					else Cooldown_Spell2 -= diff;

					// Spell3 sur la cible  (Sort secondaire tres lent , généralement utilisé comme Dot)
					if (Cooldown_Spell3 <= diff)
					{
						DoCastVictim(Spell_branche3_3);
						Cooldown_Spell3 = urand(12000, 15000);
					}
					else Cooldown_Spell3 -= diff;
				}

			}

			void Heal_En_Combat_Caster(uint32 diff)
			{
			// Heal en combat ------------------------------------------------------------------------------------------------------------------------------
				if (Cooldown_Spell_Heal <= diff)
				{
				Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, DistanceDeCast, true);		// pour heal friend

																								// heal sur lui meme
				if ((me->GetHealth() < (me->GetMaxHealth()*0.60)))								// Si PV < 60%
				{
					DoCast(me, Spell_Heal_Caster);
					Cooldown_Spell_Heal = 3500;
				}

				// heal sur Friend 
				if (target = DoSelectLowestHpFriendly(DistanceDeCast))							// Distance de l'allié = 30m
				{
					if (me->IsFriendlyTo(target) && (me != target))
					{
						if (target->GetHealth() < (target->GetMaxHealth()*0.40))				// Si PV < 40%
						{
							DoCast(target, Spell_Heal_Caster);
							Cooldown_Spell_Heal = 4000;
						}
					}
				}
				}
				else Cooldown_Spell_Heal -= diff;
			}
			void Heal_En_Combat_Heal(uint32 diff)
			{
				// Heal en combat ------------------------------------------------------------------------------------------------------------------------------
				if (Cooldown_Spell_Heal <= diff)
				{
					Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, DistanceDeCast, true);		// pour heal friend

																									// heal sur lui meme
					if ((me->GetHealth() < (me->GetMaxHealth()*0.75)))								// Si PV < 60%
					{
						DoCast(me, Spell_Heal_Heal);
						Cooldown_Spell_Heal = 3000;
					}

					// heal sur Friend 
					if (target = DoSelectLowestHpFriendly(DistanceDeCast))							// Distance de l'allié = 30m
					{
						if (me->IsFriendlyTo(target) && (me != target))
						{
							if (target->GetHealth() < (target->GetMaxHealth()*0.60))				// Si PV < 60%
							{
								DoCast(target, Spell_Heal_Heal);
								Cooldown_Spell_Heal = 3000;
							}
						}
					}
				}
				else Cooldown_Spell_Heal -= diff;
			}

			void Bonus_Degat_Arme_Done(int val) // 
			{
				// +- Bonus en % de degat des armes infligées a victim
				me->HandleStatModifier(UNIT_MOD_ATTACK_POWER, TOTAL_PCT, val, true);
				me->HandleStatModifier(UNIT_MOD_DAMAGE_OFFHAND, TOTAL_PCT, val, true);
				me->HandleStatModifier(UNIT_MOD_DAMAGE_RANGED, TOTAL_PCT, val, true);
			}
			void Bonus_Armure(int val) // 
			{
				// +- Bonus d'armure 100% = pas de bonus/malus   2700 1400 3000
				me->SetModifierValue(UNIT_MOD_ARMOR, BASE_VALUE, me->GetArmor() * (val / 100));
				me->SetCanModifyStats(true);
				me->UpdateAllStats();
			}
		};



		CreatureAI* GetAI(Creature* creature) const override
		{
			return new Stitch_npc_ai_chamanAI(creature);
		}
};

//##################################################################################################################################################################
void AddSC_npc_ai_chaman()
{
	new Stitch_npc_ai_chaman();
}















