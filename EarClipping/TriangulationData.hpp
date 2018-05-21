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

#ifndef __H_SSELL_EARCLIPPING_TRIANGULATION_DATA_H__
#define __H_SSELL_EARCLIPPING_TRIANGULATION_DATA_H__

#include <cstdint>
#include <vector>

#include "vec2.hpp"

namespace EarClipping
{
    class TriangulationData
    {
    public:

        TriangulationData(uint32_t numPoints);

        uint32_t GetIndex(glm::vec2 const& point) const;
        void AddPoint(glm::vec2 const& point);

        std::vector<uint32_t> const& GetIndices() const;
        std::vector<glm::vec2> const& GetVertices() const;

    protected:

    private:

        std::vector<uint32_t> m_Indices;
        std::vector<glm::vec2> m_Vertices;
    };
}

#endif