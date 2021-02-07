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
 * \brief GDIplusPolygonWrapper is a class for wrapping the GDI+ polygon implementation to the tests.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */


#include "polygon/base/Base.h"
#include "polygon/GDIplus/GDIplus.h"


//! Constructor.
GDIplusPolygonWrapper::GDIplusPolygonWrapper(GraphicsPath *aPath)
{
    mPath = aPath;
}


//! Virtual destructor.
GDIplusPolygonWrapper::~GDIplusPolygonWrapper()
{
    delete mPath;
}


//! Static method for creating a polygon from the data.
GDIplusPolygonWrapper * GDIplusPolygonWrapper::create(const PolygonData *aData)
{
    int subPolyCount = aData->getSubPolygonCount();
    int vertexCount = 0;
    int n,p;

    for (n = 0; n < subPolyCount; n++)
    {
        vertexCount += aData->getVertexCount(n);
    }

    PointF *points = new PointF[vertexCount];

    if (points == NULL)
        return NULL;

    BYTE *types = new BYTE[vertexCount];
    if (types == NULL)
    {
        delete[] points;
        return NULL;
    }

    int v = 0;
    for (n = 0; n < subPolyCount; n++)
    {
        const RATIONAL *data = aData->getVertexData(n);
        int count = aData->getVertexCount(n);
        for (p = 0; p < count; p++)
        {
            points[v] = PointF(data[p*2],data[p*2+1]);
            if (p == 0)
                types[v] = PathPointTypeStart;
            else if (p == vertexCount - 1)
                types[v] = PathPointTypeLine | PathPointTypeCloseSubpath;
            else
                types[v] = PathPointTypeLine;
            v++;
        }
    }

    GraphicsPath * path = new GraphicsPath(points,types,vertexCount,FillModeAlternate);

    delete[] points;
    delete[] types;

    if (path == NULL)
        return NULL;

    GDIplusPolygonWrapper * result = new GDIplusPolygonWrapper(path);

    if (result == NULL)
    {
        delete path;
        return NULL;
    }

    return result;
}
