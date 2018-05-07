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
 
#include "gl_PolygonRenderer.h"

#include <fstream>
#include <iostream>

//------------------------------------------------------------------------------------------

PolygonRenderer::PolygonRenderer( )
{
	m_ActivePoint = -1;
}

PolygonRenderer::~PolygonRenderer( )
{
}

//------------------------------------------------------------------------------------------

/// Adds a new point to the current active polygon
bool PolygonRenderer::addPoint( float x, float y )
{
	if( m_ActivePoint == -1 )
		return m_Polygon.addPoint( x, y );
	
	EarClipping::Polygon* hold = m_Polygon.getChild( m_ActivePoint );

	if( hold == NULL )
	{
		EarClipping::Polygon* p = new EarClipping::Polygon( &m_Polygon, x, y );
		return true;
	}
	else
		return hold->addPoint( x, y );
}

/// Removes last point from active polygon. If active is a child, and it has no more points, m_ActivePoint is decremented.
bool PolygonRenderer::popPoint( )
{
	if( m_ActivePoint == - 1 )
	{
		return m_Polygon.removePoint( m_Polygon.numPoints( ) - 1 );
	}

	EarClipping::Polygon* hold = m_Polygon.getChild( m_ActivePoint );

	if( hold == NULL )
	{
		m_ActivePoint--;
		return false;
	}
	else
	{
		if( hold->numPoints( ) <= 1 )
		{
			m_Polygon.removeChild( m_ActivePoint );
			m_ActivePoint--;
			return true;
		}
		else
			return hold->removePoint( hold->numPoints( ) );

	}
}

//------------------------------------------------------------------------------------------

void PolygonRenderer::newChild( )
{
	// Dont add a new polygon if active polygon is not a real polygon
	if( m_Polygon.numChildren( ) == 0 && m_Polygon.numPoints( ) >= 3 )
		m_ActivePoint++;
	else
	{  
		EarClipping::Polygon* hold = m_Polygon.getChild( m_ActivePoint );

		if( hold != NULL )
		{
			if( hold->numPoints( ) >= 3 )
				m_ActivePoint++;
		}
	}
}

//------------------------------------------------------------------------------------------

void PolygonRenderer::draw( )
{
	if( m_Polygon.numPoints( ) == 0 )
		return;

	if( m_Ears.size( ) == 0 )
	{
		drawMain( );

		if( m_Polygon.numChildren( ) > 0 )
			drawChildren( );
	}
	else
	{
		// Draw ears
		drawEars( );
	}
}

//------------------------------------------------------------------------------------------

void PolygonRenderer::drawMain( )
{
	GLubyte r, g, b;

	r = 100;
	b = 100;
	g = m_ActivePoint == -1 ? 140 : 100;

	EarClipping::Point* active = m_Polygon.get( );

	for( int i = 0; i <= m_Polygon.numPoints( ) ; i++ )
	{
		glBegin( GL_LINES );
			glColor3ub( r, g, b );
			glVertex2f( active->x, active->y );
			glVertex2f( active->next->x, active->next->y );
		glEnd( );

		active = active->next;
	}

	active = m_Polygon.get( );

	for( int i = 0; i <= m_Polygon.numPoints( ); i++ )
	{
		glBegin( GL_POINTS );
			glColor3ub( r, g, b );
			glVertex2f( active->x, active->y );
		glEnd( );

		active = active->next;
	}
}

void PolygonRenderer::drawChildren( )
{
	GLubyte r, g, b;

	r = 100;
	b = 100;
	g = 100;

	EarClipping::Point* active;

	for( int i = 0; i < m_Polygon.numChildren( ); i++ )
	{
		active = m_Polygon.getChild( i )->get( );

		if( i == m_ActivePoint )
			g = 140;

		for( int j = 0; j < m_Polygon.getChild( i )->numPoints( ); j++ )
		{
			glBegin( GL_LINES );
				glColor3ub( r, g, b );
				glVertex2f( active->x, active->y );
				glVertex2f( active->next->x, active->next->y );
			glEnd( );

			active = active->next;
		}

		active = m_Polygon.getChild( i )->get( );

		for( int j = 0; j < m_Polygon.getChild( i )->numPoints( ); j++ )
		{
			glBegin( GL_POINTS );
				glColor3ub( r, g, b );
				glVertex2f( active->x, active->y );
			glEnd( );

			active = active->next;
		}

		if( i == m_ActivePoint )
			g = 100;
	}
}

//------------------------------------------------------------------------------------------

void PolygonRenderer::drawEars( )
{
	for( int i = 0; i < m_Ears.size( ); i += 6 )
	{
		glBegin( GL_TRIANGLES );
			glColor4ub( 210, 240, 210, 50 );
			glVertex2f( m_Ears[ i ], m_Ears[ i + 1 ] );
			glVertex2f( m_Ears[ i + 2 ], m_Ears[ i + 3 ] );
			glVertex2f( m_Ears[ i + 4 ], m_Ears[ i + 5 ] );
		glEnd( );

		glBegin( GL_LINES );
			glColor3ub( 100, 100, 140 );

			glVertex2f( m_Ears[ i ], m_Ears[ i + 1 ] );
			glVertex2f( m_Ears[ i + 2 ], m_Ears[ i + 3 ] );

			glVertex2f( m_Ears[ i + 2 ], m_Ears[ i + 3 ] );
			glVertex2f( m_Ears[ i + 4 ], m_Ears[ i + 5 ] );

			glVertex2f( m_Ears[ i + 4 ], m_Ears[ i + 5 ] );
			glVertex2f( m_Ears[ i ], m_Ears[ i + 1 ] );

		glEnd( );
	}
}

//------------------------------------------------------------------------------------------

bool PolygonRenderer::performMerge( )
{
	EarClipping::orientatePolygon( &m_Polygon );

	// Polygon has children
	if( m_ActivePoint != -1 )
	{
		if( m_Polygon.getChild( m_Polygon.numChildren( ) - 1 )->numPoints( ) < 3 )
		{
			std::cout << "Child #" << m_Polygon.numChildren( ) - 1 << " does not have 3 or more vertices!" << std::endl;
			return true;
		}

		EarClipping::mergePolygon( m_Polygon );
		
		while( m_Polygon.numChildren( ) != 0 )
		{
			m_Polygon.removeChild( m_Polygon.numChildren( ) - 1 );
		}

		m_ActivePoint = -1;

		return true;
	}

	return false;
}

//------------------------------------------------------------------------------------------

bool PolygonRenderer::triangulate( )
{
	if( m_Polygon.numPoints( ) < 3 )
	{
		std::cout << "Polygon does not have three or more vertices!" << std::endl;
		return false;
	}

	if( !EarClipping::recordEars( m_Polygon, "ears.txt" ) )
	{
		std::cout << "Ear Clipping failed!" << std::endl;
		return false;
	}

	m_Ears = EarClipping::retrieveEars( "ears.txt" );

	return true;
}

//------------------------------------------------------------------------------------------

void PolygonRenderer::reset( )
{
	EarClipping::Polygon newPolygon;
	m_Polygon = newPolygon;

	m_Ears.clear( );

	m_ActivePoint = -1;
}