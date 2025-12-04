// Copyright (c) Tyler Veness

#pragma once

#include <vector>

#include <SFML/Graphics/RectangleShape.hpp>

#include "box2d_base.hpp"

class Ship;

/// Handles all bullets created by entities.
class Bullet : public Box2DBase {
 public:
  virtual ~Bullet();

  /// Call this at end of program to free all bullets and avoid memory leaks
  static void cleanup();

  static void drawAll(sf::RenderTarget& target,
                      sf::RenderStates states = sf::RenderStates::Default);
  static void syncObjects(const sf::Window& referTo);

  static void add(const Ship& ship, const sf::Window& referTo);
  static void checkCollisions(
      const Ship& ship,
      const sf::Window& referTo);  // checks if bullet collided with other
                                   // object or left the screen to destroy it

  sf::RectangleShape shape;

 protected:
  static std::vector<Bullet*> bullets;

 private:
  Bullet(const Ship& ship, const sf::Window& referTo);
};
