#ifndef __OPI_GUITEXTLABEL_H_
#define	__OPI_GUITEXTLABEL_H_

#include "Element.h"
#include "TextComponent.h"

namespace OPI_Gui
{
	class TextLabel : public Element
	{
	public:
		TextLabel(OPI_Text::TextComponent *text);
		TextLabel(int x, int y, OPI_Text::TextComponent *text);
		virtual ~TextLabel();

		// Input handlers
		bool handleMouseMove(int mouseX, int mouseY);
		bool handleMousePressLeft(int mouseX, int mouseY);
		bool handleMousePressRight(int mouseX, int mouseY);
		bool handleMouseReleaseRight(int mouseX, int mouseY);
		bool handleMouseReleaseLeft(int mouseX, int mouseY);

		// Update text value and re-render
		void setText(OPI_Text::TextComponent *text);
	private:
		OPI_Text::TextComponent *text;
	};
}

#endif