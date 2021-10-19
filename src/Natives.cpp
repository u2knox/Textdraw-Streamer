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

#include "Natives.h"

#pragma region OLUSTURMA

cell AMX_NATIVE_CALL Natives::CreatePTextdraw(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);

	size_t id = SlotManager::Free_ID(playerid);

	std::unique_ptr<PlayerText> pt(new PlayerText());

	pt->real_id = INVALID_DYNAMIC_TEXTDRAW;

	pt->create_x = amx_ctof(params[2]);
	pt->create_y = amx_ctof(params[3]);

	pt->text = Servis::Get_String(amx, params[4]);

	pt->lettersize_x = 0.0;
	pt->lettersize_y = 0.0;

	pt->textsize_x = 0.0;
	pt->textsize_y = 0.0;

	pt->alignment = 1;
	pt->color = -2;
	pt->usebox = 0;
	pt->boxcolor = -2;
	pt->shadow = 2;
	pt->outline = 0;
	pt->backgroundcolor = -2;
	pt->font = 1;
	pt->proportional = 1;
	pt->selectable = 0;

	pt->setstring[0] = '\0';

	pt->function[0] = '\0';
	pt->functionIdx = 0;

	pt->modelindex = 0;
	pt->fRotX = 0.0;
	pt->fRotY = 0.0;
	pt->fRotZ = 0.0;
	pt->fZoom = 1.0;

	pt->veh_col1 = -2;
	pt->veh_col2 = -2;

	pt->extra_id.clear();

	if (LOG_MODE)
	{
		sampgdk::logprintf("%s pCreate: X: %f, Y: %f, Text: %s", LOG, pt->create_x, pt->create_y, pt->text.c_str());
	}

	Item::pText[playerid].insert(std::make_pair(id, std::move(pt)));

	return static_cast<cell>(id);
}

void ShowDynTextDraw(int playerid, int textid)
{
	std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(textid);

	if (p->second->real_id == INVALID_DYNAMIC_TEXTDRAW)
	{
		int j = INVALID_TEXT_DRAW;

		if (p->second->setstring.length() != 0)
		{
			j = sampgdk_CreatePlayerTextDraw(playerid, p->second->create_x, p->second->create_y, p->second->setstring.c_str());
		}
		else
		{
			j = sampgdk_CreatePlayerTextDraw(playerid, p->second->create_x, p->second->create_y, p->second->text.c_str());
		}

		if (j == INVALID_TEXT_DRAW)
		{
			sampgdk::logprintf("%s pTextdraw suanda kullanilamiyor.", LOG);
		}
		else
		{
			if (p->second->lettersize_x != Item::def_pText.lettersize_x || p->second->lettersize_y != Item::def_pText.lettersize_y)
			{
				sampgdk_PlayerTextDrawLetterSize(playerid, j, p->second->lettersize_x, p->second->lettersize_y);
			}

			if (p->second->textsize_x != Item::def_pText.textsize_x || p->second->textsize_y != Item::def_pText.textsize_y)
			{
				sampgdk_PlayerTextDrawTextSize(playerid, j, p->second->textsize_x, p->second->textsize_y);
			}

			if (p->second->alignment != Item::def_pText.alignment)
			{
				sampgdk_PlayerTextDrawAlignment(playerid, j, p->second->alignment);
			}

			if (p->second->color != Item::def_pText.color)
			{
				sampgdk_PlayerTextDrawColor(playerid, j, p->second->color);
			}

			if (p->second->usebox != Item::def_pText.usebox)
			{
				sampgdk_PlayerTextDrawUseBox(playerid, j, p->second->usebox);
			}

			if (p->second->boxcolor != Item::def_pText.boxcolor)
			{
				sampgdk_PlayerTextDrawBoxColor(playerid, j, p->second->boxcolor);
			}

			if (p->second->shadow != Item::def_pText.shadow)
			{
				sampgdk_PlayerTextDrawSetShadow(playerid, j, p->second->shadow);
			}

			if (p->second->outline != Item::def_pText.outline)
			{
				sampgdk_PlayerTextDrawSetOutline(playerid, j, p->second->outline);
			}

			if (p->second->backgroundcolor != Item::def_pText.backgroundcolor)
			{
				sampgdk_PlayerTextDrawBackgroundColor(playerid, j, p->second->backgroundcolor);
			}

			if (p->second->font != Item::def_pText.font)
			{
				sampgdk_PlayerTextDrawFont(playerid, j, p->second->font);
			}

			if (p->second->proportional != Item::def_pText.proportional)
			{
				sampgdk_PlayerTextDrawSetProportional(playerid, j, p->second->proportional);
			}

			if (p->second->selectable != Item::def_pText.selectable)
			{
				sampgdk_PlayerTextDrawSetSelectable(playerid, j, p->second->selectable);
			}

			if (p->second->font == TEXT_DRAW_FONT_MODEL_PREVIEW)
			{
				if (p->second->modelindex != Item::def_pText.modelindex)
				{
					sampgdk_PlayerTextDrawSetPreviewModel(playerid, j, p->second->modelindex);
				}

				if (p->second->fRotX != Item::def_pText.fRotX || p->second->fRotY != Item::def_pText.fRotY || p->second->fRotZ != Item::def_pText.fRotZ || p->second->fZoom != Item::def_pText.fZoom)
				{
					sampgdk_PlayerTextDrawSetPreviewRot(playerid, j, p->second->fRotX, p->second->fRotY, p->second->fRotZ, p->second->fZoom);
				}

				if (p->second->veh_col1 != Item::def_pText.veh_col1 || p->second->veh_col2 != Item::def_pText.veh_col2)
				{
					sampgdk_PlayerTextDrawSetPreviewVehCol(playerid, j, p->second->veh_col1, p->second->veh_col2);
				}
			}
			sampgdk_PlayerTextDrawShow(playerid, j);
			p->second->real_id = j;
		}
	}
	else
	{
		sampgdk_PlayerTextDrawShow(playerid, p->second->real_id);
	}
}

cell AMX_NATIVE_CALL Natives::DestroyPTextdraw(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	if (!Item::pText[playerid].empty())
	{
		size_t text_id = static_cast<size_t>(params[2]);
		std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
		if (p != Item::pText[playerid].end())
		{
			if (LOG_MODE)
			{
				sampgdk::logprintf("%s pDestroy:: %d", LOG, text_id);
			}

			if (p->second->real_id != INVALID_DYNAMIC_TEXTDRAW)
			{
				sampgdk_PlayerTextDrawDestroy(playerid, p->second->real_id);
			}

			p->second->extra_id.clear();

			SlotManager::Remove_ID(playerid, text_id);
			Item::pText[playerid].erase(p);
		}
	}
	return 1;
}

cell AMX_NATIVE_CALL Natives::PTextLetterSize(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	if (!Item::pText[playerid].empty())
	{
		// Verileri �ek
		size_t text_id = static_cast<size_t>(params[2]);
		float x = amx_ctof(params[3]);
		float y = amx_ctof(params[4]);

		std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
		if (p != Item::pText[playerid].end())
		{
			// Verileri g�ncelle
			p->second->lettersize_x = x;
			p->second->lettersize_y = y;
			if (p->second->real_id != INVALID_DYNAMIC_TEXTDRAW)
			{
				sampgdk_PlayerTextDrawLetterSize(playerid, p->second->real_id, x, y);
			}

			if (LOG_MODE)
			{
				sampgdk::logprintf("%s pLetterSize:: X: %f, Y: %f", LOG, p->second->lettersize_x, p->second->lettersize_y);
			}
		}
	}
	return 1;
}

cell AMX_NATIVE_CALL Natives::PTextSize(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	if (!Item::pText[playerid].empty())
	{
		size_t text_id = static_cast<size_t>(params[2]);
		float x = amx_ctof(params[3]);
		float y = amx_ctof(params[4]);

		std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
		if (p != Item::pText[playerid].end())
		{
			p->second->textsize_x = x;
			p->second->textsize_y = y;
			if (p->second->real_id != INVALID_DYNAMIC_TEXTDRAW)
			{
				sampgdk_PlayerTextDrawTextSize(playerid, p->second->real_id, x, y);
			}

			if (LOG_MODE)
			{
				sampgdk::logprintf("%s pTextSize:: X: %f, Y: %f", LOG, p->second->textsize_x, p->second->textsize_y);
			}
		}
	}
	return 1;
}

cell AMX_NATIVE_CALL Natives::PTextAlignment(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	if (!Item::pText[playerid].empty())
	{
		size_t text_id = static_cast<size_t>(params[2]);
		size_t alignment = static_cast<size_t>(params[3]);

		std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
		if (p != Item::pText[playerid].end())
		{
			p->second->alignment = alignment;
			if (p->second->real_id != INVALID_DYNAMIC_TEXTDRAW)
			{
				sampgdk_PlayerTextDrawAlignment(playerid, p->second->real_id, alignment);
			}

			if (LOG_MODE)
			{
				sampgdk::logprintf("%s pAlignment:: %d", LOG, p->second->alignment);
			}
		}
	}
	return 1;
}

cell AMX_NATIVE_CALL Natives::PTextColor(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	if (!Item::pText[playerid].empty())
	{
		size_t text_id = static_cast<size_t>(params[2]);
		size_t color = static_cast<size_t>(params[3]);

		std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
		if (p != Item::pText[playerid].end())
		{
			p->second->color = color;
			if (p->second->real_id != INVALID_DYNAMIC_TEXTDRAW)
			{
				sampgdk_PlayerTextDrawColor(playerid, p->second->real_id, color);
			}

			if (LOG_MODE)
			{
				sampgdk::logprintf("%s pColor:: %d", LOG, p->second->color);
			}
		}
	}
	return 1;
}

cell AMX_NATIVE_CALL Natives::PTextUseBox(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	if (!Item::pText[playerid].empty())
	{
		size_t text_id = static_cast<size_t>(params[2]);
		size_t use = static_cast<size_t>(params[3]);

		std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
		if (p != Item::pText[playerid].end())
		{
			p->second->usebox = use;
			if (p->second->real_id != INVALID_DYNAMIC_TEXTDRAW)
			{
				sampgdk_PlayerTextDrawUseBox(playerid, p->second->real_id, use);
			}

			if (LOG_MODE)
			{
				sampgdk::logprintf("%s pUseBox:: %d", LOG, p->second->usebox);
			}
		}
	}
	return 1;
}

cell AMX_NATIVE_CALL Natives::PTextBoxColor(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	if (!Item::pText[playerid].empty())
	{
		size_t text_id = static_cast<size_t>(params[2]);
		size_t color = static_cast<size_t>(params[3]);

		std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
		if (p != Item::pText[playerid].end())
		{
			p->second->boxcolor = color;
			if (p->second->real_id != INVALID_DYNAMIC_TEXTDRAW)
			{
				sampgdk_PlayerTextDrawBoxColor(playerid, p->second->real_id, color);
			}

			if (LOG_MODE)
			{
				sampgdk::logprintf("%s pBoxColor:: %d", LOG, p->second->boxcolor);
			}
		}
	}
	return 1;
}

cell AMX_NATIVE_CALL Natives::PTextShadow(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	if (!Item::pText[playerid].empty())
	{
		size_t text_id = static_cast<size_t>(params[2]);
		size_t size = static_cast<size_t>(params[3]);

		std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
		if (p != Item::pText[playerid].end())
		{
			p->second->shadow = size;
			if (p->second->real_id != INVALID_DYNAMIC_TEXTDRAW)
			{
				sampgdk_PlayerTextDrawSetShadow(playerid, p->second->real_id, size);
			}

			if (LOG_MODE)
			{
				sampgdk::logprintf("%s pShadow:: %d", LOG, p->second->shadow);
			}
		}
	}
	return 1;
}

cell AMX_NATIVE_CALL Natives::PTextOutline(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	if (!Item::pText[playerid].empty())
	{
		size_t text_id = static_cast<size_t>(params[2]);
		size_t size = static_cast<size_t>(params[3]);

		std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
		if (p != Item::pText[playerid].end())
		{
			p->second->outline = size;
			if (p->second->real_id != INVALID_DYNAMIC_TEXTDRAW)
			{
				sampgdk_PlayerTextDrawSetOutline(playerid, p->second->real_id, size);
			}

			if (LOG_MODE)
			{
				sampgdk::logprintf("%s pOutline:: %d", LOG, p->second->outline);
			}
		}
	}
	return 1;
}

cell AMX_NATIVE_CALL Natives::PTextBGColor(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	if (!Item::pText[playerid].empty())
	{
		size_t text_id = static_cast<size_t>(params[2]);
		size_t color = static_cast<size_t>(params[3]);

		std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
		if (p != Item::pText[playerid].end())
		{
			p->second->backgroundcolor = color;
			if (p->second->real_id != INVALID_DYNAMIC_TEXTDRAW)
			{
				sampgdk_PlayerTextDrawBackgroundColor(playerid, p->second->real_id, color);
			}

			if (LOG_MODE)
			{
				sampgdk::logprintf("%s pBgColor:: %d", LOG, p->second->backgroundcolor);
			}
		}
	}
	return 1;
}

cell AMX_NATIVE_CALL Natives::PTextFont(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	if (!Item::pText[playerid].empty())
	{
		size_t text_id = static_cast<size_t>(params[2]);
		size_t font = static_cast<size_t>(params[3]);

		std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
		if (p != Item::pText[playerid].end())
		{
			p->second->font = font;
			if (p->second->real_id != INVALID_DYNAMIC_TEXTDRAW)
			{
				sampgdk_PlayerTextDrawFont(playerid, p->second->real_id, font);
			}

			if (LOG_MODE)
			{
				sampgdk::logprintf("%s pFont:: %d", LOG, p->second->font);
			}
		}
	}
	return 1;
}

cell AMX_NATIVE_CALL Natives::PTextProportional(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	if (!Item::pText[playerid].empty())
	{
		size_t text_id = static_cast<size_t>(params[2]);
		size_t set = static_cast<size_t>(params[3]);

		std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
		if (p != Item::pText[playerid].end())
		{
			p->second->proportional = set;
			if (p->second->real_id != INVALID_DYNAMIC_TEXTDRAW)
			{
				sampgdk_PlayerTextDrawSetProportional(playerid, p->second->real_id, set);
			}

			if (LOG_MODE)
			{
				sampgdk::logprintf("%s pProportional:: %d", LOG, p->second->proportional);
			}
		}
	}
	return 1;
}

cell AMX_NATIVE_CALL Natives::PTextSelectable(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	if (!Item::pText[playerid].empty())
	{
		size_t text_id = static_cast<size_t>(params[2]);
		size_t set = static_cast<size_t>(params[3]);

		std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
		if (p != Item::pText[playerid].end())
		{
			p->second->selectable = set;
			if (p->second->real_id != INVALID_DYNAMIC_TEXTDRAW)
			{
				sampgdk_PlayerTextDrawSetSelectable(playerid, p->second->real_id, set);
			}

			if (LOG_MODE)
			{
				sampgdk::logprintf("%s pSelectable:: %d", LOG, p->second->selectable);
			}
		}
	}
	return 1;
}

cell AMX_NATIVE_CALL Natives::PTextShow(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	if (!Item::pText[playerid].empty())
	{
		size_t text_id = static_cast<size_t>(params[2]);
		std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
		if (p != Item::pText[playerid].end())
		{
			ShowDynTextDraw(playerid, p->first);

			if (LOG_MODE)
			{
				sampgdk::logprintf("%s pShow", LOG);
			}
		}
	}
	return 1;
}

cell AMX_NATIVE_CALL Natives::PTextHide(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	if (!Item::pText[playerid].empty())
	{
		size_t text_id = static_cast<size_t>(params[2]);
		std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
		if (p != Item::pText[playerid].end())
		{
			if (p->second->real_id != INVALID_DYNAMIC_TEXTDRAW)
			{
				sampgdk_PlayerTextDrawDestroy(playerid, p->second->real_id);
				p->second->real_id = INVALID_DYNAMIC_TEXTDRAW;
			}

			if (LOG_MODE)
			{
				sampgdk::logprintf("%s pHide", LOG);
			}
		}
	}
	return 1;
}

cell AMX_NATIVE_CALL Natives::PTextSetString(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	if (!Item::pText[playerid].empty())
	{
		size_t text_id = static_cast<size_t>(params[2]);
		std::string str = Servis::Get_String(amx, params[3]);

		std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
		if (p != Item::pText[playerid].end())
		{
			p->second->setstring = str;
			if (p->second->real_id != INVALID_DYNAMIC_TEXTDRAW)
			{
				sampgdk_PlayerTextDrawSetString(playerid, p->second->real_id, str.c_str());
			}

			if (LOG_MODE)
			{
				sampgdk::logprintf("%s pSetString:: %s", LOG, p->second->setstring.c_str());
			}
		}
	}
	return 1;
}

cell AMX_NATIVE_CALL Natives::PTextPreviewModel(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	if (!Item::pText[playerid].empty())
	{
		size_t text_id = static_cast<size_t>(params[2]);
		size_t modelindex = static_cast<size_t>(params[3]);

		std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
		if (p != Item::pText[playerid].end())
		{
			p->second->modelindex = modelindex;
			if (p->second->real_id != INVALID_DYNAMIC_TEXTDRAW)
			{
				sampgdk_PlayerTextDrawSetPreviewModel(playerid, p->second->real_id, modelindex);
			}

			if (LOG_MODE)
			{
				sampgdk::logprintf("%s pPreviewModel:: %d", LOG, p->second->modelindex);
			}
		}
	}
	return 1;
}

cell AMX_NATIVE_CALL Natives::PTextPreviewRot(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	if (!Item::pText[playerid].empty())
	{
		size_t text_id = static_cast<size_t>(params[2]);
		float x = amx_ctof(params[3]);
		float y = amx_ctof(params[4]);
		float z = amx_ctof(params[5]);
		float zoom = amx_ctof(params[6]);

		std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
		if (p != Item::pText[playerid].end())
		{
			p->second->fRotX = x;
			p->second->fRotY = y;
			p->second->fRotZ = z;
			p->second->fZoom = zoom;
			if (p->second->real_id != INVALID_DYNAMIC_TEXTDRAW)
			{
				sampgdk_PlayerTextDrawSetPreviewRot(playerid, p->second->real_id, x, y, z, zoom);
			}

			if (LOG_MODE)
			{
				sampgdk::logprintf("%s pPreviewRot:: X: %f, Y: %f, Z: %f, Zoom: %f", LOG, p->second->fRotX, p->second->fRotY, p->second->fRotZ, p->second->fZoom);
			}
		}
	}
	return 1;
}

cell AMX_NATIVE_CALL Natives::PTextPreviewVehCol(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	if (!Item::pText[playerid].empty())
	{
		size_t text_id = static_cast<size_t>(params[2]);
		size_t veh_color1 = static_cast<size_t>(params[3]);
		size_t veh_color2 = static_cast<size_t>(params[4]);

		std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
		if (p != Item::pText[playerid].end())
		{
			p->second->veh_col1 = veh_color1;
			p->second->veh_col2 = veh_color2;

			if (p->second->real_id != INVALID_DYNAMIC_TEXTDRAW)
			{
				sampgdk_PlayerTextDrawSetPreviewVehCol(playerid, p->second->real_id, veh_color1, veh_color2);
			}

			if (LOG_MODE)
			{
				sampgdk::logprintf("%s pVehCol:: Color1: %d, Color2: %d", LOG, p->second->veh_col1, p->second->veh_col2);
			}
		}
	}
	return 1;
}

#pragma endregion

#pragma region VERI OKUMA

cell AMX_NATIVE_CALL Natives::IsValidPlayerTextDraw(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	size_t text_id = static_cast<size_t>(params[2]);
	std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
	if (p != Item::pText[playerid].end())
	{
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::IsPlayerTextDrawVisible(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	size_t text_id = static_cast<size_t>(params[2]);
	std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
	if (p != Item::pText[playerid].end())
	{
		if (p->second->real_id != INVALID_DYNAMIC_TEXTDRAW)
		{
			return 1;
		}
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::PlayerTextDrawGetString(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	size_t text_id = static_cast<size_t>(params[2]);
	std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
	if (p != Item::pText[playerid].end())
	{
		if (p->second->setstring.length() != 0)
		{
			Servis::Native_String(amx, params[3], params[4], p->second->setstring);
		}
		else
		{
			Servis::Native_String(amx, params[3], params[4], p->second->text);
		}
	}
	return 1;
}

cell AMX_NATIVE_CALL Natives::PlayerTextDrawSetPos(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	size_t text_id = static_cast<size_t>(params[2]);
	std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
	if (p != Item::pText[playerid].end())
	{
		if (p->second->real_id != INVALID_DYNAMIC_TEXTDRAW)
		{
			sampgdk_PlayerTextDrawDestroy(playerid, p->second->real_id);

			p->second->real_id = INVALID_DYNAMIC_TEXTDRAW;

			p->second->create_x = amx_ctof(params[3]);
			p->second->create_y = amx_ctof(params[4]);

			AMX_NATIVE callback = sampgdk::FindNative("PTextShow");
			if (callback != nullptr)
			{
				sampgdk::InvokeNative(callback, "dd", playerid, text_id);
			}
		}
		else
		{
			p->second->create_x = amx_ctof(params[3]);
			p->second->create_y = amx_ctof(params[4]);
		}
	}
	return 1;
}

cell AMX_NATIVE_CALL Natives::PlayerTextDrawGetLetterSize(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	size_t text_id = static_cast<size_t>(params[2]);
	std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
	if (p != Item::pText[playerid].end())
	{
		Servis::Native_SetFloat(amx, params[3], p->second->lettersize_x);
		Servis::Native_SetFloat(amx, params[4], p->second->lettersize_y);
	}
	return 1;
}

cell AMX_NATIVE_CALL Natives::PlayerTextDrawGetTextSize(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	size_t text_id = static_cast<size_t>(params[2]);
	std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
	if (p != Item::pText[playerid].end())
	{
		Servis::Native_SetFloat(amx, params[3], p->second->textsize_x);
		Servis::Native_SetFloat(amx, params[4], p->second->textsize_y);
	}
	return 1;
}

cell AMX_NATIVE_CALL Natives::PlayerTextDrawGetPos(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	size_t text_id = static_cast<size_t>(params[2]);
	std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
	if (p != Item::pText[playerid].end())
	{
		Servis::Native_SetFloat(amx, params[3], p->second->create_x);
		Servis::Native_SetFloat(amx, params[4], p->second->create_y);
	}
	return 1;
}

cell AMX_NATIVE_CALL Natives::PlayerTextDrawGetColor(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	size_t text_id = static_cast<size_t>(params[2]);
	std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
	if (p != Item::pText[playerid].end())
	{
		return p->second->color;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::PlayerTextDrawGetBoxColor(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	size_t text_id = static_cast<size_t>(params[2]);
	std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
	if (p != Item::pText[playerid].end())
	{
		return p->second->boxcolor;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::PlayerTextDrawGetBackgroundCol(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	size_t text_id = static_cast<size_t>(params[2]);
	std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
	if (p != Item::pText[playerid].end())
	{
		return p->second->backgroundcolor;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::PlayerTextDrawGetShadow(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	size_t text_id = static_cast<size_t>(params[2]);
	std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
	if (p != Item::pText[playerid].end())
	{
		return p->second->shadow;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::PlayerTextDrawGetOutline(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	size_t text_id = static_cast<size_t>(params[2]);
	std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
	if (p != Item::pText[playerid].end())
	{
		return p->second->outline;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::PlayerTextDrawGetFont(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	size_t text_id = static_cast<size_t>(params[2]);
	std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
	if (p != Item::pText[playerid].end())
	{
		return p->second->font;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::PlayerTextDrawIsBox(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	size_t text_id = static_cast<size_t>(params[2]);
	std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
	if (p != Item::pText[playerid].end())
	{
		return p->second->usebox;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::PlayerTextDrawIsProportional(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	size_t text_id = static_cast<size_t>(params[2]);
	std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
	if (p != Item::pText[playerid].end())
	{
		return p->second->proportional;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::PlayerTextDrawIsSelectable(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	size_t text_id = static_cast<size_t>(params[2]);
	std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
	if (p != Item::pText[playerid].end())
	{
		return p->second->selectable;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::PlayerTextDrawGetAlignment(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	size_t text_id = static_cast<size_t>(params[2]);
	std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
	if (p != Item::pText[playerid].end())
	{
		return p->second->alignment;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::PlayerTextDrawGetPreviewModel(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	size_t text_id = static_cast<size_t>(params[2]);
	std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
	if (p != Item::pText[playerid].end())
	{
		return p->second->modelindex;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::PlayerTextDrawGetPreviewRot(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	size_t text_id = static_cast<size_t>(params[2]);
	std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
	if (p != Item::pText[playerid].end())
	{
		Servis::Native_SetFloat(amx, params[3], p->second->fRotX);
		Servis::Native_SetFloat(amx, params[4], p->second->fRotY);
		Servis::Native_SetFloat(amx, params[5], p->second->fRotZ);
		Servis::Native_SetFloat(amx, params[6], p->second->fZoom);
	}
	return 1;
}

cell AMX_NATIVE_CALL Natives::PlayerTextDrawGetPreviewVehCol(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	size_t text_id = static_cast<size_t>(params[2]);
	std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
	if (p != Item::pText[playerid].end())
	{
		Servis::Native_SetInt(amx, params[3], p->second->veh_col1);
		Servis::Native_SetInt(amx, params[4], p->second->veh_col2);
	}
	return 1;
}

#pragma endregion

cell AMX_NATIVE_CALL Natives::PlayerTextDrawSetExtraID(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	size_t text_id = static_cast<size_t>(params[2]);

	int index = static_cast<size_t>(params[3]);
	int value = static_cast<size_t>(params[4]);

	std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
	if (p != Item::pText[playerid].end())
	{
		p->second->extra_id[index] = value;
	}
	return 1;
}

cell AMX_NATIVE_CALL Natives::PlayerTextDrawGetExtraID(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	size_t text_id = static_cast<size_t>(params[2]);
	int index = static_cast<size_t>(params[3]);
	std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
	if (p != Item::pText[playerid].end())
	{
		return p->second->extra_id[index];
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::PlayerTextDrawResetExtraID(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	size_t text_id = static_cast<size_t>(params[2]);
	std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
	if (p != Item::pText[playerid].end())
	{
		p->second->extra_id.clear();
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::PlayerTextDrawTotalCreate(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	if (Item::pText[playerid].empty())
	{
		return 0;
	}
	return Item::pText[playerid].size();
}

cell AMX_NATIVE_CALL Natives::PlayerTextDrawSetGroup(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	if (Item::pText[playerid].empty())
		return false;

	size_t text_id = static_cast<size_t>(params[2]);
	std::string str = Servis::Get_String(amx, params[3]);

	std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].find(text_id);
	if (p == Item::pText[playerid].end())
		return false;

	p->second->function = str;
	p->second->functionIdx = static_cast<size_t>(params[4]);

	if (LOG_MODE)
	{
		sampgdk::logprintf("%s PlayerTextDrawSetGroup:: %s", LOG, p->second->setstring.c_str());
	}
	return true;
}

cell AMX_NATIVE_CALL Natives::ShowDynTextDrawGroup(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	if (!Item::pText[playerid].empty())
	{
		std::string str = Servis::Get_String(amx, params[2]);
		for (std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].begin(); p != Item::pText[playerid].end(); p++)
		{
			if (p->second->function.compare(str) == 0)
				ShowDynTextDraw(playerid, p->first);
		}
		if (LOG_MODE)
		{
			sampgdk::logprintf("%s ShowDynTextDrawGroup", LOG);
		}
	}
	return 1;
}

cell AMX_NATIVE_CALL Natives::HideDynTextDrawGroup(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	if (!Item::pText[playerid].empty())
	{
		std::string str = Servis::Get_String(amx, params[2]);
		for (std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].begin(); p != Item::pText[playerid].end(); p++)
		{
			if (p->second->function.compare(str) == 0)
			{
				if (p->second->real_id != INVALID_DYNAMIC_TEXTDRAW)
				{
					sampgdk_PlayerTextDrawDestroy(playerid, p->second->real_id);
					p->second->real_id = INVALID_DYNAMIC_TEXTDRAW;
				}
			}
		}
	}
	return 1;
}

cell AMX_NATIVE_CALL Natives::DestroyDynTextDrawGroup(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	if (!Item::pText[playerid].empty())
	{
		std::string str = Servis::Get_String(amx, params[2]);

		for (std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].begin(); p != Item::pText[playerid].end(); )
		{
			if (p->second->function.length() != 0 && p->second->function.compare(str) == 0)
			{
				if (p->second->real_id != INVALID_DYNAMIC_TEXTDRAW)
				{
					sampgdk_PlayerTextDrawHide(playerid, p->second->real_id);
					sampgdk_PlayerTextDrawDestroy(playerid, p->second->real_id);
					p->second->real_id = INVALID_DYNAMIC_TEXTDRAW;
				}

				p->second->extra_id.clear();

				SlotManager::Remove_ID(playerid, p->first);
				p = Item::pText[playerid].erase(p);
			}
			else
				p++;
		}

		if (LOG_MODE)
		{
			sampgdk::logprintf("%s DestroyDynTextDrawGroup::", LOG);
		}
	}
	return true;
}

cell AMX_NATIVE_CALL Natives::GetTextDrawInRangeOfPoint(AMX* amx, cell* params)
{
	size_t playerid = static_cast<size_t>(params[1]);
	double center_x = amx_ctof(params[2]);
	double center_y = amx_ctof(params[3]);
	double radi = amx_ctof(params[4]);

	double d = 0;

	for (std::unordered_map<int, std::unique_ptr<PlayerText>>::iterator p = Item::pText[playerid].begin(); p != Item::pText[playerid].end(); p++)
	{
		d = pow(radi, 2.0) - pow((center_x - p->second->create_x), 2.0) + pow((center_y - p->second->create_y), 2.0);

		if (d < 0.0)
			continue;

		return p->first;
	}
	return -1;
}
