///////////////////////////////////////////////////////////////////////////////////////////////////
// Stitch Octobre 2026
// Commande telez : 
// Téléporte le GM au meme X,Y mais avec un +-Z en paramètre. Par exemple pour le placement de gob
// Cette commande applique le Mal de resurrection et ressuscite le joueur s'il est mort
///////////////////////////////////////////////////////////////////////////////////////////////////


#include "Chat.h"
#include "Player.h"
#include "ScriptMgr.h"
#include <cstdlib>

class telez_commandscript : public CommandScript
{
public:
	telez_commandscript() : CommandScript("telez_commandscript") { }

	std::vector<ChatCommand> GetCommands() const override
	{
		static std::vector<ChatCommand> commandTable =
		{
			{
				"telez",
				rbac::RBAC_PERM_COMMAND_GM,
				false,
				&HandleTeleZCommand,
				"-----------------------------------------------------------------\n"
				"|cff00ff00Syntax: .telez valeurZ|r\n"
				"Deplace un GM verticalement uniquement.\n"
				"Exemple: .telez +10\n"
				"Exemple: .telez -10"
			},
		};

		return commandTable;
	}

	static bool HandleTeleZCommand(ChatHandler* handler, char const* args)
	{
		if (!handler->GetSession())
			return false;

		Player* player = handler->getSelectedPlayer();

		if (!player)
			player = handler->GetSession()->GetPlayer();

		if (!player || !args || !*args)
			return false;

		char* end = nullptr;
		float offset = strtof(args, &end);

		if (end == args || *end != '\0' || offset == 0.0f)
			return false;

		float x = player->GetPositionX();
		float y = player->GetPositionY();
		float z = player->GetPositionZ() + offset;
		float o = player->GetOrientation();

		player->TeleportTo(
			player->GetMapId(),
			x,
			y,
			z,
			o
		);

		handler->PSendSysMessage(
			"TELEZ : %+.2f | Nouvelle hauteur Z : %.2f",
			offset,
			z
		);

		return true;
	}
};

void AddSC_telez_commandscript()
{
	new telez_commandscript();
}
