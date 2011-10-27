/**
 * \file gl_PolygonRenderer.h
 * \author ssell
 */

#ifndef __GL_POLYGON_RENDERER_H__
#define __GL_POLYGON_RENDERER_H__

//------------------------------------------------------------------------------------------

#include <GL/glfw.h>
#include "earClipping_Core.h"

//------------------------------------------------------------------------------------------

class PolygonRenderer
{
public:

	PolygonRenderer( );
	~PolygonRenderer( );

	bool addPoint( float x, float y );
	bool popPoint( );

	void newChild( );

	void draw( );

	bool performMerge( );
	bool triangulate( );

	void reset( );

protected:

	void drawMain( );
	void drawChildren( );
	void drawEars( );

private:

	int m_ActivePoint; // -1 = main, 0 - n = child
	
	std::vector< float > m_Ears;

	EarClipping::Polygon m_Polygon;
};

//------------------------------------------------------------------------------------------

#endif // __GL_POLYGON_RENDERER_H__