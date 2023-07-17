//
// Created by ilia-milioshin on 15.07.23.
//

#include "Body.h"
#include <iostream>


Body::Body(double mass, double x, double y, double vx, double vy, float r, sf::Color c, float minThickness,
           float maxThickness, u_long chainsNum, double minChainLength) : prev_x(x), prev_y(y),
                                                                          mass(mass), x(x), y(y),
                                                                          vx(vx), vy(vy), r(r), c(c),
                                                                          minThickness(minThickness),
                                                                          maxThickness(maxThickness),
                                                                          chainsNum(chainsNum),
                                                                          minChainLength(minChainLength) {
    shape = sf::CircleShape(r);
    shape.setFillColor(c);
    path = deque<sf::LineShape>();
}


void Body::draw(sf::RenderWindow &window) {
    shape.setPosition((float) x - r, (float) y - r);
    window.draw(shape);
}


void Body::drawPath(sf::RenderWindow &window) {
    int s = (int) path.size();
    for (int i = s - 1; i > -1; i--) {
        auto line = path[i];
        auto color = line.getFillColor();
        color.a = (int) ((255.0 * (i + 1)) / s);
        line.setColor(color);
        window.draw(line);
    }
}


void Body::update_path() {
    if ((x - prev_x) * (x - prev_x) + (y - prev_y) * (y - prev_y) < minChainLength) return;

    path.emplace_back(
            sf::Vector2f((float) x, (float) y),
            sf::Vector2f((float) prev_x, (float) prev_y),
            c, std::min(std::max(r / 10, minThickness), maxThickness)
    );

    if (path.size() > chainsNum) path.pop_front();

    prev_x = x;
    prev_y = y;
}
