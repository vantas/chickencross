#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <map>
#include <SFML/Graphics.hpp>

class InputManager
{
public:

    enum InputType
    {
        KeyboardInput,
        MouseInput,
        JoystickInput
    };

private:

    struct MyKeys
    {
        InputType myInputType;
        sf::Event::EventType myEventType;
        sf::Keyboard::Key myKeyCode;
        sf::Mouse::Button myMouseButton;
    };

    static std::map<std::string,MyKeys> keys;
    static void addKeyInput(sf::String name, sf::Event::EventType etype, sf::Keyboard::Key keyCode);
    static void addMouseInput(sf::String name, sf::Event::EventType etype, sf::Mouse::Button mouseButton);
};

#endif // INPUT_MANAGER_H
