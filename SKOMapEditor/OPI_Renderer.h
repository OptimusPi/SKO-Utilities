#ifndef	__OPI_RENDERER_H_
#define __OPI_RENDERER_H_

#include "OPI_Image.h"
#include "OPI_Text.h"

class OPI_Renderer
{
public:
	OPI_Renderer();
	virtual ~OPI_Renderer();

	// Drawing functions
	void drawImage(int x, int y, const OPI_Image *img);
	void drawImage(int x, int y, const OPI_Image *img, float blendTolerance);
	void drawText(OPI_Text *text);
	void drawRect(SDL_Rect rect);
private:
	float defaultBlendTolerance = 0.75f;
};

#endif

