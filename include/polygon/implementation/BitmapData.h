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
 * \brief A class for passing bitmap data to the polygon filler.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */


#ifndef BITMAP_DATA_H_INCLUDED
#define BITMAP_DATA_H_INCLUDED

//! A class for passing bitmap data to the polygon filler.
class BitmapData : public RenderTargetWrapper
{
public:
    enum BitmapFormat
    {
        BITMAP_FORMAT_XRGB = 0,
        BITMAP_FORMAT_ARGB,
        BITMAP_FORMAT_XBGR,
        BITMAP_FORMAT_ABGR,
        BITMAP_FORMAT_RGBX,
        BITMAP_FORMAT_RGBA,
        BITMAP_FORMAT_BGRX,
        BITMAP_FORMAT_BGRA
    };

    //! Constructor.
    /*! /param aWidth the width of the bitmap in pixels.
     *  /param aHeight the height of the bitmap in pixels.
     *  /param aPitch the lenght of one scanline in bytes.
     *  /param aData a pointer to the data.
     *  /param aFormat the format of the bitmap.
     */
    BitmapData(unsigned int aWidth, unsigned int aHeight,
               unsigned int aPitch, unsigned long *aData,
               BitmapFormat aFormat) :
               mWidth(aWidth), mHeight(aHeight), mPitch(aPitch), mData(aData),
               mFormat(aFormat) {}
    
    unsigned int mWidth;
    unsigned int mHeight;
    unsigned int mPitch;
    unsigned long *mData;
    BitmapFormat mFormat;
};

#endif // !BITMAP_DATA_H_INCLUDED
