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
 * \brief AGGPolygonWrapper is a class for wrapping the AGG polygon implementation to the tests.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */


#include "polygon/base/Base.h"
#include "polygon/AGG/AGG.h"


//! Constructor.
AGGPolygonWrapper::AGGPolygonWrapper()
{
    mPathIndex = 0;
}


//! Virtual destructor.
AGGPolygonWrapper::~AGGPolygonWrapper()
{
}


//! Static method for creating a polygon from the data.
AGGPolygonWrapper * AGGPolygonWrapper::create(const PolygonData *aData,agg::path_storage *aPath)
{
    AGGPolygonWrapper *wrapper = new AGGPolygonWrapper();

    if (wrapper == NULL)
        return NULL;

    int count = aData->getSubPolygonCount();

    wrapper->mPathIndex = aPath->start_new_path();

    int n;
    for (n = 0; n < count; n++)
    {
        int vertexCount = aData->getVertexCount(n);
        const RATIONAL *vertexData = aData->getVertexData(n);
        aPath->move_to(vertexData[0], vertexData[1]);
        int p;
        for (p = 0; p < vertexCount; p++)
        {
            aPath->line_to(vertexData[p*2], vertexData[p*2+1]);
        }
        aPath->close_polygon();
    }

    // aPath.arrange_orientations_all_paths(agg::path_flags_cw);

    return wrapper;
}
