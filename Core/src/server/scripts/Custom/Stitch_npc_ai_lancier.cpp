////#########################################################################################################################################################################################################################################
// Copyright (C) Juillet 2020 Stitch pour https:\\Aquayoup.123.fr
// AI generique npc par classe : Lancier Ver 2020-11-06 (caster simple, combat a distance si 8-30 sinon combat au corp a corp 0-8m)
//
// ScriptName = Stitch_npc_ai_lancier : npc d'exemple : 15100014
// le "lancer une arme" , "Tir a l'arc" ou "Tir au fusil" est automatique de 8 a 40m et est tributaire de pickpocketloot
// Si pickpocketloot = 0 : Lancier , si pickpocketloot = 1 : Archer , si pickpocketloot = 2 : Fusilier
// spell1 : Attaque principale au corp a corp
// spell2 : Attaque Dot au corp a corp
// spell3 : spell lancé a l'agro
// spell4 : spell lancé a l'évade ou respawn
// spell5 : Buf
//
//
// Si spell1 = 0 : tirage aléatoire des spells

// Il est possible d'influencer le temp entre 2 cast avec `BaseAttackTime` & `RangeAttackTime` 
// Necessite dans Creature_Template :
// UPDATE `creature_template` SET `spell1` = ATTAQUE, `spell2` = DOT, `spell3` = AGRO, `spell4` = EVADE, `spell5` = BUF, `ScriptName` = ' Stitch_npc_ai_lancier', `AIName` = '' WHERE(entry = 2237);
// REPLACE INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`, `ItemID2`, `ItemID3`, `VerifiedBuild`) VALUES
// (15100014, 1, 44654, 0,116459, 0);	## Lance + arc
//###########################################################################################################################################################################################################################################



//################################################################################################
//StitchAI AI Lancier
//################################################################################################

class Stitch_npc_ai_lancier : public CreatureScript
{
public: Stitch_npc_ai_lancier() : CreatureScript("Stitch_npc_ai_lancier") { }

		struct Stitch_npc_ai_lancierAI : public ScriptedAI
		{
			Stitch_npc_ai_lancierAI(Creature* creature) : ScriptedAI(creature) { }

			uint32 Random;
			uint32 DistanceDeCast = 30;												// Distance max a laquelle un npc attaquera , au dela il quite le combat
			uint32 ResteADistance = 15;												// Distance max a laquelle un npc s'approchera
			uint32 Dist;															// Distance entre le npc et sa cible
			Unit* victim = me->GetVictim();										 
			uint32 Mana;
			uint32 MaxMana = me->GetMaxPower(POWER_MANA);
			uint32 ForceBranche = me->GetCreatureTemplate()->pickpocketLootId;

			// Definitions des variables Cooldown et le 1er lancement
			uint32 Cooldown_Spell1 = 1000;
			uint32 Cooldown_Spell2 = 3000;
			uint32 Cooldown_ResteADistance = 2000;									// Test si en contact
			uint32 Cooldown_ResteADistance_Defaut = 4000;
			uint32 Cooldown_Anti_Bug_Figer = 2000;
			uint32 Cooldown_Npc_Emotes = urand(5000, 8000);
			uint32 Cooldown_RegenMana = 5000;
			uint32 Cooldown_RegenMana_defaut = 3500;

			// Spells
			uint32 Buf_1 = 0;
			uint32 liste_Buf[4] = { 6673, 1160, 159362, 8599 };						// Cri de guerre 6673, Cri démoralisant 1160 (8s 10m Soi-même), Folie sanguinaire 159362 (pv 1%/3s), Enrager 8599

			uint32 Spell_agro = 0;
			uint32 liste_agro[2] = { 133308, 79444 };								// Lancer le filet 133308 (25m 5s), Empaler 79444 (Jette une lance 60m) 

			uint32 Spell_1;
			uint32 liste_spell_1[4] = { 78, 126799, 118326, 172851 };				// Frappe héroïque 78, Frappe tranchante 126799, Attaque vicieuse 118326, Enchaînement 172851

			uint32 Spell_2 = 0;
			uint32 liste_spell_2[4] = { 127171, 118532, 36991, 78660 };				// Fendoir vicieux 15/lvl + 2/lvl/1s cumulable 5 fois 127171, Entaille infectée 118532, Pourfendre 36991, Dévaster 78660

			uint32 Lancer_une_Arme = 42332;											// 8-40m Empaler
			uint32 Tir_Arc = 95826;
			uint32 Tir_Fusil = 6660;
			uint32 Tir_1 = 42332;
			uint32 Spell_respawn_evade = 22812; 									// Ecorce 22812;

			// Emotes
			uint32 Npc_Emotes[22] = { 1,3,7,11,15,16,19,21,22,23,24,53,66,71,70,153,254,274,381,401,462,482 };

			uint32 Start_Agro = 0;

			void Init_AI()
			{

				// ################################################################################################################################################
				// Définition des spells
				// ################################################################################################################################################

				// Si aucun spell défini dans creature_template->spell[1] : tirage aléatoire des sorts
				if (me->m_spells[0] == 0) 
				{ 
						Buf_1 = liste_Buf[urand(0, 3)];
						Spell_agro = liste_agro[urand(0, 1)];
						Spell_1 = liste_spell_1[urand(0, 3)];
						Spell_2 = liste_spell_2[urand(0, 3)];
						Cooldown_ResteADistance_Defaut = 4000;
						Cooldown_RegenMana_defaut = 4000;
				}
				else 
				{ 
					Spell_1 = me->m_spells[0]; 
					Spell_2 = me->m_spells[1];
					Spell_agro = me->m_spells[2]; 
					Spell_respawn_evade = me->m_spells[3];
					Buf_1 = me->m_spells[4];
				}

				// Forcer le choix par creature_template->pickpocketloot . Lancier = 0 , Archer = 1 , Fusilier = 2
				if (ForceBranche == 0) { Tir_1 = Lancer_une_Arme; }									// Lance une arme
				if (ForceBranche == 1) { Tir_1 = Tir_Arc; }											// Tir a l'arc
				if (ForceBranche == 2) { Tir_1 = Tir_Fusil;	}										// Tir au fusil
				

				


				// ################################################################################################################################################
				// Divers
				// ################################################################################################################################################
				me->SetSheath(SHEATH_STATE_MELEE);													// S'equipe de l'arme au contact
				me->SetReactState(REACT_AGGRESSIVE);
				me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);								// ROOT
				// ################################################################################################################################################
			}
			void JustRespawned() override
			{
				me->GetMotionMaster()->MoveTargetedHome();								// Retour home pour rafraichir client
				me->SetSpeedRate(MOVE_RUN, 1.01f);
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
				me->GetMotionMaster()->MoveTargetedHome();											// Retour home
				me->RemoveAllControlled();															// renvois pet
				me->RemoveAura(Buf_1);

				Random = urand(1, 3);
				if (Random == 1 && Spell_respawn_evade != 0) { me->CastSpell(me, Spell_respawn_evade, true); }		// 1/3 Chance de lancer le sort au respawn ou evade
			}
			void JustReachedHome() override
			{
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

					if (Start_Agro == 0)
					{
						Start_Agro = 1;


						// Spell a lancer a l'agro ------------------------------------------------------------------------------------------------------------------------
						Random = urand(1, 3);
						if (Random == 1 && Buf_1 != 0) { me->CastSpell(me, Buf_1, true); }

						Random = urand(1, 3);
						if (Random == 1 && Spell_agro != 0 && Dist > 8) { DoCastAOE(Spell_agro,true); }	// 1/3 Chance de lancer le sort d'agro

					}

					// ############################################################################################################################################
					// Regen mana en combat -----------------------------------------------------------------------------------------------------------------------
					if (Cooldown_RegenMana <= diff)
					{
						me->SetPower(POWER_MANA, Mana + (MaxMana / 100));
						if (Mana > MaxMana) { me->SetPower(POWER_MANA, MaxMana); }

						Cooldown_RegenMana = Cooldown_RegenMana_defaut;
					}
					else Cooldown_RegenMana -= diff;

					// Combat au contat ---------------------------------------------------------------------------------------------------------------------------
					if (Dist < 6)
					{
						// Spell1 sur la cible  
						if (Cooldown_Spell1 <= diff && Spell_1 !=0)
						{
							me->StopMoving();
							me->SetSheath(SHEATH_STATE_MELEE);													// S'équipe d'armes au contact
							Bonus_Degat_Arme_Done(100);
							DoCast(victim, Spell_1);
							DoMeleeAttackIfReady();																// Combat en mélée
							Bonus_Degat_Arme_Done(-100);
							Cooldown_Spell1 = urand(2000,2500);
						}
						else Cooldown_Spell1 -= diff;

						// Spell2 sur la cible  
						if (Cooldown_Spell2 <= diff && Spell_2 != 0)
						{
							me->SetSheath(SHEATH_STATE_MELEE);
							me->CastSpell(victim, Spell_2, true);
							Cooldown_Spell2 = 6000;
						}
						else Cooldown_Spell2 -= diff;
					}
					// Combat a distance --------------------------------------------------------------------------------------------------------------------------
					if (Dist >= 6)
					{
						// Spell1 sur la cible  
						if (Cooldown_Spell1 <= diff)
						{
							if (Tir_1 != Lancer_une_Arme) { me->SetSheath(SHEATH_STATE_RANGED); }				// S'équipe d'arc ou fusil
							if (Tir_1 == Lancer_une_Arme) { me->SetSheath(SHEATH_STATE_MELEE);	}				// S'équipe d'armes au contact

							DoCast(victim, Tir_1);
							Cooldown_Spell1 = urand(1500,2000);
						}
						else Cooldown_Spell1 -= diff;
					}

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
				if (!UpdateVictim() /*|| me->HasUnitState(UNIT_STATE_CASTING)*/)
					return;

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				if (Cooldown_ResteADistance <= diff)
				{
				// Mouvement ON si distance < 8m ------------------------------------------------------------------------------------------------------------------
				if ( Dist < 8)
				{
					me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);							// UNROOT
					AttackStartCaster(victim, 1);														// Distance de cast
					DoMeleeAttackIfReady();																// Combat en mélée
				}

				// Mouvement ON si distance > 15m -----------------------------------------------------------------------------------------------------------------
				if (Dist > ResteADistance + 5)
				{
					me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);							// UNROOT
					AttackStartCaster(victim, ResteADistance);											// Distance de cast
					void DoRangedAttackIfReady();														// Combat a distance
				}

				// Mouvement OFF si distance >= 8m & <= 15m -------------------------------------------------------------------------------------------------------
				if ((Dist >= 8) && (Dist <= ResteADistance + 5))
				{
					if (me->isMoving())																	// Sinon bug d'animation
					{
						AttackStart(victim);
						AttackStartCaster(victim, ResteADistance);										// Distance de combat
						void DoRangedAttackIfReady();													// Combat a distance
						me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);							// ROOT
					}
				}
				Cooldown_ResteADistance = 2000;
				}
				else Cooldown_ResteADistance -= diff;

			}

			void Bonus_Degat_Arme_Done(int val) // 
			{
				// +- Bonus en % de degat des armes infligées a victim
				me->HandleStatModifier(UNIT_MOD_ATTACK_POWER, TOTAL_PCT, val, true);
				me->HandleStatModifier(UNIT_MOD_DAMAGE_OFFHAND, TOTAL_PCT, val, true);
				me->HandleStatModifier(UNIT_MOD_DAMAGE_RANGED, TOTAL_PCT, val, true);
			}



		};




		CreatureAI* GetAI(Creature* creature) const override
		{
			return new Stitch_npc_ai_lancierAI(creature);
		}
};

//#################################################################################################################################################################
void AddSC_npc_ai_lancier()
{
	new Stitch_npc_ai_lancier();
}















