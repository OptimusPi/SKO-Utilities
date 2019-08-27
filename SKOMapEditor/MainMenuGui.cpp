#include "MainMenuGui.h"

MainMenuGui::MainMenuGui(OPI_Gui::GuiManager *guiManager)
{
	this->guiManager = guiManager;
	setupCoordinates();
	setupHotBar();
}

MainMenuGui::~MainMenuGui()
{

}

void MainMenuGui::setCoords(std::string coordsValue)
{
	this->coords->setText(coordsValue);
}


void MainMenuGui::setupHotBar()
{
	auto *buttonAddTile = new OPI_Gui::Button("")
}

void MainMenuGui::setupCoordinates()
{
	this->coords = new OPI_Text("x, z, y", OPI_FontManager::getFont("RobotoMono-Regular"));

	// Set up coordinates text
	this->coords->setText("0,0", nullptr);
	this->coords->visible = true;
	this->coords->x = 2;
	this->coords->y = 2;
	this->coords->R = 1.0f;
	this->coords->G = 1.0f;
	this->coords->B = 1.0f;

	// Set up text label
	OPI_Gui::TextLabel * coordsLabel = new OPI_Gui::TextLabel(this->coords->x, this->coords->y, this->coords);

	// Add coords to gui manager
	this->guiManager->addElement(coordsLabel);
}

