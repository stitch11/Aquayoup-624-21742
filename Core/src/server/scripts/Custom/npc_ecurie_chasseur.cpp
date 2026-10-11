//##############################################################################################
//npc_ecurie_chasseur - PNJ 15100055 : Ecurie portable du chasseur (cage)
/*
-- FULL `creature_template` of entry 15100055
REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `HealthModifier`, `HealthModifierExtra`, `ManaModifier`, `ManaModifierExtra`, `ArmorModifier`, `DamageModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES
(15100055, 0, 0, 0, 0, 0, 32085, 0, 0, 0, 'Ecurie portable', '', '', '', 66007, 1, 1, 0, 0, 35, 1, 1, 1, 0.5, 0, 0, 2000, 2000, 1, 1, 1, 131074, 34816, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1.5, 1, 1, 1, 1.5, 1.5, 1, 0, 0, 1, 0, 0, 'npc_ecurie_chasseur', 20173);

REPLACE INTO `gossip_menu` (`entry`, `text_id`) VALUES (66007, 66007);
REPLACE INTO `npc_text` (`ID`, `Probability0`, `Probability1`, `Probability2`, `Probability3`, `Probability4`, `Probability5`, `Probability6`, `Probability7`, `BroadcastTextID0`, `BroadcastTextID1`, `BroadcastTextID2`, `BroadcastTextID3`, `BroadcastTextID4`, `BroadcastTextID5`, `BroadcastTextID6`, `BroadcastTextID7`, `VerifiedBuild`) VALUES ('66007', '1', '0', '0', '0', '0', '0', '0', '0', '15066007', '0', '0', '0', '0', '0', '0', '0', '0');
REPLACE INTO `broadcast_text` (`ID`, `Language`, `MaleText`, `FemaleText`, `EmoteID1`, `EmoteID2`, `EmoteID3`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `SoundID`, `UnkEmoteID`, `Type`, `VerifiedBuild`) VALUES ('15066007', '1', 'Information sur l\'écurie du chasseur :
--------------------------------------

L'écurie du chasseur permet de stocker son familier actif dans l'un des 10 emplacements d'écurie.

Le nombre d'emplacements dépend de votre level :
2 d'emplacements au level 5 puis 1 de plus chaque 5 levels.

*/
//##############################################################################################

#include "Pet.h"

//#include <iostream>
//#include <string>

// Doit etre placé avant les gossip
std::string InfoSlot(Player* player, uint8 slot);
std::string texticon;
uint32 GetPetFamily(uint32 petEntry);
char text[200];

class npc_ecurie_chasseur : public CreatureScript
{
public:
	npc_ecurie_chasseur() : CreatureScript("npc_ecurie_chasseur") {}

	bool OnGossipHello(Player* player, Creature* creature)
	{
		uint8 _level = player->getLevel();
		uint8 _class = player->getClass();

		if (_class == CLASS_HUNTER)
		{
			if (_level >= 5) {player->ADD_GOSSIP_ITEM(1, InfoSlot(player, 1), GOSSIP_SENDER_MAIN, 1);}
			if (_level >= 5) { player->ADD_GOSSIP_ITEM(1, InfoSlot(player, 2), GOSSIP_SENDER_MAIN, 2);}
			if (_level >= 10) { player->ADD_GOSSIP_ITEM(1, InfoSlot(player, 3), GOSSIP_SENDER_MAIN, 3);}
			if (_level >= 15) { player->ADD_GOSSIP_ITEM(1, InfoSlot(player, 4), GOSSIP_SENDER_MAIN, 4);}
			if (_level >= 20) { player->ADD_GOSSIP_ITEM(1, InfoSlot(player, 5), GOSSIP_SENDER_MAIN, 5);}
			if (_level >= 25) { player->ADD_GOSSIP_ITEM(1, InfoSlot(player, 6), GOSSIP_SENDER_MAIN, 6);}
			if (_level >= 30) { player->ADD_GOSSIP_ITEM(1, InfoSlot(player, 7), GOSSIP_SENDER_MAIN, 7);}
			if (_level >= 35) { player->ADD_GOSSIP_ITEM(1, InfoSlot(player, 8), GOSSIP_SENDER_MAIN, 8);}
			if (_level >= 40) { player->ADD_GOSSIP_ITEM(1, InfoSlot(player, 9), GOSSIP_SENDER_MAIN, 9);}
			if (_level >= 45) { player->ADD_GOSSIP_ITEM(1, InfoSlot(player, 10), GOSSIP_SENDER_MAIN, 10);}
		}
		player->PlayerTalkClass->SendGossipMenu(66007, creature->GetGUID());	//gossip_menu comprenant le npc_text
		return true;
	}


	bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
	{
		if (!player)
			return true;

		uint8 _class = player->getClass();
		uint8 _level = player->getLevel();
		Pet* pet = player->GetPet();
		

		SQLTransaction trans = CharacterDatabase.BeginTransaction();

		PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_PET_BY_SLOT);
		ObjectGuid::LowType ownerLowGUID = player->GetGUID().GetCounter();


		/*if (!pet)
		return true;*/

		switch (uiAction)
		{

		case 1: // Invoquer le familier du Box 1
		{
			uint8 slot = 1;

			Pet* oldPet = player->GetPet();
			uint32 oldPetNumber = 0;

			if (oldPet && oldPet->GetCharmInfo())
				oldPetNumber = oldPet->GetCharmInfo()->GetPetNumber();

			// 1. Verifier que le slot contient un familier
			PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_PET_BY_SLOT);
			stmt->setUInt64(0, player->GetGUID().GetCounter());
			stmt->setUInt8(1, slot);
			PreparedQueryResult result = CharacterDatabase.Query(stmt);

			if (!result)
			{
				// Le slot est vide : y ranger le familier courant.
				if (!oldPet || !oldPetNumber)
				{
					player->GetSession()->SendNotification(
						"Le slot %u est vide et aucun familier courant a ranger.", slot);
					player->PlayerTalkClass->SendCloseGossip();
					break;
				}

				// Sauvegarder le familier courant dans le slot choisi.
				SQLTransaction trans = CharacterDatabase.BeginTransaction();

				stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_PET_SLOT_BY_ID);
				stmt->setUInt8(0, slot);
				stmt->setUInt64(1, player->GetGUID().GetCounter());
				stmt->setUInt32(2, oldPetNumber);

				trans->Append(stmt);
				CharacterDatabase.CommitTransaction(trans);

				// Retirer le familier du monde.
				player->RemovePetFromWorld(oldPet);

				player->GetSession()->SendNotification(
					u8"Familier rangé dans le slot %u.", slot);

				player->PlayerTalkClass->SendCloseGossip();
				break;
			}


			// 2. Retirer l'ancien familier du monde sans le sauvegarder
			if (oldPet)
				player->RemovePetFromWorld(oldPet);

			// 3. Charger le familier du slot 
			Pet* newPet = new Pet(player, HUNTER_PET);

			if (!newPet->LoadPetFromDB(player, 0, 0, false, slot))
			{
				delete newPet;
				player->GetSession()->SendNotification("Impossible de charger le familier du slot %u.", slot);

				// Tenter de restaurer l'ancien familier
				if (oldPetNumber)
				{
					Pet* restorePet = new Pet(player, HUNTER_PET);
					if (!restorePet->LoadPetFromDB(player, 0, oldPetNumber, false, 0))
						delete restorePet;
				}

				player->PlayerTalkClass->SendCloseGossip();
				break;
			}

			// 4. Ranger l'ancien familier dans le slot 
			if (oldPetNumber)
			{
				SQLTransaction trans = CharacterDatabase.BeginTransaction();

				stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_PET_SLOT_BY_ID);

				stmt->setUInt8(0, slot);
				stmt->setUInt64(1, player->GetGUID().GetCounter());
				stmt->setUInt32(2, oldPetNumber);

				trans->Append(stmt);
				CharacterDatabase.CommitTransaction(trans);
			}

			player->GetSession()->SendNotification(u8"Echange de familier effectué avec le slot %u.", slot);
			creature->CastSpell(creature, 14867, true);
			player->PlayerTalkClass->SendCloseGossip();
			break;
		}
		case 2: // Invoquer le familier du Box 2
		{
			uint8 slot = 2;

			Pet* oldPet = player->GetPet();
			uint32 oldPetNumber = 0;

			if (oldPet && oldPet->GetCharmInfo())
				oldPetNumber = oldPet->GetCharmInfo()->GetPetNumber();

			// 1. Verifier que le slot contient un familier
			PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_PET_BY_SLOT);
			stmt->setUInt64(0, player->GetGUID().GetCounter());
			stmt->setUInt8(1, slot);
			PreparedQueryResult result = CharacterDatabase.Query(stmt);

			if (!result)
			{
				// Le slot est vide : y ranger le familier courant.
				if (!oldPet || !oldPetNumber)
				{
					player->GetSession()->SendNotification(
						"Le slot %u est vide et aucun familier courant a ranger.", slot);
					player->PlayerTalkClass->SendCloseGossip();
					break;
				}

				// Sauvegarder le familier courant dans le slot choisi.
				SQLTransaction trans = CharacterDatabase.BeginTransaction();

				stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_PET_SLOT_BY_ID);
				stmt->setUInt8(0, slot);
				stmt->setUInt64(1, player->GetGUID().GetCounter());
				stmt->setUInt32(2, oldPetNumber);

				trans->Append(stmt);
				CharacterDatabase.CommitTransaction(trans);

				// Retirer le familier du monde.
				player->RemovePetFromWorld(oldPet);

				player->GetSession()->SendNotification(
					u8"Familier rangé dans le slot %u.", slot);

				player->PlayerTalkClass->SendCloseGossip();
				break;
			}


			// 2. Retirer l'ancien familier du monde sans le sauvegarder
			if (oldPet)
				player->RemovePetFromWorld(oldPet);

			// 3. Charger le familier du slot 
			Pet* newPet = new Pet(player, HUNTER_PET);

			if (!newPet->LoadPetFromDB(player, 0, 0, false, slot))
			{
				delete newPet;
				player->GetSession()->SendNotification("Impossible de charger le familier du slot %u.", slot);

				// Tenter de restaurer l'ancien familier
				if (oldPetNumber)
				{
					Pet* restorePet = new Pet(player, HUNTER_PET);
					if (!restorePet->LoadPetFromDB(player, 0, oldPetNumber, false, 0))
						delete restorePet;
				}

				player->PlayerTalkClass->SendCloseGossip();
				break;
			}

			// 4. Ranger l'ancien familier dans le slot 
			if (oldPetNumber)
			{
				SQLTransaction trans = CharacterDatabase.BeginTransaction();

				stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_PET_SLOT_BY_ID);

				stmt->setUInt8(0, slot);
				stmt->setUInt64(1, player->GetGUID().GetCounter());
				stmt->setUInt32(2, oldPetNumber);

				trans->Append(stmt);
				CharacterDatabase.CommitTransaction(trans);
			}

			player->GetSession()->SendNotification(u8"Echange de familier effectué avec le slot %u.", slot);
			creature->CastSpell(creature, 14867, true);
			player->PlayerTalkClass->SendCloseGossip();
			break;
		}
		case 3: // Invoquer le familier du Box 3
		{
			uint8 slot = 3;

			Pet* oldPet = player->GetPet();
			uint32 oldPetNumber = 0;

			if (oldPet && oldPet->GetCharmInfo())
				oldPetNumber = oldPet->GetCharmInfo()->GetPetNumber();

			// 1. Verifier que le slot contient un familier
			PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_PET_BY_SLOT);
			stmt->setUInt64(0, player->GetGUID().GetCounter());
			stmt->setUInt8(1, slot);
			PreparedQueryResult result = CharacterDatabase.Query(stmt);

			if (!result)
			{
				// Le slot est vide : y ranger le familier courant.
				if (!oldPet || !oldPetNumber)
				{
					player->GetSession()->SendNotification(
						"Le slot %u est vide et aucun familier courant a ranger.", slot);
					player->PlayerTalkClass->SendCloseGossip();
					break;
				}

				// Sauvegarder le familier courant dans le slot choisi.
				SQLTransaction trans = CharacterDatabase.BeginTransaction();

				stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_PET_SLOT_BY_ID);
				stmt->setUInt8(0, slot);
				stmt->setUInt64(1, player->GetGUID().GetCounter());
				stmt->setUInt32(2, oldPetNumber);

				trans->Append(stmt);
				CharacterDatabase.CommitTransaction(trans);

				// Retirer le familier du monde.
				player->RemovePetFromWorld(oldPet);

				player->GetSession()->SendNotification(
					u8"Familier rangé dans le slot %u.", slot);

				player->PlayerTalkClass->SendCloseGossip();
				break;
			}


			// 2. Retirer l'ancien familier du monde sans le sauvegarder
			if (oldPet)
				player->RemovePetFromWorld(oldPet);

			// 3. Charger le familier du slot 
			Pet* newPet = new Pet(player, HUNTER_PET);

			if (!newPet->LoadPetFromDB(player, 0, 0, false, slot))
			{
				delete newPet;
				player->GetSession()->SendNotification("Impossible de charger le familier du slot %u.", slot);

				// Tenter de restaurer l'ancien familier
				if (oldPetNumber)
				{
					Pet* restorePet = new Pet(player, HUNTER_PET);
					if (!restorePet->LoadPetFromDB(player, 0, oldPetNumber, false, 0))
						delete restorePet;
				}

				player->PlayerTalkClass->SendCloseGossip();
				break;
			}

			// 4. Ranger l'ancien familier dans le slot 
			if (oldPetNumber)
			{
				SQLTransaction trans = CharacterDatabase.BeginTransaction();

				stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_PET_SLOT_BY_ID);

				stmt->setUInt8(0, slot);
				stmt->setUInt64(1, player->GetGUID().GetCounter());
				stmt->setUInt32(2, oldPetNumber);

				trans->Append(stmt);
				CharacterDatabase.CommitTransaction(trans);
			}

			player->GetSession()->SendNotification(u8"Echange de familier effectué avec le slot %u.", slot);
			creature->CastSpell(creature, 14867, true);
			player->PlayerTalkClass->SendCloseGossip();
			break;
		}
		case 4: // Invoquer le familier du Box 4
		{
			uint8 slot = 4;

			Pet* oldPet = player->GetPet();
			uint32 oldPetNumber = 0;

			if (oldPet && oldPet->GetCharmInfo())
				oldPetNumber = oldPet->GetCharmInfo()->GetPetNumber();

			// 1. Verifier que le slot contient un familier
			PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_PET_BY_SLOT);
			stmt->setUInt64(0, player->GetGUID().GetCounter());
			stmt->setUInt8(1, slot);
			PreparedQueryResult result = CharacterDatabase.Query(stmt);

			if (!result)
			{
				// Le slot est vide : y ranger le familier courant.
				if (!oldPet || !oldPetNumber)
				{
					player->GetSession()->SendNotification(
						"Le slot %u est vide et aucun familier courant a ranger.", slot);
					player->PlayerTalkClass->SendCloseGossip();
					break;
				}

				// Sauvegarder le familier courant dans le slot choisi.
				SQLTransaction trans = CharacterDatabase.BeginTransaction();

				stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_PET_SLOT_BY_ID);
				stmt->setUInt8(0, slot);
				stmt->setUInt64(1, player->GetGUID().GetCounter());
				stmt->setUInt32(2, oldPetNumber);

				trans->Append(stmt);
				CharacterDatabase.CommitTransaction(trans);

				// Retirer le familier du monde.
				player->RemovePetFromWorld(oldPet);

				player->GetSession()->SendNotification(
					u8"Familier rangé dans le slot %u.", slot);

				player->PlayerTalkClass->SendCloseGossip();
				break;
			}


			// 2. Retirer l'ancien familier du monde sans le sauvegarder
			if (oldPet)
				player->RemovePetFromWorld(oldPet);

			// 3. Charger le familier du slot 
			Pet* newPet = new Pet(player, HUNTER_PET);

			if (!newPet->LoadPetFromDB(player, 0, 0, false, slot))
			{
				delete newPet;
				player->GetSession()->SendNotification("Impossible de charger le familier du slot %u.", slot);

				// Tenter de restaurer l'ancien familier
				if (oldPetNumber)
				{
					Pet* restorePet = new Pet(player, HUNTER_PET);
					if (!restorePet->LoadPetFromDB(player, 0, oldPetNumber, false, 0))
						delete restorePet;
				}

				player->PlayerTalkClass->SendCloseGossip();
				break;
			}

			// 4. Ranger l'ancien familier dans le slot 
			if (oldPetNumber)
			{
				SQLTransaction trans = CharacterDatabase.BeginTransaction();

				stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_PET_SLOT_BY_ID);

				stmt->setUInt8(0, slot);
				stmt->setUInt64(1, player->GetGUID().GetCounter());
				stmt->setUInt32(2, oldPetNumber);

				trans->Append(stmt);
				CharacterDatabase.CommitTransaction(trans);
			}

			player->GetSession()->SendNotification(u8"Echange de familier effectué avec le slot %u.", slot);
			creature->CastSpell(creature, 14867, true);
			player->PlayerTalkClass->SendCloseGossip();
			break;
		}
		case 5: // Invoquer le familier du Box 5
		{
			uint8 slot = 5;

			Pet* oldPet = player->GetPet();
			uint32 oldPetNumber = 0;

			if (oldPet && oldPet->GetCharmInfo())
				oldPetNumber = oldPet->GetCharmInfo()->GetPetNumber();

			// 1. Verifier que le slot contient un familier
			PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_PET_BY_SLOT);
			stmt->setUInt64(0, player->GetGUID().GetCounter());
			stmt->setUInt8(1, slot);
			PreparedQueryResult result = CharacterDatabase.Query(stmt);

			if (!result)
			{
				// Le slot est vide : y ranger le familier courant.
				if (!oldPet || !oldPetNumber)
				{
					player->GetSession()->SendNotification(
						"Le slot %u est vide et aucun familier courant a ranger.", slot);
					player->PlayerTalkClass->SendCloseGossip();
					break;
				}

				// Sauvegarder le familier courant dans le slot choisi.
				SQLTransaction trans = CharacterDatabase.BeginTransaction();

				stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_PET_SLOT_BY_ID);
				stmt->setUInt8(0, slot);
				stmt->setUInt64(1, player->GetGUID().GetCounter());
				stmt->setUInt32(2, oldPetNumber);

				trans->Append(stmt);
				CharacterDatabase.CommitTransaction(trans);

				// Retirer le familier du monde.
				player->RemovePetFromWorld(oldPet);

				player->GetSession()->SendNotification(
					u8"Familier rangé dans le slot %u.", slot);

				player->PlayerTalkClass->SendCloseGossip();
				break;
			}


			// 2. Retirer l'ancien familier du monde sans le sauvegarder
			if (oldPet)
				player->RemovePetFromWorld(oldPet);

			// 3. Charger le familier du slot 
			Pet* newPet = new Pet(player, HUNTER_PET);

			if (!newPet->LoadPetFromDB(player, 0, 0, false, slot))
			{
				delete newPet;
				player->GetSession()->SendNotification("Impossible de charger le familier du slot %u.", slot);

				// Tenter de restaurer l'ancien familier
				if (oldPetNumber)
				{
					Pet* restorePet = new Pet(player, HUNTER_PET);
					if (!restorePet->LoadPetFromDB(player, 0, oldPetNumber, false, 0))
						delete restorePet;
				}

				player->PlayerTalkClass->SendCloseGossip();
				break;
			}

			// 4. Ranger l'ancien familier dans le slot 
			if (oldPetNumber)
			{
				SQLTransaction trans = CharacterDatabase.BeginTransaction();

				stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_PET_SLOT_BY_ID);

				stmt->setUInt8(0, slot);
				stmt->setUInt64(1, player->GetGUID().GetCounter());
				stmt->setUInt32(2, oldPetNumber);

				trans->Append(stmt);
				CharacterDatabase.CommitTransaction(trans);
			}

			player->GetSession()->SendNotification(u8"Echange de familier effectué avec le slot %u.", slot);
			creature->CastSpell(creature, 14867, true);
			player->PlayerTalkClass->SendCloseGossip();
			break;
		}
		case 6: // Invoquer le familier du Box 6
		{
			uint8 slot = 6;

			Pet* oldPet = player->GetPet();
			uint32 oldPetNumber = 0;

			if (oldPet && oldPet->GetCharmInfo())
				oldPetNumber = oldPet->GetCharmInfo()->GetPetNumber();

			// 1. Verifier que le slot 6 contient un familier
			PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_PET_BY_SLOT);
			stmt->setUInt64(0, player->GetGUID().GetCounter());
			stmt->setUInt8(1, slot);
			PreparedQueryResult result = CharacterDatabase.Query(stmt);

			if (!result)
			{
				// Le slot est vide : y ranger le familier courant.
				if (!oldPet || !oldPetNumber)
				{
					player->GetSession()->SendNotification(
						"Le slot %u est vide et aucun familier courant a ranger.", slot);
					player->PlayerTalkClass->SendCloseGossip();
					break;
				}

				// Sauvegarder le familier courant dans le slot choisi.
				SQLTransaction trans = CharacterDatabase.BeginTransaction();

				stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_PET_SLOT_BY_ID);
				stmt->setUInt8(0, slot);
				stmt->setUInt64(1, player->GetGUID().GetCounter());
				stmt->setUInt32(2, oldPetNumber);

				trans->Append(stmt);
				CharacterDatabase.CommitTransaction(trans);

				// Retirer le familier du monde.
				player->RemovePetFromWorld(oldPet);

				player->GetSession()->SendNotification(
					u8"Familier rangé dans le slot %u.", slot);

				player->PlayerTalkClass->SendCloseGossip();
				break;
			}


			// 2. Retirer l'ancien familier du monde sans le sauvegarder
			if (oldPet)
				player->RemovePetFromWorld(oldPet);

			// 3. Charger le familier du slot 6
			Pet* newPet = new Pet(player, HUNTER_PET);

			if (!newPet->LoadPetFromDB(player, 0, 0, false, slot))
			{
				delete newPet;
				player->GetSession()->SendNotification("Impossible de charger le familier du slot %u.", slot);

				// Tenter de restaurer l'ancien familier
				if (oldPetNumber)
				{
					Pet* restorePet = new Pet(player, HUNTER_PET);
					if (!restorePet->LoadPetFromDB(player, 0, oldPetNumber, false, 0))
						delete restorePet;
				}

				player->PlayerTalkClass->SendCloseGossip();
				break;
			}

			// 4. Ranger l'ancien familier dans le slot 6
			if (oldPetNumber)
			{
				SQLTransaction trans = CharacterDatabase.BeginTransaction();

				stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_PET_SLOT_BY_ID);

				stmt->setUInt8(0, slot);
				stmt->setUInt64(1, player->GetGUID().GetCounter());
				stmt->setUInt32(2, oldPetNumber);

				trans->Append(stmt);
				CharacterDatabase.CommitTransaction(trans);
			}

			player->GetSession()->SendNotification(u8"Echange de familier effectué avec le slot %u.", slot);
			creature->CastSpell(creature, 14867, true);
			player->PlayerTalkClass->SendCloseGossip();
			break;
		}
		case 7: // Invoquer le familier du Box 7
		{
			uint8 slot = 7;

			Pet* oldPet = player->GetPet();
			uint32 oldPetNumber = 0;

			if (oldPet && oldPet->GetCharmInfo())
				oldPetNumber = oldPet->GetCharmInfo()->GetPetNumber();

			// 1. Verifier que le slot contient un familier
			PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_PET_BY_SLOT);
			stmt->setUInt64(0, player->GetGUID().GetCounter());
			stmt->setUInt8(1, slot);
			PreparedQueryResult result = CharacterDatabase.Query(stmt);

			if (!result)
			{
				// Le slot est vide : y ranger le familier courant.
				if (!oldPet || !oldPetNumber)
				{
					player->GetSession()->SendNotification(
						"Le slot %u est vide et aucun familier courant a ranger.", slot);
					player->PlayerTalkClass->SendCloseGossip();
					break;
				}

				// Sauvegarder le familier courant dans le slot choisi.
				SQLTransaction trans = CharacterDatabase.BeginTransaction();

				stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_PET_SLOT_BY_ID);
				stmt->setUInt8(0, slot);
				stmt->setUInt64(1, player->GetGUID().GetCounter());
				stmt->setUInt32(2, oldPetNumber);

				trans->Append(stmt);
				CharacterDatabase.CommitTransaction(trans);

				// Retirer le familier du monde.
				player->RemovePetFromWorld(oldPet);

				player->GetSession()->SendNotification(
					u8"Familier rangé dans le slot %u.", slot);

				player->PlayerTalkClass->SendCloseGossip();
				break;
			}


			// 2. Retirer l'ancien familier du monde sans le sauvegarder
			if (oldPet)
				player->RemovePetFromWorld(oldPet);

			// 3. Charger le familier du slot 
			Pet* newPet = new Pet(player, HUNTER_PET);

			if (!newPet->LoadPetFromDB(player, 0, 0, false, slot))
			{
				delete newPet;
				player->GetSession()->SendNotification("Impossible de charger le familier du slot %u.", slot);

				// Tenter de restaurer l'ancien familier
				if (oldPetNumber)
				{
					Pet* restorePet = new Pet(player, HUNTER_PET);
					if (!restorePet->LoadPetFromDB(player, 0, oldPetNumber, false, 0))
						delete restorePet;
				}

				player->PlayerTalkClass->SendCloseGossip();
				break;
			}

			// 4. Ranger l'ancien familier dans le slot 
			if (oldPetNumber)
			{
				SQLTransaction trans = CharacterDatabase.BeginTransaction();

				stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_PET_SLOT_BY_ID);

				stmt->setUInt8(0, slot);
				stmt->setUInt64(1, player->GetGUID().GetCounter());
				stmt->setUInt32(2, oldPetNumber);

				trans->Append(stmt);
				CharacterDatabase.CommitTransaction(trans);
			}

			player->GetSession()->SendNotification(u8"Echange de familier effectué avec le slot %u.", slot);
			creature->CastSpell(creature, 14867, true);
			player->PlayerTalkClass->SendCloseGossip();
			break;
		}
		case 8: // Invoquer le familier du Box 8
		{
			uint8 slot = 8;

			Pet* oldPet = player->GetPet();
			uint32 oldPetNumber = 0;

			if (oldPet && oldPet->GetCharmInfo())
				oldPetNumber = oldPet->GetCharmInfo()->GetPetNumber();

			// 1. Verifier que le slot contient un familier
			PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_PET_BY_SLOT);
			stmt->setUInt64(0, player->GetGUID().GetCounter());
			stmt->setUInt8(1, slot);
			PreparedQueryResult result = CharacterDatabase.Query(stmt);

			if (!result)
			{
				// Le slot est vide : y ranger le familier courant.
				if (!oldPet || !oldPetNumber)
				{
					player->GetSession()->SendNotification(
						"Le slot %u est vide et aucun familier courant a ranger.", slot);
					player->PlayerTalkClass->SendCloseGossip();
					break;
				}

				// Sauvegarder le familier courant dans le slot choisi.
				SQLTransaction trans = CharacterDatabase.BeginTransaction();

				stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_PET_SLOT_BY_ID);
				stmt->setUInt8(0, slot);
				stmt->setUInt64(1, player->GetGUID().GetCounter());
				stmt->setUInt32(2, oldPetNumber);

				trans->Append(stmt);
				CharacterDatabase.CommitTransaction(trans);

				// Retirer le familier du monde.
				player->RemovePetFromWorld(oldPet);

				player->GetSession()->SendNotification(
					u8"Familier rangé dans le slot %u.", slot);

				player->PlayerTalkClass->SendCloseGossip();
				break;
			}


			// 2. Retirer l'ancien familier du monde sans le sauvegarder
			if (oldPet)
				player->RemovePetFromWorld(oldPet);

			// 3. Charger le familier du slot 
			Pet* newPet = new Pet(player, HUNTER_PET);

			if (!newPet->LoadPetFromDB(player, 0, 0, false, slot))
			{
				delete newPet;
				player->GetSession()->SendNotification("Impossible de charger le familier du slot %u.", slot);

				// Tenter de restaurer l'ancien familier
				if (oldPetNumber)
				{
					Pet* restorePet = new Pet(player, HUNTER_PET);
					if (!restorePet->LoadPetFromDB(player, 0, oldPetNumber, false, 0))
						delete restorePet;
				}

				player->PlayerTalkClass->SendCloseGossip();
				break;
			}

			// 4. Ranger l'ancien familier dans le slot 
			if (oldPetNumber)
			{
				SQLTransaction trans = CharacterDatabase.BeginTransaction();

				stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_PET_SLOT_BY_ID);

				stmt->setUInt8(0, slot);
				stmt->setUInt64(1, player->GetGUID().GetCounter());
				stmt->setUInt32(2, oldPetNumber);

				trans->Append(stmt);
				CharacterDatabase.CommitTransaction(trans);
			}

			player->GetSession()->SendNotification(u8"Echange de familier effectué avec le slot %u.", slot);
			creature->CastSpell(creature, 14867, true);
			player->PlayerTalkClass->SendCloseGossip();
			break;
		}
		case 9: // Invoquer le familier du Box 9
		{
			uint8 slot = 9;

			Pet* oldPet = player->GetPet();
			uint32 oldPetNumber = 0;

			if (oldPet && oldPet->GetCharmInfo())
				oldPetNumber = oldPet->GetCharmInfo()->GetPetNumber();

			// 1. Verifier que le slot contient un familier
			PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_PET_BY_SLOT);
			stmt->setUInt64(0, player->GetGUID().GetCounter());
			stmt->setUInt8(1, slot);
			PreparedQueryResult result = CharacterDatabase.Query(stmt);

			if (!result)
			{
				// Le slot est vide : y ranger le familier courant.
				if (!oldPet || !oldPetNumber)
				{
					player->GetSession()->SendNotification(
						"Le slot %u est vide et aucun familier courant a ranger.", slot);
					player->PlayerTalkClass->SendCloseGossip();
					break;
				}

				// Sauvegarder le familier courant dans le slot choisi.
				SQLTransaction trans = CharacterDatabase.BeginTransaction();

				stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_PET_SLOT_BY_ID);
				stmt->setUInt8(0, slot);
				stmt->setUInt64(1, player->GetGUID().GetCounter());
				stmt->setUInt32(2, oldPetNumber);

				trans->Append(stmt);
				CharacterDatabase.CommitTransaction(trans);

				// Retirer le familier du monde.
				player->RemovePetFromWorld(oldPet);

				player->GetSession()->SendNotification(
					u8"Familier rangé dans le slot %u.", slot);

				player->PlayerTalkClass->SendCloseGossip();
				break;
			}


			// 2. Retirer l'ancien familier du monde sans le sauvegarder
			if (oldPet)
				player->RemovePetFromWorld(oldPet);

			// 3. Charger le familier du slot 
			Pet* newPet = new Pet(player, HUNTER_PET);

			if (!newPet->LoadPetFromDB(player, 0, 0, false, slot))
			{
				delete newPet;
				player->GetSession()->SendNotification("Impossible de charger le familier du slot %u.", slot);

				// Tenter de restaurer l'ancien familier
				if (oldPetNumber)
				{
					Pet* restorePet = new Pet(player, HUNTER_PET);
					if (!restorePet->LoadPetFromDB(player, 0, oldPetNumber, false, 0))
						delete restorePet;
				}

				player->PlayerTalkClass->SendCloseGossip();
				break;
			}

			// 4. Ranger l'ancien familier dans le slot 
			if (oldPetNumber)
			{
				SQLTransaction trans = CharacterDatabase.BeginTransaction();

				stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_PET_SLOT_BY_ID);

				stmt->setUInt8(0, slot);
				stmt->setUInt64(1, player->GetGUID().GetCounter());
				stmt->setUInt32(2, oldPetNumber);

				trans->Append(stmt);
				CharacterDatabase.CommitTransaction(trans);
			}

			player->GetSession()->SendNotification(u8"Echange de familier effectué avec le slot %u.", slot);
			creature->CastSpell(creature, 14867, true);
			player->PlayerTalkClass->SendCloseGossip();
			break;
		}
		case 10: // Invoquer le familier du Box 9
		{
			uint8 slot = 10;

			Pet* oldPet = player->GetPet();
			uint32 oldPetNumber = 0;

			if (oldPet && oldPet->GetCharmInfo())
				oldPetNumber = oldPet->GetCharmInfo()->GetPetNumber();

			// 1. Verifier que le slot contient un familier
			PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_PET_BY_SLOT);
			stmt->setUInt64(0, player->GetGUID().GetCounter());
			stmt->setUInt8(1, slot);
			PreparedQueryResult result = CharacterDatabase.Query(stmt);

			if (!result)
			{
				// Le slot est vide : y ranger le familier courant.
				if (!oldPet || !oldPetNumber)
				{
					player->GetSession()->SendNotification(
						"Le slot %u est vide et aucun familier courant a ranger.", slot);
					player->PlayerTalkClass->SendCloseGossip();
					break;
				}

				// Sauvegarder le familier courant dans le slot choisi.
				SQLTransaction trans = CharacterDatabase.BeginTransaction();

				stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_PET_SLOT_BY_ID);
				stmt->setUInt8(0, slot);
				stmt->setUInt64(1, player->GetGUID().GetCounter());
				stmt->setUInt32(2, oldPetNumber);

				trans->Append(stmt);
				CharacterDatabase.CommitTransaction(trans);

				// Retirer le familier du monde.
				player->RemovePetFromWorld(oldPet);

				player->GetSession()->SendNotification(
					u8"Familier rangé dans le slot %u.", slot);

				player->PlayerTalkClass->SendCloseGossip();
				break;
			}


			// 2. Retirer l'ancien familier du monde sans le sauvegarder
			if (oldPet)
				player->RemovePetFromWorld(oldPet);

			// 3. Charger le familier du slot 
			Pet* newPet = new Pet(player, HUNTER_PET);

			if (!newPet->LoadPetFromDB(player, 0, 0, false, slot))
			{
				delete newPet;
				player->GetSession()->SendNotification("Impossible de charger le familier du slot %u.", slot);

				// Tenter de restaurer l'ancien familier
				if (oldPetNumber)
				{
					Pet* restorePet = new Pet(player, HUNTER_PET);
					if (!restorePet->LoadPetFromDB(player, 0, oldPetNumber, false, 0))
						delete restorePet;
				}

				player->PlayerTalkClass->SendCloseGossip();
				break;
			}

			// 4. Ranger l'ancien familier dans le slot 
			if (oldPetNumber)
			{
				SQLTransaction trans = CharacterDatabase.BeginTransaction();

				stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_PET_SLOT_BY_ID);

				stmt->setUInt8(0, slot);
				stmt->setUInt64(1, player->GetGUID().GetCounter());
				stmt->setUInt32(2, oldPetNumber);

				trans->Append(stmt);
				CharacterDatabase.CommitTransaction(trans);
			}

			player->GetSession()->SendNotification(u8"Echange de familier effectué avec le slot %u.", slot);
			creature->CastSpell(creature, 14867, true);
			player->PlayerTalkClass->SendCloseGossip();
			break;
		}



		default:
			break;
		}


		return true;
	}

};






// Retourne dans une variable chaine un icon Gossip selon la famille du familier suivi du nom et du level
std::string InfoSlot(Player* player, uint8 slot)
{
	if (!player)
		return "--- Joueur invalide ---";

	QueryResult result = CharacterDatabase.PQuery("SELECT id, entry, name, level, slot, owner ""FROM character_pet ""WHERE owner = '%u' AND slot = '%u'",player->GetGUID().GetCounter(), uint32(slot));

	if (!result)
	{
		//snprintf(text, sizeof(text), "Aucun familier dans le Box %u", uint32(slot));
		snprintf(text, sizeof(text), "|TInterface\\Icons\\_cadre-vide:48 : 48|t Aucun familier dans le Box %u", uint32(slot));
		return std::string(text);
	}

	Field* fields = result->Fetch();

	// Entry du familier enregistré dans ce slot
	uint32 petEntry = fields[1].GetUInt32();

	// Famille correspondant à ce familier
	uint32 familyId = GetPetFamily(petEntry);

	std::string petName = fields[2].GetString();
	uint32 petLevel = fields[3].GetUInt32();

	switch (familyId)
	{
	case 1:
	{
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Wolf:48 : 48|t  %s - Lvl %u";
		break;//Loup Wolf
	}
	case 2:
	{
		texticon = "|TInterface\\Icons\\ability_druid_skinteeth:48 : 48|t  %s - Lvl %u";
		break;//Felin cat
	}
	case 3:
	{
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Spider:48 : 48|t  %s - Lvl %u";
		break;//Araignée Spider
	}
	case 4:
	{
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Bear:48 : 48|t  %s - Lvl %u";
		break;//Ours Bear
	}
	case 5:
	{
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Boar:48 : 48|t  %s - Lvl %u";
		break;//Sanglier Boar
	}
	case 6:
	{
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Crocolisk:48 : 48|t  %s - Lvl %u";
		break;//Crocodile Crocolisk
	}
	case 7:
	{
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Vulture:48 : 48|t  %s - Lvl %u";
		break;//Oiseau charogniar   Carrion Bird
	}
	case 8:
	{
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Crab:48 : 48|t  %s - Lvl %u";
		break;//Crabe Crab
	}
	case 9:
	{
		texticon = "|TInterface\\Icons\\ability_hunter_pet_gorilla:48 : 48|t  %s - Lvl %u";
		break;//Gorille Gorilla
	}
	case 10:
	{
		texticon = "|TInterface\\Icons\\ability_physical_taunt:48 : 48|t  %s - Lvl %u";
		break;//Custom
	}
	case 11:
	{
		texticon = "|TInterface\\Icons\\ability_hunter_pet_raptor:48 : 48|t  %s - Lvl %u";
		break;//Raptor Raptor
	}
	case 12:
	{
		texticon = "|TInterface\\Icons\\ability_hunter_pet_tallstrider:48 : 48|t  %s - Lvl %u";
		break;// Grand trotteur   Tallstrider
	}
	case 20:
	{
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Scorpid:48 : 48|t  %s - Lvl %u";
		break;//Scorpion  Scorpid
	}
	case 21:
	{
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Turtle:48 : 48|t  %s - Lvl %u";
		break;// Tortue Turtle
	}
	case 24:
	{
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Bat:48 : 48|t  %s - Lvl %u";
		break;// Chauve souris  Bat
	}
	case 25:
	{
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Hyena:48 : 48|t  %s - Lvl %u";
		break;//Hyene  Hyena
	}
	case 26:
	{
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Owl:48 : 48|t  %s - Lvl %u";
		break;//Oiseau de proie   Bird of Prey
	}
	case 27:
	{
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_WindSerpent:48 : 48|t  %s - Lvl %u";
		break;// Serpent des vents  Wind Serpent
	}
	case 30:
	{
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_DragonHawk:48 : 48|t  %s - Lvl %u";
		break;// Faucon dragon  Dragonhawk
	}
	case 31:
	{
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Ravager:48 : 48|t  %s - Lvl %u";
		break;//Ravageur  Ravager
	}
	case 32:
	{
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_WarpStalker:48 : 48|t  %s - Lvl %u";
		break;//Traqueur de chaine    WarpStalker
	}
	case 33:
	{
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Sporebat:48 : 48|t  %s - Lvl %u";
		break;// Sporebat  
	}
	case 34:
	{
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_NetherRay:48 : 48|t  %s - Lvl %u";
		break;//Raie du neant   Nether Ray
	}
	case 35:
	{
		texticon = "|TInterface\\Icons\\Spell_Nature_GuardianWard:48 : 48|t  %s - Lvl %u";
		break;//Serpent
	}
	case 37:
	{
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Moth:48 : 48|t  %s - Lvl %u";
		break;//Papillon de nuit   Moth
	}
	case 38:
	{
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Chimera:48 : 48|t  %s - Lvl %u";
		break;//Chimere  Chimaera
	}
	case 39:
	{
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Devilsaur:48 : 48|t  %s - Lvl %u";
		break;//Diablosaure  Devilsaur
	}
	case 41:
	{
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Silithid:48 : 48|t  %s - Lvl %u";
		break;//Silithide  Silithid
	}
	case 42:
	{
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Worm:48 : 48|t  %s - Lvl %u";
		break;//Ver  Worm
	}
	case 43:
	{
		texticon = "|TInterface\\Icons\\ability_hunter_pet_rhino:48 : 48|t  %s - Lvl %u";
		break;// Rhinoceros/Kodo Rhino
	}
	case 44:
	{
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Wasp:48 : 48|t  %s - Lvl %u";
		break;//Guepe  Wasp
	}
	case 45:
	{
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_CoreHound:48 : 48|t  %s - Lvl %u";
		break;// Chien du magma   Core Hound
	}
	case 50:
	{
		texticon = "|TInterface\\Icons\\ability_hunter_aspectofthefox:48 : 48|t  %s - Lvl %u";
		break;//Renard Fox
	}
	case 51:
	{
		texticon = "|TInterface\\Icons\\inv_pet_monkey:48 : 48|t  %s - Lvl %u";
		break;//Singe  Monkey
	}
	case 52:
	{
		texticon = "|TInterface\\Icons\\inv_pet_mastiff:48 : 48|t  %s - Lvl %u";
		break;//Chien  Dog
	}
	case 53:
	{
		texticon = "|TInterface\\Icons\\inv_misc_ahnqirajtrinket_01:48 : 48|t  %s - Lvl %u";
		break;//Hanneton   
	}
	case 55:
	{
		texticon = "|TInterface\\Icons\\inv_pet_ shalespider:48 : 48|t  %s - Lvl %u";
		break;//Araignee de schiste  Shale Spider
	}
	case 68:
	{
		texticon = "|TInterface\\Icons\\rade_archaeology_whitehydrafigurine:48 : 48|t  %s - Lvl %u";
		break;//Hydre  Hydra
	}
	case 125:
	{
		texticon = "|TInterface\\Icons\\inv_pet_cranegod:48 : 48|t  %s - Lvl %u";
		break;//Grue  Crane
	}
	case 126:
	{
		texticon = "|TInterface\\Icons\\inv_pet_waterstrider:48 : 48|t  %s - Lvl %u";
		break;//Trotteur aquatique  Water Strider
	}
	case 127:
	{
		texticon = "|TInterface\\Icons\\inv_pet_porcupine:48 : 48|t  %s - Lvl %u";
		break;//Porc epic  Porcupine
	}
	case 128:
	{
		texticon = "|TInterface\\Icons\\achievement_moguraid_01:48 : 48|t  %s - Lvl %u";
		break;
	}
	case 129:
	{
		texticon = "|TInterface\\Icons\\inv_pet_ goat:48 : 48|t  %s - Lvl %u";
		break;//Chevre/Bouc   Goat
	}
	case 130:
	{
		texticon = "|TInterface\\Icons\\inv_pet_ basilisk:48 : 48|t  %s - Lvl %u";
		break;//Basolic  Basilisk
	}
	case 138:
	{
		texticon = "|TInterface\\Icons\\inv_pet_direhorn:48 : 48|t  %s - Lvl %u";
		break;//navrecorne   direhorn
	}
	case 149:
	{
		texticon = "|TInterface\\Icons\\ability_mount_ironchimera:48 : 48|t  %s - Lvl %u";
		break;//Rylak 
	}
	case 150:
	{
		texticon = "|TInterface\\Icons\\inv_hippo_green:48 : 48|t  %s - Lvl %u";
		break;//Hippopotame  Potamodonte 
	}
	case 151:
	{
		texticon = "|TInterface\\Icons\\inv_misc_herb_goldthorn_bramble:48 : 48|t  %s - Lvl %u";
		break;//Cerf Stag
	}


	default:
		texticon = "|TInterface\\Icons\\achievement_guildperk_fasttrack:48 : 48|t  %s - Lvl %u";
		break;

	}

	snprintf(text, sizeof(text), texticon.c_str(), petName.c_str(), petLevel);

	return std::string(text);
}

// Retourne le family familier : felin, raptor, etc
uint32 GetPetFamily(uint32 petEntry)
{
	CreatureTemplate const* cinfo = sObjectMgr->GetCreatureTemplate(petEntry);
	if (!cinfo)
		return 0;

	CreatureFamilyEntry const* cFamily = sCreatureFamilyStore.LookupEntry(cinfo->family);
	if (!cFamily)
		return 0;

	return cinfo->family;
}












void AddSC_npc_ecurie_chasseur()
{
	new npc_ecurie_chasseur();
}
