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

#ifndef POLYGON_H_INCLUDED
#define POLYGON_H_INCLUDED

class SubPolygon;
//! A class for handling polygons. Each polygon is a set of sub-polygons.
class Polygon : public PolygonWrapper
{
public:
    //! Constructor.
    /*! Note that the ownership of the sub-polygon array is transferred.
     */
    Polygon(SubPolygon **aSubPolygons, int aSubPolygonCount);

    //! Virtual destructor.
    virtual ~Polygon();

    //! Returns the amount of sub-polygons.
    inline int getSubPolygonCount() const
    {
        return mSubPolygonCount;
    }

    //! Returns a sub-polygon at given index.
    inline const SubPolygon & getSubPolygon(int aIndex) const
    {
        return *mSubPolygons[aIndex];
    }

    //! Static method for creating a polygon from the data.
    static Polygon * create(const PolygonData *aPolygonData);

protected:
    int mSubPolygonCount;
    SubPolygon **mSubPolygons;
};

#endif // !POLYGON_H_INCLUDED
