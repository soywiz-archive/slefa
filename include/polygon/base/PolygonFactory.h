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
 * \brief PolygonFactory is a base class for factories that create the actual polygon implementations.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */


#ifndef POLYGON_FACTORY_H_INCLUDED
#define POLYGON_FACTORY_H_INCLUDED

//! A base class for factories that create the actual polygon implementations.
class PolygonFactory
{
public:
    //! Notifies the factory that a construction of new vector object is starting.
    virtual void beginGraphic() {}

    //! Creates a polygon with the factory.
    virtual PolygonWrapper * createPolygonWrapper(const PolygonData *aData) = 0;

    //! Creates a paint with the factory from an RGB color.
    virtual PaintWrapper * createPaintWrapper(const unsigned int aColor)
    {
        return new DefaultPaintWrapper(aColor);
    }

    //! Notifies the factory that a construction of a vector object has ended.
    virtual void endGraphic() {}
};

#endif // !POLYGON_FACTORY_H_INCLUDED
