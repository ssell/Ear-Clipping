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

#include "Triangulator.hpp"
#include "Polygon.hpp"

namespace EarClipping
{
    //--------------------------------------------------------------------------------------
    // Constructors
    //--------------------------------------------------------------------------------------
    
    Triangulator::Triangulator()
    {

    }

    //--------------------------------------------------------------------------------------
    // Public Methods
    //--------------------------------------------------------------------------------------
    
    TriangulationData Triangulator::Triangulate(Polygon const& polygon)
    {
        if(polygon.Count() < 3)
        {
            throw std::exception("Failed to triangulate polygon: Minimum of three points is required.");
        }

        auto points = std::list<glm::vec2>{polygon.GetPoints()};
        auto current = points.begin();

        const auto expectedNumEars = static_cast<uint32_t>(points.size() - 2);
        auto discoveredEars = uint32_t{0};

        auto data = TriangulationData{static_cast<uint32_t>(points.size())};

        while(discoveredEars < expectedNumEars)
        {
            auto prev = (current == points.begin()) ? std::end(points) : std::prev(current);
            auto next = (std::next(current) == points.end()) ? std::begin(points) : std::next(current);

            if(FormsInteriorAngle(*prev, *current, *next))
            {
                if(IsValidEar(prev, current, next, points))
                {
                    data.AddEar(*prev, *current, *next);          // Add the ear to the triangulation data
                    current = points.erase(current);              // Remove the current point from the point list
                    discoveredEars++;                             // Increment our count of ears

                    continue;
                }
            }

            current = next;
        }

        return data;
    }

    //--------------------------------------------------------------------------------------
    // Protected Methods
    //--------------------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------------------
    // Private Methods
    //--------------------------------------------------------------------------------------

    bool Triangulator::FormsInteriorAngle(glm::vec2 const& a, glm::vec2 const& b, glm::vec2 const& c)
    {
        /** 
         * To determine if the three points form an interior angle or exterior angle, 
         * we imagine the points A and B sitting on an infinite line. If point C is on
         * the left-side of this line, then we form an interior angle. If point C is on
         * the right-side of this line, then we form an exterior angle.
         *
         * The definition of left vs right is determined by the winding order of the points.
         * As we enforce a counter-clockwise order, then we say that left/right is defined
         * by the orientation of A going to B.
         * 
         * -------------------------------  -------------------------------
         * | C left of line AB, Interior |  | C right of line AB, Exterior|
         * -------------------------------  -------------------------------
         *
         *       -------------                    -------------
         *       --A-------C--                    ------B------
         *       --*-------*--                    ------*------
         *          \-----/                       -----/ \-----
         *           \---/                        ----/   \----
         *            \-/                         ---/     \--- 
         *             *                          --*       *--
         *             B                            A       C
         */

        // < 0 = exterior; == 0 = on AB; > 0 = exterior
        return ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x)) >= 0;
    }

    bool Triangulator::IsValidEar(std::list<glm::vec2>::iterator const& prev, std::list<glm::vec2>::iterator const& curr, std::list<glm::vec2>::iterator const& next, std::list<glm::vec2> const& points)
    {
        // Verify that no other points are in the prospective ear.
        // If so, then we can not yet clip this ear.

        const auto a = (*prev);
        const auto b = (*curr);
        const auto c = (*next);

        const auto area = float{0.5f * ((-b.y * c.x) + (a.y * (-b.x + c.x)) + (a.x * (b.y - c.y)) + (b.x * c.y))};

        auto point = (std::next(next) == std::end(points)) ? std::begin(points) : std::next(next);

        while(point != prev)
        {
            if(InTriangle(a, b, c, area, *point))
            {
                return false;
            }

            point = (std::next(point) == std::end(points)) ? std::begin(points) : std::next(point);
        }

        return true;
    }

    bool Triangulator::InTriangle(glm::vec2 const& a, glm::vec2 const& b, glm::vec2 const&c, float const area, glm::vec2 const& p)
    {
        // https://stackoverflow.com/a/2049712/735425
        // https://stackoverflow.com/a/14382692/735425

        const auto s = float{1.0f / (2.0f * area) * ((a.y * c.x) - (a.x * c.y + (c.y - a.y) * p.x) + ((a.x - c.x) * p.y))};
        const auto t = float{1.0f / (2.0f * area) * ((a.x * b.y) - (a.y * b.x + (a.y - b.y) * p.x) + ((b.x - a.x) * p.y))};

        return ((0.0f <= s) && (s <= 1.0f)) && 
               ((0.0f <= t) && (t <= 1.0f)) && 
               ((s + t) <= 1.0f);
    }
}