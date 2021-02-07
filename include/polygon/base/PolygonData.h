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
 * \brief A class for holding raw vertex data to be used at the construction of a polygon.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */

#ifndef POLYGON_DATA_H_INCLUDED
#define POLYGON_DATA_H_INCLUDED

//! A class for holding raw vertex data to be used at the construction of a polygon.
class PolygonData
{
public:
    //! Constructor.
    PolygonData(const RATIONAL * const *aVertexData, const int *aVertexCounts, int aSubPolygonCount) :
      mVertexData(aVertexData), mVertexCounts(aVertexCounts), mSubPolygonCount(aSubPolygonCount) { }

    //! Returns the count of subpolygons
    inline int getSubPolygonCount() const
    {
        return mSubPolygonCount;
    }

    //! Returns a pointer to the vertex data array of a sub-polygon at given index.
    inline const RATIONAL * getVertexData(int aIndex) const
    {
        return mVertexData[aIndex];
    }

    //! Returns the vertex count of a sub-polygon at given index.
    inline int getVertexCount(int aIndex) const
    {
        return mVertexCounts[aIndex];
    }

protected:
    const RATIONAL * const *mVertexData;
    const int *mVertexCounts;
    int mSubPolygonCount;
};

#endif // !POLYGON_DATA_H_INCLUDED
