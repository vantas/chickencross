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

    // Implement Singleton Pattern
    static InputManager* instance()
    {
        return &m_InputManager;
    }

    void addKeyInput(sf::String name, sf::Keyboard::Key keyCode);
    void addMouseInput(sf::String name, sf::Mouse::Button mouseButton);
    bool testEvent(sf::String name);

    protected:

    InputManager() {}

private:

    struct MyKeys
    {
        InputType myInputType;
        sf::Keyboard::Key myKeyCode;
        sf::Mouse::Button myMouseButton;
    };

    typedef std::map<std::string,MyKeys>  mapT;
    mapT keys;

    static InputManager m_InputManager;
};

#endif // INPUT_MANAGER_H
