/*
 * Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

// This is where scripts' loading functions should be declared:
void AddSC_solocraft();
void AddSC_Stitch_npc_ai_level_scale();
void AddSC_npc_levels();
void AddSC_npc_changer();
void AddSC_npc_ChangeFactionPanda_quete();	
void AddSC_npc_ai_druide();	
void AddSC_npc_ai_chaman();
void AddSC_npc_ai_pretre();
void AddSC_npc_ai_demo();
void AddSC_npc_ai_mage();
void AddSC_npc_ai_paladin();
void AddSC_npc_ai_guerrier();
void AddSC_npc_ai_chasseur();
void AddSC_npc_ai_dk();
void AddSC_npc_ai_voleur();

// The name of this function should match:
// void Add${NameOfDirectory}Scripts()
void AddCustomScripts()
{
		AddSC_solocraft();
		AddSC_Stitch_npc_ai_level_scale();
		AddSC_npc_levels();
		AddSC_npc_changer();
		AddSC_npc_ChangeFactionPanda_quete();
		AddSC_npc_ai_druide();
		AddSC_npc_ai_chaman();
		AddSC_npc_ai_pretre();
		AddSC_npc_ai_demo();
		AddSC_npc_ai_mage();
		AddSC_npc_ai_paladin();
		AddSC_npc_ai_guerrier();
		AddSC_npc_ai_chasseur();
		AddSC_npc_ai_dk();
		AddSC_npc_ai_voleur();
}
