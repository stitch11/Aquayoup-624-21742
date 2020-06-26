//################################################################################################################################################################################################
// Copyright (C) Juin 2020 Stitch pour Aquayoup
// AI generique npc par classe : DRUIDE V1.3
// Il est possible de modifier le temp entre 2 cast avec `BaseAttackTime` & `RangeAttackTime` 
// Exemple pour Creature_Template :
// Minimun  : UPDATE `creature_template` SET `ScriptName` = 'Stitch_npc_ai_druide',`AIName` = '' WHERE (entry = 15100001);
// Optionel : UPDATE `creature_template` SET `HealthModifier` = 2, `ManaModifier` = 3, `ArmorModifier` = 1, `DamageModifier` = 2,`BaseAttackTime` = 2000, `RangeAttackTime` = 2000 WHERE(entry = 15100001);
// Optionel : Utilisez le spell8 de creature_template pour passer certains parametres afin de rester compatible avec tout les cores. Si spell8 = 1 (branche1), spell8 = 2 (branche2), etc
//################################################################################################################################################################################################



//################################################################################################
//StitchAI AI Druide
//################################################################################################

class Stitch_npc_ai_druide : public CreatureScript
{
public: Stitch_npc_ai_druide() : CreatureScript("Stitch_npc_ai_druide") { }

		struct Stitch_npc_ai_druideAI : public ScriptedAI
		{
			Stitch_npc_ai_druideAI(Creature* creature) : ScriptedAI(creature) { }

			uint8 BrancheSpe = 1;													// Choix de la Spécialisation : Equilibre=1, Ours=2, Felin=3, Tréant=4
			uint8 NbrDeSpe = 4;														// Nombre de Spécialisations
			uint8 Random;
			uint8 DistanceDeCast = 30;												// Distance max a laquelle un npc attaquera , au dela il quite le combat
			uint8 ResteADistance = 20;												// Distance max a laquelle un npc s'approchera
			uint8 Dist;																// Distance entre le npc et sa cible
			uint32 Mana;
			uint32 MaxMana = me->GetMaxPower(POWER_MANA);

			uint32 Morph_Branche2 = 5487;											// Morph Ours 2281 , Gros Ours 1990
			uint32 Morph_Branche3 = 892;											// Morph félin 892
			uint32 Morph_Branche4 = 57040;											// Morph Petite-Dranche 57040

			Unit* victim = me->GetVictim();											// La cible du npc
			uint32 Buf_all = 1126;													// Marque du fauve 1126
			uint32 Buf_branche1 = 22812;											// Ecorce = 22812
			uint32 Buf_branche2 = 15727;											// Rugissement démoralisant 15727
			uint32 Buf_branche3 = 33907;											// Epines 33907, Hurlement furieux 3149 (force +15, Rouge)
			uint32 Buf_branche4 = 33907;											// Epines 33907

			// Spells Equilibre
			uint32 Spell_branche1_agro;
			uint32 Spell_branche1_1;
			uint32 Spell_branche1_2;
			uint32 Spell_branche1_3;
			uint32 branche1_agro[4] = { 2912, 2912, 2912, 33844 };					// Feu stellaire 2912, Sarments (4s) 33844
			uint32 branche1_1[2] = { 119577, 5176 };								// Colère (verte) 119577 , Colère (jaune) 5176
			uint32 branche1_2[2] = { 78674, 770 };									// Éruption stellaire 78674, Lucioles 770
			uint32 branche1_3[2] = { 15798, 2912 };									// Eclat lunaire 15798, Feu stellaire 2912
																					
			// Definitions des Spells Ours
			uint32 Spell_branche2_agro;
			uint32 Spell_branche2_1;
			uint32 Spell_branche2_2;
			uint32 Spell_branche2_3;
			uint32 branche2_agro[3] = { 32323, 22863, 19196 };						// Charge 32323, Vitesse 22863 (30%) , charge afflux 19196 (charge+renverse)
			uint32 branche2_1[2] = { 300206, 84867 };								// Mutiler 300206, Balayage fracassant 84867
			uint32 branche2_2[2] = { 300207, 300208 };								// Mutilation (mob ours) 300207, Rosser Ours 300208
			uint32 branche2_3[5] = { 131172, 300209, 138766, 10887, 127987 };		// Rugissement perçant 138766 (interrompt ,empêchant les sorts de la même école pendant 6 sec)
																					// Coup de tete 131172, Coup de crâne 300209 (dps+interrompt  4s), faucher la foule 10887 (interomp, repousse 10 m), *Morsure hémorragique 127987
			// Spells Felin
			uint32 Spell_branche3_agro;
			uint32 Spell_branche3_1;
			uint32 Spell_branche3_2;
			uint32 Spell_branche3_3;
			uint32 branche3_agro[3] = { 22863, 89712, 59737 };						// Vitesse +30% 22863, Griffure bondissante 89712 (Etourdi 2s),vitesse +100% 59737
			uint32 branche3_1[2] = { 24187, 85835 };								// Griffe 24187, Mutiler 85835
			uint32 branche3_2[2] = { 113687, 138765 };								// Morsure 113687, Écraser 138765 (armure -10%/30s)
			uint32 branche3_3[4] = { 300220, 300220, 83639, 30639 };				// Griffure 300220, Bain de sang 83639 (6 attaque /3s), Morsure carnivore 30639

			// Spells Tréant 
			uint32 Spell_branche4_agro;
			uint32 Spell_branche4_1;
			uint32 Spell_branche4_2;
			uint32 Spell_branche4_3;
			uint32 branche4_agro[3] = { 33844, 82742, 82742 };						// Sarments 4s 33844, *Empaler et tirer 82742
			uint32 branche4_1[3] = { 131193, 119004, 38742 };						// Choc 131193, Violent coup direct 119004, Enchaînement gangrené 38742
			uint32 branche4_2[4] = { 80515, 113967, 113967, 118682 };				// Frénésie immortelle 80515 (Canalisée 5s), Barbelés d'épines 113967, Taper du pied 118682
			uint32 branche4_3[4] = { 129375, 689, 152571, 152571 };					// Choc terrestre 129375 (stun 3s), Drain de vie 689, Encorner 152571 (cone 5m)

			uint32 Spell_Heal = 5185;  												// Toucher guérisseur 5185

		// Definitions des variables Cooldown et le 1er lancement
			uint32 Cooldown_Spell1 = 500;
			uint32 Cooldown_Spell2 = 2500;
			uint32 Cooldown_Spell3 = 3500;
			uint32 Cooldown_Spell_Heal = 3000;
			uint32 Cooldown_RegenMana = 3000;
			uint32 Cooldown_ResteADistance = 1000;									// Test si en contact pour Equilibre pour s'eloigner, bouger en combat pour le Felin
			uint32 Cooldown_Charge = 8000;

			void JustRespawned() override
				{

					// Pour visuel seulement
					me->setPowerType(POWER_MANA);
					me->SetMaxPower(POWER_MANA, MaxMana);
					me->SetPower(POWER_MANA, MaxMana);
					me->RestoreDisplayId();													// Retire Morph
					me->GetMotionMaster()->MoveTargetedHome();								// Retour home
				}
			void EnterCombat(Unit* /*who*/) override
			{
				if (!UpdateVictim())
					return;
			
				Unit* victim = me->GetVictim();

				// Choix de la Spécialisation
				BrancheSpe = urand(1, NbrDeSpe+2);
				if (BrancheSpe > NbrDeSpe) { BrancheSpe = 1; }						// plus de chance d'etre equilibre
				
				// Forcer le choix de la Spécialisation
				float ForceBranche = me->m_spells[7];								// creature_template->spell8
				if (ForceBranche == 1) { BrancheSpe = 1; }							// branche1 forcé
				else if (ForceBranche == 2) { BrancheSpe = 2; }						// branche5 forcé
				else if (ForceBranche == 3) { BrancheSpe = 3; }						// branche3 forcé 
				else if (ForceBranche == 4) { BrancheSpe = 4; }						// branche4 forcé 

				me->SetReactState(REACT_AGGRESSIVE);

				// Spell a lancer a l'agro ---------------------------------------------------------------------------------------------------------------------
					me->CastSpell(me, Buf_all, true);										// Buf1 sur lui meme pour toutes les Sépialitées

					switch(BrancheSpe)
					{
					case 1: // Si Spécialisation Equilibre -----------------------------------------------------------------------------------------------------

						// Pour visuel seulement
						VisuelMana();

						me->CastSpell(me, Buf_branche1, true);								// Buf2 sur lui meme

						// Tirages aléatoires des spells Equilibre 
						Spell_branche1_agro = branche1_agro[urand(0, 3)];
						Spell_branche1_1 = branche1_1[urand(0, 1)];
						Spell_branche1_2 = branche1_2[urand(0, 1)];
						Spell_branche1_3 = branche1_3[urand(0, 1)];

						AttackStartCaster(victim, ResteADistance);							// Distance de cast
						me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);				// ROOT

						Random = urand(1, 3); 
						if (Random == 1) { DoCastVictim(Spell_branche1_agro); }				// 1/3 Chance de lancer le sort d'agro
					break;

					case 2: // Si Spécialisation Ours  --------------------------------------------------------------------------------------------------------
						me->CastSpell(me, 300213, true);									// Reduction des degats physiques -25%
						me->SetSpeedRate(MOVE_RUN, 1.2f);									// Vitesse de déplacement

						// Pour visuel seulement
						VisuelRage();
						me->SetDisplayId(2281);												// Morph Ours

						me->CastSpell(me, Buf_branche2, true);								// Buf3 sur lui meme

						// Tirages aléatoires des spells Ours 
						Spell_branche2_agro = branche2_agro[urand(0, 2)];
						Spell_branche2_1 = branche2_1[urand(0, 1)];
						Spell_branche2_2 = branche2_2[urand(0, 1)];
						Spell_branche2_3 = branche2_3[urand(0, 4)];

						Random = urand(1, 3);
						if (Random == 1) { DoCastVictim(Spell_branche2_agro); }				// 1/3 Chance de lancer le sort d'agro
					break;

					case 3: // Si Spécialisation Felin  --------------------------------------------------------------------------------------------------------
						me->CastSpell(me, 300216, true);									// Reduction des degats physiques -15%
						me->SetSpeedRate(MOVE_RUN, 1.2f);									// Vitesse de déplacement

						// Pour visuel seulement
						VisuelEnergy();
						me->SetDisplayId(892);												// Morph Felin

						me->CastSpell(me, Buf_branche3, true);								// Buf3 sur lui meme

						// Tirages aléatoires des spells Equilibre 
						Spell_branche3_agro = branche3_agro[urand(0, 2)];
						Spell_branche3_1 = branche3_1[urand(0, 1)];
						Spell_branche3_2 = branche3_2[urand(0, 1)];
						Spell_branche3_3 = branche3_3[urand(0, 3)];

						Random = urand(1, 3);
						if (Random == 1) { DoCastVictim(Spell_branche3_agro); }				// 1/3 Chance de lancer le sort d'agro
						break;

					case 4: // Si Spécialisation Tréant ---------------------------------------------------------------------------------------------------------
						me->CastSpell(me, 300213, true);									// Reduction des degats physiques -25%

						// Pour visuel seulement
						me->SetDisplayId(864);												// Morph Tréant - Petite-Branche 57040

						me->CastSpell(me, Buf_branche4, true);								// Buf4 sur lui meme

						// Tirages aléatoires des spells Petite-Branche 
						Spell_branche4_agro = branche4_agro[urand(0, 2)];
						Spell_branche4_1 = branche4_1[urand(0, 2)];
						Spell_branche4_2 = branche4_2[urand(0, 3)];
						Spell_branche4_3 = branche4_3[urand(0, 3)];

						Random = urand(1, 2);
						if (Random == 1) { DoCastVictim(Spell_branche4_agro); }				// 1/2 Chance de lancer le sort d'agro
						break;

					}

			}
			void JustReachedHome() override
			{
				// Pour visuel seulement
				me->setPowerType(POWER_MANA);
				me->SetMaxPower(POWER_MANA, MaxMana);
				me->SetPower(POWER_MANA, MaxMana);

				me->DeMorph();
				me->SetSpeedRate(MOVE_RUN, 1.0f);

				me->RemoveAura(Buf_all);
				me->RemoveAura(Buf_branche1);
				me->RemoveAura(Buf_branche2);
				me->RemoveAura(Buf_branche3);
				me->RemoveAura(Buf_branche4);
				me->RemoveAura(300213);														// Retire Reduction des degats physiques -25%
				me->RemoveAura(300216);														// Retire Reduction des degats physiques -15%
				me->RemoveAura(300218);														// Retire Bonus au degats physiques infligés + 25 %

				RetireBugDeCombat();
				me->SetReactState(REACT_AGGRESSIVE);
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
				case 1: // Spécialisation Equilibre ################################################################################################################

					Mouvement_Caster(diff);
					Mouvement_All();
					Combat_Equilibre(diff);
					break;

				case 2 : // Spécialisation Ours ####################################################################################################################

					Combat_Ours(diff);
					Mouvement_Ours(diff);
					Mouvement_All();
					break;

				case 3: // Spécialisation Felin ####################################################################################################################
			
					Combat_Felin(diff);
					Mouvement_Felin(diff);
					Mouvement_All();
					break;

				case 4: // Spécialisation Tréant ######################################################################################################################

					Combat_Treant(diff);
					Mouvement_Treant(diff);
					Mouvement_All();

					// heal sur lui meme
					if ( (me->GetHealth() < (me->GetMaxHealth()*0.6)) && (Cooldown_Spell_Heal <= diff)	)	// Si PV < 60%
					{
						DoCastVictim(689);																	// Drain de vie 689
						Cooldown_Spell_Heal = 8000;
					}

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
			void VisuelMana()
			{
				me->setPowerType(POWER_MANA);
				me->SetMaxPower(POWER_MANA, MaxMana);
				me->SetPower(POWER_MANA, MaxMana);
			}
			void VisuelRage()
			{
				me->SetMaxPower(POWER_MANA, 1000);
				me->setPowerType(POWER_RAGE);
				me->SetMaxPower(POWER_RAGE, 1000);
				me->SetPower(POWER_RAGE, 1000);
			}
			void VisuelEnergy()
			{
				me->SetMaxPower(POWER_MANA, 100);
				me->setPowerType(POWER_ENERGY);
				me->SetMaxPower(POWER_ENERGY, 100);
				me->SetPower(POWER_ENERGY, 100);
			}

			void Mouvement_All()
			{
				if (!UpdateVictim())
					return;
				// Mouvement Tous -----------------------------------------------------------------------------------------------------------------------------------
				Dist = me->GetDistance(me->GetVictim());
				if (Dist > DistanceDeCast | (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > 40))
				{
					RetireBugDeCombat();
					me->AddUnitState(UNIT_STATE_EVADE);
					EnterEvadeMode(EVADE_REASON_SEQUENCE_BREAK);						// Quite le combat si la cible > 30m (Caster & Mélée) ou > 40m de home
				}
			}
			void Mouvement_Caster(uint32 diff)
			{
				if ( !UpdateVictim() )
					return;

				Mana = me->GetPower(POWER_MANA);
				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				// Mouvement aléatoire si cible < 6m & Mana > 5% --------------------------------------------------------------------------------------------
				if (Cooldown_ResteADistance <= diff)
				{
					if ((Dist < 6) && (Mana > MaxMana / 20))
					{
						me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);			// UNROOT
						me->SetSpeedRate(MOVE_RUN, 1.25f);

						float x, y, z;
						me->GetClosePoint(x, y, z, me->GetObjectSize() / 3, ResteADistance + 5);			// Bouge de ResteADistance + 5m
						me->GetMotionMaster()->MovePoint(0xFFFFFE, x, y, z);

						Cooldown_ResteADistance = urand(4000,6000);
					}
				}
				else Cooldown_ResteADistance -= diff;

				// Mouvement OFF si Mana > 5% & distance > 5m & <= 20m ---------------------------------------------------------------------------------------------
				if ((Mana > MaxMana / 20) && (Dist > 5) && (Dist <= ResteADistance) )
				{
					AttackStartCaster(me->GetVictim(), ResteADistance);						// Distance de cast

					me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);					// ROOT
					me->SetSpeedRate(MOVE_RUN, 1.0f);
				}

				// Mouvement ON si distance > 20m -----------------------------------------------------------------------------------------------
				if (Dist > 20)
				{
					AttackStartCaster(me->GetVictim(), ResteADistance);						// Distance de cast

					me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);				// UNROOT
					me->SetSpeedRate(MOVE_RUN, 1.0f);
				}
			
					// Mouvement ON si Mana < 5%  -----------------------------------------------------------------------------------------------
					if (Mana < MaxMana / 20)
					{
						me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);			// UNROOT
						me->SetSpeedRate(MOVE_RUN, 1.0f);
					} 

			}
			void Mouvement_Ours(uint32 diff)
			{
				if (!UpdateVictim())
					return;

				Dist = me->GetDistance(me->GetVictim());

				// Mouvement Ours -----------------------------------------------------------------------------------------------------------------------------------
				if (Cooldown_Charge <= diff)
				{
					Random = urand(1, 2);
					if ((Dist >= 8) && (Dist <= 20))
					{
						if (Random = 1)
						{
							DoSpellAttackIfReady(32323);										// Charge 32323 - 1 chance sur 2
						}
						Cooldown_Charge = 15000;
					}
				} else Cooldown_Charge -= diff;
			}
			void Mouvement_Felin(uint32 diff)
			{
				if (!UpdateVictim())
					return;

				Dist = me->GetDistance(me->GetVictim());

				// Mouvement Felin -----------------------------------------------------------------------------------------------------------------------------------

				// Si la cible est entre 8 & 30m : Griffure bondissante
				if (Cooldown_Charge <= diff)
				{
					Random = urand(1, 4);
					if ((Dist >= 8) && (Dist <= DistanceDeCast))
					{
						if (Random = 1)
						{
							DoSpellAttackIfReady(89712);									// Griffure bondissante - 1 chance sur 4
						}
						Cooldown_Charge = urand(15000,20000);
						Cooldown_ResteADistance = urand(2000, 4000);						// Sinon bugue BOND Aleatoire ou Avance !?
					}
				}
				else Cooldown_Charge -= diff;

				// Si la cible < 8m : BOND Aleatoire ou Avance
				if (Dist < 8 && (Cooldown_ResteADistance <= diff))
				{
					Random = urand(1, 5);
					if (Random == 1)
					{
						DoCast(me, 70485);													// 1 chance sur 5 BOND Aleatoire 70485 Au contact
					}
					else
					{
						 // Au contact tourne au tour de sa victime --------------------------------------------------------------------------------------------------
						 Unit* victim = me->GetVictim();
						 Dist = me->GetDistance(me->GetVictim());

						 float x, y, z;
						 x = (victim->GetPositionX() + urand(0, 4) - 2);
						 y = (victim->GetPositionY() + urand(0, 4) - 2);
						 z = victim->GetPositionZ();
						 me->GetMotionMaster()->MovePoint(0, x, y, z);						// 4 chance sur 5 avance Au contact
					}
					Cooldown_ResteADistance = urand(2000,4000);
				}
				else Cooldown_ResteADistance -= diff;
			}
			void Mouvement_Treant(uint32 diff)
			{
				// Mouvement Tréant  ----------------------------------------------------------------------------------------------------------------------------------
				if (Cooldown_Charge <= diff)
				{
					Random = urand(1, 5);
					if ((Dist >= 8) && (Dist <= 20))
					{
						if (Random = 1)
						{
							DoSpellAttackIfReady(82742);													// Empaler et tirer - 1 chance sur 2
						} else
							DoSpellAttackIfReady(113967);													// Barbelés d'épines 

						Cooldown_Charge = 5000;
					}
				}
				else Cooldown_Charge -= diff;
			}

			void Combat_Equilibre(uint32 diff)
			{
				if (!UpdateVictim() /*| (me->HasUnitState(UNIT_STATE_MOVING))*/ )
					return;

				Mana = me->GetPower(POWER_MANA);
				Dist = me->GetDistance(me->GetVictim());

				// Combat - Equilibre --------------------------------------------------------------------------------------------------------------------------
				if (Dist <= ResteADistance)
				{
					// Regen mana en combat ------------------------------------------------------------------------------------------------------------------------
					if (Cooldown_RegenMana <= diff)
					{
						me->SetPower(POWER_MANA, Mana + (MaxMana / 20));
						if (Mana > MaxMana) { me->SetPower(POWER_MANA, MaxMana); }
						Cooldown_RegenMana = 1000;
					}
					else Cooldown_RegenMana -= diff;

					Heal_En_Combat_Caster(diff);

					// Spell1 sur la cible chaque (Sort Régulié)
					if (Cooldown_Spell1 <= diff)
					{
						DoCastVictim(Spell_branche1_1);
						Cooldown_Spell1 = 3500;
					}
					else Cooldown_Spell1 -= diff;

					// Spell2 sur la cible chaque (Sort secondaire plus lent)
					if (Cooldown_Spell2 <= diff)
					{
						DoCastVictim(Spell_branche1_2);
						Cooldown_Spell2 = urand(4000, 6000);
					}
					else Cooldown_Spell2 -= diff;

					// Spell3 sur la cible  (Sort secondaire tres lent , généralement utilisé comme Dot)
					if (Cooldown_Spell3 <= diff)
					{
						DoCastVictim(Spell_branche1_3);
						Cooldown_Spell3 = urand(8000, 10000);
					}
					else Cooldown_Spell3 -= diff;

				}
			}
			void Combat_Ours(uint32 diff)
			{
				if (!UpdateVictim())
					return;

				Dist = me->GetDistance(me->GetVictim());
				// Combat Ours --------------------------------------------------------------------------------------------------------------------------------
				if (Dist < 6)
				{
					// Spell1 sur la cible chaque (Sort Régulié)
					if (Cooldown_Spell1 <= diff)
					{
						DoCastVictim(Spell_branche2_1);
						Cooldown_Spell1 = 4000;
					}
					else Cooldown_Spell1 -= diff;

					// Spell2 sur la cible chaque (Sort secondaire plus lent)
					if (Cooldown_Spell2 <= diff)
					{
						DoCastVictim(Spell_branche2_2);
						Cooldown_Spell2 = 5000;
					}
					else Cooldown_Spell2 -= diff;

					// Spell3 sur la cible  (Sort secondaire tres lent , généralement utilisé comme Dot)
					if (Cooldown_Spell3 <= diff)
					{
						DoCastVictim(Spell_branche2_3);
						Cooldown_Spell3 = 10000;
					}
					else Cooldown_Spell3 -= diff;
				}
			}
			void Combat_Felin(uint32 diff)
			{
				if (!UpdateVictim())
					return;

				Dist = me->GetDistance(me->GetVictim());

				// Combat Felin --------------------------------------------------------------------------------------------------------------------------------
				if (Dist < 6)
				{
					// Spell1 sur la cible chaque (Sort Régulié)
					if (Cooldown_Spell1 <= diff)
					{
						DoCastVictim(Spell_branche3_1);
						Cooldown_Spell1 = 3000;
					}
					else Cooldown_Spell1 -= diff;

					// Spell2 sur la cible chaque (Sort secondaire plus lent)
					if (Cooldown_Spell2 <= diff)
					{
						DoCastVictim(Spell_branche3_2);
						Cooldown_Spell2 = 6000;
					}
					else Cooldown_Spell2 -= diff;

					// Spell3 sur la cible  (Sort secondaire tres lent , généralement utilisé comme Dot)
					if (Cooldown_Spell3 <= diff)
					{
						DoCastVictim(Spell_branche3_3);
						Cooldown_Spell3 = urand(10000, 14000);
					}
					else Cooldown_Spell3 -= diff;
				}

			}
			void Combat_Treant(uint32 diff)
			{
				if (!UpdateVictim())
					return;

				Dist = me->GetDistance(me->GetVictim());

				// Combat Treant -------------------------------------------------------------------------------------------------------------------------
				if ((Dist < 6) && (!me->HasUnitState(UNIT_STATE_CASTING)))
				{
					// Spell1 sur la cible chaque (Sort Régulié)
					if (Cooldown_Spell1 <= diff)
					{
						DoCastVictim(Spell_branche4_1);
						Cooldown_Spell1 = 3000;
					}
					else Cooldown_Spell1 -= diff;

					// Spell2 sur la cible chaque (Sort secondaire plus lent)
					if (Cooldown_Spell2 <= diff)
					{
						DoCastVictim(Spell_branche4_2);
						me->CastSpell(victim, 6146, true);												//Lenteur sur la cible
						me->CastSpell(me, 6146, true);													//Lenteur sur lui
						Cooldown_Spell2 = 5000;
					}
					else Cooldown_Spell2 -= diff;

					// Spell3 sur la cible  (Sort secondaire tres lent , généralement utilisé comme Dot)
					if (Cooldown_Spell3 <= diff)
					{
						DoCastVictim(Spell_branche4_3);
						Cooldown_Spell3 = 10000;
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
				if ((me->GetHealth() < (me->GetMaxHealth()*0.75)))								// Si PV < 75%
				{
					DoCast(me, Spell_Heal);
					Cooldown_Spell_Heal = 2000;
				}

				// heal sur Friend 
				if (target = DoSelectLowestHpFriendly(DistanceDeCast))							// Distance de l'allié = 30m
				{
					if (me->IsFriendlyTo(target) && (me != target))
					{
						if (target->GetHealth() < (target->GetMaxHealth()*0.60))				// Si PV < 60%
						{
							DoCast(target, Spell_Heal);
							Cooldown_Spell_Heal = 2000;
						}
					}
				}
			}
			else Cooldown_Spell_Heal -= diff;
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















