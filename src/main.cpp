/*
 * Copyright (C) 2020 Burak (NexoR)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "precompiler.h"
#include "Natives.h"
#include "main.h"

std::set<AMX*> global_Amx;

extern void* pAMXFunctions;

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return sampgdk::Supports() | SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void** ppData)
{
	SlotManager::Reset_All_ID();
	Item::Reset();
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	bool load = sampgdk::Load(ppData);
	sampgdk::logprintf("");
	sampgdk::logprintf(" ===============================");
	sampgdk::logprintf("                                ");
	sampgdk::logprintf("    Textdraw Streamer Yuklendi  ");
	sampgdk::logprintf("                                ");
	sampgdk::logprintf("          Surum: %s            ", PLUGIN_VERSION);
	sampgdk::logprintf("                                ");
	sampgdk::logprintf("    Developer: Burak (NexoR)    ");
	sampgdk::logprintf("                                ");
	sampgdk::logprintf(" ===============================");
	sampgdk::logprintf("");
	return load;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	SlotManager::Reset_All_ID();
	Item::Reset();
	sampgdk::logprintf("");
	sampgdk::logprintf(" ===============================");
	sampgdk::logprintf("                                ");
	sampgdk::logprintf("   Textdraw Streamer Kapatildi  ");
	sampgdk::logprintf("                                ");
	sampgdk::logprintf("          Surum: %s            ", PLUGIN_VERSION);
	sampgdk::logprintf("                                ");
	sampgdk::logprintf("    Developer: Burak (NexoR)    ");
	sampgdk::logprintf("                                ");
	sampgdk::logprintf(" ===============================");
	sampgdk::logprintf("");
	sampgdk::Unload();
}

AMX_NATIVE_INFO Nativeler[] =
{
	// Player Textdraw
	{ "CreatePTextdraw", Natives::CreatePTextdraw },
	{ "DestroyPTextdraw", Natives::DestroyPTextdraw},
	{ "PTextLetterSize", Natives::PTextLetterSize},
	{ "PTextSize", Natives::PTextSize},
	{ "PTextAlignment", Natives::PTextAlignment},
	{ "PTextColor", Natives::PTextColor},
	{ "PTextUseBox", Natives::PTextUseBox},
	{ "PTextBoxColor", Natives::PTextBoxColor},
	{ "PTextShadow", Natives::PTextShadow},
	{ "PTextOutline", Natives::PTextOutline},
	{ "PTextBGColor", Natives::PTextBGColor},
	{ "PTextFont", Natives::PTextFont},
	{ "PTextProportional", Natives::PTextProportional},
	{ "PTextSelectable", Natives::PTextSelectable},
	{ "PTextShow", Natives::PTextShow},
	{ "PTextHide", Natives::PTextHide},
	{ "PTextSetString", Natives::PTextSetString},
	{ "PTextPreviewModel", Natives::PTextPreviewModel},
	{ "PTextPreviewRot", Natives::PTextPreviewRot},
	{ "PTextPreviewVehCol", Natives::PTextPreviewVehCol},

	// Veri Okuma
	{ "IsValidPlayerTextDraw", Natives::IsValidPlayerTextDraw},
	{ "IsPlayerTextDrawVisible", Natives::IsPlayerTextDrawVisible},
	{ "PlayerTextDrawGetString", Natives::PlayerTextDrawGetString},
	{ "PlayerTextDrawSetPos", Natives::PlayerTextDrawSetPos},
	{ "PlayerTextDrawGetLetterSize", Natives::PlayerTextDrawGetLetterSize},
	{ "PlayerTextDrawGetTextSize", Natives::PlayerTextDrawGetTextSize},
	{ "PlayerTextDrawGetPos", Natives::PlayerTextDrawGetPos},
	{ "PlayerTextDrawGetColor", Natives::PlayerTextDrawGetColor},
	{ "PlayerTextDrawGetBoxColor", Natives::PlayerTextDrawGetBoxColor},
	{ "PlayerTextDrawGetBackgroundCol", Natives::PlayerTextDrawGetBackgroundCol},
	{ "PlayerTextDrawGetShadow", Natives::PlayerTextDrawGetShadow},
	{ "PlayerTextDrawGetOutline", Natives::PlayerTextDrawGetOutline},
	{ "PlayerTextDrawGetFont", Natives::PlayerTextDrawGetFont},
	{ "PlayerTextDrawIsBox", Natives::PlayerTextDrawIsBox},
	{ "PlayerTextDrawIsProportional", Natives::PlayerTextDrawIsProportional},
	{ "PlayerTextDrawIsSelectable", Natives::PlayerTextDrawIsSelectable},
	{ "PlayerTextDrawGetAlignment", Natives::PlayerTextDrawGetAlignment},
	{ "PlayerTextDrawGetPreviewModel", Natives::PlayerTextDrawGetPreviewModel},
	{ "PlayerTextDrawGetPreviewRot", Natives::PlayerTextDrawGetPreviewRot},
	{ "PlayerTextDrawGetPreviewVehCol", Natives::PlayerTextDrawGetPreviewVehCol},

	// Extra ID
	{ "PlayerTextDrawSetExtraID", Natives::PlayerTextDrawSetExtraID},
	{ "PlayerTextDrawGetExtraID", Natives::PlayerTextDrawGetExtraID},
	{ "PlayerTextDrawResetExtraID", Natives::PlayerTextDrawResetExtraID},

	// Diger
	{ "PlayerTextDrawTotalCreate", Natives::PlayerTextDrawTotalCreate},

	// Knox
	{ "PlayerTextDrawSetGroup", Natives::PlayerTextDrawSetGroup},
	{ "ShowDynTextDrawGroup", Natives::ShowDynTextDrawGroup},
	{ "HideDynTextDrawGroup", Natives::HideDynTextDrawGroup},
	{ "DestroyDynTextDrawGroup", Natives::DestroyDynTextDrawGroup},
	{ "GetTextDrawInRangeOfPoint", Natives::GetTextDrawInRangeOfPoint},

	// Null
	{ 0, 0 }
};

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX* amx)
{
	global_Amx.insert(amx);
	return amx_Register(amx, Nativeler, -1);
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX* amx)
{
	global_Amx.erase(amx);
	return AMX_ERR_NONE;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerDisconnect(int playerid, int reason)
{
	SlotManager::Reset_ID(playerid);
	for (std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].begin(); p != Item::pText[playerid].end(); p++)
	{
		p->second->extra_id.clear();
	}
	Item::pText[playerid].clear();
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerClickPlayerTextDraw(int playerid, int text_id)
{
	int idx;

	if (Item::pText[playerid].empty())
		return false;
	if (global_Amx.empty())
		return false;

	for (std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].begin(); p != Item::pText[playerid].end(); p++)
	{
		if (p->second->real_id != text_id)
			continue;

		if (p->second->function.length() > 0)
		{
			for (std::set<AMX*>::iterator j = global_Amx.begin(); j != global_Amx.end(); j++)
			{
				if (!amx_FindPublic(*j, p->second->function.c_str(), &idx))
				{
					amx_Push(*j, p->second->functionIdx);
					amx_Push(*j, playerid);
					amx_Exec(*j, NULL, idx);
					return true;
				}
			}
		}

		for (std::set<AMX*>::iterator j = global_Amx.begin(); j != global_Amx.end(); j++)
		{
			// Default
			if (!amx_FindPublic(*j, "ClickDynamicPlayerTextdraw", &idx))
			{
				amx_Push(*j, p->first);
				amx_Push(*j, playerid);
				amx_Exec(*j, NULL, idx);
			}

			// Name suggestion: Fairuz
			if (!amx_FindPublic(*j, "OnPlayerClickDynamicTextdraw", &idx))
			{
				amx_Push(*j, p->first);
				amx_Push(*j, playerid);
				amx_Exec(*j, NULL, idx);
			}

			// Name suggestion: Kursed
			if (!amx_FindPublic(*j, "OnDynamicPlayerTextdrawClicked", &idx))
			{
				amx_Push(*j, p->first);
				amx_Push(*j, playerid);
				amx_Exec(*j, NULL, idx);
			}
		}
		break;
	}
	return false;
}