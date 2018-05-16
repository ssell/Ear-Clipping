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

#include "vec3.hpp"

#include <list>
#include <vector>
#include <cstdint>

namespace EarClipping
{
    /**
     * \class Polygon
     * \brief Container for a list of points comprising a polygon.
     */
    class Polygon
    {
    public:

        Polygon();

        /**
         * \return The index of the point from the start of the point list. Returns -1 if the point was not found.
         */
        int32_t FindPoint(glm::vec3 const& point) const;

        /**
         * Adds the specified point to the polygon to the end of the point list.
         *
         * Points may be added more than once to the same polygon. This is necessary for certain operations
         * such as merging two poylgons together.
         * 
         * \param[in] point Point to add to the polygon.
         */
        void AddPoint(glm::vec3 const& point);
        
        /**
         * Returns the number of points within
         */
        size_t Count() const;

    protected:

    private:

        std::list<glm::vec3> m_Points;
    };
}

#endif