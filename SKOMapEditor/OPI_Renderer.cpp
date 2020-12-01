#include "OPI_Renderer.h"


OPI_Renderer::OPI_Renderer(std::string title, int windowWidth, int windowHeight)
{
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	this->title = title;
}

OPI_Renderer::~OPI_Renderer()
{
	SDL_GL_DeleteContext(glContext);
}

void OPI_Renderer::initScreen()
{
	window = SDL_CreateWindow(this->title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowWidth, windowHeight,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	glContext = SDL_GL_CreateContext(window);
	sizeScreen();
}

void OPI_Renderer::sizeScreen()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glViewport(0, 0, windowWidth, windowHeight);
	glClear(GL_COLOR_BUFFER_BIT);



	glOrtho(0.0f, originalWindowWidth, originalWindowHeight, 0.0f, -1.0f, 1.0f);

}


void OPI_Renderer::drawImage(int x, int y, const OPI_Image* img, const SDL_Rect selection)
{
	if (x >= (int)(0 - selection.w) &&
		x < this->originalWindowWidth &&
		y < this->originalWindowHeight &&
		y >= (int)(0 - selection.h))
	{
		drawImage(x, y, img, selection, this->defaultBlendTolerance);
	}
}

void OPI_Renderer::drawImage(int x, int y, const OPI_Image *img)
{
	if (x >= (int)(0 - img->width) &&
		x < this->originalWindowWidth &&
		y < this->originalWindowHeight &&
		y >= (int)(0 - img->height))
	{
		SDL_Rect selection;
		selection.x = 0;
		selection.y = 0;
		selection.w = img->width;
		selection.h = img->height;
		drawImage(x, y, img, selection, this->defaultBlendTolerance);
	}
}


void OPI_Renderer::drawImage(int x, int y, const OPI_Image *img, const SDL_Rect selection, float blendTolerance)
{
	// Do not attempt to render null images, just continue.
	if (img == nullptr)
		return;

	if (this->bound != img->texture)
	{
		glEnable(GL_TEXTURE_2D);
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glColor3f(1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, img->texture);
		this->bound = img->texture;

		glEnable(GL_BLEND);
		glAlphaFunc(GL_GREATER, blendTolerance);
		glEnable(GL_ALPHA_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	float
		offset_x = selection.x,
		offset_y = selection.y,
		end_x = x + selection.w,
		end_y = y + selection.h;

	float right = (offset_x + selection.w) / (float)img->width;
	float bottom = (offset_y + selection.h) / (float)img->height;
	float left = (offset_x) / (float)img->width;
	float top = (offset_y) / (float)img->height;

	glBegin(GL_QUADS);
	//Top-left vertex (corner)
	glTexCoord2f(left, top);
	glVertex2f(x, y);

	//Bottom-left vertex (corner)
	glTexCoord2f(left, bottom);
	glVertex2f(x, end_y);

	//Bottom-right vertex (corner)
	glTexCoord2f(right, bottom);
	glVertex2f(end_x, end_y);

	//Top-right vertex (corner)
	glTexCoord2f(right, top);
	glVertex2f(end_x, y);
	glEnd();
}

void OPI_Renderer::drawText(OPI_Text::TextComponent *text)
{
	//tint
	glColor3f(text->R, text->G, text->B);

	float x = text->x;
	float y = text->y;
	SDL_Rect selection;
	selection.w = text->contentRender->width;
	selection.h = text->contentRender->height;
	selection.x = 0;
	selection.y = 0;

	drawImage(x, y, text->contentRender, selection, 0.01);

	//reset tint
	glColor3f(1.0f, 1.0f, 1.0f);
}

void OPI_Renderer::drawRect(SDL_Rect rect, unsigned char r, unsigned char g, unsigned char b)
{
	float fR = r == 0? 0.0f : 255.0/r;
	float fG = g == 0? 0.0f : 255.0/g;
	float fB = b == 0? 0.0f : 255.0/b;

	glEnable(GL_BLEND);
	glAlphaFunc(GL_GREATER, 0.1);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_TEXTURE_2D);
	glLoadIdentity();

	//Draw rect
	glBegin(GL_LINE_LOOP);
	glColor4f(fR, fG, fB, 0.75f);
	glVertex2f(rect.x, rect.y);
	glVertex2f(rect.x + rect.w, rect.y);
	glVertex2f(rect.x + rect.w, rect.y + rect.h);
	glVertex2f(rect.x, rect.y + rect.h);
	glEnd();

	//Draw outside border
	glBegin(GL_LINE_LOOP);
	glColor4f(fR/2, fG/2, fB/2, 0.15f);
	glVertex2f(rect.x - 1.0f, rect.y - 1.0f);
	glVertex2f(rect.x + rect.w + 1, rect.y - 1);
	glVertex2f(rect.x + rect.w + 1, rect.y + rect.h + 1);
	glVertex2f(rect.x - 1, rect.y + rect.h + 1);
	glEnd();

	//Draw inside border
	glBegin(GL_LINE_LOOP);
	glColor3f(0, 0, 0);
	glColor4f(fR, fG, fB, 0.25f);
	glVertex2f(rect.x, rect.y);
	glVertex2f(rect.x + rect.w - 1, rect.y + 1);
	glVertex2f(rect.x + rect.w - 1, rect.y + rect.h - 1);
	glVertex2f(rect.x + 1, rect.y + rect.h - 1);
	glEnd();
}

void OPI_Renderer::startDraw()
{
	glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void OPI_Renderer::updateScreen()
{
	SDL_GL_SwapWindow(this->window);
}

int OPI_Renderer::getScaledMouseX(int mouse_x)
{
	return mouse_x * (float)this->originalWindowWidth / this->windowWidth;
}

int OPI_Renderer::getScaledMouseY(int mouse_y)
{
	return mouse_y * (float)this->originalWindowHeight / this->windowHeight;
}
