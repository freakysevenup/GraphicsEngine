#include "InputManager.h"


InputManager::InputManager()
{
}


InputManager::~InputManager()
{
}

void InputManager::update()
{
	for (auto it : m_keyMap)
	{
		m_previousKeyMap[it.first] = it.second;
	}

}

void InputManager::keyPressed(unsigned int keyId)
{
	m_keyMap[keyId] = true;
}

void InputManager::keyReleased(unsigned int keyId)
{
	m_keyMap[keyId] = false;
}

bool InputManager::isKeyDown(unsigned int keyId)
{
	auto it = m_keyMap.find(keyId);

	if (it != m_keyMap.end())
	{
		return it->second;
	}
	else
	{
		return false;
	}
}

bool InputManager::isKeyPressed(unsigned int keyId)
{
	if (isKeyDown(keyId) == true && wasKeyDown(keyId) == false)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool InputManager::wasKeyDown(unsigned int keyId)
{
	auto it = m_previousKeyMap.find(keyId);

	if (it != m_previousKeyMap.end())
	{
		return it->second;
	}
	else
	{
		return false;
	}
}
