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
 * \brief PolygonWrapper is a class for wrapping the actual polygon implementation to the tests.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */


#ifndef POLYGON_WRAPPER_H_INCLUDED
#define POLYGON_WRAPPER_H_INCLUDED

//! A class for wrapping the actual polygon implementation to the tests.
class PolygonWrapper
{
public:
    //! Virtual destructor.
    virtual ~PolygonWrapper() {};
};

#endif // !POLYGON_WRAPPER_H_INCLUDED
