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

#ifndef __H_SSELL_EARCLIPPING_UTILS_H__
#define __H_SSELL_EARCLIPPING_UTILS_H__

#include "Polygon.hpp"

namespace EarClipping
{
    namespace Utils
    {
        extern bool IsZero(float const x);
        extern bool LinesIntersect(glm::vec2 const& a0, glm::vec2 const& a1, glm::vec2 const& b0, glm::vec2 const& b1);
        extern bool LineIntersectsPolygon(Polygon const& polygon, glm::vec2 const& l0, glm::vec2 const& l1);
        extern bool IsConvex(glm::vec2 const& a, glm::vec2 const& b, glm::vec2 const& c);
    }
}

#endif