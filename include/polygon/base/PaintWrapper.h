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
 * \brief PaintWrapper is a class for wrapping the actual paint implementation to the tests.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */


#ifndef PAINT_WRAPPER_H_INCLUDED
#define PAINT_WRAPPER_H_INCLUDED

//! A class for wrapping the actual paint implementation to the tests.
class PaintWrapper
{
public:
    //! Virtual destructor.
    virtual ~PaintWrapper() {};
};

#endif // !PAINT_WRAPPER_H_INCLUDED
