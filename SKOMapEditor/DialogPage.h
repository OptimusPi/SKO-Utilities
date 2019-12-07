#ifndef __SKO_DIALOGPAGE_H_
#define __SKO_DIALOGPAGE_H_

#include <vector>
#include "TextComponent.h"

class DialogPage
{
public:
	DialogPage();
	virtual ~DialogPage();

	std::vector<OPI_Text::TextComponent*> lines;
};


#endif