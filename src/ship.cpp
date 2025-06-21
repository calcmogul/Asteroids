// Copyright (c) Tyler Veness

#include "ship.hpp"

#include <SFML/Graphics/Image.hpp>

sf::Texture Ship::m_shipTexture;
bool Ship::m_isLoaded = false;
float Ship::m_maxSpeed = 10.f;

Ship::Ship(const sf::Vector2f& position, float fullHealth)
    : Box2DBase(&shape, position, b2_dynamicBody), shape(6) {
  if (!m_isLoaded) {
    sf::Image shipImage{"resources/GalagaShip.png"};

    if (!m_shipTexture.loadFromImage(shipImage)) {
      std::exit(1);
    }

    m_isLoaded = true;
  }

  m_health = fullHealth;

  b2PolygonShape shipTriangle;

  b2Vec2
      shipVertices[6];  // all shifted left by 15 pixels and down by 20 pixels
  shipVertices[0].Set(-1.f / 30.f, (m_shipTexture.getSize().y - 20.f) / 30.f);
  shipVertices[1].Set(-15.f / 30.f, (m_shipTexture.getSize().y - 36.f) / 30.f);
  shipVertices[2].Set(-15.f / 30.f, (m_shipTexture.getSize().y - 51.f) / 30.f);
  shipVertices[3].Set(14.f / 30.f, (m_shipTexture.getSize().y - 51.f) / 30.f);
  shipVertices[4].Set(14.f / 30.f, (m_shipTexture.getSize().y - 36.f) / 30.f);
  shipVertices[5].Set(0.f / 30.f, (m_shipTexture.getSize().y - 20.f) / 30.f);

  shipTriangle.Set(shipVertices, 6);

  // Add the shape to the body.
  body->CreateFixture(&shipTriangle, 1.f);

  /* ===== Create SFML shape ===== */
  for (unsigned int index = 0; index < 6; index++) {
    shape.setPoint(index, sf::Vector2f(shipVertices[index].x * 30.f,
                                       m_shipTexture.getSize().y -
                                           shipVertices[index].y * 30.f));
  }
  shape.setOrigin({0.f, static_cast<float>(m_shipTexture.getSize().y)});

  shape.setTexture(&m_shipTexture);
  /* ============================= */
}

Ship::~Ship() {}

void Ship::controlShip() {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
    body->SetAngularVelocity(body->GetAngularVelocity() + 0.1f);
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
    body->SetAngularVelocity(body->GetAngularVelocity() - 0.1f);
  }

  // limit body angle to between 0 and 2 * pi
  float tempAngle = body->GetAngle();
  if (tempAngle > 2.f * b2_pi) {
    body->SetTransform(body->GetPosition(), tempAngle - 2.f * b2_pi);
  } else if (tempAngle < 0.f) {
    body->SetTransform(body->GetPosition(), tempAngle + 2.f * b2_pi);
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
    body->ApplyForceToCenter(
        b2Vec2(7.5f * std::cos(body->GetAngle() + b2_pi / 2.f),
               7.5f * std::sin(body->GetAngle() + b2_pi / 2)),
        true);
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
    body->ApplyForceToCenter(
        b2Vec2(-7.5f * std::cos(body->GetAngle() + b2_pi / 2.f),
               -7.5f * std::sin(body->GetAngle() + b2_pi / 2)),
        true);
  }

  m_shipSpeed = body->GetLinearVelocity();
  /*if ( m_shipSpeed.Length() > m_maxSpeed ) {
      float angle = std::atan2( m_shipSpeed.y , m_shipSpeed.x );

      body->SetLinearVelocity( b2Vec2( m_shipSpeed.x - ( m_shipSpeed.Length() -
  m_maxSpeed ) * std::cos( angle ) , m_shipSpeed.y - ( m_shipSpeed.Length() -
  m_maxSpeed ) * std::sin( angle ) ) );
  }*/
}

float Ship::getHealth() {
  return m_health;
}
