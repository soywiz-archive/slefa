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
 * \brief GDIplusPolygonFactory is a class for creating the GDI+ polygon implementations.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */


#ifndef GDI_PLUS_POLYGON_FACTORY_H_INCLUDED
#define GDI_PLUS_POLYGON_FACTORY_H_INCLUDED

//! A class for creating the GDI+ polygon implementations.
class GDIplusPolygonFactory : public PolygonFactory
{
    //! Creates a polygon with the factory.
    virtual PolygonWrapper * createPolygonWrapper(const PolygonData *aData)
    {
        return GDIplusPolygonWrapper::create(aData);
    }
};

#endif // !GDI_PLUS_POLYGON_FACTORY_H_INCLUDED
