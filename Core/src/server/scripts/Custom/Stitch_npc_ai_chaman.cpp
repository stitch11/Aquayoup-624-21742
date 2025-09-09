////#########################################################################################################################################################################################################################################
// Copyright (C) Juin 2020 Stitch pour Aquayoup
// AI generique npc par classe : CHAMAN Ver 2022-07-31
// Il est possible d'influencer le temp entre 2 cast avec `BaseAttackTime` & `RangeAttackTime` 
// Necessite dans Creature_Template :
// Minimun  : UPDATE `creature_template` SET `ScriptName` = 'Stitch_npc_ai_chaman',`AIName` = '' WHERE (entry = 15100002);
// Optionel : UPDATE `creature_template` SET `HealthModifier` = 2, `ManaModifier` = 3, `ArmorModifier` = 1, `DamageModifier` = 2,`BaseAttackTime` = 2000, `RangeAttackTime` = 2000 WHERE(entry = 15100002);
// Optionel : Utilisez pickpocketloot de creature_template pour passer certains parametres (Solution choisit afin de rester compatible avec tout les cores). Si pickpocketloot = 1 (branche1 forc�), pickpocketloot = 2 (branche2 forc�), etc
//###########################################################################################################################################################################################################################################
// # npc de Test Stitch_npc_ai_chaman  .npc 15100002
// REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `HealthModifier`, `HealthModifierExtra`, `ManaModifier`, `ManaModifierExtra`, `ArmorModifier`, `DamageModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES
// (15100002, 0, 0, 0, 0, 0, 59382, 0, 0, 0, 'Stitch_npc_ai_Chaman', '', '', '', 0, 90, 90, 0, 0, 2102, 0, 1.01, 1.01, 0.7, 0, 0, 2000, 2000, 1, 1, 2, 0, 2048, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 2, 1, 3, 1, 2, 2, 1, 0, 144, 1, 0, 0, 'Stitch_npc_ai_chaman', 20173);
// REPLACE INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`, `ItemID2`, `ItemID3`, `VerifiedBuild`) VALUES
// (15100002, 1, 854, 0, 0, 0),				// baton			- Restauration
// (15100002, 2, 2209, 63165, 0, 0),		// dague+bouclier	- Elementaire
// (15100002, 3, 2209, 2209, 0, 0);		    // dague * 2		- Amelioration
//###########################################################################################################################################################################################################################################

#include "CreatureTextMgr.h"

//################################################################################################
//StitchAI AI Chaman
//################################################################################################

class Stitch_npc_ai_chaman : public CreatureScript
{
public: Stitch_npc_ai_chaman() : CreatureScript("Stitch_npc_ai_chaman") { }

		struct Stitch_npc_ai_chamanAI : public ScriptedAI
		{
			Stitch_npc_ai_chamanAI(Creature* creature) : ScriptedAI(creature) { }

			uint32 BrancheSpe = 1;													// Choix de la Sp�cialisation : Restauration=1, Elementaire=2, Amelioration=3
			uint32 NbrDeSpe = 3;													// Nombre de Sp�cialisations
			uint32 ForceBranche;
			uint32 Random;
			uint32 DistanceDeCast = 40;												// Distance max a laquelle un npc attaquera , au dela il quite le combat
			uint32 ResteADistance = 10;												// Distance max a laquelle un npc s'approchera
			uint32 Dist;															// Distance entre le npc et sa cible
			uint32 Mana;
			uint32 MaxMana = me->GetMaxPower(POWER_MANA);
			Unit* victim = me->GetVictim();	
			uint32 MessageAlagro = 0;
			uint32 Spell_ContreAttaque = 0;

			
			// Definitions des variables Cooldown et le 1er lancement
			uint32 Cooldown_Spell1 = 500;
			uint32 Cooldown_Spell2 = 2500;
			uint32 Cooldown_Spell3 = 3500;
			uint32 Cooldown_Spell_Heal = 3000;
			uint32 Cooldown_RegenMana = 3000;
			uint32 Cooldown_ResteADistance = 4000;									// Test si en contact pour  Restauration, Elementaire et s'eloigner
			uint32 Cooldown_ResteAuContact;
			uint32 Cooldown_Anti_Bug_Figer = 2000;
			uint32 Cooldown_Charge = 8000;
			uint32 Cooldown_Npc_Emotes = urand(5000, 8000);
			uint32 Cooldown_Spell_ContreAttaque = 4000;
			uint32 Cooldown_Spell_ContreAttaque_defaut = 8000;

			// Spells Divers
			uint32 Buf_branche1 = 974;												// Bouclier de foudre 324 - Bouclier de terre 974 - Bouclier d�eau 52127
			uint32 Buf_branche2 = 52127;											// Bouclier de foudre 324 - Bouclier de terre 974 - Bouclier d�eau 52127
			uint32 Buf_branche3 = 324;												// Bouclier de foudre 324 - Bouclier de terre 974 - Bouclier d�eau 52127
			uint32 Spell_Heal_Caster = 300263;  									// Afflux de soins 300263/8004
			uint32 Spell_Heal_Heal = 300264;  										// Vague de soins 300264/77472
			uint32 Rappel_totemique = 36936;										// Rappel totemique : retire les totems invoqu�s
			uint32 Totem_guerisseur = 5394;
			uint32 Totem_incendiaire = 3599;
			uint32 Totem_peau_de_pierre = 78222;
			uint32 Totem_de_magma = 8190;											// Totem de magma
			uint32 Horion_de_terre = 13728;											// Horion de terre 
			uint32 Horion_de_givre = 12548;											// Horion de givre

			// Spells Restauration
			uint32 Spell_branche1_agro = 0;
			uint32 Spell_branche1_1 = 0;
			uint32 Spell_branche1_2 = 0;
			uint32 Spell_branche1_3 = 0;
			uint32 branche1_agro[4] = { 15039, 12548, 3599, 73393 };				// Horion de flammes 15039 - Horion de givre 12548 - Totem incendiaire 3599 - Totem de flot de mana 73393
			uint32 branche1_1[2] = { 9532, 9532 };									// Eclair 9532
			uint32 branche1_2[2] = { 12548, 13728 };								// Horion de givre 12548 - Horion de terre 13728 (stun 2s)
			uint32 branche1_3[2] = { 15039, 57994 };								// Horion de flammes 15039 30s - Cisaille de vent 57994
			
			// Definitions des Spells Elementaire
			uint32 Spell_branche2_agro = 0;
			uint32 Spell_branche2_1 = 0;
			uint32 Spell_branche2_2 = 0;
			uint32 Spell_branche2_3 = 0;
			uint32 branche2_agro[5] = { 15039, 12548, 3599, 2484, 78222 };			// Horion de flammes 15039 - Horion de givre 12548 - Totem incendiaire 3599 - Totem de lien terrestre 2484 - Totem de peau de pierre 78222
			uint32 branche2_1[4] = { 9532, 9532, 9532, 28167 };						// Eclair 9532 - Cha�ne d'�clairs 28167 
			uint32 branche2_2[3] = { 178091, 178091 ,12548 };						// Explosion de lave 178091 - Horion de givre 12548 8s
			uint32 branche2_3[4] = { 15039, 15039, 13728, 57994 };					// Horion de flammes 15039 30s - Horion de terre 13728 (stun 2s) - Cisaille de vent 57994

			// Spells Amelioration
			uint32 Spell_branche3_agro = 0;
			uint32 Spell_branche3_1 = 0;
			uint32 Spell_branche3_2 = 0;
			uint32 Spell_branche3_3 = 0;
			uint32 branche3_agro[4] = { 370, 370, 2484, 3599 };						// Purge 370 (retire 1 buf), Totem de lien terrestre 2484, Totem incendiaire 3599
			uint32 branche3_1[2] = { 79926, 79926 };								// Frappe primordiale 79926
			uint32 branche3_2[3] = { 172779, 147093, 12548 };						// Horion de givre 12548 8s - Frappe-temp�te 172779 - Fouet de lave 147093
			uint32 branche3_3[2] = { 15039, 13728 };								// Horion de flammes 15039 - Cisaille de vent 57994 -  Horion de givre 12548 8s - Horion de terre 13728 (stun 2s)

			// Emotes
			uint32 Npc_Emotes[22] = { 1,3,7,11,15,16,19,21,22,23,24,53,66,71,70,153,254,274,381,401,462,482 };

			uint32 Start_Agro = 0;

			void InitializeAI() override
			{
				// Buf au lancement du serveur (Pet, armure de givre, etc)
				uint32 Tmp = me->m_spells[3];
				if (Tmp != 0)
				{
					me->CastSpell(me, Tmp, true);
				}
				me->SetSheath(SHEATH_STATE_UNARMED);								//Arme rang�e
			}

			void Init_AI()
			{
				// ################################################################################################################################################
				// Forcer le choix de la Sp�cialisation par creature_template->pickpocketloot
				// ################################################################################################################################################
				// Forcer le choix de la Sp�cialisation par creature_template->pickpocketloot
				ForceBranche = me->GetCreatureTemplate()->pickpocketLootId;							// creature_template->pickpocketloot
				if (ForceBranche == 1) { BrancheSpe = 1; }											// branche1 forc�
				else if (ForceBranche == 2) { BrancheSpe = 2; }										// branche2 forc� 
				else if (ForceBranche == 3) { BrancheSpe = 3; }										// branche3 forc�
				else
				{
					// Sinon Choix de la Sp�cialisation Al�atoire
					BrancheSpe = urand(1, NbrDeSpe + 2);
				}

				if ((BrancheSpe > NbrDeSpe) || (BrancheSpe == 0)) { BrancheSpe = 2; }

				// ################################################################################################################################################
				// Tirages al�atoires des spells
				// ################################################################################################################################################
				// Spell a lancer a l'agro ------------------------------------------------------------------------------------------------------------------------

				// Message a l'agro forc� par spell(8)
				if (me->m_spells[7] == 1) { MessageAlagro = 1; }

				// Spell contre attaque si PV bas
				if (me->m_spells[6] != 0) { Spell_ContreAttaque = me->m_spells[6]; }

				switch (BrancheSpe)
				{
				case 1: // Si Restauration --------------------------------------------------------------------------------------------------------------------------
					me->CastSpell(me, Buf_branche1, true);
					me->LoadEquipment(1, true);													// creature_equip_template 1
					Bonus_Armure(150);															// Bonus d'armure +50%

					// Tirages al�atoires des spells Restauration 
					Spell_branche1_agro = branche1_agro[urand(0, 3)];
					Spell_branche1_1 = branche1_1[urand(0, 1)];
					Spell_branche1_2 = branche1_2[urand(0, 1)];
					Spell_branche1_3 = branche1_3[urand(0, 1)];

					// Totem
					me->CastSpell(me, Totem_guerisseur, true); 									// Totem gu�risseur 5394 (12s)
					
					// Reste a distance variable suivant ci le mob est a l'ext�rieur ou a l'interrieur
					if (me->GetMap()->IsOutdoors(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()))
					{
						ResteADistance = 15;
					}
					else
					{
						ResteADistance = 7;
					}

					break;

				case 2: // Si Elementaire -------------------------------------------------------------------------------------------------------------------------
					me->CastSpell(me, Buf_branche2, true);										// Buf2 sur lui meme
					me->LoadEquipment(2, true);													// creature_equip_template 2
					Bonus_Armure(150);															// Bonus d'armure +50%

					// Tirages al�atoires des spells Elementaire 
					Spell_branche2_agro = branche2_agro[urand(0, 4)];
					Spell_branche2_1 = branche2_1[urand(0, 3)];
					Spell_branche2_2 = branche2_2[urand(0, 2)];
					Spell_branche2_3 = branche2_3[urand(0, 3)];

					// Totem
					me->CastSpell(me, Totem_incendiaire, true);									// Totem incendiaire 3599

					// Reste a distance variable suivant ci le mob est a l'ext�rieur ou a l'Int�rieur
					if (me->GetMap()->IsOutdoors(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()))
					{
						ResteADistance = urand(14, 16);
					}
					else
					{
						ResteADistance = 7;
					}
					break;

				case 3: // Si Amelioration -------------------------------------------------------------------------------------------------------------------------
					me->CastSpell(me, Buf_branche3, true);										// Buf3 sur lui meme
					me->LoadEquipment(3, true);													// creature_equip_template 2
					Bonus_Armure(200);															// Bonus d'armure +100%

					// Tirages al�atoires des spells Elementaire 
					Spell_branche3_agro = branche2_agro[urand(0, 3)];
					Spell_branche3_1 = branche3_1[urand(0, 1)];
					Spell_branche3_2 = branche3_2[urand(0, 2)];
					Spell_branche3_3 = branche3_3[urand(0, 1)];

					ResteADistance = 5;

					// Totem
					me->CastSpell(me, Totem_incendiaire, true);									// Totem incendiaire 3599
					break;
				}
				// ################################################################################################################################################
				// Divers
				// ################################################################################################################################################
				me->SetSheath(SHEATH_STATE_MELEE);																		// S'equipe de l'arme au contact
				me->SetReactState(REACT_AGGRESSIVE);
				// ################################################################################################################################################
			}
			void JustRespawned() override
			{
				me->GetMotionMaster()->MoveTargetedHome();								// Retour home pour rafraichir client
				//me->SetSpeedRate(MOVE_RUN, 1.01f);
				me->SetReactState(REACT_AGGRESSIVE);

				Init_AI();
				me->SetSheath(SHEATH_STATE_UNARMED);								//Arme rang�e
			}

			void EnterCombat(Unit* /*who*/) override
			{
				ResteADistance = 10 + urand(0, 5);
				Init_AI();
				me->SetSheath(SHEATH_STATE_MELEE);									//Arme sortie

				if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == WAYPOINT_MOTION_TYPE)
				{
					me->StopMoving();
					me->GetMotionMaster()->MoveIdle();
				}

				//Retire certaines Aura, emotes & Bytes a l'agro
				me->RemoveAura(80264);	// Retire Marche a pas de loup
				me->RemoveAura(152891);	// Retire Avance a pas de loup invisible
				me->RemoveAura(104015);	// Retire afaiss� / Stun
				me->RemoveAura(153964);	// Retire agenouill�, avec evade
				me->RemoveAura(42648);	// Retire Dort allong� + zzz
				me->RemoveAura(18795);	// Retire Dort allong� + zzz 
				me->RemoveAura(43905);	// Retire Ivre
				me->RemoveAura(101090);	// Retire Danse
				me->HandleEmoteCommand(0);
				me->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0); // Autre fa�on de retirer des �motes pour les cas particuliers
				me->SetByteValue(UNIT_FIELD_BYTES_1, 0, 0);
				me->SetByteValue(UNIT_FIELD_BYTES_2, 0, 0);

			}
			void EnterEvadeMode(EvadeReason /*why*/) override
			{
				Start_Agro = 0;
				RetireBugDeCombat();
				me->AddUnitState(UNIT_STATE_EVADE);
				//me->SetSpeedRate(MOVE_RUN, 1.5f);										// Vitesse de d�placement
				me->GetMotionMaster()->MoveTargetedHome();								// Retour home
				me->RemoveAllControlled();												// renvois pet

				me->RemoveAura(Buf_branche1);
				me->RemoveAura(Buf_branche2);
				me->RemoveAura(Buf_branche3);
				Bonus_Armure(100);														// Retire bonus d'armure
			}
			void JustReachedHome() override
			{
				me->RemoveAura(Buf_branche1);
				me->RemoveAura(Buf_branche2);
				me->RemoveAura(Buf_branche3);
				Bonus_Armure(100);														// Retire bonus d'armure

				me->SetSheath(SHEATH_STATE_UNARMED);								//Arme rang�e
				me->SetReactState(REACT_AGGRESSIVE);
				//me->SetSpeedRate(MOVE_RUN, 1.01f);									// Vitesse par defaut d�finit a 1.01f puisque le patch modification par type,famille test si 1.0f
			}
			void UpdateAI(uint32 diff) override
			{
				if (me->HasUnitState(UNIT_STATE_CONFUSED) || me->HasUnitState(UNIT_STATE_STUNNED) || me->HasUnitState(UNIT_STATE_DISTRACTED) || me->HasUnitState(UNIT_STATE_CANNOT_TURN) || me->HasUnitState(UNIT_STATE_CONTROLLED) || me->HasUnitState(UNIT_STATE_POSSESSED) || me->HasUnitState(UNIT_STATE_CONFUSED_MOVE))
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
						if ((sCreatureTextMgr->TextExist(me->GetEntry(), 0)) && (!sCreatureTextMgr->TextExist(me->GetEntry(), 1) && Random == 1) || MessageAlagro == 1)
						{
							Talk(0);
						}


						// ########################################################################################################################################
						// Spell a lancer a l'agro 
						// ########################################################################################################################################
						switch (BrancheSpe)
						{
						case 1: // Si Restauration ----------------------------------------------------------------------------------------------------------------
							Random = urand(1, 2);
							if (Random == 1) { me->CastSpell(victim, Spell_branche1_agro, true); }		// 1/2 Chance de lancer le sort d'agro
							break;

						case 2: // Si Elementaire -----------------------------------------------------------------------------------------------------------------
							Random = urand(1, 2);
							if (Random == 1) { me->CastSpell(victim, Spell_branche2_agro, true); }		// 1/2 Chance de lancer le sort d'agro
							break;

						case 3: // Si Amelioration ----------------------------------------------------------------------------------------------------------------
							Random = urand(1, 2);
							if (Random == 1) { me->CastSpell(victim, Spell_branche3_agro, true); }		// 1/2 Chance de lancer le sort d'agro
							break;

							// ####################################################################################################################################
						}
					}


					// ############################################################################################################################################
					// Combat suivant la Sp�cialisation
					switch (BrancheSpe)
					{
					case 1: // Sp�cialisation Restauration ##########################################################################################################
							// Regen mana en combat ---------------------------------------------------------------------------------------------------------------
						if (Cooldown_RegenMana <= diff)
						{
							me->SetPower(POWER_MANA, Mana + (MaxMana / 5));
							if (Mana > MaxMana) { me->SetPower(POWER_MANA, MaxMana); }
							Cooldown_RegenMana = urand(1000, 1500);
						}
						else Cooldown_RegenMana -= diff;

						// Combat ---------------------------------------------------------------------------------------------------------------------------------
						// Spell1 sur la cible chaque (Sort R�guli�)
						if (Cooldown_Spell1 <= diff)
						{
							if (!me->isMoving() /*!me->HasUnitState(UNIT_STATE_MOVE)*/)
							{
								me->StopMoving();
								DoCastVictim(Spell_branche1_1);
								Cooldown_Spell1 = 4000;
							}
						}
						else Cooldown_Spell1 -= diff;

						// Spell2 sur la cible chaque (Sort secondaire plus lent)
						if (Cooldown_Spell2 <= diff)
						{
							me->StopMoving();
							DoCastVictim(Spell_branche1_2);
							Cooldown_Spell2 = 8000;
						}
						else Cooldown_Spell2 -= diff;

						// Spell3 sur la cible  (Sort secondaire tres lent , g�n�ralement utilis� comme Dot)
						if (Cooldown_Spell3 <= diff)
						{
							DoCastVictim(Spell_branche1_3);
							Cooldown_Spell3 = urand(10000, 12000);
						}
						else Cooldown_Spell3 -= diff;

						Heal_En_Combat_Heal(diff);
						Mouvement_Caster(diff);
						break;

					case 2: // Sp�cialisation Elementaire #############################################################################################################
							// Regen mana en combat -------------------------------------------------------------------------------------------------------------------
						if (Cooldown_RegenMana <= diff)
						{
							me->SetPower(POWER_MANA, Mana + (MaxMana / 5));
							if (Mana > MaxMana) { me->SetPower(POWER_MANA, MaxMana); }
							Cooldown_RegenMana = urand(1500, 2000);
						}
						else Cooldown_RegenMana -= diff;

						// Combat -------------------------------------------------------------------------------------------------------------------------------------
						// Spell1 sur la cible chaque (Sort R�guli�)
						if (Cooldown_Spell1 <= diff)
						{
							if (!me->HasUnitState(UNIT_STATE_MOVE))
							{
								me->StopMoving();
								DoCastVictim(Spell_branche2_1);
								Cooldown_Spell1 = 4000;
							}
						}
						else Cooldown_Spell1 -= diff;

						// Spell2 sur la cible chaque (Sort secondaire plus lent)
						if (Cooldown_Spell2 <= diff)
						{
							me->StopMoving();
							DoCastVictim(Spell_branche2_2);
							Cooldown_Spell2 = 8000;
						}
						else Cooldown_Spell2 -= diff;

						// Spell3 sur la cible  (Sort secondaire tres lent , g�n�ralement utilis� comme Dot)
						if (Cooldown_Spell3 <= diff)
						{
							DoCastVictim(Spell_branche2_3);
							Cooldown_Spell3 = urand(10000, 12000);
						}
						else Cooldown_Spell3 -= diff;

						Heal_En_Combat_Caster(diff);
						Mouvement_Caster(diff);
						break;

					case 3: // Sp�cialisation Amelioration #############################################################################################################
							// Regen mana en combat ------------------------------------------------------------------------------------------------------------------------
						if (Cooldown_RegenMana <= diff)
						{
							me->SetPower(POWER_MANA, Mana + (MaxMana / 10));
							if (Mana > MaxMana) { me->SetPower(POWER_MANA, MaxMana); }
							Cooldown_RegenMana = 1000;
						}
						else Cooldown_RegenMana -= diff;

						// Combat --------------------------------------------------------------------------------------------------------------------------------------
						if (Dist < 8)
						{
							// Spell1 sur la cible chaque (Sort R�guli�)
							if (Cooldown_Spell1 <= diff)
							{
								if (!me->HasUnitState(UNIT_STATE_MOVE))
								{
									DoCastVictim(Spell_branche3_1);
									Cooldown_Spell1 = 3000;
								}
							}
							else Cooldown_Spell1 -= diff;

							// Spell2 sur la cible chaque (Sort secondaire plus lent)
							if (Cooldown_Spell2 <= diff)
							{
								Bonus_Degat_Arme_Done(-50);										// Reduction de degat pour ce sort trop puissant
								DoCastVictim(Spell_branche3_2);
								Bonus_Degat_Arme_Done(50);										// On le re initialise 
								Cooldown_Spell2 = urand(5000, 7000);
							}
							else Cooldown_Spell2 -= diff;

							// Spell3 sur la cible  (Sort secondaire tres lent , g�n�ralement utilis� comme Dot)
							if (Cooldown_Spell3 <= diff)
							{
								DoCastVictim(Spell_branche3_3);
								Cooldown_Spell3 = urand(10000, 12000);
							}
							else Cooldown_Spell3 -= diff;

						}

						Heal_En_Combat_Melee(diff);
						Mouvement_Contact(diff);
						break;

					}
					// ################################################################################################################################################

				}
				// ################################################################################################################################################

				ContreAttaque(diff);
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
				// Anti bug de combat
				if (me->IsAlive() && (me->GetDistance(me->GetHomePosition()) >  50))
				{
					RetireBugDeCombat();

					if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE) && me->IsInCombat())
					{
						me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
					}

					EnterEvadeMode(EVADE_REASON_SEQUENCE_BREAK);

					if (me->IsInCombat() && me->getAttackers().empty())
					{
						EnterEvadeMode(EVADE_REASON_NO_HOSTILES);
					}
				}





				if (!UpdateVictim())
					return;

				Dist = me->GetDistance(me->GetVictim());
				if ((Dist > DistanceDeCast) || (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > 40))
				{
					RetireBugDeCombat();
					me->AddUnitState(UNIT_STATE_EVADE);
					EnterEvadeMode(EVADE_REASON_SEQUENCE_BREAK);						// Quite le combat si la cible > 30m (Caster & M�l�e) ou > 40m de home
				}
			}
			void Mouvement_Caster(uint32 diff)
			{
				if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING) || AuraFig�())
					return;

				Mana = me->GetPower(POWER_MANA);
				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				if (Cooldown_ResteADistance <= diff)
				{
					// Mouvement al�atoire si cible < 6m & Mana > 5% --------------------------------------------------------------------------------------------------

					if ((Dist <6) && (Mana > MaxMana / 20))
					{
						me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);						// UNROOT
						me->SetWalk(false);																// Ne pas marcher

						if (!AuraLenteur() && !Interieur())
						{
							me->SetSpeedRate(MOVE_RUN, 1.1f); // Uniquement si non ralenti par un spell 
						}

						float x = 0.0f, y = 0.0f, z = 0.0f;
						uint32 mapid = 0;

						x = (victim->GetPositionX() + urand(0, ResteADistance * 2) - ResteADistance);
						y = (victim->GetPositionY() + urand(0, ResteADistance * 2) - ResteADistance);
						if (me->GetMap()->IsOutdoors(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()) && !me->IsUnderWater())
						{
							z = victim->GetMap()->GetHeight(me->GetPhaseMask(), x, y, MAX_HEIGHT, true);
						}
						else
						{
							z = victim->GetPositionZ();	// Sinon bug en interieur
						}
						mapid = victim->GetMapId();
						me->GetMotionMaster()->MovePoint(mapid, x, y, z);
						Cooldown_ResteADistance = 3000;
					}
				}
				else Cooldown_ResteADistance -= diff;

				// Speed normal si distance > 10m ------------------------------------------------------------------------------------------------------------------
				if (AuraLenteur() == false && Dist> 10 && me->GetSpeedRate(MOVE_RUN) == 1.2f)
				{
					me->SetSpeedRate(MOVE_RUN, 1.0f); // Uniquement si non ralenti par un spell joueur
				}

				// Mouvement OFF si Mana > 5% & distance >= 6 & <= 10m ---------------------------------------------------------------------------------------------
				if ((Mana > MaxMana / 20) && (Dist >= ResteADistance - 4) && (Dist <= ResteADistance))
				{
					if (me->isMoving())																	// Sinon bug d'animation
					{
						AttackStart(victim);
						AttackStartCaster(victim, ResteADistance);										// Distance de combat
						void DoRangedAttackIfReady();													// Combat a distance
						me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);							// ROOT
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
					me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);							// UNROOT
				}

			}
			void Mouvement_Contact(uint32 diff)
			{
				if (!UpdateVictim() || AuraFig�())
					return;

				Mana = me->GetPower(POWER_MANA);
				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				//DoMeleeAttackIfReady();														// Combat en m�l�e

				// ------ ALLER A LA CIBLE -------------------------------------------------------------------------------------------------------------------------
				if (Cooldown_Anti_Bug_Figer <= diff)
				{
					if (Dist >= ResteADistance && !AuraFig�())
					{
						float x = 0.0f, y = 0.0f, z = 0.0f;
						uint32 mapid = 0;

						x = (victim->GetPositionX() + urand(0, 4) - 2);
						y = (victim->GetPositionY() + urand(0, 4) - 2);
						if (me->GetMap()->IsOutdoors(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()) && !me->IsUnderWater())
						{
							z = victim->GetMap()->GetHeight(me->GetPhaseMask(), x, y, MAX_HEIGHT, true);
						}
						else
						{
							z = victim->GetPositionZ();	// Sinon bug en interieur
						}
						mapid = victim->GetMapId();
						me->GetMotionMaster()->MovePoint(mapid, x, y, z);
					}
					Cooldown_Anti_Bug_Figer = 1000;
				}
				else Cooldown_Anti_Bug_Figer -= diff;

				// Si la cible est entre 8 & 30m : Horion de terre ou Horion de givre ------------------------------------------------------------------------------

				if (Cooldown_Charge <= diff)
				{
					Random = urand(1, 8);
					if ((Dist >= 8) && (Dist <= DistanceDeCast))
					{
						if (Random == 1)
						{
							DoCastVictim(Horion_de_terre);									// Horion de terre 13728 (stun 2s) - 1 chance sur 8
						}
						else if (Random == 2)
						{
							DoCastVictim(Horion_de_givre);									// Horion de givre 12548 8s - 1 chance sur 8
						}
						Cooldown_Charge = urand(15000, 20000);
						Cooldown_ResteADistance = urand(2000, 4000);						// Sinon bugue Totem de magma 8190
					}
				}
				else Cooldown_Charge -= diff;

				// Si la cible < 8m : Totem de magma ou Avance ou Tourne au tour -----------------------------------------------------------------------------------

				if (Dist < 8 && (Cooldown_ResteADistance <= diff))
				{
					Random = urand(1, 5);
					if ((Random == 1 || Random == 2) && !AuraFig�())
					{
						Tourne_Au_Tour_En_Combat();											// 2 chances sur 5 tourne au tour de sa victime
					}
					else if (Random == 3 || Random == 4)
					{
						Avance_3m_En_Combat();												// 2 chances sur 5 avance
					}
					else
					{
						DoCast(me, Totem_de_magma);											// 1 chance sur 5 : Totem de magma 8190
					}
					Cooldown_ResteADistance = urand(4000, 6000);
				}
				else Cooldown_ResteADistance -= diff;

			}
			void Tourne_Au_Tour_En_Combat()
			{
				if (!UpdateVictim())
					return;

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(me->GetVictim());

				float x, y, z;
				x = (victim->GetPositionX() + urand(0, 6) - 3);
				y = (victim->GetPositionY() + urand(0, 6) - 3);
				if (me->GetMap()->IsOutdoors(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()) && !me->IsUnderWater())
				{
					z = victim->GetMap()->GetHeight(me->GetPhaseMask(), x, y, MAX_HEIGHT, true);
				}
				else
				{
					z = victim->GetPositionZ();	// Sinon bug en interieur
				}
				me->GetMotionMaster()->MovePoint(0, x, y, z);
			}
			void Avance_3m_En_Combat()
			{
				if (!UpdateVictim())
					return;

				float x, y, z;
				me->GetClosePoint(x, y, z, me->GetObjectSize() / 3, 3);
				me->GetMotionMaster()->MovePoint(0, x, y, z);
			}
			void Heal_En_Combat_Caster(uint32 diff)
			{
				if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_MOVE))
					return;

			// Heal en combat ------------------------------------------------------------------------------------------------------------------------------
				if (Cooldown_Spell_Heal <= diff)
				{
				Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, DistanceDeCast, true);		// pour heal friend

				// heal sur lui meme
				if ((me->GetHealth() < (me->GetMaxHealth()*0.70)))								// Si PV < 70%
				{
					DoCast(me, Spell_Heal_Caster);
					Cooldown_Spell_Heal = 5000;
				}

				// heal sur Friend 
				if (target = DoSelectLowestHpFriendly(DistanceDeCast))							// Distance de l'alli� = 30m
				{
					if (me->IsFriendlyTo(target) && (me != target))
					{
						if (target->GetHealth() < (target->GetMaxHealth()*0.40))				// Si PV < 40%
						{
							DoCast(target, Spell_Heal_Caster);
							Cooldown_Spell_Heal = 6000;
						}
					}
				}
				}
				else Cooldown_Spell_Heal -= diff;
			}
			void Heal_En_Combat_Heal(uint32 diff)
			{
				// Heal en combat ------------------------------------------------------------------------------------------------------------------------------
				if (Cooldown_Spell_Heal <= diff)
				{
					Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, DistanceDeCast, true);		// pour heal friend

																									// heal sur lui meme
					if ((me->GetHealth() < (me->GetMaxHealth()*0.75)))								// Si PV < 75%
					{
						DoCast(me, Spell_Heal_Heal);
						Cooldown_Spell_Heal = 4500;
					}

					// heal sur Friend 
					if (target = DoSelectLowestHpFriendly(DistanceDeCast))							// Distance de l'alli� = 30m
					{
						if (me->IsFriendlyTo(target) && (me != target))
						{
							if (target->GetHealth() < (target->GetMaxHealth()*0.60))				// Si PV < 60%
							{
								DoCast(target, Spell_Heal_Heal);
								Cooldown_Spell_Heal = 5500;
							}
						}
					}
				}
				else Cooldown_Spell_Heal -= diff;
			}
			void Heal_En_Combat_Melee(uint32 diff)
			{
				// Heal en combat ------------------------------------------------------------------------------------------------------------------------------
				if (Cooldown_Spell_Heal <= diff)
				{

					// heal sur lui meme
					if ((me->GetHealth() < (me->GetMaxHealth()*0.50)))								// Si PV < 50%
					{
						DoCast(me, Spell_Heal_Caster);
						Cooldown_Spell_Heal = 5000;
					}
				}
				else Cooldown_Spell_Heal -= diff;

			}
			
			void Bonus_Degat_Arme_Done(int val) // 
			{
				// +- Bonus en % de degat des armes inflig�es a victim
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
			void ContreAttaque(uint32 diff)
			{
				if (!UpdateVictim())
					return;
				Unit* victim = me->GetVictim();
				if (victim->HasAura(Spell_ContreAttaque) || Spell_ContreAttaque == 0)
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
				if (me->HasAura(137573)		// vitesse (+70%/4s) , annule tous les effets affectant le d�placement
					|| me->HasAura(31224)	// Cape d'ombre    
					|| me->HasAura(1856)	// Disparition
					) return false;

				if (me->HasAura(116)		// Eclair_de_givre 116 
					|| me->HasAura(71318)	// Eclair_de_givre 71318
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
					|| me->HasAura(6343)	// Coup de tonnerre
					|| me->HasAura(8147)	// Coup de tonnerre
					) return true;
				else return false;
			}
			bool AuraFig�()
			{
				if (me->HasAura(122)		// Nova de givre
					|| me->HasAura(3600)	// Totem de lien terrestre
					|| me->HasAura(6474)	// Totem de lien terrestre passif
					) return true;
				else return false;
			}
			bool Interieur()
			{
				if (me->GetMap()->IsOutdoors(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()))
					return false;
				else return true;
			}

		};



		CreatureAI* GetAI(Creature* creature) const override
		{
			return new Stitch_npc_ai_chamanAI(creature);
		}
};

//##################################################################################################################################################################
void AddSC_npc_ai_chaman()
{
	new Stitch_npc_ai_chaman();
}















