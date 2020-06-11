//##############################################################################################
//Stitch npc_level_scale - A l'agro s'adapte au level de l'agresseur + AI generique
// Ces mobs utilisent les sorts definit dans Creature_Template->spellx
// Spell1 : Buff a l'agro
// Spell2 : Sort sur la cible a l'agro
// Spell3 : Sort sur la cible pendant le combat

// Druide equilibre : UPDATE `creature_template` SET `spell1` = 1126,`spell2` = 8921,`spell3` = 119577, `spell4` = 0,`ScriptName` = 'Stitch_npc_ai_level_scale_caster' WHERE (entry = 15000319);
//##############################################################################################






//##############################################################################################
//Stitch npc_level_scale - A l'agro s'adapte au level de l'agresseur
//##############################################################################################

class Stitch_npc_ai_level_scale_caster : public CreatureScript
{
public: Stitch_npc_ai_level_scale_caster() : CreatureScript("Stitch_npc_ai_level_scale_caster") { }

		struct Stitch_npc_ai_level_scale_casterAI : public ScriptedAI
		{
			Stitch_npc_ai_level_scale_casterAI(Creature* creature) : ScriptedAI(creature) { }

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
			void UpdateAI(uint32 /*diff*/)
			{
				if (!UpdateVictim())
					return;



				// Distance de la cible en fonction de son mana
				if (me->GetPower(POWER_MANA) > me->GetMaxPower(POWER_MANA) / 20)
				{
					AttackStartCaster(me->GetVictim(), 20.0f);
					void DoRangedAttackIfReady();					// Combat a distance
					DoSpellAttackIfReady(me->m_spells[2]);			// Creature_Template->Spell3 : spell sur la cible
				}
				if (me->GetPower(POWER_MANA) < me->GetMaxPower(POWER_MANA) / 20)
				{
					AttackStartCaster(me->GetVictim(), 4.0f);
					DoMeleeAttackIfReady();							// Combat au contact
				}

			}

		};

		CreatureAI* GetAI(Creature* creature) const override
		{
			return new Stitch_npc_ai_level_scale_casterAI(creature);
		}
}; 
void AddSC_Stitch_npc_ai_level_scale()
{
	new Stitch_npc_ai_level_scale_caster();					//Stitch npc_level_scale - s'adapte au level de l'agresseur
}