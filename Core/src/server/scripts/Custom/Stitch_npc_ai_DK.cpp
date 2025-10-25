////#########################################################################################################################################################################################################################################
// Copyright (C) Juin 2020 Stitch pour Aquayoup
// AI generique npc par classe : DK Ver 2025-10
// Il est possible d'influencer le temp entre 2 cast avec `BaseAttackTime` & `RangeAttackTime` 
// Necessite dans Creature_Template :
// Minimun  : UPDATE `creature_template` SET `ScriptName` = 'Stitch_npc_ai_dk',`AIName` = '' WHERE (entry = 15100009);
// Optionel : Utilisez pickpocketloot de creature_template pour passer certains parametres (Solution choisit afin de rester compatible avec tout les cores). Si pickpocketloot = 1 (branche1 forc�), pickpocketloot = 2 (branche2 forc�), etc
//###########################################################################################################################################################################################################################################
// # npc de Test Stitch_npc_ai_dk  .npc 15100009
//REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `HealthModifier`, `HealthModifierExtra`, `ManaModifier`, `ManaModifierExtra`, `ArmorModifier`, `DamageModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES
//(15100009, 0, 0, 0, 0, 0, 25447, 0, 0, 0, 'npc_ai_DK', '', '', '', 0, 90, 90, 0, 0, 2102, 0, 1.01, 1.01, 1, 0, 0, 2000, 2000, 1, 1, 1, 0, 2048, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 2, 1, 1, 1, 2, 2, 1, 0, 144, 1, 0, 0, 'Stitch_npc_ai_dk', 20173);
//REPLACE INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`, `ItemID2`, `ItemID3`, `VerifiedBuild`) VALUES
//(15100009, 1, 38707, 0, 0, 0),
//(15100009, 2, 25168, 0, 0, 0),
//(15100009, 3, 28184, 28184, 0, 0),
//(15100009, 4, 88857, 0, 0, 0);
//###########################################################################################################################################################################################################################################

#include "CreatureTextMgr.h"

//################################################################################################
//StitchAI AI DK
//################################################################################################

class Stitch_npc_ai_dk : public CreatureScript
{
public: Stitch_npc_ai_dk() : CreatureScript("Stitch_npc_ai_dk") { }

		struct Stitch_npc_ai_dkAI : public ScriptedAI
		{
			Stitch_npc_ai_dkAI(Creature* creature) : ScriptedAI(creature) { }

			uint32 BrancheSpe = 1;													// Choix de la Sp�cialisation : Sang=1, Givre=2, Impie=3, Chaos=4
			uint32 NbrDeSpe = 4;													// Nombre de Sp�cialisations
			uint32 ForceBranche;
			uint32 Random;
			uint32 DistanceDeCast = 40;												// Distance max a laquelle un npc attaquera , au dela il quite le combat
			uint32 ResteADistance = 5;												// Distance max a laquelle un npc s'approchera
			uint32 Dist;															// Distance entre le npc et sa cible
			uint32 Mana;
			uint32 MaxMana = 100;
			Unit* victim = me->GetVictim();										 
			uint32 MessageAlagro = 0;
			uint32 Spell_ContreAttaque = 0;

			// Definitions des variables Cooldown et le 1er lancement
			uint32 Cooldown_Spell1 = 500;
			uint32 Cooldown_Spell2 = 2000;
			uint32 Cooldown_Spell3 = 4000;
			uint32 Cooldown_Spell4 = 6000;
			uint32 Cooldown_Spell_Heal_1 = 4000;
			uint32 Cooldown_Spell_Heal_2 = 6000;
			uint32 Cooldown_RegenMana = 3000;
			uint32 Cooldown_ResteADistance = 4000;									// Test si en contact
			uint32 Cooldown_Cible_trop_loin = 2000;
			uint32 Cooldown_ResteAuContact;
			uint32 Cooldown_Anti_Bug_Figer = 1000;
			uint32 Cooldown_Npc_Emotes = urand(5000, 8000);
			uint32 Cooldown_Peste_et_Fievre = 12000;
			uint32 Cooldown_Strangulation = urand(3000,10000);
			uint32 Cooldown_Spell_ContreAttaque = 4000;
			uint32 Cooldown_Spell_ContreAttaque_defaut = 8000;

			// Spells Divers    
			uint32 Buf_branche1 = 50689;											// Pr�sence de sang 48263/50689
			uint32 Buf_branche1a = 57330;											// V�t�ran de la Troisi�me guerre 50029 (endu+10%), Cor de l�hiver 57330
			uint32 Buf_branche2 = 48266;											// Pr�sence de givre 48266
			uint32 Buf_branche2a = 31821;											// Puissance des terres gel�es 81333 (m�l�e+35%), Pilier de givre 51271 (force+15% insensible mouvement), 
			uint32 Buf_branche3 = 48265;											// Pr�sence impie 48265
			uint32 Buf_branche3a = 49222;											// Carapace anti-magie 48707, Bouclier d�os 49222, Invocation d'une gargouille 49206, R�animation morbide 46584
			uint32 Buf_branche4 = 300049;											// Pr�sence du Chaos 300049
			uint32 Buf_branche4a = 300248;											// Chancre impie 300248
			uint32 Spell_Heal_1 = 48982;  											// Connexion runique 48982
			uint32 Spell_Heal_2 = 46283;  											// Voile mortel 46283
			uint32 Spell_Cible_trop_loin = 300239;									// Poigne_de_la_mort 49576/30010, Nova de sang 300059, Souffle de givre 300239
			uint32 Peste_de_sang = 60950;											// 3s/12s
			uint32 fievre_de_givre = 67719;											// 12s, vit -25%
			uint32 Peste_necrotique = 300232;										// 3s/30s
			uint32 Passage_de_givre = 3714;
			uint32 Robustesse_glaciale = 48792;
			uint32 Strangulation = 47476;
			uint32 Pet_Dk;
			uint32 Liste_Pet_Dk[5] = { 46584, 46584, 46584, 49206 ,49206 };			// R�animation morbide 46584, Arm�e des morts 42650, Invocation d'une gargouille 49206 

			// Spells Sang
			uint32 Spell_branche1_agro = 0;
			uint32 Spell_branche1_1 = 0;
			uint32 Spell_branche1_2 = 0;
			uint32 Spell_branche1_3 = 0;
			uint32 Spell_branche1_4 = 0;
			uint32 branche1_agro[3] = { 49576, 300233, 130736 };					// Poigne de la mort 49576, Pouss�e de fi�vre 300233, Faucheur d��me 130736
			uint32 branche1_1[2] = { 300198, 300198 };								// Frappe de sang 300198   
			uint32 branche1_2[2] = { 45470, 45470 };								// Frappe de mort 45470
			uint32 branche1_3[2] = { 165760, 165760 };								// Frappe au c�ur 165760
			uint32 branche1_4[2] = { 43265, 92025 };								// Mort et d�composition 43265, Furoncle sanglant 92025

			// Spells Givre
			uint32 Spell_branche2_agro = 0;
			uint32 Spell_branche2_1 = 0;
			uint32 Spell_branche2_2 = 0;
			uint32 Spell_branche2_3 = 0;
			uint32 Spell_branche2_4 = 0;
			uint32 branche2_agro[4] = { 45524, 45524, 47528, 47528 };				// Cha�nes de glace 45524, Gel de l'esprit 47528 
			uint32 branche2_1[2] = { 300197, 300197 };								// Toucher de glace 300197
			uint32 branche2_2[2] = { 49143, 49143 };								// Frappe de givre 49143, 
			uint32 branche2_3[2] = { 300234, 300234 };								// An�antissement 300234
			uint32 branche2_4[2] = { 49184, 49184 };								// Rafale hurlante 49184
			
			// Spells Impie
			uint32 Spell_branche3_agro = 0;
			uint32 Spell_branche3_1 = 0;
			uint32 Spell_branche3_2 = 0;
			uint32 Spell_branche3_3 = 0;
			uint32 Spell_branche3_4 = 0;
			uint32 branche3_agro[3] = { 300233, 130736, 130736 };					// Pouss�e de fi�vre 300233, Faucheur d��me 130736
			uint32 branche3_1[2] = { 300199, 300199 };								// Frappe de peste 300199
			uint32 branche3_2[2] = { 172680, 172680 };								// Frappe du Fl�au 172680 
			uint32 branche3_3[2] = { 300235, 300235 };								// Frappe purulente 300235
			uint32 branche3_4[2] = { 92025, 92025 };								// Furoncle sanglant 92025 

			// Spells Chaos
			uint32 Spell_branche4_agro = 0;
			uint32 Spell_branche4_1 = 0;
			uint32 Spell_branche4_2 = 0;
			uint32 Spell_branche4_3 = 0;
			uint32 Spell_branche4_4 = 0;
			uint32 branche4_agro[3] = { 300240, 300240, 45524 };					// Choc impie 300240, Strangulation 47476, Cha�nes de glace 45524
			uint32 branche4_1[2] = { 300237, 300237 };								// Javelot de givre 300237
			uint32 branche4_2[2] = { 300238, 300238 };								// Javelot de peste 300238
			uint32 branche4_3[2] = { 300236, 300236 };								// Javelot de sang 300236
			uint32 branche4_4[2] = { 300240, 300239 };								// Choc impie 300240, Souffle de givre 300239
			
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
				else if (ForceBranche == 4) { BrancheSpe = 4; }										// branche4 forc�
				else
				{
					// Sinon Choix de la Sp�cialisation Al�atoire
					BrancheSpe = urand(1, NbrDeSpe);
				}

				if ((BrancheSpe > NbrDeSpe) || (BrancheSpe == 0)) { BrancheSpe = 1; }


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
				case 1: // Si Sang --------------------------------------------------------------------------------------------------------------------------------
					me->CastSpell(me, Buf_branche1, true);
					me->CastSpell(me, Buf_branche1a, true);
					me->LoadEquipment(1, true);													// creature_equip_template 1

					Spell_branche1_agro = branche1_agro[urand(0, 2)];
					Spell_branche1_1 = branche1_1[urand(0, 1)];
					Spell_branche1_2 = branche1_2[urand(0, 1)];
					Spell_branche1_3 = branche1_3[urand(0, 1)];
					Spell_branche1_4 = branche1_4[urand(0, 1)];

					Pet_Dk = Liste_Pet_Dk[urand(0, 4)];

					Bonus_Armure(150);															// Bonus d'armure +50%
					ResteADistance = 5;
					break;

				case 2: // Si Givre -------------------------------------------------------------------------------------------------------------------------------
					me->CastSpell(me, Buf_branche2, true);										// Buf2 sur lui meme
					me->CastSpell(me, Buf_branche2a, true);
					me->LoadEquipment(2, true);													// creature_equip_template 2

					Spell_branche2_agro = branche2_agro[urand(0, 3)];
					Spell_branche2_1 = branche2_1[urand(0, 1)];
					Spell_branche2_2 = branche2_2[urand(0, 1)];
					Spell_branche2_3 = branche2_3[urand(0, 1)];
					Spell_branche2_4 = branche2_4[urand(0, 1)];

					Pet_Dk = Liste_Pet_Dk[urand(0, 4)];

					Bonus_Armure(150);															// Bonus d'armure +50%
					ResteADistance = 5;
					break;

				case 3: // Si Impie -------------------------------------------------------------------------------------------------------------------------------
					me->CastSpell(me, Buf_branche3, true);										// Buf3 sur lui meme
					me->CastSpell(me, Buf_branche3a, true);
					me->LoadEquipment(3, true);													// creature_equip_template 3

					Spell_branche3_agro = branche3_agro[urand(0, 2)];
					Spell_branche3_1 = branche3_1[urand(0, 1)];
					Spell_branche3_2 = branche3_2[urand(0, 1)];
					Spell_branche3_3 = branche3_3[urand(0, 1)];
					Spell_branche3_4 = branche3_4[urand(0, 1)];

					Pet_Dk = Liste_Pet_Dk[urand(0, 4)];

					Bonus_Armure(150);															// Bonus d'armure +50%
					ResteADistance = 5;
					break;

				case 4: // Si Chaos -------------------------------------------------------------------------------------------------------------------------------
					me->CastSpell(me, Buf_branche4, true);										// Buf4 sur lui meme
					me->CastSpell(me, Buf_branche4a, true);
					me->LoadEquipment(4, true);													// creature_equip_template 4

					Spell_branche4_agro = branche4_agro[urand(0, 2)];
					Spell_branche4_1 = branche4_1[urand(0, 1)];
					Spell_branche4_2 = branche4_2[urand(0, 1)];
					Spell_branche4_3 = branche4_3[urand(0, 1)];
					Spell_branche4_4 = branche4_4[urand(0, 1)];

					Spell_Cible_trop_loin = 47476;												// Nova de sang 300059, Strangulation 47476

					Bonus_Armure(150);															// Bonus d'armure +50%

					// Reste a distance variable suivant ci le mob est a l'ext�rieur ou a l'Int�rieur
					if (me->GetMap()->IsOutdoors(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()))
					{
						ResteADistance = urand(14, 16);
					}
					else
					{
						ResteADistance = 7;
					}

					Cooldown_ResteADistance = 1000;
					break;

				}
				// ################################################################################################################################################
				// Divers
				// ################################################################################################################################################
				me->SetSheath(SHEATH_STATE_MELEE);												// S'equipe de l'arme au contact
				me->SetReactState(REACT_AGGRESSIVE);
				VisuelPowerRunic();
				// ################################################################################################################################################
			}
			void JustRespawned() override
			{
				me->GetMotionMaster()->MoveTargetedHome();								// Retour home pour rafraichir client
				//me->SetSpeedRate(MOVE_RUN, 1.01f);
				me->SetReactState(REACT_AGGRESSIVE);

				VisuelPowerRunic();
				Init_AI();
				me->SetSheath(SHEATH_STATE_UNARMED);								//Arme rang�e
			}

			void EnterCombat(Unit* /*who*/) override
			{
				VisuelPowerRunic();
				ResteADistance = urand(4, 6);
				Init_AI();

				if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == WAYPOINT_MOTION_TYPE)
				{
					me->StopMoving();
					//me->GetMotionMaster()->MoveIdle();
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

				me->RemoveAura(Buf_branche1);
				me->RemoveAura(Buf_branche1a);
				me->RemoveAura(Buf_branche2);
				me->RemoveAura(Buf_branche2a);
				me->RemoveAura(Buf_branche3);
				me->RemoveAura(Buf_branche3a);
				me->RemoveAura(Buf_branche4);
				me->RemoveAura(Buf_branche4a);

				Bonus_Armure(100);														// Retire bonus d'armure
				VisuelPowerRunic();
			}
			void JustReachedHome() override
			{
				me->RemoveAura(Buf_branche1);
				me->RemoveAura(Buf_branche1a);
				me->RemoveAura(Buf_branche2);
				me->RemoveAura(Buf_branche2a);
				me->RemoveAura(Buf_branche3);
				me->RemoveAura(Buf_branche3a);
				me->RemoveAura(Buf_branche4);
				me->RemoveAura(Buf_branche4a);

				Bonus_Armure(100);														// Retire bonus d'armure

				me->SetSheath(SHEATH_STATE_UNARMED);								//Arme rang�e
				me->SetReactState(REACT_AGGRESSIVE);
				VisuelPowerRunic();
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
					Unit* victim = me->GetVictim();
					Dist = me->GetDistance(victim);

					if (Start_Agro == 0)
					{
						Start_Agro = 1;
						VisuelPowerRunic();


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
						case 1: // Si Sang ------------------------------------------------------------------------------------------------------------------------
							me->CastSpell(me, Buf_branche1, true);										// Buf3 sur lui meme
							me->CastSpell(me, Buf_branche1a, true); 

							Random = urand(1, 3);
							if (Random != 3 && UpdateVictim()) { DoCastVictim(Spell_branche1_agro); }	// 2/3 Chance de lancer le sort d'agro
							else
								me->CastSpell(victim, Pet_Dk, true);
							break;

						case 2: // Si Givre -----------------------------------------------------------------------------------------------------------------------
							me->CastSpell(me, Buf_branche2, true);										// Buf3 sur lui meme
							me->CastSpell(me, Buf_branche2a, true); 
							
							me->CastSpell(victim, Robustesse_glaciale, true);							// Pour visuel
							me->CastSpell(victim, Passage_de_givre, true);								// Pour visuel

							Random = urand(1, 3);
							if (Random != 1 && UpdateVictim()) { DoCastVictim(Spell_branche2_agro); }	// 2/3 Chance de lancer le sort d'agro
							else
								me->CastSpell(victim, Pet_Dk, true);
							break;

						case 3: // Si Impie -----------------------------------------------------------------------------------------------------------------------
							me->CastSpell(me, Buf_branche3, true);										// Buf3 sur lui meme
							me->CastSpell(me, Buf_branche3a, true); 
							
							Random = urand(1, 3);
							if (Random != 3 && UpdateVictim()) { DoCastVictim(Spell_branche3_agro); }	// 2/3 Chance de lancer le sort d'agro
							else
								me->CastSpell(victim, Pet_Dk, true);
							break;

						case 4: // Si Chaos -----------------------------------------------------------------------------------------------------------------------
							me->CastSpell(me, Buf_branche4, true);										// Buf4 sur lui meme
							me->CastSpell(me, Buf_branche4a, true);

							Random = urand(1, 3);
							if (Random != 3 && UpdateVictim()) { DoCastVictim(Spell_branche4_agro); }	// 2/3 Chance de lancer le sort d'agro
							else
								me->CastSpell(victim, Pet_Dk, true);
							break;

						}
					}

					// ############################################################################################################################################
					// Combat suivant la Sp�cialisation
					// ############################################################################################################################################

					// Visuel Power en combat ---------------------------------------------------------------------------------------------------------------------
					if (Cooldown_RegenMana <= diff)
					{
						me->ModifyPower(POWER_RUNES, 66); // +1 Rune Sang , +1 Rune Givre , +1 Rune Impie
						Cooldown_RegenMana = 3000;
					}
					else Cooldown_RegenMana -= diff;

						switch (BrancheSpe)
						{
						case 1: // Sp�cialisation Sang ############################################################################################################
							// Combat -----------------------------------------------------------------------------------------------------------------------------
							Bonus_Degat_Arme_Done(-10);													// Reduction des degats inflig�s
							if (Dist < 6)
							{

								// Applique Peste_de_sang, fievre_de_givre ou Peste_necrotique sur la cible
								if (Cooldown_Peste_et_Fievre <= diff)
								{
									me->CastSpell(victim, Peste_de_sang, true);
									Cooldown_Peste_et_Fievre = 12000;
								}
								else Cooldown_Peste_et_Fievre -= diff;

								// Applique Strangulation sur la cible
								if (Cooldown_Strangulation <= diff)
								{
									me->CastSpell(victim, Strangulation, true);
									Cooldown_Strangulation = 60000;
								}
								else Cooldown_Strangulation -= diff;

								// ------------------------------------------------------------------------------------------------------------------------------------

								// Spell1 sur la cible
								if (Cooldown_Spell1 <= diff)
								{
									me->CastSpell(victim, Spell_branche1_1, true);
									Cooldown_Spell1 = 6000;
								}
								else Cooldown_Spell1 -= diff;

								// Spell2 sur la cible
								if (Cooldown_Spell2 <= diff)
								{
									me->CastSpell(victim, Spell_branche1_2, true);
									Cooldown_Spell2 = 6000;
								}
								else Cooldown_Spell2 -= diff;

								// Spell3 sur la cible 
								if (Cooldown_Spell3 <= diff)
								{
									me->CastSpell(victim, Spell_branche1_3, true);
									Cooldown_Spell3 = 6000;
								}
								else Cooldown_Spell3 -= diff;

								// Spell4 sur la cible 
								if (Cooldown_Spell4 <= diff)
								{
									me->CastSpell(victim, Spell_branche1_4, true);
									Cooldown_Spell4 = 25000;
								}
								else Cooldown_Spell4 -= diff;
							}
							Bonus_Degat_Arme_Done(10);													// Degats inflig�s d'origines
							Heal_En_Combat_Melee(diff);
							Mouvement_Contact(diff);
							break;

						case 2: // Sp�cialisation Givre ###########################################################################################################

							// Combat -----------------------------------------------------------------------------------------------------------------------------
							Bonus_Degat_Arme_Done(-25);													// Reduction des degats inflig�s
							if (Dist < 6)
							{
								// Applique Peste_de_sang, fievre_de_givre ou Peste_necrotique sur la cible
								if (Cooldown_Peste_et_Fievre <= diff)
								{
									me->CastSpell(victim, fievre_de_givre, true);
									Cooldown_Peste_et_Fievre = 15000;
								}
								else Cooldown_Peste_et_Fievre -= diff;

								// Applique Strangulation sur la cible
								if (Cooldown_Strangulation <= diff)
								{
									me->CastSpell(victim, Strangulation, true);
									Cooldown_Strangulation = 60000;
								}
								else Cooldown_Strangulation -= diff;

								// ------------------------------------------------------------------------------------------------------------------------------------

								// Spell1 sur la cible
								if (Cooldown_Spell1 <= diff)
								{
									me->CastSpell(victim, Spell_branche2_1, true);
									Cooldown_Spell1 = 6000;
								}
								else Cooldown_Spell1 -= diff;

								// Spell2 sur la cible
								if (Cooldown_Spell2 <= diff)
								{
									me->CastSpell(victim, Spell_branche2_2, true);
									Cooldown_Spell2 = 6000;
								}
								else Cooldown_Spell2 -= diff;

								// Spell3 sur la cible
								if (Cooldown_Spell3 <= diff)
								{
									me->CastSpell(victim, Spell_branche2_3, true);
									Cooldown_Spell3 = 6000;
								}
								else Cooldown_Spell3 -= diff;

								// Spell4 sur la cible
								if (Cooldown_Spell4 <= diff)
								{
									me->CastSpell(victim, Spell_branche2_4, true);
									Cooldown_Spell4 = 10000;
								}
								else Cooldown_Spell4 -= diff;
							}
							Bonus_Degat_Arme_Done(25);													// Degats inflig�s d'origines
							Heal_En_Combat_Melee(diff);
							Mouvement_Contact(diff);
							break;

						case 3: // Sp�cialisation Impie ###########################################################################################################

							// Combat -----------------------------------------------------------------------------------------------------------------------------
							Bonus_Degat_Arme_Done(-50);													// Reduction des degats inflig�s
							if (Dist < 6)
							{
								// Applique Peste_de_sang, fievre_de_givre ou Peste_necrotique sur la cible
								if (Cooldown_Peste_et_Fievre <= diff)
								{
									//me->CastSpell(victim, Peste_necrotique, true);
									Cooldown_Peste_et_Fievre = 30000;
								}
								else Cooldown_Peste_et_Fievre -= diff;

								// Applique Strangulation sur la cible
								if (Cooldown_Strangulation <= diff)
								{
									me->CastSpell(victim, Strangulation, true);
									Cooldown_Strangulation = 60000;
								}
								else Cooldown_Strangulation -= diff;

								// ------------------------------------------------------------------------------------------------------------------------------------

								// Spell1 sur la cible chaque (Sort R�guli�)
								if (Cooldown_Spell1 <= diff)
								{
									me->CastSpell(victim, Spell_branche3_1, true);
									Cooldown_Spell1 = 6000;
									DoMeleeAttackIfReady();						// Combat en m�l�e
								}
								else Cooldown_Spell1 -= diff;

								// Spell2 sur la cible chaque (Sort secondaire plus lent)
								if (Cooldown_Spell2 <= diff)
								{
									me->CastSpell(victim, Spell_branche3_2, true);
									Cooldown_Spell2 = 6000;
								}
								else Cooldown_Spell2 -= diff;

								// Spell3 sur la cible  (Sort secondaire tres lent , g�n�ralement utilis� comme Dot)
								if (Cooldown_Spell3 <= diff)
								{
									me->CastSpell(victim, Spell_branche3_3, true);
									Cooldown_Spell3 = 6000;
								}
								else Cooldown_Spell3 -= diff;

								// Spell4 sur la cible
								if (Cooldown_Spell4 <= diff)
								{
									DoCastVictim(Spell_branche3_4);
									Cooldown_Spell4 = 10000;
								}
								else Cooldown_Spell4 -= diff;
							}
							Bonus_Degat_Arme_Done(50);													// Degats inflig�s d'origines
							Heal_En_Combat_Melee(diff);
							Mouvement_Contact(diff);
							break;

						case 4: // Sp�cialisation Chaos ###########################################################################################################

							// Combat -----------------------------------------------------------------------------------------------------------------------------
							Bonus_Degat_Arme_Done(-25);													// Reduction des degats inflig�s

							// Applique Peste_de_sang, fievre_de_givre ou Peste_necrotique sur la cible
							if (Cooldown_Peste_et_Fievre <= diff)
							{
								me->CastSpell(victim, fievre_de_givre, true);
								Cooldown_Peste_et_Fievre = 15000;
							}
							else Cooldown_Peste_et_Fievre -= diff;

							// Applique Strangulation sur la cible
							if (Cooldown_Strangulation <= diff)
							{
								me->CastSpell(victim, Strangulation, true);
								Cooldown_Strangulation = 60000;
							}
							else Cooldown_Strangulation -= diff;

							// ------------------------------------------------------------------------------------------------------------------------------------

							// Spell1 sur la cible chaque (Sort R�guli�)
							if (Cooldown_Spell1 <= diff)
							{
								if (!me->HasUnitState(UNIT_STATE_MOVE))
								{
									//DoCast(victim, Spell_branche4_1);
									me->StopMoving();
									DoCastVictim(Spell_branche4_1);
									Cooldown_Spell1 = 7500;
									DoMeleeAttackIfReady();						// Combat en m�l�e
								}
							}
							else Cooldown_Spell1 -= diff;

							// Spell2 sur la cible chaque (Sort secondaire plus lent)
							if (Cooldown_Spell2 <= diff)
							{
								DoCast(victim, Spell_branche4_2);
								Cooldown_Spell2 = 7500;
							}
							else Cooldown_Spell2 -= diff;

							// Spell3 sur la cible  (Sort secondaire tres lent , g�n�ralement utilis� comme Dot)
							if (Cooldown_Spell3 <= diff)
							{
								DoCast(victim, Spell_branche4_3);
								Cooldown_Spell3 = 7500;
							}
							else Cooldown_Spell3 -= diff;

							// Spell4 sur la cible
							if (Cooldown_Spell4 <= diff)
							{
								me->CastSpell(victim, Spell_branche4_4, true);
								Cooldown_Spell4 = urand(10000,14000);
							}
							else Cooldown_Spell4 -= diff;

							Bonus_Degat_Arme_Done(25);													// Degats inflig�s d'origines
							Heal_En_Combat_Melee(diff);
							Mouvement_Caster(diff);
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
				if (!UpdateVictim() || AuraFig�())
					return;

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				// ------ ALLER A LA CIBLE -------------------------------------------------------------------------------------------------------------------------
				if (Cooldown_Anti_Bug_Figer <= diff)
				{
					if (Dist >= ResteADistance)
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

				// Si la cible est entre 10 & 25m : Poigne de la mort ---------------------------------------------------------------------------------------------
				if (Cooldown_Cible_trop_loin <= diff && Dist >= 10 && Dist <= 25 )
				{
					Random = urand(1, 2);
						if (Random == 1 || (BrancheSpe == 3))
						{
							DoCastVictim(Spell_Cible_trop_loin);							// Poigne de la mort - 1 chance sur 2 ou impie  
							Cooldown_Cible_trop_loin = urand(15000, 20000);
							Cooldown_ResteADistance = 4000;									
						}
				}
				else Cooldown_Cible_trop_loin -= diff;


				// Si la cible < 8m : avance ou tourne au tour de sa victime
				if (Dist < 8)
				{
					if (Cooldown_ResteADistance <= diff)
					{
					Random = urand(1, 5);
					if (Random == 1)														// 1 chance sur 5 avance Au contact
					{
						Avance_3m_En_Combat();							
						Cooldown_ResteADistance = 3000;
					} else if(Random == 2 || Random == 3 )									// 2 chances sur 5 Tourne_Au_Tour 
					{
						Tourne_Au_Tour_En_Combat();
						Cooldown_ResteADistance = urand(4000, 6000);
					}
				}
				else Cooldown_ResteADistance -= diff;
				}
			}
			void Mouvement_Caster(uint32 diff)
			{
				if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING) || AuraFig�())
					return;

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				if (Cooldown_ResteADistance <= diff)
				{
					// Mouvement al�atoire si cible < 6m ----------------------------------------------------------------------------------------------------------

					if (Dist <6)
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

				// Mouvement OFF si distance >= 6m & <= 10/15m ---------------------------------------------------------------------------------------------
				if ((Dist >= ResteADistance - 4) && (Dist <= ResteADistance))
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
					me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);							// UNROOT
					me->GetMotionMaster()->MoveChase(victim, ResteADistance);							// Pour suivre la cible
					void DoRangedAttackIfReady();														// Combat a distance
				}

			}

			void Tourne_Au_Tour_En_Combat()
			{
				if (!UpdateVictim() || AuraFig�())
					return;

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

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

				x = (victim->GetPositionX() );
				y = (victim->GetPositionY() );
				z = victim->GetMap()->GetHeight(me->GetPhaseMask(), x, y, MAX_HEIGHT, true);
				mapid = victim->GetMapId();
				me->GetClosePoint(x, y, z, me->GetObjectSize() / 3, 3);
				me->GetMotionMaster()->MovePoint(mapid, x, y, z);
			}

			void Heal_En_Combat_Melee(uint32 diff)
			{
				// Heal en combat ------------------------------------------------------------------------------------------------------------------------------
				if (!UpdateVictim())
					return;

				// heal_1 sur lui meme --------------------------------------------------------------------
				if (Cooldown_Spell_Heal_1 <= diff)
				{
					if (me->GetHealth() < (me->GetMaxHealth()*0.70)) 								// Si PV < 70%
					{
						DoCast(me, Spell_Heal_1);
						Cooldown_Spell_Heal_1 = 20000;
					}
				}else Cooldown_Spell_Heal_1 -= diff;

				// heal_2 sur lui meme --------------------------------------------------------------------
				if (Cooldown_Spell_Heal_2 <= diff)
				{
					Unit* victim = me->GetVictim();
					if (me->GetHealth() < (me->GetMaxHealth()*0.50))								// Si PV < 50%
					{
						//me->CastSpell(victim, Spell_Heal_2, true);								// D�sactiv� parce que fear penible
						Cooldown_Spell_Heal_2 = 30000;
					}
				}else Cooldown_Spell_Heal_2 -= diff;
			

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
			void VisuelPowerRunic()
			{
				me->setPowerType(POWER_RUNIC_POWER);
				me->SetMaxPower(POWER_MANA, 1000);
				me->ModifyPower(POWER_MANA, 1000);
				me->setPowerType(POWER_RUNIC_POWER);
				me->SetMaxPower(POWER_RUNIC_POWER, 1000);
				me->SetPower(POWER_RUNIC_POWER, 1000);
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
					|| me->HasAura(3600)	// Totem de Lien � la terre
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
			bool Interieur()
			{
				if (me->GetMap()->IsOutdoors(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()))
					return false;
				else return true;
			}

		};




		CreatureAI* GetAI(Creature* creature) const override
		{
			return new Stitch_npc_ai_dkAI(creature);
		}
};

//##################################################################################################################################################################
void AddSC_npc_ai_dk()
{
	new Stitch_npc_ai_dk();
}















