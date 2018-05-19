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

#include "Utils.hpp"
#include <exception>

namespace EarClipping
{
    namespace Utils
    {
        bool IsZero(float const x)
        {
            return std::abs(x) < std::numeric_limits<float>::epsilon();
        }

        bool LinesIntersect(glm::vec2 const& a0, glm::vec2 const& a1, glm::vec2 const& b0, glm::vec2 const& b1)
        {
            // Derived from multiple sources:
            //     * "Line/Line Intersection Tests." Real-Time Rendering, Third Edition, pp. 780–781.
            //     * "Faster Line Segment Intersection." Graphics Gems 3, pp. 199-202
            //     * http://www.realtimerendering.com/resources/GraphicsGems/gemsiii/insectc.c
            //     * http://www.stefanbader.ch/faster-line-segment-intersection-for-unity3dc/

            const auto a = a1 - a0;
            const auto b = b0 - b1;
            
            const auto denominator = (a.y * b.x) - (a.x * b.y);

            if(IsZero(denominator))
            {
                return false; // Parallel
            }
            
            const auto c = a0 - b0;
            const auto alphaNumerator = (b.y * c.x) - (b.x * c.y);

            if(denominator > 0.0f)
            {
                if((alphaNumerator < 0.0f) || (alphaNumerator > denominator))
                {
                    return false;
                }
            }
            else if((alphaNumerator > 0.0f) || (alphaNumerator < denominator))
            {
                return false;
            }

            const auto betaNumerator = (a.x * c.y) - (a.y * c.x);

            if(denominator > 0.0f)
            {
                if((betaNumerator < 0.0f) || (betaNumerator > denominator))
                {
                    return false;
                }
            }
            else if((betaNumerator > 0.0f) || (betaNumerator < denominator))
            {
                return false;
            }

            return true;
        }

        bool LineIntersectsPolygon(Polygon const& polygon, glm::vec2 const& l0, glm::vec2 const& l1)
        {
            auto points = &polygon.GetPoints();

            // Have to compare against each segment of the polygon.
            for(auto curr = std::begin(*points); curr != std::end(*points); ++curr)
            {
                auto next = (std::next(curr) == std::end(*points)) ? std::begin(*points) : std::next(curr);

                if(LinesIntersect(l0, l1, *curr, *next))
                {
                    return true;
                }
            }

            return false;
        }

        bool IsConvex(glm::vec2 const& a, glm::vec2 const& b, glm::vec2 const& c)
        {
            return ((a.x * (c.y - b.y)) + (b.x * (a.y - c.y)) + (c.x * (b.y - a.y))) < 0.0f;
        }
    }
}