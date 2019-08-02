#ifndef __OPI_GUITEXTLABEL_H_
#define	__OPI_GUITEXTLABEL_H_

#include "OPI_GuiElement.h"
#include "OPI_Text.h"

namespace OPI_Gui
{
	class TextLabel : public Element
	{
	public:
		TextLabel();
		TextLabel(int x, int y, OPI_Text *text);
		virtual ~TextLabel();

		// Input handlers
		bool handleMouseMove(int mouseX, int mouseY);
		bool handleMousePressLeft(int mouseX, int mouseY);
		bool handleMousePressRight(int mouseX, int mouseY);
		bool handleMouseReleaseRight(int mouseX, int mouseY);
		bool handleMouseReleaseLeft(int mouseX, int mouseY);
	private:
		OPI_Text *text;
	};
}

#endif