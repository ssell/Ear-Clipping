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
#include <iostream>
#include <string>
//------------------------------------------------------------------------------------------

namespace EarClipping
{
	bool inTriangle( Point pointToCheck, Point earTip, Point earTipPlusOne, Point earTipMinusOne )
	{
		/*
			http://www.blackpawn.com/texts/pointinpoly/default.html
			Original equation

			P = A + u * ( C - A ) + v * ( B - A )

			->  ( P - A ) = u * ( C - A ) + c * ( B - A )
			->  v2 = u * v0 + v1

			->  v2 DOT v0 = ( u * v0 + v * v1 ) DOT v0
			->  v2 DOT v1 = ( u * v0 + v * v1 ) DOT v1

			->  v2 DOT v0 = u * ( v0 DOT v0 ) + v * ( v1 DOT v0 )
			->  v2 DOT v1 = u * ( v0 DOT v1 ) + v * ( v1 DOT v1 )
		*/

		if( ( pointToCheck.x == earTip.x && pointToCheck.y == earTip.y ) ||
			( pointToCheck.x == earTipPlusOne.x && pointToCheck.y == earTipPlusOne.y ) ||
			( pointToCheck.x == earTipMinusOne.x && pointToCheck.y == earTipMinusOne.y ) )
			return false; // ignore duplicates

		Vector v0 = Vector( ( earTipMinusOne.x - earTip.x ), ( earTipMinusOne.y - earTip.y ) );
		Vector v1 = Vector( ( earTipPlusOne.x - earTip.x ), ( earTipPlusOne.y - earTip.y ) );
		Vector v2 = Vector( ( pointToCheck.x - earTip.x ), ( pointToCheck.y - earTip.y ) );

		float u = ( v1.dot( v1 ) * v2.dot( v0 ) - v1.dot( v0 ) * v2.dot( v1 ) ) / ( v0.dot( v0 ) * v1.dot( v1 ) - v0.dot( v1 ) * v1.dot( v0 ) );
		float v = ( v0.dot( v0 ) * v2.dot( v1 ) - v0.dot( v1 ) * v2.dot( v0 ) ) / ( v0.dot( v0 ) * v1.dot( v1 ) - v0.dot( v1 ) * v1.dot( v0 ) );

		// u OR v < 0 = wrong direction from triangle and thus outside
		// u OR v > 1 = went past triangle and thus outside
		// u  + v > 1  = crossed diagonal of Vi+1 and Vi-1 and thus outside
		if( u < 0 || v < 0 || u > 1 || v > 1 || ( u + v ) > 1 )
			return false;

		return true;
	}

	//------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------

	bool isConvex( Point* active )
	{
		Point a = *active->previous;
		Point b = *active;
		Point c = *active->next;

		// a  = c->next; b = c->previous
		// testing what side of the diagonal ac that b is on
		// source: http://www.gamedev.net/topic/542870-determine-which-side-of-a-line-a-point-is/page__view__findpost__p__4500667
		//return ( b.x - a.x ) * ( c.y - a.y ) - ( b.y - a.y ) * ( c.x - a.x ) > 0;

		// If sign of area is '-', then angle is convex
		return ( ( a.x * ( c.y - b.y ) ) + ( b.x * ( a.y - c.y ) ) + ( c.x * ( b.y - a.y ) ) ) < 0;
	}

	//------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------

	/** 
	 * \brief Check if any other points are inside the prospective ear
	 */
	bool isEar( Point* active )
	{
		Point* checker = active->next->next;

		// Check every point not part of the ear
		while( checker != active->previous )
		{
			if( inTriangle( *checker, *active, *active->next, *active->previous ) )
			{
				return false;
			}

			checker = checker->next;
		}

		return true;
	}

	//------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------

	bool verifyEarCount( unsigned count, const char* path )
	{
		std::ifstream file( path );

		if( file.is_open( ) )
		{
			std::string hold;
			unsigned cnt = 0;

			while( file.good( ) )
			{
				std::getline( file, hold );
				cnt++;
			}

			file.close( );

			if( cnt - 2 == count )
				return true;
		}

		return false;
	}

	//------------------------------------------------------------------------------------------

	bool recordEars( Polygon poly, const char* path )
	{
		std::ofstream file( path );

		if( !file.is_open( ) )
			return false; //failed to open file

		//--------------------------------------------

		bool earFound; // will need to prove this false

		Point* active = poly.get( );

		unsigned numPoints = poly.numPoints( ) - 2;

		//--------------------------------------------

		// if all goes well, there will be n-2 ears (n=number of vertices)
		// let the user (or retrieveEars) know how many ears to expect
		file << poly.numPoints( ) - 2 << "\n";

		//--------------------------------------------

		while( poly.numPoints( ) >= 3 )
		{
			earFound = true;

			if( isConvex( active ) )
			{
				if( isEar( active ) )
				{
					// handle ear
					file << active->previous->x << "," << active->previous->y << ":" << active->x << "," << active->y << ":" << active->next->x << "," << active->next->y << "\n";
					file.flush( );

					// remove ear tip (active) from the polygon)
					active = active->next;
					poly.removePoint( *active->previous );

					continue;
				}
			}

			active = active->next;
		}

		file.close( );

		return verifyEarCount( numPoints, path );
	}

}