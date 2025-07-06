//###################################################################################################################################################################
//Stitch Avril 2021 - Player_Even_Action_handler - version 1.0 , Wod 6.2.4-21742 
// Action (Apprentissage, ajout d'item,etc) suite a un événement Joueur (connexion,levelup,zone, création) suivant la classe ou la race
//###################################################################################################################################################################

//#include "Config.h"
#include "ScriptMgr.h"
#include "Player.h"
#include "Map.h"
#include "WorldSession.h"
#include "GameEventMgr.h"
#include "Channel.h"
#include "ChannelMgr.h"
#include "Chat.h"
#include "ChatPackets.h"
#include "ChatTextBuilder.h"
#include "Config.h"
#include "Unit.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Guild.h"
#include "GuildFinderMgr.h"
#include "GuildMgr.h"
#include "DatabaseEnv.h"
#include "SpellAuraEffects.h"
#include "SpellHistory.h"
#include "SpellMgr.h"
#include "SpellScript.h"

uint32 SPELL_POUR_VISUEL = 14867;
uint32 PERTE_DE_DURABILITE = 45317;         //  Perte de 10 % de la durabilité
uint32 DISPARITIONS = 35205;



namespace {

class Player_Even_Action_handler : public PlayerScript {

public:
    Player_Even_Action_handler() : PlayerScript("Player_Even_Action_handler") {}

	// ################################################################################################################################################
    // Routines de test d"evenements 
	// ################################################################################################################################################


    void OnLogin(Player* player, bool firstLogin) override
    {
		uint8 SpecActive = player->GetSpecId(player->GetActiveTalentGroup());
		uint8 _class = player->getClass();

		Apprentissage_Suivant_classes_races(player);

        // 1ere connexion joueur
		if(firstLogin == true)						// a tester
		{
			player->LearnSpell(300251, true);      // Nuit Claire : Rend les nuits ou lieux sombres plus clair.
        }

		switch (_class)
		{
		case CLASS_WARRIOR: 
			// Rafraichissement posture a la connexion 
			if (player->HasAura(2457))
			{
				player->RemoveAurasDueToSpell(2457);	// Posture de combat
				player->AddAura(2457, player);
			}
			if (player->HasAura(71))
			{
				player->RemoveAurasDueToSpell(71);		// Posture défensive  
				player->AddAura(71, player);
			}

			if (player->HasAura(156291))
			{
				player->RemoveAurasDueToSpell(156291);	// Posture du gladiateur
				player->AddAura(156291, player);
			}

			break;
		case CLASS_PALADIN: break;
		case CLASS_HUNTER: break;
		case CLASS_ROGUE: break;
		case CLASS_PRIEST: break;
		case CLASS_DEATH_KNIGHT: break;
		case CLASS_SHAMAN: break;
		case CLASS_MAGE: break;
		case CLASS_WARLOCK: break;
		case CLASS_MONK: 
			// Rafraichissement posture a la connexion 
			if (player->HasAura(103985))
			{
				player->RemoveAurasDueToSpell(103985);	// Posture du tigre féroce		
				player->AddAura(103985, player);
			}
			if (player->HasAura(115069))
			{
				player->RemoveAurasDueToSpell(115069);	// Posture du buffle vigoureux	
				player->AddAura(115069, player);
			}
			if (player->HasAura(154436))
			{
				player->RemoveAurasDueToSpell(154436);	// Posture de la grue fougueuse
				player->AddAura(154436, player);
			}
			break;
		case CLASS_DRUID:break;
		default:
			break;

		}


		player->LearnSpell(300048, true);      // Reparation de 10% de la durabilité

		player->UpdateAllStats();

    }

    // Au level up
    virtual void OnLevelChanged(Player* player, uint8 /*oldLevel*/) 
    {
		Apprentissage_Suivant_classes_races(player);
    }
	//void OnPlayerTalentsReset(Player* player, bool noCost) override
	void OnPlayerTalentsReset(Player* player, bool noCost) 
	{
		Apprentissage_Suivant_classes_races(player);
	}




    // Sans action pour le moment
    void OnPlayerEnterZone(Player* player, uint32 newZone, uint32 newArea)
    {
		//VOIR Stitch fatigue forcé pour interdire une zone - Vashj'ir
    }
    void OnPlayerUpdate(Player* player, uint32 p_time)
    {
        if (!player->IsInWorld()) { return; }

    }
	void OnPlayerEnterZone(Player* player)
	{
	}


	//player->CastSpell(player, 14867, true);		// Pour visuel

    ////// TMP a voir/vérifier : public: /* CreatureScript */
    // void OnPlayerTalentsReset(Player* player, bool noCost)
    // void OnPlayerEnter(map, player) {}
	// void ScriptMgr::OnPlayerEnterMap(Map* map, Player* player)
	// void OnPlayerEnterZone(Player* player, uint32 newZone, uint32 newArea)
	// OnPlayerEnterZone(Player* player) {}
	// void OnPlayerUpdateZone(Player* player, uint32 newZone, uint32 newArea);
	// void OnPlayerLeaveZone(Player* player)
    // void OnPlayerLogin(Player* player, bool firstLogin) {}
    // void OnPlayerRepop(Player* player) {}
    // void OnPlayerKilledByCreature(Creature* /*killer*/, Player* player/*killed*/) {}
    // void OnCreatureKill(Player* /*killer*/, Creature* /*killed*/) { }
    // void OnLogout(Player* /*player*/) { }
    // void OnCreate(Player* /*player*/) { }
	// void OnPlayerCreate(Player* player);
    // void OnMapChanged(Player* /*player*/) { }
    // void Player::Update(uint32 p_time)
    // void Player::KillPlayer()
    // void UpdateAI(uint32 diff) override ---> if (CanSpawn)
    // void Player::OnCombatExit()
    // void OnPlayerSpellCast(Player* player, Spell* spell, bool skipCheck) {}
    // void OnAccountLogin(uint32 /*accountId*/) {}
	// void OnCombatExit();
	// void OnPlayerReputationChange(Player* player, uint32 factionID, int32& standing, bool incremental);
	// void OnPlayerChat(Player* player, uint32 type, uint32 lang, std::string& msg);
	// void OnPlayerChat(Player* player, uint32 type, uint32 lang, std::string& msg, Player* receiver);
	// void OnPlayerChat(Player* player, uint32 type, uint32 lang, std::string& msg, Guild* guild);
	// void OnPlayerChat(Player* player, uint32 type, uint32 lang, std::string& msg, Channel* channel);



	// ################################################################################################################################################
    // Routines d'actions secondaires suite a un évènement 
	// ################################################################################################################################################

	void Apprentissage_Suivant_classes_races(Player* player)	//Ou_Additem_
    {
    uint8 _team = player->GetTeamId();      
    uint8 _class = player->getClass();
    uint8 _level = player->getLevel();
    uint8 _race = player->getRace();
    uint8 _GenderID = player->getGender();

	if (_level > 10) 
	{ 
		player->LearnSpell(300283, false);//Feu de camp
	}	
	else player->RemoveSpell(300283, false); 


    switch (_class)
{

case CLASS_WARRIOR:
	if (_level > 3) { player->LearnSpell(6343, true); }			//Coup de tonnerre 
	else player->RemoveSpell(6343, false); 
    break;
case CLASS_PALADIN:
    break;
case CLASS_HUNTER:
	player->setPowerType(POWER_FOCUS);
	player->SetPower(POWER_FOCUS, 100);
    if (_level >= 5)
    {
        player->LearnSpell(883, true);      // Appel du familier 1
        player->LearnSpell(2641, true);     // Renvoyer le familier
        player->LearnSpell(9321, true);     // Contrôle du familier
        player->LearnSpell(6991, true);     // Nourrir le familier
        player->LearnSpell(136, true);      // Guérison du familier - ne s'apprend pas ?!
        player->LearnSpell(33976, true);    // Guérison du familier : temporaire : parce que 136 ne veux pas s'apprendre seul
        player->LearnSpell(982, true);      // Ressusciter le familier
        player->LearnSpell(1515, true);     // Apprivoiser une bête
        player->LearnSpell(1462, true);     // Connaissance des bêtes
        player->GetSession()->SendNotification("|cffffffff[Vous avez appris de nouvelles competances]:|r");
    }
	else
	{
		player->RemoveSpell(883, false);
		player->RemoveSpell(2641, false);
		player->RemoveSpell(9321, false);
		player->RemoveSpell(6991, false);
		player->RemoveSpell(136, false);
		player->RemoveSpell(33976, false);
		player->RemoveSpell(982, false);
		player->RemoveSpell(1515, false);
		player->RemoveSpell(1462, false);
	}

    if (_level >= 10)
    {
        player->LearnSpell(83242, true);      // Appel du familier 2
    }
	else player->RemoveSpell(83242, false);

    if (_level >= 42)
    {
        player->LearnSpell(83243, true);      // Appel du familier 3
    }
	else player->RemoveSpell(83243, false);

    if (_level >= 62)
    {
        player->LearnSpell(83244, true);      // Appel du familier 4
    }
	else player->RemoveSpell(83244, false);

    if (_level >= 82)
    {
        player->LearnSpell(83245, true);      // Appel du familier 5
    }
	else player->RemoveSpell(83245, false);

    break;
case CLASS_ROGUE:
	// Vampire ------
	if (player->GetUInt32Value(PLAYER_FIELD_CURRENT_SPEC_ID) == PLAYER_SPEC_ID_VAMPIRE) //Stitch Vampire - 538 correspond a ID de ChrSpecialization.dbc
	{
		if (player->HasAura(300124) || player->HasAura(300125))
		{
			player->setPowerType(POWER_DEMONIC_FURY);
			player->SetMaxPower(POWER_DEMONIC_FURY, 100);
			player->SetMaxPower(POWER_ENERGY, 0);
		}
		else
		{
			player->setPowerType(POWER_ENERGY);
			player->SetMaxPower(POWER_ENERGY, 100);
		}

		//Le voleur vampire n'utilisera plus ces sorts
		player->RemoveSpell(883);
		player->RemoveSpell(1784);
		player->RemoveSpell(6770);
		player->RemoveSpell(1752);
		player->RemoveSpell(31224);
		player->RemoveSpell(2094);
		player->RemoveSpell(73651);
		player->RemoveSpell(1833);
		player->RemoveSpell(1856);
		player->RemoveSpell(8676);
		player->RemoveSpell(703);
		player->RemoveSpell(121411);
		player->RemoveSpell(114018);
		player->RemoveSpell(14185);
		player->RemoveSpell(1776);
		player->RemoveSpell(5938);
		player->RemoveSpell(1966);
		player->RemoveSpell(2098);
		player->RemoveSpell(5277);
		player->RemoveSpell(408);
		player->RemoveSpell(108209);
	}
    break;
case CLASS_PRIEST:
	player->SetPower(POWER_SHADOW_ORBS, 0); // RAZ POWER_SHADOW_ORBS a la connexion sinon bug visuel
    break;
case CLASS_DEATH_KNIGHT:
    if (_level >= 6)
    {
        player->LearnSpell(53428, true); // Runeforge
    }
	else player->RemoveSpell(53428, false);

    if (_level >= 10)
    {
        player->LearnSpell(50977, true); // Porte de la mort
        player->LearnSpell(73313, true); // Destrier de la mort cramoisi
    }
	else
	{
		player->RemoveSpell(50977, false);
		player->RemoveSpell(73313, false);
	}

	if (player->GetUInt32Value(PLAYER_FIELD_CURRENT_SPEC_ID) == TALENT_SPEC_DEATHKNIGHT_CHAOS)
	{
		player->RemoveSpell(43265);//
		player->RemoveSpell(49576);//
	}

    break;
case CLASS_SHAMAN:
    break;
case CLASS_MAGE:
    break;
case CLASS_WARLOCK:
	player->SetPower(POWER_SOUL_SHARDS, 0); // RAZ POWER_SOUL_SHARDS a la connexion
    break;
case CLASS_MONK:
    break;
case CLASS_DRUID:
    break;
}

	switch (_race)
{
case RACE_HUMAN:
    break;
case RACE_ORC:
    break;
case RACE_DWARF:
    break;
case RACE_NIGHTELF:
    break;
case RACE_UNDEAD_PLAYER:
    break;
case RACE_TAUREN:
    break;
case RACE_GNOME:
    break;
case RACE_TROLL:
    break;
case RACE_GOBLIN:
    break;
case RACE_BLOODELF:
    break;
case RACE_DRAENEI:
    break;
case RACE_WORGEN:
    player->LearnSpell(68978, true); // Ecorcheur
    player->LearnSpell(69270, true); // Langue (gilnéen)
    player->LearnSpell(68976, true); // Aberration
    player->LearnSpell(68975, true); // Acharnement
	if (_level >= 5)	
	{		
		player->LearnSpell(68992, true); // Sombre course	
	}
	else player->RemoveSpell(68992, false);

	if (_level >= 10)	
	{		
		player->LearnSpell(68996, true); // Deux formes
		player->LearnSpell(94293, true); // Forme modifiée
	}
	else
	{
		player->RemoveSpell(68996, false);
		player->RemoveSpell(94293, false);
	}

	if (_level >= 20)
	{
		player->LearnSpell(87840, true); // Ventre à terre	
	}
	else player->RemoveSpell(87840, false);

    break;
case RACE_PANDAREN_NEUTRAL:
    break;
case RACE_PANDAREN_ALLIANCE:
    break;
case RACE_PANDAREN_HORDE:
    break;

}
    }

	/*
    void Apprentissage_Rez_sur_son_corp(Player* player)
    {
        uint8 rez = sConfigMgr->GetIntDefault("Rez_sur_son_corp", 1);
        if (rez == 1)
        {
            player->LearnSpell(SPELL_POUR_REZ, true);      // Résurrection de Pierre d'âme
        }
    }
    */




private:
	std::map<ObjectGuid, int> _unitDifficulty;
};

}

void AddSC_Player_Even_Action() {
	new Player_Even_Action_handler();
}
