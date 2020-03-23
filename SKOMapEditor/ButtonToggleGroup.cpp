#include "ButtonToggleGroup.h"

OPI_Gui::ButtonToggleGroup::ButtonToggleGroup()
{
}

OPI_Gui::ButtonToggleGroup::ButtonToggleGroup(std::vector<Button*> buttonGroup)
{
	for (auto &button : buttonGroup) // access by reference to avoid copying
	{
		this->addButton(button);
	}
}


OPI_Gui::ButtonToggleGroup::~ButtonToggleGroup()
{

}

void OPI_Gui::ButtonToggleGroup::processToggle(Button * buttonToggle)
{
	for (auto &button : buttonGroup) // access by reference to avoid copying
	{
		if (button != buttonToggle && button->isToggleOn)
		{
			button->toggleOff();
		}
	}
}

void OPI_Gui::ButtonToggleGroup::addButton(Button * button)
{
	// Add the button to my collection of buttons
	this->buttonGroup.push_back(button);

	// Add the button toggle group to the button elemnt.
	// That way, it can ask the group to process the toggle graphic without worrying about it.
	button->toggleGroups.push_back(this);
}
