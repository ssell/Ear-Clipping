/**
 * \file earClipping_Core.h
 * \author ssell
 * \brief Header file for the two main EarClipping functions: merge and recordEars.
 */
#ifndef __EAR_CLIPPING__CORE_H__
#define __EAR_CLIPPING__CORE_H__

//------------------------------------------------------------------------------------------

#include <utility>

#include "earClipping_Structures.h"

//------------------------------------------------------------------------------------------

/*!
 *  \addtogroup EarClipping
 *  @{
 */
namespace EarClipping
{
    typedef Point Vector;

    //--------------------------------------------------------------------------------------
    // Polygon Mergers (Outer with child inner)
    // source: earClipping_Merge.cpp

	/// Ensures all polygons are in counter-clockwise order
	void orientatePolygon( Polygon* poly );

    /**
        When mergePolygon is called on a Polygon, the children are added to the parent.

            1. Order inner polygons by farthest (leftward) x-values
            2. Take first Polygon and order it's points (this employs pointCompare( ))
            3. Find the closest, mutually visible points of the exterior and interior polygons
            4. Ensure the diagonal created by these two points does not intersect any other lines
            5. Add the points of the child to that of the parent

                ex:

                Exterior has 10 points, Interior has 5.

                Ext[3] and Int[4] are closest mutually visible and pass intersect test.

                Add Int points to Ext:

                    Original Ext:

                        E0 - E1 - E2 - E3 - E4 - E5 - E6 - E7 - E8 - E9

                    New Ext:

                        E0 - E1 - E2 - E3 - I4 - I3 - I2 - I1 - I0 - I4 - E5 - E6 - E7 - E8 - E9

                    note that the Exterior points remain ccw, while the added interior are cw.

            6. Remove child Polygon from container
            7. If there exists another child, repeat from step [2]
    **/
    void mergePolygon( Polygon &poly );

    //--------------------------------------------------------------------------------------
    // Polygon Triangulation

	/// Triangulates the polygon and records the Ears in the specified path. Returns false on any critical errors.
	bool recordEars( Polygon poly, const char* path );

	//--------------------------------------------------------------------------------------

	std::vector< float > retrieveEars( char* path );
}
/*! @} End of Doxygen Groups*/

//------------------------------------------------------------------------------------------

#endif // __EAR_CLIPPING__CORE_H__
