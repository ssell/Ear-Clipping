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

#include "Polygon.hpp"
#include "Utils.hpp"
#include "gtc/epsilon.hpp"

#include <algorithm>
#include <numeric>
#include <exception>
#include <string>
#include <sstream>

namespace EarClipping
{
    //--------------------------------------------------------------------------------------
    // Constructors
    //--------------------------------------------------------------------------------------
    
    Polygon::Polygon(bool const yIsUp)
        : m_YUp{yIsUp},
          m_MinPoint{std::numeric_limits<float>::max()},
          m_MaxPoint{std::numeric_limits<float>::min()}
    {

    }

    //--------------------------------------------------------------------------------------
    // Public Methods
    //--------------------------------------------------------------------------------------

    bool Polygon::ContainsPoint(glm::vec2 const& point) const
    {
        if(m_Points.size() < 3)
        {
            return false;
        }

        if((point.x < m_MinPoint.x) || (point.x > m_MaxPoint.x) ||
           (point.y < m_MinPoint.y) || (point.y > m_MaxPoint.y))
        {
            return false;
        }

        auto inside = bool{false};

        for(auto curr = m_Points.begin(); curr != m_Points.end(); ++curr)
        {
            auto next = (std::next(curr) == std::end(m_Points)) ? std::begin(m_Points) : std::next(curr);

            // 1 < ((0 - 3) * (1 - 3) / (0 - 3) + 1
            if((curr->y > point.y) != (next->y > point.y) &&     // If point.y is between curr->y and next->y
               point.x < (next->x - curr->x) * (point.y - curr->y) / (next->y - curr->y) + curr->x)
            {
                inside = !inside;
            }
        }

        return inside;
    }

    int32_t Polygon::FindPoint(glm::vec2 const& point) const
    {
        auto find = std::find_if(std::begin(m_Points), std::end(m_Points), 
            [point](glm::vec2 const& other)
            { 
                return glm::all(glm::lessThan(glm::abs(point - other), glm::vec2{std::numeric_limits<float>::epsilon()}));
            });

        return static_cast<int32_t>(std::distance(std::begin(m_Points), find));
    }

    void Polygon::AddPoint(glm::vec2 const& point)
    {
        m_MinPoint = glm::vec2{std::min(m_MinPoint.x, point.x), std::min(m_MinPoint.y, point.y)};
        m_MaxPoint = glm::vec2{std::max(m_MaxPoint.x, point.x), std::max(m_MaxPoint.y, point.y)};

        m_Points.push_back(point);
    }

    std::list<glm::vec2> const& Polygon::GetPoints() const
    {
        return m_Points;
    }

    size_t Polygon::Count() const 
    {
        return m_Points.size();
    }
    
    Winding Polygon::CalculateWinding() const 
    {
        auto sum = float{0.0f};

        for(auto iter = std::begin(m_Points); iter != std::end(m_Points); ++iter)
        {
            auto next = (std::next(iter) == m_Points.end()) ? std::begin(m_Points) : std::next(iter);
            sum += (next->x - iter->x) * (next->y + iter->y);
        }

        if(m_YUp)
        {
            return (sum < 0.0f) ? Winding::CounterClockwise : Winding::Clockwise;
        }
        else
        {
            return (sum < 0.0f) ? Winding::Clockwise : Winding::CounterClockwise;
        }
    }

    void Polygon::OrientatePoints()
    {
        if(CalculateWinding() == Winding::CounterClockwise)
        {
            return;
        }

        std::reverse(m_Points.begin(), m_Points.end());
        std::swap(m_Points.begin(), std::prev(m_Points.end()));
    }

    void Polygon::AddHole(Polygon hole)
    {
        /** 
         * The nearest point creates a bridge with the first point of the hole polygon.
         * We use these two points to merge the hole polygon into the parent.
         * So we simply insert the hole polygon after the nearest point, and then 
         * duplicate the bridge points (nearest and first in hole).
         * 
         *    Parent: A, B, C, D, E, F
         *    Child:  1, 2, 3
         *    Bridge: E, 1
         *    New:    A, B, C, D, E, 1, 3, 2, 1, E, F
         *    
         *        C---------------------------------B
         *       /                                  |
         *      /            3                      |
         *     /           /  \                     |
         *    D           /    \                    |
         *     \         /      \                   |
         *      \       /        \                  |
         *       \    1-----------2                 |
         *        \ /                               |
         *         E                                |
         *          \                               |
         *            F-----------------------------A
         */

        ValidateChild(hole);

        // Find the points nearest each other on the two polygons that have line-of-sight on each other
        const auto nearest = FindNearestPoint(hole);

        if(nearest == std::end(m_Points))
        {
            throw std::exception("Failed to find nearest point pair in the two polygons");
        }

        // Create a modifiable copy of the hole points and then duplicate the bridge points at the end
        auto holePoints = std::list<glm::vec2>(hole.GetPoints());
        holePoints.push_back(holePoints.front());
        holePoints.push_back(*nearest);

        // Insert the hole polygon into the parent just after the nearest point
        m_Points.splice(std::next(nearest), holePoints);
    }

    //--------------------------------------------------------------------------------------
    // Protected Methods
    //--------------------------------------------------------------------------------------
    
    void Polygon::ValidateChild(Polygon& child)
    {
        if(child.Count() < 3)
        {
            throw std::exception{"Failed to validate polygon: Minimum of three points required."};
        }

        for(auto point : child.GetPoints())
        {
            if(!ContainsPoint(point))
            {
                std::stringstream sstream;
                sstream << "Failed to validate polygon: Point (" << point.x << ", " << point.y << ") lies outside of the parent polygon.";

                throw std::exception(sstream.str().c_str());
            }
        }

        child.OrientatePoints();
    }

    std::list<glm::vec2>::iterator Polygon::FindNearestPoint(Polygon& child)
    {
        // Finds the point in the parent polygon that is the nearest visible point to
        // the first point in the child polygon. This will be the bridge points where
        // we integrate the hole into the parent.

        // The point to compare against in the child is arbitrary, and any point can
        // be used. So for simplicity we choose the first.

        auto nearest = std::end(m_Points);
        auto distance = std::numeric_limits<float>::max();

        const auto firstChild = child.GetPoints().front();

        for(auto iter = std::begin(m_Points); iter != std::end(m_Points); ++iter)
        {
            auto d = glm::distance(*iter, firstChild);

            if((d < distance) &&                                              // Nearest point
               !Utils::LineIntersectsPolygon(this, *iter, firstChild) &&      // Does not intersect a parent polygon segment
               !Utils::LineIntersectsPolygon(child, *iter, firstChild))       // Does not intersect a child polygon segment
            {
                nearest = iter;
                distance = d;
            }
        }

        return nearest;
    }

    //--------------------------------------------------------------------------------------
    // Private Methods
    //--------------------------------------------------------------------------------------
}