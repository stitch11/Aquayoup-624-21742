//################################################################################################################################################################################################
// Copyright (C) Juin 2020 Stitch pour Aquayoup
// AI generique npc par classe : DRUIDE
// Ces mobs utilisent les sorts definit dans Creature_Template->spellx
// Il est possible de modifier le temp entre 2 cast avec `BaseAttackTime` & `RangeAttackTime` 
// Exemple pour Creature_Template :
// UPDATE `creature_template` SET `HealthModifier` = 2, `ManaModifier` = 3, `ArmorModifier` = 1, `DamageModifier` = 2,`lootid` = 0,`ScriptName` = 'Stitch_npc_ai_druide',`AIName` = '' WHERE (entry = 15100001);
// UPDATE `creature_template` SET `spell1` = 2912, `spell2` = 119577, `spell3` = 770, `spell4` = 87187, `spell5` = 6807, `spell6` = 33917, `spell7` = 15798, `spell8` = 5185, `BaseAttackTime` = 2000, `RangeAttackTime` = 2000 WHERE(entry = 15100001);
//################################################################################################################################################################################################




//################################################################################################
//StitchAI générique Druide
//################################################################################################

class Stitch_npc_ai_druide : public CreatureScript
{
public: Stitch_npc_ai_druide() : CreatureScript("Stitch_npc_ai_druide") { }

		struct Stitch_npc_ai_druideAI : public ScriptedAI
		{
			Stitch_npc_ai_druideAI(Creature* creature) : ScriptedAI(creature) { }

				uint32 DistanceDeCast = 25.0f;
				uint32 Timer_ResteADistance = 100;
				
				uint32 Spell_buf1 = 1126;										// Marque du fauve
				uint32 Spell_buf2 = 22812;										// Epines 33907, Ecorce = 22812
				uint32 Spell_buf3 = 33907;										// Epines 33907, Ecorce = 22812

				uint32 Spell_branche1_agro = me->m_spells[0];					//spell1 Feu stellaire 2912 , Sarments 339
				uint32 Spell_branche1_1 = me->m_spells[1]; 						//spell2 Colère (verte) 119577 , jaune 5176
				uint32 Spell_branche1_2 = me->m_spells[2]; 						//spell3 Lucioles 770
				uint32 Spell_branche2_agro = me->m_spells[3];  					//spell4 Charge farouche 87187
				uint32 Spell_branche2_1 = 6807;  								//spell5 Mutiler 6807
				uint32 Spell_branche2_2 = me->m_spells[5];  					//spell6 Mutilation 33917 , Rosser Ours 106832
				uint32 Spell_Dot = me->m_spells[6];  							//spell7 Eclat lunaire 15798
				uint32 Spell_Heal = me->m_spells[7];  							//spell8 Toucher guérisseur 5185

				uint32 Timer_Spell = 100;
				uint32 Timer_Dot = 3000;
				uint32 Timer_Heal = 4000;
				uint32 Timer_RegenMana = 5000;

				Unit* victim = me->GetVictim();
				uint32 ChangeForm_Branche2 = 5487;										// Forme d'ours 5487
				uint32 ChangeForm_Branche3 = 768;										// Forme de Forme de félin 768
																													
				//uint32 MaxMana = me->GetMaxPower(POWER_MANA);
				//uint32 Mana = me->GetPower(POWER_MANA);

				uint8 BrancheSpe /*= urand(1, 2)*/;								// Choix de la Spécialisation : Equilibre=1, Ours=2

				uint8 ChoixBrancheSpe()
				{
					return BrancheSpe = urand(1, 2);
				}


			void JustRespawned() override
			{
				//me->CastSpell(me, Spell_buf1, true);							// Buf1
			}
			void EnterCombat(Unit* /*who*/) override
			{
				if (!UpdateVictim())
					return;
	
			
				Unit* victim = me->GetVictim();
				BrancheSpe = urand(1,2);										// Choix de la Spécialisation : Equilibre=0, Ours=1
				//ChoixBrancheSpe();

				// Spell a lancer a l'agro ---------------------------------------------------------------------------------------------------------------------
				me->CastSpell(me, Spell_buf1, true);							// Buf1 sur lui meme
				
				// Si Spécialisation Equilibre -----------------------------------------------------------------------------------------------------------------
				if (BrancheSpe == 1)
				{
					Mouvement_All(NULL);
					Mouvement_Caster(NULL);
					//me->setPowerType(POWER_MANA);
					//me->SetMaxPower(POWER_MANA, MaxMana);
					me->CastSpell(me, Spell_buf2, true);						// Buf2 sur lui meme
					uint8 Random = urand(0, 1); 
					if (Random = 1) { DoSpellAttackIfReady(Spell_branche1_agro);  /*DoCast(victim, Spell_branche1_agro)*/; }	// 1/2 spell1 Feu stellaire 2912 , Sarments 339

					AttackStartCaster(victim, DistanceDeCast);					// Distance de cast
					void DoRangedAttackIfReady();								// Combat a distance
				}
				// Si Spécialisation Ours ----------------------------------------------------------------------------------------------------------------------
				if (BrancheSpe == 2)
				{
					Mouvement_All(NULL);
					me->CastSpell(me, ChangeForm_Branche2, true);						// Forme d'ours
					me->CastSpell(me, Spell_buf3, true);						// Buf3 sur lui meme
					//me->setPowerType(POWER_RAGE);
					me->SetMaxPower(POWER_RAGE, 10);
					me->SetPower(POWER_RAGE, 100);
					AttackStart(victim);										// Combat au corp a corp
					AttackStartCaster(victim, 4.0f);							// Distance de cast

					uint8 Random = urand(0, 1);
					//if (Random = 1) { DoCast(victim, Spell_branche2_agro, true); }	// 1/2 Charge farouche 
					if (Random = 1) { DoSpellAttackIfReady(Spell_branche2_agro); }	// 1/2 Charge farouche 
				}
			}
			void JustReachedHome() override
			{
				RetireBugDeCombat();
			}
			void EnterEvadeMode(EvadeReason /*why*/) override
			{
				me->setPowerType(POWER_MANA);
				me->SetMaxPower(POWER_MANA, me->GetMaxPower(POWER_MANA));
				RetireBugDeCombat();
				me->RemoveAura(ChangeForm_Branche2);
				me->RemoveAura(ChangeForm_Branche3);
				me->RemoveAura(Spell_buf1);
				me->RemoveAura(Spell_buf2);
				me->RemoveAura(Spell_buf3);
			}
			void UpdateAI(uint32 diff) override
			{
				if (!UpdateVictim()/* || me->isPossessed() || me->IsCharmed() || me->HasAuraType(SPELL_AURA_MOD_FEAR)*/)
					return;



				// Si Spécialisation Equilibre -----------------------------------------------------------------------------------------------------------------
				if (BrancheSpe == 1)
				{
					Mouvement_All(NULL);
					Mouvement_Caster(1);

					// Regen mana en combat -------------------------------------------------------------------------------------------------------------------------
					if (Timer_RegenMana <= diff)
					{
						uint32 _Mana = me->GetPower(POWER_MANA);
						int32 _MaxMana = me->GetMaxPower(POWER_MANA);
						me->SetPower(POWER_MANA, _Mana + (_MaxMana / 10));
						if (me->GetPower(POWER_MANA) > _MaxMana) { me->SetPower(POWER_MANA, _MaxMana); }
						Timer_RegenMana = 5000;
					}
					else Timer_RegenMana -= diff;

					// Cast en combat ------------------------------------------------------------------------------------------------------------------------------
					// Creature_Template->Spell2 : spell  sur la cible chaque --------------------------------------------------------------------------------------
					if (Timer_Spell <= diff)
					{
						DoSpellAttackIfReady(Spell_branche1_1);
						//DoCast(victim, Spell_branche1_1);
						Timer_Spell = 1000;
					}
					else Timer_Spell -= diff;

					// Creature_Template->Spell7 : spell Dot sur la cible ------------------------------------------------------------------------------------------
					if (Timer_Dot <= diff)
					{
						DoSpellAttackIfReady(Spell_Dot);
						//DoCast(victim, Spell_Dot);
						Timer_Dot = urand(4000, 8000);
					}
					else Timer_Dot -= diff;

					// Heal en combat ------------------------------------------------------------------------------------------------------------------------------
					if (Timer_Heal <= diff)
					{
						Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, DistanceDeCast, true); // pour heal friend

																									// Creature_Template->Spell8 : heal sur lui meme -------------------------------------------------------------------------------------------
						if (me->GetHealth() < (me->GetMaxHealth()*0.60))							// Si PV < 60%
						{
							DoCast(me, Spell_Heal);
							Timer_Heal = 2000;
						}

						// Creature_Template->Spell8 : heal sur Friend ---------------------------------------------------------------------------------------------
						else if (Unit* target = DoSelectLowestHpFriendly(DistanceDeCast))			// Distance de l'allié = 30m
						{
							if (me->IsFriendlyTo(target))
							{
								if (target->GetHealth() < (target->GetMaxHealth()*0.60))			// Si PV < 60%
								{
									DoCast(target, Spell_Heal);
									Timer_Heal = 2000;
								}
							}
						}
					}
					else Timer_Heal -= diff;

				}
				// Si Spécialisation Ours ----------------------------------------------------------------------------------------------------------------------
				if (BrancheSpe == 2)
				{
					//Mouvement_All(NULL);
					// Regen rage en combat -------------------------------------------------------------------------------------------------------------------------
					me->setPowerType(POWER_RAGE);
					me->SetMaxPower(POWER_RAGE, 100);
					me->SetPower(POWER_RAGE, 100);

					// Cast en combat ------------------------------------------------------------------------------------------------------------------------------
					// Creature_Template->Spell5 : spell  sur la cible chaque --------------------------------------------------------------------------------------
					if (Timer_Spell <= diff)
					{
						//DoSpellAttackIfReady(Spell_branche2_1);
						//DoCast(victim, Spell_branche2_1);
						me->CastSpell(victim, Spell_branche2_1, true);
						Timer_Spell = 2000;
					}
					else Timer_Spell -= diff;

					// Creature_Template->Spell7 : spell Dot sur la cible ------------------------------------------------------------------------------------------
					if (Timer_Dot <= diff)
					{
						DoSpellAttackIfReady(Spell_Dot);
						Timer_Dot = urand(8000, 12000);
					}
					else Timer_Dot -= diff;

				}
			}

			void Mouvement_All(uint32 diff)
			{
				// Quite le combat si la cible > 40m -----------------------------------------------------------------------------------------------------------
				if ((!me->IsWithinCombatRange(me->GetVictim(), DistanceDeCast + 10.0f)))
				{
					RetireBugDeCombat();
					//me->GetMotionMaster()->MoveTargetedHome();					// Retour home
					EnterEvadeMode(EVADE_REASON_NO_HOSTILES);
				}
			}
			void Mouvement_Caster(uint32 diff)
			{
				if (me->HasUnitState(UNIT_STATE_CASTING))
					return;

				if (/*BrancheSpe == 1 &&*/ UpdateVictim())
				{

					uint32 Mana = me->GetPower(POWER_MANA);
					uint32 MaxMana = me->GetMaxPower(POWER_MANA);
					uint32 Dist = me->GetDistance(me->GetVictim());

					// Mouvement off si Mana > 5% ou distance < 20m ------------------------------------------------------------------------------------------------
					if ((me->GetPower(POWER_MANA) > me->GetMaxPower(POWER_MANA) / 20) && (me->IsWithinCombatRange(me->GetVictim(), 20.0f)))
					{
						AttackStartCaster(me->GetVictim(), 20.0f);						// Distance de cast
						void DoRangedAttackIfReady();									// Combat a distance
						me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);			// ROOT
					}

					// Mouvement on si Mana < 5% ou distance > 20m -------------------------------------------------------------------------------------------------
					if ((me->GetPower(POWER_MANA) < me->GetMaxPower(POWER_MANA) / 20) | (!me->IsWithinCombatRange(me->GetVictim(), 20.0f)))
					{
						AttackStart(me->GetVictim());									// Combat au corp a corp
						AttackStartCaster(me->GetVictim(), 20.0f);						// Distance de cast
						DoMeleeAttackIfReady();
						me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);		//UNROOT
					}

					// Mouvement aléatoire si cible < 8m -------------------------------------------------------------------------------------------------------
					if ((Timer_ResteADistance <= diff))
					{
						//if (me->IsWithinCombatRange(me->GetVictim(), 6.0f) && (me->GetPower(POWER_MANA) > me->GetMaxPower(POWER_MANA) / 20))
						if ((Dist < 8.0f) && (me->GetPower(POWER_MANA) > me->GetMaxPower(POWER_MANA) / 20))

						{
							float x, y, z;
							me->GetClosePoint(x, y, z, me->GetObjectSize() / 3, 15.0f);
							me->GetMotionMaster()->MovePoint(0xFFFFFE, x, y, z);
							Timer_ResteADistance = urand(100, 200);
						}
					}
					else Timer_ResteADistance -= diff;
				}
			}
			void RetireBugDeCombat()
			{
				me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);		// UNROOT
				me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);			// Retire flag "en combat"
				me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);		// Retire flag "non attaquable"
				me->GetMotionMaster()->MoveTargetedHome();						// Retour home
			}
		};

		CreatureAI* GetAI(Creature* creature) const override
		{
			return new Stitch_npc_ai_druideAI(creature);
		}
};

//################################################################################################
void AddSC_npc_ai_druide()
{
	new Stitch_npc_ai_druide();
}















