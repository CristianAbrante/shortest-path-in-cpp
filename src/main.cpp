#include <iostream>
#include "ClassGraphicGrid.hpp"
#include "Button.hpp"

// This is defined below main
void updateGridCamera( const GraphicGrid& grid, sf::View& camera );


int main( int argc, char *argv[] )
{
    sf::RenderWindow window(
        sf::VideoMode::getDesktopMode(),
        "Shortest path"
    );

    // Columns and rows of the grid
    const unsigned M = 100   // Num cols in grid
                 , N = 100;  // Num rows in grid

    try
    {
        // Create grid
        auto grid = GraphicGrid::init(
            { 100, 100 },                 // Grid top left position
            {                             // Grid bottom right position
                window.getSize().x - 100,                
                window.getSize().y - 140 
            },
            M, N,                         // Number of columns and rows of the grid
            "sprites/sprites.png",        // Location of the sprite sheet
            { 16, 16 },                   // The size of a single sprite image in the sheet
            { 1, 0 }                      // The position of the default sprite image in the sheet
        );
        
        // This will allow us to zoom and move the "camera" that shows the grid
        sf::View gridCamera({
            (float)grid.startPos().x,
            (float)grid.startPos().y,
            (float)grid.endPos().x,
            (float)grid.endPos().y
        });
        
        // Open texture for buttons
        sf::Texture buttonsTexture;
        if ( !buttonsTexture.loadFromFile( "sprites/buttons.png" ) ) {
            std::cerr << "\n\nError loading sprites\n\n";
            return -1;
        }

        // Create buttons
        Button nextButton({100, grid.endPos().y}, window.getSize(), buttonsTexture, {1,2}, {1,0});
        Button runButton({window.getSize().x - 100 - nextButton.getSize().x, grid.endPos().y},  window.getSize(),
                         buttonsTexture, {1,2}, {0,0});

        // Dummy variable for demo. When we press a key a cell texture will change,
        // this variable is to hold which cell to change
        sf::Vector2u posToChangeTexture = {0, 0};

        while (window.isOpen())
        {
            // Event loop
            sf::Event event;
            while (window.pollEvent(event))
            {
                switch( event.type )
                {
                    case sf::Event::Closed:
                        window.close();
                        break;

                    case sf::Event::Resized:
                        nextButton.resize(window.getSize());
                        runButton.resize(window.getSize());
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

                    case sf::Event::MouseButtonPressed:
                      if (nextButton.isClicked(sf::Mouse::getPosition(window))) {
                        std::cout << "Next button pressed!!" << '\n';
                      }
                      if (runButton.isClicked(sf::Mouse::getPosition(window))) {
                        std::cout << "Run button pressed!!" << '\n';
                      }
                }
            }
            
            // Update camera grid
            updateGridCamera( grid, gridCamera );

            // Clear screen
            window.clear( sf::Color::White );

            // Draw grid
            window.setView( gridCamera );
            window.draw( grid );
            window.setView( window.getDefaultView() );

            // Draw buttons
            window.draw(nextButton);
            window.draw(runButton);
            
            // Display drawings
            window.display();
        }
    }
    catch( ... )
    {
        std::cerr << "\n\nEntered catch\n\n";
    }

    return 0;
}


void updateGridCamera( const GraphicGrid& grid, sf::View& camera ){
    
    // This is the quantity to add/substract to the current
    // position and size to move and zoom the camera.
    // Static because this value should not change in the execution
    // of the program so we don't want to compute it every time.
    static const sf::Vector2f offset = {
        grid.numRows() / 100.0f,
        grid.numCols() / 100.0f
    };
    
    // These will hold the total offset to apply
    // to the current position and size to perform the
    // zoom and movement
    sf::Vector2f moveOffset = {0.0f,0.0f}
               , zoomOffset = {0.0f,0.0f};
    
    // Check user input for zoom in/out
    if( sf::Keyboard::isKeyPressed( sf::Keyboard::Add ) ){
        zoomOffset -= {offset.x, offset.x};
    }
    if( sf::Keyboard::isKeyPressed( sf::Keyboard::Subtract ) ){
        zoomOffset += {offset.x, offset.x};
    }

    // Check user input for camera move
    if( sf::Keyboard::isKeyPressed( sf::Keyboard::Right ) ){
        moveOffset.x += offset.x;
    }
    if( sf::Keyboard::isKeyPressed( sf::Keyboard::Left ) ){
        moveOffset.x -= offset.x;
    }
    if( sf::Keyboard::isKeyPressed( sf::Keyboard::Up ) ){
        moveOffset.y -= offset.y;
    }
    if( sf::Keyboard::isKeyPressed( sf::Keyboard::Down ) ){
        moveOffset.y += offset.y;
    }
    
    // Update zoom
    auto cameraSize = camera.getSize() + zoomOffset;
    if( cameraSize.x < 1.0f ) cameraSize.x = 1.0f;
    if( cameraSize.y < 1.0f ) cameraSize.y = 1.0f;

    camera.setSize( cameraSize );

    // Update position
    camera.move( moveOffset );
}