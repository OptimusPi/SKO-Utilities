#ifndef __MAINMENUGUI_H_
#define __MAINMENUGUI_H_

#include "OPI_GuiManager.h"
#include "OPI_FontManager.h"
#include "OPI_GuiTextLabel.h"
#include "OPI_GuiButton.h"

class MainMenuGui
{
public:
	MainMenuGui(OPI_Gui::GuiManager *guiManager);
	virtual ~MainMenuGui();

	// Helper functions
	void setCoords(std::string coordsValue);
private:
	OPI_Gui::GuiManager * guiManager = nullptr;
	OPI_Text* coords = nullptr;
	OPI_Gui::TextLabel * coordsLabel;
	void setupHotBar();
	void setupCoordinates();
};

#endif
