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

#include "TriangulationData.hpp"
#include "gtc/epsilon.hpp"

#include <algorithm>
#include <stdexcept>
#include <numeric>

namespace EarClipping
{
    //--------------------------------------------------------------------------------------
    // Constructors
    //--------------------------------------------------------------------------------------
    
    TriangulationData::TriangulationData(uint32_t const numPoints)
    {
        const auto numEars = numPoints - 2;

        m_Indices.reserve(numEars * 3);         // 3 indices for each ear/triangle
        m_Vertices.reserve(numPoints);          // One unique vertex for each point
    }

    //--------------------------------------------------------------------------------------
    // Public Methods
    //--------------------------------------------------------------------------------------

    uint32_t TriangulationData::GetIndex(glm::vec2 const& point) const
    {
        auto find = std::find_if(std::begin(m_Vertices), std::end(m_Vertices), 
            [point](glm::vec2 const& other)
            { 
                return glm::all(glm::lessThan(glm::abs(point - other), glm::vec2{std::numeric_limits<float>::epsilon()}));
            });

        if(find == std::end(m_Vertices))
        {
            throw std::exception("No index found that matches the specified point");
        }

        // std::distance can return negative integer value, but not when we call it comparing against std::begin
        return static_cast<uint32_t>(std::distance(std::begin(m_Vertices), find));
    }

    void TriangulationData::AddEar(glm::vec2 const& a, glm::vec2 const& b, glm::vec2 const& c)
    {
        AddPoint(a);
        AddPoint(b);
        AddPoint(c);
    }

    std::vector<uint32_t> const& TriangulationData::GetIndices() const
    {
        return m_Indices;
    }

    std::vector<glm::vec2> const& TriangulationData::GetVertices() const
    {
        return m_Vertices;
    }
    
    //--------------------------------------------------------------------------------------
    // Protected Methods
    //--------------------------------------------------------------------------------------

    void TriangulationData::AddPoint(glm::vec2 const& point)
    {
        auto index = static_cast<uint32_t>(m_Vertices.size());

        try
        {
            index = GetIndex(point);
        } 
        catch(...) 
        {
            m_Vertices.emplace_back(point);
        }

        m_Indices.emplace_back(index);
    }
    
    //--------------------------------------------------------------------------------------
    // Private Methods
    //--------------------------------------------------------------------------------------
}