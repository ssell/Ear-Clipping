/**
 * MIT License
 * 
 * Copyright (c) 2018 Steven T Sell (ssell@vertexfragment.com)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __H_SSELL_EARCLIPPING_POLYGON_H__
#define __H_SSELL_EARCLIPPING_POLYGON_H__

#include "vec2.hpp"

#include <list>
#include <vector>
#include <cstdint>

namespace EarClipping
{
    enum Winding
    {
        Clockwise = 0,
        CounterClockwise = 1
    };

    /**
     * \class Polygon
     * \brief Container for a list of points comprising a polygon.
     */
    class Polygon
    {
    public:
        
        /**
         * \param[in] yIsUp True if positive-y is up (ie origin in lower-left), false if down (ie origin in upper-left).
         */
        Polygon(bool yIsUp);

        /**
         * Tests whether the polygon contains the specified point.
         *
         * \param[in] point Point to test whether it is inside the polygon.
         * \return True if the specified point lies within the polygon.
         */
        bool ContainsPoint(glm::vec2 const& point) const;

        /**
         * \return The index of the point from the start of the point list. Returns -1 if the point was not found.
         */
        int32_t FindPoint(glm::vec2 const& point) const;

        /**
         * Adds the specified point to the polygon to the end of the point list.
         *
         * Points may be added more than once to the same polygon. This is necessary for certain operations
         * such as merging two poylgons together.
         * 
         * \param[in] point Point to add to the polygon.
         */
        void AddPoint(glm::vec2 const& point);

        /**
         *
         */
        std::list<glm::vec2> const& GetPoints() const;
        
        /**
         * Returns the number of points within
         */
        size_t Count() const;

        /**
         * Calculates whether the points are defined in clockwise or counter-clockwise order.
         * This is done by calculating the sum of the point edges. In a normal cartesian coordinate
         * system, a positive result indicates a clockwise winding while a negative result 
         * is indicative of a counter-clockwise winding.
         *
         * \param[in] yIsUp True if positive-y is up (ie origin in lower-left), false if down (ie origin in upper-left).
         */
        Winding CalculateWinding() const;

        /**
         * Orientates the points in the polygon so that they are in counter-clockwise order.
         * If the points are already counter-clockwise, then no further action is taken.
         */
        void OrientatePoints();

        /**
         *
         */
        void AddHole(Polygon hole);

    protected:

        void ValidateChild(Polygon& child);
        std::list<glm::vec2>::iterator FindNearestPoint(Polygon& child);

    private:

        bool m_YUp;

        glm::vec2 m_MinPoint;
        glm::vec2 m_MaxPoint;

        std::list<glm::vec2> m_Points;
    };
}

#endif