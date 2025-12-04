// Copyright (c) Tyler Veness

#pragma once

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "box2d_base.hpp"

/// Controls Box2D ship.
class Ship : public Box2DBase {
 public:
  Ship(const sf::Vector2f& position, float fullHealth);
  virtual ~Ship();

  void controlShip();
  float getHealth();

  sf::ConvexShape shape;

 protected:
  static sf::Texture m_shipTexture;
  static bool m_isLoaded;
  float m_health;

 private:
  static float m_maxSpeed;

  b2Vec2 m_shipSpeed;
};
