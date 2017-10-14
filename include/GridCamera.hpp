
#ifndef CLASS_GRID_CAMERA_HPP
#define CLASS_GRID_CAMERA_HPP

#include "ClassGraphicGrid.hpp"

#include <SFML/Graphics.hpp>

class GridCamera
{
  private:
      sf::View view_;

      // This is used to reset the view to the original value
      sf::View defaultView_;

      // This is the quantity to add/subtract to the current
      // position and size to move and zoom the camera.
      const sf::Vector2f offset_;

  public:
      GridCamera( const GraphicGrid& grid );

      // These methods set the camera size and position respectively to the current
      // size * offset_ * offsetFactor. The "this" object pointer is returned
      // so we can chain methods calls if we want.
      GridCamera* zoom( sf::Vector2f offsetFactor );
      GridCamera* move( const sf::Vector2f& offsetFactor );

      // Set the camera to the default view
      GridCamera* resetCamera();

      // Return the view so we can set the window view
      const sf::View& getView()const { return view_; }
};

#endif // CLASS_GRID_CAMERA_HPP
