//
// Created by ilia-milioshin on 14.12.22.
//

#include "LineShape.h"
#include <cmath>


namespace sf {
    LineShape::LineShape(const Vector2f &start, const Vector2f &end, const Color &color, float thickness) :
            start(start), end(end), m_direction(end - start), color(color), m_thickness(thickness) {
        setPosition(start);
        setFillColor(color);
        update();
    }


    void LineShape::setThickness(float thickness) {
        m_thickness = thickness;
        update();
    }


    float LineShape::getThickness() const {
        return m_thickness;
    }


    float LineShape::getLength() const {
        return std::sqrt(m_direction.x * m_direction.x + m_direction.y * m_direction.y);
    }


    size_t LineShape::getPointCount() const {
        return 4;
    }


    Vector2f LineShape::getPoint(size_t index) const {
        Vector2f unitDirection = m_direction / getLength();
        Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

        Vector2f offset = (m_thickness / 2.f) * unitPerpendicular;

        switch (index) {
            default:
            case 0:
                return offset;
            case 1:
                return (m_direction + offset);
            case 2:
                return (m_direction - offset);
            case 3:
                return (-offset);
        }
    }

    Vector2f LineShape::getStart() const {
        return start;
    }

    void LineShape::setStart(const Vector2f &s) {
        this->start = Vector2f(s);
        setPosition(this->start);
        m_direction = end - start;
        update();
    }

    Vector2f LineShape::getEnd() const {
        return end;
    }

    void LineShape::setEnd(const Vector2f &e) {
        this->end = Vector2f(e);
        m_direction = end - start;
        update();
    }

    LineShape::LineShape() :
            LineShape(sf::Vector2f(0, 0), sf::Vector2f(100, 0),
                      sf::Color::White, 2.f) {}

    void LineShape::setColor(const Color &c) {
        this->color = c;
        this->setFillColor(color);
        update();
    }

    LineShape::LineShape(const Color &color, float thickness) :
            LineShape(sf::Vector2f(0, 0), sf::Vector2f(100, 0),
                      color, thickness) {}
}
