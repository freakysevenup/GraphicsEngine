#pragma once
#include <SDL/SDL.h>
#include <string>

enum WindowFlags
{
	INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4, RESIZABLE = 0x8
};

class ViewPort
{
public:
	ViewPort();
	ViewPort(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags);
	~ViewPort();

	void swapBuffer(float r, float g, float b, float a);
	void Update();
	bool IsClosed();

	int getScreenWidth() const { return m_screenWidth; }
	int getScreenHeight() const { return m_screenHeight; }
	SDL_Window * getWindow() const { return m_window; }

	void setViewPortWidth(int width) { m_screenWidth = width; }
	void setViewPortHeight(int height) { m_screenHeight = height; }

private:
	Uint32 getFlags(unsigned int currentFlags);


	SDL_Window * m_window = nullptr;
	SDL_GLContext m_glContext;
	bool m_isClosed = false;
	int m_screenWidth = 0, m_screenHeight = 0;
};

