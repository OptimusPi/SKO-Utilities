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

void OPI_Renderer::drawImage(int x, int y, const OPI_Image *img)
{
	drawImage(x, y, img, this->defaultBlendTolerance);
}


void OPI_Renderer::drawImage(int x, int y, const OPI_Image *img, float blendTolerance)
{
	// Do not attempt to render null images, just continue.
	if (img == nullptr)
		return;
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
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

	glFlush();
}

void OPI_Renderer::drawText(OPI_Text::TextComponent *text)
{
	//tint
	glColor3f(text->R, text->G, text->B);

	float screen_x = text->x;
	float screen_y = text->y;
	drawImage(10, 10, &text->contentRender, 0.01);

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
