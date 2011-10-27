#include "earClipping_Structures.h"
#include <iostream>
//------------------------------------------------------------------------------------------

namespace EarClipping
{
    Polygon::Polygon( Polygon *p_Parent, float xA, float yA, float xB, float yB, float xC, float yC )
    {
        parent = p_Parent;

        // Add the points

        Point* a = new Point( xA, yA );
        Point* b = new Point( xB, yB );
        Point* c = new Point( xC, yC );

        head = a;
        head->next = b;
        head->previous = c;

        b->next = c;
        b->previous = a;

        c->next = a;
        c->previous = b;

        m_NumberOfPoints = 3;

		if( parent != NULL )
        {
            parent->addChild( this );
        }
    }

	 Polygon::Polygon( Polygon *p_Parent, float xA, float yA, float xB, float yB )
    {
        parent = p_Parent;

        // Add the points

        Point* a = new Point( xA, yA );
        Point* b = new Point( xB, yB );

        head = a;
        head->next = b;
        head->previous = b;

        b->next = a;
        b->previous = a;

        m_NumberOfPoints = 2;

		if( parent != NULL )
        {
            parent->addChild( this );
        }
    }

	Polygon::Polygon( Polygon *p_Parent, float xA, float yA )
    {
        parent = p_Parent;

        // Add the points

        Point* a = new Point( xA, yA );

        head = a;
        head->next = a;
        head->previous = a;

        m_NumberOfPoints = 1;

		if( parent != NULL )
        {
            parent->addChild( this );
        }
    }

    Polygon::Polygon( Polygon *p_Parent )
    {
        parent = p_Parent;

        head = NULL;

        m_NumberOfPoints = 0;

		if( parent != NULL )
        {
            parent->addChild( this );
        }
    }

	Polygon::Polygon( )
	{
		parent = NULL;
		head = NULL;
		m_NumberOfPoints = 0;
	}

    //--------------------------------------------------------------------------------------

    Polygon::~Polygon( )
    {
		Point* deleteMe;

		while( m_NumberOfPoints != 0 )
		{
			deleteMe = head->previous;
			head->previous = head->previous->previous;

			delete deleteMe;

			m_NumberOfPoints--;
		}
    }

    //--------------------------------------------------------------------------------------

    bool Polygon::addChild( Polygon *child )
    {
        if( child == NULL )
            return false;

        children.push_back( child );

        return true;
    }

	bool Polygon::removeChild( unsigned pos )
	{
		if( pos > children.size( ) )
			return false;

		delete children[ pos ];
		children.erase( children.begin( ) + pos );
	
		return true;
	}

    //--------------------------------------------------------------------------------------

    bool Polygon::addPoint( float x, float y )
    {
		if( head == NULL )
		{
			head = new Point( x, y );
			head->next = head;
			head->previous = head;

			m_NumberOfPoints++;

			return true;
		}

        Point* find = head;

        // Cycle through all points to check if point already exists in the Polygon
        do
        {
            find = find->next;

            if( find == head ) break;
        } while( find->x != x && find->y != y );

        // If point is not already in the Polygon
        if( ( find->x != x ) || ( find->y != y ) )
        {
            find = new Point( x, y );

            find->next = head;
            find->previous = head->previous;
            find->previous->next = find;
            head->previous = find;

            m_NumberOfPoints++;

            return true;
        }

        return false;
    }

    //--------------------------------------------------------------------------------------

    bool Polygon::removePoint( float x, float y )
    {
        if( m_NumberOfPoints == 0 )
            return false;

        Point* find = head;

        do
        {
            find = find->next;

            if( find == head ) break;
        } while( find->x != x && find->y != y );

        if( find->x == x && find->y == y )
        {
            find->previous->next = find->next;
            find->next->previous = find->previous;

            if( head == find ) head = find->next;

            delete( find );

            m_NumberOfPoints--;

			if( m_NumberOfPoints == 0 )
				head = NULL;

            return true;
        }

        return false;
    }

    //--------------------------------------------------------------------------------------

	bool Polygon::removePoint( unsigned pos )
    {
        if( pos > m_NumberOfPoints || m_NumberOfPoints == 0 )
            return false;

        Point* find = head;

        for( int i = 0; i < pos; i++ )
			find = find->next;

        find->previous->next = find->next;
        find->next->previous = find->previous;

        if( head == find ) head = find->next;

        delete( find );

        m_NumberOfPoints--;

		if( m_NumberOfPoints == 0 )
			head = NULL;

        return true;
    }

	bool Polygon::removePoint( float aX, float aY, float bX, float bY, float cX, float cY )
	{
		if( m_NumberOfPoints == 0 )
            return false;

        Point* find = head;

        do
        {
			if( find->x == aX           && find->y == aY &&
				find->next->x == bX     && find->next->y == bY &&
				find->previous->x == cX && find->previous->y == cY )
				 break;

            find = find->next;
        } while( find != head );

        if( find->x == aX && find->y == aY )
        {
            find->previous->next = find->next;
            find->next->previous = find->previous;

            if( head == find ) head = find->next;

            delete( find );

            m_NumberOfPoints--;

			if( m_NumberOfPoints == 0 )
				head = NULL;

            return true;
        }

		return false;
	}

    //--------------------------------------------------------------------------------------

	Point* Polygon::getPoint( float x, float y )
	{
		Point* find = head;

		do
		{
			if( find->x == x && find->y == y )
				return find;

			find = find->next;
		} while( find != head );

		return NULL;
	}

	//--------------------------------------------------------------------------------------

	Point* Polygon::getPoint( unsigned pos )
	{
		Point* find = head;

		if( pos >= m_NumberOfPoints )
			return NULL;

		for( int i = 0; i != pos; i++ )
			find = find->next;

		return find;
	}

	//--------------------------------------------------------------------------------------

	bool Polygon::insertPoint( float x, float y, Point* p_Point )
	{
		if( p_Point == NULL )
			return false;

		Point* find = head;

		// Cycle through all points to find the 'real' p_Point.
		// This is done since the p_Point provided may be a distant copy of
		// the point that we want to insert prior to, and not the original,
		// and hence not part of the original polygon.
		for( int i = 0; i < m_NumberOfPoints; i++ )
		{
			if( find->x == p_Point->x && find->y == p_Point->y )
				break;

			find = find->next;
		}

        // Make sure the point that we want to insert prior to exists
		if( find->x != p_Point->x && find->y != p_Point->y )
				return false;

		Point* newPoint = new Point( x, y );

		find->previous->next = newPoint;
		newPoint->previous = find->previous;

		find->previous = newPoint;
		newPoint->next = find;

		m_NumberOfPoints++;

		return true;
	}

	//--------------------------------------------------------------------------------------

	void Polygon::reverse( int pos )
	{
		if( pos >= 0 )
		{
			if( pos < children.size( ) )
				children[ pos ]->reverse( -1 );
		
			return;
		}

		// pos == -1; so reverse this polygon
		Point* active = head;
		Point* temp;

		do
		{
			temp = active->next;

			active->next = active->previous;
			active->previous = temp;

			active = temp;
		} while( active != head );
	}
};
