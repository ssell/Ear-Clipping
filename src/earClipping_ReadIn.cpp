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
 
#include "earClipping_Core.h"

#include <fstream>
#include <string>

//------------------------------------------------------------------------------------------

namespace EarClipping
{
	std::vector< float > retrieveEars( char* path )
	{
		std::ifstream file( path );
		std::string line;

		std::vector< float > ears;

		if( file.is_open( ) )
		{
			if( file.good( ) )
				std::getline( file, line );
			else
				return ears;

			//----------------------------------------
			
			unsigned cntr = 0;
			unsigned num = atoi( line.c_str( ) );

			size_t find1;
			size_t find2;
			size_t find3;

			std::string p0;
			std::string p1;
			std::string p2;

			//----------------------------------------

			while( file.good( ) && cntr != num )
			{
				getline( file, line );

				// Line looks like:
				//		X1,Y1:X2,Y2:X3,Y3
				find1 = line.find( ':' );

				if( find1 != std::string::npos )
					p0 = line.substr( 0, find1 );
				else
					return ears;

				find2 = line.find( ':', find1 + 1 );

				if( find2 != std::string::npos )
					p1 = line.substr( find1 + 1, find2 - find1 - 1 );
				else
					return ears;

				p2 = line.substr( find2 + 1 );

				//------------------------------------
				// Now have the 3 points. Separate them into X and Y components and add to array

				find1 = p0.find( ',' );
				find2 = p1.find( ',' );
				find3 = p2.find( ',' );

				if( find1 == std::string::npos ||
					find2 == std::string::npos ||
					find3 == std::string::npos )
					return ears;

				ears.push_back( atoi( p0.substr( 0, find1 ).c_str( ) ) );
				ears.push_back( atoi( p0.substr( find1 + 1 ).c_str( ) ) );
				ears.push_back( atoi( p1.substr( 0, find2 ).c_str( ) ) );
				ears.push_back( atoi( p1.substr( find2 + 1 ).c_str( ) ) );
				ears.push_back( atoi( p2.substr( 0, find3 ).c_str( ) ) );
				ears.push_back( atoi( p2.substr( find3 + 1 ).c_str( ) ) );

				cntr++;
			}
			
			file.close( );
		}

		return ears;
	}
}