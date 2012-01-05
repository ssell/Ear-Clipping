# Triangulation via Ear Clipping
### by Steven Sell

## Overview 

Source code that correlates with the article located at:

> http://www.ywdac.com/articles/1

     i.   Library
     
          a. Structures
          
          b. Merger
          
          c. Triangulation
 
    ii.   Demo Application
    
          a. Controls
 
    iii.  Setup
 
    iv.   Contact


## Library

The files beginning with earClipping_* compose a stand-alone
library that can triangulate any simple polygon passed to it.
The only requirements are that the data points are provided
in the appropriate Polygon structure.

### Structures

There are two structures in the library: points and polygons.

Points are a minimal structure of X and Y points stored in
a float. Overloaded methods are implemented for arithmetic
operations and point comparison. Additionally methods to
perform dot and cross products are included which are crucial
to the merge and triangulation functions.

Finally, the points are implemented as a doubly-linked list.

The polygon structure is at its core a container for a head
point and the circular list that it creates. Helper functions
are provided to maintain the list including: add, remove,
number of points, etc. A polygon can also have children 
polygons which reperesent any holes that are present in the
original object.

An important note is that polygons should be created in a
counter-clockwise fashion. This is due to the triangulator
requiring that the points are in this order for the
calculations that it performs. There is a method located
in earClipping_Core.h that should be called in the event
that the points are not guarenteed to be provided in a
counter-clockwise order. This is the orientatePolygon
function and it puts the points in the correct order
for not only the parent but for all children as well.

### Merger

Any polygon that has holes must be passed into the
mergePolygon method. In brief the function orders the
children by whom has the smallest x-value, then one-by-one
finds the closest mutually-visible point located on the
outer polygon and creates a new line segment to connect
the two polygons and merge them together.

### Triangulation

recordEars is the triangulation function and it takes any
passed Polygon and discovers the ears that form it. These
ears are then recorded to the specified file using the 
format that follows:

    # of ears in file
    
    X1,Y1:X2,Y2:X3,Y3       // ear one
    
    ...

Example:

    3
    
    45,315:38,216:248,41
    
    45,315:248,41:339,186
    
    45,315:339,186:175,346
    

These files can either be read in by writing your own
method or the retrieveEars function can be used. The 
points are stored in X,Y order in a std::vector and 
every three pairs (or 6 total) comprise a single ear.

## Demo Application

A simple demo is included and is composed of the main.cpp
file and all files starting with gl_*. A simple 'renderer'
is employed, and the GLFW library is used for rapid-prototyping.

The following keybindings are used:

`

    Mouse 1     |   Adds a new point to active polygon
    
    Spacebar    |   If current polygon has minimum of three points
                    then a new polygon is created as a child of
                    the original
                    
    Backspace   |   Removes the newest point. If the point removed
                    is the final vertex of the active polygon then
                    the previous polygon becomes active
                    
    Enter       |   Changes the current mode
    
                    CREATION :      add/remove points
                    
                    MERGE    :      if children exist, merge them
                    
                    COMPLETE :      triangulates and displays ears
                    
`

## Setup

`

    EarClipping/

        src/
        projects/
        glfw-2.7.2/
        
`

GLFW is an external library used for input handling and OpenGL
process creation. It can be found at:

> http://www.glfw.org/

## Contact

> ssell@knights.ucf.edu

> http://www.ywdac.com
