////#########################################################################################################################################################################################################################################
// Copyright (C) Juillet 2020 Stitch pour Aquayoup
// AI generique npc par classe : MOINE Ver 2020-11-07
// Il est possible d'influencer le temp entre 2 cast avec `BaseAttackTime` & `RangeAttackTime` 
// Necessite dans Creature_Template :
// Minimun  : UPDATE `creature_template` SET `ScriptName` = 'Stitch_npc_ai_voleur',`AIName` = '' WHERE (entry = 15100010);
// Optionel : UPDATE `creature_template` SET `HealthModifier` = 2, `ManaModifier` = 3, `ArmorModifier` = 1, `DamageModifier` = 2,`BaseAttackTime` = 2000, `RangeAttackTime` = 2000 WHERE(entry = 15100010);
// Optionel : Utilisez pickpocketloot de creature_template pour passer certains parametres (Solution choisit afin de rester compatible avec tout les cores). Si pickpocketloot = 1 (branche1 forcé), pickpocketloot = 2 (branche2 forcé), etc
//###########################################################################################################################################################################################################################################
// # npc de Test Stitch_npc_ai_voleur  .npc 15100010
// REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `HealthModifier`, `HealthModifierExtra`, `ManaModifier`, `ManaModifierExtra`, `ArmorModifier`, `DamageModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES
// (15100011, 0, 0, 0, 0, 0, 45640, 0, 0, 0, 'npc_ai_moine', '', '', '', 0, 90, 90, 0, 0, 2102, 0, 1.01, 1.01, 1, 0, 0, 2000, 2000, 1, 1, 8, 0, 2048, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 2, 1, 1, 1, 2, 2, 1, 0, 144, 1, 0, 0, 'Stitch_npc_ai_moine', 20173);
//REPLACE INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`, `ItemID2`, `ItemID3`, `VerifiedBuild`) VALUES
// (15100011, 1, 88857, 0, 0, 0), ## Lance
// (15100011, 2, 866, 0, 0, 0), ## Baton
// (15100011, 3, 12583, 0, 0, 0);	 ## lance epee rouge
//###########################################################################################################################################################################################################################################



//################################################################################################
//StitchAI AI Moine
//################################################################################################

class Stitch_npc_ai_moine : public CreatureScript
{
public: Stitch_npc_ai_moine() : CreatureScript("Stitch_npc_ai_moine") { }

		struct Stitch_npc_ai_moineAI : public ScriptedAI
		{
			Stitch_npc_ai_moineAI(Creature* creature) : ScriptedAI(creature) { }

			uint32 BrancheSpe = 1;													// Choix de la Spécialisation : Maitre-Brasseur = 1, Tisse-Brume = 2, Marche-vent =3
			uint32 NbrDeSpe = 3;													// Nombre de Spécialisations
			uint32 ForceBranche;
			uint32 Random;
			uint32 DistanceDeCast = 30;												// Distance max a laquelle un npc attaquera , au dela il quite le combat
			uint32 ResteADistance = 5;												// Distance max a laquelle un npc s'approchera
			uint32 Dist;															// Distance entre le npc et sa cible
			uint32 Mana;
			uint32 MaxMana = me->GetMaxPower(POWER_ENERGY);
			Unit* victim = me->GetVictim();										 
			
			// Definitions des variables Cooldown et le 1er lancement
			uint32 Cooldown_Spell1 = 500;
			uint32 Cooldown_Spell2 = 2000;
			uint32 Cooldown_Spell3 = 3500;
			uint32 Cooldown_Spell4 = urand(5000, 12000);;
			uint32 Cooldown_Spell_Heal = 3000;
			uint32 Cooldown_RegenMana = 2000;
			uint32 Cooldown_ResteADistance = 4000;									// Test si en contact
			uint32 Cooldown_ResteAuContact;
			uint32 Cooldown_Anti_Bug_Figer = 2000;
			uint32 Cooldown_Npc_Emotes = urand(5000, 8000);
			uint32 Cooldown_Trop_Loin = 8000;

			// Spells Divers
			uint32 Buf_branche1 = 115069;											// Posture du buffle vigoureux 115069, Posture du tigre féroce 103985
			uint32 Buf_branche1a;
			uint32 Liste_Buf_branche1a[2] = { 116781, 115203 };						// Héritage du tigre blanc 116781 (F,AG,Int +10%), Boisson fortifiante 115203 (pv +20%), 
			
			uint32 Buf_branche2 = 115070;											// Posture du serpent avisé 115070
			uint32 Buf_branche2a;													// 
			uint32 Liste_Buf_branche2a[2] = { 115203, 115203 };						// Boisson fortifiante 115203 (pv +20%), Cocon de vie 116849

			uint32 Buf_branche3 = 103985;											// Posture du tigre féroce 103985
			uint32 Buf_branche3a;													// 
			uint32 Liste_Buf_branche3a[3] = { 115176, 115203, 115203 };				// Méditation zen 115176 (degat -90%/8s), Boisson fortifiante 115203 (pv +20%)

			uint32 Spell_Heal1 = 116694;  											// Déferlante de brume 116694
			uint32 Spell_Heal2 = 115151;  											// Brume de rénovation 115151
			uint32 Spell_Heal3 = 116694;  											// Déferlante de brume 116694
			uint32 Spell_Trop_Loin = 109132;										// Roulade 109132

			// Spells Maitre-Brasseur
			uint32 Spell_branche1_agro;												// 
			uint32 Liste_branche1_agro[3] = { 121253, 109132, 109132 };				// Fracasse-tonneau 121253, Roulade 109132
			uint32 Spell_branche1_1 = 100780;										// Coup direct 100780
			uint32 Spell_branche1_2 = 100787;										// Paume du tigre 100787
			uint32 Spell_branche1_3 = 100784;										// Frappe du voile noir 100784
			uint32 Spell_branche1_4 ; 
			uint32 Liste_branche1_4[3] = { 101546, 115181, 115080 };				// Coup tournoyant de la grue 101546, Souffle de feu 115181, Toucher mortel 115080
						
			// SpellsTisse-Brume
			uint32 Spell_branche2_agro ;											// Paralysie 115078, Pique de main 116705 
			uint32 Liste_branche2_agro[2] = { 121253, 109132};						// Fracasse-tonneau 121253, Roulade 109132
			uint32 Spell_branche2_1 = 100780;										// Coup direct 100780
			uint32 Spell_branche2_2 = 100780;										// Coup direct 100780
			uint32 Spell_branche2_3 = 115080;										// Toucher mortel 115080
			uint32 Spell_branche2_4 = 115072;										// Extraction du mal 115072 (damage+heal)
			
			// Spells Marche-vent
			uint32 Spell_branche3_agro;  
			uint32 Liste_branche3_agro[3] = { 116095, 101545, 101546 };				// Handicap 116095, Coup du serpent volant 101545, Pique de main 116705, Coup tournoyant de la grue 101546
			uint32 Spell_branche3_1 = 100780;										// Coup direct 100780
			uint32 Spell_branche3_2 = 122470;										// Toucher du karma 122470 6s
			uint32 Spell_branche3_3= 116095;										// Coup de pied du soleil levant 107418 8s, Handicap 116095 15s
			uint32 Spell_branche3_4 = 113656;										// Poings de fureur 113656 15s

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
				else if (ForceBranche == 3) { BrancheSpe = 3; }										// branche2 forcé 
				else
				{
					// Sinon Choix de la Spécialisation Aléatoire
					BrancheSpe = urand(1, NbrDeSpe);
				}

				if ((BrancheSpe > NbrDeSpe) || (BrancheSpe == 0)) { BrancheSpe = 1; }

				// ################################################################################################################################################
				// Tirages aléatoires des spells
				// ################################################################################################################################################
				// Spell a lancer a l'agro ------------------------------------------------------------------------------------------------------------------------

				switch (BrancheSpe)
				{
				case 1: // Si Maitre-Brasseur ---------------------------------------------------------------------------------------------------------------------
					me->LoadEquipment(1, true);													// creature_equip_template 1

					me->CastSpell(me, Buf_branche1, true);
					Buf_branche1a = Liste_Buf_branche1a[urand(0, 1)];							
					me->CastSpell(me, Buf_branche1a, true);

					Spell_branche1_4 = Liste_branche1_4[urand(0, 2)];

					VisuelPowerEnergy();
					break;

				case 2: // Si Tisse-Brume -------------------------------------------------------------------------------------------------------------------------
					me->LoadEquipment(2, true);													// creature_equip_template 2

					me->CastSpell(me, Buf_branche2, true);										
					Buf_branche2a = Liste_Buf_branche2a[urand(0, 1)];							
					me->CastSpell(me, Buf_branche2a, true);

					VisuelPowerMana();
					break;
				case 3: // Si Marche-vent -------------------------------------------------------------------------------------------------------------------------
					me->LoadEquipment(3, true);													// creature_equip_template 2

					me->CastSpell(me, Buf_branche3, true);										
					Buf_branche3a = Liste_Buf_branche3a[urand(0, 2)];							
					me->CastSpell(me, Buf_branche3a, true);

					VisuelPowerEnergy();
					break;

				}
				// ################################################################################################################################################
				// Divers
				// ################################################################################################################################################
				//me->SetSheath(SHEATH_STATE_MELEE);												// S'equipe de l'arme au contact
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
				ResteADistance = urand(3, 5);
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

				me->RemoveAura(Buf_branche1);
				me->RemoveAura(Buf_branche1a);
				me->RemoveAura(Buf_branche2);
				me->RemoveAura(Buf_branche2a);
				me->RemoveAura(Buf_branche3);
				me->RemoveAura(Buf_branche3a);

				Bonus_Armure(100);														// Retire bonus d'armure
			}
			void JustReachedHome() override
			{
				me->RemoveAura(Buf_branche1);
				me->RemoveAura(Buf_branche1a);
				me->RemoveAura(Buf_branche2);
				me->RemoveAura(Buf_branche2a);
				me->RemoveAura(Buf_branche3);
				me->RemoveAura(Buf_branche3a);

				Bonus_Armure(100);
				//VisuelPowerEnergy();

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
					Unit* victim = me->GetVictim();
					Dist = me->GetDistance(victim);

					if (Start_Agro == 0)
					{
						Start_Agro = 1;

						// ########################################################################################################################################
						// Spell a lancer a l'agro 
						// ########################################################################################################################################
						switch (BrancheSpe)
						{
						case 1: // Si Maitre-Brasseur -------------------------------------------------------------------------------------------------------------
							Mana = me->GetPower(POWER_ENERGY);

							Random = urand(1, 2);																			// 1/2 Chance de lancer le sort d'agro
							if (Random == 1 && UpdateVictim()) 
							{ 
								Spell_branche1_agro = Liste_branche1_agro[urand(0, 2)];
								me->CastSpell(victim, Spell_branche1_agro, true); 
							}		

							//Bonus_Armure(125);																			// Bonus d'armure +25%
							break;

						case 2: // Si Tisse-Brume -----------------------------------------------------------------------------------------------------------------
							Mana = me->GetPower(POWER_ENERGY);

							Random = urand(1, 2);																			// 1/2 Chance de lancer le sort d'agro
							if (Random == 1 && UpdateVictim()) 
							{ 
								Spell_branche2_agro = Liste_branche2_agro[urand(0, 1)]; 
								DoCastVictim(Spell_branche2_agro); 
							}						

							//Bonus_Armure(125);																			// Bonus d'armure +25%
							break;

						case 3: // Si Marche-vent  ----------------------------------------------------------------------------------------------------------------
							Mana = me->GetPower(POWER_ENERGY);

							Random = urand(1, 2);																			// 1/2 Chance de lancer le sort d'agro
							if (Random == 1 && UpdateVictim()) 
							{ 
								Spell_branche3_agro = Liste_branche3_agro[urand(0, 2)];
								me->CastSpell(victim, Spell_branche3_agro, true); 
							}		

							//Bonus_Armure(125);																			// Bonus d'armure +25%
							break;
							// ####################################################################################################################################
						}
					}


					// ############################################################################################################################################
					// Combat suivant la Spécialisation


					switch (BrancheSpe)
					{
					case 1: // Spécialisation Maitre-Brasseur  ####################################################################################################
							// Regen en combat --------------------------------------------------------------------------------------------------------------------
						Mana = me->GetPower(POWER_ENERGY);
						if (Cooldown_RegenMana <= diff)
						{
							me->SetPower(POWER_ENERGY, Mana + (MaxMana / 20));
							if (Mana > MaxMana) { me->SetPower(POWER_ENERGY, MaxMana); }
							Cooldown_RegenMana = 2000;
						}
						else Cooldown_RegenMana -= diff;

						me->SetPower(POWER_CHI, 4);

						// Combat ---------------------------------------------------------------------------------------------------------------------------------
						// Spell1 sur la cible
						if (Cooldown_Spell1 <= diff)
						{
							//Bonus_Degat_Arme_Done(-40);											// Reduction des degats infligés
							me->CastSpell(victim, Spell_branche1_1, true);
							//Bonus_Degat_Arme_Done(40);
							DoMeleeAttackIfReady();													// Combat en mélée
							Cooldown_Spell1 = 4500;
						}
						else Cooldown_Spell1 -= diff;

						// Spell2 sur la cible
						if (Cooldown_Spell2 <= diff)
						{
							me->CastSpell(victim, Spell_branche1_2, true);
							Cooldown_Spell2 = 4500;
						}
						else Cooldown_Spell2 -= diff;

						// Spell3 sur la cible 
						if (Cooldown_Spell3 <= diff)
						{
							me->CastSpell(victim, Spell_branche1_3, true);
							Cooldown_Spell3 = 4500;
						}
						else Cooldown_Spell3 -= diff;

						// Spell4 sur la cible 
						if (Cooldown_Spell4 <= diff)
						{
							me->CastSpell(victim, Spell_branche1_4, true);
							Cooldown_Spell4 = urand(12000, 15000);
						}
						else Cooldown_Spell4 -= diff;

						Heal_En_Combat_Melee(diff);
						break;

					case 2: // Spécialisation Tisse-Brume #########################################################################################################
							// Regen en combat --------------------------------------------------------------------------------------------------------------------
						Mana = me->GetPower(POWER_DEMONIC_FURY);
						if (Cooldown_RegenMana <= diff)
						{
							me->SetPower(POWER_DEMONIC_FURY, Mana + (MaxMana / 10));
							if (Mana > MaxMana) { me->SetPower(POWER_DEMONIC_FURY, MaxMana); }
							Cooldown_RegenMana = 2000;
						}
						else Cooldown_RegenMana -= diff;

						me->SetPower(POWER_CHI, 4);

						// Combat ---------------------------------------------------------------------------------------------------------------------------------
						// Spell1 sur la cible
						if (Cooldown_Spell1 <= diff)
						{
							Bonus_Degat_Arme_Done(-35);												// Reduction des degats infligés
							me->CastSpell(victim, Spell_branche2_1, true);
							Bonus_Degat_Arme_Done(35);	
							DoMeleeAttackIfReady();													// Combat en mélée
							Cooldown_Spell1 = 4500;
						}
						else Cooldown_Spell1 -= diff;

						// Spell2 sur la cible
						if (Cooldown_Spell2 <= diff)
						{
							Bonus_Degat_Arme_Done(-35);												// Reduction des degats infligés
							me->CastSpell(victim, Spell_branche2_2, true);
							Bonus_Degat_Arme_Done(35);
							Cooldown_Spell2 = 5500;
						}
						else Cooldown_Spell2 -= diff;

						// Spell3 sur la cible
						if (Cooldown_Spell3 <= diff)
						{
							Bonus_Degat_Arme_Done(-15);												// Reduction des degats infligés
							me->CastSpell(victim, Spell_branche2_3, true);
							Bonus_Degat_Arme_Done(15);
							Cooldown_Spell3 = urand(6000,8000);
						}
						else Cooldown_Spell3 -= diff;

						// Spell4 sur la cible
						if (Cooldown_Spell4 <= diff)
						{
							me->CastSpell(victim, Spell_branche2_4, true);
							Cooldown_Spell4 = urand(15000,20000);
						}
						else Cooldown_Spell4 -= diff;

						Heal_En_Combat_Melee(diff);
						break;
					case 3: // Spécialisation Marche-vent #########################################################################################################
							// Regen en combat --------------------------------------------------------------------------------------------------------------------
						Mana = me->GetPower(POWER_ENERGY);
						if (Cooldown_RegenMana <= diff)
						{
							me->SetPower(POWER_ENERGY, Mana + (MaxMana / 10));
							if (Mana > MaxMana) { me->SetPower(POWER_ENERGY, MaxMana); }
							Cooldown_RegenMana = 2000;
						}
						else Cooldown_RegenMana -= diff;

						me->SetPower(POWER_CHI, 4);

						// Combat ---------------------------------------------------------------------------------------------------------------------------------
						// Spell1 sur la cible
						if (Cooldown_Spell1 <= diff)
						{
							//Bonus_Degat_Arme_Done(-40);											// Reduction des degats infligés
							me->CastSpell(victim, Spell_branche3_1, true);
							//Bonus_Degat_Arme_Done(40);
							DoMeleeAttackIfReady();													// Combat en mélée
							Cooldown_Spell1 = 2500;
						}
						else Cooldown_Spell1 -= diff;

						// Spell2 sur la cible
						if (Cooldown_Spell2 <= diff)
						{
							me->CastSpell(victim, Spell_branche3_2, true);
							Cooldown_Spell2 = urand(6000,8000);
						}
						else Cooldown_Spell2 -= diff;

						// Spell3 sur la cible 
						if (Cooldown_Spell3 <= diff)
						{
							me->CastSpell(victim, Spell_branche3_3, true);
							Cooldown_Spell3 = urand(15000,20000);
						}
						else Cooldown_Spell3 -= diff;

						// Spell4 sur la cible
						if (Cooldown_Spell4 <= diff)
						{
							me->CastSpell(victim, Spell_branche3_4, true);
							Cooldown_Spell4 = 15000;
						}
						else Cooldown_Spell4 -= diff;

						Heal_En_Combat_Melee(diff);
						break;

					}

					Mouvement_Contact(diff);

					// ############################################################################################################################################

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
			void Mouvement_Contact(uint32 diff)
			{
				if (!UpdateVictim())
					return;


				if (BrancheSpe == 1 || BrancheSpe == 3)
				{
					Mana = me->GetPower(POWER_ENERGY); 
				} 
				else Mana = me->GetPower(POWER_MANA);

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				//DoMeleeAttackIfReady();													// Combat en mélée

				// Si la cible >= 8m (pour éviter bug de rester figé) --------------------------------------------------------------------------------------------
				if ((Dist >= 8) && (Cooldown_Anti_Bug_Figer <= diff))
				{
					float x, y, z, mapid;
					x = (victim->GetPositionX() + urand(0, 4) - 2);
					y = (victim->GetPositionY() + urand(0, 4) - 2);
					z = victim->GetPositionZ();
					mapid = victim->GetMapId();
					me->GetMotionMaster()->MovePoint(mapid, x, y, z);
					me->GetMotionMaster()->MoveChase(victim, 1, frand(0, 6.2836f));			// Pour suivre la cible avec un angle
					Cooldown_Anti_Bug_Figer = 1000;
				}
				else Cooldown_Anti_Bug_Figer -= diff;

				// Si la cible est entre 10 & 20m 
				if (Cooldown_Trop_Loin <= diff)
				{
					if ((Dist >= 10) && (Dist <= 20))
					{
						Random = urand(1, 4);
						if (Random != 1)
						{
							DoCastVictim(Spell_Trop_Loin);									// 3 chance sur 4    
						}
						Cooldown_Trop_Loin = urand(10000, 15000);
					}
				}
				else Cooldown_Trop_Loin -= diff;

				// Si la cible < 8m -------------------------------------------------------------------------------------------------------------------------------------------
				if ((Dist < 8) & (Cooldown_ResteADistance <= diff) )
				{
					Random = urand(1, 4);
					if (Random == 1 || Random == 2)
					{
						Tourne_Au_Tour_En_Combat();											// 2 chances sur 4 tourne au tour de sa victime
					}
					else if (Random == 3 || Random == 4)
					{
						Avance_3m_En_Combat();												// 2 chances sur 4 avance
					}
					Cooldown_ResteADistance = urand(5000, 7000);
				}
				else Cooldown_ResteADistance -= diff;

			}
			void Tourne_Au_Tour_En_Combat()
			{
				if (!UpdateVictim())
					return;

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				float x, y, z, mapid;
				x = (victim->GetPositionX() + urand(0, 4) - 2);
				y = (victim->GetPositionY() + urand(0, 4) - 2);
				z = victim->GetPositionZ();
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

				float x, y, z, mapid;
				x = (victim->GetPositionX());
				y = (victim->GetPositionY());
				z = victim->GetPositionZ();
				mapid = victim->GetMapId();
				me->GetClosePoint(x, y, z, me->GetObjectSize() / 3, 3);
				me->GetMotionMaster()->MovePoint(mapid, x, y, z);
			}

			void Heal_En_Combat_Melee(uint32 diff)
			{
				// Heal en combat ------------------------------------------------------------------------------------------------------------------------------
				Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, DistanceDeCast, true);		// pour heal friend

				switch (BrancheSpe)
				{
					// Si Maitre-Brasseur ----------------------------------------------------------------------------------------------------------------------
				case 1:
					if (Cooldown_Spell_Heal <= diff)
					{
						if (me->GetHealth() < (me->GetMaxHealth()*0.50)) 							// Si PV < 50%
						{
							me->CastSpell(me, Spell_Heal1, true);
							Cooldown_Spell_Heal = urand(6000, 9000);
						}
					}
					else Cooldown_Spell_Heal -= diff;
					break;

					// Si Tisse-Brume --------------------------------------------------------------------------------------------------------------------------
					case 2:
					if (Cooldown_Spell_Heal <= diff)
					{
						if (me->GetHealth() < (me->GetMaxHealth()*0.70)) 							// Si PV < 70%
						{
							me->CastSpell(me, Spell_Heal2, true);
							Cooldown_Spell_Heal = 8000;
						}
					}
					else Cooldown_Spell_Heal -= diff;
					
					// heal sur Friend
					if (target = DoSelectLowestHpFriendly(DistanceDeCast))							// Distance de l'allié = 30m
					{
						if (me->IsFriendlyTo(target) && (me != target))
						{
							if (target->GetHealth() < (target->GetMaxHealth()*0.50))				// Si PV < 50%
							{
								DoCast(target, Spell_Heal2);
								Cooldown_Spell_Heal = 8000;
							}
						}
					}
					break;

					// Si Marche-vent -----------------------------------------------------------------------------------------------------------------------------
					case 3:
					if (Cooldown_Spell_Heal <= diff)
					{
						if (me->GetHealth() < (me->GetMaxHealth()*0.50)) 						// Si PV < 50%
						{
							me->CastSpell(me, Spell_Heal3, true);
							Cooldown_Spell_Heal = urand(7000, 10000);
						}
					}
					else Cooldown_Spell_Heal -= diff;
					break;
				}
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
				// +- Bonus d'armure 100% = pas de bonus/malus   ex : Bonus_Armure(115); // Bonus d'armure +15%
				me->SetModifierValue(UNIT_MOD_ARMOR, BASE_VALUE, me->GetArmor() * (val / 100));
				me->SetCanModifyStats(true);
				me->UpdateAllStats();
			}
			
			void VisuelPowerEnergy()
		{
				//me->SetByteValue(UNIT_FIELD_BYTES_0, UNIT_BYTES_0_OFFSET_CLASS, 4);

				me->SetMaxPower(POWER_MANA, 100);
				me->SetPower(POWER_MANA, 100);
				me->setPowerType(POWER_ENERGY);
				me->SetMaxPower(POWER_ENERGY, 100);
				me->SetPower(POWER_ENERGY, 100);
		}
			void VisuelPowerMana()
			{
				me->setPowerType(POWER_MANA);
				me->SetMaxPower(POWER_MANA, 100);
				me->SetPower(POWER_MANA, 100);
			}

		};
		CreatureAI* GetAI(Creature* creature) const override
		{
			return new Stitch_npc_ai_moineAI(creature);
		}
};

//#################################################################################################################################################################
void AddSC_npc_ai_moine()
{
	new Stitch_npc_ai_moine();
}















