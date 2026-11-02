//////////////////////////////////////////////////////////////////////////////////////////////
// Stitch Octobre 2026
// Commande .debloc : 
// Commande de deblocage : teleporte le joueur a Hurlevent ou Orgrimmar suivant sa faction
// Cette commande applique le Mal de resurrection et ressuscite le joueur s'il est mort
//////////////////////////////////////////////////////////////////////////////////////////////

#include "Chat.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellMgr.h"

class debloc_commandscript : public CommandScript
{
public:
	debloc_commandscript() : CommandScript("debloc_commandscript") { }

	std::vector<ChatCommand> GetCommands() const override
	{
		static std::vector<ChatCommand> commandTable =
		{
			{ "debloc", 0, false, &HandleDeblocCommand,
			"-----------------------------------------------------------------\n"
			"|cff00ff00Syntax: .debloc|r\n"
			"Ressuscite le personnage et le teleporte dans sa capitale.\n"
			"Alliance : Hurlevent\n"
			"Horde    : Orgrimmar.\n"
			"Applique le Mal de resurrection\n" },
		};

		return commandTable;
	}

	static bool HandleDeblocCommand(ChatHandler* handler, char const* args)
	{
		if (!handler->GetSession())
			return false;

		// Le joueur ne peut débloquer que son propre personnage
		Player* player = handler->GetSession()->GetPlayer();

		if (!player)
			return false;

		// Résurrection si le joueur est mort
		if (!player->IsAlive())
		{
			player->ResurrectPlayer(1.0f);
			player->SpawnCorpseBones();
		}

		// Alliance : Hurlevent
		if (player->GetTeam() == ALLIANCE)
		{
			player->TeleportTo(
				0,
				-8833.38f,
				628.62f,
				95.0f,
				0.7f
			);
			// Appliquer le Mal de resurrection
			player->CastSpell(player, 15007, true);

			handler->PSendSysMessage(
				"DEBLOC : Vous avez ete envoye a Hurlevent."
			);
		}
		// Horde : Orgrimmar
		else
		{
			player->TeleportTo(
				1,
				1502.71f,
				-4415.42f,
				24.55f,
				0.0f
			);
			// Appliquer le Mal de resurrection
			player->CastSpell(player, 15007, true);

			handler->PSendSysMessage(
				"DEBLOC : Vous avez ete envoye a Orgrimmar."
			);
		}

		return true;
	}
};

void AddSC_debloc_commandscript()
{
	new debloc_commandscript();
}
