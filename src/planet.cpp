// Copyright (c) Tyler Veness

#include "planet.hpp"

#include <vector>

#include "ship.hpp"

std::vector<Planet*> Planet::m_planets;

static constexpr double kG = 6.6738480f;

Planet::~Planet() {
  std::vector<Planet*>::iterator index;
  for (index = m_planets.begin(); *index != this; index++) {
    if (index >= m_planets.end()) {
      return;
    }
  }

  m_planets.erase(index);
}

void Planet::cleanup() {
  for (unsigned int index = m_planets.size(); index > 0; index--) {
    delete m_planets[0];
  }
}

void Planet::drawAll(const Ship& ship, sf::RenderTarget& target,
                     [[maybe_unused]] sf::RenderStates states) {
  for (unsigned int index = 0; index < m_planets.size(); index++) {
    // Realign shading with position of given ship's current position
    m_planets[index]->shader.setUniform(
        "currentPos",
        sf::Vector2f{m_planets[index]->shape.getPosition().x -
                         ship.shape.getPosition().x + target.getSize().x / 2.f,
                     ship.shape.getPosition().y -
                         m_planets[index]->shape.getPosition().y +
                         target.getSize().y / 2.f +
                         2.f /* offset for shading */});

    // Redraw planet
    target.draw(*m_planets[index], m_planets[index]->m_shaderState);
  }
}

void Planet::syncObjects(const sf::Window& referTo) {
  for (unsigned int index = 0; index < m_planets.size(); index++) {
    m_planets[index]->syncObject(referTo);
  }
}

Planet* Planet::add(const sf::Vector2f& position, const float& radius,
                    const sf::Color& color) {
  m_planets.push_back(new Planet(position, radius, color));

  return *m_planets.rbegin();
}

void Planet::applyUnivGravity() {
  // Applies universal gravitation to all combinations of bodies
  for (b2Body* startBody = Box2DBase::world.GetBodyList(); startBody != nullptr;
       startBody = startBody->GetNext()) {
    for (b2Body* moveBody = startBody->GetNext(); moveBody != nullptr;
         moveBody = moveBody->GetNext()) {
      if (moveBody !=
          startBody) {  // shouldn't apply universal gravitation on same body
        b2Vec2 delta = startBody->GetWorldCenter() - moveBody->GetWorldCenter();
        float r = delta.Length();

        float force = kG * moveBody->GetMass() * startBody->GetMass() / (r * r);

        delta.Normalize();
        startBody->ApplyForceToCenter(-force * delta, true);
        moveBody->ApplyForceToCenter(force * delta, true);
      }
    }
  }
}

Planet::Planet(const sf::Vector2f& position, const float& radius,
               const sf::Color& color)
    : Box2DBase(&shape, position, b2_dynamicBody), shape(radius * 30.f) {
  b2CircleShape earthCircle;

  earthCircle.m_p.Set(0.f, 0.f);
  earthCircle.m_radius = radius;

  // Add the Earth fixture to the ground body.
  body->CreateFixture(&earthCircle, 1.f);

  body->SetAngularVelocity(20.f);

  shape.setFillColor(color);
  shape.setOrigin({shape.getRadius(), shape.getRadius()});
  shape.setPosition(position);

  shader.setUniform("radius", shape.getRadius());
  shader.setUniform("centerColor", sf::Glsl::Vec4{shape.getFillColor()});

  m_shaderState.shader = &shader;
}
