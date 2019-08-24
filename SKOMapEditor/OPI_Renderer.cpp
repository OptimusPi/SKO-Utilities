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
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glViewport(0, 0, windowWidth, windowHeight);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0f, originalWindowWidth, originalWindowHeight, 0.0f, -1.0f, 1.0f);

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
}

void OPI_Renderer::drawImage(int x, int y, const OPI_Image *img)
{
	drawImage(x, y, img, this->defaultBlendTolerance);
}


void OPI_Renderer::drawImage(int x, int y, const OPI_Image *img, float blendTolerance)
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
	drawImage(10, 10, &text->contentRender, 0.01);

	//reset tint
	glColor3f(1.0f, 1.0f, 1.0f);
}

void OPI_Renderer::drawRect(SDL_Rect rect)
{
	glColor3f(0.1f, 0.1f, 1.0f);

	float lineWidth[2];
	glGetFloatv(GL_LINE_WIDTH_RANGE, lineWidth);

	glBegin(GL_LINE_LOOP);
	glLineWidth(2.0f);
	glVertex2f(rect.x + 0.5, rect.y + 0.5);
	glVertex2f(rect.x + rect.w + 0.5, rect.y + 0.5);
	glVertex2f(rect.x + rect.w + 0.5, rect.y + rect.h + 0.5);
	glVertex2f(rect.x + 0.5, rect.y + rect.h + 0.5);
	glEnd();
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
