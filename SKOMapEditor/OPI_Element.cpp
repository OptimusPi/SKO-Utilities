#include "Element.h"


void OPI_Gui::Element::addElement(OPI_Gui::Element *element)
{
	// Set myself to the parent of element
	element->parent = this;

	// Make sure it's within the bounds
	element->ensureBounds();

	// Add to this Element's collection of child Elements
	this->children.push_back(element);
}

void OPI_Gui::Element::destroy()
{
	this->destroyed = true;
	this->isVisible = false;
}

void OPI_Gui::Element::setTexture(OPI_Image * texture)
{
	this->texture = texture;
}

OPI_Image * OPI_Gui::Element::getTexture()
{
	return this->texture;
}

int OPI_Gui::Element::getMinimumX()
{
	return 0;
}

int OPI_Gui::Element::getMinimumY()
{
	return 0;
}

int OPI_Gui::Element::getMaximumX()
{
	if (this->parent == nullptr)
		return 0;

	return this->parent->width - this->width;
}

int OPI_Gui::Element::getMaximumY()
{
	if (this->parent == nullptr)
		return 0;

	return this->parent->height - this->height;
}

void OPI_Gui::Element::ensureBounds()
{
	if (this->parent == nullptr)
		return;

	int minimumX = this->getMinimumX();
	if (this->x < minimumX)
		this->x = minimumX;
	int minimumY = this->getMinimumY();
	if (this->y < minimumY)
		this->y = minimumY;
	int maximumX = this->getMaximumX();
	if (this->x > maximumX)
		this->x = maximumX;
	int maximumY = this->getMaximumY();
	if (this->y > maximumY)
		this->y = maximumY;
}

bool OPI_Gui::Element::isInteracting()
{
	return false;
}
