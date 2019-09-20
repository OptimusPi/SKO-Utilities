#ifndef __OPI_GUIBUTTONTOGGLEGROUP_H_
#define __OPI_GUIBUTTONTOGGLEGROUP_H_

#include <vector>
#include "Button.h"

namespace OPI_Gui
{
	class ButtonToggleGroup
	{
	public:
		ButtonToggleGroup();
		ButtonToggleGroup(std::vector<Button *> buttonGroup);
		virtual ~ButtonToggleGroup();
		void processToggle(Button* button);
		void addButton(Button *button);
	private:
		std::vector<Button *> buttonGroup;
	};
}

#endif

