
//########################################################################################################################
// Stitch npc_ChangeFactionPanda - PNJ 56013 : choix de faction des panda pour la quete finale 31450 by monsieur Noc
//########################################################################################################################

#define GOSSIP_TEXT_EXP1 15000049

#include "world.h"
#include "Config.h"

class npc_ChangeFactionPanda : public CreatureScript
{
public:
	npc_ChangeFactionPanda() : CreatureScript("npc_ChangeFactionPanda") {}

	bool OnGossipHello(Player* player, Creature* creature) override
	{
		if (!player)
			return true;

		if (player->GetQuestStatus(31450) == QUEST_STATUS_INCOMPLETE)
		{
			// Langue forcée depuis worldserver.conf
			std::string ForceClientLangue = sConfigMgr->GetStringDefault("ForceClientLangue", "frFR");

			// Français par défaut
			std::string GOSSIP_CHOOSE_ALLIANCE = u8"Je voudrais rejoindre l'Alliance";
			std::string GOSSIP_CHOOSE_HORDE = u8"Je voudrais rejoindre la Horde";

			// Anglais
			if (ForceClientLangue == "enUS")
			{
				GOSSIP_CHOOSE_ALLIANCE = "I would like to join the Alliance";
				GOSSIP_CHOOSE_HORDE = "I would like to join the Horde";
			}
			// Espagnol
			else if (ForceClientLangue == "esES")
			{
				GOSSIP_CHOOSE_ALLIANCE = u8"Me gustaría unirme a la Alianza";
				GOSSIP_CHOOSE_HORDE = u8"Me gustaría unirme a la Horda";
			}
			// Allemand
			else if (ForceClientLangue == "deDE")
			{
				GOSSIP_CHOOSE_ALLIANCE = u8"Ich möchte der Allianz beitreten";
				GOSSIP_CHOOSE_HORDE = u8"Ich möchte der Horde beitreten";
			}

			uint8 _Race = player->getRace();

			if (_Race == RACE_PANDAREN_HORDE || _Race == RACE_PANDAREN_NEUTRAL)
			{
				player->ADD_GOSSIP_ITEM(0, GOSSIP_CHOOSE_ALLIANCE.c_str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
			}

			if (_Race == RACE_PANDAREN_ALLIANCE || _Race == RACE_PANDAREN_NEUTRAL)
			{
				player->ADD_GOSSIP_ITEM(0, GOSSIP_CHOOSE_HORDE.c_str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
			}

			player->PlayerTalkClass->SendGossipMenu(GOSSIP_TEXT_EXP1, creature->GetGUID());
		}

		return true;
	}

	bool OnGossipSelect(Player* player, Creature* Creature, uint32 /*uiSender*/, uint32 uiAction) override
	{
		if (!player)
			return true;

		//      player->CLOSE_GOSSIP_MENU();

		if (uiAction == GOSSIP_ACTION_INFO_DEF + 1)
		{
			player->ShowNeutralPlayerFactionSelectUI();
			player->SetPandaFactionAlliance();

			player->PlayerTalkClass->SendCloseGossip();
		}
		else if (uiAction == GOSSIP_ACTION_INFO_DEF + 2)
		{
			player->ShowNeutralPlayerFactionSelectUI();
			player->SetPandaFactionHorde();

			player->PlayerTalkClass->SendCloseGossip();
		}

		return true;
	}
};

void AddSC_npc_ChangeFactionPanda_quete()
{
	new npc_ChangeFactionPanda();
}
