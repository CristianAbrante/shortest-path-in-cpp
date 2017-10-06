//
// Created by cristian on 3/10/17.
//

#ifndef IA_P1_BUTTON_H
#define IA_P1_BUTTON_H

#include <SFML/Graphics.hpp>

class Button {

 public:

  Button(sf::Vector2f position,
           sf::Texture &buttonTexture,
           sf::Vector2u imageCounter,
           sf::Vector2u imageSelector);

  bool isClicked(sf::Vector2i mousePosition) const;

  sf::Vector2f getSize(void) const;

  void Draw(sf::RenderWindow &window);

 private:

  sf::Sprite body;
  sf::IntRect textureSection;

};

#endif //IA_P1_BUTTON_H
