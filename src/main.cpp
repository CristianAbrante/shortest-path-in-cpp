#include <iostream>
#include <stdexcept> // std::invalid_argument
#include <fstream>

#include "ClassGraphicGrid.hpp"
#include "Button.hpp"
#include "ProblemSpecification.hpp"
#include "GridCamera.hpp"

// This is defined below main
void updateGridCameraFromKeyboardInput( GridCamera& camera );


int main( int argc, char *argv[] )
{
    sf::RenderWindow window(
        sf::VideoMode::getDesktopMode(),
        "Shortest path"
    );

    try
    {

        std::ifstream textFile;

        // We open the file that contains the specification
        // of our problem.
        if (argc >= 2) {
          textFile.open(argv[1]);
        } else {
          // TODO: Look for default file
          //textFile.open("test/default.txt");
          throw std::invalid_argument("Expected file with problem configuration.");
        }

        if (!textFile.is_open()) {
          std::clog << "Hola incorrect" << '\n';
          throw std::invalid_argument("Incorrect file of specifications.");
        }

        problemSpecification new_problem(textFile);

        // Create grid
        auto grid = GraphicGrid::init(
            { 100, 100 },                 // Grid top left position
            {                             // Grid bottom right position
                window.getSize().x - 100,
                window.getSize().y - 140
            },
            new_problem.row(), new_problem.column(),// Number of columns and rows of the grid
            "sprites/sprite-sheet2.png",   // Location of the sprite sheet
            { 32, 32 },                   // The size of a single sprite image in the sheet
            { 0, 0 }                      // The position of the default sprite image in the sheet
        );

        // Set obstacles in grid
        for( int i = 0; i < new_problem.getNumberOfObstaces(); ++i )
        {
          const auto& pos = new_problem.getObstacle(i);
          grid.changeCellTexture( {pos.x, pos.y}, {2,0} );
        }

        // Set car in grid
        grid.changeCellTexture(
          {
            new_problem.car_position().x,
            new_problem.car_position().y
          },
          {1, 0}
        );

        // Set goal in grid
        grid.changeCellTexture(
          {
            new_problem.final_position().x,
            new_problem.final_position().y
          },
          {0, 1}
        );

        // This object will allow us to zoom and move the "camera" that shows the grid
        GridCamera gridCamera( grid );


        // Open sprite sheet file for buttons
        sf::Texture buttonsTexture;
        if ( !buttonsTexture.loadFromFile( "sprites/buttons.png" ) ) {
            throw std::invalid_argument("Error loading sprites");
        }

        // Create buttons
        Button nextButton({100, grid.endPos().y}, window.getSize(), buttonsTexture, {1,2}, {1,0});
        Button runButton({window.getSize().x - 100 - nextButton.getSize().x, grid.endPos().y},  window.getSize(),
                         buttonsTexture, {1,2}, {0,0});

        // These variable are needed to track the mouse position to update the camera
        // when the user wants to move it with the mouse
        bool isMousePressed = false;
        sf::Vector2i mousePositionWhenUserClicked;

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

                    /*
                    // This is for demo only
                    // When the user enters any letter we show it and change the texture of a cell
                    case sf::Event::TextEntered:
                        // Update next cell to change the texture
                        grid.changeCellTexture( posToChangeTexture, {2,0} );

                        // Update the cell to change next
                        posToChangeTexture.x += 1;
                        if (posToChangeTexture.x >= new_problem.row())
                        {
                            posToChangeTexture.x = 0;
                            posToChangeTexture.y += 1;
                            if (posToChangeTexture.y >= new_problem.column())
                                posToChangeTexture = {0,0};
                        }
                        break;
                    */
                    // Listen for mouse wheel scroll to zoom in/out the camera

                    case sf::Event::MouseWheelScrolled:
                    {
                        float offsetFactor = event.mouseWheelScroll.delta * 10.0f;
                        gridCamera.zoom( {offsetFactor, offsetFactor} );
                    }
                        break;

                    case sf::Event::MouseButtonPressed:
                      // Check if user clicked a button
                      if (nextButton.isClicked(sf::Mouse::getPosition(window))) {
                        std::cout << "Next button pressed!!" << '\n';
                      }
                      if (runButton.isClicked(sf::Mouse::getPosition(window))) {
                        std::cout << "Run button pressed!!" << '\n';
                      }
                      // If the mouse is clicked in the grid section, lets indicate
                      // that we have to update the camera relative to the mouse position.
                      if( sf::Mouse::getPosition().y < (int)grid.endPos().y ){
                          isMousePressed = true;
                          mousePositionWhenUserClicked = sf::Mouse::getPosition( window );
                      }
                      break;

                    // When the user releases the mouse button we have to stop
                    // updating the camera according to the mouse
                    case sf::Event::MouseButtonReleased:
                        isMousePressed = false;
                        break;
                }
            }

            // Close window on Ctrl + Q
            if( sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
            &&  sf::Keyboard::isKeyPressed(sf::Keyboard::Q) )
            {
                window.close();
            }

            // Reset camera view on Ctrl + R
            if( sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
            &&  sf::Keyboard::isKeyPressed(sf::Keyboard::R) )
            {
                gridCamera.resetCamera();
            }

            // Update camera position from user mouse input
            if( isMousePressed )
            {
                sf::Vector2f offsetFactor = {
                    (float)(mousePositionWhenUserClicked.x - sf::Mouse::getPosition( window ).x) / 100,
                    (float)(mousePositionWhenUserClicked.y - sf::Mouse::getPosition( window ).y) / 100
                };

                gridCamera.move( offsetFactor );
            }

            // Update grid camera position and zoom from the user keyboard input
            updateGridCameraFromKeyboardInput( gridCamera );

            // Clear screen
            window.clear( sf::Color::White );

            // Draw grid
            window.setView( gridCamera.getView() );
            window.draw( grid );
            window.setView( window.getDefaultView() );

            // Draw buttons
            window.draw(nextButton);
            window.draw(runButton);

            // Display drawings
            window.display();
        }
    }
    catch( const std::invalid_argument& ia )
    {
        std::cerr << "\nInvalid argument " << ia.what() << '\n';
    }
    catch( ... )
    {
        std::cerr << "\nError\n";
    }

    return 0;
}


void updateGridCameraFromKeyboardInput( GridCamera& camera ){
    // These will hold the total offset to apply
    // to the current position and size to perform the
    // zoom and movement
    sf::Vector2f moveOffset = {0.0f,0.0f}
               , zoomOffset = {0.0f,0.0f};

    // Check user input for zoom in/out
    if( sf::Keyboard::isKeyPressed( sf::Keyboard::Add ) ){
        zoomOffset -= {1.0f, 1.0f};
    }
    if( sf::Keyboard::isKeyPressed( sf::Keyboard::Subtract ) ){
        zoomOffset += {1.0f, 1.0f};
    }

    // Check user input for camera move
    if( sf::Keyboard::isKeyPressed( sf::Keyboard::Right ) ){
        moveOffset.x += 1.0f;
    }
    if( sf::Keyboard::isKeyPressed( sf::Keyboard::Left ) ){
        moveOffset.x -= 1.0f;
    }
    if( sf::Keyboard::isKeyPressed( sf::Keyboard::Up ) ){
        moveOffset.y -= 1.0f;
    }
    if( sf::Keyboard::isKeyPressed( sf::Keyboard::Down ) ){
        moveOffset.y += 1.0f;
    }

    // Update zoom
    camera.zoom( zoomOffset );

    // Update position
    camera.move( moveOffset );

}
