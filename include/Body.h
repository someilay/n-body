//
// Created by ilia-milioshin on 15.07.23.
//

#ifndef N_BODY_BODY_H
#define N_BODY_BODY_H

#include <SFML/Graphics.hpp>
#include <LineShape.h>
#include <deque>


using std::deque;


class Body {
private:
    deque<sf::LineShape> path;
    sf::CircleShape shape;
    double prev_x, prev_y;
    float minThickness;
    float maxThickness;
    u_long chainsNum;
    double minChainLength;
public:
    double mass, x, y, vx, vy;
    float r;
    sf::Color c;

    Body(double mass, double x = 0, double y = 0, double vx = 0, double vy = 0, float r = 50,
         sf::Color c = sf::Color::Blue, float minThickness = 1, float maxThickness = 10, u_long chainsNum = 200,
         double minChainLength = 10);

    void draw(sf::RenderWindow &window);

    void drawPath(sf::RenderWindow &window);

    void update_path();
};


#endif //N_BODY_BODY_H
