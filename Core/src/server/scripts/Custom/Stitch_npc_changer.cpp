//##############################################################################################
//Stitch npc_changer - PNJ 15000142 : change race , faction , apparence gratuitement
//##############################################################################################

#include "world.h"
#include "Config.h"

enum  defines
{
	faction_token = 100,	// Faction Change Token
	race_token = 100,		// Race Change Token
	customize_token = 100,  // Customize Change Token
	rename_token = 100,		// Rename Change Token
	location = 1,
};
class npc_changer : public CreatureScript
{
public:
	npc_changer() : CreatureScript("npc_changer") {}

	bool OnGossipHello(Player* player, Creature* creature)
	{
		TeamId team = player->GetTeamId();
		uint8 _level = player->getLevel();
		uint8 _race = player->getRace();

		// Textes
		std::string factionText;
		std::string raceText;
		std::string pandaAllianceText;
		std::string pandaHordeText;
		std::string appearanceText;
		std::string talentsText;

		// Lecture de worldserver.conf
		std::string ForceClientLangue = sConfigMgr->GetStringDefault("ForceClientLangue", "auto");

		// Anglais
		if (ForceClientLangue == "enUS")
		{
			factionText = "Change Faction (With Teleport)";
			raceText = "Change Race";
			pandaAllianceText = "Change to Alliance Pandaren (TP)";
			pandaHordeText = "Change to Horde Pandaren (TP)";
			appearanceText = "Change My Appearance";
			talentsText = "Reset My Talents";
		}
		// Espagnol
		else if (ForceClientLangue == "esES")
		{
			factionText = u8"Cambiar de facción (Con TP)";
			raceText = u8"Cambiar de raza";
			pandaAllianceText = u8"Convertirse en Pandaren Alianza (TP)";
			pandaHordeText = u8"Convertirse en Pandaren Horda (TP)";
			appearanceText = u8"Cambiar mi apariencia";
			talentsText = u8"Reiniciar mis talentos";
		}
		// Allemand
		else if (ForceClientLangue == "deDE")
		{
			factionText = u8"Fraktion wechseln (Mit Teleport)";
			raceText = u8"Volk wechseln";
			pandaAllianceText = u8"Zu Allianz-Pandaren wechseln (TP)";
			pandaHordeText = u8"Zu Horde-Pandaren wechseln (TP)";
			appearanceText = u8"Mein Aussehen ändern";
			talentsText = u8"Meine Talente zurücksetzen";
		}
		// Français par défaut (frFR, auto ou autre)
		else
		{
			factionText = u8"Changer de faction (Avec TP)";
			raceText = u8"Changer de race";
			pandaAllianceText = u8"Devenir Pandaren Alliance (TP)";
			pandaHordeText = u8"Devenir Pandaren Horde (TP)";
			appearanceText = u8"Changer mon apparence";
			talentsText = u8"Effacer mes talents";
		}

		// Menu Gossip
		player->ADD_GOSSIP_ITEM(2, factionText.c_str(), GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(8, raceText.c_str(), GOSSIP_SENDER_MAIN, 0);

		if (_race == RACE_PANDAREN_NEUTRAL || team == TEAM_ALLIANCE)
		{
			player->ADD_GOSSIP_ITEM(8, pandaAllianceText.c_str(), GOSSIP_SENDER_MAIN, 4);
		}

		if (_race == RACE_PANDAREN_NEUTRAL || team == TEAM_HORDE)
		{
			player->ADD_GOSSIP_ITEM(8, pandaHordeText.c_str(), GOSSIP_SENDER_MAIN, 5);
		}

		player->ADD_GOSSIP_ITEM(3, appearanceText.c_str(), GOSSIP_SENDER_MAIN, 9);
		player->ADD_GOSSIP_ITEM(3, talentsText.c_str(), GOSSIP_SENDER_MAIN, 10);

		player->PlayerTalkClass->SendGossipMenu(9425, creature->GetGUID());

		return true;
	}



	bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
	{
		if (!player)
			return true;

		uint8 _class = player->getClass();
		uint8 _level = player->getLevel();
		uint8 _race = player->getRace();

		player->PlayerTalkClass->SendCloseGossip();

		switch (uiAction)
		{
		case 0: // Changer de Race
			player->CastSpell(player, 14867, true);
			player->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);

			break;
		case 1: // Changer de Faction(Avec TP)
			player->CastSpell(player, 14867, true);
			player->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);

			break;
		case 4: // Changer de Race en Panda de l'Alliance
			player->CastSpell(player, 14867, true);

			if (_race == RACE_PANDAREN_NEUTRAL)
			{
				player->SetAtLoginFlag(AT_LOGIN_allianceverspanda);
			}
			else if (_race != RACE_PANDAREN_HORDE)
			{
				player->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
			}

			player->SetPandaFactionAlliance();

			break;
		case 5: // Changer de Race en Panda de la Horde
			player->CastSpell(player, 14867, true);

			if (_race == RACE_PANDAREN_NEUTRAL)
			{
				player->SetAtLoginFlag(AT_LOGIN_hordeverspanda);
			}
			else if (_race != RACE_PANDAREN_ALLIANCE)
			{
				player->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
			}

			player->SetPandaFactionHorde();

			break;
		case 9: // Changer mon Apparence
			player->CastSpell(player, 14867, true);
			player->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);

			break;
		case 10: // Effacer mes talents
			player->CastSpell(player, 14867, true);
			player->SetAtLoginFlag(AT_LOGIN_RESET_TALENTS);

			break;
		}

		// Sauvegarde
		switch (uiAction)
		{
		case 0:		// Changer de Race
		case 1:		// Changer de Faction(Avec TP)
		case 9:		// Changer mon Apparence
		case 10:	// Effacer mes talents
			player->SaveToDB();
			player->GetSession()->LogoutPlayer(false);

			break;
		}
		return true;
	}

};

void AddSC_npc_changer()
{
	new npc_changer();
}
