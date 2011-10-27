/**
 * \file earClipping_Structures.h
 * \author ssell
 * \brief Header for the Point and Polygon data types.
 */

#ifndef __EAR_CLIPPING__STRUCTURES_H__
#define __EAR_CLIPPING__STRUCTURES_H__

//------------------------------------------------------------------------------------------

#include <vector>

//------------------------------------------------------------------------------------------

/*!
 *  \addtogroup EarClipping
 *  @{
 */
namespace EarClipping
{

    //--------------------------------------------------------------------------------------
    // Point

	/**
	 * \struct Point
	 * \brief Basic Point data structure.
	 * \author ssell
	 *
	 * Contains fields for x and y-values as well as methods for calculating dot/cross products.
	 * The points form a doubly-linked circular list.
	 */
    struct Point
    {
        Point( float pX, float pY )
            : x( pX ), y( pY ), next( NULL ), previous( NULL ) { }

        Point( )
            : next( NULL ), previous( NULL ) { }

		///  returns a vector perpendicular to the pair of two-dimensional vectors
		float cross( Point rhs ){ return ( this->x * rhs.y ) - ( this->y * rhs.x ); }

		/// cosine of the angle between the pair of two-dimensional vectors
		float dot( Point rhs ){ return ( this->x * rhs.x ) + ( this->y * rhs.y ); }

        //--------------------------------------------

        float x, y;

        Point* next;
        Point* previous;

        //--------------------------------------------

        bool operator==( const Point rhs ){ return ( this->x == rhs.x ) && ( this->y == rhs.y ); }
        bool operator!=( const Point rhs ){ return !( ( this->x == rhs.x ) && ( this->y == rhs.y ) ); }

		Point operator-( const Point rhs ){ return Point( this->x - rhs.x, this->y - rhs.y ); }
		Point operator+( const Point rhs ){ return Point( this->x + rhs.x, this->y + rhs.y ); }
    };

    //--------------------------------------------------------------------------------------
    // Polygon
    // source: earClipping_Polygon.cpp

	/**
	 * \class Polygon
	 * \author ssell
	 * \brief Container for a list of points comprising a polygon.
	 *
	 * Main point list is composed of the points forming the exterior polygon.
	 * All 'child' polygons represent holes in the parent polygon.
	 * Contains numerous helpers for adding, removing, inserting, finding, etc. specific points.
	 */
    class Polygon
    {

    public:

        /// Creates 3 Points with the Polygon and assigns the Parent polygon (NULL if no parent)
        Polygon( Polygon *p_Parent, float xA, float yA, float xB, float yB, float xC, float yC );
		Polygon( Polygon *p_Parent, float xA, float yA, float xB, float yB );
		Polygon( Polygon *p_Parent, float xA, float yA );
		Polygon( );

        /// Creates an empty Polygon with specified Parent (NULL if no parent)
        Polygon( Polygon *p_Parent );

        /// Deletes all Points existing directly within Polygon. Does not touch child Polygons.
        ~Polygon( );

        /// Adds a child (interior/hole) polygon to this polygon
        bool addChild( Polygon *child );

        /// Return true if point was added. False if adding failed (point already exists in Polygon)
        bool addPoint( float x, float y );

        /// Return true if point was removed. False if point DNE in Polygon (including if Polygon is empty)
        bool removePoint( float x, float y );
		/// Removes the point at the specified location in the list
		bool removePoint( unsigned pos );
		bool removePoint( float aX, float aY, float bX, float bY, float cX, float cY ); //this is a cop-out to simplifiy the triangulation

        /// Return head point in Polygon
        Point* get( ){ return head; }
		Polygon* getChild( unsigned pos ){ if( children.size( ) > pos ) return children[ pos ]; else return NULL; }
		bool removeChild( unsigned pos );

        std::vector< Polygon* > getChildren( ){ return children; }

		///Find a specific point
		Point* getPoint( float x, float y );

		/// Returns the point at the specified position in the list
		Point* getPoint( unsigned position );

		/// Number of points currently comprising the polygon.
		inline unsigned numPoints( ){ return m_NumberOfPoints; }
		inline unsigned numChildren( ){ return children.size( ); }

		/**
		 * Creates a new Point at location of p_Point->previous. Returns TRUE if Point already exists in Polygon.<br>
		 * Insert allows for multiple copies of the same data point, unlike addPoint
		 */
		bool insertPoint( float x, float y, Point* p_Point );

		/// If pos == -1, reverse the order of the points of the polygon. Else reverses order of child with index pos.
		void reverse( int pos );

    protected:

        /// Head of the Point doubly-linked, circular, linked-list
        Point* head;

        /// Collection of all interior Polygons
        std::vector< Polygon* > children;

        /// Number of Points directly associated with this Polygon. Does not include interior Points
        unsigned m_NumberOfPoints;

		bool isDuplicate( float x, float y );

    private:

        /// Parent Polygon. If NULL, then no Parent
        Polygon* parent;

    };
};
/*! @} End of Doxygen Groups*/

//------------------------------------------------------------------------------------------

#endif // __EAR_CLIPPING__STRUCTURES_H__
