
//##############################################################################################
// npc_ecurie_chasseur - PNJ 15100055 : Ecurie portable du chasseur
// TrinityCore 6.2.4 - Build 21742
// Traductions : frFR / enUS / esES / deDE
//##############################################################################################

#include "Pet.h"
#include "world.h"
#include "Config.h"

#include <string>
#include <cstdio>

//##############################################################################################
// DECLARATIONS
//##############################################################################################

std::string InfoSlot(Player* player, uint8 slot);
uint32 GetPetFamily(uint32 petEntry);

static std::string texticon;
static char text[200];

//##############################################################################################
// LANGUE
//##############################################################################################

static std::string GetPetStableLanguage()
{
	// Conserver ici la syntaxe d'accès à sConfigMgr qui compile dans ton fork.
	return sConfigMgr->GetStringDefault("ForceClientLangue", "frFR");
}

//##############################################################################################
// MESSAGES TRADUITS
//##############################################################################################

enum PetStableMessage
{
	PET_STABLE_EMPTY = 1,
	PET_STABLE_STORED,
	PET_STABLE_LOAD_ERROR,
	PET_STABLE_EXCHANGE
};

static void SendPetStableNotification(Player* player, uint8 message, uint8 slot)
{
	if (!player)
		return;

	std::string lang = GetPetStableLanguage();
	const char* messageText = "";

	switch (message)
	{
	case PET_STABLE_EMPTY:
	{
		if (lang == "enUS")
			messageText = "Slot %u is empty and there is no current pet to store.";
		else if (lang == "esES")
			messageText = "El espacio %u esta vacio y no hay una mascota actual que guardar.";
		else if (lang == "deDE")
			messageText = "Platz %u ist leer und es gibt kein aktuelles Tier zum Einlagern.";
		else
			messageText = u8"Le slot %u est vide et aucun familier courant à ranger.";
		break;
	}

	case PET_STABLE_STORED:
	{
		if (lang == "enUS")
			messageText = "Pet stored in slot %u.";
		else if (lang == "esES")
			messageText = "Mascota guardada en el espacio %u.";
		else if (lang == "deDE")
			messageText = "Begleiter in Platz %u eingelagert.";
		else
			messageText = u8"Familier rangé dans le slot %u.";
		break;
	}

	case PET_STABLE_LOAD_ERROR:
	{
		if (lang == "enUS")
			messageText = "Unable to load the pet from slot %u.";
		else if (lang == "esES")
			messageText = "No se pudo cargar la mascota del espacio %u.";
		else if (lang == "deDE")
			messageText = "Der Begleiter aus Platz %u konnte nicht geladen werden.";
		else
			messageText = u8"Impossible de charger le familier du slot %u.";
		break;
	}

	case PET_STABLE_EXCHANGE:
	{
		if (lang == "enUS")
			messageText = "Pet exchange completed with slot %u.";
		else if (lang == "esES")
			messageText = "Intercambio de mascota realizado con el espacio %u.";
		else if (lang == "deDE")
			messageText = "Begleiter erfolgreich mit Platz %u ausgetauscht.";
		else
			messageText = u8"Echange de familier effectué avec le slot %u.";
		break;
	}

	default:
		return;
	}

	player->GetSession()->SendNotification(messageText, slot);
}

//##############################################################################################
// NPC
//##############################################################################################

class npc_ecurie_chasseur : public CreatureScript
{
public:

	npc_ecurie_chasseur() : CreatureScript("npc_ecurie_chasseur") {}

	//------------------------------------------------------------------------------------------
	// Ouverture du menu
	//------------------------------------------------------------------------------------------

	bool OnGossipHello(Player* player, Creature* creature)
	{
		if (!player)
			return true;

		uint8 level = player->getLevel();
		uint8 playerClass = player->getClass();

		if (playerClass == CLASS_HUNTER)
		{
			if (level >= 5)
				player->ADD_GOSSIP_ITEM(1, InfoSlot(player, 1).c_str(), GOSSIP_SENDER_MAIN, 1);

			if (level >= 5)
				player->ADD_GOSSIP_ITEM(1, InfoSlot(player, 2).c_str(), GOSSIP_SENDER_MAIN, 2);

			if (level >= 10)
				player->ADD_GOSSIP_ITEM(1, InfoSlot(player, 3).c_str(), GOSSIP_SENDER_MAIN, 3);

			if (level >= 15)
				player->ADD_GOSSIP_ITEM(1, InfoSlot(player, 4).c_str(), GOSSIP_SENDER_MAIN, 4);

			if (level >= 20)
				player->ADD_GOSSIP_ITEM(1, InfoSlot(player, 5).c_str(), GOSSIP_SENDER_MAIN, 5);

			if (level >= 25)
				player->ADD_GOSSIP_ITEM(1, InfoSlot(player, 6).c_str(), GOSSIP_SENDER_MAIN, 6);

			if (level >= 30)
				player->ADD_GOSSIP_ITEM(1, InfoSlot(player, 7).c_str(), GOSSIP_SENDER_MAIN, 7);

			if (level >= 35)
				player->ADD_GOSSIP_ITEM(1, InfoSlot(player, 8).c_str(), GOSSIP_SENDER_MAIN, 8);

			if (level >= 40)
				player->ADD_GOSSIP_ITEM(1, InfoSlot(player, 9).c_str(), GOSSIP_SENDER_MAIN, 9);

			if (level >= 45)
				player->ADD_GOSSIP_ITEM(1, InfoSlot(player, 10).c_str(), GOSSIP_SENDER_MAIN, 10);
		}

		// Le texte de présentation de ce menu provient de npc_text /
		// broadcast_text, et non de ce fichier C++.
		player->PlayerTalkClass->SendGossipMenu(66007, creature->GetGUID());

		return true;
	}

	//------------------------------------------------------------------------------------------
	// Sélection d'un slot
	//------------------------------------------------------------------------------------------

	bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
	{
		if (!player)
			return true;

		if (player->getClass() != CLASS_HUNTER)
		{
			player->PlayerTalkClass->SendCloseGossip();
			return true;
		}

		// Les actions 1 à 10 correspondent aux slots 1 à 10.
		if (uiAction < 1 || uiAction > 10)
		{
			player->PlayerTalkClass->SendCloseGossip();
			return true;
		}

		uint8 slot = uint8(uiAction);

		// Récupération du familier actuellement invoqué.
		Pet* oldPet = player->GetPet();
		uint32 oldPetNumber = 0;

		if (oldPet && oldPet->GetCharmInfo())
			oldPetNumber = oldPet->GetCharmInfo()->GetPetNumber();

		//--------------------------------------------------------------------------------------
		// 1. Vérifier si le slot contient déjà un familier
		//--------------------------------------------------------------------------------------

		PreparedStatement* stmt =
			CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_PET_BY_SLOT);

		stmt->setUInt64(0, player->GetGUID().GetCounter());
		stmt->setUInt8(1, slot);

		PreparedQueryResult result = CharacterDatabase.Query(stmt);

		//--------------------------------------------------------------------------------------
		// 2. Si le slot est vide, y ranger le familier courant
		//--------------------------------------------------------------------------------------

		if (!result)
		{
			// Aucun familier courant à ranger.
			if (!oldPet || !oldPetNumber)
			{
				SendPetStableNotification(player, PET_STABLE_EMPTY, slot);
				player->PlayerTalkClass->SendCloseGossip();
				return true;
			}

			SQLTransaction trans = CharacterDatabase.BeginTransaction();

			stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_PET_SLOT_BY_ID);

			stmt->setUInt8(0, slot);
			stmt->setUInt64(1, player->GetGUID().GetCounter());
			stmt->setUInt32(2, oldPetNumber);

			trans->Append(stmt);
			CharacterDatabase.CommitTransaction(trans);

			// Retirer le familier du monde.
			player->RemovePetFromWorld(oldPet);

			SendPetStableNotification(player, PET_STABLE_STORED, slot);

			player->PlayerTalkClass->SendCloseGossip();
			return true;
		}

		//--------------------------------------------------------------------------------------
		// 3. Retirer l'ancien familier du monde
		//--------------------------------------------------------------------------------------

		if (oldPet)
			player->RemovePetFromWorld(oldPet);

		//--------------------------------------------------------------------------------------
		// 4. Charger le familier enregistré dans le slot sélectionné
		//--------------------------------------------------------------------------------------

		Pet* newPet = new Pet(player, HUNTER_PET);

		if (!newPet->LoadPetFromDB(player, 0, 0, false, slot))
		{
			delete newPet;

			SendPetStableNotification(player, PET_STABLE_LOAD_ERROR, slot);

			// Tenter de restaurer l'ancien familier.
			if (oldPetNumber)
			{
				Pet* restorePet = new Pet(player, HUNTER_PET);

				if (!restorePet->LoadPetFromDB(player, 0, oldPetNumber, false, 0))
					delete restorePet;
			}

			player->PlayerTalkClass->SendCloseGossip();
			return true;
		}

		//--------------------------------------------------------------------------------------
		// 5. Ranger l'ancien familier dans le slot sélectionné
		//--------------------------------------------------------------------------------------

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

		//--------------------------------------------------------------------------------------
		// 6. Finaliser l'échange
		//--------------------------------------------------------------------------------------

		SendPetStableNotification(player, PET_STABLE_EXCHANGE, slot);

		creature->CastSpell(creature, 14867, true);

		player->PlayerTalkClass->SendCloseGossip();

		return true;
	}
};

//##############################################################################################
// AFFICHAGE DES SLOTS
//##############################################################################################

std::string InfoSlot(Player* player, uint8 slot)
{
	if (!player)
		return "--- Joueur invalide ---";

	QueryResult result = CharacterDatabase.PQuery(
		"SELECT id, entry, name, level, slot, owner "
		"FROM character_pet "
		"WHERE owner = '%u' AND slot = '%u'",
		player->GetGUID().GetCounter(),
		uint32(slot)
	);

	std::string lang = GetPetStableLanguage();

	//------------------------------------------------------------------------------------------
	// Slot vide
	//------------------------------------------------------------------------------------------

	if (!result)
	{
		const char* emptyFormat = "";

		if (lang == "enUS")
			emptyFormat = "|TInterface\\Icons\\_cadre-vide:48 : 48|t No pet in slot %u";
		else if (lang == "esES")
			emptyFormat = "|TInterface\\Icons\\_cadre-vide:48 : 48|t No hay mascota en el espacio %u";
		else if (lang == "deDE")
			emptyFormat = "|TInterface\\Icons\\_cadre-vide:48 : 48|t Kein Begleiter in Platz %u";
		else
			emptyFormat = "|TInterface\\Icons\\_cadre-vide:48 : 48|t Aucun familier dans le slot %u";

		snprintf(text, sizeof(text), emptyFormat, uint32(slot));

		return std::string(text);
	}

	//------------------------------------------------------------------------------------------
	// Informations sur le familier
	//------------------------------------------------------------------------------------------

	Field* fields = result->Fetch();

	uint32 petEntry = fields[1].GetUInt32();
	uint32 familyId = GetPetFamily(petEntry);

	// Le nom du familier est celui enregistré par le joueur.
	// Il ne doit donc pas être traduit.
	std::string petName = fields[2].GetString();
	uint32 petLevel = fields[3].GetUInt32();

	//------------------------------------------------------------------------------------------
	// Icônes des familles de familiers
	//------------------------------------------------------------------------------------------

	switch (familyId)
	{
	case 1:
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Wolf:48 : 48|t  %s - Lvl %u";
		break; // Loup

	case 2:
		texticon = "|TInterface\\Icons\\ability_druid_skinteeth:48 : 48|t  %s - Lvl %u";
		break; // Félin

	case 3:
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Spider:48 : 48|t  %s - Lvl %u";
		break; // Araignée

	case 4:
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Bear:48 : 48|t  %s - Lvl %u";
		break; // Ours

	case 5:
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Boar:48 : 48|t  %s - Lvl %u";
		break; // Sanglier

	case 6:
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Crocolisk:48 : 48|t  %s - Lvl %u";
		break; // Crocodile

	case 7:
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Vulture:48 : 48|t  %s - Lvl %u";
		break; // Oiseau charognard

	case 8:
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Crab:48 : 48|t  %s - Lvl %u";
		break; // Crabe

	case 9:
		texticon = "|TInterface\\Icons\\ability_hunter_pet_gorilla:48 : 48|t  %s - Lvl %u";
		break; // Gorille

	case 10:
		texticon = "|TInterface\\Icons\\ability_physical_taunt:48 : 48|t  %s - Lvl %u";
		break; // Custom

	case 11:
		texticon = "|TInterface\\Icons\\ability_hunter_pet_raptor:48 : 48|t  %s - Lvl %u";
		break; // Raptor

	case 12:
		texticon = "|TInterface\\Icons\\ability_hunter_pet_tallstrider:48 : 48|t  %s - Lvl %u";
		break; // Grand trotteur

	case 20:
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Scorpid:48 : 48|t  %s - Lvl %u";
		break; // Scorpion

	case 21:
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Turtle:48 : 48|t  %s - Lvl %u";
		break; // Tortue

	case 24:
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Bat:48 : 48|t  %s - Lvl %u";
		break; // Chauve-souris

	case 25:
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Hyena:48 : 48|t  %s - Lvl %u";
		break; // Hyène

	case 26:
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Owl:48 : 48|t  %s - Lvl %u";
		break; // Oiseau de proie

	case 27:
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_WindSerpent:48 : 48|t  %s - Lvl %u";
		break; // Serpent des vents

	case 30:
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_DragonHawk:48 : 48|t  %s - Lvl %u";
		break; // Faucon-dragon

	case 31:
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Ravager:48 : 48|t  %s - Lvl %u";
		break; // Ravageur

	case 32:
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_WarpStalker:48 : 48|t  %s - Lvl %u";
		break; // Traqueur dimensionnel

	case 33:
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Sporebat:48 : 48|t  %s - Lvl %u";
		break; // Sporoptère

	case 34:
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_NetherRay:48 : 48|t  %s - Lvl %u";
		break; // Raie du Néant

	case 35:
		texticon = "|TInterface\\Icons\\Spell_Nature_GuardianWard:48 : 48|t  %s - Lvl %u";
		break; // Serpent

	case 37:
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Moth:48 : 48|t  %s - Lvl %u";
		break; // Papillon de nuit

	case 38:
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Chimera:48 : 48|t  %s - Lvl %u";
		break; // Chimère

	case 39:
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Devilsaur:48 : 48|t  %s - Lvl %u";
		break; // Diablosaure

	case 41:
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Silithid:48 : 48|t  %s - Lvl %u";
		break; // Silithide

	case 42:
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Worm:48 : 48|t  %s - Lvl %u";
		break; // Ver

	case 43:
		texticon = "|TInterface\\Icons\\ability_hunter_pet_rhino:48 : 48|t  %s - Lvl %u";
		break; // Rhinocéros

	case 44:
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_Wasp:48 : 48|t  %s - Lvl %u";
		break; // Guêpe

	case 45:
		texticon = "|TInterface\\Icons\\Ability_Hunter_Pet_CoreHound:48 : 48|t  %s - Lvl %u";
		break; // Chien du magma

	case 50:
		texticon = "|TInterface\\Icons\\ability_hunter_aspectofthefox:48 : 48|t  %s - Lvl %u";
		break; // Renard

	case 51:
		texticon = "|TInterface\\Icons\\inv_pet_monkey:48 : 48|t  %s - Lvl %u";
		break; // Singe

	case 52:
		texticon = "|TInterface\\Icons\\inv_pet_mastiff:48 : 48|t  %s - Lvl %u";
		break; // Chien

	case 53:
		texticon = "|TInterface\\Icons\\inv_misc_ahnqirajtrinket_01:48 : 48|t  %s - Lvl %u";
		break; // Hanneton

	case 55:
		texticon = "|TInterface\\Icons\\inv_pet_shalespider:48 : 48|t  %s - Lvl %u";
		break; // Araignée de schiste

	case 68:
		texticon = "|TInterface\\Icons\\rade_archaeology_whitehydrafigurine:48 : 48|t  %s - Lvl %u";
		break; // Hydre

	case 125:
		texticon = "|TInterface\\Icons\\inv_pet_cranegod:48 : 48|t  %s - Lvl %u";
		break; // Grue

	case 126:
		texticon = "|TInterface\\Icons\\inv_pet_waterstrider:48 : 48|t  %s - Lvl %u";
		break; // Trotteur aquatique

	case 127:
		texticon = "|TInterface\\Icons\\inv_pet_porcupine:48 : 48|t  %s - Lvl %u";
		break; // Porc-épic

	case 128:
		texticon = "|TInterface\\Icons\\achievement_moguraid_01:48 : 48|t  %s - Lvl %u";
		break;

	case 129:
		texticon = "|TInterface\\Icons\\inv_pet_goat:48 : 48|t  %s - Lvl %u";
		break; // Chèvre / Bouc

	case 130:
		texticon = "|TInterface\\Icons\\inv_pet_basilisk:48 : 48|t  %s - Lvl %u";
		break; // Basilic

	case 138:
		texticon = "|TInterface\\Icons\\inv_pet_direhorn:48 : 48|t  %s - Lvl %u";
		break; // Navrecorne

	case 149:
		texticon = "|TInterface\\Icons\\ability_mount_ironchimera:48 : 48|t  %s - Lvl %u";
		break; // Rylak

	case 150:
		texticon = "|TInterface\\Icons\\inv_hippo_green:48 : 48|t  %s - Lvl %u";
		break; // Hippopotame

	case 151:
		texticon = "|TInterface\\Icons\\inv_misc_herb_goldthorn_bramble:48 : 48|t  %s - Lvl %u";
		break; // Cerf

	default:
		texticon = "|TInterface\\Icons\\achievement_guildperk_fasttrack:48 : 48|t  %s - Lvl %u";
		break;
	}

	// Construire le texte avec le nom et le niveau du familier.
	snprintf(text, sizeof(text), texticon.c_str(), petName.c_str(), petLevel);

	std::string resultText(text);

	//------------------------------------------------------------------------------------------
	// Traduction de l'intitulé du niveau
	//------------------------------------------------------------------------------------------

	std::string levelLabel = "Niv.";

	if (lang == "enUS")
		levelLabel = "Lvl";
	else if (lang == "esES")
		levelLabel = "Niv.";
	else if (lang == "deDE")
		levelLabel = "Stufe";

	size_t pos = resultText.find("Lvl");

	if (pos != std::string::npos)
		resultText.replace(pos, 3, levelLabel);

	return resultText;
}

//##############################################################################################
// FAMILLE DU FAMILIER
//##############################################################################################

uint32 GetPetFamily(uint32 petEntry)
{
	CreatureTemplate const* cinfo = sObjectMgr->GetCreatureTemplate(petEntry);

	if (!cinfo)
		return 0;

	CreatureFamilyEntry const* cFamily =
		sCreatureFamilyStore.LookupEntry(cinfo->family);

	if (!cFamily)
		return 0;

	return cinfo->family;
}

//##############################################################################################
// ENREGISTREMENT DU SCRIPT
//##############################################################################################

void AddSC_npc_ecurie_chasseur()
{
	new npc_ecurie_chasseur();
}
