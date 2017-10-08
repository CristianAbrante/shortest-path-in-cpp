//
// Created by cristian on 3/10/17.
//

#include "Button.hpp"

Button::Button(sf::Vector2f position,
               sf::Vector2u windowSize,
               sf::Texture &buttonTexture,
               sf::Vector2u imageCounter,
               sf::Vector2u imageSelector) {

  this->windowSize = windowSize;

  sf::IntRect textureSection;

  textureSection.width = buttonTexture.getSize().x / imageCounter.x;
  textureSection.height = buttonTexture.getSize().y / imageCounter.y;

  textureSection.top = imageSelector.x * textureSection.height;
  textureSection.left = imageSelector.y * textureSection.width;

  body.setTexture(buttonTexture);
  body.setTextureRect(textureSection);

  body.setPosition(position);

  currentButtonSize.width = (float)textureSection.width;
  currentButtonSize.height = (float)textureSection.height;
  currentButtonSize.top = position.y;
  currentButtonSize.left = position.x;

}

bool Button::isClicked(sf::Vector2i mousePosition) {

  sf::Vector2f fMousePosition = {(float)mousePosition.x, (float)mousePosition.y};

  return (currentButtonSize.contains(fMousePosition));

}

void Button::resize(sf::Vector2u newWindowSize) {

  currentButtonSize.width = (newWindowSize.x * currentButtonSize.width) / windowSize.x;
  currentButtonSize.height = (newWindowSize.y * currentButtonSize.height) / windowSize.y;

  currentButtonSize.left = (newWindowSize.x * currentButtonSize.left) / windowSize.x;
  currentButtonSize.top = (newWindowSize.y * currentButtonSize.top) / windowSize.y;

  windowSize = newWindowSize;

}

sf::Vector2f Button::getSize(void) const {
  return {currentButtonSize.width, currentButtonSize.height};
}


void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const {

    // We set the texture that we want to display.
    states.texture = body.getTexture();

    // Draw the body (sprite).
    target.draw(body);

}
