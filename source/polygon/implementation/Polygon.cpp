/*!
 * \file
 * Scanline edge-flag algorithm for antialiasing <br>
 * Copyright (c) 2005-2007 Kiia Kallio
 *
 * http://mlab.uiah.fi/~kkallio/antialiasing/
 * 
 * This code is distributed under the three-clause BSD license.
 * Read the LICENSE file or visit the URL above for details.
 *
 * \brief A class for handling polygons. Each polygon is a set of sub-polygons.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */

#include "polygon/base/Base.h"
#include "polygon/implementation/Implementation.h"

//! Constructor.
/*! Note that the ownership of the sub-polygon array is transferred.
 */
Polygon::Polygon(SubPolygon **aSubPolygons, int aSubPolygonCount)
{
    mSubPolygons = aSubPolygons;
    mSubPolygonCount = aSubPolygonCount;
}


//! Virtual destructor.
Polygon::~Polygon()
{
    int n;
    for (n = 0; n < mSubPolygonCount; n++)
        delete mSubPolygons[n];
    delete[] mSubPolygons;
}


//! Static method for creating a polygon from the data.
Polygon * Polygon::create(const PolygonData *aPolygonData)
{
    bool error = false;

    int subPolygonCount = aPolygonData->getSubPolygonCount();

    Polygon *polygon = NULL;
    SubPolygon **subPolygons = new SubPolygon*[subPolygonCount];

    if (subPolygons == NULL)
        error = true;

    if (!error)
    {
        int n;
        for (n = 0; n < subPolygonCount; n++)
            subPolygons[n] = NULL;

        n = 0;
        while (!error && n < subPolygonCount)
        {
            int vertexCount = aPolygonData->getVertexCount(n);

            VECTOR2D *vertices = new VECTOR2D[vertexCount];
            if (vertices == NULL)
                error = true;

            if (!error)
            {
                int p;
                const RATIONAL *vertexData = aPolygonData->getVertexData(n);
                for (p = 0; p < vertexCount; p++)
                {
                    VECTOR2D_SETX(vertices[p],vertexData[p * 2]);
                    VECTOR2D_SETY(vertices[p],vertexData[p * 2 + 1]);
                }

                subPolygons[n] = new SubPolygon(vertices,vertexCount);
                if (subPolygons[n] == NULL || !subPolygons[n]->init())
                {
                    delete[] vertices;
                    error = true;
                }
            }

            n++;
        }
    }

    if (!error)
    {
        polygon = new Polygon(subPolygons,subPolygonCount);
        if (polygon == NULL)
            error = true;
    }

    if (error)
    {
        int n;
        for (n = 0; n < subPolygonCount; n++)
            delete subPolygons[n];
        delete[] subPolygons;
        return NULL;
    }

    return polygon;
}


