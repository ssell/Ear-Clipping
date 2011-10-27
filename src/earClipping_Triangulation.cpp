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

	bool isConvex( Point a, Point b, Point c )
	{
		// a  = c->next; b = c->previous
		// testing what side of the diagonal ac that b is on
		// source: http://www.gamedev.net/topic/542870-determine-which-side-of-a-line-a-point-is/page__view__findpost__p__4500667
		return ( b.x - a.x ) * ( c.y - a.y ) - ( b.y - a.y ) * ( c.x - a.x ) > 0;
	}

	//------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------

	bool verifyEarCount( unsigned count, char* path )
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

	bool recordEars( Polygon poly, char* path )
	{
		std::ofstream file( path );

		if( !file.is_open( ) )
			return false; //failed to open file

		//--------------------------------------------

		bool earFound; // will need to prove this false

		Point* active;

		std::string status = "";

		unsigned numPoints = poly.numPoints( ) - 2;

		//--------------------------------------------

		// if all goes well, there will be n-2 ears (n=number of vertices)
		// let the user (or retrieveEars) know how many ears to expect
		file << numPoints << "\n";

		//--------------------------------------------

		active = poly.get( );

		while( active != poly.get( )->previous )
		{
			earFound = true; // prove me wrong

			if( active == NULL )
				break; // this should never happen

			// Is a convex angle formed? If so, then the diagonal will be inside the polygon.
			// The only exception is if another point of the polygon is within the potential ear.
			if( isConvex( *active->next, *active->previous, *active ) )
			{
				Point* checker = active->next->next;

				if( checker == NULL )
					break; // again should never happen

				// Here comes the part that makes this algorithm O(n^2):
				// check every point (except active->previous, active, and active->next of course)
				// to see if they lie within our new ear
				do
				{
					if( inTriangle( *checker, *active, *active->next, *active->previous ) )
					{
						earFound = false; // contains another point
						break;
					}
     
					checker = checker->next;
				} while( checker != active->previous );
			}
			else
			{
				earFound = false; // concave angle
			}

			if( earFound )
			{
				// hooray! have an ear
				// record to file
				file << active->previous->x << "," << active->previous->y << ":" << active->x << "," << active->y << ":" << active->next->x << "," << active->next->y << "\n";

				// remove ear tip (active) from the polygon)
				if( !poly.removePoint( active->x, active->y, active->next->x, active->next->y, active->previous->x, active->previous->y ) )
					std::cout << "FAILED" << std::endl;

				active = poly.get( ); 
			}
			else
				active = active->next;
		}

		file.close( );

		return verifyEarCount( numPoints, path );
	}
}