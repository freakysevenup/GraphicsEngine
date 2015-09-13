#pragma once
#include <SDL/SDL.h>
#include <GLM/glm.hpp>
#include <unordered_map>



class InputManager
{

public:
	InputManager();
	~InputManager();

	void update();

	void keyPressed(unsigned int keyID);
	void keyReleased(unsigned int keyID);

	bool isKeyDown(unsigned int keyId);
	bool isKeyPressed(unsigned int keyId);
	bool wasKeyDown(unsigned int keyId);

	// Getters
	inline glm::vec2 & getMouseCoords() { return m_mouseCoords; }

	// Setters
	void setMouseCoords(float x, float y) {
		m_mouseCoords.x = x; 
		m_mouseCoords.y = y;
	}

private:

	std::unordered_map<unsigned int, bool> m_keyMap;
	std::unordered_map<unsigned int, bool> m_previousKeyMap;

	glm::vec2 m_mouseCoords = glm::vec2(0.0f);

};

