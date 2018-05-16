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
#include "gtc/epsilon.hpp"
#include <algorithm>
#include <numeric>

namespace EarClipping
{
    //--------------------------------------------------------------------------------------
    // Constructors
    //--------------------------------------------------------------------------------------
    
    Polygon::Polygon()
    {

    }

    //--------------------------------------------------------------------------------------
    // Public Methods
    //--------------------------------------------------------------------------------------
    
    int32_t Polygon::FindPoint(glm::vec3 const& point) const
    {
        auto find = std::find_if(std::begin(m_Points), std::end(m_Points), 
            [point](glm::vec3 const& other)
            { 
                return glm::all(glm::lessThan(glm::abs(point - other), glm::vec3{std::numeric_limits<float>::epsilon()}));
            });

        return std::distance(m_Points.begin(), find);
    }

    void Polygon::AddPoint(glm::vec3 const& point)
    {
        m_Points.push_back(point);
    }

    size_t Polygon::Count() const 
    {
        return m_Points.size();
    }

    //--------------------------------------------------------------------------------------
    // Protected Methods
    //--------------------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------------------
    // Private Methods
    //--------------------------------------------------------------------------------------
}