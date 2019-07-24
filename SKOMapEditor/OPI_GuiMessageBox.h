#ifndef	__OPI_GUIMESSAGEBOX_
#define __OPI_GUIMESSAGEBOX_

#include "OPI_GuiElement.h"
#include "OPI_GuiMessageBoxType.h"

namespace OPI_Gui
{
	class MessageBox : public Element
	{
	public:
		MessageBox(std::string message);
		MessageBox(std::string message, MessageBoxType mode);
		virtual ~MessageBox();
	};
}

#endif

