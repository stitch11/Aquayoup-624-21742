//########################################################################################################################
//Stitch npc_ChangeFactionPanda - PNJ 56013 : choix de faction des panda pour la quete finale 31450 by monsieur Noc
//########################################################################################################################

#define GOSSIP_TEXT_EXP1           15000049
#define GOSSIP_CHOOSE_ALLIANCE		"Je voudrais rejoindre l'alliance"
#define GOSSIP_CHOOSE_HORDE			"Je voudrais rejoindre la horde"

#define GOSSIP_CHOOSE_FACTION		"Je suis pret a choisir mon destin."
#define GOSSIP_TP_STORMIND			"J'aimerais aller a Hurlevent"
#define GOSSIP_TP_ORGRI				"J'aimerais aller a Orgrimmar"



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
			uint8 _Race = player->getRace();
			if (_Race == RACE_PANDAREN_HORDE || _Race == RACE_PANDAREN_NEUTRAL)
			{
				player->ADD_GOSSIP_ITEM(0, GOSSIP_CHOOSE_ALLIANCE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
			}
			if (_Race == RACE_PANDAREN_ALLIANCE || _Race == RACE_PANDAREN_NEUTRAL)
			{
				player->ADD_GOSSIP_ITEM(0, GOSSIP_CHOOSE_HORDE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
			}

			player->PlayerTalkClass->SendGossipMenu(GOSSIP_TEXT_EXP1, creature->GetGUID());
		}
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* Creature, uint32 /*uiSender*/, uint32 uiAction) override
	{
		if (!player)
			return true;

		//		player->CLOSE_GOSSIP_MENU();

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