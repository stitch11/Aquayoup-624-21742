////#########################################################################################################################################################################################################################################
// Copyright (C) Juin 2020 Stitch pour Aquayoup
// AI generique npc par classe : PALADIN Ver 2022-07-31
// Il est possible d'influencer le temp entre 2 cast avec `BaseAttackTime` & `RangeAttackTime` 
// Necessite dans Creature_Template :
// Minimun  : UPDATE `creature_template` SET `ScriptName` = 'Stitch_npc_ai_paladin',`AIName` = '' WHERE (entry = 15100006);
// Optionel : UPDATE `creature_template` SET `HealthModifier` = 2, `ManaModifier` = 3, `ArmorModifier` = 1, `DamageModifier` = 2,`BaseAttackTime` = 2000, `RangeAttackTime` = 2000 WHERE(entry = 15100006);
// Optionel : Utilisez pickpocketloot de creature_template pour passer certains parametres (Solution choisit afin de rester compatible avec tout les cores). Si pickpocketloot = 1 (branche1 forc�), pickpocketloot = 2 (branche2 forc�), etc
//###########################################################################################################################################################################################################################################
// # npc de Test Stitch_npc_ai_paladin  .npc 15100006
// REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `HealthModifier`, `HealthModifierExtra`, `ManaModifier`, `ManaModifierExtra`, `ArmorModifier`, `DamageModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES
// (15100006, 0, 0, 0, 0, 0, 29491, 0, 0, 0, 'npc_ai_Paladin', '', '', '', 0, 90, 90, 0, 0, 2102, 0, 1.01, 1.01, 1, 0, 0, 2000, 2000, 1, 1, 2, 0, 2048, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 2, 1, 1.5, 1, 2, 2, 1, 0, 144, 1, 0, 0, 'Stitch_npc_ai_paladin', 20173);
// REPLACE INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`, `ItemID2`, `ItemID3`, `VerifiedBuild`) VALUES
// (15100006, 1, 37659, 0, 0, 0),
// (15100006, 2, 52636, 0, 0, 0),
// (15100006, 3, 1899, 6078, 0, 0);
//###########################################################################################################################################################################################################################################

#include "CreatureTextMgr.h"

//################################################################################################
//StitchAI AI Paladin
//################################################################################################

class Stitch_npc_ai_paladin : public CreatureScript
{
public: Stitch_npc_ai_paladin() : CreatureScript("Stitch_npc_ai_paladin") { }

		struct Stitch_npc_ai_paladinAI : public ScriptedAI
		{
			Stitch_npc_ai_paladinAI(Creature* creature) : ScriptedAI(creature) { }

			uint32 BrancheSpe = 1;													// Choix de la Sp�cialisation : Vindice=1, Sacr�=2, Protection=3
			uint32 NbrDeSpe = 3;													// Nombre de Sp�cialisations
			uint32 ForceBranche;
			uint32 Random;
			uint32 DistanceDeCast = 40;												// Distance max a laquelle un npc attaquera , au dela il quite le combat
			uint32 ResteADistance = 5;												// Distance max a laquelle un npc s'approchera
			uint32 Dist;															// Distance entre le npc et sa cible
			uint32 Mana;
			uint32 MaxMana = me->GetMaxPower(POWER_MANA);
			Unit* victim = me->GetVictim();										 
			uint32 MessageAlagro = 0;
			uint32 Spell_ContreAttaque = 0;

			// Definitions des variables Cooldown et le 1er lancement
			uint32 Cooldown_Spell1 = 500;
			uint32 Cooldown_Spell2 = 2000;
			uint32 Cooldown_Spell3 = 3500;
			uint32 Cooldown_Spell_Heal = 3000;
			uint32 Cooldown_RegenMana = 3000;
			uint32 Cooldown_ResteADistance = 4000;									// Test si en contact
			uint32 Cooldown_ResteAuContact;
			uint32 Cooldown_Anti_Bug_Figer = 2000;
			uint32 Cooldown_Npc_Emotes = urand(5000, 8000);
			uint32 Cooldown_Spell_ContreAttaque = 4000;
			uint32 Cooldown_Spell_ContreAttaque_defaut = 8000;

			// Spells Divers
			uint32 Buf_branche1 = 20164;											// Sceau de justice 20164, Sceau de clairvoyance 20165, Sceau de v�rit� 30801, Sceau de m�ditation 105361
			uint32 Buf_branche1a = 79977;											// B�n�diction de puissance 79977
			uint32 Buf_branche2 = 20165;											// Sceau de clairvoyance 20165, Sceau de protection de mana 20154,
			uint32 Buf_branche2a = 31821;											// Aura de d�votion 31821, Aura de d�votion 52442 (armure+25%)
			uint32 Buf_branche3 = 31801;											// Sceau de clairvoyance 20165, Sceau de v�rit� 31801
			uint32 Buf_branche3a = 31850;											// Ardent d�fenseur 31850 (dmg -20% 10s), B�n�diction des rois 72043, Protection divine 498, Aura de d�votion 52442 (armure+25%) 
			uint32 Spell_Heal_Caster = 300262;  									// Eclair lumineux 300262
			uint32 Spell_Heal_Heal = 177551;  										// Lumi�re sacr�e 177551

			// Spells Vindice
			uint32 Spell_branche1_agro = 0;
			uint32 Spell_branche1_1 = 0;
			uint32 Spell_branche1_2 = 0;
			uint32 Spell_branche1_3 = 0;
			uint32 branche1_agro[4] = { 853, 853, 853, 62124 };						// Marteau de la justice 853 (stun 6s), R�primandes 96231 (interrompt 4s), R�tribution(agro) 62124
			uint32 branche1_1[2] = { 57774, 57774 };								// Jugement 57774
			uint32 branche1_2[2] = { 66003, 66003 };								// Frappe du crois� 66003
			uint32 branche1_3[3] = { 79964, 150628, 79970 };						// Exorcisme 79964, Marteau du vertueux 150628 6s, Temp�te divine 79970
			
			// Spells Sacr�
			uint32 Spell_branche2_agro = 0;
			uint32 Spell_branche2_1 = 0;
			uint32 Spell_branche2_2 = 0;
			uint32 Spell_branche2_3 = 0;
			uint32 branche2_agro[4] = { 853, 96231, 96231, 96231 };					// D�noncer 2812 , Marteau de la justice 853 (stun 6s), R�primandes 96231 (interrompt 4s)
			uint32 branche2_1[2] = { 57774, 57774 };								// Jugement 57774
			uint32 branche2_2[2] = { 66003, 66003 };								// Frappe du crois� 66003
			uint32 branche2_3[2] = { 32771, 32771 };								// Horion sacr� 32771

			// Spells Protection
			uint32 Spell_branche3_agro = 0;
			uint32 Spell_branche3_1 = 0;
			uint32 Spell_branche3_2 = 0;
			uint32 Spell_branche3_3 = 0;
			uint32 branche3_agro[3] = { 853, 96231, 62124 };						// Marteau de la justice 853 (stun 6s), R�primandes 96231 (interrompt 4s), R�tribution 62124
			uint32 branche3_1[2] = { 57774, 57774 };								// Jugement 57774
			uint32 branche3_2[2] = { 53600, 53600 };								// Frappe du crois� 66003, Bouclier du vertueux 53600
			uint32 branche3_3[2] = { 162638, 26573 };								// Bouclier du vengeur 162638 (interrompt 3s), Cons�cration 26573, 

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

				// Message a l'agro forc� par spell(8)
				if (me->m_spells[7] == 1) { MessageAlagro = 1; }

				// Spell contre attaque si PV bas
				if (me->m_spells[6] != 0) { Spell_ContreAttaque = me->m_spells[6]; }

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
					BrancheSpe = urand(1, NbrDeSpe);
				}

				if ((BrancheSpe > NbrDeSpe) || (BrancheSpe == 0)) { BrancheSpe = 2; }

				// ################################################################################################################################################
				// Tirages al�atoires des spells
				// ################################################################################################################################################
				// Spell a lancer a l'agro ------------------------------------------------------------------------------------------------------------------------

				switch (BrancheSpe)
				{
				case 1: // Si Vindice -------------------------------------------------------------------------------------------------------------------------------
					me->CastSpell(me, Buf_branche1, true);
					me->CastSpell(me, Buf_branche1a, true);
					me->LoadEquipment(1, true);													// creature_equip_template 1
					Bonus_Armure(150);															// Bonus d'armure +50%

					Spell_branche1_agro = branche1_agro[urand(0, 3)];
					Spell_branche1_1 = branche1_1[urand(0, 1)];
					Spell_branche1_2 = branche1_2[urand(0, 1)];
					Spell_branche1_3 = branche1_3[urand(0, 2)];

					break;

				case 2: // Si Sacr� ------------------------------------------------------------------------------------------------------------------------------
					me->CastSpell(me, Buf_branche2, true);										// Buf2 sur lui meme
					me->CastSpell(me, Buf_branche2a, true);
					me->LoadEquipment(2, true);													// creature_equip_template 2
					Bonus_Armure(150);															// Bonus d'armure +50%

					Spell_branche2_agro = branche2_agro[urand(0, 3)];
					Spell_branche2_1 = branche2_1[urand(0, 1)];
					Spell_branche2_2 = branche2_2[urand(0, 1)];
					Spell_branche2_3 = branche2_3[urand(0, 1)];

					break;

				case 3: // Si Protection --------------------------------------------------------------------------------------------------------------------------
					me->CastSpell(me, Buf_branche3, true);										// Buf3 sur lui meme
					me->CastSpell(me, Buf_branche3a, true);
					me->LoadEquipment(3, true);													// creature_equip_template 3
					Bonus_Armure(175);															// Bonus d'armure +75%

					Spell_branche3_agro = branche3_agro[urand(0, 2)];
					Spell_branche3_1 = branche3_1[urand(0, 1)];
					Spell_branche3_2 = branche3_2[urand(0, 1)];
					Spell_branche3_3 = branche3_3[urand(0, 1)];

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
				me->GetMotionMaster()->MoveTargetedHome();							// Retour home pour rafraichir client
				//me->SetSpeedRate(MOVE_RUN, 1.01f);
				me->SetReactState(REACT_AGGRESSIVE);

				Init_AI();
				me->SetSheath(SHEATH_STATE_UNARMED);								//Arme rang�e
			}

			void EnterCombat(Unit* /*who*/) override
			{
				ResteADistance = urand(4, 6);
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
				me->RemoveAura(Buf_branche1a);
				me->RemoveAura(Buf_branche2);
				me->RemoveAura(Buf_branche2a);
				me->RemoveAura(Buf_branche3);
				me->RemoveAura(Buf_branche3a);

				Bonus_Armure(100);													// Retire bonus d'armure
			}
			void JustReachedHome() override
			{
				me->RemoveAura(Buf_branche1);
				me->RemoveAura(Buf_branche1a);
				me->RemoveAura(Buf_branche2);
				me->RemoveAura(Buf_branche2a);
				me->RemoveAura(Buf_branche3);
				me->RemoveAura(Buf_branche3a);

				Bonus_Armure(100);													// Retire bonus d'armure

				me->SetReactState(REACT_AGGRESSIVE);
				me->SetSheath(SHEATH_STATE_UNARMED);								//Arme rang�e
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
						Bonus_Degat_Arme_Done(-50);
						switch (BrancheSpe)
						{
						case 1: // Si Vindice ---------------------------------------------------------------------------------------------------------------------
							me->CastSpell(me, Buf_branche1, true);										// Buf3 sur lui meme
							me->CastSpell(me, Buf_branche1a, true); 

							Random = urand(1, 2);
							if (Random == 1 && UpdateVictim()) { DoCastVictim(Spell_branche1_agro); }	// 1/2 Chance de lancer le sort d'agro

							break;

						case 2: // Si Sacr� -----------------------------------------------------------------------------------------------------------------------
							me->CastSpell(me, Buf_branche2, true);										// Buf3 sur lui meme
							me->CastSpell(me, Buf_branche2a, true); 
							
							Random = urand(1, 2);
							if (Random == 1 && UpdateVictim()) { DoCastVictim(Spell_branche2_agro); }	// 1/2 Chance de lancer le sort d'agro

							break;

						case 3: // Si Protection ------------------------------------------------------------------------------------------------------------------
							me->CastSpell(me, Buf_branche3, true);										// Buf3 sur lui meme
							me->CastSpell(me, Buf_branche3a, true); 
							
							Random = urand(1, 2);
							if (Random == 1 && UpdateVictim()) { DoCastVictim(Spell_branche3_agro); }	// 1/2 Chance de lancer le sort d'agro

							break;

							// ####################################################################################################################################
						}
						Bonus_Degat_Arme_Done(50);
					}


					// ################################################################################################################################################
					// Combat suivant la Sp�cialisation



						switch (BrancheSpe)
						{
						case 1: // Sp�cialisation Armes (epee 2m)######################################################################################################
								// Regen Mana en combat ---------------------------------------------------------------------------------------------------------------
							if (Cooldown_RegenMana <= diff)
							{
								me->SetPower(POWER_MANA, Mana + (MaxMana / 2));
								if (Mana > MaxMana) { me->SetPower(POWER_MANA, MaxMana); }
								Cooldown_RegenMana = 2000;
							}
							else Cooldown_RegenMana -= diff;

							// Combat ---------------------------------------------------------------------------------------------------------------------------------
							//Bonus_Degat_Arme_Done(-10);										// Bonus de d�gat 

																							// Spell1 sur la cible
							if (Cooldown_Spell1 <= diff)
							{
								DoCastVictim(Spell_branche1_1);
								Cooldown_Spell1 = 6000;
							}
							else Cooldown_Spell1 -= diff;

							// Spell2 sur la cible
							if (Cooldown_Spell2 <= diff)
							{
								DoCastVictim(Spell_branche1_2);
								Cooldown_Spell2 = 2500;
							}
							else Cooldown_Spell2 -= diff;

							// Spell3 sur la cible 
							if (Cooldown_Spell3 <= diff)
							{
								DoCastVictim(Spell_branche1_3);
								Cooldown_Spell3 = 4000;
							}
							else Cooldown_Spell3 -= diff;

							//Bonus_Degat_Arme_Done(10);										// Retrait Bonus de d�gat 

							Heal_En_Combat_Melee(diff);
							break;

						case 2: // Sp�cialisation Sacre (Masse)########################################################################################################
								// Regen Mana en combat ---------------------------------------------------------------------------------------------------------------
							if (Cooldown_RegenMana <= diff)
							{
								me->setPowerType(POWER_MANA);
								me->SetPower(POWER_MANA, Mana + (MaxMana / 2));
								if (Mana > MaxMana) { me->SetPower(POWER_MANA, MaxMana); }

								Cooldown_RegenMana = 2000;
							}
							else Cooldown_RegenMana -= diff;

							//Bonus_Degat_Arme_Done(-10);

							// Combat ---------------------------------------------------------------------------------------------------------------------------------
							// Spell1 sur la cible
							if (Cooldown_Spell1 <= diff)
							{
								DoCastVictim(Spell_branche2_1);
								Cooldown_Spell1 = 6000;
							}
							else Cooldown_Spell1 -= diff;

							// Spell2 sur la cible
							if (Cooldown_Spell2 <= diff)
							{
								DoCastVictim(Spell_branche2_2);
								Cooldown_Spell2 = 3000;
							}
							else Cooldown_Spell2 -= diff;

							// Spell3 sur la cible
							if (Cooldown_Spell3 <= diff)
							{
								DoCastVictim(Spell_branche2_3);
								Cooldown_Spell3 = urand(8000, 10000);
							}
							else Cooldown_Spell3 -= diff;


							Heal_En_Combat_Heal(diff);
							//Bonus_Degat_Arme_Done(10);										// Reduction des degats parce que trop trop puissant

							break;

						case 3: // Sp�cialisation Protection (bouclier)################################################################################################
								// Regen Mana en combat ---------------------------------------------------------------------------------------------------------------
							if (Cooldown_RegenMana <= diff)
							{
								me->SetPower(POWER_MANA, Mana + (MaxMana / 2));
								if (Mana > MaxMana) { me->SetPower(POWER_MANA, MaxMana); }
								Cooldown_RegenMana = 2000;
							}
							else Cooldown_RegenMana -= diff;

							//Bonus_Degat_Arme_Done(-20);

							// Combat ---------------------------------------------------------------------------------------------------------------------------------
							// Spell1 sur la cible chaque (Sort R�guli�)
							if (Cooldown_Spell1 <= diff)
							{
								me->CastSpell(victim, Spell_branche3_1, true);
								Cooldown_Spell1 = 6000;
							}
							else Cooldown_Spell1 -= diff;

							// Spell2 sur la cible chaque (Sort secondaire plus lent)
							if (Cooldown_Spell2 <= diff)
							{
								me->CastSpell(victim, Spell_branche3_2, true);
								Cooldown_Spell2 = 3000;
							}
							else Cooldown_Spell2 -= diff;

							// Spell3 sur la cible  (Sort secondaire tres lent , g�n�ralement utilis� comme Dot)
							if (Cooldown_Spell3 <= diff)
							{
								me->CastSpell(victim, Spell_branche3_3, true);
								Cooldown_Spell3 = urand(10000, 12000);
							}
							else Cooldown_Spell3 -= diff;

							//Bonus_Degat_Arme_Done(20);

							Heal_En_Combat_Melee(diff);
							break;

						}

						ContreAttaque(diff);
						Mouvement_Contact(diff);

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
			void Mouvement_Contact(uint32 diff)
			{
				if (!UpdateVictim() || AuraFig�() /*|| me->HasUnitState(UNIT_STATE_CASTING)*/)
					return;

				Mana = me->GetPower(POWER_MANA);
				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				DoMeleeAttackIfReady();													// Combat en m�l�e

				// Si la cible >= 6m (pour �viter bug de rester fig�) --------------------------------------------------------------------------------------------
				if ((Dist >= 6) && (Cooldown_Anti_Bug_Figer <= diff))
				{
					float x = 0.0f, y = 0.0f, z = 0.0f;
					uint32 mapid = 0;
					x = victim->GetPositionX();
					y = victim->GetPositionY();
					if (me->GetMap()->IsOutdoors(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()) && !me->IsUnderWater())
					{
						z = victim->GetMap()->GetHeight(me->GetPhaseMask(), x, y, MAX_HEIGHT, true);
					}
					else
					{
						z = victim->GetPositionZ();	// Sinon bug en interieur
					}
					me->GetClosePoint(x, y, z, me->GetObjectSize() / 2, 2);
					me->GetMotionMaster()->MovePoint(mapid, x, y, z);
					me->GetMotionMaster()->MoveChase(victim, 1, frand(0, 6.2836f));			// Pour suivre la cible avec un angle
					Cooldown_Anti_Bug_Figer = 1000;
				}
				else Cooldown_Anti_Bug_Figer -= diff;

				// Si la cible < 8m -------------------------------------------------------------------------------------------------------------------------------------------
				if ((Dist < 10) && (Cooldown_ResteADistance <= diff) && (BrancheSpe != 3))
				{
					Random = urand(1, 5);
					if (Random == 1)
					{
						Tourne_Au_Tour_En_Combat();											// 2 chances sur 5 tourne au tour de sa victime
					}
					else if (Random == 2)
					{
						Avance_3m_En_Combat();												// 2 chances sur 5 avance
					}
					Cooldown_ResteADistance = urand(4000, 6000);
				}
				else Cooldown_ResteADistance -= diff;

			}

			void Tourne_Au_Tour_En_Combat()
			{
				if (!UpdateVictim() || AuraFig�())
					return;

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				float x = 0.0f, y = 0.0f, z = 0.0f;
				uint32 mapid = 0;

				x = (victim->GetPositionX() + urand(0, 2) - 1);
				y = (victim->GetPositionY() + urand(0, 2) - 1);
				if (me->GetMap()->IsOutdoors(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()) && !me->IsUnderWater())
				{
					z = victim->GetMap()->GetHeight(me->GetPhaseMask(), x, y, MAX_HEIGHT, true);
				}
				else
				{
					z = victim->GetPositionZ();	// Sinon bug en interieur
				}
				mapid = victim->GetMapId();
				//me->GetClosePoint(x, y, z, me->GetObjectSize() / 3, 3);
				me->GetMotionMaster()->MovePoint(mapid, x, y, z);
			}
			void Avance_3m_En_Combat()
			{
				if (!UpdateVictim() || AuraFig�())
					return;

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				float x = 0.0f, y = 0.0f, z = 0.0f;
				uint32 mapid = 0;

				x = (victim->GetPositionX());
				y = (victim->GetPositionY());
				if (me->GetMap()->IsOutdoors(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()) && !me->IsUnderWater())
				{
					z = victim->GetMap()->GetHeight(me->GetPhaseMask(), x, y, MAX_HEIGHT, true);
				}
				else
				{
					z = victim->GetPositionZ();	// Sinon bug en interieur
				}
				mapid = victim->GetMapId();
				me->GetClosePoint(x, y, z, me->GetObjectSize() / 3, 3);
				me->GetMotionMaster()->MovePoint(mapid, x, y, z);
			}
			void Heal_En_Combat_Heal(uint32 diff)
			{
				// Heal en combat ------------------------------------------------------------------------------------------------------------------------------
				if (Cooldown_Spell_Heal <= diff)
				{
					Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, DistanceDeCast, true);		// pour heal friend

																									// heal sur lui meme
					if ((me->GetHealth() < (me->GetMaxHealth()*0.50)))								// Si PV < 50%
					{
						DoCast(me, Spell_Heal_Heal);
						Cooldown_Spell_Heal = 6000;
					}

					// heal sur Friend 
					if (target = DoSelectLowestHpFriendly(DistanceDeCast))							// Distance de l'alli� = 30m
					{
						if (me->IsFriendlyTo(target) && (me != target))
						{
							if (target->GetHealth() < (target->GetMaxHealth()*0.50))				// Si PV < 50%
							{
								DoCast(target, Spell_Heal_Heal);
								Cooldown_Spell_Heal = 6000;
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
					Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, DistanceDeCast, true);		// pour heal friend
					// heal sur lui meme
					if ((me->GetHealth() < (me->GetMaxHealth()*0.50)))								// Si PV < 50%
					{
						DoCast(me, Spell_Heal_Caster);
						Cooldown_Spell_Heal = 7000;
					}

					// heal sur Friend 
					if (target = DoSelectLowestHpFriendly(DistanceDeCast))							// Distance de l'alli� = 30m
					{
						if (me->IsFriendlyTo(target) && (me != target))
						{
							if (target->GetHealth() < (target->GetMaxHealth()*0.50))				// Si PV < 50%
							{
								DoCast(target, Spell_Heal_Heal);
								Cooldown_Spell_Heal = 7000;
							}
						}
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
				// +- Bonus d'armure 100% = pas de bonus/malus   ex : Bonus_Armure(115) = Bonus d'armure +15%
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
					|| me->HasAura(33844)	// Sarments 4s
					|| me->HasAura(22127)	// Sarments 6s
					|| me->HasAura(31409)	// Sarment multiple
					|| me->HasAura(160402)	// Emprise terrestre (4s, 30m, comme Sarment mais avec des rocher )
					|| me->HasAura(45524)	// Cha�nes de glace
					|| me->HasAura(853)		// Marteau de la justice
					|| me->HasAura(339)		// Sarment du Totem de poigne de terre
					|| me->HasAura(64695)	// Sarment du Totem de poigne de terre
					|| me->HasAura(125467)	// Auto ROOT
					) return true;
				else return false;
			}

		};


		CreatureAI* GetAI(Creature* creature) const override
		{
			return new Stitch_npc_ai_paladinAI(creature);
		}
};

//##################################################################################################################################################################
void AddSC_npc_ai_paladin()
{
	new Stitch_npc_ai_paladin();
}















