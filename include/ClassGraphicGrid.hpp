#ifndef CLASS_GRAPHIC_GRID_HPP
#define CLASS_GRAPHIC_GRID_HPP

#include <SFML/Graphics.hpp>

// Inherits from Drawable so we can overload the draw function
class GraphicGrid : public sf::Drawable
{
  private:
    const int M_  // Number of columns
            , N_; // Number of rows

    const sf::Vector2u gridStart_  // Point in the window that is the top left corner of the grid
                     , gridEnd_    // Point in the window that is the bottom right corner of the grid
                     , texSz_;     // The size of each sprite texture in the sprite sheet

    const sf::Texture spriteSheet_;   // Holds the texture object of the sprite sheet to use
        
    sf::VertexArray cells_;     // Holds the vertices of the cells, the size will be M*N*4 because
                                // each cell has 4 vertices

    const unsigned MIN_CELL_SZ; // Minimum length (in px) for a side of a cell

    // Private constructor, objects have to be created with the static init method
    GraphicGrid(
        const sf::Vector2u& gridStart,    // Point in the window that is the top left corner of the grid
        const sf::Vector2u& gridEnd,      // Point in the window that is the bottom right corner of the grid
        const int M,                      // Number of columns in the grid
        const int N,                      // Number of rows in the grid
        const sf::Texture& spriteSheet,   // Holds the texture object of the sprite sheet to use
        const sf::Vector2u& texSz,        // Size of each sprite texture in the sprite sheet
        const sf::Vector2u& defaultTexPos // Position of the texture sprite in the sheet to put in the cells as default
    );

    
  public:   
    // This static method is used to initialize a grid. We use this instead of the
    // constructor because this way we erase the throws from the constructor.
    // Also, this handles the loading of the texture so we don't pollute the main
    static GraphicGrid init(
        const sf::Vector2u& gridStart,    // Point in the window that is the top left corner of the grid
        const sf::Vector2u& gridEnd,      // Point in the window that is the bottom right corner of the grid
        const int M,                      // Number of columns in the grid
        const int N,                      // Number of rows in the grid
        const std::string& spriteSheetLocation,   // Location in the file system of the spriteSheet to use
        const sf::Vector2u& texSz,        // Size of each sprite texture in the sprite sheet
        const sf::Vector2u& defaultTexPos = {0, 0} // Position of the texture sprite in the sheet to put in the cells as default        
    );
    
    // Copies are not allowed, moves are
    GraphicGrid( const GraphicGrid& ) = delete;
    GraphicGrid& operator= ( const GraphicGrid& ) = delete;
    GraphicGrid( GraphicGrid&& ) = default;
    GraphicGrid& operator= ( GraphicGrid&& ) = default;

    virtual ~GraphicGrid(){}    

    // No need to use this method directly. By overloading this function we can use instances of this
    // class as argument to the draw method of a render target.
    virtual void draw( sf::RenderTarget& target, sf::RenderStates states )const;

    // Given a cell position and a sprite texture position we change the texture that
    // the cell has to the new texture.
    void changeCellTexture( const sf::Vector2u& cellPos, const sf::Vector2u& texPosInSpriteSheet );

    // Getters
    // These are made so the main doesn't have to be polluted with useless variables
    // that grid objects already holds
    int numRows()const{ return M_; }
    int numCols()const{ return N_; }
    const sf::Vector2u& startPos()const{ return gridStart_; }
    const sf::Vector2u& endPos()const{ return gridEnd_; }
    sf::Vector2u size()const{ return (gridEnd_ - gridStart_); }
    const sf::Vector2u& spriteSize()const{ return texSz_; }
    const sf::Texture& spriteSheetTex()const { return spriteSheet_; }
    
};

#endif // CLASS_GRAPHIC_GRID_HPP
