// Copyright (c) Tyler Veness

#pragma once

#include <vector>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shader.hpp>

#include "Box2DBase.hpp"

class Ship;

/**
 * Handles creating planets of different colors in world.
 */
class Planet : public Box2DBase {
 public:
  virtual ~Planet();

  // Call this at end of program to free all planets and avoid memory leaks
  static void cleanup();

  static void drawAll(const Ship& ship, sf::RenderTarget& target,
                      sf::RenderStates states = sf::RenderStates::Default);
  static void syncObjects(const sf::Window& referTo);

  static Planet* add(const sf::Vector2f& position, const float& radius,
                     const sf::Color& color);
  static void applyUnivGravity();

  sf::CircleShape shape;
  sf::Shader shader;

 private:
  Planet(const sf::Vector2f& position, const float& radius,
         const sf::Color& color);

  static std::vector<Planet*> m_planets;

  sf::RenderStates m_shaderState;
};
