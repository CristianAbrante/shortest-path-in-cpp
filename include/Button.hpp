
/*!
   \file Button.hpp
   \brief We define a class of a button to be displayed
          on the screen.
   \author Cristian Abrante Dorta
*/

#ifndef IA_P1_BUTTON_H
#define IA_P1_BUTTON_H

#include <SFML/Graphics.hpp>

class Button : public sf::Drawable {

 public:

  Button(sf::Vector2f position,
         sf::Vector2u windowSize,
         sf::Texture &buttonTexture,
         sf::Vector2u imageCounter,
         sf::Vector2u imageSelector);

  bool isClicked(sf::Vector2i mousePosition);

  void resize(sf::Vector2u newWindowSize);

  sf::Vector2f getSize(void) const;

  void changeButtonTexture(const sf::Vector2u imageSelector);

  void draw(sf::RenderTarget &target, sf::RenderStates states) const;


 private:

  sf::Sprite body;
  sf::FloatRect currentButtonSize;
  sf::Vector2u windowSize;

};

#endif //IA_P1_BUTTON_H
