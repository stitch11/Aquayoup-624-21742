//################################################################################################
//Stitch npc_level_scale - A l'agro s'adapte au level de l'agresseur + AI generique
// Ces mobs utilisent les sorts definit dans Creature_Template->spellx
// Spell1 : Buff a l'agro
// Spell2 : Sort sur la cible a l'agro
// Spell3 : Sort sur la cible pendant le combat
// Spell4 : Sort Dot sur la cible pendant le combat
// Druide equilibre : UPDATE `creature_template` SET `spell1` = 1126,`spell2` = 2912,`spell3` = 119577, `spell4` = 8921,`ScriptName` = 'Stitch_npc_ai_level_scale_caster' WHERE (entry = 15000319);
// Mage : UPDATE `creature_template` SET `spell1` = 165743, `spell2` = 31589, `spell3` = 9053, `spell4` = 31589, `ScriptName` = 'Stitch_npc_ai_level_scale_caster' WHERE(entry = 15000319);
// Demo : UPDATE `creature_template` SET `spell1` = 79934, `spell2` = 348, `spell3` = 686, `spell4` = 172, `ScriptName` = 'Stitch_npc_ai_level_scale_caster' WHERE(entry = 15000319);
// Chasseur : UPDATE `creature_template` SET `spell1` = 1543, `spell2` = 145663, `spell3` = 171943, `spell4` = 31975, `ScriptName` = 'Stitch_npc_ai_level_scale_caster' WHERE(entry = 15000319);
// Chaman : UPDATE `creature_template` SET `spell1` = 324, `spell2` = 8056, `spell3` = 9532, `spell4` = 51505, `ScriptName` = 'Stitch_npc_ai_level_scale_caster' WHERE(entry = 15000319);
// Pretre : UPDATE `creature_template` SET `spell1` = 15473, `spell2` = 2944, `spell3` = 15407, `spell4` = 48045, `ScriptName` = 'Stitch_npc_ai_level_scale_caster' WHERE(entry = 15000319);
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

			uint32 spell = me->m_spells[2];
			uint32 spelldot = me->m_spells[3];
			uint32 spelltimerdot = 3000;

			void JustRespawned()  override
			{
				me->SetLevel(95);	// repasse au lvl 95 pour l'agro suivante

				// Rafraichissement de sa vie
				CreatureTemplate const* cInfo = me->GetCreatureTemplate();
				CreatureBaseStats const* stats = sObjectMgr->GetCreatureBaseStats(100, 1);
				uint32 basehp = stats->GenerateHealth(cInfo);
				uint32 health = uint32(basehp * 1.1);
				me->SetCreateHealth(health);
				me->SetMaxHealth(health);
			}
			void EnterCombat(Unit* /*who*/) override
			{
				Unit* victim = me->GetVictim();
				uint32 _level = victim->getLevel();
				me->SetLevel(_level);

				// Rafraichissement de sa vie
				CreatureTemplate const* cInfo = me->GetCreatureTemplate();
				CreatureBaseStats const* stats = sObjectMgr->GetCreatureBaseStats(_level, 1);
				uint32 basehp = stats->GenerateHealth(cInfo);
				uint32 health = uint32(basehp * 1.1);
				me->SetCreateHealth(health);
				me->SetMaxHealth(health);

				// Distance de cast
				uint32 m_attackDist = 20.0f;
				AttackStartCaster(victim, m_attackDist);

				// Spell a lancer a l'agro
				me->CastSpell(me, me->m_spells[0], true);		// Creature_Template->Spell1 : Buf sur soit
				me->CastSpell(victim, me->m_spells[1], true);   // Creature_Template->Spell2 : spell d'agro sur la cible
			}
			void JustReachedHome() override
			{
				me->SetLevel(95);	// repasse au lvl 95 pour l'agro suivante

				// Rafraichissement de sa vie
				CreatureTemplate const* cInfo = me->GetCreatureTemplate();
				CreatureBaseStats const* stats = sObjectMgr->GetCreatureBaseStats(100, 1);
				uint32 basehp = stats->GenerateHealth(cInfo);
				uint32 health = uint32(basehp * 1.1);
				me->SetCreateHealth(health);
				me->SetMaxHealth(health);
			}
			void UpdateAI(uint32 diff) override
			{
				if (!UpdateVictim())
					return;


				// Combat a distance si mana >5%
				if (me->GetPower(POWER_MANA) > me->GetMaxPower(POWER_MANA) / 20)
				{
					AttackStartCaster(me->GetVictim(), 20.0f);
					void DoRangedAttackIfReady();					// Combat a distance

					// Creature_Template->Spell4 : spell Dot sur la cible chaque 10-15s
					if (spelltimerdot <= diff)
					{
						if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
							//DoSpellAttackIfReady(spelldot);
							me->CastSpell(target, spelldot, true);
						spelltimerdot = urand(10000, 15000);
					}
					else
						spelltimerdot -= diff;


					// Creature_Template->Spell3 : spell sur la cible
					DoSpellAttackIfReady(spell);




				}
				// Combat au contact si mana <5 %
				if (me->GetPower(POWER_MANA) < me->GetMaxPower(POWER_MANA) / 20)
				{
					AttackStartCaster(me->GetVictim(), 4.0f);
					DoMeleeAttackIfReady();		
				}
			}
		};

		CreatureAI* GetAI(Creature* creature) const override
		{
			return new Stitch_npc_ai_level_scale_casterAI(creature);
		}
}; 


//################################################################################################
//Stitch Stitch_npc_ai_level_scale_caster - A l'agro s'adapte au level de l'agresseur - AI Caster
//################################################################################################



void AddSC_Stitch_npc_ai_level_scale()
{
	new Stitch_npc_ai_level_scale_caster();					//Stitch npc_level_scale - s'adapte au level de l'agresseur
}















