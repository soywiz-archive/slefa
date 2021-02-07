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
 * \brief AGGPolygonFactory is a class for creating the AGG polygon implementations.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */


#ifndef AGG_POLYGON_FACTORY_H_INCLUDED
#define AGG_POLYGON_FACTORY_H_INCLUDED

//! A class for creating the AGG polygon implementations.
class AGGPolygonFactory : public PolygonFactory
{
public:
    //! Constructor.
    /*! aPathStorage is used as the storage for the constructed paths.
     */ 
    AGGPolygonFactory(agg::path_storage *aPathStorage)
    {
        mPathStorage = aPathStorage;
    }

    //! Creates a polygon with the factory.
    virtual PolygonWrapper * createPolygonWrapper(const PolygonData *aData)
    {
        return AGGPolygonWrapper::create(aData,mPathStorage);
    }

protected:
    agg::path_storage *mPathStorage;
};

#endif // !AGG_POLYGON_FACTORY_H_INCLUDED
