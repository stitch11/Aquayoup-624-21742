////#########################################################################################################################################################################################################################################
// Copyright (C) Juillet 2020 Stitch pour Aquayoup
// AI generique npc par classe : VOLEUR Ver 2020-12-09
// Il est possible d'influencer le temp entre 2 cast avec `BaseAttackTime` & `RangeAttackTime` 
// Necessite dans Creature_Template :
// Minimun  : UPDATE `creature_template` SET `ScriptName` = 'Stitch_npc_ai_voleur',`AIName` = '' WHERE (entry = 15100010);
// Optionel : UPDATE `creature_template` SET `HealthModifier` = 2, `ManaModifier` = 3, `ArmorModifier` = 1, `DamageModifier` = 2,`BaseAttackTime` = 2000, `RangeAttackTime` = 2000 WHERE(entry = 15100010);
// Optionel : Utilisez pickpocketloot de creature_template pour passer certains parametres (Solution choisit afin de rester compatible avec tout les cores). Si pickpocketloot = 1 (branche1 forcé), pickpocketloot = 2 (branche2 forcé), etc
//###########################################################################################################################################################################################################################################
// # npc de Test Stitch_npc_ai_voleur  .npc 15100010
//REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `HealthModifier`, `HealthModifierExtra`, `ManaModifier`, `ManaModifierExtra`, `ArmorModifier`, `DamageModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES
//(15100010, 0, 0, 0, 0, 0, 13171, 0, 0, 0, 'npc_ai_voleur', '', '', '', 0, 90, 90, 0, 0, 2102, 0, 1.01, 1.01, 1, 0, 0, 2000, 2000, 1, 1, 4, 0, 2048, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 2, 1, 1, 1, 2, 2, 1, 0, 144, 1, 0, 0, 'Stitch_npc_ai_voleur', 20173);
//REPLACE INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`, `ItemID2`, `ItemID3`, `VerifiedBuild`) VALUES
//(15100010, 1, 2209, 2209, 0, 0),
//(15101510, 2, 36669, 36669, 0, 0);
//###########################################################################################################################################################################################################################################



//################################################################################################
//StitchAI AI Voleur
//################################################################################################

class Stitch_npc_ai_voleur : public CreatureScript
{
public: Stitch_npc_ai_voleur() : CreatureScript("Stitch_npc_ai_voleur") { }

		struct Stitch_npc_ai_voleurAI : public ScriptedAI
		{
			Stitch_npc_ai_voleurAI(Creature* creature) : ScriptedAI(creature) { }

			uint32 BrancheSpe = 1;													// Choix de la Spécialisation : Voleur=1, Vampire Berserker=2
			uint32 NbrDeSpe = 2;													// Nombre de Spécialisations
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
			uint32 Cooldown_Spell4 = 4000;
			uint32 Cooldown_Spell_Heal = 3000;
			uint32 Cooldown_RegenMana = 1000;
			uint32 Cooldown_ResteADistance = 4000;									// Test si en contact
			uint32 Cooldown_ResteAuContact;
			uint32 Cooldown_Anti_Bug_Figer = 2000;
			uint32 Cooldown_Npc_Emotes = urand(5000, 8000);
			uint32 Cooldown_Lancer = 1000;

			// Spells Divers
			uint32 Liste_Buf_branche1[5] = { 31209, 2983, 1784, 31220, 5171 };		// Pied léger 31209 (vit+10%), Sprint 2983,  Camouflage 1784, Vocation pernicieuse 31220 (-ag+15,Armure +100%), Débiter 5171
			uint32 Buf_branche1;								
			uint32 Liste_Buf_branche1a[3] = { 2823, 8679, 3408 };					// Poison mortel 2823, Poison douloureux 8679, Poison affaiblissant 3408
			uint32 Buf_branche1a;
			
			uint32 Vampire_Berserker = 300124;										// Vampire Berserker 300124
			uint32 Buf_branche2 = 300137;											// Peau de demon 300137 (armure+100% esquive/parade+2%), 
			uint32 Liste_Buf_branche2a[3] = { 2823, 8679, 3408 };					// Poison mortel 2823, Poison douloureux 8679, Poison affaiblissant 3408
			uint32 Buf_branche2a;													//  
			uint32 Spell_Heal1 = 97462;  											// Evasion 5277 (esquive +100%/10s)
			uint32 Spell_Lancer = 121733;

			// Spells Voleur
			uint32 Spell_branche1_agro;
			uint32 Spell_branche1_1;
			uint32 Spell_branche1_2;
			uint32 Spell_branche1_3;
			uint32 branche1_agro[3] = { 2094, 121733, 79140 };						// Cécité 2094, Lancer 121733, Vendetta 79140
			uint32 branche1_1[2] = { 1752, 1329 };									// Attaque pernicieuse 1752, Estropier 1329
			uint32 branche1_2[2] = { 2098, 2098 };									// Eviscération 2098
			uint32 branche1_3[5] = { 1776, 1776, 84617, 16511, 1943 };				// Suriner 1776 (stun 4s), Hémorragie 16511 (dps 24s), Frappe révélatrice 84617 (regen 24s), Rupture 1943 (regen + dps 12s)  
			
			// Spells Vampire Berserker
			uint32 Spell_branche2_agro = 300131;									// Griffure bondissante 300131
			uint32 Spell_branche2_1 = 300126;										// Frappe sanglante 300126 
			uint32 Spell_branche2_2 = 300127;										// Frappe sauvage 300127
			uint32 Spell_branche2_3 = 300176;										// Enchaînement Sanglant 300128, Enchaînement Sanglant amélioré 300176
			uint32 Spell_branche2_4 = 300174;										// Griffure sanglante 300130/300174 (version talent)
			
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
				case 1: // Si Voleur ------------------------------------------------------------------------------------------------------------------------------
					me->LoadEquipment(1, true);													// creature_equip_template 1

					// Tirages aléatoires des spells  
					Spell_branche1_agro = branche1_agro[urand(0, 2)];
					Spell_branche1_1 = branche1_1[urand(0, 1)];
					Spell_branche1_2 = branche1_2[urand(0, 1)];
					Spell_branche1_3 = branche1_3[urand(0, 4)];
					
					Buf_branche1 = Liste_Buf_branche1[urand(0, 4)];
					me->CastSpell(me, Buf_branche1, true);
					Buf_branche1a = Liste_Buf_branche1a[urand(0, 2)];							// Poisons
					me->CastSpell(me, Buf_branche1a, true);

					//VisuelPowerEnergy();
					break;

				case 2: // Si Vampire Berserker -------------------------------------------------------------------------------------------------------------------
					DoCastAOE(Vampire_Berserker, true);
					me->CastSpell(me, Buf_branche2, true);										// Buf2 sur lui meme
					Buf_branche2a = Liste_Buf_branche2a[urand(0, 2)];							// Poisons
					me->CastSpell(me, Buf_branche2a, true);
					me->LoadEquipment(2, true);													// creature_equip_template 2

					VisuelPowerDemonicFury();
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
				me->RemoveAura(Vampire_Berserker);

				Bonus_Armure(100);														// Retire bonus d'armure
			}
			void JustReachedHome() override
			{
				me->RemoveAura(Buf_branche1);
				me->RemoveAura(Buf_branche1a);
				me->RemoveAura(Buf_branche2);
				me->RemoveAura(Buf_branche2a);
				me->RemoveAura(Vampire_Berserker);

				Bonus_Armure(100);
				VisuelPowerEnergy();

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
						case 1: // Si Voleur ----------------------------------------------------------------------------------------------------------------------
							Mana = me->GetPower(POWER_ENERGY);

							Random = urand(1, 2); 
							if (Random == 1 && UpdateVictim()) { me->CastSpell(victim, Spell_branche1_agro, true); }		// 1/2 Chance de lancer le sort d'agro

							//Bonus_Armure(125);															// Bonus d'armure +25%
							break;

						case 2: // Si Vampire Berserker -----------------------------------------------------------------------------------------------------------
							Mana = me->GetPower(POWER_DEMONIC_FURY);

							Random = urand(1, 2);
							if (Random == 1 && UpdateVictim()) { DoCastVictim(Spell_branche2_agro); }		// 1/2 Chance de lancer le sort d'agro

							//Bonus_Armure(125);															// Bonus d'armure +25%
							break;

							// ####################################################################################################################################
						}
					}


					// ############################################################################################################################################
					// Combat suivant la Spécialisation


					switch (BrancheSpe)
					{
					case 1: // Spécialisation Voleur ##############################################################################################################
							// Regen en combat --------------------------------------------------------------------------------------------------------------------
						Mana = me->GetPower(POWER_ENERGY);
						if (Cooldown_RegenMana <= diff)
						{
							me->SetPower(POWER_ENERGY, Mana + (MaxMana / 10));
							if (Mana > MaxMana) { me->SetPower(POWER_ENERGY, MaxMana); }
							Cooldown_RegenMana = 2000;
						}
						else Cooldown_RegenMana -= diff;

						// Combat ---------------------------------------------------------------------------------------------------------------------------------
						// Spell1 sur la cible
						if (Cooldown_Spell1 <= diff)
						{
							Bonus_Degat_Arme_Done(-40);													// Reduction des degats infligés
							me->CastSpell(victim, Spell_branche1_1, true);
							Bonus_Degat_Arme_Done(40);
							DoMeleeAttackIfReady();														// Combat en mélée
							Cooldown_Spell1 = 1500;
						}
						else Cooldown_Spell1 -= diff;

						// Spell2 sur la cible
						if (Cooldown_Spell2 <= diff)
						{
							Bonus_Degat_Arme_Done(-25);//
							me->CastSpell(victim, Spell_branche1_2, true);
							Cooldown_Spell2 = urand(3000, 5000);
							Bonus_Degat_Arme_Done(25);//
						}
						else Cooldown_Spell2 -= diff;

						// Spell3 sur la cible 
						if (Cooldown_Spell3 <= diff)
						{
							Bonus_Degat_Arme_Done(-25);//
							me->CastSpell(victim, Spell_branche1_3, true);
							Cooldown_Spell3 = urand(8000, 12000);
							Bonus_Degat_Arme_Done(25);//
						}
						else Cooldown_Spell3 -= diff;

						Heal_En_Combat_Melee(diff);
						break;

					case 2: // Spécialisation Vampire Berserker ###################################################################################################
							// Regen en combat --------------------------------------------------------------------------------------------------------------------
						Mana = me->GetPower(POWER_DEMONIC_FURY);
						if (Cooldown_RegenMana <= diff)
						{
							me->SetPower(POWER_DEMONIC_FURY, Mana + (MaxMana / 10));
							if (Mana > MaxMana) { me->SetPower(POWER_DEMONIC_FURY, MaxMana); }
							Cooldown_RegenMana = 2000;
						}
						else Cooldown_RegenMana -= diff;

						// Combat ---------------------------------------------------------------------------------------------------------------------------------
						// Spell1 sur la cible
						if (Cooldown_Spell1 <= diff)
						{
							Bonus_Degat_Arme_Done(-40);													// Reduction des degats infligés
							me->CastSpell(victim, Spell_branche2_1, true);
							Bonus_Degat_Arme_Done(40);	
							DoMeleeAttackIfReady();														// Combat en mélée
							Cooldown_Spell1 = 4500;
						}
						else Cooldown_Spell1 -= diff;

						// Spell2 sur la cible
						if (Cooldown_Spell2 <= diff)
						{
							Bonus_Degat_Arme_Done(-25);//
							me->CastSpell(victim, Spell_branche2_2, true);
							Cooldown_Spell2 = 4500;
							Bonus_Degat_Arme_Done(25);//
						}
						else Cooldown_Spell2 -= diff;

						// Spell3 sur la cible
						if (Cooldown_Spell3 <= diff)
						{
							Bonus_Degat_Arme_Done(-25);//
							me->CastSpell(victim, Spell_branche2_3, true);
							Cooldown_Spell3 = 4500;
							Bonus_Degat_Arme_Done(25);//
						}
						else Cooldown_Spell3 -= diff;

						// Spell4 sur la cible
						if (Cooldown_Spell4 <= diff)
						{
							Bonus_Degat_Arme_Done(-25);//
							me->CastSpell(victim, Spell_branche2_4, true);
							Cooldown_Spell4 = 9000;
							Bonus_Degat_Arme_Done(25);//
						}
						else Cooldown_Spell4 -= diff;

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


				if (BrancheSpe == 1) 
				{
					Mana = me->GetPower(POWER_ENERGY); 
				} 
				else Mana = me->GetPower(POWER_DEMONIC_FURY);

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				//DoMeleeAttackIfReady();													// Combat en mélée

				// Si la cible >= 8m (pour éviter bug de rester figé) --------------------------------------------------------------------------------------------
				if (Cooldown_Anti_Bug_Figer <= diff)
				{
					if (Dist >= 8)
					{
					float x, y, z, mapid;
					x = (victim->GetPositionX() + urand(0, 2) - 1);
					y = (victim->GetPositionY() + urand(0, 2) - 1);
					z = victim->GetPositionZ();
					mapid = victim->GetMapId();
					me->GetMotionMaster()->MovePoint(mapid, x, y, z);
					me->GetMotionMaster()->MoveChase(victim, 1, frand(0, 6.2836f));			// Pour suivre la cible avec un angle
					Cooldown_Anti_Bug_Figer = 1000;
				}
				}
				else Cooldown_Anti_Bug_Figer -= diff;

				// Si la cible est entre 10 & 25m 
				if (Cooldown_Lancer <= diff)
				{
					Random = urand(1, 2);
					if ((Dist >= 10) && (Dist <= 25))
					{
						if (Random = 1)
						{
							//DoCastVictim(Spell_Lancer);									// Lancer une dague - 1 chance sur 2    
							me->CastSpell(victim, Spell_Lancer, true);						// Lancer une dague - 1 chance sur 2  
						}
						Cooldown_Lancer = urand(4000, 6000);
					}
				}
				else Cooldown_Lancer -= diff;

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
					Cooldown_ResteADistance = urand(3000, 5000);
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
				x = (victim->GetPositionX() + urand(0, 2) - 1);
				y = (victim->GetPositionY() + urand(0, 2) - 1);
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
				if (Cooldown_Spell_Heal <= diff)
				{
					Random = urand(1, 2);

					if ( (me->GetHealth() < (me->GetMaxHealth()*0.40)) && Random == 1)								// Si PV < 40%
					{
						DoCast(me, Spell_Heal1);
						Cooldown_Spell_Heal = 60000;
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
				// +- Bonus d'armure 100% = pas de bonus/malus   ex : Bonus_Armure(115); // Bonus d'armure +15%
				me->SetModifierValue(UNIT_MOD_ARMOR, BASE_VALUE, me->GetArmor() * (val / 100));
				me->SetCanModifyStats(true);
				me->UpdateAllStats();
			}
			
			void VisuelPowerEnergy()
		{
				me->setPowerType(POWER_ENERGY);
				me->SetMaxPower(POWER_ENERGY, 100);
				me->SetPower(POWER_ENERGY, 100);
		}
			void VisuelPowerDemonicFury()
		{
				me->SetMaxPower(POWER_ENERGY, 100);
				me->setPowerType(POWER_DEMONIC_FURY);
				me->SetMaxPower(POWER_DEMONIC_FURY, 100);
				me->SetPower(POWER_DEMONIC_FURY, 100);
		}

		};
		CreatureAI* GetAI(Creature* creature) const override
		{
			return new Stitch_npc_ai_voleurAI(creature);
		}
};

//#################################################################################################################################################################
void AddSC_npc_ai_voleur()
{
	new Stitch_npc_ai_voleur();
}















