//################################################################################################
//Stitch npc_level_scale - A l'agro s'adapte au level de l'agresseur + AI generique
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
// Caster :
// UPDATE `creature_template` SET `spell1` = 1126,`spell2` = 2912,`spell3` = 119577, `spell4` = 15798,`BaseAttackTime` = 2500, `RangeAttackTime` = 3000,`ScriptName` = 'Stitch_npc_ai_level_scale_caster' WHERE (entry = 15000319);
// Mage     : UPDATE `creature_template` SET `spell1` = 165743, `spell2` = 31589, `spell3` = 9053, `spell4` = 31589,`BaseAttackTime` = 2500, `RangeAttackTime` = 3000, `ScriptName` = 'Stitch_npc_ai_level_scale_caster' WHERE(entry = 15000319);
// Demo     : UPDATE `creature_template` SET `spell1` = 79934, `spell2` = 348, `spell3` = 686, `spell4` = 172,`BaseAttackTime` = 2500, `RangeAttackTime` = 3000, `ScriptName` = 'Stitch_npc_ai_level_scale_caster' WHERE(entry = 15000319);
// Chaman   : UPDATE `creature_template` SET `spell1` = 324, `spell2` = 8056, `spell3` = 9532, `spell4` = 51505,`BaseAttackTime` = 2500, `RangeAttackTime` = 3000, `ScriptName` = 'Stitch_npc_ai_level_scale_caster' WHERE(entry = 15000319);
// Pretre   : UPDATE `creature_template` SET `spell1` = 15473, `spell2` = 2944, `spell3` = 15407, `spell4` = 48045,`BaseAttackTime` = 2500, `RangeAttackTime` = 3000, `ScriptName` = 'Stitch_npc_ai_level_scale_caster' WHERE(entry = 15000319);
// Mélee :
// UPDATE `creature_template` SET `spell1` = 109212,`spell2` = 133308,`spell3` = 78660, `spell4` = 31975,`ArmorModifier` = 2,`DamageModifier` = 2,`BaseAttackTime` = 2500, `RangeAttackTime` = 3000,`ScriptName` = 'Stitch_npc_ai_level_scale_melee' WHERE (entry = 15000318);
// UPDATE `creature_template` SET `spell1` = 2457,`spell2` = 32323,`spell3` = 29426, `spell4` = 36991,ArmorModifier` = 2,`DamageModifier` = 2,`BaseAttackTime` = 2500, `RangeAttackTime` = 3000,`ScriptName` = 'Stitch_npc_ai_level_scale_melee' WHERE (entry = 15000317);
// Heal :
//################################################################################################




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
			uint32 resteadistancetimer = urand(400, 800);
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
				Unit* victim = me->GetVictim();
				uint32 _level = victim->getLevel();

				// Distance de cast
				AttackStartCaster(victim, 20.0f);							// Distance de cast

				// Spell a lancer a l'agro
				mouvementmob(6);
				me->CastSpell(me, spellbuf, true);							// Creature_Template->Spell1 : Buf sur soi
				spellagrornd = urand(0, 100);								// Creature_Template->Spell2 : 30% spell d'agro sur la cible
				if (spellagrornd <30)
				{
				DoSpellAttackIfReady(spellagro);							
				}

				me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);		// ROOT 
			}
			void JustReachedHome() override
			{
				initlevelmob();
				retirebugdecombat();
			}
			void EnterEvadeMode(EvadeReason /*why*/) override
			{
				retirebugdecombat();
			}
			void UpdateAI(uint32 diff) override
			{
				if (!UpdateVictim())
					return;

				mouvementmob(6);
								
					// Creature_Template->Spell3 : spell  sur la cible chaque 2.5 a 3.5s					
					if (spelltimer <= diff)
					{
						DoSpellAttackIfReady(spell);
						spelltimer = urand(2500, 3500);
					}
					else spelltimer -= diff;


					// Creature_Template->Spell4 : spell Dot sur la cible chaque 10-15s
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
				/*
				me->SetLevel(levelscaledefaut);	// repasse au lvl par defaut pour l'agro suivante

				// Rafraichissement des Stats
				CreatureTemplate const* cInfo = me->GetCreatureTemplate();
				CreatureBaseStats const* stats = sObjectMgr->GetCreatureBaseStats(95, 8); // Guerrier=1,Paladin=2,Voleur=4,Mage/Moine=8
				uint32 basehp = stats->GenerateHealth(cInfo);
				uint32 health = uint32(basehp * 0.8);
				me->SetCreateHealth(health);
				me->SetMaxHealth(health);
				*/
			}
			void agrolevelmob()
			{
				// Prend le lvl de la cible
				Unit* victim = me->GetVictim();
				uint32 _level = victim->getLevel();
				me->SetLevel(_level);

				// Rafraichissement des Stats
				CreatureTemplate const* cInfo = me->GetCreatureTemplate();
				CreatureBaseStats const* stats = sObjectMgr->GetCreatureBaseStats(_level, 8); // Guerrier=1,Paladin=2,Voleur=4,Mage/Moine=8
				uint32 basehp = stats->GenerateHealth(cInfo);
				uint32 health = uint32(basehp*0.8);
				me->SetCreateHealth(health);
				me->SetMaxHealth(health);

			}
			void mouvementmob(uint32 diff)
			{
				// Mouvement off si Mana >5% ou distance > 20m
				if ((me->GetPower(POWER_MANA) > me->GetMaxPower(POWER_MANA) / 20) && (me->IsWithinCombatRange(me->GetVictim(), 20.0f)))
				{
					AttackStartCaster(me->GetVictim(), 20.0f);						// Distance de cast
					void DoRangedAttackIfReady();									// Combat a distance
					me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);			// ROOT
				}

				// Mouvement on si Mana <5% ou distance > 20m
				if ((me->GetPower(POWER_MANA) < me->GetMaxPower(POWER_MANA) / 20) | (!me->IsWithinCombatRange(me->GetVictim(), 20.0f)))
				{
					DoMeleeAttackIfReady();
					me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);		//UNROOT
				}

				// Mouvement aléatoire si cible < 6m
				if ((resteadistancetimer <= diff) )
				{
					if (me->IsWithinCombatRange(me->GetVictim(), 6.0f) && (me->GetPower(POWER_MANA) > me->GetMaxPower(POWER_MANA) / 20))
					{

						float x, y, z;
						me->GetClosePoint(x, y, z, me->GetObjectSize() / 3, 15.0f);
						me->GetMotionMaster()->MovePoint(0xFFFFFE, x, y, z);
					}
					resteadistancetimer = urand(400,800);	
				}
				else resteadistancetimer -= diff;

				// Quite le combat si la cible > 40m
				if ((!me->IsWithinCombatRange(me->GetVictim(), 40.0f)))
				{
					retirebugdecombat();
				}
			}
			void retirebugdecombat()
			{
				me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);		// UNROOT
				me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);			// Retire flag "en combat"
				me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);		// Retire flag "non attaquable"
				me->GetMotionMaster()->MoveTargetedHome();						// Retour home
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
				DoMeleeAttackIfReady();								// Combat en Melee

				// Spell a lancer a l'agro
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
				retirebugdecombat();
			}
			void EnterEvadeMode(EvadeReason /*why*/) override
			{
				retirebugdecombat();
			}
			void UpdateAI(uint32 diff) override
			{
				if (!UpdateVictim())
					return;
				if (me->IsWithinCombatRange(me->GetVictim(), 6.0f)) // Uniquement si au contact
				{
				// Creature_Template->Spell3 : spell  sur la cible chaque 2 a 2.5s					
				if (spelltimer <= diff)
				{
					//DoSpellAttackIfReady(spell);
					me->CastSpell(victim, spell, true);
					spelltimer = urand(2000, 2500);
				}
				else spelltimer -= diff;


				// Creature_Template->Spell4 : spell Dot sur la cible chaque 10-15s
				if (spelltimerdot <= diff)
				{
					me->CastSpell(victim, spelldot, true);
					spelltimerdot = urand(10000, 15000);
				}
				else spelltimerdot -= diff;
				}

				// Quite le combat si la cible > 40m
				if ((!me->IsWithinCombatRange(me->GetVictim(), 40.0f)))
				{
					retirebugdecombat();
				}
			}

			void initlevelmob()
			{
				me->SelectLevel();
			}
			void agrolevelmob()
			{
				// Prend le lvl de la cible
				Unit* victim = me->GetVictim();
				uint32 _level = victim->getLevel();
				me->SetLevel(_level);

				// Rafraichissement des Stats
				CreatureTemplate const* cInfo = me->GetCreatureTemplate();
				CreatureBaseStats const* stats = sObjectMgr->GetCreatureBaseStats(_level, 1); // Guerrier=1,Paladin=2,Voleur=4,Mage/Moine=8
				uint32 basehp = stats->GenerateHealth(cInfo);
				uint32 health = uint32(basehp);
				me->SetCreateHealth(health);
				me->SetMaxHealth(health);

			}
			void retirebugdecombat()
			{
				me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);		// UNROOT
				me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);			// Retire flag "en combat"
				me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);		// Retire flag "non attaquable"
				me->GetMotionMaster()->MoveTargetedHome();						// Retour home
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


//class Stitch_npc_ai_level_scale_heal : public CreatureScript



void AddSC_Stitch_npc_ai_level_scale()
{
	new Stitch_npc_ai_level_scale_caster();					//Stitch npc_level_scale - s'adapte au level de l'agresseur +AI Caster - cast a distance
	new Stitch_npc_ai_level_scale_melee();					//Stitch npc_level_scale - s'adapte au level de l'agresseur +AI Melee - cast < 6m
	//new Stitch_npc_ai_level_scale_heal();					//Stitch npc_level_scale - s'adapte au level de l'agresseur +AI Melee
}















