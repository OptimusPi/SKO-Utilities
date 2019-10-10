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

void MainMenuGui::drawGui()
{
	for (OPI_Gui::Element* child : this->guiManager->children) {
		if (child->isVisible)
			this->guiManager->drawElement(0, 0, child);
	}
}

void MainMenuGui::setupHotBar()
{
	// 9 buttons with dimensions 64x64
	auto panelWidth = 9 * 64;
	auto panelHeight = 1 * 64 + 16;
	// center horizontally, place at bottom of the screen
	auto panelX = guiManager->getScreenWidth()/2 - panelWidth/2;
	auto panelY = guiManager->getScreenHeight() - panelHeight;
	// Create invisible panel 
	//auto hotbarPanel = new OPI_Gui::Panel(panelX, panelY, panelWidth, panelHeight);
	auto hotbarPanel = new OPI_Gui::Panel(OPI_Gui::ElementThemeType::GridRect, "ice", panelX, panelY, panelWidth, panelHeight);
	hotbarPanel->isClosable = false;
	hotbarPanel->isMovable = true;
	hotbarPanel->isResizable = false;

	
	// Create the 8 main menu buttons with actions
	auto *buttonAddTile = new OPI_Gui::Button("addTile", 64 * 0, 16);
	buttonAddTile->isToggle = true;
	buttonAddTile->addCallback([this]() {
		setEditorMode(TILE_DRAW);
	});
	auto *buttonDeleteTile = new OPI_Gui::Button("deleteTile", 64 * 1, 16);
	buttonDeleteTile->isToggle = true;
	buttonDeleteTile->addCallback([this]() {
		setEditorMode(TILE_DELETE);
	});
	auto *buttonToggleFringe = new OPI_Gui::Button("toggleFringe", 64 * 2, 16);
	buttonToggleFringe->isToggle = true;
	buttonToggleFringe->addCallback([this]() {
		toggleEditorFringe();
	});
	auto *buttonAddCollision = new OPI_Gui::Button("addCollision", 64 * 3, 16);
	buttonAddCollision->isToggle = true;
	buttonAddCollision->addCallback([this]() {
		setEditorMode(COLLISION_DRAW);
	});
	auto *buttonDeleteCollision = new OPI_Gui::Button("deleteCollision", 64 * 4, 16);
	buttonDeleteCollision->isToggle = true;
	buttonDeleteCollision->addCallback([this]() {
		setEditorMode(COLLISION_DELETE);
	});
	auto *buttonToggleTest = new OPI_Gui::Button("toggleTest", 64 * 5, 16);
	buttonToggleTest->isToggle = true;
	buttonToggleTest->addCallback([this]() {
		setEditorMode(TOGGLE_TEST);
	});
	auto *buttonEditTile = new OPI_Gui::Button("editTile", 64 * 6, 16);
	buttonEditTile->isToggle = true;
	buttonEditTile->addCallback([this]() {
		setEditorMode(EDIT_TILE);
	});
	auto *buttonEditCollision = new OPI_Gui::Button("editCollision", 64 * 7, 16);
	buttonEditCollision->isToggle = true;
	buttonEditCollision->addCallback([this]() {
		setEditorMode(EDIT_COLLISION);
	});
	auto *buttonSave = new OPI_Gui::Button("save", 64 * 8, 16);
	buttonSave->addCallback([this]() {
		saveMap();
	});


	// Consider these buttons as radio button group
	new OPI_Gui::ButtonToggleGroup({ buttonAddTile, buttonDeleteTile, buttonEditTile });

	// Add the 8 buttons to the hotbar panel
	hotbarPanel->addElement(buttonAddTile);
	hotbarPanel->addElement(buttonDeleteTile);
	hotbarPanel->addElement(buttonToggleFringe);
	hotbarPanel->addElement(buttonAddCollision);
	hotbarPanel->addElement(buttonDeleteCollision);
	hotbarPanel->addElement(buttonToggleTest);
	hotbarPanel->addElement(buttonEditTile);
	hotbarPanel->addElement(buttonEditCollision);
	hotbarPanel->addElement(buttonSave);

	// Add hotbar panel to gui manager
	this->guiManager->addElement(hotbarPanel);
}

void MainMenuGui::setupCoordinates()
{
	// Set up coordinates text
	this->coords = new OPI_Text::TextComponent("(0, 0)", OPI_Text::FontManager::getFont("RobotoMono-Regular"), 23);

	this->coords->visible = true;
	this->coords->x = 2;
	this->coords->y = 2;
	this->coords->R = 0.90f;
	this->coords->G = 0.90f;
	this->coords->B = 1.00f;

	// Set up text label
	OPI_Gui::TextLabel * coordsLabel = new OPI_Gui::TextLabel(this->coords->x, this->coords->y, this->coords);

	// Add coords to gui manager
	this->guiManager->addElement(coordsLabel);
}


void MainMenuGui::setEditorMode(char setting)
{
	// TODO - replace Global include with map editor state machine
	mode = setting;
}

void MainMenuGui::toggleEditorFringe()
{
	fringe_mode = !fringe_mode;
}

