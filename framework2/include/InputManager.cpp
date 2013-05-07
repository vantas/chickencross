#include "InputManager.h"

void InputManager::addKeyInput(sf::String name, sf::Event::EventType etype, sf::Keyboard::Key keyCode)
{
    MyKeys key;
    key.myInputType = KeyboardInput;
    key.myEventType = etype;
    key.myKeyCode   = keyCode;
    keys[name] = key;
}

void InputManager::addMouseInput(sf::String name, sf::Event::EventType etype, sf::Mouse::Button mouseButton)
{

}
