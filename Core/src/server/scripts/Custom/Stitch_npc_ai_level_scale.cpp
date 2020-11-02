//################################################################################################################################################################################################
//  Copyright (C) Novembre 2020 Stitch pour Aquayoup
// npc a l'agro s'adapte au level de l'agresseur + AI generique
// Ces mobs utilisent les sorts definit dans Creature_Template->spellx
// Il est possible de modifier le temp entre 2 cast avec `BaseAttackTime` & `RangeAttackTime` 
// Ajustez les mélée avec UPDATE `creature_template` SET `ArmorModifier` = 2,`DamageModifier` = 2 WHERE (entry = 15000317);
//
// Afin d'activer l'agro des lvl gris Régler worldserver.conf : NoGrayAggro.Above = 0 et NoGrayAggro.Below = 0
//
// Spell1   : Buff a l'agro
// Spell2   : Sort sur la cible a l'agro
// Spell3   : Sort sur la cible pendant le combat
// Spell4   : Sort Dot sur la cible pendant le combat
// Spell5   : Heal sur caster et Allié
// Caster :
// Druide   : UPDATE `creature_template` SET `spell1` = 1126,`spell2` = 2912,`spell3` = 119577, `spell4` = 15798,`HealthModifier` = 2, `ManaModifier` = 2, `ArmorModifier` = 1, `DamageModifier` = 2,`BaseAttackTime` = 2500, `RangeAttackTime` = 3000,`lootid` = 0,`ScriptName` = 'Stitch_npc_ai_level_scale_caster' WHERE (entry = 15000319);
// Mélee :
// Guerrier : UPDATE `creature_template` SET `spell1` = 2457,`spell2` = 32323,`spell3` = 29426, `spell4` = 36991,`HealthModifier` = 2, `ManaModifier` = 1,`ArmorModifier` = 2,`DamageModifier` = 2,`BaseAttackTime` = 2500, `RangeAttackTime` = 3000,`lootid` = 0,`ScriptName` = 'Stitch_npc_ai_level_scale_melee' WHERE (entry = 15000317);
// Heal :
// Chaman   : UPDATE `creature_template` SET `HealthModifier` = 2, `ManaModifier` = 3, `ArmorModifier` = 1, `DamageModifier` = 2,`lootid` = 0,`ScriptName` = 'Stitch_npc_ai_level_scale_heal',`AIName` = '' WHERE (entry = 15000320);
//            UPDATE `creature_template` SET `spell1` = 974, `spell2` = 57994, `spell3` = 9532, `spell4` = 8056, `spell5` = 77472, `BaseAttackTime` = 2000, `RangeAttackTime` = 2000 WHERE(entry = 15000320);
//################################################################################################################################################################################################


//################################################################################################
//Stitch Stitch_npc_ai_level_scale_caster - A l'agro s'adapte au level de l'agresseur - AI Caster
//################################################################################################


class Stitch_npc_ai_level_scale_caster : public CreatureScript
{
public: Stitch_npc_ai_level_scale_caster() : CreatureScript("Stitch_npc_ai_level_scale_caster") { }

		struct Stitch_npc_ai_level_scale_casterAI : public ScriptedAI
		{
			Stitch_npc_ai_level_scale_casterAI(Creature* creature) : ScriptedAI(creature) { }

			uint32 spellbuf = me->m_spells[0];
			uint32 spellagro = me->m_spells[1];
			uint32 spellagrornd;
			uint32 distancedecast = 20.0f;
			uint32 spell = me->m_spells[2];
			uint32 spelldot = me->m_spells[3];
			uint32 spelltimerdot = 3000;
			uint32 spelltimer = 100;
			uint32 resteadistancetimer = urand(400, 600);
			Unit* victim = me->GetVictim();
			uint32 Cooldown_RegenMana = 3000;
			uint32 Mana;
			uint32 MaxMana = me->GetMaxPower(POWER_MANA);

			void JustRespawned() override
			{
				initlevelmob();
			}
			void EnterCombat(Unit* /*who*/) override
			{
				if (!UpdateVictim())
					return;

				agrolevelmob();
				Unit* victim = me->GetVictim();
				uint32 _level = victim->getLevel();

				me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);
				AttackStartCaster(victim, 20.0f);							// Distance de cast
				mouvementmob(6);

				// Spell a lancer a l'agro ---------------------------------------------------------------------------------------------------------------------
				me->CastSpell(me, spellbuf, true);							// Creature_Template->Spell1 : Buf sur soi
				spellagrornd = urand(0, 100);								// Creature_Template->Spell2 : 30% spell d'agro sur la cible
				if (spellagrornd <30)
				{
				DoSpellAttackIfReady(spellagro);
				}

			}
			void JustReachedHome() override
			{
				initlevelmob();
				me->SetReactState(REACT_AGGRESSIVE);
			}
			void EnterEvadeMode(EvadeReason /*why*/) override
			{
				me->GetMotionMaster()->MoveTargetedHome();						// Retour home
				retirebugdecombat();
			}
			void UpdateAI(uint32 diff) override
			{
				if (me->IsAlive() && !me->IsInCombat() /*&& !UpdateVictim()*/ && !me->HasUnitState(UNIT_STATE_MOVE) && (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > 15))
				{
					EnterEvadeMode(EVADE_REASON_SEQUENCE_BREAK);
				}

				if (!UpdateVictim() || me->isPossessed() || me->IsCharmed() || me->HasAuraType(SPELL_AURA_MOD_FEAR))
					return;

				mouvementmob(6);

				Mana = me->GetPower(POWER_MANA);
				MaxMana = me->GetMaxPower(POWER_MANA);

				// Regen mana en combat ------------------------------------------------------------------------------------------------------------------------
				if (Cooldown_RegenMana <= diff)
				{
					me->SetPower(POWER_MANA, Mana + (MaxMana / 20));
					if (Mana > MaxMana) { me->SetPower(POWER_MANA, MaxMana); }
					Cooldown_RegenMana = 1000;
				}
				else Cooldown_RegenMana -= diff;

					// Creature_Template->Spell3 : spell  sur la cible chaque 2.5 a 3s ---------------------------------------------------------------------
					if (spelltimer <= diff)
					{
						DoSpellAttackIfReady(spell);
						spelltimer = urand(2500, 3000);
					}
					else spelltimer -= diff;

					// Creature_Template->Spell4 : spell Dot sur la cible chaque 10-15s -----------------------------------------------------------------------
					if (spelltimerdot <= diff)
					{
						me->CastSpell(victim, spelldot, true);
						spelltimerdot = urand(10000, 15000);
					}
					else spelltimerdot -= diff;
			}

			void initlevelmob()
			{
				me->SelectLevel();
				me->SetMaxPower(POWER_MANA, me->GetMaxPower(POWER_MANA));
				//me->SetLevel(levelscaledefaut);	// repasse au lvl par defaut pour l'agro suivante
			}
			void agrolevelmob()
			{
				// Prend le lvl de la cible ----------------------------------------------------------------------------------------------------------------
				Unit* victim = me->GetVictim();
				uint32 _level = victim->getLevel();
				me->SetLevel(_level);

				// Rafraichissement des Stats --------------------------------------------------------------------------------------------------------------
				CreatureTemplate const* cInfo = me->GetCreatureTemplate();
				CreatureBaseStats const* stats = sObjectMgr->GetCreatureBaseStats(_level, 8); // Guerrier=1,Paladin=2,Voleur=4,Mage/Moine=8

				CreatureTemplate const* cinfo = me->GetCreatureTemplate();
				ASSERT(cinfo);
				me->SetLevel(_level);
				float basedamage = stats->GenerateBaseDamage(cInfo);
				float weaponBaseMinDamage = basedamage;
				float weaponBaseMaxDamage = basedamage * 1.5f;
				me->SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, weaponBaseMinDamage);
				me->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, weaponBaseMaxDamage);
				me->SetBaseWeaponDamage(OFF_ATTACK, MINDAMAGE, weaponBaseMinDamage);
				me->SetBaseWeaponDamage(OFF_ATTACK, MAXDAMAGE, weaponBaseMaxDamage);
				me->SetBaseWeaponDamage(RANGED_ATTACK, MINDAMAGE, weaponBaseMinDamage);
				me->SetBaseWeaponDamage(RANGED_ATTACK, MAXDAMAGE, weaponBaseMaxDamage);
				me->SetModifierValue(UNIT_MOD_ATTACK_POWER, BASE_VALUE, stats->AttackPower);
				me->SetModifierValue(UNIT_MOD_ATTACK_POWER_RANGED, BASE_VALUE, stats->RangedAttackPower);
				me->UpdateAllStats();

				uint32 health = stats->GenerateHealth(cInfo);
				me->SetCreateHealth(health);
				me->SetMaxHealth(health);
				uint32 mana = stats->GenerateMana(cInfo);
				me->SetCreateMana(mana);
				me->SetMaxPower(POWER_MANA, mana);
				me->SetPower(POWER_MANA, MaxMana);
			}
			void mouvementmob(uint32 diff)
			{
				// Mouvement off si Mana > 5% ou distance < 20m ---------------------------------------------------------------------------------------------
				if ((me->GetPower(POWER_MANA) > me->GetMaxPower(POWER_MANA) / 20) && (me->IsWithinCombatRange(me->GetVictim(), 20.0f)))
				{
					AttackStartCaster(me->GetVictim(), 20.0f);						// Distance de cast
					void DoRangedAttackIfReady();									// Combat a distance
					me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);			// ROOT
				}

				// Mouvement on si Mana < 5% ou distance > 20m ----------------------------------------------------------------------------------------------
				if ((me->GetPower(POWER_MANA) < me->GetMaxPower(POWER_MANA) / 20) | (!me->IsWithinCombatRange(me->GetVictim(), 20.0f)))
				{
					AttackStart(me->GetVictim());									// Combat au corp a corp
					AttackStartCaster(me->GetVictim(), 20.0f);						// Distance de cast
					DoMeleeAttackIfReady();
					me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);		//UNROOT
				}

				// Mouvement aléatoire si cible < 6m -------------------------------------------------------------------------------------------------------
				if ((resteadistancetimer <= diff))
				{
					if (me->IsWithinCombatRange(me->GetVictim(), 6.0f) && (me->GetPower(POWER_MANA) > me->GetMaxPower(POWER_MANA) / 20))
					{
						float x, y, z;
						me->GetClosePoint(x, y, z, me->GetObjectSize() / 3, 15.0f);
						me->GetMotionMaster()->MovePoint(0xFFFFFE, x, y, z);
					}
					resteadistancetimer = urand(300, 500);
				}
				else resteadistancetimer -= diff;

				// Quite le combat si la cible > 30m ou >40m de home----------------------------------------------------------------------------------------
				if ((!me->IsWithinCombatRange(me->GetVictim(), 30.0f)) || (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > 40))
				{
					me->AddUnitState(UNIT_STATE_EVADE);
					EnterEvadeMode(EVADE_REASON_SEQUENCE_BREAK);					// Quite le combat si la cible > 30m ou > 40m de home
					me->GetMotionMaster()->MoveTargetedHome();						// Retour home
					retirebugdecombat();
				}

			}
			void retirebugdecombat()
			{
				me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);		// UNROOT
				me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);			// Retire flag "en combat"
				me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);		// Retire flag "non attaquable"

				me->AddUnitState(UNIT_STATE_EVADE);
				me->DeleteThreatList();
				me->CombatStop(true);
				me->LoadCreaturesAddon();
				me->SetLootRecipient(NULL);
				me->ResetPlayerDamageReq();
				me->SetLastDamagedTime(0);
			}
		};

		CreatureAI* GetAI(Creature* creature) const override
		{
			return new Stitch_npc_ai_level_scale_casterAI(creature);
		}
}; 

//################################################################################################
//Stitch Stitch_npc_ai_level_scale_melee - A l'agro s'adapte au level de l'agresseur - AI Melee
//################################################################################################


class Stitch_npc_ai_level_scale_melee : public CreatureScript
{
public: Stitch_npc_ai_level_scale_melee() : CreatureScript("Stitch_npc_ai_level_scale_melee") { }

		struct Stitch_npc_ai_level_scale_meleeAI : public ScriptedAI
		{
			Stitch_npc_ai_level_scale_meleeAI(Creature* creature) : ScriptedAI(creature) { }

			uint32 spellbuf = me->m_spells[0];
			uint32 spellagro = me->m_spells[1];
			uint32 spellagrornd;
			uint32 spell = me->m_spells[2];
			uint32 spelldot = me->m_spells[3];
			uint32 spelltimerdot = 3000;
			uint32 spelltimer = 100;
			uint32 Dist;

			Unit* victim = me->GetVictim();



			void JustRespawned()  override
			{
				initlevelmob();
			}
			void EnterCombat(Unit* /*who*/) override
			{
				if (!UpdateVictim())
					return;

				agrolevelmob();
				me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);
				DoMeleeAttackIfReady();								// Combat en Melee

				// Spell a lancer a l'agro ------------------------------------------------------------------------------------------------------------------------------
				me->CastSpell(me, spellbuf, true);					// Creature_Template->Spell1 : Buf sur soi
				spellagrornd = urand(0, 100);						// Creature_Template->Spell2 : 40% spell d'agro sur la cible
				if (spellagrornd <40)
				{
					DoSpellAttackIfReady(spellagro);
				}
			}
			void JustReachedHome() override
			{
				initlevelmob();
				me->SetReactState(REACT_AGGRESSIVE);
			}
			void EnterEvadeMode(EvadeReason /*why*/) override
			{
				me->GetMotionMaster()->MoveTargetedHome();						// Retour home
				retirebugdecombat();
			}
			void UpdateAI(uint32 diff) override
			{
				if (me->IsAlive() && !me->IsInCombat() /*&& !UpdateVictim()*/ && !me->HasUnitState(UNIT_STATE_MOVE) && (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > 15)) 
				{
					EnterEvadeMode(EVADE_REASON_SEQUENCE_BREAK);
				}

				if (!UpdateVictim() || me->isPossessed() || me->IsCharmed() || me->HasAuraType(SPELL_AURA_MOD_FEAR))
					return;


				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);


				if (me->IsWithinCombatRange(me->GetVictim(), 6.0f)) // Uniquement si au contact
				{
				// Creature_Template->Spell3 : spell  sur la cible chaque 2 a 2.5s ----------------------------------------------------------------------------------------------
				if (spelltimer <= diff)
				{
					me->CastSpell(victim, spell, true);
					spelltimer = urand(2000, 2500);
				}
				else spelltimer -= diff;


				// Creature_Template->Spell4 : spell Dot sur la cible chaque 10-15s ---------------------------------------------------------------------------------------------
				if (spelltimerdot <= diff)
				{
					me->CastSpell(victim, spelldot, true);
					spelltimerdot = urand(10000, 15000);
				}
				else spelltimerdot -= diff;
				}

				// Quite le combat si la cible > 40m -----------------------------------------------------------------------------------------------------------------------------
				if ((!me->IsWithinCombatRange(me->GetVictim(), 40.0f)))
				{
					retirebugdecombat();
				}

				// Quite le combat si la cible > 30m ou >40m de home----------------------------------------------------------------------------------------
				if ( ((Dist > 30) || (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > 40)) && !me->HasUnitState(UNIT_STATE_MOVE))
				{
					me->AddUnitState(UNIT_STATE_EVADE);
					EnterEvadeMode(EVADE_REASON_SEQUENCE_BREAK);					// Quite le combat si la cible > 30m ou > 40m de home
					me->GetMotionMaster()->MoveTargetedHome();						// Retour home
					retirebugdecombat();
				}

			}

			void initlevelmob()
			{
					me->SelectLevel();
			}
			void agrolevelmob()
			{
				// Prend le lvl de la cible ----------------------------------------------------------------------------------------------------------------
				Unit* victim = me->GetVictim();
				uint32 _level = victim->getLevel();
				me->SetLevel(_level);

				// Rafraichissement des Stats --------------------------------------------------------------------------------------------------------------
				CreatureTemplate const* cInfo = me->GetCreatureTemplate();
				CreatureBaseStats const* stats = sObjectMgr->GetCreatureBaseStats(_level, 8); // Guerrier=1,Paladin=2,Voleur=4,Mage/Moine=8

				CreatureTemplate const* cinfo = me->GetCreatureTemplate();
				ASSERT(cinfo);
				me->SetLevel(_level);
				float basedamage = stats->GenerateBaseDamage(cInfo);
				float weaponBaseMinDamage = basedamage;
				float weaponBaseMaxDamage = basedamage * 1.5f;
				me->SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, weaponBaseMinDamage);
				me->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, weaponBaseMaxDamage);
				me->SetBaseWeaponDamage(OFF_ATTACK, MINDAMAGE, weaponBaseMinDamage);
				me->SetBaseWeaponDamage(OFF_ATTACK, MAXDAMAGE, weaponBaseMaxDamage);
				me->SetBaseWeaponDamage(RANGED_ATTACK, MINDAMAGE, weaponBaseMinDamage);
				me->SetBaseWeaponDamage(RANGED_ATTACK, MAXDAMAGE, weaponBaseMaxDamage);
				me->SetModifierValue(UNIT_MOD_ATTACK_POWER, BASE_VALUE, stats->AttackPower);
				me->SetModifierValue(UNIT_MOD_ATTACK_POWER_RANGED, BASE_VALUE, stats->RangedAttackPower);
				me->UpdateAllStats();

				uint32 health = stats->GenerateHealth(cInfo);
				me->SetCreateHealth(health);
				me->SetMaxHealth(health);
			}
			void retirebugdecombat()
			{
				me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);		// UNROOT
				me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);			// Retire flag "en combat"
				me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);		// Retire flag "non attaquable"

				me->AddUnitState(UNIT_STATE_EVADE);
				me->DeleteThreatList();
				me->CombatStop(true);
				me->LoadCreaturesAddon();
				me->SetLootRecipient(NULL);
				me->ResetPlayerDamageReq();
				me->SetLastDamagedTime(0);
			}

		};

		CreatureAI* GetAI(Creature* creature) const override
		{
			return new Stitch_npc_ai_level_scale_meleeAI(creature);
		}
};

//################################################################################################
//Stitch Stitch_npc_ai_level_scale_heal - A l'agro s'adapte au level de l'agresseur - AI heal
//################################################################################################

class Stitch_npc_ai_level_scale_heal : public CreatureScript
{
public: Stitch_npc_ai_level_scale_heal() : CreatureScript("Stitch_npc_ai_level_scale_heal") { }

		struct Stitch_npc_ai_level_scale_healAI : public ScriptedAI
		{
			Stitch_npc_ai_level_scale_healAI(Creature* creature) : ScriptedAI(creature) { }

			uint32 spellbuf = me->m_spells[0];
			uint32 spellagro = me->m_spells[1];
			uint32 spellagrornd;
			uint32 distancedecast = 30.0f;
			uint32 spell = me->m_spells[2];
			uint32 spelldot = me->m_spells[3];
			uint32 resteadistancetimer = urand(400, 800);
			Unit* victim = me->GetVictim();
			uint32 spellheal = me->m_spells[4];
			uint32 spelltimerheal = 3000;
			uint32 spelltimer = 100;
			uint32 spelltimerdot = 3000;
			uint32 spelltimerMana = 2000;

			void JustRespawned() override
			{
				initlevelmob();
			}
			void EnterCombat(Unit* /*who*/) override
			{
				if (!UpdateVictim())
					return;

				agrolevelmob();
				Unit* victim = me->GetVictim();
				uint32 _level = victim->getLevel();

				AttackStartCaster(victim, 20.0f);							// Distance de cast
				mouvementmob(6);
				me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);

				// Spell a lancer a l'agro ---------------------------------------------------------------------------------------------------------------------
				me->CastSpell(me, spellbuf, true);							// Creature_Template->Spell1 : Buf sur soi
				spellagrornd = urand(0, 100);								// Creature_Template->Spell2 : 30% spell d'agro sur la cible
				if (spellagrornd < 30)
				{
					DoSpellAttackIfReady(spellagro);
				}
			}
			void JustReachedHome() override
			{
				initlevelmob();
				me->SetReactState(REACT_AGGRESSIVE);
			}
			void EnterEvadeMode(EvadeReason /*why*/) override
			{
				me->GetMotionMaster()->MoveTargetedHome();						// Retour home
				retirebugdecombat();
			}

			void UpdateAI(uint32 diff) override
			{
				if (me->IsAlive() && !me->IsInCombat() /*&& !UpdateVictim()*/ && !me->HasUnitState(UNIT_STATE_MOVE) && (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > 15)) 
				{
					EnterEvadeMode(EVADE_REASON_SEQUENCE_BREAK);
				}

				if (!UpdateVictim() || me->isPossessed() || me->IsCharmed() || me->HasAuraType(SPELL_AURA_MOD_FEAR))
					return;

				mouvementmob(6);

				// Regen mana en combat -------------------------------------------------------------------------------------------------------------------------
				if (spelltimerMana <= diff)
				{
					uint32 _Mana = me->GetPower(POWER_MANA);
					int32 _MaxMana = me->GetMaxPower(POWER_MANA);
					me->SetPower(POWER_MANA, _Mana + (_MaxMana / 5));
					if (me->GetPower(POWER_MANA) > _MaxMana) { me->SetPower(POWER_MANA, _MaxMana); }
					spelltimerMana = 4000;
				} else spelltimerMana -= diff;

				// Cast & Heal en combat -----------------------------------------------------------------------------------------------------------------------
				if (spelltimerheal <= diff)
				{

					Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, distancedecast, true); // pour heal friend

					// Creature_Template->Spell5 : heal sur lui meme -------------------------------------------------------------------------------------------
					if (me->GetHealth() < (me->GetMaxHealth()*0.75))
					{
						//DoSpellAttackIfReady(spellheal);
						DoCast(me, spellheal);
						spelltimerheal = 2000;
					}
					// Creature_Template->Spell5 : heal sur Friend --------------------------------------------------------------------------------------------------
					else if (Unit* target = DoSelectLowestHpFriendly(distancedecast)) // Distance de l'allié = 30m
					{
						if (me->IsFriendlyTo(target))
						{
							if (target->GetHealth() < (target->GetMaxHealth()*0.75))
							{
								DoCast(target, spellheal);
								spelltimerheal = 2000;
							}
						}
					}
				} else spelltimerheal -= diff;

				// Creature_Template->Spell3 : spell  sur la cible chaque ----------------------------------------------------------------------------------
				if (spelltimer <= diff)
				{
					DoSpellAttackIfReady(spell);
					spelltimer = 2000;
				} else spelltimer -= diff;


				// Creature_Template->Spell4 : spell Dot sur la cible --------------------------------------------------------------------------------------
				if (spelltimerdot <= diff)
				{
					me->CastSpell(victim, spelldot, true);
					spelltimerdot = urand(10000, 15000);
				} else spelltimerdot -= diff;

				// Quite le combat si la cible > 30m ou >40m de home----------------------------------------------------------------------------------------
				if ((!me->IsWithinCombatRange(me->GetVictim(), 30.0f)) || (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > 40))
				{
					me->AddUnitState(UNIT_STATE_EVADE);
					EnterEvadeMode(EVADE_REASON_SEQUENCE_BREAK);					// Quite le combat si la cible > 30m ou > 40m de home
					me->GetMotionMaster()->MoveTargetedHome();						// Retour home
					retirebugdecombat();
				}

			}

			void initlevelmob()
			{
				me->SelectLevel();
				me->SetMaxPower(POWER_MANA, me->GetMaxPower(POWER_MANA));
			}
			void agrolevelmob()
			{
				// Prend le lvl de la cible ----------------------------------------------------------------------------------------------------------------
				Unit* victim = me->GetVictim();
				uint32 _level = victim->getLevel();
				me->SetLevel(_level);

				// Rafraichissement des Stats --------------------------------------------------------------------------------------------------------------
				CreatureTemplate const* cInfo = me->GetCreatureTemplate();
				CreatureBaseStats const* stats = sObjectMgr->GetCreatureBaseStats(_level, 8); // Guerrier=1,Paladin=2,Voleur=4,Mage/Moine=8

				CreatureTemplate const* cinfo = me->GetCreatureTemplate();
				ASSERT(cinfo);
				me->SetLevel(_level);
				float basedamage = stats->GenerateBaseDamage(cInfo);
				float weaponBaseMinDamage = basedamage;
				float weaponBaseMaxDamage = basedamage * 1.5f;
				me->SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, weaponBaseMinDamage);
				me->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, weaponBaseMaxDamage);
				me->SetBaseWeaponDamage(OFF_ATTACK, MINDAMAGE, weaponBaseMinDamage);
				me->SetBaseWeaponDamage(OFF_ATTACK, MAXDAMAGE, weaponBaseMaxDamage);
				me->SetBaseWeaponDamage(RANGED_ATTACK, MINDAMAGE, weaponBaseMinDamage);
				me->SetBaseWeaponDamage(RANGED_ATTACK, MAXDAMAGE, weaponBaseMaxDamage);
				me->SetModifierValue(UNIT_MOD_ATTACK_POWER, BASE_VALUE, stats->AttackPower);
				me->SetModifierValue(UNIT_MOD_ATTACK_POWER_RANGED, BASE_VALUE, stats->RangedAttackPower);
				me->UpdateAllStats();

				uint32 health = stats->GenerateHealth(cInfo);
				me->SetCreateHealth(health);
				me->SetMaxHealth(health);
				uint32 mana = stats->GenerateMana(cInfo);
				me->SetCreateMana(mana);
				me->SetMaxPower(POWER_MANA, mana);
			}
			void mouvementmob(uint32 diff)
			{
				// Mouvement off si Mana > 5% ou distance < 20m ---------------------------------------------------------------------------------------------
				if ((me->GetPower(POWER_MANA) > me->GetMaxPower(POWER_MANA) / 20) && (me->IsWithinCombatRange(me->GetVictim(), 20.0f)))
				{
					AttackStartCaster(me->GetVictim(), 20.0f);						// Distance de cast
					void DoRangedAttackIfReady();									// Combat a distance
					me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);			// ROOT
				}

				// Mouvement on si Mana < 5% ou distance > 20m ----------------------------------------------------------------------------------------------
				if ((me->GetPower(POWER_MANA) < me->GetMaxPower(POWER_MANA) / 20) | (!me->IsWithinCombatRange(me->GetVictim(), 20.0f)))
				{
					AttackStart(me->GetVictim());									// Combat au corp a corp
					AttackStartCaster(me->GetVictim(), 20.0f);						// Distance de cast
					DoMeleeAttackIfReady();
					me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);		//UNROOT
				}

				// Mouvement aléatoire si cible < 6m -------------------------------------------------------------------------------------------------------
				if ((resteadistancetimer <= diff))
				{
					if (me->IsWithinCombatRange(me->GetVictim(), 6.0f) && (me->GetPower(POWER_MANA) > me->GetMaxPower(POWER_MANA) / 20))
					{

						float x, y, z;
						me->GetClosePoint(x, y, z, me->GetObjectSize() / 3, 15.0f);
						me->GetMotionMaster()->MovePoint(0xFFFFFE, x, y, z);
					}
					resteadistancetimer = urand(300, 500);
				} else resteadistancetimer -= diff;

				// Quite le combat si la cible > 30m ou >40m de home----------------------------------------------------------------------------------------
				if ((!me->IsWithinCombatRange(me->GetVictim(), distancedecast + 10.0f)) || (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > 40))
				{
					me->AddUnitState(UNIT_STATE_EVADE);
					EnterEvadeMode(EVADE_REASON_SEQUENCE_BREAK);					// Quite le combat si la cible > 30m ou > 40m de home
					me->GetMotionMaster()->MoveTargetedHome();						// Retour home
					retirebugdecombat();
				}
			}
			void retirebugdecombat()
			{
				me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);		// UNROOT
				me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);			// Retire flag "en combat"
				me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);		// Retire flag "non attaquable"

				me->AddUnitState(UNIT_STATE_EVADE);
				me->DeleteThreatList();
				me->CombatStop(true);
				me->LoadCreaturesAddon();
				me->SetLootRecipient(NULL);
				me->ResetPlayerDamageReq();
				me->SetLastDamagedTime(0);
			}
		};

		CreatureAI* GetAI(Creature* creature) const override
		{
			return new Stitch_npc_ai_level_scale_healAI(creature);
		}
};

//################################################################################################
void AddSC_Stitch_npc_ai_level_scale()
{
	new Stitch_npc_ai_level_scale_caster();					//Stitch npc_level_scale - s'adapte au level de l'agresseur +AI Caster - cast a distance
	new Stitch_npc_ai_level_scale_melee();					//Stitch npc_level_scale - s'adapte au level de l'agresseur +AI Melee - cast < 6m
	new Stitch_npc_ai_level_scale_heal();					//Stitch npc_level_scale - s'adapte au level de l'agresseur +AI Heal
}















