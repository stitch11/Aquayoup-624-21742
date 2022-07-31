//###########################################################################################################################################################################################################################################
// Copyright (C) Juin 2020 Stitch pour Aquayoup
// AI generique npc par classe : DRUIDE Ver 2022-07-31
// Il est possible d'influencer le temp entre 2 cast avec `BaseAttackTime` & `RangeAttackTime` 
// Necessite dans Creature_Template :
// Minimun  : UPDATE `creature_template` SET `ScriptName` = 'Stitch_npc_ai_druide',`AIName` = '' WHERE (entry = 15100001);
// Optionel : UPDATE `creature_template` SET `HealthModifier` = 2, `ManaModifier` = 3, `ArmorModifier` = 1, `DamageModifier` = 2,`BaseAttackTime` = 2000, `RangeAttackTime` = 2000 WHERE(entry = 15100001);
// Optionel : Utilisez pickpocketloot de creature_template pour passer certains parametres (Solution choisit afin de rester compatible avec tout les cores). Si pickpocketloot = 1 (branche1 forcé), pickpocketloot = 2 (branche2 forcé), etc
//###########################################################################################################################################################################################################################################
// # npc de Test Stitch_npc_ai_druide  .npc 15100001
// REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `HealthModifier`, `HealthModifierExtra`, `ManaModifier`, `ManaModifierExtra`, `ArmorModifier`, `DamageModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES
// (15100001, 0, 0, 0, 0, 0, 55745, 0, 0, 0, 'npc_ai_Druide', '', '', '', 0, 90, 90, 0, 0, 2102, 0, 1.01, 1.01, 0.8, 0, 0, 2000, 2000, 1, 1, 2, 0, 2048, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 2, 1, 3, 1, 1.5, 2, 1, 0, 144, 1, 0, 0, 'Stitch_npc_ai_druide', 20173);
// REPLACE INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`, `ItemID2`, `ItemID3`, `VerifiedBuild`) VALUES
// (15100001, 1, 69208, 0, 0, 0);
//###########################################################################################################################################################################################################################################



#include "CreatureTextMgr.h"

//################################################################################################
//StitchAI AI Druide
//################################################################################################

class Stitch_npc_ai_druide : public CreatureScript
{
public: Stitch_npc_ai_druide() : CreatureScript("Stitch_npc_ai_druide") { }

		struct Stitch_npc_ai_druideAI : public ScriptedAI
		{
			Stitch_npc_ai_druideAI(Creature* creature) : ScriptedAI(creature) { }

			uint32 BrancheSpe = 1;													// Choix de la Spécialisation : Equilibre=1, Ours=2, Felin=3, Tréant=4
			uint32 NbrDeSpe = 4;													// Nombre de Spécialisations
			uint32 ForceBranche;
			uint32 DistanceDeCast = 40;												// Distance max a laquelle un npc attaquera , au dela il quite le combat
			uint32 ResteADistance = 5;												// Distance max a laquelle un npc s'approchera
			uint32 Dist;															// Distance entre le npc et sa cible
			uint32 Random;
			uint32 Mana;
			uint32 MaxMana = me->GetMaxPower(POWER_MANA);
			uint32 MessageAlagro = 0;
			uint32 Spell_ContreAttaque = 0;
			Unit* victim = me->GetVictim();											// La cible du npc

			uint32 Modelid_Branche2 = 2281;											// Modelid Ours 2281 , Gros Ours 1990
			uint32 Modelid_Branche3 = 892;											// Modelid félin 892
			uint32 Modelid_Branche4 = 864;											// Tréant 864 , Modelid Petite-Branche 57040

			// Spells Divers
			uint32 Buf_all = 1126;													// Marque du fauve 1126
			uint32 Buf_branche1 = 22812;											// Ecorce = 22812
			uint32 Buf_branche2 = 15727;											// Rugissement démoralisant 15727
			uint32 Buf_branche3 = 33907;											// Epines 33907, Hurlement furieux 3149 (force +15, Rouge)
			uint32 Buf_branche4 = 33907;											// Epines 33907
			uint32 Drain_de_vie = 46155;											// Drain de vie 46155/689
			uint32 Charge_ours = 32323;												// Charge
			uint32 BOND_Aleatoire = 70485;											// BOND Aleatoire
			uint32 Empaler_et_tirer = 82742;										// Empaler et tirer
			uint32 Barbeles_depines = 113967;										// Barbelés d'épines 
			uint32 Lenteur_Treant = 6146;											// Lenteur
			uint32 Spell_Heal_Caster = 300261;  									// Toucher guérisseur 300261/5185
			uint32 Griffure_Bondissante = 89712;									// Griffure Bondissante (saut sur la cible + stun)
			uint32 Cooldown_Spell_ContreAttaque = 4000;
			uint32 Cooldown_Spell_ContreAttaque_defaut = 8000;

			// Spells Equilibre
			uint32 Spell_branche1_agro = 0;
			uint32 Spell_branche1_1 = 0;
			uint32 Spell_branche1_2 = 0;
			uint32 Spell_branche1_3 = 0;
			uint32 branche1_agro[4] = { 21668, 21668, 21668, 33844 };				// Feu stellaire 21668, Sarments (4s) 33844
			uint32 branche1_1[2] = { 119577, 119577 };								// Colère (verte) 119577 
			uint32 branche1_2[2] = { 98995, 770 };									// Éruption stellaire 98995, Lucioles 770
			uint32 branche1_3[2] = { 15798, 21668 };								// Eclat lunaire 15798, Feu stellaire 21668
																					
			// Definitions des Spells Ours
			uint32 Spell_branche2_agro = 0;
			uint32 Spell_branche2_1 = 0;
			uint32 Spell_branche2_2 = 0;
			uint32 Spell_branche2_3 = 0;
			uint32 branche2_agro[3] = { 32323, 22863, 19196 };						// Charge 32323, Vitesse 22863 (30%) , charge afflux 19196 (charge+renverse)
			uint32 branche2_1[2] = { 300206, 84867 };								// Mutiler 300206, Balayage fracassant 84867
			uint32 branche2_2[2] = { 300207, 300208 };								// Mutilation (mob ours) 300207, Rosser Ours 300208
			uint32 branche2_3[5] = { 131172, 300209, 138766, 10887, 127987 };		// Rugissement perçant 138766 (interrompt ,empêchant les sorts de la même école pendant 6 sec)
																					// Coup de tete 131172, Coup de crâne 300209 (dps+interrompt  4s), faucher la foule 10887 (interomp, repousse 10 m), *Morsure hémorragique 127987
			// Spells Felin
			uint32 Spell_branche3_agro = 0;
			uint32 Spell_branche3_1 = 0;
			uint32 Spell_branche3_2 = 0;
			uint32 Spell_branche3_3 = 0;
			uint32 branche3_agro[3] = { 22863, 89712, 59737 };						// Vitesse +30% 22863, Griffure bondissante 89712 (Etourdi 2s),vitesse +100% 59737
			uint32 branche3_1[2] = { 31289, 300206 };								// Griffe 31289, Mutiler 300206
			uint32 branche3_2[2] = { 113687, 138765 };								// Morsure 113687, Écraser 138765 (armure -10%/30s)
			uint32 branche3_3[4] = { 300220, 300220, 83639, 30639 };				// Griffure 300220, Bain de sang 83639 (6 attaque /3s), Morsure carnivore 30639

			// Spells Tréant 
			uint32 Spell_branche4_agro = 0;
			uint32 Spell_branche4_1 = 0;
			uint32 Spell_branche4_2 = 0;
			uint32 Spell_branche4_3 = 0;
			uint32 branche4_agro[3] = { 33844, 82742, 82742 };						// Sarments 4s 33844, *Empaler et tirer 82742
			uint32 branche4_1[3] = { 131193, 119004, 38742 };						// Choc 131193, Violent coup direct 119004, Enchaînement gangrené 38742
			uint32 branche4_2[4] = { 80515, 113967, 113967, 118682 };				// Frénésie immortelle 80515 (Canalisée 5s), Barbelés d'épines 113967, Taper du pied 118682
			uint32 branche4_3[4] = { 129375, 46155, 152571, 152571 };				// Choc terrestre 129375 (stun 3s), Drain de vie 46155, Encorner 152571 (cone 5m)


		// Definitions des variables Cooldown et le 1er lancement
			uint32 Cooldown_Spell1 = 500;
			uint32 Cooldown_Spell2 = 2500;
			uint32 Cooldown_Spell3 = 4000;
			uint32 Cooldown_Spell_Heal = 3000;
			uint32 Cooldown_RegenMana = 3000;
			uint32 Cooldown_ResteADistance = 2000;									// Test si en contact pour Equilibre pour s'eloigner, bouger en combat pour le Felin
			uint32 Cooldown_ResteAuContact;
			uint32 Cooldown_Anti_Bug_Figer = 2000;
			uint32 Cooldown_Charge = 8000;
			uint32 Cooldown_Npc_Emotes = urand(5000, 8000);

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
				else if (ForceBranche == 3) { BrancheSpe = 3; }										// branche3 forcé
				else if (ForceBranche == 4) { BrancheSpe = 4; }										// branche4 forcé
				else
				{
					// Sinon Choix de la Spécialisation Aléatoire
					BrancheSpe = urand(1, NbrDeSpe + 1);

					if ((BrancheSpe > NbrDeSpe) || (BrancheSpe == 0)) { BrancheSpe = 1; }

					me->RemoveAllControlled();															// renvois pet
				}

				// ################################################################################################################################################
				// Tirages aléatoires des spells
				// ################################################################################################################################################
				// Spell a lancer a l'agro ------------------------------------------------------------------------------------------------------------------------

				// Message a l'agro forcé par spell(8)
				if (me->m_spells[7] == 1) { MessageAlagro = 1; }

				// Spell contre attaque si PV bas
				if (me->m_spells[6] != 0) { Spell_ContreAttaque = me->m_spells[6]; }


				me->CastSpell(me, Buf_all, true);												// Buf_all sur lui meme pour toutes les Spécialitées
				Start_Agro = 0;

				switch (BrancheSpe)
				{
				case 1: // Si Equilibre ---------------------------------------------------------------------------------------------------------------------------
					//me->SetSpeedRate(MOVE_RUN, 1.01f);
					me->CastSpell(me, Buf_branche1, true);
					me->LoadEquipment(1, true);													// creature_equip_template 1

					me->CastSpell(me, Buf_branche1, true);										// Buf2 sur lui meme

					VisuelMana();

					// Tirages aléatoires des spells Equilibre 
					Spell_branche1_agro = branche1_agro[urand(0, 3)];
					Spell_branche1_1 = branche1_1[urand(0, 1)];
					Spell_branche1_2 = branche1_2[urand(0, 1)];
					Spell_branche1_3 = branche1_3[urand(0, 1)];

					// Reste a distance variable suivant ci le mob est a l'extérieur ou a l'Intérieur
					if (me->GetMap()->IsOutdoors(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()))
					{
						ResteADistance = urand(14, 16);
					}
					else
					{
						ResteADistance = 10;
					}
					break;

				case 2: // Si Ours --------------------------------------------------------------------------------------------------------------------------------
					me->SetSpeedRate(MOVE_RUN, 1.1f);											// Vitesse de déplacement
					me->CastSpell(me, Buf_branche2, true);										// Buf2 sur lui meme
					me->LoadEquipment(2, true);													// creature_equip_template 2

					Bonus_Armure(250);															// Bonus d'armure +150%

					// Pour visuel seulement
					me->SetDisplayId(Modelid_Branche2);											// Modelid Ours
					VisuelRage();

					// Tirages aléatoires des spells Ours 
					Spell_branche2_agro = branche2_agro[urand(0, 2)];
					Spell_branche2_1 = branche2_1[urand(0, 1)];
					Spell_branche2_2 = branche2_2[urand(0, 1)];
					Spell_branche2_3 = branche2_3[urand(0, 4)];
					break;

				case 3: // Si Felin -------------------------------------------------------------------------------------------------------------------------------
					me->SetSpeedRate(MOVE_RUN, 1.1f);											// Vitesse de déplacement
					me->CastSpell(me, Buf_branche3, true);										// Buf3 sur lui meme
					me->LoadEquipment(3, true);													// creature_equip_template 2

					Bonus_Armure(150);															// Bonus d'armure +50%
					
					// Pour visuel seulement
					me->SetDisplayId(Modelid_Branche3);											// Modelid Felin
					VisuelEnergy();

					// Tirages aléatoires des spells Elementaire 
					Spell_branche3_agro = branche3_agro[urand(0, 2)];
					Spell_branche3_1 = branche3_1[urand(0, 1)];
					Spell_branche3_2 = branche3_2[urand(0, 1)];
					Spell_branche3_3 = branche3_3[urand(0, 3)];
					break;

				case 4: // Si Spécialisation Tréant ---------------------------------------------------------------------------------------------------------
					me->SetSpeedRate(MOVE_RUN, 0.9f);
					me->CastSpell(me, Buf_branche4, true);										// Buf4 sur lui meme
					me->LoadEquipment(0, true);													// creature_equip_template 0

					Bonus_Armure(200);															// Bonus d'armure +100%

					// Pour visuel seulement
					me->SetDisplayId(Modelid_Branche4);											// Modelid Tréant - Petite-Branche 57040
					VisuelMana();

					// Tirages aléatoires des spells Petite-Branche 
					Spell_branche4_agro = branche4_agro[urand(0, 2)];
					Spell_branche4_1 = branche4_1[urand(0, 2)];
					Spell_branche4_2 = branche4_2[urand(0, 3)];
					Spell_branche4_3 = branche4_3[urand(0, 3)];
					break;
				}
				// ################################################################################################################################################
				// Divers
				// ################################################################################################################################################
				me->SetSheath(SHEATH_STATE_MELEE);												// S'equipe de l'arme au contact
				me->SetReactState(REACT_AGGRESSIVE);
				// ################################################################################################################################################
			}
			void JustRespawned() override
			{
				// Pour visuel seulement
				me->setPowerType(POWER_MANA);
				me->SetMaxPower(POWER_MANA, MaxMana);
				me->SetPower(POWER_MANA, MaxMana);
				me->RestoreDisplayId();													// Retire Morph

				me->GetMotionMaster()->MoveTargetedHome();								// Retour home pour rafraichir client
				//me->SetSpeedRate(MOVE_RUN, 1.01f);
				me->SetReactState(REACT_AGGRESSIVE);

				Init_AI();
			}

			void EnterCombat(Unit* /*who*/) override
			{
				Init_AI();

				if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == WAYPOINT_MOTION_TYPE)
				{
					me->StopMoving();
					me->GetMotionMaster()->MoveIdle();
				}

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
				me->RemoveAura(Buf_branche2);
				me->RemoveAura(Buf_branche3);
			}
			void JustReachedHome() override
			{
				// Pour visuel seulement
				me->setPowerType(POWER_MANA);
				me->SetMaxPower(POWER_MANA, MaxMana);
				me->SetPower(POWER_MANA, MaxMana);

				me->DeMorph();

				me->RemoveAura(Buf_all);
				me->RemoveAura(Buf_branche1);
				me->RemoveAura(Buf_branche2);
				me->RemoveAura(Buf_branche3);
				me->RemoveAura(Buf_branche4);

				me->SetReactState(REACT_AGGRESSIVE);
				//me->SetSpeedRate(MOVE_RUN, 1.01f);										// Vitesse par defaut définit a 1.01f puisque le patch modification par type,famille test si 1.0f

				Bonus_Armure(100);														// Retire bonus d'armure
				me->SetReactState(REACT_AGGRESSIVE);
			}
			void UpdateAI(uint32 diff) override
			{
				if (me->HasUnitState(UNIT_STATE_CONTROLLED) /*||me->HasUnitState(UNIT_STATE_CONFUSED) || me->HasUnitState(UNIT_STATE_STUNNED) || me->HasUnitState(UNIT_STATE_DISTRACTED) || me->HasUnitState(UNIT_STATE_CANNOT_TURN) || me->HasUnitState(UNIT_STATE_POSSESSED) || me->HasUnitState(UNIT_STATE_CONFUSED_MOVE)*/)
					return;
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
					Mana = me->GetPower(POWER_MANA);
					Unit* victim = me->GetVictim();
					Dist = me->GetDistance(victim);

					if (Start_Agro == 0)
					{
						Start_Agro = 1;

						// Message a l'agro , ci le mob a plusieurs lignes (creature_text groupid>0) il y a de forte chance que ce soit pour un dialogue
						// et non un simple message a l'agro. Donc on l'ignore.
						Random = urand(1, 5);
						if (!sCreatureTextMgr->TextExist(me->GetEntry(), 1) && Random == 1 || MessageAlagro == 1)
						{
							Talk(0);
						}

						// ########################################################################################################################################
						// Spell a lancer a l'agro 
						// ########################################################################################################################################
						switch (BrancheSpe)
						{
						case 1: // Si Equilibre ----------------------------------------------------------------------------------------------------------------
							// Pour visuel seulement
							me->DeMorph(); 
							VisuelMana();
							Random = urand(1, 3);
							if (Random == 1 && UpdateVictim()) { DoCastVictim(Spell_branche1_agro); }	// 1/3 Chance de lancer le sort d'agro
							break;

						case 2: // Si Ours -----------------------------------------------------------------------------------------------------------------
							// Pour visuel seulement
							me->DeMorph();
							VisuelRage();
							me->SetDisplayId(Modelid_Branche2);											// Modelid Ours

							Random = urand(1, 2);
							if (Random == 1) { me->CastSpell(victim, Spell_branche2_agro, true); }		// 1/2 Chance de lancer le sort d'agro
							break;

						case 3: // Si Felin ----------------------------------------------------------------------------------------------------------------
							// Pour visuel seulement
							me->DeMorph(); 
							VisuelEnergy();
							me->SetDisplayId(Modelid_Branche3);											// Modelid Felin

							Random = urand(1, 2);
							if (Random == 1) { me->CastSpell(victim, Spell_branche3_agro, true); }		// 1/2 Chance de lancer le sort d'agro
							break;

						case 4: // Si Tréant ----------------------------------------------------------------------------------------------------------------
							// Pour visuel seulement
							me->DeMorph(); 
							VisuelMana();
							me->SetDisplayId(Modelid_Branche4);											// Modelid Tréant - Petite-Branche 57040

							Random = urand(1, 2);
							if (Random == 1) { me->CastSpell(victim, Spell_branche4_agro, true); }		// 1/2 Chance de lancer le sort d'agro
							break;

							// ####################################################################################################################################
						}
					}


					// ############################################################################################################################################
					// Combat suivant la Spécialisation
					switch (BrancheSpe)
					{
					case 1: // Spécialisation Equilibre ###########################################################################################################
							// Regen mana en combat ---------------------------------------------------------------------------------------------------------------
						if (Cooldown_RegenMana <= diff)
						{
							me->SetPower(POWER_MANA, Mana + (MaxMana / 5));
							if (Mana > MaxMana) { me->SetPower(POWER_MANA, MaxMana); }
							Cooldown_RegenMana = urand(1000, 1500);
						}
						else Cooldown_RegenMana -= diff;

						// Combat ---------------------------------------------------------------------------------------------------------------------------------
						// Spell3 sur la cible  (Sort secondaire tres lent , généralement utilisé comme Dot)
						if (Cooldown_Spell3 <= diff)
						{
							//DoCastVictim(Spell_branche1_3);
							me->StopMoving();
							me->CastSpell(victim, Spell_branche1_3, true);
							Cooldown_Spell3 = urand(10000, 12000);
						}
						else Cooldown_Spell3 -= diff;

						// Spell2 sur la cible chaque (Sort secondaire plus lent)
						if (Cooldown_Spell2 <= diff)
						{
							DoCastVictim(Spell_branche1_2);
							Cooldown_Spell2 = urand(5000, 6000);
						}
						else Cooldown_Spell2 -= diff;

						// Spell1 sur la cible chaque (Sort Régulié)
						if (Cooldown_Spell1 <= diff)
						{
							if (!me->HasUnitState(UNIT_STATE_MOVE))
							{
								DoCastVictim(Spell_branche1_1);
								Cooldown_Spell1 = 3500;
							}
						}
						else Cooldown_Spell1 -= diff;

						Heal_En_Combat_Caster(diff);
						Mouvement_Caster(diff);
						break;

					case 2: // Spécialisation Ours ################################################################################################################
							// Regen mana en combat ---------------------------------------------------------------------------------------------------------------
						if (Cooldown_RegenMana <= diff)
						{
							me->SetPower(POWER_MANA, Mana + (MaxMana / 5));
							if (Mana > MaxMana) { me->SetPower(POWER_MANA, MaxMana); }
							Cooldown_RegenMana = urand(1500, 2000);
						}
						else Cooldown_RegenMana -= diff;

						// Combat ---------------------------------------------------------------------------------------------------------------------------------
						if (Dist < 6)
						{
							// Spell1 sur la cible chaque (Sort Régulié)
							if (Cooldown_Spell1 <= diff)
							{
								DoCastVictim(Spell_branche2_1);
								Cooldown_Spell1 = 3000;
							}
							else Cooldown_Spell1 -= diff;

							// Spell2 sur la cible chaque (Sort secondaire plus lent)
							if (Cooldown_Spell2 <= diff)
							{
								//DoCastVictim(Spell_branche2_2);
								DoCast(victim, Spell_branche2_2);
								Cooldown_Spell2 = 4000;
							}
							else Cooldown_Spell2 -= diff;

							// Spell3 sur la cible  (Sort secondaire tres lent , généralement utilisé comme Dot)
							if (Cooldown_Spell3 <= diff)
							{
								DoCastVictim(Spell_branche2_3);
								Cooldown_Spell3 = urand(7000,9000);
							}
							else Cooldown_Spell3 -= diff;
						}

						Mouvement_Ours(diff);
						break;

					case 3: // Spécialisation Felin ###############################################################################################################
							// Regen mana en combat ---------------------------------------------------------------------------------------------------------------
						if (Cooldown_RegenMana <= diff)
						{
							me->SetPower(POWER_MANA, Mana + (MaxMana / 10));
							if (Mana > MaxMana) { me->SetPower(POWER_MANA, MaxMana); }
							Cooldown_RegenMana = 1000;
						}
						else Cooldown_RegenMana -= diff;


						// Combat ---------------------------------------------------------------------------------------------------------------------------------
						if (Dist < 6)
						{
							// Spell1 sur la cible chaque (Sort Régulié)
							if (Cooldown_Spell1 <= diff)
							{
								DoCastVictim(Spell_branche3_1);
								Cooldown_Spell1 = 2750;
							}
							else Cooldown_Spell1 -= diff;

							// Spell2 sur la cible chaque (Sort secondaire plus lent)
							if (Cooldown_Spell2 <= diff)
							{
								DoCastVictim(Spell_branche3_2);
								Cooldown_Spell2 = 4000;
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

						Mouvement_Felin(diff);
						break;

					case 4: // Spécialisation Tréant ##############################################################################################################
							// Regen mana en combat ---------------------------------------------------------------------------------------------------------------
						if (Cooldown_RegenMana <= diff)
						{
							me->SetPower(POWER_MANA, Mana + (MaxMana / 5));
							if (Mana > MaxMana) { me->SetPower(POWER_MANA, MaxMana); }
							Cooldown_RegenMana = urand(1500, 2000);
						}
						else Cooldown_RegenMana -= diff;

						// Combat -------------------------------------------------------------------------------------------------------------------------------------
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
								me->CastSpell(victim, Lenteur_Treant, true);										//Lenteur 6146 sur la cible
								me->CastSpell(me, Lenteur_Treant, true);											//Lenteur 6146 sur lui
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



						Mouvement_Treant(diff);
						break;

					}

					// ############################################################################################################################################

				}
				// ################################################################################################################################################

				ContreAttaque(diff);
				Mouvement_All();
			}

			void RetireBugDeCombat()
			{
				me->DeMorph();
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

				Mana = me->GetPower(POWER_MANA);
				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				if (Cooldown_ResteADistance <= diff)
				{
					// Mouvement aléatoire si cible < 6m & Mana > 5% --------------------------------------------------------------------------------------------------

					if ((Dist <6) && (Mana > MaxMana / 20))
					{
						me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);						// UNROOT
						
						if (AuraLenteur() == false)
						{
							me->SetSpeedRate(MOVE_RUN, 1.2f); // Uniquement si non ralenti par un spell 
						}

						float x = 0.0f, y = 0.0f, z = 0.0f;
						uint32 mapid = 0;

						x = (me->GetPositionX() + urand(0, ResteADistance * 2) - ResteADistance);
						y = (me->GetPositionY() + urand(0, ResteADistance * 2) - ResteADistance);
						z = me->GetPositionZ();
						mapid = victim->GetMapId();
						me->GetMotionMaster()->MovePoint(mapid, x, y, z);
						Cooldown_ResteADistance = 2000;
					}
				}
				else Cooldown_ResteADistance -= diff;

				// Speed normal si distance > 10m ------------------------------------------------------------------------------------------------------------------
				if (Dist> ResteADistance && me->GetSpeedRate(MOVE_RUN) == 1.2f && AuraLenteur() == false)
				{
					me->SetSpeedRate(MOVE_RUN, 1.0f);
				}

				// Mouvement OFF si Mana > 5% & distance >= 6m & <= 10m ---------------------------------------------------------------------------------------------
				if ((Dist >= ResteADistance - 4) && (Dist <= ResteADistance))
				{
					if (Mana > MaxMana / 20)
					{
						if (me->isMoving())																	// Sinon bug d'animation
						{
							AttackStart(victim);
							AttackStartCaster(victim, ResteADistance);										// Distance de combat
							void DoRangedAttackIfReady();													// Combat a distance
							me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);							// ROOT
						}
					}
				}

				// Mouvement ON si distance > 15m ------------------------------------------------------------------------------------------------------------------
				if (Dist > ResteADistance)
				{
					me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);						// UNROOT
					AttackStartCaster(victim, ResteADistance);										// Distance de cast
					void DoRangedAttackIfReady();													// Combat a distance
				}

				// Mouvement ON si Mana < 5%  ----------------------------------------------------------------------------------------------------------------------
				if (Mana < MaxMana / 20)
				{
					me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);						// UNROOT
				}

			}
			void Mouvement_Ours(uint32 diff)
			{
				if (!UpdateVictim())
					return;

				Mana = me->GetPower(POWER_MANA);
				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				DoMeleeAttackIfReady();														// Combat en mélée

				// Si la cible >= 8m (pour éviter bug de rester figé) ---------------------------------------------------------------------------------------------
				if (Dist >= 8 && Cooldown_Anti_Bug_Figer <= diff && !me->HasAura(122) && !me->HasAura(3600) && !me->HasAura(6474))
				{
					float x = 0.0f, y = 0.0f, z = 0.0f;
					uint32 mapid = 0;
					x = victim->GetPositionX();
					y = victim->GetPositionY();
					z = victim->GetMap()->GetHeight(me->GetPhaseMask(), x, y, MAX_HEIGHT, true);
					me->GetClosePoint(x, y, z, me->GetObjectSize() / 3, 3);
					me->GetMotionMaster()->MovePoint(mapid, x, y, z);
					me->GetMotionMaster()->MoveChase(victim, 1, frand(0, 6.2836f));			// Pour suivre la cible avec un angle
					Cooldown_Anti_Bug_Figer = 2000;
				}
				else Cooldown_Anti_Bug_Figer -= diff;

				// Si la cible est entre 10 & 20m : Charge --------------------------------------------------------------------------------------------------------
				if (Cooldown_Charge <= diff)
				{
					Random = urand(1, 2);
					if ((Dist >= 10) && (Dist <= 20))
					{
						if (Random == 1)
						{
							DoCastVictim(Charge_ours);						// Charge 32323 - 1 chance sur 2
						}
						Cooldown_Charge = 15000;
					}
				}
				else Cooldown_Charge -= diff;
			}
			void Mouvement_Felin(uint32 diff)
			{
				if (!UpdateVictim())
					return;

				Mana = me->GetPower(POWER_MANA);
				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				DoMeleeAttackIfReady();

				// Si la cible >= 6m (pour éviter bug de rester figé) ---------------------------------------------------------------------------------------------

				if (Cooldown_Anti_Bug_Figer <= diff)
				{
					if (Dist >= 6 && !me->HasAura(122) && !me->HasAura(3600) && !me->HasAura(6474))
					{
						float x = 0.0f, y = 0.0f, z = 0.0f;
						uint32 mapid = 0;
						x = victim->GetPositionX();
						y = victim->GetPositionY();
						z = victim->GetMap()->GetHeight(me->GetPhaseMask(), x, y, MAX_HEIGHT, true);
						me->GetClosePoint(x, y, z, me->GetObjectSize() / 2, 2);
						me->GetMotionMaster()->MovePoint(mapid, x, y, z);
						me->GetMotionMaster()->MoveChase(victim, 1, frand(0, 6.2836f));			// Pour suivre la cible avec un angle
						Cooldown_Anti_Bug_Figer = 1000;

						if (AuraLenteur() == false)
						{
							me->SetSpeedRate(MOVE_RUN, 1.3f);									// Uniquement si non ralenti par un spell joueur
						}
					}
				}
				else Cooldown_Anti_Bug_Figer -= diff;

				// Si la cible est entre 8 & 20m : Griffure bondissante --------------------------------------------------------------------------------------------------------
				if (Cooldown_Charge <= diff)
				{
					if ((Dist >= 8) && (Dist <= 20))
					{
						Random = urand(1, 4);
						if (Random == 1)
						{
							DoCastVictim(Griffure_Bondissante);									// Griffure bondissante - 1 chance sur 4 
						}
						Cooldown_Charge = urand(15000, 20000);
						Cooldown_ResteADistance = urand(2000, 3000);							// Sinon bugue BOND Aleatoire ou Avance !?
					}
				} else Cooldown_Charge -= diff;


				// Si la cible < 6m : BOND Aleatoire ou tourne au tour de sa victime ------------------------------------------------------------------------------------------
				if (Cooldown_ResteADistance <= diff)
				{
					if (Dist < 6 && !me->HasAura(122) && !me->HasAura(3600) && !me->HasAura(6474))
					{
						if (AuraLenteur() == false)
						{
							me->SetSpeedRate(MOVE_RUN, 1.2f);									// Uniquement si non ralenti par un spell joueur
						}

						Random = urand(1, 5);
						if (Random == 1)
						{
							DoCast(me, BOND_Aleatoire);											// 1 chance sur 5 BOND Aleatoire 70485 Au contact
						}
						else
						{
							// Au contact tourne au tour de sa victime --------------------------------------------------------------------------------------------------
							Tourne_Au_Tour_En_Combat();											// 4 chance sur 5 avance Au contact
						}
						Cooldown_ResteADistance = urand(3500, 5000);
					}
				}
				else Cooldown_ResteADistance -= diff;

			}
			void Mouvement_Treant(uint32 diff)
			{
				if (!UpdateVictim())
					return;

				Mana = me->GetPower(POWER_MANA);
				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				DoMeleeAttackIfReady();															// Combat en mélée

				// Si la cible >= 8m (pour éviter bug de rester figé) ---------------------------------------------------------------------------------------------
				if (Dist >= 8 && Cooldown_Anti_Bug_Figer <= diff && !me->HasAura(122) && !me->HasAura(3600) && !me->HasAura(6474))
				{
					float x, y, z;
					x = victim->GetPositionX();
					y = victim->GetPositionY();
					z = victim->GetMap()->GetHeight(me->GetPhaseMask(), x, y, MAX_HEIGHT, true);
					me->GetClosePoint(x, y, z, me->GetObjectSize() / 3, 3);
					me->GetMotionMaster()->MovePoint(0xFFFFFE, x, y, z);
					me->GetMotionMaster()->MoveChase(victim, 1, frand(0, 6.2836f));			// Pour suivre la cible avec un angle
					Cooldown_Anti_Bug_Figer = 2000;
				}
				else Cooldown_Anti_Bug_Figer -= diff;

				// Si la cible est entre 10 & 30m : Empaler et tirer --------------------------------------------------------------------------------------------------------
				if (Cooldown_Charge <= diff)
				{
					Random = urand(1, 5);
					if ((Dist >= 8) && (Dist <= 20))
					{
						if (Random == 1)
						{
							DoCastVictim(Empaler_et_tirer);											// Empaler et tirer - 1 chance sur 2
						}
						else
							DoCast(me, Barbeles_depines);											// Barbelés d'épines 113967

						Cooldown_Charge = 5000;
					}
				}
				else Cooldown_Charge -= diff;

			}
			void Tourne_Au_Tour_En_Combat()
			{
				if (!UpdateVictim())
					return;

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				float x = 0.0f, y = 0.0f, z = 0.0f;
				uint32 mapid = 0;

				x = (victim->GetPositionX() + urand(0, 4) - 2);
				y = (victim->GetPositionY() + urand(0, 4) - 2);
				z = victim->GetMap()->GetHeight(me->GetPhaseMask(), x, y, MAX_HEIGHT, true);
				mapid = victim->GetMapId();
				me->GetClosePoint(x, y, z, me->GetObjectSize() / 3, 3);
				me->GetMotionMaster()->MovePoint(mapid, x, y, z);
				DoMeleeAttackIfReady();
			}
			void Avance_3m_En_Combat()
			{
				if (!UpdateVictim())
					return;

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				float x = 0.0f, y = 0.0f, z = 0.0f;
				uint32 mapid = 0;

				x = (victim->GetPositionX());
				y = (victim->GetPositionY());
				z = victim->GetMap()->GetHeight(me->GetPhaseMask(), x, y, MAX_HEIGHT, true);
				mapid = victim->GetMapId();
				me->GetClosePoint(x, y, z, me->GetObjectSize() / 3, 3);
				me->GetMotionMaster()->MovePoint(mapid, x, y, z);
			}
			void Heal_En_Combat_Caster(uint32 diff)
			{
				if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_MOVE))
					return;

				// Heal en combat ------------------------------------------------------------------------------------------------------------------------------
				if (Cooldown_Spell_Heal <= diff)
				{
																								// heal sur lui meme
					if ((me->GetHealth() < (me->GetMaxHealth()*0.50)))								// Si PV < 50%
					{
						DoCast(me, Spell_Heal_Caster);
						Cooldown_Spell_Heal = 4500;
					}
					
					// heal sur Friend 
					Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, DistanceDeCast, true);		// pour heal friend
					if (target = DoSelectLowestHpFriendly(DistanceDeCast))							// Distance de l'allié = 30m
					{
						if (me->IsFriendlyTo(target) && (me != target))
						{
							if (target->GetHealth() < (target->GetMaxHealth()*0.40))				// Si PV < 40%
							{
								DoCast(target, Spell_Heal_Caster);
								Cooldown_Spell_Heal = 5500;
							}
						}
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
			void ContreAttaque(uint32 diff)
			{
				if (!UpdateVictim())
					return;

				// Contre attaque sur la cible (2 chance sur 3) -------------------------------------------------------------------------------------------------
				if (Cooldown_Spell_ContreAttaque <= diff && Spell_ContreAttaque != 0)
				{
					if ((me->GetHealth() < (me->GetMaxHealth()*0.40)))									// Si PV =< 40%
					{
						Random = urand(1, 3);
						if (Random != 1)
						{
							DoCastVictim(Spell_ContreAttaque);
						}
					}
					Cooldown_Spell_ContreAttaque = Cooldown_Spell_ContreAttaque_defaut;
				}
				else Cooldown_Spell_ContreAttaque -= diff;
			}
			bool AuraLenteur()
			{
				if (me->HasAura(116)		// Eclair_de_givre 116 
					|| me->HasAura(71318)	// Eclair_de_givre 71318
					|| me->HasAura(122)		// Nova de givre
					|| me->HasAura(31589)	// Lenteur 31589
					|| me->HasAura(6136) 	// Armure_de_givre 6136
					|| me->HasAura(8056) 	// Horion_de_givre 8056
					|| me->HasAura(12548) 	// Horion_de_givre 12548
					|| me->HasAura(9080) 	// Brise_genou 9080
					|| me->HasAura(1715) 	// Brise_genou 1715
					|| me->HasAura(69917) 	// Fievre_de_givre 69917
					|| me->HasAura(67719) 	// Fievre_de_givre 67719
					|| me->HasAura(45477) 	// Toucher_de_glace 45477
					|| me->HasAura(300051) 	// Javelot_de_givre 300051
					|| me->HasAura(300237) 	// Javelot_de_givre 300237
					|| me->HasAura(60192) 	// Gel_de_zone 60192
					|| me->HasAura(116095) 	// Handicap 116095
					|| me->HasAura(300197) 	// Toucher_de_glace 300197
					|| me->HasAura(20170)	// Sceau de justice 20170
					|| me->HasAura(3600)	// Totem de lien terrestre
					|| me->HasAura(6474))	// Totem de lien terrestre passif
					return true;
				else return false;
			}

			//void JustDied(Unit * /*victim*/) override {}
			//void KilledUnit(Unit* /*victim*/) override {Talk(SAY_SLAY);}

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















