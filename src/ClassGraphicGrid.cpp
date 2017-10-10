#include "ClassGraphicGrid.hpp"
#include <algorithm> // std::max
#include <stdexcept> // std::invalid_argument

GraphicGrid GraphicGrid::init(
    const sf::Vector2u& gridStart,    // Point in the window that is the top left corner of the grid
    const sf::Vector2u& gridEnd,      // Point in the window that is the bottom right corner of the grid
    const int M,                      // Number of columns in the grid
    const int N,                      // Number of rows in the grid
    const std::string& spriteSheetLocation,   // Location in the file system of the spriteSheet to use
    const sf::Vector2u& texSz,        // Size of each sprite texture in the sprite sheet
    const sf::Vector2u& defaultTexPos // Position of the texture sprite in the sheet to put in the cells as default        
){
    // Check that the number of grid rows and columns are positive and one at least
    if( M < 1  || N < 1 )
        throw std::invalid_argument( "Invalid number of row/columns." );

    // Check that the grid position is legal. Because we don't have the
    // window size we can only check that the start point is really a top left corner
    // with respect to the grid end point
    if( gridStart.x >= gridEnd.x  || gridStart.y >= gridEnd.y )
        throw std::invalid_argument( "Invalid grid size." );

    sf::Texture tex;
    if ( !tex.loadFromFile(spriteSheetLocation) )
        throw std::invalid_argument( "Cannot open sprite sheet file." );

    return GraphicGrid( gridStart, gridEnd, M, N, tex, texSz, defaultTexPos );
}


// Constructor
GraphicGrid::GraphicGrid(
    const sf::Vector2u& gridStart,
    const sf::Vector2u& gridEnd,
    const int M,
    const int N,
    const sf::Texture& spriteSheet,
    const sf::Vector2u& texSz,        // Size of each sprite texture in the sprite sheet
    const sf::Vector2u& defaultTexPos // Position of the texture in the sheet to put in the cell as default
): sf::Drawable(),
   M_( M ),
   N_( N ),
   gridStart_( gridStart ),
   gridEnd_( gridEnd ),
   spriteSheet_( spriteSheet ),
   texSz_( texSz ),
   cells_( sf::Quads, 0 ), // We indicate the real number of elements once we make sure that
                          // the parameters are valid
   MIN_CELL_SZ( 10 )  
{
    // Auxiliar variable that holds the width and height that will have each cell in the grid
    const sf::Vector2u cellSz = {
        std::max( MIN_CELL_SZ, ((gridEnd_.x - gridStart_.x) / M_) ),  // x coordinate
        std::max( MIN_CELL_SZ, ((gridEnd_.y - gridStart_.y) / N_) )   // y coordinate
    };

    // Resize the cell vertices array
    cells_.resize( N_ * M_ * 4 );

    // Populate the array
    for( unsigned x = 0;  x < M;  ++x )
        for( unsigned y = 0;  y < N;  ++y )
        {
            // Because the cells array is one dimensional we have to translate the
            // position from the 2d array view that the user has to the real 1 dimensional array position
            unsigned pos = x * N_ + y;

            // To calculate the position in pixels of the cells in the window we will take the
            // logic position, that is in range [0, M*N), and multiply it by the size that each
            // cell has, this way we have each cell with a width and height of cellSz.x and cellSz.y.
            // Actually, to accomplish this for a cell we have to calculate the position for each vertex
            // of the cell.

            // Bottom left vertex
            cells_[pos*4 + 0].position  = sf::Vector2f(
                    gridStart.x + cellSz.x * x,
                    gridStart.y + cellSz.y * (y + 1)
            );
            cells_[pos*4 + 0].texCoords = sf::Vector2f(
                    texSz_.x  * defaultTexPos.x,
                    texSz_.y  * (defaultTexPos.y + 1)
            );

            // Upper left vertex
            cells_[pos*4 + 1].position  = sf::Vector2f(
                    gridStart.x + cellSz.x * x,
                    gridStart.y + cellSz.y * y
            );
            cells_[pos*4 + 1].texCoords = sf::Vector2f(
                    texSz_.x  * defaultTexPos.x,
                    texSz_.y  * defaultTexPos.y
            );

            // Upper right vertex
            cells_[pos*4 + 2].position  = sf::Vector2f(
                    gridStart.x + cellSz.x * (x + 1),
                    gridStart.y + cellSz.y * y
            );
            cells_[pos*4 + 2].texCoords = sf::Vector2f(
                    texSz_.x *  (defaultTexPos.x + 1),
                    texSz_.y  * defaultTexPos.y
            );

            // Down right vertex
            cells_[pos*4 + 3].position  = sf::Vector2f(
                    gridStart.x + cellSz.x * (x + 1),
                    gridStart.y + cellSz.y * (y + 1)
            );
            cells_[pos*4 + 3].texCoords = sf::Vector2f(
                    texSz_.x  * (defaultTexPos.x + 1),
                    texSz_.y  * (defaultTexPos.y + 1)
            );
        }

}


void GraphicGrid::draw( sf::RenderTarget& target, sf::RenderStates states )const
{
    // Apply the texture
    states.texture = &spriteSheet_;

    // Draw the vertex array
    target.draw( cells_, states );
}


void GraphicGrid::changeCellTexture( const sf::Vector2u& cellPos, const sf::Vector2u& texPosInSpriteSheet )
{
    // Checking for errors
    if( cellPos.x >= M_  || cellPos.y >= N_ )
        throw "TODO";

    // Translating position from the 2d array view that the user has to the real 1 dimensional array position
    unsigned pos = cellPos.x * N_ + cellPos.y;

    cells_[pos*4 + 0].texCoords = sf::Vector2f(
            texSz_.x * texPosInSpriteSheet.x,
            texSz_.y * (texPosInSpriteSheet.y + 1)
    );

    cells_[pos*4 + 1].texCoords = sf::Vector2f(
            texSz_.x * texPosInSpriteSheet.x,
            texSz_.y * texPosInSpriteSheet.y
    );

    cells_[pos*4 + 2].texCoords = sf::Vector2f(
            texSz_.x * (texPosInSpriteSheet.x + 1),
            texSz_.y * texPosInSpriteSheet.y
    );

    cells_[pos*4 + 3].texCoords = sf::Vector2f(
            texSz_.x * (texPosInSpriteSheet.x + 1),
            texSz_.y * (texPosInSpriteSheet.y + 1)
    );

}
