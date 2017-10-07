#include <iostream>
#include "ClassGraphicGrid.hpp"
#include "Button.hpp"

int main( int argc, char *argv[] )
{
    sf::RenderWindow window(
        sf::VideoMode::getDesktopMode(),
        "Sample program to test the grid"
    );

    const unsigned M = 50   // Num cols in grid
                 , N = 50;  // Num rows in grid

    sf::Vector2u gridStart = { 100, 100 }   // Top left corner of the grid
               , gridEnd   = { window.getSize().x - 100, window.getSize().y - 140 } // Bottom right
               , texSz   = { 16, 16 }   // Size in pixels of each sprite texture
               , texPos  = { 1, 0 };    // Which sprite image of the spritesheet to use

    sf::Texture spriteSheetTexture;
    if ( !spriteSheetTexture.loadFromFile( "sprites/sprites.png" ) )
    {
        std::cerr << "\n\nError loading sprites\n\n";
        return -1;
    }

    sf::Texture buttonsTexture;
    if ( !buttonsTexture.loadFromFile( "sprites/buttons.png" ) ) {
        std::cerr << "\n\nError loading sprites\n\n";
        return -1;
    }

    try
    {
        GraphicGrid grid( gridStart, gridEnd, M, N, spriteSheetTexture, texSz, texPos );

        Button nextButton({100, gridEnd.y}, buttonsTexture, {1,2}, {1,0});
        Button runButton({window.getSize().x - 100 - nextButton.getSize().x, gridEnd.y}, buttonsTexture, {1,2}, {0,0});

        // For demo. When we press a key a cell texture will change,
        // this variable is to hold which cell to change
        sf::Vector2u posToChangeTexture = {0, 0};

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                switch( event.type )
                {
                    case sf::Event::Closed:
                        window.close();
                        break;

                    // Close window on Ctrl + Q
                    case sf::Event::KeyReleased:
                        if(
                            event.key.code == sf::Keyboard::Q  &&
                            sf::Keyboard::isKeyPressed( sf::Keyboard::LControl )
                          )
                            window.close();
                        break;

                    // When the user enters any letter we show it and change the texture of a cell
                    case sf::Event::TextEntered:
                        std::cerr << (char)event.text.unicode << ' ';

                        // Update next cell to change the texture
                        grid.changeCellTexture( posToChangeTexture, {1,1} );

                        // Update the cell to change next
                        posToChangeTexture.x += 1;
                        if (posToChangeTexture.x >= M)
                        {
                            posToChangeTexture.x = 0;
                            posToChangeTexture.y += 1;
                            if (posToChangeTexture.y >= N)
                                posToChangeTexture = {0,0};
                        }
                        break;
                }
            }

            window.clear( sf::Color::White );
            window.draw( grid );
            nextButton.Draw(window);
            runButton.Draw(window);
            window.display();
        }
    }
    catch( ... )
    {
        std::cerr << "\n\nEntered catch\n\n";
    }

    return 0;
}
