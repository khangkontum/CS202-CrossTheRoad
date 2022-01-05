#pragma once

#include "../lib/olcPixelGameEngine.h"
#include "../lib/json.hpp"
#include <unordered_map>
#include "AudioManager.h"
#include "para.h"
#include "SaveData.h"

constexpr int32_t nPatch = 8;


class menuobject
{
public:
	menuobject()
	{
		sName = "root";
	}

	menuobject(const std::string& name)
	{
		sName = name;
	}

	menuobject& SetTable(int nColumns, int nRows)
	{
		vCellTable = { nColumns, nRows };	return *this;
	}

	menuobject& SetID(int32_t id)
	{
		nID = id; return *this;
	}

	int32_t GetID()
	{
		return nID;
	}

	std::string& GetName()
	{
		return sName;
	}

	menuobject& Enable(bool b)
	{
		bEnabled = b; return *this;
	}

	bool Enabled()
	{
		return bEnabled;
	}

	bool HasChildren()
	{
		return !items.empty();
	}

	// For now, cells are simply one line strings
	olc::vi2d GetSize()
	{
		return { int32_t(sName.size()), 1 };
	}

	olc::vi2d& GetCursorPosition()
	{
		return vCursorPos;
	}

	menuobject& operator[](const std::string& name)
	{
		if (itemPointer.count(name) == 0)
		{
			itemPointer[name] = items.size();
			items.push_back(menuobject(name));
		}

		return items[itemPointer[name]];
	}

	void Build()
	{
		// Recursively build all children, so they can determine their size, use
		// that size to indicate cell sizes if this object contains more than 
		// one item
		for (auto& m : items)
		{
			if (m.HasChildren())
			{
				m.Build();
			}

			// Longest child name determines cell width
			vCellSize.x = std::max(m.GetSize().x, vCellSize.x);
			vCellSize.y = std::max(m.GetSize().y, vCellSize.y);
		}

		// Adjust size of this object (in patches) if it were rendered as a panel
		vSizeInPatches.x = vCellTable.x * vCellSize.x + (vCellTable.x - 1) * vCellPadding.x + 2;
		vSizeInPatches.y = vCellTable.y * vCellSize.y + (vCellTable.y - 1) * vCellPadding.y + 2;

		// Calculate how many rows this item has to hold
		nTotalRows = (items.size() / vCellTable.x) + (((items.size() % vCellTable.x) > 0) ? 1 : 0);
	}

	void DrawSelf(olc::PixelGameEngine& pge, olc::Sprite* sprGFX, olc::vi2d vScreenOffset)
	{
		// === Draw Panel

		// Record current pixel mode user is using
		pge.Clear(olc::SOFT_GREEN);
		olc::Pixel::Mode currentPixelMode = pge.GetPixelMode();
		pge.SetPixelMode(olc::Pixel::MASK);

		// Draw Panel & Border
		olc::vi2d vPatchPos = { 0,0 };
		for (vPatchPos.x = 0; vPatchPos.x < vSizeInPatches.x; vPatchPos.x++)
		{
			for (vPatchPos.y = 0; vPatchPos.y < vSizeInPatches.y; vPatchPos.y++)
			{
				// Determine position in screen space
				olc::vi2d vScreenLocation = vPatchPos * nPatch + vScreenOffset;

				// Calculate which patch is needed
				olc::vi2d vSourcePatch = { 0, 0 };
				if (vPatchPos.x > 0) vSourcePatch.x = 1;
				if (vPatchPos.x == vSizeInPatches.x - 1) vSourcePatch.x = 2;
				if (vPatchPos.y > 0) vSourcePatch.y = 1;
				if (vPatchPos.y == vSizeInPatches.y - 1) vSourcePatch.y = 2;

				// Draw Actual Patch
				pge.DrawPartialSprite(vScreenLocation, sprGFX, vSourcePatch * nPatch, vPatchSize);
			}
		}

		// === Draw Panel Contents
		olc::vi2d vCell = { 0,0 };
		vPatchPos = { 1,1 };

		// Work out visible items
		int32_t nTopLeftItem = nTopVisibleRow * vCellTable.x;
		int32_t nBottomRightItem = vCellTable.y * vCellTable.x + nTopLeftItem;

		// Clamp to size of child item vector
		nBottomRightItem = std::min(int32_t(items.size()), nBottomRightItem);
		int32_t nVisibleItems = nBottomRightItem - nTopLeftItem;

		// Draw Scroll Markers (if required)
		if (nTopVisibleRow > 0)
		{
			vPatchPos = { vSizeInPatches.x - 2, 0 };
			olc::vi2d vScreenLocation = vPatchPos * nPatch + vScreenOffset;
			olc::vi2d vSourcePatch = { 3, 0 };
			pge.DrawPartialSprite(vScreenLocation, sprGFX, vSourcePatch * nPatch, vPatchSize);
		}

		if ((nTotalRows - nTopVisibleRow) > vCellTable.y)
		{
			vPatchPos = { vSizeInPatches.x - 2, vSizeInPatches.y - 1 };
			olc::vi2d vScreenLocation = vPatchPos * nPatch + vScreenOffset;
			olc::vi2d vSourcePatch = { 3, 2 };
			pge.DrawPartialSprite(vScreenLocation, sprGFX, vSourcePatch * nPatch, vPatchSize);
		}

		// Draw Visible Items
		for (int32_t i = 0; i < nVisibleItems; i++)
		{
			// Cell location
			vCell.x = i % vCellTable.x;
			vCell.y = i / vCellTable.x;

			// Patch location (including border offset and padding)
			vPatchPos.x = vCell.x * (vCellSize.x + vCellPadding.x) + 1;
			vPatchPos.y = vCell.y * (vCellSize.y + vCellPadding.y) + 1;

			// Actual screen location in pixels
			olc::vi2d vScreenLocation = vPatchPos * nPatch + vScreenOffset;

			// Display Item Header
			pge.DrawString(vScreenLocation, items[nTopLeftItem + i].sName, items[nTopLeftItem + i].bEnabled ? olc::WHITE : olc::DARK_GREY);

			if (items[nTopLeftItem + i].HasChildren())
			{
				// Display Indicator that panel has a sub panel
				vPatchPos.x = vCell.x * (vCellSize.x + vCellPadding.x) + 1 + vCellSize.x;
				vPatchPos.y = vCell.y * (vCellSize.y + vCellPadding.y) + 1;
				olc::vi2d vSourcePatch = { 3, 1 };
				vScreenLocation = vPatchPos * nPatch + vScreenOffset;
				pge.DrawPartialSprite(vScreenLocation, sprGFX, vSourcePatch * nPatch, vPatchSize);
			}
		}

		// Calculate cursor position in screen space in case system draws it
		vCursorPos.x = (vCellCursor.x * (vCellSize.x + vCellPadding.x)) * nPatch + vScreenOffset.x - nPatch;
		vCursorPos.y = ((vCellCursor.y - nTopVisibleRow) * (vCellSize.y + vCellPadding.y)) * nPatch + vScreenOffset.y + nPatch;
	}

	void ClampCursor()
	{
		// Find item in children
		nCursorItem = vCellCursor.y * vCellTable.x + vCellCursor.x;

		// Clamp Cursor
		if (nCursorItem >= int32_t(items.size()))
		{
			vCellCursor.y = (items.size() / vCellTable.x);
			vCellCursor.x = (items.size() % vCellTable.x) - 1;
			nCursorItem = items.size() - 1;
		}
	}

	void OnUp()
	{
		vCellCursor.y--;
		if (vCellCursor.y < 0) vCellCursor.y = 0;

		if (vCellCursor.y < nTopVisibleRow)
		{
			nTopVisibleRow--;
			if (nTopVisibleRow < 0) nTopVisibleRow = 0;
		}

		ClampCursor();
	}

	void OnDown()
	{
		vCellCursor.y++;
		if (vCellCursor.y == nTotalRows) vCellCursor.y = nTotalRows - 1;

		if (vCellCursor.y > (nTopVisibleRow + vCellTable.y - 1))
		{
			nTopVisibleRow++;
			if (nTopVisibleRow > (nTotalRows - vCellTable.y))
				nTopVisibleRow = nTotalRows - vCellTable.y;
		}

		ClampCursor();
	}

	void OnLeft()
	{
		vCellCursor.x--;
		if (vCellCursor.x < 0) vCellCursor.x = 0;
		ClampCursor();
	}

	void OnRight()
	{
		vCellCursor.x++;
		if (vCellCursor.x == vCellTable.x) vCellCursor.x = vCellTable.x - 1;
		ClampCursor();
	}

	menuobject* OnConfirm()
	{
		// Check if selected item has children
		if (items[nCursorItem].HasChildren())
		{
			return &items[nCursorItem];
		}
		else
			return this;
	}

	menuobject* GetSelectedItem()
	{
		return &items[nCursorItem];
	}


protected:
	int32_t nID = -1;
	olc::vi2d vCellTable = { 1, 0 };
	std::unordered_map<std::string, size_t> itemPointer;
	std::vector<menuobject> items;
	olc::vi2d vSizeInPatches = { 0, 0 };
	olc::vi2d vCellSize = { 0, 0 };
	olc::vi2d vCellPadding = { 2, 0 };
	olc::vi2d vCellCursor = { 0, 0 };
	int32_t nCursorItem = 0;
	int32_t nTopVisibleRow = 0;
	int32_t nTotalRows = 0;
	const olc::vi2d vPatchSize = { nPatch, nPatch };
	std::string sName;
	olc::vi2d vCursorPos = { 0, 0 };
	bool bEnabled = true;
};


class menumanager
{
public:
	menumanager() { }

	void Open(menuobject* mo) { Close(); panels.push_back(mo); }
	void Close() { panels.clear(); }

	void OnUp() { if (!panels.empty()) panels.back()->OnUp(); }
	void OnDown() { if (!panels.empty()) panels.back()->OnDown(); }
	void OnLeft() { if (!panels.empty()) panels.back()->OnLeft(); }
	void OnRight() { if (!panels.empty()) panels.back()->OnRight(); }
	void OnBack() { if (!panels.empty() && panels.size() > 1) panels.pop_back(); }
	void Reset() { while (!panels.empty() && panels.size() > 1) panels.pop_back(); }

	menuobject* OnConfirm()
	{
		if (panels.empty()) return nullptr;

		menuobject* next = panels.back()->OnConfirm();
		if (next == panels.back())
		{
			if (panels.back()->GetSelectedItem()->Enabled())
				return panels.back()->GetSelectedItem();
		}
		else
		{
			if (next->Enabled())
				panels.push_back(next);
		}

		return nullptr;
	}

	void Draw(olc::PixelGameEngine& pge, olc::Sprite* sprGFX, olc::vi2d vScreenOffset)
	{ 
		if (panels.empty()) return;


		//AudioManager->play("MENU", Sound, false);
		// Draw Visible Menu System
		for (auto& p : panels)
		{
			p->DrawSelf(pge, sprGFX, vScreenOffset);
			//vScreenOffset += {10, 10};
		}

		// Draw Cursor
		olc::Pixel::Mode currentPixelMode = pge.GetPixelMode();
		pge.SetPixelMode(olc::Pixel::ALPHA);
		pge.DrawPartialSprite(panels.back()->GetCursorPosition(), sprGFX, olc::vi2d(4, 0) * nPatch, { nPatch * 2, nPatch * 2 });
		pge.SetPixelMode(currentPixelMode);
	}

private:
	std::list<menuobject*> panels;
};

#include "Level.h"

using json = nlohmann::json;

class Menu {
private:
	class MenuState
	{
	public:
		virtual void build(menuobject& menu) = 0;
		virtual menuobject& open(menuobject& menu) = 0;
		virtual int interact(int key, menumanager* manager, bool& stop, json* gameConfig, std::string* configPath, Level* level) = 0;
	protected: 
		AudioManager* AudioManager = AudioManager->getAudioManager();
	};
	class Loading : public Menu::MenuState
	{
	public:
		void build(menuobject& menu);
		int interact(int key, menumanager* manager, bool& stop, json* gameConfig, std::string* configPath, Level* level);
		menuobject& open(menuobject& menu);
	};
	class Ingame : public Menu::MenuState
	{
	public:
		void build(menuobject& menu);
		int interact(int key, menumanager* manager, bool& stop, json* gameConfig, std::string* configPath, Level* level);
		menuobject& open(menuobject& menu);
	};
	class Settings : public Menu::MenuState
	{
	public:
		void build(menuobject& menu);
		int interact(int key, menumanager* manager, bool& stop, json* gameConfig, std::string* configPath, Level* level);
		menuobject& open(menuobject& menu);
	};

public:
	void draw();
	int interact(bool& isIngame, bool& stop);
	void init(olc::PixelGameEngine* pge, json* gameConfig, std::string* configPath);
	static Menu& getInstance();
	

private:
	Menu();
	~Menu();
	Menu(const Menu& other) {}
	static Menu* instance;
	bool execute(int command, bool& isIngame);
	olc::PixelGameEngine* pge;
	olc::Sprite* sprite;
	json* gameConfig;
	std::string* configPath;
	menumanager manager;
	menuobject menu;
	AudioManager* AudioManager = AudioManager->getAudioManager();

	static MenuState* MenuLoading;
	static MenuState* MenuSettings;
	static MenuState* MenuIngame;
	static MenuState* MenuCurrent;
};
