#include "OPI_Renderer.h"


OPI_Renderer::OPI_Renderer()
{

}


OPI_Renderer::~OPI_Renderer()
{

}


void OPI_Renderer::DrawImage(int x, int y, const OPI_Image *img)
{
	DrawImage(x, y, img, this->defaultBlendTolerance);
}


void OPI_Renderer::DrawImage(int x, int y, const OPI_Image *img, float blendTolerance)
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, img->texture);

	glEnable(GL_BLEND);
	glAlphaFunc(GL_GREATER, blendTolerance);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glBegin(GL_QUADS);
	//Top-left vertex (corner)
	glTexCoord2i(0, 0);
	glVertex3i(x, y, 0);

	//Bottom-left vertex (corner)
	glTexCoord2i(0, 1);
	glVertex3i(x, y + img->height, 0);

	//Bottom-right vertex (corner)
	glTexCoord2i(1, 1);
	glVertex3i(x + img->width, y + img->height, 0);

	//Top-right vertex (corner)
	glTexCoord2i(1, 0);
	glVertex3i(x + img->width, y, 0);
	glEnd();

	glDisable(GL_BLEND);
}

void OPI_Renderer::drawText(OPI_Text *text)
{
	//tint
	glColor3f(text->R, text->G, text->B);

	float screen_x = text->x;
	float screen_y = text->y;
	DrawImage(10, 10, &text->contentRender, 0.01);

	//reset tint
	glColor3f(1.0f, 1.0f, 1.0f);
}

void OPI_Renderer::drawRect(SDL_Rect rect)
{
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(rect.x + 0.5, rect.y + 0.5);
	glVertex2f(rect.x + rect.w + 0.5, rect.y + 0.5);
	glVertex2f(rect.x + rect.w + 0.5, rect.y + rect.h + 0.5);
	glVertex2f(rect.x + 0.5, rect.y + rect.h + 0.5);
	glEnd();
}