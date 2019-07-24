#ifndef	__OPI_GUIMESSAGEBOX_
#define __OPI_GUIMESSAGEBOX_

#include "OPI_GuiElement.h"
#include "OPI_GuiMessageBoxType.h"
#include "OPI_Image.h"
#include "OPI_Text.h"

namespace OPI_Gui
{
	class MessageBox : public Element
	{
	public:
		MessageBox(std::string message);
		MessageBox(std::string message, MessageBoxType type);
		virtual ~MessageBox();
		OPI_Image *texture;
	};
}

#endif

