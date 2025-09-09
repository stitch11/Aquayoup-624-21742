////#########################################################################################################################################################################################################################################
// Copyright (C) Juin 2020 Stitch pour Aquayoup
// AI generique npc par classe : DEMO Ver 2022-07-31
// Il est possible d'influencer le temp entre 2 cast avec `BaseAttackTime` & `RangeAttackTime` 
// Necessite dans Creature_Template :
// Minimun  : UPDATE `creature_template` SET `ScriptName` = 'Stitch_npc_ai_demo',`AIName` = '' WHERE (entry = 15100004);
// Optionel : UPDATE `creature_template` SET `HealthModifier` = 2, `ManaModifier` = 3, `ArmorModifier` = 1, `DamageModifier` = 2,`BaseAttackTime` = 2000, `RangeAttackTime` = 2000 WHERE(entry = 15100004);
// Optionel : Utilisez pickpocketloot de creature_template pour passer certains parametres (Solution choisit afin de rester compatible avec tout les cores). Si pickpocketloot = 1 (branche1 forc�), pickpocketloot = 2 (branche2 forc�), etc
//###########################################################################################################################################################################################################################################
// # npc de Test Stitch_npc_ai_demo  .npc 15100004
// REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `HealthModifier`, `HealthModifierExtra`, `ManaModifier`, `ManaModifierExtra`, `ArmorModifier`, `DamageModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES
// (15100004, 0, 0, 0, 0, 0, 7669, 0, 0, 0, 'npc_ai_Pretre', '', '', '', 0, 90, 90, 0, 0, 2102, 0, 1.01, 1.01, 0.7, 0, 0, 2000, 2000, 1, 1, 2, 0, 2048, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 2, 1, 3, 1, 2, 2, 1, 0, 144, 1, 0, 0, 'Stitch_npc_ai_demo', 20173);
// REPLACE INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`, `ItemID2`, `ItemID3`, `VerifiedBuild`) VALUES
// (15100004, 1, 15979, 0, 0, 0),
// (15100004, 2, 15979, 0, 0, 0),
// (15100004, 3, 15979, 0, 0, 0);
//###########################################################################################################################################################################################################################################


#include "CreatureTextMgr.h"

//################################################################################################
//StitchAI AI Demo
//################################################################################################

class Stitch_npc_ai_demo : public CreatureScript
{
public: Stitch_npc_ai_demo() : CreatureScript("Stitch_npc_ai_demo") { }

		struct Stitch_npc_ai_demoAI : public ScriptedAI
		{
			Stitch_npc_ai_demoAI(Creature* creature) : ScriptedAI(creature) { }

			uint32 BrancheSpe = 1;													// Choix de la Sp�cialisation : Affliction = 1, Demonologie = 2 , Destruction = 3
			uint32 NbrDeSpe = 3;													// Nombre de Sp�cialisations 
			uint32 ForceBranche;
			uint32 Random;
			uint32 DistanceDeCast = 40;												// Distance max a laquelle un npc attaquera , au dela il quite le combat
			uint32 ResteADistance = 12;												// Distance a laquelle un npc cast : ROOT +-5m
			uint32 Dist;															// Distance entre le npc et sa cible 
			uint32 Mana;
			uint32 MaxMana = me->GetMaxPower(POWER_MANA);
			uint32 MessageAlagro = 0;
			uint32 Spell_ContreAttaque = 0;
			Unit* victim = me->GetVictim();

			// Definitions des variables Cooldown et le 1er lancement
			uint32 Cooldown_Spell1 = 1000;
			uint32 Cooldown_Spell2 = 3000;
			uint32 Cooldown_Spell3 = 0;
			uint32 Cooldown_Spell4 = 4500;
			uint32 Cooldown_Spell_Heal = 3000;											// Cooldown pour la fr�quence du heal
			uint32 Cooldown_RegenMana = 3000;											// Cooldown pour le regen du mana
			uint32 Cooldown_ResteADistance = 1000;										// Test si en contact pour s'eloigner
			uint32 Cooldown_Npc_Emotes = urand(5000, 8000);
			uint32 Cooldown_Spell_ContreAttaque = 4000;
			uint32 Cooldown_Spell_ContreAttaque_defaut = 8000;

			// Spells Divers
			uint32 Buf_all = 79954;														// Armure d�moniaque 79934 , Gangrarmure 79954 
			uint32 Buf_branche1 = 0;
			uint32 Buf_branche1_liste[2] = { 104773, 104773 };							// R�solution interminable 104773 degat -40% 8s , Rune de puissance 116011 PS +40% 10s
			uint32 Buf_branche2 = 0;
			uint32 Buf_branche2_liste[2] = { 104773, 104773 };							// R�solution interminable 104773 degat -40% 8s , Rune de puissance 116011 PS +40% 10s
			uint32 Buf_branche3 = 0;
			uint32 Buf_branche3_liste[2] = { 104773, 104773 };							// R�solution interminable 104773 degat -40% 8s , Rune de puissance 116011 PS +40% 10s 
			uint32 Spell_Heal_Caster = 46155;  											// Drain de vie 46155
			uint32 Armure_Demoniaque = 79934;											// Armure_Demoniaque
			uint32 Gangrarmure = 79954;													// Gangrarmure
			uint32 Spell_Agonie = 300230;

			// Spells Affliction
			uint32 Spell_branche1_agro = 0;
			uint32 Spell_branche1_1 = 0;
			uint32 Spell_branche1_2 = 0;
			uint32 Spell_branche1_3 = 0;
			uint32 Spell_branche1_4 = 0;
			uint32 branche1_agro[2] = { 35183, 35183 };									// Affliction instable 35183
			uint32 branche1_1[2] = { 9613, 9613 };										// Trait de l'ombre 9613
			uint32 branche1_2[2] = { 300230, 300230 };									// Agonie 300230 
			uint32 branche1_3[2] = { 39621, 39621 };									// corruption 39621
			uint32 branche1_4[2] = { 171788, 171788 };									// Hanter 171788

			// Spells Demonologie
			uint32 Spell_branche2_agro = 0;	//    
			uint32 Spell_branche2_1 = 0;
			uint32 Spell_branche2_2 = 0;
			uint32 Spell_branche2_3 = 0;
			uint32 branche2_agro[2] = { 131381, 105174 };								// Feu de l'�me 131381, Main de Gul�dan 176725
			uint32 branche2_1[2] = { 9613, 9613 };										// Trait de l'ombre 9613
			uint32 branche2_2[2] = { 300230, 300230 };									// Agonie 300230 , corruption 39621
			uint32 branche2_3[2] = { 39367, 39367 };									// Graine de Corruption 39367
			uint32 Pet_Demo;
			uint32 Pet_Demo_Liste[5] = { 30146, 30146, 30146, 30146, 30146 };			// Gangregarde 30146, Succube 712, Chasseur corrompu 691, Marcheur du Vide 697, Diablotin 688

			// Spells Destruction
			uint32 Spell_branche3_agro = 0;
			uint32 Spell_branche3_1 = 0;
			uint32 Spell_branche3_2 = 0;
			uint32 Spell_branche3_3 = 0;
			uint32 branche3_agro[2] = { 77069, 77069 };									// Trait du chaos 77069
			uint32 branche3_1[2] = { 44519, 44519 };									// Incin�rer 44519
			uint32 branche3_2[2] = { 154083, 154083 };									// Conflagration 154083
			uint32 branche3_3[2] = { 44267, 44267 };									// Immolation 44267   
			
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
					BrancheSpe = urand(1, NbrDeSpe);
				}

				if ((BrancheSpe > NbrDeSpe) || (BrancheSpe == 0)) { BrancheSpe = 1; }

				me->RemoveAllControlled();															// renvois pet

				// Reste a distance variable suivant ci le mob est a l'ext�rieur ou a l'Int�rieur
				if (me->GetMap()->IsOutdoors(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()))
				{
					ResteADistance = urand(12, 14);
				}
				else
				{
					ResteADistance = 7;
				}

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
				case 1: // Si Affliction --------------------------------------------------------------------------------------------------------------------------
					Buf_branche1 = Buf_branche1_liste[urand(0, 1)];
					me->CastSpell(me, Buf_branche1, true);															// Buf1 sur lui meme
					me->CastSpell(me, Armure_Demoniaque, true);														// Gangrarmure, Armure_Demoniaque

					me->LoadEquipment(1, true);																		// creature_equip_template 1

					// Tirages al�atoires des spells Affliction 
					Spell_branche1_agro = branche1_agro[urand(0, 1)];
					Spell_branche1_1 = branche1_1[urand(0, 1)];
					Spell_branche1_2 = branche1_2[urand(0, 1)];
					Spell_branche1_3 = branche1_3[urand(0, 1)];
					Spell_branche1_4 = branche1_4[urand(0, 1)];
					break;

				case 2: // Si Demonologie -------------------------------------------------------------------------------------------------------------------------
					Buf_branche2 = Buf_branche2_liste[urand(0, 1)];
					me->CastSpell(me, Buf_branche2, true);															// Buf2 sur lui meme
					me->CastSpell(me, Armure_Demoniaque, true);														// Gangrarmure, Armure_Demoniaque

					me->LoadEquipment(2, true);																		// creature_equip_template 2

					// Tirages al�atoires des spells Demonologie 
					Spell_branche2_agro = branche2_agro[urand(0, 1)];
					Spell_branche2_1 = branche2_1[urand(0, 1)];
					Spell_branche2_2 = branche2_2[urand(0, 1)];
					Spell_branche2_3 = branche2_3[urand(0, 1)];

					// Tirages al�atoires du pet
					 Pet_Demo = Pet_Demo_Liste[urand(0, 4)];
					 me->CastSpell(me, Pet_Demo, true);
					break;

				case 3: // Si Destruction -------------------------------------------------------------------------------------------------------------------------
					Buf_branche3 = Buf_branche3_liste[urand(0, 1)];
					me->CastSpell(me, Buf_branche3, true);															// Buf3 sur lui meme
					me->CastSpell(me, Gangrarmure, true);															// Gangrarmure, Armure_Demoniaque

					me->LoadEquipment(3, true);																		// creature_equip_template 3

					// Tirages al�atoires des spells Affliction 
					Spell_branche3_agro = branche3_agro[urand(0, 1)];
					Spell_branche3_1 = branche3_1[urand(0, 1)];
					Spell_branche3_2 = branche3_2[urand(0, 1)];
					Spell_branche3_3 = branche3_3[urand(0, 1)];
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

				if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == WAYPOINT_MOTION_TYPE)
				{
					me->StopMoving();
					me->GetMotionMaster()->MoveIdle();
				}
				me->SetSheath(SHEATH_STATE_MELEE);									//Arme sortie

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
			}
			void JustReachedHome() override
			{
				me->RemoveAura(Buf_branche1);
				me->RemoveAura(Buf_branche2);
				me->RemoveAura(Buf_branche3);
				me->RemoveAura(Armure_Demoniaque);
				me->RemoveAura(Gangrarmure);

				me->SetSheath(SHEATH_STATE_UNARMED);								//Arme rang�e
				me->SetReactState(REACT_AGGRESSIVE);
				//me->SetSpeedRate(MOVE_RUN, 1.01f);										// Vitesse par defaut d�finit a 1.01f puisque le patch modification par type,famille test si 1.0f
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
				if (UpdateVictim() && !me->isMoving())

				{
					Mana = me->GetPower(POWER_MANA);
					Unit* victim = me->GetVictim();
					Dist = me->GetDistance(victim);

					// ################################################################################################################################################
					// Spell a lancer a l'agro 
					// ################################################################################################################################################
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


						switch (BrancheSpe)
						{
						case 1: // Si Affliction --------------------------------------------------------------------------------------------------------------------------
							me->CastSpell(me, Armure_Demoniaque, true);														// Buf sur lui meme
							Random = urand(1, 2);
							if (Random == 1) { me->CastSpell(victim, Spell_branche1_agro, true); }		// 1/2 Chance de lancer le sort d'agro
							break;

						case 2: // Si Demonologie -------------------------------------------------------------------------------------------------------------------------
							me->CastSpell(me, Armure_Demoniaque, true);														// Buf sur lui meme
							Random = urand(1, 2);
							if (Random == 1) { me->CastSpell(victim, Spell_branche2_agro, true); }		// 1/2 Chance de lancer le sort d'agro

							// Tirages al�atoires du pet
							Pet_Demo = Pet_Demo_Liste[urand(0, 4)];
							me->CastSpell(me, Pet_Demo, true);
							break;

						case 3: // Si Destruction -------------------------------------------------------------------------------------------------------------------------
							me->CastSpell(me, Gangrarmure, true);															// Buf sur lui meme
							Random = urand(1, 2);
							if (Random == 1) { me->CastSpell(victim, Spell_branche3_agro, true); }		// 1/2 Chance de lancer le sort d'agro
							break;

							// ################################################################################################################################################
						}
					}


					// ############################################################################################################################################
					// Combat suivant la Sp�cialisation
				switch (BrancheSpe)
				{
				case 1: // Sp�cialisation Affliction ##############################################################################################################
						// Regen mana en combat -------------------------------------------------------------------------------------------------------------------
					if (Cooldown_RegenMana <= diff)
					{
						me->SetPower(POWER_MANA, Mana + (MaxMana / 12));
						if (Mana > MaxMana) { me->SetPower(POWER_MANA, MaxMana); }
						Cooldown_RegenMana = urand(2500, 3000);
					}
					else Cooldown_RegenMana -= diff;

					// Combat -------------------------------------------------------------------------------------------------------------------------------------
					// Spell4 sur la cible  (Sort secondaire tres lent , g�n�ralement utilis� comme Dot)
					if (Cooldown_Spell4 <= diff && !victim->HasAura(Spell_branche1_4))
					{
						DoCastAOE(Spell_branche1_4, true);
						Cooldown_Spell4 = urand(8000, 10000);
					}
					else Cooldown_Spell4 -= diff;

					// Spell3 sur la cible  (Sort secondaire tres lent , g�n�ralement utilis� comme Dot)
					if (Cooldown_Spell3 <= diff && !victim->HasAura(Spell_branche1_3))
					{
						DoCastVictim(Spell_branche1_3);
						Cooldown_Spell3 = urand(10000, 12000);
					}
					else Cooldown_Spell3 -= diff;

					// Spell2 sur la cible chaque (Sort secondaire plus lent)
					if (Cooldown_Spell2 <= diff && !victim->HasAura(Spell_branche1_2) && Spell_branche1_2 != Spell_Agonie)
					{
						DoCastVictim(Spell_branche1_2);
						Cooldown_Spell2 = urand(12000, 14000);
					}
					else Cooldown_Spell2 -= diff;

					// Spell1 sur la cible chaque (Sort R�guli�)
					if (Cooldown_Spell1 <= diff)
					{
							me->StopMoving();
							DoCastVictim(Spell_branche1_1);
							Cooldown_Spell1 = 3000;
					}
					else Cooldown_Spell1 -= diff;
					break;
				case 2: // Sp�cialisation Demonologie #############################################################################################################
						// Regen mana en combat -------------------------------------------------------------------------------------------------------------------
					if (Cooldown_RegenMana <= diff)
					{
						me->SetPower(POWER_MANA, Mana + (MaxMana / 12));
						if (Mana > MaxMana) { me->SetPower(POWER_MANA, MaxMana); }
						Cooldown_RegenMana = urand(2500, 3000);
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
							Cooldown_Spell1 = 3500;
						}
					}
					else Cooldown_Spell1 -= diff;

					// Spell2 sur la cible chaque (Sort secondaire plus lent)
					if (Cooldown_Spell2 <= diff && !victim->HasAura(Spell_branche2_2) && Spell_branche2_2 != Spell_Agonie)
					{
						DoCastVictim(Spell_branche2_2);
						Cooldown_Spell2 = urand(14000, 18000);
					}
					else Cooldown_Spell2 -= diff;

					// Spell3 sur la cible  (Sort secondaire tres lent , g�n�ralement utilis� comme Dot)
					if (Cooldown_Spell3 <= diff && !victim->HasAura(Spell_branche2_3))
					{
						DoCastVictim(Spell_branche2_3);
						Cooldown_Spell3 = urand(12000, 14000);
					}
					else Cooldown_Spell3 -= diff;
					break;
				case 3: // Sp�cialisation Destruction #############################################################################################################
						// Regen mana en combat ------------------------------------------------------------------------------------------------------------------------
					if (Cooldown_RegenMana <= diff)
					{
						me->SetPower(POWER_MANA, Mana + (MaxMana / 20));
						if (Mana > MaxMana) { me->SetPower(POWER_MANA, MaxMana); }
						Cooldown_RegenMana = urand(2000, 2500);
					}
					else Cooldown_RegenMana -= diff;

					// Combat --------------------------------------------------------------------------------------------------------------------------------------
					// Spell3 sur la cible  (Sort secondaire tres lent , g�n�ralement utilis� comme Dot)
					if (Cooldown_Spell3 <= diff && !victim->HasAura(Spell_branche3_3))
					{
						DoCastVictim(Spell_branche3_3);
						Cooldown_Spell3 = urand(10000, 12000);
					}
					else Cooldown_Spell3 -= diff;

					// Spell2 sur la cible chaque (Sort secondaire plus lent)
					if (Cooldown_Spell2 <= diff && !victim->HasAura(Spell_branche3_2))
					{
						DoCastVictim(Spell_branche3_2, true);
						Cooldown_Spell2 = 6500;
					}
					else Cooldown_Spell2 -= diff;

					// Spell1 sur la cible chaque (Sort R�guli�)
					if (Cooldown_Spell1 <= diff)
					{
						if (!me->HasUnitState(UNIT_STATE_MOVE))
						{
							me->StopMoving();
							DoCastVictim(Spell_branche3_1);
							Cooldown_Spell1 = 3500;
						}
					}
					else Cooldown_Spell1 -= diff;
					break;

				}

				// ################################################################################################################################################
				Heal_En_Combat_Caster(diff);
				ContreAttaque(diff);
				}
				// ################################################################################################################################################
				Mouvement_Caster(diff);
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
				if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING) || me->isMoving() || AuraFig�())
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

						if (AuraLenteur() == false && !Interieur())
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

				// Mouvement OFF si Mana > 5% & distance >= 6m & <= 10m ---------------------------------------------------------------------------------------------
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
			void Heal_En_Combat_Caster(uint32 diff)
			{
				if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_MOVE))
					return;
				
				// heal sur lui meme-------------------------------------------------------------------------------------------------------------------------------
				if (Cooldown_Spell_Heal <= diff)
				{
					if ((me->GetHealth() < (me->GetMaxHealth()*0.50)))								// Si PV < 50%
					{
						DoCastVictim(Spell_Heal_Caster);
						Cooldown_Spell_Heal = 3500;
					}
				}
				else Cooldown_Spell_Heal -= diff;


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
			return new Stitch_npc_ai_demoAI(creature);
		}
};

//##################################################################################################################################################################
void AddSC_npc_ai_demo()
{
	new Stitch_npc_ai_demo;
}















