////#########################################################################################################################################################################################################################################
// Copyright (C) Juillet 2020 Stitch pour https:\\Aquayoup.123.fr
// AI generique npc par famille : Mobs Ver 2020-12-08 (family)
//
// Si spell[1] = 0 : alors affectation aléatoire de tous les spells(prédéfini dans le core), sinon utilisera les spells définis dans creature_template spell[1 a 5]
// BETE
// UPDATE `creature_template` SET `spell1` = ATTAQUE, `spell2` = DOT, `spell3` = AGRO, `spell4` = EVADE, `spell5` = BUF, `ScriptName` = 'Stitch_npc_ai_mobs', `AIName` = '' WHERE(entry = 15100015);

// spell1 : Attaque principale
// spell2 : Dot
// spell3 : spell lancé a l'agro
// spell4 : spell lancé a l'évade ou respawn
// spell5 : Buf
// spell6 : Heal(lui même uniquement)

/*
1     Loup - CREATURE_FAMILY_WOLF								UPDATE `creature_template` SET `modelid1` = 903, `family` = 1,`InhabitType` = 3, `unit_class` = 1,`name` = 'npc_ai_Loup', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
2     Felin - CREATURE_FAMILY_CAT								UPDATE `creature_template` SET `modelid1` = 320, `family` = 2,`InhabitType` = 3, `unit_class` = 1,`name` = 'npc_ai_Felin', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
3 	  Araignee - CREATURE_FAMILY_SPIDER							UPDATE `creature_template` SET `modelid1` = 513, `family` = 3,`InhabitType` = 3, `unit_class` = 1,`name` = 'npc_ai_Araignée', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
4 	  Ours - CREATURE_FAMILY_BEAR								UPDATE `creature_template` SET `modelid1` = 820, `family` = 4,`InhabitType` = 3, `unit_class` = 1,`name` = 'npc_ai_Ours', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
5 	  Sanglier - CREATURE_FAMILY_BOAR							UPDATE `creature_template` SET `modelid1` = 503, `family` = 5,`InhabitType` = 3, `unit_class` = 1,`name` = 'npc_ai_Sanglier', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
6	  Crocodile - CREATURE_FAMILY_CROCOLISK						UPDATE `creature_template` SET `modelid1` = 1039, `family` = 6,`InhabitType` = 3, `unit_class` = 1,`name` = 'npc_ai_Crocodile', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
7 	  Oiseau charogniar - CREATURE_FAMILY_CARRION_BIRD			UPDATE `creature_template` SET `modelid1` = 2305,`family` = 7,`InhabitType` = 4, `unit_class` = 1,`name` = 'npc_ai_Charognard', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
8 	  Crabe - CREATURE_FAMILY_CRAB								UPDATE `creature_template` SET `modelid1` = 1938,`family` = 8,`InhabitType` = 3, `unit_class` = 1,`name` = 'npc_ai_Crabe', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
9 	  Gorille - CREATURE_FAMILY_GORILLA							UPDATE `creature_template` SET `modelid1` = 809, `family` = 9,`InhabitType` = 3, `unit_class` = 1,`name` = 'npc_ai_Gorille', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
11    Raptor - CREATURE_FAMILY_RAPTOR							UPDATE `creature_template` SET `modelid1` =1747, `family` = 11,`InhabitType` = 3, `unit_class` = 1,`name` = 'npc_ai_Raptor', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
16    Marcheur du Vide - CREATURE_FAMILY_VOIDWALKER				UPDATE `creature_template` SET `modelid1` =1132, `family` = 16,`InhabitType` = 3, `unit_class` = 4,`name` = 'npc_ai_Marcheur_du_Vide', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
17    Succube - CREATURE_FAMILY_SUCCUBUS						UPDATE `creature_template` SET `modelid1` =4162, `family` = 17,`InhabitType` = 3, `unit_class` = 4,`name` = 'npc_ai_Succube', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
19    Garde funeste (Doomguard) - CREATURE_FAMILY_DOOMGUARD		UPDATE `creature_template` SET `modelid1`=65072, `family` = 19,`InhabitType` = 3, `unit_class` = 1,`name` = 'npc_ai_Garde funeste', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
20    Scorpion - CREATURE_FAMILY_SCORPID						UPDATE `creature_template` SET `modelid1`= 9749, `family` = 20,`InhabitType` = 3, `unit_class` = 1,`name` = 'npc_ai_Scorpion', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
21    Tortue - CREATURE_FAMILY_TURTLE							UPDATE `creature_template` SET `modelid1`= 45691,`family` = 21,`InhabitType` = 3, `unit_class` = 1,`name` = 'npc_ai_Tortue', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
23    Diablotin (Imp) - CREATURE_FAMILY_IMP						UPDATE `creature_template` SET `modelid1`= 10812,`family` = 23,`InhabitType` = 3, `unit_class` = 4,`name` = 'npc_ai_Imp', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
24    Chauve-Souris - CREATURE_FAMILY_BAT						UPDATE `creature_template` SET `modelid1`= 47213,`family` = 24,`InhabitType` = 4, `unit_class` = 1,`name` = 'npc_ai_Chauve-Souris', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
25    Hyene - CREATURE_FAMILY_HYENA								UPDATE `creature_template` SET `modelid1`= 1536, `family` = 25,`InhabitType` = 1, `unit_class` = 1,`name` = 'npc_ai_Hyene', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
26    Oiseau de proie - CREATURE_FAMILY_BIRD_OF_PREY			UPDATE `creature_template` SET `modelid1`= 34007,`family` = 26,`InhabitType` = 4, `unit_class` = 1,`name` = 'npc_ai_Oiseau de proie', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
27    Serpent des vents - CREATURE_FAMILY_WIND_SERPENT			UPDATE `creature_template` SET `modelid1`= 10991,`family` = 27,`InhabitType` = 5, `unit_class` = 2,`name` = 'npc_ai_Serpent des vents', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
29    Gangregarde (Felguard) - CREATURE_FAMILY_FELGUARD			UPDATE `creature_template` SET `modelid1`= 7970, `family` = 29,`InhabitType` = 3, `unit_class` = 4,`name` = 'npc_ai_ Gangregarde', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
30    Faucon dragon - CREATURE_FAMILY_DRAGONHAWK				UPDATE `creature_template` SET `modelid1`= 29363,`family` = 30,`InhabitType` = 5, `unit_class` = 2,`name` = 'npc_ai_Faucon dragon', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
31    Ravageur - CREATURE_FAMILY_RAVAGER						UPDATE `creature_template` SET `modelid1`= 16631,`family` = 31,`InhabitType` = 1, `unit_class` = 1,`name` = 'npc_ai_Ravageur', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
34    Raie du Neant - CREATURE_FAMILY_NETHER_RAY				UPDATE `creature_template` SET `modelid1`= 21155,`family` = 34,`InhabitType` = 4, `unit_class` = 2,`name` = 'npc_ai_Raie du Neant', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
35    Serpent - CREATURE_FAMILY_SERPENT							UPDATE `creature_template` SET `modelid1`= 33990,`family` = 35,`InhabitType` = 3, `unit_class` = 1,`name` = 'npc_ai_Serpent', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
37    Papillon de nuit - CREATURE_FAMILY_MOTH					UPDATE `creature_template` SET `modelid1`= 17798,`family` = 37,`InhabitType` = 4, `unit_class` = 2,`name` = 'npc_ai_Papillon', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
38    Chimere - CREATURE_FAMILY_CHIMAERA						UPDATE `creature_template` SET `modelid1`= 20571,`family` = 38,`InhabitType` = 4, `unit_class` = 2,`name` = 'npc_ai_Chimere', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
39    Diablosaure - CREATURE_FAMILY_DEVILSAUR					UPDATE `creature_template` SET `modelid1`= 5238, `family` = 39,`InhabitType` = 1, `unit_class` = 1,`name` = 'npc_ai_Diablosaure', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
40    Goule - CREATURE_FAMILY_GHOUL								UPDATE `creature_template` SET `modelid1`= 26329,`family` = 40,`InhabitType` = 1, `unit_class` = 1,`name` = 'npc_ai_Goule', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
42    Ver - CREATURE_FAMILY_WORM								UPDATE `creature_template` SET `modelid1`= 7549, `family` = 42,`InhabitType` = 1, `unit_class` = 1,`name` = 'npc_ai_Ver', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
43    Rhinoceros - CREATURE_FAMILY_RHINO						UPDATE `creature_template` SET `modelid1`= 26280,`family` = 43,`InhabitType` = 1, `unit_class` = 1,`name` = 'npc_ai_Rhinoceros', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
44    Guepe - CREATURE_FAMILY_WASP								UPDATE `creature_template` SET `modelid1`= 37743,`family` = 44,`InhabitType` = 4, `unit_class` = 1,`name` = 'npc_ai_Guepe', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
45    Chien de base - CREATURE_FAMILY_CORE_HOUND				UPDATE `creature_template` SET `modelid1`= 24908,`family` = 45,`InhabitType` = 1, `unit_class` = 1,`name` = 'npc_ai_Chien de base', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
49    Elementaire d'eau - CREATURE_FAMILY_WATER_ELEMENTAL		UPDATE `creature_template` SET `modelid1`= 525,  `family` = 49,`InhabitType` = 1, `unit_class` = 2,`name` = "npc_ai_Elementaire d'eau", `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
50    Renard - CREATURE_FAMILY_FOX								UPDATE `creature_template` SET `modelid1`= 30302,`family` = 50,`InhabitType` = 1, `unit_class` = 1,`name` = "npc_ai_Renard", `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
51    Singe - CREATURE_FAMILY_MONKEY							UPDATE `creature_template` SET `modelid1`= 30180,`family` = 51,`InhabitType` = 1, `unit_class` = 1,`name` = "npc_ai_Singe", `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
52    Chien - CREATURE_FAMILY_DOG								UPDATE `creature_template` SET `modelid1`= 24908,`family` = 52,`InhabitType` = 1, `unit_class` = 1,`name` = 'npc_ai_Chien', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
55    Araignee de schiste - CREATURE_FAMILY_SHALE_SPIDER		UPDATE `creature_template` SET `modelid1`= 46290,`family` = 55,`InhabitType` = 1, `unit_class` = 1,`name` = 'npc_ai_Araignee de schiste', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
56    Zombie - CREATURE_FAMILY_ZOMBIE							UPDATE `creature_template` SET `modelid1`= 1065, `family` = 56,`InhabitType` = 1, `unit_class` = 1,`name` = 'npc_ai_Zombie', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
68    Hydre - CREATURE_FAMILY_HYDRA								UPDATE `creature_template` SET `modelid1`= 2423, `family` = 68,`InhabitType` = 3, `unit_class` = 1,`name` = 'npc_ai_Hydre', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
100   Diablotin gangrene - CREATURE_FAMILY_FELIMP				UPDATE `creature_template` SET `modelid1`= 16890,`family` =100,`InhabitType` = 1, `unit_class` = 1,`name` = 'npc_ai_Diablotin gangrene', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
104   Garde-courroux - CREATURE_FAMILY_WRATHGUARD				UPDATE `creature_template` SET `modelid1`= 63969,`family` =104,`InhabitType` = 1, `unit_class` = 1,`name` = 'npc_ai_Garde-courroux', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
108   Infernal - CREATURE_FAMILY_INFERNAL						UPDATE `creature_template` SET `modelid1`= 169,`family` =108,`InhabitType` = 1, `unit_class` = 1,`name` = 'npc_ai_Infernal', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
116   elementaire de feu - CREATURE_FAMILY_FIREELEMENTAL		UPDATE `creature_template` SET `modelid1`= 1405,`family` =116,`InhabitType` = 1, `unit_class` = 2,`name` = 'npc_ai_elementaire de feu', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
117   elementaire de terre - CREATURE_FAMILY_EARTHELEMENTAL		UPDATE `creature_template` SET `modelid1`= 14511,`family` =117,`InhabitType` = 1, `unit_class` = 2,`name` = 'npc_ai_elementaire de terre', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
130   Basilic - CREATURE_FAMILY_BASILISK						UPDATE `creature_template` SET `modelid1`= 46056,`family` =130,`InhabitType` = 1, `unit_class` = 1,`name` = 'npc_ai_Basilic', `ScriptName` = 'Stitch_npc_ai_mobs' WHERE (entry = 15100015);
*/

// Il est possible d'influencer le temp entre 2 cast avec `BaseAttackTime` & `RangeAttackTime` 
// Necessite dans Creature_Template :
// Minimun  : UPDATE `creature_template` SET `ScriptName` = 'Stitch_npc_ai_mobs',`AIName` = '' WHERE (entry = 15100015);
// Optionel : UPDATE `creature_template` SET `HealthModifier` = 2, `ManaModifier` = 3, `ArmorModifier` = 1, `DamageModifier` = 2,`BaseAttackTime` = 2000, `RangeAttackTime` = 2000 WHERE(entry = 15100013);
// REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `HealthModifier`, `HealthModifierExtra`, `ManaModifier`, `ManaModifierExtra`, `ArmorModifier`, `DamageModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES
// (15100015, 0, 0, 0, 0, 0, 903, 0, 0, 0, 'npc_ai_Loup', '', '', '', 0, 90, 90, 0, 0, 2102, 0, 1.01, 1.01, 0.75, 0, 0, 2000, 2000, 1, 1, 1, 0, 2048, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 2, 1, 1, 1, 2, 2, 1, 0, 144, 1, 0, 0, 'Stitch_npc_ai_mobs', 20173);
//###########################################################################################################################################################################################################################################



//################################################################################################
//StitchAI AI Mobs
//################################################################################################

class Stitch_npc_ai_mobs : public CreatureScript
{
public: Stitch_npc_ai_mobs() : CreatureScript("Stitch_npc_ai_mobs") { }

		struct Stitch_npc_ai_mobsAI : public ScriptedAI
		{
			Stitch_npc_ai_mobsAI(Creature* creature) : ScriptedAI(creature) { }


			uint32 Random;
			uint32 DistanceDeCast = 30;												// Distance max a laquelle un npc attaquera , au dela il quite le combat
			uint32 ResteADistance = 5;												// Distance max a laquelle un npc s'approchera
			uint32 Dist;															// Distance entre le npc et sa cible
			Unit* victim = me->GetVictim();					
			uint32 Crfamily = me->GetCreatureTemplate()->family;
			uint32 ForceFamily = me->GetCreatureTemplate()->pickpocketLootId;
			uint32 Ordre_De_Charge = 0;
			float x, y, z, mapid;
			uint8 Spell_B_Non_Cumulable;											// == 1 : Spell_B ne sera pas appliqué s'il est deja actif sur la cible . Par exemple pour Brise-genou

			// Definitions des variables Cooldown et le 1er lancement
			uint32 Cooldown_SpellA = 1000;											// Sort principal
			uint32 Cooldown_SpellA_defaut = 3000;
			uint32 Cooldown_SpellB = 3000;											// Sort secondaire moins rapide (généralement un DOT)
			uint32 Cooldown_SpellB_defaut = 5000;
			uint32 Cooldown_SpellB_rapide = 3000;									// Cadence de tir SpellB rapide pour Mouvement_Cast_Puis_Contact
			uint32 Cooldown_SpellB_rapide_defaut = Cooldown_SpellB_defaut;			// Cadence de tir SpellB normale pour Mouvement_Cast_Puis_Contact
			uint32 Cooldown_Principal_A = 2000;										// ex Cooldown_Anti_Bug_Figer
			uint32 Cooldown_Principal_A_Defaut = 2000;								// Cooldown_Anti_Bug_Figer_Defaut
			uint32 Cooldown_Spell_Heal;												// Heal ou sort de sauvegarde si perte de pv
			uint32 Cooldown_Spell_Heal_defaut;
			uint32 Cooldown_Principal_B = 2000;										// Test si en contact , Cooldown_ResteADistance
			uint32 Cooldown_Principal_B_Defaut = 4000;								// Cooldown_ResteADistance_Defaut
			uint32 Cooldown_Trop_Loin = 4000;
			uint32 Cooldown_Trop_Loin_Defaut = 10000;
			uint32 Base_Cooldown_Cast_A = 4000;										// Cooldown de base pour l'attaque principal, il est utilisé avec des valeurs ajouté en +-, sert a definir Cooldown_SpellA_defaut
			uint32 Base_Cooldown_Cast_B = 10000;										// Idem pour le sort secondaire, généralement un DOT
			uint32 AI_Random;


			// Spells 
			uint32 Spell_A;
			uint32 Spell_B;
			uint32 Spell_agro;
			uint32 Spell_respawn_evade;
			uint32 Buf_A;
			uint32 Spell_Heal;
			uint32 Spell_Trop_Loin = 0; 
			uint32 Spell_Bond_Aleatoire = 70485;								// BOND Aleatoire
			uint32 Spell_Instincts_de_survie = 61336;
			uint32 Spell_Griffure_bondissante = 89712;							// Griffure bondissante (bond)
			uint32 Spell_Toile_Araignee = 15471;								// Linceul de toiles 20m 4s
			uint32 Spell_Charge_Stun = 87187;									// Charge farouche 87187 (8-25 m ours, stun 4s)
			uint32 Spell_Charge_Repousse = 19196;								// charge afflux 
			uint32 Spell_Charge = 20508;										// Charge (dmg et sans effet) 93515, charge avec trainée 20508, charge 32323 (sonne 2s,8-25m ) 
			uint32 Spell_Poursuite = 30151;										// Poursuite 30151 (Gangregarde : charge, vit + 30 %/6s, >8m, recharge 15s)
			uint32 Spell_Vitesse_Flamboyante = 108843;							// 6s 150 %
			uint32 Spell_Vitesse_4s = 137573;									// vitesse + 70 % / 4s
			uint32 Spell_stun_1s = 149902;										// stun 1s 10m
			uint32 Spell_Bondir_Guerrier = 145763;								// Bondir 8-40m
			uint32 Spell_Invisible = 1784;										// Invisibilitée fufu
			uint32 Spell_No_ModelID = 137358;									// ModelID non visible , arme && aura visible 
			uint32 Spell_Senterre = 79690;										// visuel terre Forage de tunnel Passif gris
			uint32 Spell_Gaz_Corrosif = 126437;
			uint32 Spell_Armure_De_Givre = 165743;
			uint32 Spell_Armure_De_La_Fournaise = 79849;
			uint32 Spell_Armure_Des_Elements = 29718;
			uint32 Spell_Ecorce = 22812;
			uint32 Spell_Bouclier_De_Foudre = 324;
			uint32 Spell_Bouclier_De_Terre = 974;
			uint32 Spell_Epines = 33907;
			uint32 Spell_Forme_Dombre = 15473;
			uint32 Spell_Bouclier_Dos = 49222;
			uint32 Spell_Robustesse_Glaciale = 48792;
			uint32 Spell_Passage_De_Givre = 3714;
			uint32 Poison_Tir_Catapulte = 79607;
			uint32 Lancer_Une_Pierre = 130775;
			uint32 Cri_Incapacitant = 18328;



			// --- family defaut
			uint32 liste_spell_A_0[2] = { 24187, 119004 };						// griffe 24187, Violent coup direct 119004
			uint32 liste_spell_B_0[3] = { 131193, 131172, 140476 };				// Choc(coup de poing 30 / lvl) 131193, Coup de tete(10 / lvl) 131172, Vil crachat 140476
			uint32 liste_agro_0[2] = { 0, 0 };
			uint32 liste_Buf_0[3] = { 22863, 22863, 18328 };					// Vitesse 10s 30% 22863, Cri incapacitant 18328




			//--- family : tirage aléatoire des spells
			// 1	Loup - CREATURE_FAMILY_WOLF 
			uint32 liste_spell_A_1[3] = { 85835, 113687, 113687 };				// Mutiler 85835, morsure 113687
			uint32 liste_spell_B_1[4] = { 3427, 9080, 9080, 50075 };			// Blessure infectée 3427, Brise-genou 9080, Chair mutilée 50075 (dégâts +25x5)
			uint32 liste_agro_1[2] = { 75355,75355 };							// Hurlement horrible 75355 (vit. d'attaque -10%/15s) 
			uint32 liste_Buf_1[6] = { 59737, 3149, 3149, 15727, 15727, 8599 };	// vitesse 59737, Hurlement furieux	3149 (force +15), Rugissement démoralisant 15727, Enrager 8599 (+DPS,+Vit dattaque)

			// 2	Felin - CREATURE_FAMILY_CAT
			uint32 liste_spell_A_2[2] = { 24187, 24187 };						// griffe 24187
			uint32 liste_spell_B_2[4] = { 84867, 300220, 1079, 300220 };		// Balayage fracassant (cum 5 fois) 84867, Griffure 300220, Dechirure 1079 (24s)
			uint32 liste_agro_2[2] = { 89712, 138766 };							// Griffure bondissante (bond) 89712, Rugissement perçant 138766
			uint32 liste_Buf_2[3] = { 15971, 15727, 44531  };					// Rugissement démoralisant 15971 (Réduit PA mêlée des ennemis 10m/30s), Rugissement démoralisant 15727, Célérité 44531
			
			// 3	Araignee - CREATURE_FAMILY_SPIDER
			uint32 liste_spell_A_3[2] = { 113687, 300206 };						// Morsure	113687, Mutiler	300206
			uint32 liste_spell_B_3[2] = { 21067, 88876 };						// Eclair de poison 21067, Venin paralysant 88876
			uint32 liste_agro_3[4] = { 15471, 15471, 15471, 28434 };			// Toile d'ataignée (Linceul de toiles) 15471,Toile d'araignée (attire la cible + cocoon) 28434
			uint32 liste_Buf_3[2] = { 0,0 };						
			
			//4		Ours - CREATURE_FAMILY_BEAR
			uint32 liste_spell_A_4[3] = { 300206, 300206, 300207 };				// Mutiler 300206, Mutilation 300207
			uint32 liste_spell_B_4[3] = { 300208, 33745, 84867 };				// Rosser 300208, Lacerer 33745, Balayage fracassant 84867 (DPS, armure -5% + 2/lvl/20s cumulable 5 fois)
			uint32 liste_agro_4[2] = { 87187, 300209 };							// Charge farouche 87187 8 - 25 m ours, Coup de crâne 300209 (interrompt  4s)
			uint32 liste_Buf_4[2] = { 106898, 22812  };							// Ruée rugissante 106898 (vit +60%), Ecorce (degats -20% /12s) 22812

			//5		Sanglier - CREATURE_FAMILY_BOAR
			uint32 liste_spell_A_5[3] = { 300206, 152571, 300208 };				// Mutiler 300206, Rosser 300208 (Ours) , encorner 152571 (cone 5m ; Soi-même)
			uint32 liste_spell_B_5[5] = { 81691, 50303, 50303, 50303, 131172 };	// Cornépine 5m (repousse) 81691, Grippe porcine 50303, Coup de tete 131172
			uint32 liste_agro_5[2] = { 32323,32323 };							// charge (sonne 2s,8-25m ) 32323
			uint32 liste_Buf_5[3] = { 183883, 1160, 8599 };						// Hurlement de rage vitesse 183883 (dps +25%/12s), Cri démoralisant 1160, Enrager	(+degat physique+vit dattaque, 2mn, visuel rouge) 8599

			//6 	Crocodile - CREATURE_FAMILY_CROCOLISK
			uint32 liste_spell_A_6[2] = { 113687, 113687 };						// Morsure 113687
			uint32 liste_spell_B_6[4] = { 138765, 136870, 127987, 131626 };		// Écraser 138765 armure -10%/30s, Heurt de queue 136870 (repousse legement, armure -10%/20s ,cumulable 10 fois), Morsure hémorragique 127987, Gifle de la queue 131626
			uint32 liste_agro_6[2] = { 12097, 12097 };							// Perce-armure 12097 (armure -75%/15s 5m)
			uint32 liste_Buf_6[2] = { 0, 0 };									// 

			// 7 	Oiseau charogniar - CREATURE_FAMILY_CARRION_BIRD
			uint32 liste_spell_A_7[3] = { 55079, 115132, 134537 };				// Rapace 55079, Plongeon 115132, Coup de bec 134537
			uint32 liste_spell_B_7[3] = { 30639, 163716, 124515 };				// Morsure carnivore 30639, Griffure de serres 163716, Barrage de coups de bec 124515
			uint32 liste_agro_7[2] = { 89712, 18328 };							// Griffure_bondissante 89712, Cri incapacitant 18328 (vit -60%)
			uint32 liste_Buf_7[5] = { 44531, 44531, 183883, 3149, 70485 };		// Célérité 44531, *Hurlement de rage 183883 (vit, dps +25%/12s), Hurlement furieux 3149, Bond_Aleatoire 70485

			// 8	Crabe - CREATURE_FAMILY_CRAB	
			uint32 liste_spell_A_8[2] = { 118140, 118140 };						// Pince 118140
			uint32 liste_spell_B_8[3] = { 84867, 84867, 79175 };				// Balayage fracassant 84867 (griffe, armure -5%, cumulable 5 fois), Coup frénétique 79175
			uint32 liste_agro_8[2] = { 89712, 80807 };							// Griffure bondissante (bond) 89712, Splendeur 80807 (cone nuage terre)
			uint32 liste_Buf_8[2] = { 22863, 5171 };							// Vitesse 22863 (10s 30%), Débiter 5171 (vitesse d'attaque)

			// 9	Gorille - CREATURE_FAMILY_GORILLA	
			uint32 liste_spell_A_9[4] = { 51944, 119004, 119004, 131193 };			// Piétiner 51944, Violent coup direct 119004, Choc 131193 (coup de poing 30/lvl)
			uint32 liste_spell_B_9[5] = { 138765, 138765, 118682, 118682, 6253 };	// Écraser 138765 (armure -10%/30s), Taper du pied 118682, revers 6253 (sonne 2s)
			uint32 liste_agro_9[3] = { 18328, 50231, 145763 };						// Cri incapacitant 18328 (vit -60%), agro 50231, Bondir 145763 (guerrier)
			uint32 liste_Buf_9[3] = { 183883, 15727, 18499 };						// Hurlement de rage 183883 (vit,dps +25%/12s), Rugissement démoralisant 15727 (Réduit PA mêlée 10m/20s), Rage de berserker	18499

			// 11    Raptor - CREATURE_FAMILY_RAPTOR
			uint32 liste_spell_A_11[2] = { 113687, 113687 };					// Morsure 113687, 
			uint32 liste_spell_B_11[4] = { 30639, 136870, 118576, 36991 };		// Morsure carnivore 30639, Heurt de queue 136870 (repousse legerement, armure -10%/20s, cumulable 10 fois), Kigrince 118576 (canalisé 3s, cone 10/lvl), Pourfendre 36991/772
			uint32 liste_agro_11[3] = { 89712, 50231, 18328 };					// Griffure bondissante 89712 (bond), agro 50231, Cri incapacitant 18328 (vit -60%)	
			uint32 liste_Buf_11[3] = { 22863, 1160, 8599 };						// Vitesse 22863 (10s 30%), Cri démoralisant 1160, Enrager 8599 (+degat physique+vit dattaque,visuel rouge)

			// 16    Marcheur du Vide - CREATURE_FAMILY_VOIDWALKER
			uint32 liste_spell_A_16[2] = { 24187, 24187 };						// griffe 24187
			uint32 liste_spell_B_16[2] = { 3716, 3716 };						// Tourment 3716
			uint32 liste_agro_16[2] = { 17735, 17735 };							// Souffrance 17735
			uint32 liste_Buf_16[2] = { 115232, 115232 };						// Bouclier de l’ombre 115232 (dmg -60%/30s)

			// 17    Succube - CREATURE_FAMILY_SUCCUBUS
			uint32 liste_spell_A_17[2] = { 7814, 7814 };						// Fouet de la douleur 7814
			uint32 liste_spell_B_17[2] = { 6360, 6360 };						// Coup de fouet 6360 (dmg,repousse)
			uint32 liste_agro_17[2] = { 86377, 86377 };							// Séduction 86377 (stun 6s) /6358 (stun 30s)
			uint32 liste_Buf_17[2] = { 0, 0 };									//  

			// 19    Garde funeste (Doomguard) - CREATURE_FAMILY_DOOMGUARD
			uint32 liste_spell_A_19[2] = { 77558, 77558 };						// Frappe sanglante 77558
			uint32 liste_spell_B_19[2] = { 127171, 118532 };					// Fendoir vicieux 127171, Entaille infectée 118532
			uint32 liste_agro_19[3] = { 85692, 145763, 70485 };					// Trait funeste 85692, Spell_Bondir_Guerrier 145763, BOND Aleatoire 70485
			uint32 liste_Buf_19[2] = { 0, 0 };									//  

			// 20    Scorpion - CREATURE_FAMILY_SCORPID
			uint32 liste_spell_A_20[2] = { 118140, 118140 };					// Pince 118140
			uint32 liste_spell_B_20[2] = { 131626, 131626 };					// Gifle de la queue 131626
			uint32 liste_agro_20[3] = { 744, 21067, 88876 };					// Poison cumulable 744, Eclair de poison 21067, Venin paralysant 88876
			uint32 liste_Buf_20[3] = { 2823, 8679, 3408 };						// Poison mortel 2823 (dmg 12s), Poison douloureux 8679 (dmg, -de soins), Poison affaiblissant 3408 (vit -70%/12s)

			// 21    Tortue - CREATURE_FAMILY_TURTLE
			uint32 liste_spell_A_21[2] = { 113687, 113687 };						// Morsure 113687 
			uint32 liste_spell_B_21[5] = { 118576, 30639,118576, 30639, 125384 };	// Kigrince 118576, Morsure carnivore 30639, Chute sur le ventre 125384 (degats + repousse)
			uint32 liste_agro_21[2] = { 80807, 50231 };								// Splendeur 80807 (cone,nuage terre), agro 50231
			uint32 liste_Buf_21[3] = { 8599, 300210, 300211 };						// Enrager 8599, armure +50% 300210, armure +100% 300211

			// 23    Diablotin (Imp) - CREATURE_FAMILY_IMP	
			uint32 liste_spell_A_23[2] = { 24187, 24187 };							// griffe 24187
			uint32 liste_spell_B_23[2] = { 3110, 3110 };							// Eclair de feu 3110
			uint32 liste_agro_23[2] = { 89808, 89808 };								// Brûle-magie 89808
			uint32 liste_Buf_23[2] = { 0, 0 };										//  

			// 24    Chauve-Souris - CREATURE_FAMILY_BAT
			uint32 liste_spell_A_24[2] = { 113687, 30639 };							// Morsure 113687, Morsure carnivore 30639
			uint32 liste_spell_B_24[4] = { 8281, 155722, 29362, 37838 };			// Explosion sonore 8281, Griffure 155722, Toucher drainant 29362, 37838
			uint32 liste_agro_24[2] = { 19196, 19196 };								// Spell_Charge_Repousse 19196 
			uint32 liste_Buf_24[2] = { 6605, 6605 };								// Hurlement terrifiant 6605 (fear/vit - 50% 4s)

			// 25    Hyene - CREATURE_FAMILY_HYENA
			uint32 liste_spell_A_25[2] = { 113687, 33745 };						// Morsure 113687, Lacerer 33745
			uint32 liste_spell_B_25[2] = { 1715, 1715 };						// Brise-genou	1715
			uint32 liste_agro_25[2] = { 0, 0 };									// 
			uint32 liste_Buf_25[2] = { 44531, 44531 };							// Célérité 44531

			// 26    Oiseau de proie - CREATURE_FAMILY_BIRD_OF_PREY
			uint32 liste_spell_A_26[2] = { 24187, 24187  };						// griffe 24187
			uint32 liste_spell_B_26[2] = { 163716, 300220 };					// Griffure de serres 163716, Griffure 300220
			uint32 liste_agro_26[2] = { 89712, 89712 };							// Griffure bondissante (bond) 89712
			uint32 liste_Buf_26[2] = { 3149, 59737 };							// Hurlement furieux 3149, vitesse 59737 (10s 100%)

			// 27    Serpent des vents - CREATURE_FAMILY_WIND_SERPENT
			uint32 liste_spell_A_27[2] = { 24187, 24187 };						// griffe 24187
			uint32 liste_spell_B_27[3] = { 9532, 9532, 12058 };					// eclair 9532, chaine d'eclair	12058
			uint32 liste_agro_27[2] = { 8056, 57994 };							// Horion de givre 8056, Cisaille de vent 57994
			uint32 liste_Buf_27[3] = { 324, 324, 32182 };						// Bouclier de foudre 324, Héroïsme 32182

			// 29 Gangregarde (Felguard) - CREATURE_FAMILY_FELGUARD
			uint32 liste_spell_A_29[2] = { 127171, 127171  };					// Frappe de la Légion 30213, , Fendoir vicieux 127171
			uint32 liste_spell_B_29[2] = { 89751, 89751 };						// Gangrorage 89751
			uint32 liste_agro_29[2] = { 89766, 30151 };							// Lancer de hache 89766,  Poursuite 30151 (charge, vit +30%/6s)
			uint32 liste_Buf_29[2] = { 0, 0 };									//

			// 30    Faucon dragon - CREATURE_FAMILY_DRAGONHAWK
			uint32 liste_spell_A_30[2] = { 29722, 29722 };						// Incinérer 29722, Brulure 2948, Bouffée d’air 128134
			uint32 liste_spell_B_30[2] = { 51219, 51219 };						// Souffle de flammes 51219
			uint32 liste_agro_30[2] = { 0, 0 };									// 
			uint32 liste_Buf_30[2] = { 79849, 29718 };							// Armure de la fournaise 79849, Armure des éléments 29718 (resistance magie 200)

			// 31    Ravageur - CREATURE_FAMILY_RAVAGER
			uint32 liste_spell_A_31[2] = { 113687, 113687 };					// Morsure 113687 
			uint32 liste_spell_B_31[4] = { 36991, 33745, 83639, 33745 };		// Pourfendre 36991, Lacerer 33745, Bain de sang 83639 (6 attaque rapide 7/lvl 3s)
			uint32 liste_agro_31[2] = { 89712, 50231 };							// Griffure bondissante 89712 (bond), agro 50231
			uint32 liste_Buf_31[4] = { 33907, 33907, 106898, 15727 };			// Epines 33907, Ruée rugissante 106898 (vit +60%), Rugissement démoralisant 15727 (Réduit PA mêlée des ennemis 10m/20s)

			// 34    Raie du Neant - CREATURE_FAMILY_NETHER_RAY
			uint32 liste_spell_A_34[2] = { 113687, 113687 };					// Morsure 113687 
			uint32 liste_spell_B_34[2] = { 12058, 12058 };						// Chaine d'eclair 12058	
			uint32 liste_agro_34[2] = { 0, 0 };									// 
			uint32 liste_Buf_34[2] = { 0, 0 };									// 

			// 35    Serpent - CREATURE_FAMILY_SERPENT	
			uint32 liste_spell_A_35[2] = { 113687, 113687 };					// Morsure 113687
			uint32 liste_spell_B_35[2] = { 12097, 88876 };						// Perce-armure 12097 (armure -75%/15s 5m), *Venin paralysant 88876 (vit & d'attaque -30%)
			uint32 liste_agro_35[2] = { 89712, 89712 };							// Griffure bondissante 89712 (bond) 
			uint32 liste_Buf_35[3] = { 2823, 8679, 3408 };						// Poison mortel 2823 (dmg 12s), Poison douloureux 8679 (dmg, -de soins), Poison affaiblissant 3408 (vit -70%/12s)

			// 37    Papillon de nuit - CREATURE_FAMILY_MOTH
			uint32 liste_spell_A_37[2] = { 24187, 24187 };						// griffe 24187
			uint32 liste_spell_B_37[2] = { 128134, 128134 };					// Bouffée d’air 128134
			uint32 liste_agro_37[2] = { 57994, 57994 };							// Cisaille de vent 57994
			uint32 liste_Buf_37[2] = { 0, 0 };									//  

			// 38    Chimere - CREATURE_FAMILY_CHIMAERA
			uint32 liste_spell_A_38[2] = { 118576, 118576 };					// Kigrince 118576
			uint32 liste_spell_B_38[2] = { 15797, 15797 };						// Souffle de foudre 15797
			uint32 liste_agro_38[2] = { 0, 0 };									// 
			uint32 liste_Buf_38[2] = { 324, 324 };								// Bouclier de foudre 324

			// 39    Diablosaure - CREATURE_FAMILY_DEVILSAUR
			uint32 liste_spell_A_39[2] = { 137504, 137504 };							// Écraser 137504 (armure -25%/1mn)
			uint32 liste_spell_B_39[5] = { 118682, 118682, 136870, 136870, 140409 };	// Taper du pied 118682, Piétinement de diablosaure 140409 (stun), Heurt de queue 136870 (repousse legement armure -10%/20s cumulable 10 fois) 
			uint32 liste_agro_39[2] = { 0, 0 };											// 
			uint32 liste_Buf_39[2] = { 138766, 15727 };									// Rugissement perçant 138766, Rugissement démoralisant 15727 (Réduit PA mêlée 10m/20s)

			// 40    Goule - CREATURE_FAMILY_GHOUL
			uint32 liste_spell_A_40[2] = { 24187, 24187 };						// griffe 24187
			uint32 liste_spell_B_40[2] = { 300220, 300220 };					// Griffure	300220
			uint32 liste_agro_40[2] = { 3436, 3436 };							// peste galopante 3436 (degat +5%,endurance -5% , 10% de contaminer un allier quand touché,30s)
			uint32 liste_Buf_40[2] = { 50366, 50366 };							// Nuée de peste 50366 (infecte les cibles proches)	

			// 42    Ver - CREATURE_FAMILY_WORM
			uint32 liste_spell_A_42[2] = { 69203, 20667 };						// Morsure vicieuse 69203, jet dacide 20667 (cône  20m,Réduit l'armure,dmg 5s/15s )
			uint32 liste_spell_B_42[2] = { 26419, 26419 };						// pluie d acide 26419 (degat nature augmenté par le nbr d'application)
			uint32 liste_agro_42[2] = { 0, 0 };									// 
			uint32 liste_Buf_42[2] = { 0, 0 };									//  

			// 43    Rhinoceros - CREATURE_FAMILY_RHINO
			uint32 liste_spell_A_43[2] = { 131172, 131172 };					// Coup de tete 131172
			uint32 liste_spell_B_43[2] = { 125384, 125384 };					// Chute sur le ventre 125384 (dmg, repousse, légèrement)
			uint32 liste_agro_43[2] = { 19196, 17230 };							// Spell_Charge_Repousse 19196, blessure infecté 17230 (degat physique auglmenté de 2/lvl+10/5mn)
			uint32 liste_Buf_43[2] = { 118682, 152571 };						// Taper du pied 118682, encorner 152571

			// 44    Guepe - CREATURE_FAMILY_WASP
			uint32 liste_spell_A_44[2] = { 37838, 37838 };						// Mutiler 85835, Sangsue 37838 (drain pv)
			uint32 liste_spell_B_44[2] = { 128425, 128425 };					// Résine corrosive 128425
			uint32 liste_agro_44[2] = { 0, 0 };									// 
			uint32 liste_Buf_44[2] = { 22863, 22863 };							// Vitesse 22863 (10s/30%)

			// 45    Chien de base - CREATURE_FAMILY_CORE_HOUND
			uint32 liste_spell_A_45[2] = { 113687, 113687 };					// Morsure 113687
			uint32 liste_spell_B_45[2] = { 1079, 1715 };						// Dechirure 1079, Brise-genou 1715
			uint32 liste_agro_45[2] = { 0, 0 };									// 
			uint32 liste_Buf_45[2] = { 44531, 44531 };							// Célérité 44531

			// 49    Elementaire d'eau - CREATURE_FAMILY_WATER_ELEMENTAL
			uint32 liste_spell_A_49[2] = { 119004, 119004 };					// Violent coup direct 119004
			uint32 liste_spell_B_49[2] = { 34425, 34425 };						// Trait d'eau 34425
			uint32 liste_agro_49[3] = { 127463, 116, 116 };						// Destruction glaciale 127463, Eclair de givre	116
			uint32 liste_Buf_49[2] = { 165743, 29718 };							// Armure de givre 165743, Armure des éléments 29718

			// 50    Renard - CREATURE_FAMILY_FOX
			uint32 liste_spell_A_50[2] = { 113687, 113687 };					// Morsure 113687
			uint32 liste_spell_B_50[2] = { 1715, 1715 };						// Brise-genou 1715
			uint32 liste_agro_50[2] = { 85691, 85691 };							// Hurlement perçant 85691 (stun 4s)
			uint32 liste_Buf_50[3] = { 85690, 22863, 1784 };					// Ruse du renard 85690 (esquive +25%/5s), Vitesse 22863 (10s/30%), Camouflage 1784

			// 51    Singe - CREATURE_FAMILY_MONKEY
			uint32 liste_spell_A_51[2] = { 24187, 24187 };						// griffe 24187
			uint32 liste_spell_B_51[2] = { 127412, 127412 };					// Lancer de peau de banane 127412
			uint32 liste_agro_51[3] = { 46483, 3436, 15848 };					// Maladie volatile 46483, peste galopante 3436, urticaire 15848
			uint32 liste_Buf_51[2] = { 70485, 70485 };							// Bond_Aleatoire 70485

			// 52    Chien - CREATURE_FAMILY_DOG
			uint32 liste_spell_A_52[2] = { 113687, 113687 };					// Morsure 113687
			uint32 liste_spell_B_52[2] = { 1079, 1715 };						// Dechirure 1079, Brise-genou 1715
			uint32 liste_agro_52[2] = { 0, 0 };									// 
			uint32 liste_Buf_52[2] = { 44531, 44531 };							// Célérité 44531

			// 55    Araignee de schiste - CREATURE_FAMILY_SHALE_SPIDER
			uint32 liste_spell_A_55[2] = { 118140, 118140 };					// Pince 118140
			uint32 liste_spell_B_55[2] = { 12097, 3604 };						// Perce-armure 12097 (armure -75%/15s 5m), Déchirure du tendon 3604 (5s)
			uint32 liste_agro_55[2] = { 20667, 89712  };						// jet dacide 20667 (cône  20m,Réduit l'armure,dmg 5s/15s ), Griffure bondissante 89712
			uint32 liste_Buf_55[2] = { 22812, 43547 };							// Ecorce 22812, Débiter (Vit d’attaque +50%/40s) 43547

			// 56    Zombie - CREATURE_FAMILY_ZOMBIE	
			uint32 liste_spell_A_56[2] = { 24187, 113687 };						// griffe 24187, Morsure 113687
			uint32 liste_spell_B_56[2] = { 58514, 58514  };						// Odeur de banane pourrie 58514
			uint32 liste_agro_56[4] = { 3436, 15848, 46483, 50303 };			// peste galopante 3436, urticaire 15848, Maladie volatile 46483, Grippe porcine 50303
			uint32 liste_Buf_56[2] = { 0, 0 };									//  

			// 68    Hydre - CREATURE_FAMILY_HYDRA
			uint32 liste_spell_A_68[2] = { 113687, 113687 };					// Morsure 113687
			uint32 liste_spell_B_68[3] = { 131626, 127987, 50169 };				// Gifle de la queue 131626, Morsure hémorragique 127987, Grand coup de nageoire 50169
			uint32 liste_agro_68[2] = { 79607, 140476 };						// poison 79607 (Tir catapulte), Vil crachat 140476
			uint32 liste_Buf_68[2] = { 33907, 33907 };							// Spell_Epines 33907

			// 100   Diablotin gangrene - CREATURE_FAMILY_FELIMP
			uint32 liste_spell_A_100[2] = { 24187, 24187 };						// griffe 24187
			uint32 liste_spell_B_100[2] = { 26419, 26419 };						// pluie d acide 26419
			uint32 liste_agro_100[2] = { 113967, 35328 };						// Barbelés d'épines 113967, Sang diapré 35328 (Impossible d'utiliser Camouflage)
			uint32 liste_Buf_100[2] = { 79934, 79934 };							// Armure démoniaque 79934

			// 104   Garde-courroux - CREATURE_FAMILY_WRATHGUARD
			uint32 liste_spell_A_104[2] = { 77558, 77558 };						// Frappe sanglante 77558
			uint32 liste_spell_B_104[2] = { 127171, 118532  };					// Fendoir vicieux 127171, Entaille infectée 118532
			uint32 liste_agro_104[2] = { 32323, 145763 };						// charge 32323 (sonne 2s,8-25m ), Spell_Bondir_Guerrier 145763
			uint32 liste_Buf_104[2] = { 0, 0 };									//  

			// 108   Infernal - CREATURE_FAMILY_INFERNAL
			uint32 liste_spell_A_108[2] = { 82616, 82616 };						// Frappe focalisée 82616, Violent coup direct 119004
			uint32 liste_spell_B_108[2] = { 348, 6253 };						// Immolation 348, revers 6253 (stun 2s)	
			uint32 liste_agro_108[3] = { 6353, 105174, 177288 };				// Feu de l'âme 6353 , Main de Gul’dan 105174, Onde de chaos 177288
			uint32 liste_Buf_108[4] = { 104773, 109773, 79849, 79849 };			// Résolution interminable 104773, Sombre intention 109773, Armure De La Fournaise 79849

			// 116   elementaire de feu - CREATURE_FAMILY_FIREELEMENTAL
			uint32 liste_spell_A_116[2] = { 119004, 119004 };					// Violent coup direct 119004
			uint32 liste_spell_B_116[2] = { 9053, 9053 };						// Boule de feu 9053
			uint32 liste_agro_116[2] = { 348, 348 };							// Immolation 348
			uint32 liste_Buf_116[2] = { 79849, 79849 };							// Armure de la fournaise 79849

			// 117   elementaire de terre - CREATURE_FAMILY_EARTHELEMENTAL
			uint32 liste_spell_A_117[2] = { 119004, 119004 };					// Violent coup direct 119004
			uint32 liste_spell_B_117[3] = { 80182, 80807, 79872 };				// Uppercut 80182, Splendeur 80807 (cone), Onde de choc 79872    
			uint32 liste_agro_117[2] = { 138976, 138976 };						// Bourrasque imminente 138976
			uint32 liste_Buf_117[2] = { 974, 974 };								// bouclier de terre 974

			// 130   Basilic - CREATURE_FAMILY_BASILISK
			uint32 liste_spell_A_130[2] = { 113687, 113687 };					// Morsure 113687
			uint32 liste_spell_B_130[4] = { 138765, 136870, 127987, 131626 };	// Écraser 138765 armure -10%/30s, Heurt de queue 136870 (repousse legement, armure -10%/20s ,cumulable 10 fois), Morsure hémorragique 127987, Gifle de la queue 131626
			uint32 liste_agro_130[2] = { 35313, 12097 };						// Regard hypnotique 35313 (stun 6s, 5-30m), Perce-armure 12097 (armure -75%/15s 5m)
			uint32 liste_Buf_130[2] = { 0, 0 };									// 




			uint32 Start_Agro = 0;

			void InitializeAI()
			{
				// --- Spécificitée par family ---
				Random = urand(1, 3);
				if (Random == 1)
				{
					if (Crfamily == 20 || Crfamily == 42) { Senterre(); }
				}
				// -------------------------------
			}
			void Init_AI()
			{
				Spell_B_Non_Cumulable = 0;

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

					// Forcer la famille de mob par creature_template->pickpocketloot
					if (ForceFamily != 0) { Crfamily = ForceFamily; }

					// Tirage aléatoire des spells ----------------------------------------------------------------------------------------------------------------
					if (me->m_spells[0] == 0)
					{

					switch (Crfamily)
					{
					case 1:		// Loup
						me->SetMeleeDamageSchool(SpellSchools(0));															// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_1[urand(0, 2)];
						Spell_B = liste_spell_B_1[urand(0, 3)];
						Spell_B_Non_Cumulable = 1;
						Spell_agro = liste_agro_1[urand(0, 1)];
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_1[urand(0, 5)];
						Spell_Heal = 0;	
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A;
						Cooldown_SpellB = 2500;
						Cooldown_SpellB_defaut = Base_Cooldown_Cast_B+4000;
						Cooldown_SpellB_rapide = 0;													
						Cooldown_SpellB_rapide_defaut = 0;											
						Cooldown_Spell_Heal_defaut = 30000;
						Cooldown_Principal_A = 1000;																		// Temp de test pour aller sur la cible
						Cooldown_Principal_A_Defaut = 1000;
						Cooldown_Principal_B = 6000;																		// Temp de test pour mouvement (s'eloigner, passer dans le dos,...)
						Cooldown_Principal_B_Defaut = 8000 + ((urand(0, 4) * 500));
						ResteADistance = 5;
						Spell_Trop_Loin = 0;
						Cooldown_Trop_Loin = 6000;																			// Temp de test ci la cible est trop loin (pour charge etc)
						Cooldown_Trop_Loin_Defaut = 15000;
						break;
					case 2:		// Felin
						me->SetMeleeDamageSchool(SpellSchools(0));															// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_2[urand(0, 1)];
						Spell_B = liste_spell_B_2[urand(0, 3)];
						Spell_B_Non_Cumulable = 1;
						Spell_agro = liste_agro_2[urand(0, 1)];
						Spell_respawn_evade = Spell_Invisible;
						Buf_A = liste_Buf_2[urand(0, 2)];
						Spell_Heal = Spell_Instincts_de_survie;
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A;
						Cooldown_SpellB = 2500;
						Cooldown_SpellB_defaut = Base_Cooldown_Cast_B;
						Cooldown_SpellB_rapide = 0;													
						Cooldown_SpellB_rapide_defaut = 0;											
						Cooldown_Spell_Heal_defaut = 60000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1000;
						Cooldown_Principal_B = 6000;
						Cooldown_Principal_B_Defaut = 6000 + ((urand(0, 4) * 500));
						ResteADistance = 5;

						AI_Random = urand(1, 2);
						if (AI_Random == 1) {
							Spell_Trop_Loin = Spell_Vitesse_4s;
							Cooldown_Trop_Loin = 6000;
							Cooldown_Trop_Loin_Defaut = 10000;
						}
						if (AI_Random == 2) {
							Spell_Trop_Loin = Spell_Griffure_bondissante; 
							Cooldown_Trop_Loin = 6000;
							Cooldown_Trop_Loin_Defaut = 15000;
							Cooldown_Trop_Loin = urand(10000, 20000);
						}

						break;
					case 3:		// Araignee - CREATURE_FAMILY_SPIDER
						me->SetMeleeDamageSchool(SpellSchools(3));															// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_3[urand(0, 1)];
						Spell_B = liste_spell_B_3[urand(0, 1)];
						Spell_B_Non_Cumulable = 1;
						Spell_agro = liste_agro_3[urand(0, 3)];
						Spell_respawn_evade = 0;
						Buf_A = 0;
						Spell_Heal = 0;
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A;
						Cooldown_SpellB = 3000;
						Cooldown_SpellB_defaut = Base_Cooldown_Cast_B-2000;
						Cooldown_SpellB_rapide = Base_Cooldown_Cast_B-4000;													// Cadence de tir SpellB rapide pour Mouvement_Cast_Puis_Contact
						Cooldown_SpellB_rapide_defaut = Cooldown_SpellB_defaut;												// Cadence de tir SpellB normale pour Mouvement_Cast_Puis_Contact
						Cooldown_Spell_Heal_defaut = 60000;
						Cooldown_Principal_A = 2000;
						Cooldown_Principal_A_Defaut = 2000;
						Cooldown_Principal_B = 1000;
						Cooldown_Principal_B_Defaut = 1000;
						ResteADistance = 10;
						Spell_Trop_Loin = Spell_Toile_Araignee;
						Cooldown_Trop_Loin = 8000;
						Cooldown_Trop_Loin_Defaut = urand(8000,15000);
						break;
					case 4:		// Ours - CREATURE_FAMILY_BEAR 
						me->SetMeleeDamageSchool(SpellSchools(0));															// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_4[urand(0, 2)];
						Spell_B = liste_spell_B_4[urand(0, 2)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_4[urand(0, 1)];
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_4[urand(0, 1)];
						Spell_Heal = Spell_Instincts_de_survie; ;
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A;
						Cooldown_SpellB = 3000;
						Cooldown_SpellB_defaut = Base_Cooldown_Cast_B+2000;
						Cooldown_SpellB_rapide = 0;														
						Cooldown_SpellB_rapide_defaut = 0;												
						Cooldown_Spell_Heal_defaut = 60000;
						Cooldown_Principal_A = 1500;
						Cooldown_Principal_A_Defaut = 1500;
						Cooldown_Principal_B = 8000;
						Cooldown_Principal_B_Defaut = 8000 + ((urand(0, 4) * 500));
						ResteADistance = 5;
						Spell_Trop_Loin = Spell_Charge_Stun;																// Charge farouche 87187 (8-25 m ours)
						Cooldown_Trop_Loin = 10000;
						Cooldown_Trop_Loin_Defaut = urand(8000,12000);
						break;
					case 5:		// Sanglier - CREATURE_FAMILY_BOAR 
						me->SetMeleeDamageSchool(SpellSchools(0));															// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_5[urand(0, 2)];
						Spell_B = liste_spell_B_5[urand(0, 4)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_5[urand(0, 1)];
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_5[urand(0, 2)];
						Spell_Heal = 0;
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A;
						Cooldown_SpellB = 2500;
						Cooldown_SpellB_defaut = Base_Cooldown_Cast_B+2000;
						Cooldown_SpellB_rapide = 0;													
						Cooldown_SpellB_rapide_defaut = 0;											
						Cooldown_Spell_Heal_defaut = 30000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1000;
						Cooldown_Principal_B = 6000;
						Cooldown_Principal_B_Defaut = 8000 + ((urand(0, 4) * 500));
						ResteADistance = 5;
						Spell_Trop_Loin = Spell_Charge;																		//
						Cooldown_Trop_Loin = 6000;
						Cooldown_Trop_Loin_Defaut = urand(7000,10000);
						break;
					case 6:		// Crocodile - CREATURE_FAMILY_CROCOLISK
						me->SetMeleeDamageSchool(SpellSchools(0));															// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_6[urand(0, 1)];
						Spell_B = liste_spell_B_6[urand(0, 3)];
						Spell_B_Non_Cumulable = 1;
						Spell_agro = liste_agro_6[urand(0, 1)];
						Spell_respawn_evade = Spell_Ecorce;
						Buf_A = liste_spell_A_6[urand(0, 1)];
						Spell_Heal = 0;
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A;
						Cooldown_SpellB = 3000;
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B, Base_Cooldown_Cast_B+3000);
						Cooldown_SpellB_rapide = Base_Cooldown_Cast_B-3000;													// Cadence de tir SpellB rapide pour Mouvement_Cast_Puis_Contact
						Cooldown_SpellB_rapide_defaut = Cooldown_SpellB_defaut;												// Cadence de tir SpellB normale pour Mouvement_Cast_Puis_Contact
						Cooldown_Spell_Heal_defaut = 60000;
						Cooldown_Principal_A = 2000;
						Cooldown_Principal_A_Defaut = 2000;
						Cooldown_Principal_B = urand(5000, 7000);
						Cooldown_Principal_B_Defaut = urand(7000, 9000);
						ResteADistance = 5;
						Spell_Trop_Loin = 0;
						Cooldown_Trop_Loin = urand(0, 4000);
						Cooldown_Trop_Loin_Defaut = urand(8000, 12000);
						AI_Random = urand(1, 3);
						break;
					case 7:		// Oiseau charogniar - CREATURE_FAMILY_CARRION_BIRD
						me->SetMeleeDamageSchool(SpellSchools(0));															// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_7[urand(0, 2)];
						Spell_B = liste_spell_B_7[urand(0, 2)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_7[urand(0, 1)];
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_7[urand(0, 4)];
						Spell_Heal = 0;
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A;
						Cooldown_SpellB = 2500;
						Cooldown_SpellB_defaut = Base_Cooldown_Cast_B+2000;
						Cooldown_SpellB_rapide = 0;
						Cooldown_SpellB_rapide_defaut = 0;
						Cooldown_Spell_Heal_defaut = 30000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1000;
						Cooldown_Principal_B = 8000;
						Cooldown_Principal_B_Defaut = 7000 + ((urand(0, 4) * 500));
						ResteADistance = 5;
						Spell_Trop_Loin = 0;									//
						Cooldown_Trop_Loin = 500;
						Cooldown_Trop_Loin_Defaut = urand(6000, 8000);
						break;
					case 8:		// Crabe - CREATURE_FAMILY_CRAB
						me->SetMeleeDamageSchool(SpellSchools(0));															// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_8[urand(0, 1)];
						Spell_B = liste_spell_B_8[urand(0, 2)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_8[urand(0, 1)];
						Spell_respawn_evade = Spell_Ecorce;
						Buf_A = liste_Buf_8[urand(0, 1)];
						Spell_Heal = 0;
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A;
						Cooldown_SpellB = 2500;
						Cooldown_SpellB_defaut = Base_Cooldown_Cast_B+2000;
						Cooldown_SpellB_rapide = 0;
						Cooldown_SpellB_rapide_defaut = 0;
						Cooldown_Spell_Heal_defaut = 30000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1000;
						Cooldown_Principal_B = 8000;
						Cooldown_Principal_B_Defaut = 5000 + ((urand(0, 8) * 500));
						ResteADistance = 5;
						Spell_Trop_Loin = 0;									//
						Cooldown_Trop_Loin = 1000;
						Cooldown_Trop_Loin_Defaut = urand(6000, 8000);
						break;
					case 9:		// Gorille - CREATURE_FAMILY_GORILLA
						me->SetMeleeDamageSchool(SpellSchools(0));															// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_9[urand(0, 3)];
						Spell_B = liste_spell_B_9[urand(0, 4)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_9[urand(0, 2)];
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_9[urand(0, 2)];
						Spell_Heal = 0;
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A;
						Cooldown_SpellB = 2500;
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B, Base_Cooldown_Cast_B+2000);
						Cooldown_SpellB_rapide = 0;
						Cooldown_SpellB_rapide_defaut = 0;
						Cooldown_Spell_Heal_defaut = 30000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1500;
						Cooldown_Principal_B = 6000;
						Cooldown_Principal_B_Defaut = 5000 + ((urand(0, 6) * 500));
						ResteADistance = 5;
						Spell_Trop_Loin = Spell_Bondir_Guerrier;									//
						Cooldown_Trop_Loin = 6000;
						Cooldown_Trop_Loin_Defaut = urand(8000, 10000);
						break;
					case 11:	// Raptor - CREATURE_FAMILY_RAPTOR
						me->SetMeleeDamageSchool(SpellSchools(0));															// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_11[urand(0, 1)];
						Spell_B = liste_spell_B_11[urand(0, 3)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_11[urand(0, 2)];
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_11[urand(0, 2)];
						Spell_Heal = 0;
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A;
						Cooldown_SpellB = 2500;
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B, Base_Cooldown_Cast_B+2000);
						Cooldown_SpellB_rapide = 0;
						Cooldown_SpellB_rapide_defaut = 0;
						Cooldown_Spell_Heal_defaut = 30000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1500;
						Cooldown_Principal_B = 6000;
						Cooldown_Principal_B_Defaut = 5000 + ((urand(0, 6) * 500));
						ResteADistance = 5;
						Spell_Trop_Loin = Cri_Incapacitant;																// Cri incapacitant 18328 (vit -60%)	
						Cooldown_Trop_Loin = 6000;
						Cooldown_Trop_Loin_Defaut = urand(8000, 12000);
						break;
					case 16:	// Marcheur du Vide (Voidwalker) - CREATURE_FAMILY_VOIDWALKER
						me->SetMeleeDamageSchool(SpellSchools(5));														// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_16[urand(0, 1)];
						Spell_B = liste_spell_B_16[urand(0, 1)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_16[urand(0, 1)];
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_16[urand(0, 1)];
						Spell_Heal = 17767;																				// Rempart de l’ombre 17767 (PV +65%/20s), Bouclier de l’ombre 115232 (dmg -60%/30s)
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A+250;
						Cooldown_SpellB = 2500;
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B-2000, Base_Cooldown_Cast_B-1000);
						Cooldown_SpellB_rapide = 0;
						Cooldown_SpellB_rapide_defaut = 0;
						Cooldown_Spell_Heal_defaut = 120000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1000;
						Cooldown_Principal_B = 6000;
						Cooldown_Principal_B_Defaut = 5000 + ((urand(0, 6) * 500));
						ResteADistance = 5;
						Spell_Trop_Loin = 0;																			// 	
						Cooldown_Trop_Loin = 6000;
						Cooldown_Trop_Loin_Defaut = urand(8000, 12000);
						break;
					case  17:	// Succube - CREATURE_FAMILY_SUCCUBUS
						me->SetMeleeDamageSchool(SpellSchools(5));														// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_17[urand(0, 1)];
						Spell_B = liste_spell_B_17[urand(0, 1)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_17[urand(0, 1)];
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_17[urand(0, 1)];
						Spell_Heal = 0;																					// 
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A-1500;
						Cooldown_SpellB = urand(2500, 5000);
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B+3000, Base_Cooldown_Cast_B+9000);
						Cooldown_SpellB_rapide = 0;
						Cooldown_SpellB_rapide_defaut = 0;
						Cooldown_Spell_Heal_defaut = 120000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1000;
						Cooldown_Principal_B = 6000;
						Cooldown_Principal_B_Defaut = 5000 + ((urand(0, 4) * 500));
						ResteADistance = 5;
						Spell_Trop_Loin = 0;																			// 	
						Cooldown_Trop_Loin = 6000;
						Cooldown_Trop_Loin_Defaut = urand(8000, 12000);
						break;
					case 19:	// Garde funeste (Doomguard) - CREATURE_FAMILY_DOOMGUARD
						me->SetMeleeDamageSchool(SpellSchools(5));														// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_19[urand(0, 1)];
						Spell_B = liste_spell_B_19[urand(0, 1)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_19[urand(0, 2)];
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_19[urand(0, 1)];
						Spell_Heal = 0;																					// 
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A;
						Cooldown_SpellB = urand(2500, 3500);
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B-1000, Base_Cooldown_Cast_B+1000);
						Cooldown_SpellB_rapide = 0;
						Cooldown_SpellB_rapide_defaut = 0;
						Cooldown_Spell_Heal_defaut = 120000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1000;
						Cooldown_Principal_B = 6000;
						Cooldown_Principal_B_Defaut = 8000 + ((urand(0, 8) * 500));
						ResteADistance = 5;
						Spell_Trop_Loin = Spell_Bondir_Guerrier;														// 
						Cooldown_Trop_Loin = urand(1000,4000);
						Cooldown_Trop_Loin_Defaut = 5000;
						break;
					case 20:	// Scorpion - CREATURE_FAMILY_SCORPID
						me->SetMeleeDamageSchool(SpellSchools(3));														// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_20[urand(0, 1)];
						Spell_B = liste_spell_B_20[urand(0, 1)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_20[urand(0, 2)];
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_20[urand(0, 2)];
						Spell_Heal = 0;																					// 
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A;
						Cooldown_SpellB = urand(2500, 3500);
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B-1000, Base_Cooldown_Cast_B+1000);
						Cooldown_SpellB_rapide = 0;
						Cooldown_SpellB_rapide_defaut = 0;
						Cooldown_Spell_Heal_defaut = 120000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1000;
						Cooldown_Principal_B = urand(2000, 5000);
						Cooldown_Principal_B_Defaut = 8000 + ((urand(0, 4) * 500));
						ResteADistance = 5;
						Spell_Trop_Loin = 0;																			// 
						Cooldown_Trop_Loin = urand(1000, 4000);
						Cooldown_Trop_Loin_Defaut = 5000;
						break;
					case 21:	// Tortue - CREATURE_FAMILY_TURTLE
						me->SetMeleeDamageSchool(SpellSchools(0));														// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_21[urand(0, 1)];
						Spell_B = liste_spell_B_21[urand(0, 2)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_21[urand(0, 1)];
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_21[urand(0, 2)];
						Spell_Heal = 0;																					// 
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A;
						Cooldown_SpellB = urand(2500, 3500);
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B, Base_Cooldown_Cast_B+2000);
						Cooldown_SpellB_rapide = 0;
						Cooldown_SpellB_rapide_defaut = 0;
						Cooldown_Spell_Heal_defaut = 120000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 2000;
						Cooldown_Principal_B = urand(2000, 5000);
						Cooldown_Principal_B_Defaut = 8000 + ((urand(0, 4) * 500));
						ResteADistance = 5;
						Spell_Trop_Loin = 0;																			// 
						Cooldown_Trop_Loin = urand(1000, 4000);
						Cooldown_Trop_Loin_Defaut = 5000;
						break;
					case 23:	// Diablotin (Imp) - CREATURE_FAMILY_IMP	
						me->SetMeleeDamageSchool(SpellSchools(2));														// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_23[urand(0, 1)];
						Spell_B = liste_spell_B_23[urand(0, 1)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_23[urand(0, 1)];
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_23[urand(0, 1)];
						Spell_Heal = 0;																// 
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = urand(Base_Cooldown_Cast_A-500, Base_Cooldown_Cast_A-250);
						Cooldown_SpellB = 2500;
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B-3000, Base_Cooldown_Cast_B-2500);
						Cooldown_SpellB_rapide = Base_Cooldown_Cast_B-4000;												// Cadence de tir SpellB rapide pour Mouvement_Cast_Puis_Contact
						Cooldown_SpellB_rapide_defaut = Cooldown_SpellB_defaut;											// Cadence de tir SpellB normale pour Mouvement_Cast_Puis_Contact
						Cooldown_Spell_Heal_defaut = 120000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 2000;
						Cooldown_Principal_B = 1000;
						Cooldown_Principal_B_Defaut = 1000;
						ResteADistance = 10;
						Spell_Trop_Loin = 0;																			// 
						Cooldown_Trop_Loin = urand(1000, 4000);
						Cooldown_Trop_Loin_Defaut = 5000;
						break;
					case 24:	// Chauve-souris - CREATURE_FAMILY_BAT
						me->SetMeleeDamageSchool(SpellSchools(3));														// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_24[urand(0, 1)];
						Spell_B = liste_spell_B_24[urand(0, 3)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_24[urand(0, 1)];
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_24[urand(0, 1)];
						Spell_Heal = 0;																					// 
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A;
						Cooldown_SpellB = 2500;
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B, Base_Cooldown_Cast_B+2000);
						Cooldown_SpellB_rapide = 0;
						Cooldown_SpellB_rapide_defaut = 0;
						Cooldown_Spell_Heal_defaut = 120000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1000;
						Cooldown_Principal_B = 6000;
						Cooldown_Principal_B_Defaut = urand(6000, 8000);
						ResteADistance = 5;
						Spell_Trop_Loin = 0;																			// 
						Cooldown_Trop_Loin = urand(7000, 12000);
						Cooldown_Trop_Loin_Defaut = urand(8000, 12000);
						break;
					case 25:	// Hyene - CREATURE_FAMILY_HYENA
						me->SetMeleeDamageSchool(SpellSchools(0));														// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_25[urand(0, 1)];
						Spell_B = liste_spell_B_25[urand(0, 1)];
						Spell_B_Non_Cumulable = 1;
						Spell_agro = liste_agro_25[urand(0, 1)];
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_25[urand(0, 1)];
						Spell_Heal = 0;																					// 
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A;
						Cooldown_SpellB = urand(2500,8000);
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B+6000, Base_Cooldown_Cast_B+12000);
						Cooldown_SpellB_rapide = 0;
						Cooldown_SpellB_rapide_defaut = 0;
						Cooldown_Spell_Heal_defaut = 120000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1500;
						Cooldown_Principal_B = 5000;
						Cooldown_Principal_B_Defaut = urand(4000, 6000);
						ResteADistance = 5;
						Spell_Trop_Loin = 0;																			// 
						Cooldown_Trop_Loin = urand(7000, 12000);
						Cooldown_Trop_Loin_Defaut = urand(8000, 12000);
						break;
					case 26:	// Oiseau de proie - CREATURE_FAMILY_BIRD_OF_PREY
						me->SetMeleeDamageSchool(SpellSchools(0));														// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_26[urand(0, 1)];
						Spell_B = liste_spell_B_26[urand(0, 1)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_26[urand(0, 1)];
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_26[urand(0, 1)];
						Spell_Heal = 0;																					// 
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A;
						Cooldown_SpellB = urand(2500, 8000);
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B, Base_Cooldown_Cast_B+2000);
						Cooldown_SpellB_rapide = 0;
						Cooldown_SpellB_rapide_defaut = 0;
						Cooldown_Spell_Heal_defaut = 120000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1500;
						Cooldown_Principal_B = 5000;
						Cooldown_Principal_B_Defaut = urand(4000, 8000);
						ResteADistance = 5;
						Spell_Trop_Loin = Spell_Griffure_bondissante;													// Griffure bondissante (bond) 89712
						Cooldown_Trop_Loin = urand(3000, 8000);
						Cooldown_Trop_Loin_Defaut = urand(8000, 15000);
						break;
					case 27:	// Serpent des vents - CREATURE_FAMILY_WIND_SERPENT
						me->SetMeleeDamageSchool(SpellSchools(6));														// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_27[urand(0, 1)];
						Spell_B = liste_spell_B_27[urand(0, 2)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_27[urand(0, 1)];
						Spell_respawn_evade = Spell_Bouclier_De_Foudre;
						Buf_A = liste_Buf_27[urand(0, 2)];
						Spell_Heal = 974;																				// bouclier de terre 974
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A-250;
						Cooldown_SpellB = Base_Cooldown_Cast_B-4000;
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B-1000, Base_Cooldown_Cast_B);
						Cooldown_SpellB_rapide = urand(Base_Cooldown_Cast_B-3000, Base_Cooldown_Cast_B-2000);
						Cooldown_SpellB_rapide_defaut = Cooldown_SpellB_defaut;
						Cooldown_Spell_Heal_defaut = 60000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1000;
						Cooldown_Principal_B = 1000;
						Cooldown_Principal_B_Defaut =1000;
						ResteADistance = urand(15,20);
						Spell_Trop_Loin = 0;													
						Cooldown_Trop_Loin = urand(3000, 8000);
						Cooldown_Trop_Loin_Defaut = urand(8000, 15000);
						break;
					case 29:	// Gangregarde (Felguard) - CREATURE_FAMILY_FELGUARD
						me->SetMeleeDamageSchool(SpellSchools(0));														// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_29[urand(0, 1)];
						Spell_B = liste_spell_B_29[urand(0, 1)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_29[urand(0, 1)];
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_29[urand(0, 1)];
						Spell_Heal = 0;																					// 
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A;
						Cooldown_SpellB = 2500;
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B-2000, Base_Cooldown_Cast_B);
						Cooldown_SpellB_rapide = 0;
						Cooldown_SpellB_rapide_defaut = 0;
						Cooldown_Spell_Heal_defaut = 120000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1000;
						Cooldown_Principal_B = 6000;
						Cooldown_Principal_B_Defaut = 8000 + ((urand(0, 8) * 500));
						ResteADistance = 5;
						Spell_Trop_Loin = Spell_Poursuite;																// 
						Cooldown_Trop_Loin = urand(7000, 12000);
						Cooldown_Trop_Loin_Defaut = urand(8000,12000);

						//me->SetVirtualItem(0, 12784);																	// Equipé d'une hache
						me->LoadEquipment(1, true);																		// creature_equip_template 1

						break;
					case 30:	// Faucon dragon - CREATURE_FAMILY_DRAGONHAWK
						me->SetMeleeDamageSchool(SpellSchools(0));														// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_30[urand(0, 1)];
						Spell_B = liste_spell_B_30[urand(0, 1)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_30[urand(0, 1)];
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_30[urand(0, 1)];
						Spell_Heal = 0;
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = urand(Base_Cooldown_Cast_A, Base_Cooldown_Cast_A+500);
						Cooldown_SpellB = urand(5000, 7000);
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B+2000, Base_Cooldown_Cast_B+6000);
						Cooldown_SpellB_rapide = 0;
						Cooldown_SpellB_rapide_defaut = Cooldown_SpellB_defaut;
						Cooldown_Spell_Heal_defaut = 60000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1000;
						Cooldown_Principal_B = 3000;
						Cooldown_Principal_B_Defaut = 3000;
						ResteADistance = urand(10, 15);
						Spell_Trop_Loin = 0;
						Cooldown_Trop_Loin = urand(3000, 8000);
						Cooldown_Trop_Loin_Defaut = urand(8000, 15000);
						break;
					case 31:	// Ravageur - CREATURE_FAMILY_RAVAGER
						me->SetMeleeDamageSchool(SpellSchools(0));														// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_31[urand(0, 1)];
						Spell_B = liste_spell_B_31[urand(0, 3)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_31[urand(0, 1)];
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_31[urand(0, 3)];
						Spell_Heal = 0;
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A;
						Cooldown_SpellB = urand(Base_Cooldown_Cast_B, 6000);
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B+2000, Base_Cooldown_Cast_B+6000);
						Cooldown_SpellB_rapide = 0;
						Cooldown_SpellB_rapide_defaut = Cooldown_SpellB_defaut;
						Cooldown_Spell_Heal_defaut = 60000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1000;
						Cooldown_Principal_B = urand(3000,5000);
						Cooldown_Principal_B_Defaut = urand(5000, 9000);
						ResteADistance = 5;

						AI_Random = urand(1, 2);
						if (AI_Random == 1) { Spell_Trop_Loin = Spell_Poursuite; }										//
						if (AI_Random == 2) { Spell_Trop_Loin = Spell_Griffure_bondissante; }							//

						Cooldown_Trop_Loin = urand(5000, 8000);
						Cooldown_Trop_Loin_Defaut = urand(10000, 15000);
						AI_Random = urand(1, 4);
						break;
					case 34:	// Raie du Neant - CREATURE_FAMILY_NETHER_RAY
						me->SetMeleeDamageSchool(SpellSchools(6));														// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_34[urand(0, 1)];
						Spell_B = liste_spell_B_34[urand(0, 1)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_34[urand(0, 1)];
						Spell_respawn_evade = Spell_Bouclier_De_Foudre;
						Buf_A = liste_Buf_34[urand(0, 1)];
						Spell_Heal = 0;
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = urand(Base_Cooldown_Cast_A, Base_Cooldown_Cast_A+500);
						Cooldown_SpellB = 2000;
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B-1500, Base_Cooldown_Cast_B-500);
						Cooldown_SpellB_rapide = urand(Base_Cooldown_Cast_B-3000, Base_Cooldown_Cast_B-3500);
						Cooldown_SpellB_rapide_defaut = Cooldown_SpellB_defaut;
						Cooldown_Spell_Heal_defaut = 60000;
						Cooldown_Principal_A = 1500;
						Cooldown_Principal_A_Defaut = 1500;
						Cooldown_Principal_B = 1000;
						Cooldown_Principal_B_Defaut = 1500;
						ResteADistance = urand(10, 15);
						Spell_Trop_Loin = 0;
						Cooldown_Trop_Loin = urand(3000, 8000);
						Cooldown_Trop_Loin_Defaut = urand(8000, 15000);
						AI_Random = urand(1, 2);

						if (AI_Random == 2)
						{
							Cooldown_Principal_B_Defaut = 4500;
							ResteADistance = 20;
						}
						break;
					case 35:	// Serpent - CREATURE_FAMILY_SERPENT
						me->SetMeleeDamageSchool(SpellSchools(3));										// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_35[urand(0, 1)];
						Spell_B = liste_spell_B_35[urand(0, 1)];
						Spell_B_Non_Cumulable = 1;
						Spell_agro = liste_agro_35[urand(0, 1)];
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_35[urand(0, 2)];
						Spell_Heal = 0;
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = urand(Base_Cooldown_Cast_A, Base_Cooldown_Cast_A+500);
						Cooldown_SpellB = 2000;
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B-1500, Base_Cooldown_Cast_B-500);
						Cooldown_SpellB_rapide = 0;
						Cooldown_SpellB_rapide_defaut = Cooldown_SpellB_defaut;
						Cooldown_Spell_Heal_defaut = 60000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1000;
						Cooldown_Principal_B = 5000;
						Cooldown_Principal_B_Defaut = 8000;
						ResteADistance = 5;
						Spell_Trop_Loin = 0;
						Cooldown_Trop_Loin = urand(3000, 8000);
						Cooldown_Trop_Loin_Defaut = urand(8000, 15000);
						break;
					case 37:	// Papillon de nuit - CREATURE_FAMILY_MOTH
						me->SetMeleeDamageSchool(SpellSchools(3));										// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_37[urand(0, 1)];
						Spell_B = liste_spell_B_37[urand(0, 1)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_37[urand(0, 1)];
						Spell_respawn_evade = Spell_Bouclier_De_Terre;
						Buf_A = liste_Buf_37[urand(0, 1)];
						Spell_Heal = 0;
						Cooldown_SpellA = 500;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A;
						Cooldown_SpellB = 1000;
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B-2000, Base_Cooldown_Cast_B-1000);
						Cooldown_SpellB_rapide = 0;
						Cooldown_SpellB_rapide_defaut = Cooldown_SpellB_defaut;
						Cooldown_Spell_Heal_defaut = 60000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1000;
						Cooldown_Principal_B = 2000;
						Cooldown_Principal_B_Defaut = 6000;
						ResteADistance = urand(12, 15);
						Spell_Trop_Loin = 0;
						Cooldown_Trop_Loin = urand(3000, 8000);
						Cooldown_Trop_Loin_Defaut = urand(8000, 15000);
						break;
					case 38:	// Chimere - CREATURE_FAMILY_CHIMAERA
						me->SetMeleeDamageSchool(SpellSchools(0));										// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_38[urand(0, 1)];
						Spell_B = liste_spell_B_38[urand(0, 1)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_38[urand(0, 1)];
						Spell_respawn_evade = Spell_Epines;
						Buf_A = liste_Buf_38[urand(0, 1)];
						Spell_Heal = 0;
						Cooldown_SpellA = 500;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A+500;
						Cooldown_SpellB = 1000;
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B-1000, Base_Cooldown_Cast_B);
						Cooldown_SpellB_rapide = urand(Base_Cooldown_Cast_B-2250, Base_Cooldown_Cast_B-1250);
						Cooldown_SpellB_rapide_defaut = Cooldown_SpellB_defaut;
						Cooldown_Spell_Heal_defaut = 60000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1000;
						Cooldown_Principal_B = 1000;
						Cooldown_Principal_B_Defaut = 1000;
						ResteADistance = urand(15,20);
						Spell_Trop_Loin = 0;
						Cooldown_Trop_Loin = urand(3000, 8000);
						Cooldown_Trop_Loin_Defaut = urand(8000, 15000);
						break;
					case 39:	// Diablosaure - CREATURE_FAMILY_DEVILSAUR
						me->SetMeleeDamageSchool(SpellSchools(0));										// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_39[urand(0, 1)];
						Spell_B = liste_spell_B_39[urand(0, 4)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_39[urand(0, 1)];
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_39[urand(0, 1)];
						Spell_Heal = 0;
						Cooldown_SpellA = 500;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A+1000;
						Cooldown_SpellB = 1000;
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B, Base_Cooldown_Cast_B+2000);
						Cooldown_SpellB_rapide = 0;
						Cooldown_SpellB_rapide_defaut = Cooldown_SpellB_defaut;
						Cooldown_Spell_Heal_defaut = 60000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1000;
						Cooldown_Principal_B = 8000;
						Cooldown_Principal_B_Defaut = 20000;
						ResteADistance = 5;
						Spell_Trop_Loin = 0;
						Cooldown_Trop_Loin = 0;
						Cooldown_Trop_Loin_Defaut = 0;
						break;
					case 40:	// Goule - CREATURE_FAMILY_GHOUL
						me->SetMeleeDamageSchool(SpellSchools(5));										// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_40[urand(0, 1)];
						Spell_B = liste_spell_B_40[urand(0, 1)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_40[urand(0, 1)];
						Spell_respawn_evade = Spell_Bouclier_Dos;
						Buf_A = liste_Buf_40[urand(0, 1)];
						Spell_Heal = 0;
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A;
						Cooldown_SpellB = urand(3000,5000);
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B, Base_Cooldown_Cast_B+2000);
						Cooldown_SpellB_rapide = 0;
						Cooldown_SpellB_rapide_defaut = Cooldown_SpellB_defaut;
						Cooldown_Spell_Heal_defaut = 60000;
						Cooldown_Principal_A = 3000;
						Cooldown_Principal_A_Defaut = 3000;
						Cooldown_Principal_B = 8000;
						Cooldown_Principal_B_Defaut = 20000;
						ResteADistance = 5;
						Spell_Trop_Loin = 0;
						Cooldown_Trop_Loin = 0;
						Cooldown_Trop_Loin_Defaut = 0;
						break;
					case 42:	// Ver - CREATURE_FAMILY_WORM
						me->SetMeleeDamageSchool(SpellSchools(0));										// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_42[urand(0, 1)];
						Spell_B = liste_spell_B_42[urand(0, 1)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_42[urand(0, 1)];
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_42[urand(0, 1)];
						Spell_Heal = 0;
						Cooldown_SpellA = 500;
						Cooldown_SpellA_defaut = urand(Base_Cooldown_Cast_A+500, Base_Cooldown_Cast_A+1500);
						Cooldown_SpellB = 1000;
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B-1000, Base_Cooldown_Cast_B);
						Cooldown_SpellB_rapide = urand(Base_Cooldown_Cast_B-2500, Base_Cooldown_Cast_B-1500);
						Cooldown_SpellB_rapide_defaut = Cooldown_SpellB_defaut;
						Cooldown_Spell_Heal_defaut = 60000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1000;
						Cooldown_Principal_B = 1000;
						Cooldown_Principal_B_Defaut = 1000;
						ResteADistance = urand(15, 20);
						Spell_Trop_Loin = 0;
						Cooldown_Trop_Loin = urand(3000, 8000);
						Cooldown_Trop_Loin_Defaut = urand(8000, 15000);
						break;
					case 43:	// Rhinoceros - CREATURE_FAMILY_RHINO
						me->SetMeleeDamageSchool(SpellSchools(0));										// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_43[urand(0, 1)];
						Spell_B = liste_spell_B_43[urand(0, 1)];
						Spell_agro = liste_agro_43[urand(0, 1)];
						Spell_B_Non_Cumulable = 0;
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_43[urand(0, 1)];
						Spell_Heal = 0;
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A;
						Cooldown_SpellB = 2500;
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B, Base_Cooldown_Cast_B+2000);
						Cooldown_SpellB_rapide = 0;
						Cooldown_SpellB_rapide_defaut = 0;
						Cooldown_Spell_Heal_defaut = 30000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1000;
						Cooldown_Principal_B = 6000;
						Cooldown_Principal_B_Defaut = urand(8000,10000);
						ResteADistance = 5;
						Spell_Trop_Loin = Spell_Charge_Repousse;									//
						Cooldown_Trop_Loin = urand(6000, 8000);
						Cooldown_Trop_Loin_Defaut = urand(6000, 8000);
						AI_Random = urand(1, 3);
						break;
					case 44:	// Guepe - CREATURE_FAMILY_WASP
						me->SetMeleeDamageSchool(SpellSchools(3));										// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_44[urand(0, 1)];
						Spell_B = liste_spell_B_44[urand(0, 1)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_44[urand(0, 1)];
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_44[urand(0, 1)];
						Spell_Heal = 0;
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A;
						Cooldown_SpellB = 2500;
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B, Base_Cooldown_Cast_B+2000);
						Cooldown_SpellB_rapide = 0;
						Cooldown_SpellB_rapide_defaut = 0;
						Cooldown_Spell_Heal_defaut = 30000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1000;
						Cooldown_Principal_B = 5000;
						Cooldown_Principal_B_Defaut = urand(4000,6000);
						ResteADistance = 15;
						Spell_Trop_Loin = 0;														//
						Cooldown_Trop_Loin = 500;
						Cooldown_Trop_Loin_Defaut = urand(6000, 8000);
						break;
					case 45:	// Chien de base - CREATURE_FAMILY_CORE_HOUND
						me->SetMeleeDamageSchool(SpellSchools(0));										// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_45[urand(0, 1)];
						Spell_B = liste_spell_B_45[urand(0, 1)];
						Spell_B_Non_Cumulable = 1;	
						Spell_agro = liste_agro_45[urand(0, 1)];
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_45[urand(0, 1)];
						Spell_Heal = 0;																// 
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A;
						Cooldown_SpellB = urand(2500, 8000);
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B+6000, Base_Cooldown_Cast_B+12000);
						Cooldown_SpellB_rapide = 0;
						Cooldown_SpellB_rapide_defaut = 0;
						Cooldown_Spell_Heal_defaut = 120000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1500;
						Cooldown_Principal_B = 5000;
						Cooldown_Principal_B_Defaut = urand(4000, 6000);
						ResteADistance = 5;
						Spell_Trop_Loin = 0;														// 
						Cooldown_Trop_Loin = urand(7000, 12000);
						Cooldown_Trop_Loin_Defaut = urand(8000, 12000);
						AI_Random = urand(1, 3);
						break;
					case 49:	// Elementaire d'eau - CREATURE_FAMILY_WATER_ELEMENTAL
						me->SetMeleeDamageSchool(SpellSchools(4));										// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_49[urand(0, 1)];
						Spell_B = liste_spell_B_49[urand(0, 1)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_49[urand(0, 2)];
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_49[urand(0, 1)];
						Spell_Heal = 0;
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A;
						Cooldown_SpellB = 3000;
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B-2500, Base_Cooldown_Cast_B-500);
						Cooldown_SpellB_rapide = Base_Cooldown_Cast_B-3000;													// Cadence de tir SpellB rapide pour Mouvement_Cast_Puis_Contact
						Cooldown_SpellB_rapide_defaut = Cooldown_SpellB_defaut;												// Cadence de tir SpellB normale pour Mouvement_Cast_Puis_Contact
						Cooldown_Spell_Heal_defaut = 60000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1000;
						Cooldown_Principal_B = 2000;
						Cooldown_Principal_B_Defaut = 4000;
						ResteADistance = 5;
						Spell_Trop_Loin = 0;
						Cooldown_Trop_Loin = 8000;
						Cooldown_Trop_Loin_Defaut = 30000;
						AI_Random = urand(1, 4);
						if (AI_Random == 1)
						{
							ResteADistance = 12;
							Cooldown_Principal_A_Defaut = 2000;
							Cooldown_Principal_B_Defaut = 1000;
						}
						else if (AI_Random == 2)
						{
							ResteADistance = 15;
							Cooldown_Principal_A_Defaut = 2000;
							Cooldown_Principal_B_Defaut = 5000;
						}
						else
						{
							ResteADistance = 5;
							Cooldown_Principal_A_Defaut = 1000;
							Cooldown_Principal_B_Defaut = 6000;
						}
						break;

					case 50:	// Renard - CREATURE_FAMILY_FOX
						me->SetMeleeDamageSchool(SpellSchools(0));														// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_50[urand(0, 1)];
						Spell_B = liste_spell_B_50[urand(0, 1)];
						Spell_B_Non_Cumulable = 1;											
						Spell_agro = liste_agro_50[urand(0, 1)];
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_50[urand(0, 2)];
						Spell_Heal = 0;																					// 
						Cooldown_SpellA = 2000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A;
						Cooldown_SpellB = urand(4000, 6000);
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B+4000, Base_Cooldown_Cast_B+9000);
						Cooldown_SpellB_rapide = 0;
						Cooldown_SpellB_rapide_defaut = 0;
						Cooldown_Spell_Heal_defaut = 120000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1000;
						Cooldown_Principal_B = 5000;
						Cooldown_Principal_B_Defaut = urand(6000, 8000);
						ResteADistance = 5;
						Spell_Trop_Loin = 0;																			// 
						Cooldown_Trop_Loin = urand(7000, 12000);
						Cooldown_Trop_Loin_Defaut = urand(8000, 12000);

						AI_Random = urand(1, 3);
						if (AI_Random != 1) { Cooldown_Principal_B_Defaut = urand(4000, 6000); }						//
						break;
					case 51:	// Singe - CREATURE_FAMILY_MONKEY
						me->SetMeleeDamageSchool(SpellSchools(0));														// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_51[urand(0, 1)];
						Spell_B = liste_spell_B_51[urand(0, 1)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_51[urand(0, 2)];
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_51[urand(0, 1)];
						Spell_Heal = 0;																					// 
						Cooldown_SpellA = 2000;
						Cooldown_SpellA_defaut = urand(Base_Cooldown_Cast_A-500, Base_Cooldown_Cast_A);
						Cooldown_SpellB = 1000;
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B-3500, Base_Cooldown_Cast_B-3000);
						Cooldown_SpellB_rapide = 0;
						Cooldown_SpellB_rapide_defaut = Cooldown_SpellB_defaut;
						Cooldown_Spell_Heal_defaut = 120000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1000;
						Cooldown_Principal_B = 1500;
						Cooldown_Principal_B_Defaut = urand(3500, 4500);
						ResteADistance = urand(12,15);
						Spell_Trop_Loin = 0;																			// 
						Cooldown_Trop_Loin = urand(7000, 12000);
						Cooldown_Trop_Loin_Defaut = urand(8000, 12000);
						break;
					case 52:	// Chien - CREATURE_FAMILY_DOG
						me->SetMeleeDamageSchool(SpellSchools(0));														// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_52[urand(0, 1)];
						Spell_B = liste_spell_B_52[urand(0, 1)];
						Spell_B_Non_Cumulable = 1;
						Spell_agro = liste_agro_52[urand(0, 1)];
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_52[urand(0, 1)];
						Spell_Heal = 0;																					// 
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A-250;
						Cooldown_SpellB = urand(4000, 6000);
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B+2000, Base_Cooldown_Cast_B+6000);
						Cooldown_SpellB_rapide = 0;
						Cooldown_SpellB_rapide_defaut = 0;
						Cooldown_Spell_Heal_defaut = 120000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1000;
						Cooldown_Principal_B = 5000;
						Cooldown_Principal_B_Defaut = urand(5000, 7000);
						ResteADistance = 5;
						Spell_Trop_Loin = 0;																			// 
						Cooldown_Trop_Loin = urand(7000, 12000);
						Cooldown_Trop_Loin_Defaut = urand(8000, 12000);
						AI_Random = urand(1, 3);
						break;
					case 55:	// Araignee de schiste - CREATURE_FAMILY_SHALE_SPIDER
						me->SetMeleeDamageSchool(SpellSchools(6));														// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_55[urand(0, 1)];
						Spell_B = liste_spell_B_55[urand(0, 1)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_55[urand(0, 1)];
						Spell_respawn_evade = Spell_Armure_De_La_Fournaise;
						Buf_A = liste_Buf_55[urand(0, 1)];
						Buf_A = 0;
						Spell_Heal = 0;
						Cooldown_SpellA = 1500;
						Cooldown_SpellA_defaut = urand(Base_Cooldown_Cast_A-500, Base_Cooldown_Cast_A-250);
						Cooldown_SpellB = 3000;
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B+2000, Base_Cooldown_Cast_B+4000);
						Cooldown_SpellB_rapide = Base_Cooldown_Cast_B-1000;												// Cadence de tir SpellB rapide pour Mouvement_Cast_Puis_Contact
						Cooldown_SpellB_rapide_defaut = Cooldown_SpellB_defaut;											// Cadence de tir SpellB normale pour Mouvement_Cast_Puis_Contact
						Cooldown_Spell_Heal_defaut = 60000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 2000;
						Cooldown_Principal_B = 3000;
						Cooldown_Principal_B_Defaut = urand(4000,5000);
						ResteADistance = 5;
						Spell_Trop_Loin = Spell_Bondir_Guerrier;														//
						Cooldown_Trop_Loin = 8000;
						Cooldown_Trop_Loin_Defaut = urand(8000, 15000);

						AI_Random = urand(1, 2);
						if (AI_Random == 1)
						{
							Cooldown_Principal_B_Defaut = urand(5000, 7000);
						}

						break;
					case 56:	// Zombie - CREATURE_FAMILY_ZOMBIE
						me->SetMeleeDamageSchool(SpellSchools(0));														// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_56[urand(0, 1)];
						Spell_B = liste_spell_B_56[urand(0, 1)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_56[urand(0, 4)];
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_56[urand(0, 1)];
						Spell_Heal = 0;																					// 
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = urand(Base_Cooldown_Cast_A, Base_Cooldown_Cast_A+500);
						Cooldown_SpellB = urand(4000, 6000);
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B+2000, Base_Cooldown_Cast_B+6000);
						Cooldown_SpellB_rapide = 0;
						Cooldown_SpellB_rapide_defaut = 0;
						Cooldown_Spell_Heal_defaut = 120000;
						Cooldown_Principal_A = 2000;
						Cooldown_Principal_A_Defaut = 2500;
						Cooldown_Principal_B = 5000;
						Cooldown_Principal_B_Defaut = urand(7000, 11000);
						ResteADistance = 5;
						Spell_Trop_Loin = 0;																			// 
						Cooldown_Trop_Loin = urand(7000, 12000);
						Cooldown_Trop_Loin_Defaut = urand(8000, 12000);
						break;
					case 68:	// Hydre - CREATURE_FAMILY_HYDRA
						me->SetMeleeDamageSchool(SpellSchools(3));														// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_68[urand(0, 1)];
						Spell_B = liste_spell_B_68[urand(0, 2)];
						Spell_B_Non_Cumulable = 1;
						Spell_agro = liste_agro_68[urand(0, 1)];
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_68[urand(0, 1)];
						Spell_Heal = 0;																					// 
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = urand(Base_Cooldown_Cast_A-250, Base_Cooldown_Cast_A+250);
						Cooldown_SpellB = urand(3000, 4000);
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B+1000, Base_Cooldown_Cast_B+2000);
						Cooldown_SpellB_rapide = 0;
						Cooldown_SpellB_rapide_defaut = 0;
						Cooldown_Spell_Heal_defaut = 120000;
						Cooldown_Principal_A = 2000;
						Cooldown_Principal_A_Defaut = 2500;
						Cooldown_Principal_B = 7000;
						Cooldown_Principal_B_Defaut = urand(7000, 11000);
						ResteADistance = 5;
						Spell_Trop_Loin = Poison_Tir_Catapulte;															// 
						Cooldown_Trop_Loin = urand(5000, 7000);
						Cooldown_Trop_Loin_Defaut = urand(6000, 8000);

						AI_Random = urand(1, 2);
						if (AI_Random == 2)
						{
							Cooldown_Principal_B_Defaut = urand(6000, 8000);
						}

						break;
					case 100:	// Diablotin gangrene - CREATURE_FAMILY_FELIMP
						me->SetMeleeDamageSchool(SpellSchools(5));														// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_100[urand(0, 1)];
						Spell_B = liste_spell_B_100[urand(0, 1)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_100[urand(0, 1)];
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_100[urand(0, 1)];
						Spell_Heal = 0;																					// 
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = urand(Base_Cooldown_Cast_A-250, Base_Cooldown_Cast_A);
						Cooldown_SpellB = 2000;
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B-3000, Base_Cooldown_Cast_B-2500);
						Cooldown_SpellB_rapide = Base_Cooldown_Cast_B-3000;
						Cooldown_SpellB_rapide_defaut = Cooldown_SpellB_defaut;
						Cooldown_Spell_Heal_defaut = 120000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1000;
						Cooldown_Principal_B = 1000;
						Cooldown_Principal_B_Defaut = urand(5000, 7000);
						ResteADistance = urand(12,15);
						Spell_Trop_Loin = 0;																			// 
						Cooldown_Trop_Loin = urand(5000, 7000);
						Cooldown_Trop_Loin_Defaut = urand(6000, 8000);

						AI_Random = urand(1, 2);
						if (AI_Random == 2)
						{
							Cooldown_Principal_B_Defaut = 1000;
						}

						break;
					case 104:	// Garde - courroux - CREATURE_FAMILY_WRATHGUARD
						me->SetMeleeDamageSchool(SpellSchools(0));														// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_104[urand(0, 1)];
						Spell_B = liste_spell_B_104[urand(0, 1)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_104[urand(0, 1)];
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_104[urand(0, 1)];
						Spell_Heal = 0;																					// 
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A;
						Cooldown_SpellB = 2000;
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B-2000, Base_Cooldown_Cast_B);
						Cooldown_SpellB_rapide = 0;
						Cooldown_SpellB_rapide_defaut = 0;
						Cooldown_Spell_Heal_defaut = 120000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1000;
						Cooldown_Principal_B = 6000;
						Cooldown_Principal_B_Defaut = urand(7000, 9000);
						ResteADistance = 5;
						Spell_Trop_Loin = Spell_Poursuite;																// 
						Cooldown_Trop_Loin = urand(7000, 12000);
						Cooldown_Trop_Loin_Defaut = urand(8000, 12000);

						AI_Random = urand(1, 2);
						if (AI_Random == 1)
						{
							Cooldown_Principal_B_Defaut = urand(5000, 6000);;
						}

						//me->SetVirtualItem(0, 12784);																	// Equipé d'une hache
						me->LoadEquipment(1, true);																		// creature_equip_template 1
						break;
					case 108:	// Infernal - CREATURE_FAMILY_INFERNAL
						me->SetMeleeDamageSchool(SpellSchools(2));														// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_108[urand(0, 1)];
						Spell_B = liste_spell_B_108[urand(0, 1)];
						Spell_B_Non_Cumulable = 1;
						Spell_agro = liste_agro_108[urand(0, 2)];
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_108[urand(0, 3)];
						Spell_Heal = 0;																					// 
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = urand(Base_Cooldown_Cast_A+3500, Base_Cooldown_Cast_A+4500);
						Cooldown_SpellB = 2000;
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B+1000, Base_Cooldown_Cast_B+3000);
						Cooldown_SpellB_rapide = 0;
						Cooldown_SpellB_rapide_defaut = 0;
						Cooldown_Spell_Heal_defaut = 120000;
						Cooldown_Principal_A = 2000;
						Cooldown_Principal_A_Defaut = 2000;
						Cooldown_Principal_B = 6000;
						Cooldown_Principal_B_Defaut = urand(7000, 9000);
						ResteADistance = 5;
						Spell_Trop_Loin = 0;																			// 
						Cooldown_Trop_Loin = urand(7000, 12000);
						Cooldown_Trop_Loin_Defaut = urand(8000, 12000);
						break;
					case 116:	// elementaire de feu - CREATURE_FAMILY_FIREELEMENTAL
						me->SetMeleeDamageSchool(SpellSchools(2));														// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_116[urand(0, 1)];
						Spell_B = liste_spell_B_116[urand(0, 1)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_116[urand(0, 1)];
						Spell_respawn_evade = Spell_Armure_De_La_Fournaise;
						Buf_A = liste_Buf_116[urand(0, 1)];
						Spell_Heal = 0;
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A;
						Cooldown_SpellB = 1500;
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B-2500, Base_Cooldown_Cast_B-1500);
						Cooldown_SpellB_rapide = Base_Cooldown_Cast_B-3500;												// Cadence de tir SpellB rapide pour Mouvement_Cast_Puis_Contact
						Cooldown_SpellB_rapide_defaut = Cooldown_SpellB_defaut;											// Cadence de tir SpellB normale pour Mouvement_Cast_Puis_Contact
						Cooldown_Spell_Heal_defaut = 60000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1000;
						Cooldown_Principal_B = 2000;
						Cooldown_Principal_B_Defaut = 4000;
						ResteADistance = 5;
						Spell_Trop_Loin = 0;
						Cooldown_Trop_Loin = 8000;
						Cooldown_Trop_Loin_Defaut = 30000;

						AI_Random = urand(1, 4);
						if (AI_Random == 1)
						{
							ResteADistance = urand(8, 10);
							Cooldown_Principal_B_Defaut = 1000;
						}
						else if (AI_Random == 2)
						{
							ResteADistance = urand(8,10);
							Cooldown_Principal_B_Defaut = 4000;
						}
						break;
					case 117:	// elementaire de terre - CREATURE_FAMILY_EARTHELEMENTAL   
						me->SetMeleeDamageSchool(SpellSchools(3));														// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_117[urand(0, 1)];
						Spell_B = liste_spell_B_117[urand(0, 2)];
						Spell_B_Non_Cumulable = 0;
						Spell_agro = liste_agro_117[urand(0, 1)];
						Spell_respawn_evade = Spell_Ecorce;
						Buf_A = liste_Buf_117[urand(0, 1)];
						Spell_Heal = 0;
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A;
						Cooldown_SpellB = 2000;
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B, Base_Cooldown_Cast_B+3000);
						Cooldown_SpellB_rapide = Base_Cooldown_Cast_B-3500;												// Cadence de tir SpellB rapide pour Mouvement_Cast_Puis_Contact
						Cooldown_SpellB_rapide_defaut = Cooldown_SpellB_defaut;											// Cadence de tir SpellB normale pour Mouvement_Cast_Puis_Contact
						Cooldown_Spell_Heal_defaut = 60000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1000;
						Cooldown_Principal_B = urand(4000,4500);
						Cooldown_Principal_B_Defaut = urand(5000, 6000);
						ResteADistance = 5;
						Spell_Trop_Loin = Lancer_Une_Pierre;															// lancer une pierre 130775
						Cooldown_Trop_Loin = urand(0,4000);
						Cooldown_Trop_Loin_Defaut = urand(3000, 5000);
						break;
					case 130:	// Basilic - CREATURE_FAMILY_BASILISK
						me->SetMeleeDamageSchool(SpellSchools(3));														// Physique=0, Sacré=1, Feu=2, Nature=3, Givre=4, Ombre=5, Arcane=6
						Spell_A = liste_spell_A_130[urand(0, 1)];
						Spell_B = liste_spell_B_130[urand(0, 3)];
						Spell_B_Non_Cumulable = 1;
						Spell_agro = liste_agro_130[urand(0, 1)];
						Spell_respawn_evade = Spell_Ecorce;
						Buf_A = liste_spell_A_130[urand(0, 1)];
						Spell_Heal = 0;
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A;
						Cooldown_SpellB = 3000;
						Cooldown_SpellB_defaut = urand(Base_Cooldown_Cast_B, Base_Cooldown_Cast_B+3000);
						Cooldown_SpellB_rapide = Base_Cooldown_Cast_B-3500;												// Cadence de tir SpellB rapide pour Mouvement_Cast_Puis_Contact
						Cooldown_SpellB_rapide_defaut = Cooldown_SpellB_defaut;											// Cadence de tir SpellB normale pour Mouvement_Cast_Puis_Contact
						Cooldown_Spell_Heal_defaut = 60000;
						Cooldown_Principal_A = 2000;
						Cooldown_Principal_A_Defaut = 2000;
						Cooldown_Principal_B = urand(5000, 7000);
						Cooldown_Principal_B_Defaut = urand(7000, 9000);
						ResteADistance = 5;
						Spell_Trop_Loin = 0;														
						Cooldown_Trop_Loin = urand(0, 4000);
						Cooldown_Trop_Loin_Defaut = urand(3000, 5000);
						AI_Random = urand(1, 2);
						break;




					default:
						Spell_A = liste_spell_A_0[urand(0, 1)];
						Spell_B = liste_spell_B_0[urand(0, 2)];
						Spell_agro = 0;
						Spell_respawn_evade = 0;
						Buf_A = liste_Buf_0[urand(0, 2)];
						Spell_Heal = 0;
						Cooldown_SpellA = 1000;
						Cooldown_SpellA_defaut = Base_Cooldown_Cast_A;
						Cooldown_SpellB = 3000;
						Cooldown_SpellB_defaut = Base_Cooldown_Cast_B-1000;
						Cooldown_SpellB_rapide = Base_Cooldown_Cast_B-3000;
						Cooldown_SpellB_rapide_defaut = 5000;								
						Cooldown_Spell_Heal_defaut = 60000;
						Cooldown_Principal_A = 1000;
						Cooldown_Principal_A_Defaut = 1000;
						Cooldown_Principal_B = 4000;
						Cooldown_Principal_B_Defaut = 6000 + ((urand(0, 4) * 500));
						ResteADistance = 5;
						Spell_Trop_Loin = 0;
						Cooldown_Trop_Loin = 60000;
						Cooldown_Trop_Loin_Defaut = 60000;
						break;


					}

				} else	// Ou lecture des spells dans creature_template->spells1-6 --------------------------------------------------------------------------------
				{ 
					Spell_A = me->m_spells[0]; 
					if (me->m_spells[1] != 0) 
					{ Spell_B = me->m_spells[1]; } 
					else  Spell_B = 0;

					if (me->m_spells[2] != 0) 
					{ Spell_agro = me->m_spells[2]; }
					else  Spell_agro = 0;

					if (me->m_spells[3] != 0) 
					{ Spell_respawn_evade = me->m_spells[3]; }
					else  Spell_respawn_evade = 0;

					if (me->m_spells[4] != 0) 
					{ Buf_A = me->m_spells[4]; }
					else  Buf_A = 0;

					if (me->m_spells[5] != 0) 
					{ Spell_Heal = me->m_spells[5]; }
					else  Spell_Heal = 0;
				}

				// Divers  ----------------------------------------------------------------------------------------------------------------------------------------
				me->SetReactState(REACT_AGGRESSIVE);

				// ################################################################################################################################################
			}
			void JustRespawned() override
			{
				me->GetMotionMaster()->MoveTargetedHome();															// Retour home pour rafraichir client
				me->SetSpeedRate(MOVE_RUN, 1.01f);
				me->SetReactState(REACT_AGGRESSIVE);

				Random = urand(1, 3);
				if (Random == 1 && Spell_respawn_evade != 0) { me->CastSpell(me, Spell_respawn_evade, true); }		// 1/3 Chance de lancer le sort sur lui au respawn ou evade

				// --- Spécificitée par family ---
				Random = urand(1, 3);
				if (Random == 1)
				{
					if (Crfamily == 20 || Crfamily == 42) { Senterre(); }
				}
				// -------------------------------
				// -------------------------------
			}
			void EnterCombat(Unit* /*who*/) override
			{
				if (Crfamily == 20)
				{
					Se_Deterre();
				}

				Init_AI();

			}
			void EnterEvadeMode(EvadeReason /*why*/) override
			{
				Start_Agro = 0;
				Spell_B_Non_Cumulable = 0;
				RetireBugDeCombat();
				me->AddUnitState(UNIT_STATE_EVADE);
				me->GetMotionMaster()->MoveTargetedHome();															// Retour home
				me->RemoveAllControlled();																			// renvois pet

				me->RemoveAura(Buf_A);

				Random = urand(1, 3);
				if (Random == 1 && Spell_respawn_evade != 0) { me->CastSpell(me, Spell_respawn_evade, true); }		// 1/3 Chance de lancer le sort sur lui au respawn ou evade
			}
			void JustReachedHome() override
			{
				me->SetReactState(REACT_AGGRESSIVE);
				me->SetSpeedRate(MOVE_RUN, 1.01f);		


				Random = urand(1, 3);
				if (Random == 1)
				{
					if (Crfamily == 20 || Crfamily == 42) { Senterre(); }
				}
				// -------------------------------



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

				//Position const& homePos = me->GetHomePosition();
				if (me->IsAlive() && !me->IsInCombat() && !me->isMoving() && (me->GetDistance(me->GetHomePosition()) >  15))
				//if (me->IsAlive() && !me->IsInCombat() && !me->isMoving() && (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > 1))
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

			void UpdateAI(uint32 diff) override
			{

				// ################################################################################################################################################
				// En Combat ######################################################################################################################################
				// ################################################################################################################################################
				if (UpdateVictim() /*&& !me->HasUnitState(UNIT_STATE_MOVE) || me->HasUnitState(UNIT_STATE_CASTING)*/)
				{
					Unit* victim = me->GetVictim();
					Dist = me->GetDistance(victim);

					// Ce heal s'il a un sort de heal -------------------------------------------------------------------------------------------------------------
					if (Spell_Heal != 0 && Spell_Heal != 0)
					{
						Heal_En_Combat(diff);
					}

						// Spell a lancer a l'agro ----------------------------------------------------------------------------------------------------------------
					if (Start_Agro == 0)
					{
						Start_Agro = 1;

						// ------ Scorpion, Ver ------
						if (Crfamily == 20 || Crfamily == 42) 
						{
							Se_Deterre();
						}
						// ---------------------------

						Random = urand(1, 4);
						if (Random == 1 && Buf_A != 0) { me->CastSpell(me, Buf_A, true); }								// 1 chance sur 4 de lancer au sort de buf a l'agro

						Random = urand(1, 4);
						if (Random == 1 && Spell_agro != 0) { me->CastSpell(victim, Spell_agro, true); }				// 1 Chance sur 4 de lancer le sort sur la cible a d'agro
					}

					// ############################################################################################################################################
					// COMBAT 

						// SpellA sur la cible  -------------------------------------------------------------------------------------------------------------------
					if (Spell_A != 0)
					{
						if (Cooldown_SpellA <= diff)
						{
							if (Dist <= 6)
							{
								DoMeleeAttackIfReady();																	// Combat en mélée
								me->CastSpell(victim, Spell_A, true);

								Cooldown_SpellA = Cooldown_SpellA_defaut + urand(0,1000) -500;
							}
						}
						else Cooldown_SpellA -= diff;
					}

						// SpellB sur la cible  -------------------------------------------------------------------------------------------------------------------
						if (Spell_B != 0)
						{
							if (Cooldown_SpellB <= diff)
							{
								if (Dist <= DistanceDeCast /*ResteADistance*/)
								{
									if (Spell_B_Non_Cumulable == 0 || (Spell_B_Non_Cumulable == 1) && !victim->HasAura(Spell_B) )
									{
										me->CastSpell(victim, Spell_B, true);
										Cooldown_SpellB = Cooldown_SpellB_defaut;
									}
								}
							}
							else Cooldown_SpellB -= diff;
						}

					// ############################################################################################################################################
					// MOUVEMENT
						switch (Crfamily)
						{
						case 1: // Loup
							Mouvement_Contact_Prudent(diff);
							break;
						case 2: // Felin
							Mouvement_Contact_Tournant_Aleatoire(diff);
							break;
						case 3:		// Araignee - CREATURE_FAMILY_SPIDER
							Mouvement_Caster_Puis_Contact(diff);
							break;
						case 4:		// Ours - CREATURE_FAMILY_BEAR
							Mouvement_Contact_Basique(diff);
							break;
						case 5:		// Sanglier - CREATURE_FAMILY_BOAR
							Mouvement_Contact_Charges_Multiples(diff);
							break;

						case 6:		// Crocodile - CREATURE_FAMILY_CROCOLISK
							if (AI_Random == 1)
							{
								Mouvement_Contact_Avance_Recule(diff);
							}
							else
								Mouvement_Contact_Basique(diff);
							break;
						case 7:		// Oiseau charogniar - CREATURE_FAMILY_CARRION_BIRD
							Mouvement_Contact_Prudent_Volant(diff);
							break;
						case 8:		// Crabe - CREATURE_FAMILY_CRAB
							Mouvement_Contact_Avance_Recule(diff);
							break;
						case 9:		// Gorille - CREATURE_FAMILY_GORILLA
							Mouvement_Contact_Bondissant(diff);
							break;
						case 11:	// Raptor - CREATURE_FAMILY_RAPTOR
							Mouvement_Contact_Basique(diff);
							break;
						case 16:	// Voidwalker - CREATURE_FAMILY_VOIDWALKER
							Mouvement_Contact_Basique(diff);
							break;
						case  17:	// Succube - CREATURE_FAMILY_SUCCUBUS
							Mouvement_Contact_Tournant_Aleatoire(diff);
							break;
						case 19:	// Doomguard - CREATURE_FAMILY_DOOMGUARD
							Mouvement_Contact_Charges_Multiples(diff);
							break;
						case 20:	// Scorpion - CREATURE_FAMILY_SCORPID
							Mouvement_Contact_Avance_Recule(diff);
							break;
						case 21:	// Tortue - CREATURE_FAMILY_TURTLE
							Mouvement_Contact_Basique(diff);
							break;
						case 23:	// Imp - CREATURE_FAMILY_IMP
							Mouvement_Caster_Puis_Contact(diff);
							break;
						case 24:	// Chauve souris - CREATURE_FAMILY_BAT
							Mouvement_Contact_Prudent_Volant(diff);
							break;
						case 25:	// Hyene - CREATURE_FAMILY_HYENA
							Mouvement_Contact_Prudent(diff);
							break;
						case 26:	// Oiseau de proie - CREATURE_FAMILY_BIRD_OF_PREY
							Mouvement_Contact_Basique(diff);
							break;
						case 27:	// Serpent des vents - CREATURE_FAMILY_WIND_SERPENT
							Mouvement_Caster_Puis_Contact(diff);
							break;
						case 29:	// Gangregarde (Felguard) - CREATURE_FAMILY_FELGUARD
							Mouvement_Contact_Basique(diff);
							break;
						case 30:	// Faucon dragon - CREATURE_FAMILY_DRAGONHAWK
							Mouvement_Caster(diff);
							break;
						case 31:	// Ravageur - CREATURE_FAMILY_RAVAGER
							if (AI_Random == 1) { Mouvement_Contact_Prudent(diff); }
							if (AI_Random == 2) { Mouvement_Contact_Avance_Recule(diff); }
							if (AI_Random == 3) { Mouvement_Contact_Tournant_Aleatoire(diff); }
							if (AI_Random == 4) { Mouvement_Contact_Charges_Multiples(diff); }
							
							break;
						case 34:	// Raie du Neant - CREATURE_FAMILY_NETHER_RAY
							if (AI_Random == 1)
							{ 
								Mouvement_Caster_Puis_Contact(diff); 
							}
							if (AI_Random == 2)
							{ 
								Mouvement_Caster(diff); 
							}
							break;
						case 35:	// Serpent - CREATURE_FAMILY_SERPENT
							Mouvement_Contact_Basique(diff);
							break;
						case 37:	// Papillon de nuit - CREATURE_FAMILY_MOTH
							Mouvement_Caster(diff);
							break;
						case 38:	// Chimere - CREATURE_FAMILY_CHIMAERA
							Mouvement_Caster_Puis_Contact(diff);
							break;
						case 39:	// Diablosaure - CREATURE_FAMILY_DEVILSAUR
							Mouvement_Contact_Basique(diff);
							break;
						case 40:	// Goule - CREATURE_FAMILY_GHOUL
							Mouvement_Contact_Basique(diff);
							break;
						case 42:	// Ver - CREATURE_FAMILY_WORM
							Mouvement_Caster_Puis_Contact(diff);
							break;
						case 43:	// Rhinoceros - CREATURE_FAMILY_RHINO
							if (AI_Random == 1)
							{
								Mouvement_Contact_Basique(diff);
							}
							else
								Mouvement_Contact_Charges_Multiples(diff);
							break;
						case 44:	// Guepe - CREATURE_FAMILY_WASP
							Mouvement_Contact_Prudent_Volant(diff);
							break;
						case 45:	// Chien de base - CREATURE_FAMILY_CORE_HOUND
							if (AI_Random == 1)
							{
								Mouvement_Contact_Prudent(diff);
							}
							else
								Mouvement_Contact_Charges_Multiples(diff);
							break;
						case 49:	// Elementaire d'eau - CREATURE_FAMILY_WATER_ELEMENTAL
							if (AI_Random == 1) 
							{ 
								Mouvement_Caster_Puis_Contact(diff); 
							}
							else if (AI_Random == 2) 
							{
								Mouvement_Caster(diff); 
							}
							else
							{
								Mouvement_Contact_Basique(diff);
							}
							break;
						case 50:	// Renard - CREATURE_FAMILY_FOX
							if (AI_Random == 1) { Mouvement_Contact_Basique(diff); }
							if (AI_Random != 1) { Mouvement_Contact_Prudent(diff); }
							break;
						case 51:	// Singe - CREATURE_FAMILY_MONKEY
							Mouvement_Caster(diff);
							break;
						case 52:	// Chien - CREATURE_FAMILY_DOG
							if (AI_Random == 1)
							{
								Mouvement_Contact_Prudent(diff);
							}
							else
								Mouvement_Contact_Basique(diff);
							break;
						case 55:	// Araignee de schiste - CREATURE_FAMILY_SHALE_SPIDER
							if (AI_Random == 1)
							{
								Mouvement_Contact_Avance_Recule(diff);
							}
							else
								Mouvement_Contact_Tournant_Aleatoire(diff);
							break;
						case 56:	// Zombie - CREATURE_FAMILY_ZOMBIE
							Mouvement_Contact_Basique(diff);
							break;
						case 68:	// Hydre - CREATURE_FAMILY_HYDRA
							if (AI_Random == 1)
							{
								Mouvement_Contact_Basique(diff);
							}
							else
								Mouvement_Contact_Avance_Recule(diff);

							break;
						case 100:	// Diablotin gangrene - CREATURE_FAMILY_FELIMP
							if (AI_Random == 1)
							{
								Mouvement_Contact_Avance_Recule(diff);
							}
							else
								Mouvement_Caster_Puis_Contact(diff);
							break;
						case 104:	// Garde - courroux - CREATURE_FAMILY_WRATHGUARD
							if (AI_Random == 1)
							{
								Mouvement_Contact_Basique(diff);
							}
							else
								Mouvement_Contact_Charges_Multiples(diff);
							break;
						case 108:	// Infernal - CREATURE_FAMILY_INFERNAL
							Mouvement_Contact_Basique(diff);
							break;
						case 116:	// elementaire de feu - CREATURE_FAMILY_FIREELEMENTAL
							if (AI_Random == 1)
							{
								Mouvement_Caster_Puis_Contact(diff);
							}
							else if (AI_Random == 2)
							{
								Mouvement_Caster(diff);
							}
							else
							{
								Mouvement_Contact_Basique(diff);
							}
							break;
						case 117:	// elementaire de terre - CREATURE_FAMILY_EARTHELEMENTAL
							Mouvement_Contact_Basique(diff);
							break;
						case 130:	// Basilic - CREATURE_FAMILY_BASILISK
							if (AI_Random == 1)
							{
								Mouvement_Contact_Avance_Recule(diff);
							}
							else 
								Mouvement_Contact_Basique(diff);
							break;


						default:
							Mouvement_Contact_Basique(diff);
							break;


						}

				}

				// ################################################################################################################################################
				Mouvement_All();
			}


			// ------ AI PRINCIPALES ------
			// ###### Caster , reste a distance ####################################################################################################################
			void Mouvement_Caster(uint32 diff)
			{
				if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
					return;

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				if (Cooldown_Principal_B <= diff)
				{
					// Mouvement aléatoire si cible < 4m  ---------------------------------------------------------------------------------------------------------

					if (Dist <4)
					{
						me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);						// UNROOT
						me->SetSpeedRate(MOVE_RUN, 1.01f);

						//float x, y, z, mapid;
						x = (me->GetPositionX() + urand(0, ResteADistance * 2) - ResteADistance);
						y = (me->GetPositionY() + urand(0, ResteADistance * 2) - ResteADistance);
						z = me->GetPositionZ();
						mapid = victim->GetMapId();
						me->GetMotionMaster()->MovePoint(mapid, x, y, z);
						Cooldown_Principal_B = Cooldown_Principal_B_Defaut;
					}
				}
				else Cooldown_Principal_B -= diff;

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

			}

			// ###### Reste a distance mais va au contact si la cible ce raproche , spellB plus rapide de loin #####################################################
			void Mouvement_Caster_Puis_Contact(uint32 diff)
			{
				if (!UpdateVictim())
					return;

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				if (Cooldown_Principal_B <= diff)
				{
					// Combat au contact si la cible <= 4m  -------------------------------------------------------------------------------------------------------

					if (Dist <= 4)
					{
						Cooldown_SpellB_defaut = Cooldown_SpellB_rapide_defaut;							// Cadence de tir SpellB normale

						me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);						// UNROOT
						me->GetMotionMaster()->MoveChase(victim, 1, frand(0, 6.2836f));					// Pour suivre la cible avec un angle
					}

					// RESTE A DISTANCE ---------------------------------------------------------------------------------------------------------------------------
					if (Dist > 4 && Dist <= ResteADistance)
					{
						Cooldown_SpellB_defaut = Cooldown_SpellB_rapide;								// Cadence de tir SpellB rapide

						if (me->isMoving())
						{
							//float x, y, z, mapid;
							x = (victim->GetPositionX());
							y = (victim->GetPositionY());
							z = victim->GetPositionZ();
							mapid = victim->GetMapId();
							me->GetClosePoint(x, y, z, me->GetObjectSize() / 3, -0.85f);				// Indispensable pour stoper le mouvement
							me->GetMotionMaster()->MovePoint(mapid, x, y, z);
						}
						me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);							// ROOT
					}
					Cooldown_Principal_B = Cooldown_Principal_B_Defaut;
				}
				else Cooldown_Principal_B -= diff;


				// Cadence de tir OFF si distance > 10m -----------------------------------------------------------------------------------------------------------
				if (Dist > ResteADistance)
				{
					Cooldown_SpellB_defaut = Cooldown_SpellB_rapide_defaut;								// Cadence de tir SpellB normale

					me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);							// UNROOT
					AttackStartCaster(victim, ResteADistance);											// Distance de cast
				}

				// SI LA CIBLE EST TROP LOIN ----------------------------------------------------------------------------------------------------------------------
				if ((Dist >= ResteADistance + 5 && Dist <= ResteADistance + 15) && (Cooldown_Trop_Loin <= diff))
				{
					Random = urand(1, 4);
					if (Random == 1 && Spell_Trop_Loin != 0)
					{
						DoCastAOE(Spell_Trop_Loin, true);
					}
					Cooldown_Trop_Loin = Cooldown_Trop_Loin_Defaut;
				}
				else Cooldown_Trop_Loin -= diff;

			}

			// ###### Va sur la cible et reste au contact , tourne au tour de la cible tres rarement ###############################################################
			void Mouvement_Contact_Basique(uint32 diff)
			{
				if (!UpdateVictim())
					return;

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				DoMeleeAttackIfReady();																		// Combat en mélée

				// ------ ALLER A LA CIBLE -------------------------------------------------------------------------------------------------------------------------
				if (Cooldown_Principal_A <= diff)
				{
					if (Dist >= 6)
					{
					me->GetMotionMaster()->MoveChase(victim, 1, urand(0, 6));								// Pour suivre la cible avec un angle
					Cooldown_Principal_A = Cooldown_Principal_A_Defaut;
					}
				}
				else Cooldown_Principal_A -= diff;

				// TOURNE AU TOUR DE LA CIBLE ----------------------------------------------------------------------------------------------------------------------
				if (Cooldown_Principal_B <= diff)
				{
					if (Dist < 6)
					{
					Random = urand(1, 4);
					if (Random == 1)
					{
						Tourne_Au_Tour_Aleatoire(2);														// 1 chances sur 4 : Tourne_Au_Tour_Aleatoire
					}
					Cooldown_Principal_B = Cooldown_Principal_B_Defaut;
					}
				}
				else Cooldown_Principal_B -= diff;

				// ------ CIBLE TROP LOIN -------------------------------------------------------------------------------------------------------------------------
				if (Cooldown_Trop_Loin <= diff)
				{
					if (Dist >= 8 && Dist <= 25)
					{
						if (Spell_Trop_Loin != 0)
						{
							Random = urand(1, 2);
							if (Random == 1)
							{
								me->CastSpell(victim, Spell_Trop_Loin, true);								// 1 chances sur 2 : Charge/Bond...
							}
						}
						Cooldown_Trop_Loin = Cooldown_Trop_Loin_Defaut;
					}
				}
				else Cooldown_Trop_Loin -= diff;

			} 

			// ###### Va sur la cible et reste au contact , tourne au tour de la cible ou passe dans son dos (exemple : felin) #####################################
			void Mouvement_Contact_Tournant_Aleatoire(uint32 diff)
			{
				if (!UpdateVictim())
					return;

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

					DoMeleeAttackIfReady();																	// Combat en mélée

				// ------ ALLER A LA CIBLE -------------------------------------------------------------------------------------------------------------------------
					if (Cooldown_Principal_A <= diff)
					{
						if (Dist >= 6)
						{
					me->GetMotionMaster()->MoveChase(victim, 2, urand(0, 6));								// Pour suivre la cible avec un angle
					Cooldown_Principal_A = Cooldown_Principal_A_Defaut;
						}
					}
				else Cooldown_Principal_A -= diff;

				// TOURNE AU TOUR , PASSE DANS LE DOS DE LA CIBLE OU BONDIT ----------------------------------------------------------------------------------------
				if (Cooldown_Principal_B <= diff)
				{
					if (Dist < 6)
					{
					Random = urand(1, 5);
					if (Random == 1 || Random == 2 || Random == 3)
					{
						Tourne_Au_Tour_Aleatoire(1);														// 3 chances sur 5 : tourne au tour de sa victime
					}
					else if (Random == 4)
					{
						Recule_ou_Avance(3);																// 1 chances sur 5 : avance
					}
					else if (Random == 5)
					{
						BOND_Aleatoire();																	// 1 chances sur 5 : Bond aléatoire
					}
					Cooldown_Principal_B = Cooldown_Principal_B_Defaut + ((urand(0, 2) * 1000));
					Cooldown_Principal_A = 3000;
					}
				}
				else Cooldown_Principal_B -= diff;

				// BONDIR ------------------------------------------------------------------------------------------------------------------------------------------
				if (Cooldown_Trop_Loin <= diff)
				{
					if (Dist >= 8 && Dist <= 25)
					{
					Random = urand(1, 2);
					if (Random == 1 && Spell_Trop_Loin != 0)
					{
						DoCastAOE(Spell_Trop_Loin, true); 
					}
					Cooldown_Trop_Loin = Cooldown_Trop_Loin_Defaut;
					}
				}
				else Cooldown_Trop_Loin -= diff;
			}

			// ###### Va sur la cible et reste au contact mais recule puis avance périodiquement sur cible en mélée, passe dans le dos pour les cibles caster ######
			void Mouvement_Contact_Prudent(uint32 diff)
			{
				if (!UpdateVictim())
					return;

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);


				// ------ ALLER A LA CIBLE -------------------------------------------------------------------------------------------------------------------------
				if (Cooldown_Principal_A <= diff)
				{
					if (Dist > 3)
					{
						//me->SetSpeedRate(MOVE_RUN, 1.01f);													// Vitesse de déplacement
						Tourne_Au_Tour_Aleatoire(2);
						DoMeleeAttackIfReady();																	// Combat en mélée
						Cooldown_Principal_B = Cooldown_Principal_B_Defaut;
						Cooldown_Principal_A = Cooldown_Principal_A_Defaut + ((urand(0, 2) * 1000));
					}
				}
				else Cooldown_Principal_A -= diff;

					// S'ELOIGNE OU PASSE DANS LE DOS --------------------------------------------------------------------------------------------------------------
				if (Cooldown_Principal_B <= diff)
				{
					if (Dist < 6) 
					{
						Random = urand(1, 5);
						if ( (Random == 1) && Player_Caster() == false )
						{
							Recule_ou_Avance(3);																// 3 chances sur 5 pour passe dans le dos de la cible
						}
						else 
						{
							Tourne_Au_Tour_Aleatoire(urand(8, 12));												// 2 chances sur 5 pour s'eloigner si le joueur n'est pas un caster
							Cooldown_Principal_A = 3000;
						}
						Cooldown_Principal_B = Cooldown_Principal_B_Defaut + ((urand(0, 2) * 1000));
					}
				}
					else Cooldown_Principal_B -= diff;




			}

			// ###### Va sur la cible et reste au contact mais s'envole puis revient en mélée sur la cible périodiquement  #########################################
			void Mouvement_Contact_Prudent_Volant(uint32 diff)
			{
				if (!UpdateVictim())
					return;

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);


				// ------ ALLER A LA CIBLE -------------------------------------------------------------------------------------------------------------------------
				if (Cooldown_Principal_A <= diff)
				{
					if (Dist > 3)
					{
						Tourne_Au_Tour_Aleatoire(3);
						DoMeleeAttackIfReady();																		// Combat en mélée
						Cooldown_Principal_B = Cooldown_Principal_B_Defaut;
						Cooldown_Principal_A = Cooldown_Principal_A_Defaut + ((urand(0, 2) * 1000));
					}
				}
				else Cooldown_Principal_A -= diff;

				// S'ELOIGNE --------------------------------------------------------------------------------------------------------------------------------------
				if (Cooldown_Principal_B <= diff)
				{
					if (Dist < 6)
					{
						Tourne_Au_Tour_Aleatoire_Volant( urand(6, 10) );
						Cooldown_Principal_A = 3000;
						Cooldown_Principal_B = Cooldown_Principal_B_Defaut + ((urand(0, 2) * 1000));
					}
				}
				else Cooldown_Principal_B -= diff;




			}

			// ###### Va sur la cible et reste au contact mais s'eloigne et "Charge périodiquement  ################################################################
			void Mouvement_Contact_Charges_Multiples(uint32 diff)
			{
				if (!UpdateVictim())
					return;

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				// ------ CHARGE -----------------------------------------------------------------------------------------------------------------------------------
				if (Cooldown_Trop_Loin <= diff)
				{
					if ( (Dist >= 8) && Dist <= 25)
					{
						if (Spell_Trop_Loin != 0)
						{
							DoCastAOE(Spell_Trop_Loin, true);
						}
					Cooldown_Principal_B = Cooldown_Principal_B_Defaut;
					Cooldown_Trop_Loin = Cooldown_Trop_Loin_Defaut;
					}
				}
				else Cooldown_Trop_Loin -= diff;



				// ------ ALLER A LA CIBLE -------------------------------------------------------------------------------------------------------------------------
				if (Cooldown_Principal_A <= diff)
				{
					if (Dist > 3)
					{
					Tourne_Au_Tour_Aleatoire(1);
					DoMeleeAttackIfReady();																	// Combat en mélée
					Cooldown_Principal_B = Cooldown_Principal_B_Defaut;
					Cooldown_Principal_A = Cooldown_Principal_A_Defaut;
					}
				}
				else Cooldown_Principal_A -= diff;



				// ------ S'ELOIGNE DE LA CIBLE ---------------------------------------------------------------------------------------------------------------------
				if (Cooldown_Principal_B <= diff)
				{
					if (Dist <= 3)
					{
					Tourne_Au_Tour_Aleatoire(15);
					Cooldown_Principal_A = 3000;
					Cooldown_Trop_Loin = 2000;
					Cooldown_Principal_B = Cooldown_Principal_B_Defaut;
					}
				}
				else Cooldown_Principal_B -= diff;




			}

			// ###### Va sur la cible et reste au contact , recule et revient périodiquement #######################################################################
			void Mouvement_Contact_Avance_Recule(uint32 diff)
			{
				if (!UpdateVictim())
					return;

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				DoMeleeAttackIfReady();																			// Combat en mélée

				// ------ ALLER A LA CIBLE ------------------------------------------------------------------------------------------------------------------------
				if (Cooldown_Principal_A <= diff)
				{
					if (Dist >= ResteADistance)
					{
						Tourne_Au_Tour_Aleatoire(1);
						Cooldown_Principal_A = Cooldown_Principal_A_Defaut;
					}
				}
				else Cooldown_Principal_A -= diff;

				// TOURNE AU TOUR , RECULE ------------------------------------------------------------------------------------------------------------------------
				if (Cooldown_Principal_B <= diff)
				{
					if (Dist < ResteADistance)
					{
						Random = urand(1, 2);
						if (Random == 1)
						{
							Tourne_Au_Tour_Aleatoire(2);														// 1 chances sur 2 : tourne au tour de sa victime
						}
						else 
						{
							Recule_ou_Avance( 1 - urand(5,11) );												// 1 chances sur 2 : Recule
						}
						Cooldown_Principal_B = Cooldown_Principal_B_Defaut;
						Cooldown_Principal_A = 2000 + urand(0,1500);
					}
				}
				else Cooldown_Principal_B -= diff;

				// VITESSE ------------------------------------------------------------------------------------------------------------------------------------------
				if (Cooldown_Trop_Loin <= diff)
				{
					if (Dist >= 8 && Dist <= 25)
					{
						Random = urand(1, 2);
						if (Random == 1 && Spell_Trop_Loin != 0)
						{
							//DoCastAOE(Spell_Trop_Loin, true);													// 1 chance sur 2 : vitesse
							me->CastSpell(me, Spell_Trop_Loin, true);											// 1 chance sur 2 : vitesse
						}
						Cooldown_Trop_Loin = Cooldown_Trop_Loin_Defaut;
					}
				}
				else Cooldown_Trop_Loin -= diff;
			}

			// ###### Va sur la cible et reste au contact , effectue un bond aléatoire régulierement ###############################################################
			void Mouvement_Contact_Bondissant(uint32 diff)
			{
				if (!UpdateVictim())
					return;

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				DoMeleeAttackIfReady();																		// Combat en mélée

				// ------ ALLER A LA CIBLE -------------------------------------------------------------------------------------------------------------------------
				if (Cooldown_Principal_A <= diff)
				{
					if (Dist >= 6)
					{
						me->GetMotionMaster()->MoveChase(victim, 1, urand(0, 6));								// Pour suivre la cible avec un angle
						Cooldown_Principal_A = Cooldown_Principal_A_Defaut;
					}
				}
				else Cooldown_Principal_A -= diff;

				// TOURNE AU TOUR DE LA CIBLE ----------------------------------------------------------------------------------------------------------------------
				if (Cooldown_Principal_B <= diff)
				{
					if (Dist < 6)
					{
						Random = urand(1, 3);
						if (Random == 1)
						{
							Tourne_Au_Tour_Aleatoire(2);														// 1 chances sur 3 : Tourne_Au_Tour_Aleatoire
						}
						else
						{
							BOND_Aleatoire();																	// 2 chances sur 3 : Bond aléatoire
						}
						Cooldown_Principal_B = Cooldown_Principal_B_Defaut;
					}
				}
				else Cooldown_Principal_B -= diff;

				// ------ CIBLE TROP LOIN : BONDIR ----------------------------------------------------------------------------------------------------------------
				if (Cooldown_Trop_Loin <= diff)
				{
					if (Dist >= 8 && Dist <= 15)
					{
						if (Spell_Trop_Loin != 0)
						{
							Random = urand(1, 2);
							if (Random == 1)
							{
								me->CastSpell(victim, Spell_Trop_Loin, true);								// 1 chances sur 2 : Charge/Bond...
							}
						}
						Cooldown_Trop_Loin = Cooldown_Trop_Loin_Defaut;
					}
				}
				else Cooldown_Trop_Loin -= diff;

			}


			// ------ AI SECONDAIRES ------
			void Heal_En_Combat(uint32 diff)
			{

				if (Cooldown_Spell_Heal <= diff && Spell_Heal != 0)
				{
					// heal sur lui meme----------------------------------------------------------------------------------------------------------------------------
					if ((me->GetHealth() < (me->GetMaxHealth()*0.60)))								// Si PV < 60%
					{
						//DoCastAOE(Spell_Heal);
						me->CastSpell(me, Spell_Heal, true);
						Cooldown_Spell_Heal = Cooldown_Spell_Heal_defaut;
					}
				}
				else Cooldown_Spell_Heal -= diff;
			}
			void Tourne_Au_Tour_Aleatoire(uint32 Distance)
			{
				if (!UpdateVictim())
					return;

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				float x, y, z, mapid;
				x = (victim->GetPositionX() + urand(0, Distance*2) - Distance);	// 4 pour contact
				y = (victim->GetPositionY() + urand(0, Distance*2) - Distance);	// 4 pour contact
				z = victim->GetPositionZ();
				mapid = victim->GetMapId();
				me->GetMotionMaster()->MovePoint(mapid, x, y, z);
				DoMeleeAttackIfReady();																	// Combat en mélée
			}
			void Tourne_Au_Tour_Aleatoire_Volant(uint32 Distance)
			{
				if (!UpdateVictim())
					return;

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				float x, y, z, mapid;
				x = (victim->GetPositionX() + urand(0, Distance * 2) - Distance);	// 4 pour contact
				y = (victim->GetPositionY() + urand(0, Distance * 2) - Distance);	// 4 pour contact
				z = victim->GetPositionZ() + urand(3,6);
				mapid = victim->GetMapId();
				me->GetMotionMaster()->MovePoint(mapid, x, y, z);
				DoMeleeAttackIfReady();																	// Combat en mélée
			}
			void BOND_Aleatoire() 
			{
				DoCastAOE(Spell_Bond_Aleatoire, true);
			}
			void Recule_ou_Avance(int32 val)
			{
				if (!UpdateVictim())
					return;

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				//float x, y, z, mapid;
				x = (victim->GetPositionX());
				y = (victim->GetPositionY());
				z = victim->GetPositionZ();
				mapid = victim->GetMapId();
				me->GetClosePoint(x, y, z, me->GetObjectSize() / 3, val);
				me->GetMotionMaster()->MovePoint(mapid, x, y, z);
			}
			void Senterre()
			{
				me->CastSpell(me, Spell_No_ModelID, true);										// Masque le ModelID
				me->CastSpell(me, Spell_Senterre, true);													// Fumée et terre remuée
				me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);						// Non selectionnable
			}
			void Se_Deterre()
			{
				me->RemoveAurasDueToSpell(Spell_Senterre);												// Retire fumée et terre remuée
				me->RemoveAurasDueToSpell(Spell_No_ModelID);									// Retire invisible
				me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);						// Selectionnable
			}

			// ------ DIVERS ------
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
			bool Player_Caster()
			{
				if (!UpdateVictim())
					return false;

				Unit* victim = me->GetVictim();

				if (victim->GetTypeId() != TYPEID_PLAYER)
					return false;

				// Retourne True pour un Joueur Caster sinon False
				uint32 Player_Spe = victim->GetUInt32Value(PLAYER_FIELD_CURRENT_SPEC_ID);
				if (Player_Spe == 62 || Player_Spe == 63 || Player_Spe == 64 || Player_Spe == 102 || Player_Spe == 105 || Player_Spe == 253 || Player_Spe == 254 || Player_Spe == 255 || Player_Spe == 256 || Player_Spe == 257 || Player_Spe == 258 || Player_Spe == 262 || Player_Spe == 264 || Player_Spe == 265 || Player_Spe == 266 || Player_Spe == 267 || Player_Spe == 539)
				{
					return true;
				} else return false;
			}



		};
		
		CreatureAI* GetAI(Creature* creature) const override
		{
			return new Stitch_npc_ai_mobsAI(creature);
		}
};

//#################################################################################################################################################################
void AddSC_npc_ai_mobs()
{
	new Stitch_npc_ai_mobs();
}















