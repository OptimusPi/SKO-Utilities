#ifndef __MAINMENUGUI_H_
#define __MAINMENUGUI_H_

#include "GuiManager.h"
#include "FontManager.h"
#include "TextLabel.h"
#include "Button.h"
#include "Panel.h"
#include <functional>
#include "Global.h"
#include "ButtonToggleGroup.h"

class MainMenuGui
{
public:
	MainMenuGui(OPI_Gui::GuiManager *guiManager);
	virtual ~MainMenuGui();

	// Helper functions
	void setCoords(std::string coordsValue);
private:
	OPI_Gui::GuiManager * guiManager = nullptr;
	OPI_Text::TextComponent* coords = nullptr;
	OPI_Gui::TextLabel * coordsLabel;

	// Create Gui Elements
	void setupHotBar();
	void setupLayersHotbar();
	void setupCoordinates();
	void drawGui();

	// Helper callback functions
	void setEditorMode(char setting);
	void toggleEditorFringe();
	void toggleEditorFringeMask();
	void toggleEditorBackground();
	void toggleEditorBackgroundMask();
};

#endif
