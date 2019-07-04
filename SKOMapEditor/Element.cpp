#include "Element.h"

void OPI_Gui::Element::setTexture(OPI_Image * texture)
{
	if (this->texture)
		delete this->texture;
	this->texture = texture;
}

OPI_Image * OPI_Gui::Element::getTexture()
{
	return this->texture;
}

bool OPI_Gui::Element::isInteracting()
{
	return false;
}
