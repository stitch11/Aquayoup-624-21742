#include <map>

#include "Config.h"
#include "ScriptMgr.h"
#include "Unit.h"
#include "Player.h"
#include "Pet.h"
#include "Map.h"
#include "Group.h"
#include "InstanceScript.h"



/*
 * TODO:
 * 1. Dispel target regeneration
 * 2. Provide unlimited http://www.wowhead.com/item=17333/aqual-quintessence
 */

namespace {

class solocraft_player_instance_handler : public PlayerScript {
public:
	solocraft_player_instance_handler() : PlayerScript("solocraft_player_instance_handler") {
		TC_LOG_INFO("scripts.solocraft.player.instance", "[Solocraft] solocraft_player_instance_handler Loaded");
	}

	void OnLogin(Player *player, bool firstLogin) override {
		ChatHandler(player->GetSession()).SendSysMessage(">>> Mode solocraft activé <<<");
	}

	void OnMapChanged(Player *player) override {
		Map *map = player->GetMap();
		int difficulty = CalculateDifficulty(map, player);
		int numInGroup = GetNumInGroup(player);
		ApplyBuffs(player, map, difficulty, numInGroup);
	}
private:
	std::map<ObjectGuid, int> _unitDifficulty;

	int CalculateDifficulty(Map *map, Player *player) {
		int difficulty = 1;
		if (map) {
			if (map->Is25ManRaid()) {
				difficulty = 25;
			} else if (map->IsHeroic()) {
				difficulty = 10;
			} else if (map->IsRaid()) {
				difficulty = 40;
			} else if (map->IsDungeon()) {
				difficulty = 5;
			}
		}
		return difficulty;
	}

	int GetNumInGroup(Player *player) {
		int numInGroup = 1;
		Group *group = player->GetGroup();
		if (group) {
			Group::MemberSlotList const& groupMembers = group->GetMemberSlots();
			numInGroup = groupMembers.size();
		}
		return numInGroup;
	}

	void ApplyBuffs(Player *player, Map *map, int difficulty, int numInGroup) {
		ClearBuffs(player, map);
		if (difficulty > 1) {
			//InstanceMap *instanceMap = map->ToInstanceMap();
			//InstanceScript *instanceScript = instanceMap->GetInstanceScript();

			ChatHandler(player->GetSession()).PSendSysMessage("Entrée vers %s (difficulty = %d, numInGroup = %d)",
					map->GetMapName(), difficulty, numInGroup);

			_unitDifficulty[player->GetGUID()] = difficulty;
/*			for (int32 i = STAT_STRENGTH; i < MAX_STATS; ++i) {
				player->HandleStatModifier(UnitMods(UNIT_MOD_STAT_START + i), TOTAL_PCT, float(difficulty * 100), true);


			}
*/
			player->HandleStatModifier(UnitMods(UNIT_MOD_MANA), TOTAL_PCT, float(difficulty * 75), true);// Total mana
			player->HandleStatModifier(UnitMods(UNIT_MOD_HEALTH), TOTAL_PCT, float(difficulty * 75), true);
//			player->HandleStatModifier(UnitMods(UNIT_MOD_ATTACK_POWER), TOTAL_PCT, float(difficulty * 25), true); //  certains spells , n'ameliore pas les heal

			
//			player->HandleStatModifier(UnitMods(UNIT_MOD_ATTACK_POWER_RANGED), TOTAL_PCT, float(difficulty * 25), true);
//			player->RemoveAura(300047); // pour risque de doublon
			player->RemoveAurasDueToSpell(300047);// pour risque de doublon , retire toutes les auras 300047
			player->AddAura(300047, player);








			player->SetFullHealth();
			if (player->getPowerType() == POWER_MANA) {
				player->SetPower(POWER_MANA, player->GetMaxPower(POWER_MANA));
			}
		}
	}

	void ClearBuffs(Player *player, Map *map) {
		std::map<ObjectGuid, int>::iterator unitDifficultyIterator = _unitDifficulty.find(player->GetGUID());
		if (unitDifficultyIterator != _unitDifficulty.end()) {
			int difficulty = unitDifficultyIterator->second;
			_unitDifficulty.erase(unitDifficultyIterator);

			ChatHandler(player->GetSession()).PSendSysMessage("Sortie vers %s (removing difficulty = %d)",
					map->GetMapName(), difficulty);


/*			for (int32 i = STAT_STRENGTH; i < MAX_STATS; ++i) {
				player->HandleStatModifier(UnitMods(UNIT_MOD_STAT_START + i), TOTAL_PCT, float(difficulty * 100), false);
			}
*/

			player->HandleStatModifier(UnitMods(UNIT_MOD_MANA), TOTAL_PCT, float(difficulty * 75), false);
			player->HandleStatModifier(UnitMods(UNIT_MOD_HEALTH), TOTAL_PCT, float(difficulty * 75), false);
			player->RemoveAura(300047);

		}
	}
};

}

void AddSC_solocraft() {
	new solocraft_player_instance_handler();
}
