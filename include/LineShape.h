//
// Created by ilia-milioshin on 14.12.22.
//

#ifndef LAGRANGE_EXAMPLE_LINE_SHAPE_H
#define LAGRANGE_EXAMPLE_LINE_SHAPE_H


#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Shape.hpp>


namespace sf {
    class SFML_GRAPHICS_API LineShape : public Shape
    {
    public :
        explicit LineShape();

        explicit LineShape(const Color& color, float thickness);

        explicit LineShape(const Vector2f& start, const Vector2f& end, const Color& color, float thickness);

        void setThickness(float thickness);

        float getThickness() const;

        float getLength() const;

        size_t getPointCount() const override;

        Vector2f getPoint(size_t index) const override;

        Vector2f getStart() const;

        void setStart(const Vector2f& s);

        Vector2f getEnd() const;

        void setEnd(const Vector2f& e);

        void setColor(const Color &c);

    private:
        Vector2f start;
        Vector2f end;
        Vector2f m_direction;
        Color color;
        float m_thickness;
    };
}


#endif //LAGRANGE_EXAMPLE_LINE_SHAPE_H
