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
