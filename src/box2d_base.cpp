// Copyright (c) Tyler Veness

#include "box2d_base.hpp"

float BoxToSFML_x(float x) {
  return x * 30.f;
}

float BoxToSFML_y(float y, float boxHeight) {
  return boxHeight - y * 30.f;
}

sf::Vector2f BoxToSFML(float x, float y, float boxHeight) {
  return sf::Vector2f(x * 30.f, boxHeight - y * 30.f);
}

float SFMLToBox_x(float x) {
  return x / 30.f;
}

float SFMLToBox_y(float y, float boxHeight) {
  return (boxHeight - y) / 30.f;
}

b2Vec2 SFMLToBox(float x, float y, float boxHeight) {
  return b2Vec2(x / 30.f, (boxHeight - y) / 30.f);
}

b2World Box2DBase::world(b2Vec2(0.f, 0.f));

Box2DBase::Box2DBase(sf::Shape* sfShape, const sf::Vector2f& position,
                     b2BodyType bodyType) {
  b2BodyDef bodyDef;

  bodyDef.type = bodyType;
  bodyDef.position = b2Vec2(position.x / 30.f, (600 - position.y) / 30.f);

  body = world.CreateBody(&bodyDef);

  drawShape = sfShape;
}

Box2DBase::~Box2DBase() {
  world.DestroyBody(body);
}

void Box2DBase::syncObject(const sf::Window& referTo) {
  drawShape->setPosition(BoxToSFML(body->GetPosition().x, body->GetPosition().y,
                                   referTo.getSize().y));
  drawShape->setRotation(sf::radians(-body->GetAngle()));
}

void Box2DBase::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(*drawShape, states);
}
