////#########################################################################################################################################################################################################################################
// Copyright (C) Juillet 2020 Stitch pour https:\\Aquayoup.123.fr
// AI generique npc par classe : Caster Ver 2020-11-04 (caster simple, combat a distance)
//
// ScriptName = Stitch_npc_ai_caster : npc d'exemple : 15100013
// spell1 : Attaque principale
// spell2 : Dot
// spell3 : spell lancé a l'agro
// spell4 : spell lancé a l'évade ou respawn
// spell5 : Buf
// spell6 : Heal(lui même uniquement)
// 
// Si spell[1] = 0 :
// alors affectation aléatoire de tous les spells(prédéfini dans le core), sinon utilisera les spells définis dans creature_template spell[1a5]
// Ou bien, si spell[1] = 0 : Il est possible de forcer le choix en éditant le pickpocketloot de creature_template avec le numéro de la Classe :
// 
// Ou bien si spell1 = 0
// pickpocketloot = 0 : Random
// pickpocketloot = 1 : Mage feu
// pickpocketloot = 2 : Mage Givre
// pickpocketloot = 3 : Demo,
// pickpocketloot = 4 : Druide
// pickpocketloot = 5 : Pretre
// pickpocketloot = 6 : DK Chaos


// Il est possible d'influencer le temp entre 2 cast avec `BaseAttackTime` & `RangeAttackTime` 
// Necessite dans Creature_Template :
// UPDATE `creature_template` SET `spell1` = ATTAQUE , `spell2` =  DOT , `spell3` =  AGRO, `spell4` = EVADE , `spell5` = BUF , `spell6` =  HEAL , `ScriptName` = ' Stitch_npc_ai_caster',`AIName` = '' WHERE (entry = 2237);
// REPLACE INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`, `ItemID2`, `ItemID3`, `VerifiedBuild`) VALUES
// (15100013, 1, 854, 0, 0, 0);	## Baton
//###########################################################################################################################################################################################################################################



//################################################################################################
//StitchAI AI Caster
//################################################################################################

class Stitch_npc_ai_caster : public CreatureScript
{
public: Stitch_npc_ai_caster() : CreatureScript("Stitch_npc_ai_caster") { }

		struct Stitch_npc_ai_casterAI : public ScriptedAI
		{
			Stitch_npc_ai_casterAI(Creature* creature) : ScriptedAI(creature) { }

			uint32 Random;
			uint32 DistanceDeCast = 30;												// Distance max a laquelle un npc attaquera , au dela il quite le combat
			uint32 ResteADistance = 12;												// Distance max a laquelle un npc s'approchera
			uint32 Dist;															// Distance entre le npc et sa cible
			Unit* victim = me->GetVictim();										 
			uint32 Mana;
			uint32 MaxMana = me->GetMaxPower(POWER_MANA);
			uint32 ForceBranche;

			// Definitions des variables Cooldown et le 1er lancement
			uint32 Cooldown_Spell1 = 1000;
			uint32 Cooldown_Spell2 = 3000;
			uint32 Cooldown_Spell2_defaut = 4000;
			uint32 Cooldown_ResteADistance = 2000;									// Test si en contact
			uint32 Cooldown_ResteADistance_Defaut = 4000;
			uint32 Cooldown_Anti_Bug_Figer = 2000;
			uint32 Cooldown_Npc_Emotes = urand(5000, 8000);
			uint32 Cooldown_Spell_Heal = 5000;
			uint32 Cooldown_Spell_Heal_defaut = 6000;
			uint32 Cooldown_RegenMana = 5000;
			uint32 Cooldown_RegenMana_defaut = 3500;
			// Spells
			uint32 Buf_1 = 0;													
			uint32 Spell_agro = 0;
			uint32 Spell_1 = 0;
			uint32 Spell_2 = 0;
			uint32 Spell_respawn_evade = 0;
			uint32 Spell_Heal = 0;	

			// Mage Feu
			uint32 Liste_Spell_agro_1[4] = { 31589, 2120, 11366, 2136 };		// Lenteur 31589, Choc de flammes 2120, Double explosions pyrotechniques 11366, Trait de feu 2136
			uint32 Liste_Spell_2_1[2] = { 108853, 2948 };						// Déflagration infernale 108853, Brûlure 2948
			// Mage Givre
			uint32 Liste_Spell_agro_2[3] = { 30449, 44572, 2139 };				// Vol de sort 30449, Congélation 44572, Contresort 2139
			uint32 Liste_Spell_2_2[2] = { 120, 30455 };							// Cône de froid 120, Javelot de glace 30455
			// Demo
			uint32 Liste_Spell_2_3[4] = { 172, 172, 980, 30108 };				// Corruption 172, Immolation 348, Agonie 980, Affliction instable 30108
			// Druide
			uint32 Liste_Spell_agro_4[3] = { 78674, 102355, 33844 };			// Eruption stellaire 78674, Essaim de lucioles 102355, Sarments 33844
			uint32 Liste_Spell_1_4[2] = { 119577, 5176 };						// Colère (verte) 119577, Colère (jaune) 5176
			uint32 Liste_Spell_2_4[2] = { 8921, 8921 };							// Eclat lunaire 8921, Feu stellaire 2912
			uint32 Liste_Spell_Heal_4[3] = { 5185, 5185, 8936 };				// Toucher guérisseur 5185, Croissance sauvage 48438, Récupération 774, Rétablissement 8936
			// Pretre
			uint32 Liste_Spell_Heal_5[2] = { 2061, 2060 };						// Soins rapides 2061, Soins 2060
			// DK Chaos
			uint32 Liste_Spell_agro_6[4] = { 45524, 47528, 77575, 47476 };		// Chaînes de glace 45524, Gel de l'esprit 47528, Poussée de fièvre 77575, Strangulation 47476
			uint32 Liste_Spell_1[3] = { 300051, 300052, 300050 };				// Javelot de givre 300051, Javelot de peste 300052, Javelot de sang 300050
			uint32 Liste_Spell_respawn_evade_6[3] = { 57330, 3114, 48792 };		// Cor de l’hiver 57330, Passage de givre 3114, Robustesse glaciale 48792

			// Emotes
			uint32 Npc_Emotes[22] = { 1,3,7,11,15,16,19,21,22,23,24,53,66,71,70,153,254,274,381,401,462,482 };

			uint32 Start_Agro = 0;

			void Init_AI()
			{

				// ################################################################################################################################################
				// Tirages aléatoires des spells
				// ################################################################################################################################################
				// Si m_spells[0] != 0 alors affectation aléatoire de tous les spells (prédéfinis dans le core) , sinon utilisera les spell défini dans vreature_template spells[1 a 5]
				// m_spells[0] : Attaque principale						- Correspond a spell1 de creature_template
				// m_spells[1] : Dot									- Correspond a spell2 de creature_template
				// m_spells[2] : spell lancé a l'agro					- Correspond a spell3 de creature_template
				// m_spells[3] : spell lancé a l'évade ou respawn		- Correspond a spell4 de creature_template
				// m_spells[4] : Buf									- Correspond a spell5 de creature_template
				// m_spells[5] : Heal(lui meme uniquement)				- Correspond a spell6 de creature_template

				if (me->m_spells[0] == 0) 
				{ 

					// Forcer le choix de la classe par creature_template->pickpocketloot
					ForceBranche = me->GetCreatureTemplate()->pickpocketLootId;
					if (ForceBranche == 1) { Random = 1; }															// Mage feu forcé
					else if (ForceBranche == 2) { Random = 2; }														// Mage Givre forcé 
					else if (ForceBranche == 3) { Random = 3; }														// Demo forcé
					else if (ForceBranche == 4) { Random = 4; }														// Druide forcé
					else if (ForceBranche == 5) { Random = 5; }														// Pretre forcé
					else if (ForceBranche == 6) { Random = 6; }														// DK Chaos forcé
					else { 
						Random = urand(1, 6); 
					}

					// Tirage aléatoire d'une pseudo classe
					switch (Random)
					{
					case 1: // Mage feu
						Buf_1 = 165743;													// Armure de givre	
						Spell_agro = Liste_Spell_agro_1[urand(0, 3)];
						Spell_1 = 9053;													// Boule de feu 
						Spell_2 = Liste_Spell_2_1[urand(0, 1)];
						Spell_respawn_evade = 19726;									// Aura de résistance
						Spell_Heal = 12051;												// Evocation
						Cooldown_Spell2_defaut = 6000;
						Cooldown_Spell_Heal_defaut = 30000;
						Cooldown_ResteADistance_Defaut = 4000;
						Cooldown_RegenMana_defaut = 4000;
						break;
					case 2: // Mage Givre
						Buf_1 = 165743;													// Armure de givre	
						Spell_agro = Liste_Spell_agro_2[urand(0, 2)];
						Spell_1 = 116;													// Eclair de givre
						Spell_2 = Liste_Spell_2_2[urand(0, 1)];
						Spell_respawn_evade = 19726;									// Aura de résistance
						Spell_Heal = 12051;												// Evocation
						Cooldown_Spell2_defaut = 7000;
						Cooldown_Spell_Heal_defaut = 30000;
						Cooldown_ResteADistance_Defaut = 4000;
						Cooldown_RegenMana_defaut = 2750;
						break;
					case 3: // Demo
						Buf_1 = 79934;  												// Armure démoniaque
						Spell_agro = 348;												// Immolation
						Spell_1 = 686;													// Trait de l'ombre
						Spell_2 = Liste_Spell_2_3[urand(0, 3)];
						Spell_respawn_evade = 0;										// Sombre intention (puissance des sorts + 10 % )
						Spell_Heal = 0;													// Drain de vie 689
						Cooldown_Spell2_defaut = 10000;
						Cooldown_Spell_Heal_defaut = 6000;
						Cooldown_ResteADistance_Defaut = 5000;
						Cooldown_RegenMana_defaut = 2500;
						break;
					case 4: // Druide
						Buf_1 = 1126;													// Marque du fauve
						Spell_agro = Liste_Spell_agro_4[urand(0, 2)];
						Spell_1 = Liste_Spell_1_4[urand(0, 1)];
						Spell_2 = Liste_Spell_2_4[urand(0, 1)]; 							
						Spell_respawn_evade = 22812; 									// Ecorce 22812
						Spell_Heal = Liste_Spell_Heal_4[urand(0, 2)];
						Cooldown_Spell2_defaut = 20000;
						Cooldown_Spell_Heal_defaut = 6000;
						Cooldown_ResteADistance_Defaut = 3500;
						Cooldown_RegenMana_defaut = 1000;
						break;
					case 5: // Pretre
						Buf_1 = 21562;													// Mot de pouvoir  : Robustesse 21562
						Spell_agro = 8092;												// Attaque mentale 8092
						Spell_1 = 585;													// Châtiment 585
						Spell_2 = 589;													// Mot de l’ombre : Douleur 589
						Spell_respawn_evade = 528;										// Dissipation de la magie 528, Suppression de la douleur 33206
						Spell_Heal = Liste_Spell_Heal_5[urand(0, 1)];
						Cooldown_Spell2_defaut = 15000;
						Cooldown_Spell_Heal_defaut = 5000;
						Cooldown_ResteADistance_Defaut = 4000;
						Cooldown_RegenMana_defaut = 2000;
						break;
					case 6: // DK Chaos
						Buf_1 = 300049;													// Présence du Chaos 30049
						Spell_agro = Liste_Spell_agro_6[urand(0, 3)];
						Spell_1 = Liste_Spell_1[urand(0, 2)];							// Javelot de givre 300051, Javelot de peste 300052, Javelot de sang 300050
						Spell_2 = 300061;												// Choc impie 300061
						Spell_respawn_evade = Liste_Spell_respawn_evade_6[urand(0, 2)];	
						Spell_Heal = 48982;												// Connexion runique 48982
						Cooldown_Spell2_defaut = 10000;
						Cooldown_Spell_Heal_defaut = 30000;
						Cooldown_ResteADistance_Defaut = 5000;
						Cooldown_RegenMana_defaut = 5000;
						break;
					}

				} else 
				{ 
					Spell_1 = me->m_spells[0]; 
					if (me->m_spells[1] != 0) { Spell_2 = me->m_spells[1]; }
					if (me->m_spells[2] != 0) { Spell_agro = me->m_spells[2]; }
					if (me->m_spells[3] != 0) { Spell_respawn_evade = me->m_spells[3]; }
					if (me->m_spells[4] != 0) { Buf_1 = me->m_spells[4]; }
					if (me->m_spells[5] != 0) { Spell_Heal = me->m_spells[5]; }
				}


				// Spell a lancer a l'agro ------------------------------------------------------------------------------------------------------------------------

					Random = urand(1, 2);
					if (Random == 1 && Buf_1 != 0) { me->CastSpell(me, Buf_1, true); }

				// ################################################################################################################################################
				// Divers
				// ################################################################################################################################################
				me->SetSheath(SHEATH_STATE_MELEE);												// S'equipe de l'arme au contact
				me->SetReactState(REACT_AGGRESSIVE);
				// ################################################################################################################################################
			}
			void JustRespawned() override
			{
				me->GetMotionMaster()->MoveTargetedHome();								// Retour home pour rafraichir client
				//me->SetSpeedRate(MOVE_RUN, 1.01f);
				me->SetReactState(REACT_AGGRESSIVE);

				Init_AI();
				Random = urand(1, 2);
				if (Random == 1 && Spell_respawn_evade != 0) { me->CastSpell(me, Spell_respawn_evade, true); }		// 1/2 Chance de lancer le sort au respawn ou evade
			}

			void EnterCombat(Unit* /*who*/) override
			{
				Init_AI();
				me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);								// ROOT
			}
			void EnterEvadeMode(EvadeReason /*why*/) override
			{
				Start_Agro = 0;
				RetireBugDeCombat();
				me->AddUnitState(UNIT_STATE_EVADE);
				//me->SetSpeedRate(MOVE_RUN, 1.5f);													// Vitesse de déplacement
				me->GetMotionMaster()->MoveTargetedHome();											// Retour home
				me->RemoveAllControlled();															// renvois pet

				me->RemoveAura(Buf_1);

				Random = urand(1, 3);
				if (Random == 1 && Spell_respawn_evade != 0) { me->CastSpell(me, Spell_respawn_evade, true); }		// 1/3 Chance de lancer le sort au respawn ou evade
			}
			void JustReachedHome() override
			{
				me->SetReactState(REACT_AGGRESSIVE);
				//me->SetSpeedRate(MOVE_RUN, 1.01f);										// Vitesse par defaut définit a 1.01f puisque le patch modification par type,famille test si 1.0f
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
					Cooldown_Npc_Emotes = urand(10000, 20000);
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

					// Ce heal s'il a un sort de heal -------------------------------------------------------------------------------------------------------------
					if (Spell_Heal != 0 && Spell_Heal != 0)
					{
						Heal_En_Combat_Caster(diff);
					}

					if (Start_Agro == 0)
					{
						Start_Agro = 1;

						// ########################################################################################################################################
						// Spell a lancer a l'agro 
						// ########################################################################################################################################

							Random = urand(1, 2);
							//if (Random == 1 && Spell_agro != 0) { me->CastSpell(victim, Spell_agro, true); }	// 1/2 Chance de lancer le sort d'agro
							if (Random == 1 && Spell_agro != 0) { DoCast(victim, Spell_agro); }	// 1/2 Chance de lancer le sort d'agro

							// ####################################################################################################################################
					}


					// ############################################################################################################################################
					// Combat 
					// Regen mana en combat -----------------------------------------------------------------------------------------------------------------------
					if (Cooldown_RegenMana <= diff)
					{
						me->SetPower(POWER_MANA, Mana + (MaxMana / 100));
						if (Mana > MaxMana) { me->SetPower(POWER_MANA, MaxMana); }

						Cooldown_RegenMana = Cooldown_RegenMana_defaut;
					}
					else Cooldown_RegenMana -= diff;

						// Spell1 sur la cible  -------------------------------------------------------------------------------------------------------------------
						if (Cooldown_Spell1 <= diff && Spell_1 !=0)
						{
							me->StopMoving();
							DoCast(victim, Spell_1);
							DoMeleeAttackIfReady();																// Combat en mélée
							Cooldown_Spell1 = urand(3500,4000);
						}
						else Cooldown_Spell1 -= diff;

						// Spell2 sur la cible  -------------------------------------------------------------------------------------------------------------------
						if (Cooldown_Spell2 <= diff && Spell_2 != 0)
						{
							me->CastSpell(victim, Spell_2, true);
							Cooldown_Spell2 = Cooldown_Spell2_defaut;
						}
						else Cooldown_Spell2 -= diff;

					Mouvement_Caster(diff);

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
			void Mouvement_Caster(uint32 diff)
			{
				if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
					return;

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				if (Cooldown_ResteADistance <= diff)
				{
					// Mouvement aléatoire si cible < 4m & mana > 10%  ---------------------------------------------------------------------------------------------

					if (Dist <4 && (Mana > MaxMana / 10))
					{
						me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);						// UNROOT
						//me->SetSpeedRate(MOVE_RUN, 1.01f);

						float x, y, z, mapid;
						x = (me->GetPositionX() + urand(0, ResteADistance * 2) - ResteADistance);
						y = (me->GetPositionY() + urand(0, ResteADistance * 2) - ResteADistance);
						z = me->GetPositionZ();
						mapid = victim->GetMapId();
						me->GetMotionMaster()->MovePoint(mapid, x, y, z);
						Cooldown_ResteADistance = Cooldown_ResteADistance_Defaut;
					}
				}
				else Cooldown_ResteADistance -= diff;

				// Mouvement OFF si distance >= 8m & <= 15m -------------------------------------------------------------------------------------------------------
				if ((Dist >= 6) && (Dist <= ResteADistance))
				{
					if (me->isMoving())																	// Sinon bug d'animation
					{
						AttackStart(victim);
						AttackStartCaster(victim, ResteADistance);										// Distance de combat
						void DoRangedAttackIfReady();													// Combat a distance
						me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);							// ROOT
					}
				}

				// Mouvement ON si distance > 20m -----------------------------------------------------------------------------------------------------------------
				if (Dist > ResteADistance)
				{
					me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);							// UNROOT
					AttackStartCaster(victim, ResteADistance);											// Distance de cast
					void DoRangedAttackIfReady();														// Combat a distance
				}

				// Mouvement ON si Mana < 10%  ---------------------------------------------------------------------------------------------------------------------
				if (Mana < MaxMana / 10)
				{
					me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);							// UNROOT
					AttackStartCaster(victim, 1);														// Distance de cast
					DoMeleeAttackIfReady();																// Combat en mélée
				}
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

			void Heal_En_Combat_Caster(uint32 diff)
			{
				//if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_MOVE))
					//return;

				if (Cooldown_Spell_Heal <= diff && Spell_Heal != 0)
				{
					// heal sur lui meme-------------------------------------------------------------------------------------------------------------------------------
					if ((me->GetHealth() < (me->GetMaxHealth()*0.60)))								// Si PV < 60%
					{
						//DoCast(me, Spell_Heal);
						DoCastAOE(Spell_Heal);
						Cooldown_Spell_Heal = Cooldown_Spell_Heal_defaut;
					}
				}
				else Cooldown_Spell_Heal -= diff;
			}
		};




		CreatureAI* GetAI(Creature* creature) const override
		{
			return new Stitch_npc_ai_casterAI(creature);
		}
};

//#################################################################################################################################################################
void AddSC_npc_ai_caster()
{
	new Stitch_npc_ai_caster();
}















