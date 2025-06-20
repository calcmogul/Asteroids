//=============================================================================
//File Name: Main.cpp
//Description: Runs main game
//Author: Tyler Veness
//=============================================================================

#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Sleep.hpp>
#include <cmath>

#include "Ship.hpp"
#include "Planet.hpp"
#include "ProgressBar.hpp"
#include "Bullet.hpp"

int main() {
    sf::RenderWindow mainWin( sf::VideoMode( {800u , 600u} )  , "Prototype I: Space Exploration" , sf::Style::Resize | sf::Style::Close );
    mainWin.setMouseCursorVisible( false );
    mainWin.setVerticalSyncEnabled( true );

    sf::Image appIcon{"resources/GalagaShip.png"};
    mainWin.setIcon( appIcon );

    sf::Texture backgroundTexture{"resources/SpaceBackground.png"};

    backgroundTexture.setRepeated( true ); // Tiling background

    sf::Sprite backgroundSprite( backgroundTexture );
    backgroundSprite.setTextureRect( sf::IntRect( {-86 , -86} , {static_cast<int>(mainWin.getSize().x + 2 * 86) , static_cast<int>(mainWin.getSize().y + 2 * 86)} ) );
    backgroundSprite.setPosition( {0.f , 0.f} );

    sf::RectangleShape HUDBackground( sf::Vector2f( mainWin.getSize().x , 45.f ) );
    HUDBackground.setFillColor( sf::Color( 90 , 90 , 90 , 170 ) );

    Ship myShip( sf::Vector2f( 0.f , 300.f ) , 100.f );
    ProgressBar healthBar( sf::Vector2f( 100.f , 19.f ) , "Health" , sf::Color( 120 , 0 , 0 ) , sf::Color( 40 , 40 , 40 ) , sf::Color( 50 , 50 , 50 ) );

    Planet::add( sf::Vector2f( 0.f , 200.f ) , 5.f , sf::Color( 0 , 210 , 0 ) );

#if 0
    for ( unsigned int index = 0 ; index < 20 ; index++ ) {
        Planet* tempPlanet = Planet::add( sf::Vector2f( rand() % 2000 - 1000 , rand() % 2000 - 1000 ) , (rand() % 149 + 1.f) / 30.f , sf::Color( rand() % 256 , rand() % 256 , rand() % 256 ) );
        float temp = rand() / 100.f;
        float randDir = (temp - static_cast<int>(temp)) * 100.f;
        tempPlanet->body->SetLinearVelocity( b2Vec2( 30.f * cos( randDir ) , 30.f * sin( randDir ) ) );
    }
#endif
    /*Planet::add( sf::Vector2f( 0.f , 0.f ) , 100.f / 30.f , sf::Color( 0 , 0 , 255 ) ); // 200.f / 30.f
    Planet::add( sf::Vector2f( 500.f , 0.f ) , 100.f / 30.f , sf::Color( 255 , 0 , 0 ) );
    Planet::add( sf::Vector2f( 900.f , 0.f ) , 1000.f / 30.f , sf::Color( 255 , 255 , 0 ) );*/

    // Prepare for simulation. Typically we use a time step of 1/60 of a
    // second (60Hz) and 10 iterations. This provides a high quality simulation
    // in most game scenarios.
    float timeStep = 1.0f / 60.f;
    int32 velocityIterations = 1; //6
    int32 positionIterations = 1; //2

    mainWin.setView( sf::View( sf::FloatRect( {myShip.shape.getPosition().x - mainWin.getSize().x / 2 , myShip.shape.getPosition().y - mainWin.getSize().y / 2} , {static_cast<float>(mainWin.getSize().x) , static_cast<float>(mainWin.getSize().y)} ) ) );

    sf::Clock shootClock;

    bool isPaused = false;

    //backgroundMusic.play();
    while ( mainWin.isOpen() ) {
        while ( auto event = mainWin.pollEvent() ) {
            if ( event->is<sf::Event::Closed>() ) {
                mainWin.close();
            }
            else if ( event->is<sf::Event::Resized>() ) {
                backgroundSprite.setTextureRect( sf::IntRect( {-86 , -86}, {static_cast<int>(mainWin.getSize().x + 2.f * 86.f) , static_cast<int>(mainWin.getSize().y + 2.f * 86.f)} ) );

                HUDBackground.setScale( {mainWin.getSize().x / 50.f , HUDBackground.getScale().y} );
            }

            else if ( auto key_event = event->getIf<sf::Event::KeyReleased>() ) {
                if ( key_event->code == sf::Keyboard::Key::Enter ) {
                    isPaused = !isPaused;
                }
            }
        }

        if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::Escape ) ) {
            mainWin.close();
        }

        if ( !isPaused ) {
            // Instruct the world to perform a single step of simulation.
            // It is generally best to keep the time step and iterations fixed.
            Box2DBase::world.Step( timeStep , velocityIterations , positionIterations );

            myShip.syncObject( mainWin );
            Planet::syncObjects( mainWin );
            Bullet::syncObjects( mainWin );

            Planet::applyUnivGravity();
            Bullet::checkCollisions( myShip , mainWin );

            myShip.controlShip();

            if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::Space ) && shootClock.getElapsedTime().asMilliseconds() > 100 ) {
                //Bullet::add( myShip , mainWin );
                Planet* tempPlanet = Planet::add( sf::Vector2f( myShip.drawShape->getPosition().x + 1.5f * cos( myShip.body->GetAngle() + b2_pi / 2.f ) , myShip.drawShape->getPosition().y + 1.5f * sin( myShip.body->GetAngle() + b2_pi / 2.f ) ) , (rand() % 49 + 1.f) / 30.f , sf::Color( rand() % 256 , rand() % 256 , rand() % 256 ) );
                tempPlanet->body->SetLinearVelocity( b2Vec2( 15.f * cos( myShip.body->GetAngle() + b2_pi / 2.f ) , 15.f * sin( myShip.body->GetAngle() + b2_pi / 2.f ) ) );
                shootClock.restart();
            }

            /* ===== Handle background texture shifting with ship ===== */
            // Move background left
            if ( myShip.shape.getPosition().x - backgroundSprite.getPosition().x < mainWin.getSize().x / 2 + 86.f ) {
                backgroundSprite.setPosition( {backgroundSprite.getPosition().x - 86.f * ( std::ceil( std::fabs( myShip.shape.getPosition().x - backgroundSprite.getPosition().x - mainWin.getSize().x / 2 - 86.f ) / 86.f ) ) , backgroundSprite.getPosition().y} );
            }

            // Move background right
            if ( myShip.shape.getPosition().x - backgroundSprite.getPosition().x > mainWin.getSize().x / 2 + 86.f ) {
                backgroundSprite.setPosition( {backgroundSprite.getPosition().x + 86.f * ( std::ceil( std::fabs( myShip.shape.getPosition().x - backgroundSprite.getPosition().x - mainWin.getSize().x / 2 - 86.f ) / 86.f ) ) , backgroundSprite.getPosition().y} );
            }

            // Move background up
            if ( myShip.shape.getPosition().y - backgroundSprite.getPosition().y < mainWin.getSize().y / 2 + 86.f ) {
                backgroundSprite.setPosition( {backgroundSprite.getPosition().x , backgroundSprite.getPosition().y - 86.f * ( std::ceil( std::fabs( myShip.shape.getPosition().y - backgroundSprite.getPosition().y - mainWin.getSize().y / 2 - 86.f ) / 86.f ) )} );
            }

            // Move background down
            if ( myShip.shape.getPosition().y - backgroundSprite.getPosition().y > mainWin.getSize().y / 2 + 86.f ) {
                backgroundSprite.setPosition( {backgroundSprite.getPosition().x , backgroundSprite.getPosition().y + 86.f * ( std::ceil( std::fabs( myShip.shape.getPosition().y - backgroundSprite.getPosition().y - mainWin.getSize().y / 2 - 86.f ) / 86.f ) )} );
            }
            /* ======================================================== */

            healthBar.setPercent( myShip.getHealth() );
            //healthBar.shader.setParameter( "currentPos" , healthBar.getPosition().x - myShip.shape.getPosition().x + mainWin.getSize().x / 2.f , myShip.shape.getPosition().y - healthBar.getPosition().y + mainWin.getSize().y / 2.f );
            healthBar.shader.setUniform( "currentPos" , sf::Vector2f{6.f , mainWin.getSize().y - 6.f} );
        }

        HUDBackground.setPosition( {mainWin.getView().getCenter().x - mainWin.getSize().x / 2.f , mainWin.getView().getCenter().y - mainWin.getSize().y / 2.f} );
        healthBar.setPosition( mainWin.getView().getCenter().x - mainWin.getSize().x / 2.f + 6.f , mainWin.getView().getCenter().y - mainWin.getSize().y / 2.f + 6.f );

        mainWin.clear( sf::Color( 10 , 10 , 10 ) );

        mainWin.draw( backgroundSprite );
        Planet::drawAll( myShip , mainWin );
        mainWin.draw( myShip );
        Bullet::drawAll( mainWin );

        mainWin.draw( HUDBackground );
        mainWin.draw( healthBar );

        mainWin.display();

        mainWin.setView( sf::View( sf::FloatRect( {myShip.shape.getPosition().x - mainWin.getSize().x / 2.f , myShip.shape.getPosition().y - mainWin.getSize().y / 2.f} , sf::Vector2f{mainWin.getSize()} ) ) );
    }

    Planet::cleanup();
    Bullet::cleanup();

    return 0;
}
