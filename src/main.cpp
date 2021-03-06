#include <iostream>
#include <stdexcept> // std::invalid_argument
#include <fstream>
#include <vector>
#include <time.h>

#include "Button.hpp"
#include "ClassGraphicGrid.hpp"
#include "GridCamera.hpp"
#include "ProblemSpecification.hpp"
#include "AStar.hpp"


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
        std::string file_name;

        if (argc >= 2) {
          file_name = argv[1];
        } else {
          file_name = "";
        }

        problemSpecification new_problem(file_name);

        // Create grid
        auto grid = GraphicGrid::init(
            { 100, 100 },                 // Grid top left position
            {                             // Grid bottom right position
                window.getSize().x - 100,
                window.getSize().y - 140
            },
            new_problem.rows(),
            new_problem.columns(),
            "sprites/sprite-sheet.png",   // Location of the sprite sheet
            { 32, 32 },                   // The size of a single sprite image in the sheet
            { 0, 0 }                      // The position of the default sprite image in the sheet
        );

        // Show obstacles in grid
        // And store them to pass to the shortest path algorithm
        std::vector<bool> obstacles( grid.numCols() * grid.numRows() );
        for( int i = 0; i < new_problem.getNumberOfObstaces(); ++i )
        {
          const auto& pos = new_problem.getObstacle(i);
          grid.changeCellTexture( {(unsigned)pos.x, (unsigned)pos.y}, {2,0} );

          obstacles[ pos.x * grid.numCols() + pos.y ] = true;
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

        AStar shortestPathFinder(
            new_problem.rows(), new_problem.columns(),
            new_problem.car_position().x, new_problem.car_position().y,
            new_problem.final_position().x, new_problem.final_position().y,
            obstacles,
            new_problem.heuristic() // Heuristic function to use
        );

        // This object will allow us to zoom and move the "camera" that shows the grid
        GridCamera gridCamera( grid );


        // Open sprite sheet file for buttons
        sf::Texture buttonsTexture;
        if ( !buttonsTexture.loadFromFile( "sprites/buttons.png" ) ) {
            throw std::invalid_argument("Error loading sprites");
        }

        // Create buttons
        Button nextButton({100.0, (float)grid.endPos().y}, window.getSize(), buttonsTexture, {1,2}, {1,0});
        Button runButton({(float)(window.getSize().x - 100 - nextButton.getSize().x), (float)grid.endPos().y},  window.getSize(),
                         buttonsTexture, {1,2}, {0,0});

        // Load texture of the final button.
        sf::Texture final_buttons_texture;
        if ( !final_buttons_texture.loadFromFile( "sprites/final-button.png" ) ) {
           throw std::invalid_argument("Error loading sprites");
        }

        Button finalButton( {(window.getSize().x / 2 - final_buttons_texture.getSize().x / 2),
          (float)(grid.endPos().y - 100)}, window.getSize(), final_buttons_texture, {1,2}, {0,0});

        // These variable are needed to track the mouse position to update the camera
        // when the user wants to move it with the mouse
        sf::Vector2i mousePositionWhenUserClicked;
        bool isMousePressed = false
            // Variable needed for knowing when the algorithm has to run
           , runAlgorithmOnce = false
           , nonInteractiveMode = false
           , algorithmHadFinished = false;

        int number_of_steps = 0;
        clock_t timer;

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
                        std::clog << "Next Button pressed" << std::endl;
                          runAlgorithmOnce = true;
                      }
                      if (runButton.isClicked(sf::Mouse::getPosition(window))) {
                        std::clog << "Run Button pressed" << std::endl;
                          nonInteractiveMode = true;
                          timer = clock();
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

            // Run algorithm
            if( !algorithmHadFinished && (runAlgorithmOnce || nonInteractiveMode) )
            {
                if( !shortestPathFinder.nextIteration() )
                {

                    number_of_steps++;


                      // Change the texture into yellow and green path.
                      grid.changeCellTexture( shortestPathFinder.lastAdditionToClose , {2,1} );
                      for( const auto& pos : shortestPathFinder.lastAdditionsToOpen )
                          grid.changeCellTexture( pos , {1,1} );

                      grid.changeCellTexture(
                      {
                          new_problem.car_position().x,
                          new_problem.car_position().y
                      },
                          {2, 2}
                      );

                      grid.changeCellTexture(
                      {
                          new_problem.final_position().x,
                          new_problem.final_position().y
                      },
                          {0, 1}
                      );
                  
                }
                else
                {
                  timer = clock() - timer;
                    std::cout << "\nFinished\n";
                    std::cout << "It has taken: " << number_of_steps << " steps\n";
                    std::cout << "With a path size of " << shortestPathFinder.getShortestPath().size() << " \n";
                    std::cout << "In " << (float)timer/CLOCKS_PER_SEC << " seconds" << '\n';

                    // Change the texture to the blue path
                    for( const auto& pos : shortestPathFinder.getShortestPath() )
                        grid.changeCellTexture( pos , {0,2} );

                    if (!shortestPathFinder.getShortestPath().empty()){
                      grid.changeCellTexture(
                      {
                          new_problem.car_position().x,
                          new_problem.car_position().y
                      },
                          {1, 2}
                      );

                      grid.changeCellTexture(
                      {
                          new_problem.final_position().x,
                          new_problem.final_position().y
                      },
                          {0, 3}
                      );
                    }

                    algorithmHadFinished = true;
                }

                runAlgorithmOnce = false;
            }

            // Clear screen
            window.clear( sf::Color::White );

            // Draw grid
            window.setView( gridCamera.getView() );
            window.draw( grid );
            window.setView( window.getDefaultView() );

            if (!algorithmHadFinished) {
              // Draw buttons
              window.draw(nextButton);
              window.draw(runButton);
            } else {

              window.draw(finalButton);

              if (shortestPathFinder.getShortestPath().empty()){
                finalButton.changeButtonTexture({1,0});
                window.draw(finalButton);
              }

            }

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
