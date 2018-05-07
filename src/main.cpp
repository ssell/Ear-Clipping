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

#include <GL/glfw.h>
#include <iostream>

#include "earClipping_Core.h"
#include "gl_PolygonRenderer.h"

using namespace std;

//------------------------------------------------------------------------------------------

#define CONSTRUCTING 0
#define MERGING 1
#define COMPLETE 2

PolygonRenderer* pRend = new PolygonRenderer( );

unsigned status = CONSTRUCTING;

//------------------------------------------------------------------------------------------

void drawGrid( )
{
    for( float i = 0; i < 40; i++ )
    {
        glBegin( GL_LINES );
            glColor3f( 0.9f, 0.9f, 0.9f ); glVertex2f( ( i * 10 ), 0.f ); glVertex2f( ( i * 10 ), 400.f );
            glColor3f( 0.9f, 0.9f, 0.9f ); glVertex2f( 0.f, ( i * 10 ) ); glVertex2f( 400.f, ( i * 10 ) );
        glEnd( );
    }

    glBegin( GL_LINES );
        glColor3f( 0.2f, 0.2f, 0.2f ); glVertex2f( 0.f, 0.f ); glVertex2f( 0.f, 400.f );
        glColor3f( 0.2f, 0.2f, 0.2f ); glVertex2f( 0.f, 0.f ); glVertex2f( 400.f, 0.f );
    glEnd( );
}

//------------------------------------------------------------------------------------------

void initGL( )
{
    glViewport( 0, 0, 400, 400 );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0, 400, 0, 400, 0.f, 100.f );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glPointSize( 5 );
}

//------------------------------------------------------------------------------------------

void GLFWCALL handleKey( int key, int action )
{
    if( key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS && status == CONSTRUCTING )
    {
        pRend->popPoint( );
    }
	else if( key == GLFW_KEY_SPACE && action == GLFW_PRESS && status == CONSTRUCTING )
	{
		pRend->newChild( );
	}
	else if( key == GLFW_KEY_ENTER && action == GLFW_PRESS )
	{
		bool worked;

		if( status == CONSTRUCTING  )
		{
			worked = pRend->performMerge( );

			if( worked == true )
				status = MERGING;
			else // no children to merge
			{
				if( !pRend->triangulate( ) )
					std::cout << "Failed to triangulate!";
				else
					status = COMPLETE;
			}
		}
		else if( status == MERGING )
		{
			if( !pRend->triangulate( ) )
				std::cout << "Failed to triangulate!";
			else
				status = COMPLETE;
		}
		else
		{
			pRend->reset( );
			status = CONSTRUCTING;
		}
	}
}

//------------------------------------------------------------------------------------------

void GLFWCALL handleClick( int button, int action )
{
    if( button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && status == CONSTRUCTING )
    {
        int x, y;

        glfwGetMousePos( &x, &y );

		y = ( y - 400 ) * -1;

        std::cout << "clicked " << x << "," << y << std::endl;

		pRend->addPoint( x, y );
    }
}

//------------------------------------------------------------------------------------------

int main()
{
	bool    running = true;

    //--------------------------------------------------------------------------------------
    // Set up glfw ...

    glfwInit();

    if( !glfwOpenWindow( 400, 400, 0, 0, 0, 0, 0, 0, GLFW_WINDOW ) )
    {
        glfwTerminate( );
        return 0;
    }

    glfwSetWindowTitle("Ear Clipping");

    //Register callbacks
    glfwSetKeyCallback( handleKey );
    glfwSetMouseButtonCallback( handleClick );

    //--------------------------------------------------------------------------------------

    initGL( );

    while( running )
    {
		glClearColor( 0.98f, 0.98f, 0.98f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT );

		drawGrid( );

		pRend->draw( );

		glfwSwapBuffers( );

        // exit if ESC was pressed or window was closed
        running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
	}

    glfwTerminate( );

    return 0;
}
