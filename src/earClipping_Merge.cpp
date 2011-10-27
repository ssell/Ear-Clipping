#include "earClipping_Core.h"
#include <algorithm>
#include <iostream>

//------------------------------------------------------------------------------------------

namespace EarClipping
{
	// Used in pointCompare called in orderPoints
	// Can not directly pass it to the method
	Point activePoint;

	/**
	 * \author ssell
	 * \brief Used by the STL sorting algorithm while ordering from smallest x-value to greatest. Distance formula.
	 */
    bool pointCompare( Point i, Point j )
    {
		return std::sqrt( ( ( ( i.x - activePoint.x ) * ( i.x - activePoint.x ) ) + ( ( i.y - activePoint.y ) * ( i.y - activePoint.y ) ) ) ) <
               std::sqrt( ( ( ( j.x - activePoint.x ) * ( j.x - activePoint.x ) ) + ( ( j.y - activePoint.y ) * ( j.y - activePoint.y ) ) ) );
    }

	//--------------------------------------------------------------------------------------

	/**
	 * \author ssell
	 * \brief Ordered the points of the provided Polygon in order of least to greatest distance from param point.
	 */
    std::vector< Point > orderPoints( Polygon &poly, Point point )
	{
		// Put points in poly into a vector
    
		Point head = *poly.get( );

		std::vector< Point > pointContainer;

		activePoint = point;

		/**
			Outer polygon may have duplicate points.
			Fix this loop to compensate for that.
		**/

		for( unsigned i = 0; i < poly.numPoints( ); i++ )
		{
			pointContainer.push_back( head );
			head = *head.next;
		}

		// Sort the points
		std::sort( pointContainer.begin( ), pointContainer.end( ), pointCompare );

		return pointContainer;
	}


	//--------------------------------------------------------------------------------------

	/**
	 * \author ssell
	 * \brief Returns TRUE if the two line segments intersect, else returns FALSE
	 */
    bool doIntersect( Point a, Point b, Point c, Point d, bool endpoint_touch_is_intersection = false )
	{
		Point p = a;
		Point r = b - a;
		Point q = c;
		Point s = d - c;

		if( r.cross( s ) == 0 )
		{
			if( ( q - p ).cross( r ) == 0 )
				return true; // collinear (bad data set?)

			return false; // parallel
		}

		float t = ( q - p ).cross( s ) / r.cross( s );
		float u = ( q - p ).cross( r ) / r.cross( s );

		// u == 0 || u == 1, endpoints touch
		if( t >= 0 && t <= 1 )
		{
			if( (  endpoint_touch_is_intersection && u >= 0 && u <= 1 ) || 
				( !endpoint_touch_is_intersection && u >  0 && u <  1 ) )
				return true;
		}

		if( t >= 0 && t <= 1 && u > 0 && u < 1 )
			return true;

		return false;
	}

	//--------------------------------------------------------------------------------------

	/**
	 * \author ssell
	 * \brief Returns the two closest points of the specified Polygons.
	 *
	 * Finds the Point that belongs to the Outer Polygon that is closest to the specified inner point. 
	 *
	 * \param $first
	 *	"@param" is the points of the outer polygon, ordered from smallest-x to greatest.
	 * \param $second
	 *	The index of the ordered points at which to begin checking.
	 * \param $third
	 *	Outer (parent) Polygon
	 * \param $fourth
	 *	The Point with the smallest x-value of the inner polygon.
	 */
    Point getClosest( std::vector< Point > pointsOrdered, int index, Polygon* poly, Point innerPoint )
    {
		Point a = innerPoint;
		Point b = pointsOrdered[ index ];
		Point* c = poly->get( );

		bool intersection = false;

		//--------------------------------------------

		/*
			Segment AB is the segment formed by the
			farthest 'left' point of the inner polygon
			and the 'closest' point on the outer polygon.

			Segment AB is then compared to each segment
			of the outer polygon to make sure that the
			segment C,C+1 does not block the LOS of the
			points A and B.

			If the segments intersect then it is checked
			if AB is intersected by the next segments of
			the outer polygon.
		*/
		do
		{
			//Does segment [a,b] intersect with [c,c.next]?
			intersection = doIntersect( a, b, *c, *c->next );
			c = c->next;
		} while( ( intersection == false ) && ( c != poly->get( ) ) );

		if( !intersection )
		{
			c = &a;

			// Make sure [a,b] does not intersect any points of the inner polygon now
			do
			{
				intersection = doIntersect( a, b, *c, *c->next );
				c = c->next;
			} while( ( intersection == false ) && ( c->x != a.x && c->y != a.y ) );

			// no intersection anywhere. b is valid
			if( !intersection )
				return b;
		}

		/*
			Return b if intersection is false, else if somehow
			segment of the outer polygon intersects with AB then 
			we will increment B to the next closest point on the outer polygon.
		*/
		return getClosest( pointsOrdered, index + 1, poly, innerPoint );
    }

	//--------------------------------------------------------------------------------------

	/**
	 * \author ssell
	 * \brief Finds two, mutually visible Points for the outer (parent) and inner (child) Polygons.
	 *
	 * The line segment found by this function is the new line that will connect the outer and inner polygons.
	 * It is what the merger of the two will based around.
	 */
    std::pair< Point, Point > getSplit( Polygon &outer, Polygon &inner, int smallestX )
    {
		// 1. Get point from inner with X that matches smallestX
		// 2. Find closest mutually visible point on outer to point found in step 1

		std::pair< Point, Point > split;

		Point* smallest = inner.get( );

		do
		{
			if( smallest->x == smallestX )
				break;

			smallest = smallest->next;
		} while( smallest != inner.get( ) );

		// Have the point from inner with the smallest X value
		Point closest = getClosest( orderPoints( outer, *smallest ), 0, &outer, *smallest );

		split.first = *smallest;
		split.second = closest;

        return split;
    }

	//--------------------------------------------------------------------------------------

	/**
	 * \author ssell
	 * \brief Recursive function to find the smallest x-value in a given Point-list
	 * \param $second
			For the initial call, head->next should be provided as the current Point
	 */

	int getSmallest( Point* head, Point* current, unsigned smallest )
	{
		// If the x of the current point is smaller, set smallest to it
		smallest = current->x < smallest ? current->x : smallest;

		// If we are at the head, the the whole list has been traversed
		if( current->x == head->x )
			return smallest;
		else
			getSmallest( head, current->next, smallest );
	}

	/**
	 * \author ssell
	 * \brief Sorts the provided pair. Used in child ordering. first = value, second = child #
	 */
	bool sortThem( std::pair< int, int > i, std::pair< int, int > j )
	{
		return i.second < j.second;
	}

	/**
	 * \author ssell
	 * \brief Orders the specified child Polygons in order of smallest x-value
	 */
	std::vector< std::pair< int, int > > childOrder( std::vector< Polygon* > children )
	{
		int size = children.size( );

		std::vector< std::pair< int, int > > toSort;		//child number, value

		//--------------------------------------------

		Point* head;

		// Get the smallest the x-value for each child Polygon
		for( int i = 0; i < size; i++ )
		{
			head = children[ i ]->get( );

			toSort.push_back( std::pair< int, int >( i, getSmallest( head, head->next, head->x ) ) );
		}

		// Using a simple sort as this should not be the bottle-neck of the program.
		// If you want a more efficient process, then this would be a spot to optimize

		std::sort( toSort.begin( ), toSort.end( ), sortThem );

		return toSort;
	}

	//--------------------------------------------------------------------------------------

	/*
	 * \author ssell
	 * \brief Ensures that the polygon points traverse in the proper direction.
	 */
	void orientatePolygon( Polygon *poly )
	{
		// Direction needs to be counterclockwise for all polygons

		Point* active = poly->get( );

		long total = 0;

		//--------------------------------------------
		// First do parent
		for( int i = 0; i < poly->numPoints( ); i++ )
		{
			total += ( active->next->x - active->x ) * ( active->next->y + active->y );
			active = active->next;
		}

		// Positive, the polygon is clockwise
		if( total >= 0 )
			poly->reverse( -1 );

		//--------------------------------------------
		// Check children
		for( int i = 0; i < poly->numChildren( ); i++ )
		{
			active = poly->getChild( i )->get( );
			total = 0;

			if( active == NULL )
				break;

			for( int j = 0; j < poly->getChild( i )->numPoints( ); j++ )
			{
				total += ( active->next->x - active->x ) * ( active->next->y + active->y );
				active = active->next;
			}

			// Positive, the polygon is clockwise
			if( total >= 0 )
				poly->reverse( i );
		}
	}

	//--------------------------------------------------------------------------------------

	/**
	 * \author ssell
	 * \brief Merges a Polygon with its children to create one unified Polygon that may be triangulated.
	 */
    void mergePolygon( Polygon &poly )
    {
        std::vector< Polygon* > children = poly.getChildren( );
		std::vector< std::pair< int, int > > order = childOrder( children );

		std::pair< Point, Point > connection;

		Point* temp;

		//--------------------------------------------

		for( int i = 0; i < order.size( ); i++ )
		{
			connection = getSplit( poly, *children.at( order[ i ].first ), order[ i ].second );

			/*
			 Insert the points of the child into the parent polygon.
			 We will insert prior to the parent point (connection.second), and insert in the order
			 of smallest->next (connection.first.next), and iterate through the points until
			 we insert the smallest. To finish, the closest will be inserted once more.

			 Parent: A, B, C, D, E, F
			 Child: 1, 2, 3
			 Connection: 1, E
			 New: A, B, C, D, E, 1, 3, 2, 1, E, F

				Terrible ASCII visual


					    C-------------------------------------B
						.                                     |
					   /                                      |
					  .            3                          |
					 /           /  \                         |
					D           /    \                        |
					 .         /      \                       |
					  \       /        \                      |
					   .    1-----------2                     |
						\ /                                    |
						E                                     |
						  \                                   |
							.                                 |
							  F-------------------------------A
			*/

			poly.insertPoint( connection.first.x, connection.first.y, &connection.second );
			temp = &connection.first;

			do
			{
				temp = temp->next;
				poly.insertPoint( temp->x, temp->y, temp->previous );
			} while( temp->x != connection.first.x && temp->y != connection.first.y );

			poly.insertPoint( connection.second.x, connection.second.y, temp );	// insert stops at the first occurence of a particular
																					    // point and so no need to fear the wrong one being used.
		}
    }
}