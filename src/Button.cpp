//
// Created by cristian on 3/10/17.
//

#include "Button.hpp"

Button::Button(sf::Vector2f position,
               sf::Texture &buttonTexture,
               sf::Vector2u imageCounter,
               sf::Vector2u imageSelector) {

  textureSection.width = buttonTexture.getSize().x / imageCounter.x;
  textureSection.height = buttonTexture.getSize().y / imageCounter.y;

  textureSection.top = imageSelector.x * textureSection.height;
  textureSection.left = imageSelector.y * textureSection.width;

  body.setTexture(buttonTexture);
  body.setTextureRect(textureSection);

  body.setPosition(position);

}

bool Button::isClicked(sf::Vector2i mousePosition) const {
  //TODO: Implement click function.
  return true;
}

sf::Vector2f Button::getSize(void) const {
  return {textureSection.width, textureSection.height};
}


void Button::Draw(sf::RenderWindow &window) {
  window.draw(body);
}
