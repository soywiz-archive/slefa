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


#ifndef GDI_PLUS_POLYGON_WRAPPER_H_INCLUDED
#define GDI_PLUS_POLYGON_WRAPPER_H_INCLUDED

//! A class for wrapping the GDI+ polygon implementation to the tests.
class GDIplusPolygonWrapper : public PolygonWrapper
{
public:
    //! Constructor.
    GDIplusPolygonWrapper(GraphicsPath *aPath);

    //! Virtual destructor.
    virtual ~GDIplusPolygonWrapper();

    //! Static method for creating a polygon from the data.
    static GDIplusPolygonWrapper * create(const PolygonData *aData);

    //! Getter for the native implementation.
    inline GraphicsPath * getPath()
    {
        return mPath;
    }

protected:
    GraphicsPath *mPath;
};

#endif // !GDI_PLUS_POLYGON_WRAPPER_H_INCLUDED
