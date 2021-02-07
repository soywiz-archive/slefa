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


#ifndef AGG_POLYGON_WRAPPER_H_INCLUDED
#define AGG_POLYGON_WRAPPER_H_INCLUDED

//! A class for wrapping the AGG polygon implementation to the tests.
class AGGPolygonWrapper : public PolygonWrapper
{
public:
    //! Constructor.
    AGGPolygonWrapper();

    //! Virtual destructor.
    virtual ~AGGPolygonWrapper();

    //! Static method for creating a polygon from the data.
    static AGGPolygonWrapper * create(const PolygonData *aData,agg::path_storage *aPath);

    //! Getter for the native implementation.
    inline unsigned int getPathIndex()
    {
        return mPathIndex;
    }

protected:
    unsigned int mPathIndex;
};

#endif // !AGG_POLYGON_WRAPPER_H_INCLUDED
