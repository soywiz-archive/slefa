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
 * \brief Default paint wrapper implements a wrapper that contains just the paint colour as unsigned int.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */


#ifndef DEFAULT_PAINT_WRAPPER_H_INCLUDED
#define DEFAULT_PAINT_WRAPPER_H_INCLUDED

//! Implements a PaintWrapper that contains just the paint colour as unsigned int.
class DefaultPaintWrapper: public PaintWrapper
{
public:
    //! Constructor.
    DefaultPaintWrapper(unsigned int aColor)
    {
        mColor = aColor;
    }

    //! Virtual destructor.
    virtual ~DefaultPaintWrapper() {}

    //! Inline getter for the color.
    inline unsigned int getColor()
    {
        return mColor;
    }

protected:
    unsigned int mColor;
};

#endif // !PAINT_WRAPPER_H_INCLUDED
