// library
#include <SFML/Graphics.hpp>

// main program
int main()
{
    // load texture from file
    sf::Texture texture("Images/Blacksmith.png");
    texture.setSmooth(true);
    texture.setRepeated(true);

    // create sprite from texture
    sf::Sprite sprite(texture);
    sprite.setTextureRect(sf::IntRect({0, 0}, sf::Vector2i(texture.getSize())*2));
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.size.componentWiseMul({0.5f, 0.5f}));
    sprite.setPosition({400.f, 300.f});
    sprite.setScale({0.3f, 0.3f});
    // sprite.setColor(sf::Color(0, 170, 255, 128));

    // create window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Title");

    // window loop
    while (window.isOpen())
    {
        // handle events
        while (auto event = window.pollEvent())
        {
            // on close button release
            if (event->is<sf::Event::Closed>())
            {
                // close window
                window.close();
            }

            // when window is resized
            else if (event->is<sf::Event::Resized>())
            {
                // update view
                window.setView(sf::View(sf::FloatRect({0.f, 0.f}, sf::Vector2f(window.getSize()))));
            }
        }

        // fill background color
        window.clear(sf::Color(64, 64, 64));

        // draw sprite
        window.draw(sprite);

        // display
        window.display();
    }

    // program end successfully
    return 0;
}