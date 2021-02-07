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
 * \brief PolygonFiller implementation version B. Uses fixed point math in the edge calculations.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */

#include "polygon/base/Base.h"
#include "polygon/implementation/Implementation.h"

//! Constructor.
PolygonVersionB::PolygonVersionB()
{
    mMaskBuffer = NULL;
    mWindingBuffer = NULL;
    // Scale the coordinates by SUBPIXEL_COUNT in vertical direction
    VECTOR2D scale(INT_TO_RATIONAL(1),INT_TO_RATIONAL(SUBPIXEL_COUNT));
    MATRIX2D_MAKESCALING(mRemappingMatrix,scale);
    // The sampling point for the sub-pixel is at the top right corner. This
    // adjustment moves it to the pixel center.
    VECTOR2D translation(FLOAT_TO_RATIONAL(0.5f/SUBPIXEL_COUNT),FLOAT_TO_RATIONAL(-0.5));
    MATRIX2D_TRANSLATE(mRemappingMatrix,translation);
}


//! Initializer.
/*! aWidth and aHeight define the maximum output size for the filler.
 *  The filler will output to larger bitmaps as well, but the output will
 *  be cropped.
 */
bool PolygonVersionB::init(unsigned int aWidth, unsigned int aHeight)
{
    // The buffer used for filling needs to be one pixel wider than the bitmap.
    // This is because the end flag that turns the fill off is the first pixel
    // after the actually drawn edge.
    unsigned int bufferWidth = aWidth + 1;

    mMaskBuffer = new SUBPIXEL_DATA[bufferWidth * aHeight];
    if (mMaskBuffer == NULL)
        return false;

    mWindingBuffer = new NON_ZERO_MASK_DATA_UNIT[bufferWidth * aHeight * SUBPIXEL_COUNT];
    if (mWindingBuffer == NULL)
        return false;

    mWidth = aWidth;
    mBufferWidth = bufferWidth;
    mHeight = aHeight;

    mClipRect.setClip(0,0,mWidth,mHeight,SUBPIXEL_COUNT);

    return true;
}


//! Virtual destructor.
PolygonVersionB::~PolygonVersionB()
{
    delete[] mMaskBuffer;
    delete[] mWindingBuffer;
}


//! Renders the polygon with even-odd fill.
/*! \param aTarget the target bitmap.
 *  \param aPolygon the polygon to render.
 *  \param aColor the color to be used for rendering.
 *  \param aTransformation the transformation matrix.
 */
void PolygonVersionB::renderEvenOdd(BitmapData *aTarget, const Polygon *aPolygon, unsigned long aColor, const MATRIX2D &aTransformation)
{
    if (!isSupported(aTarget->mFormat))
        return;

    // Sets the round down mode in case it has been modified.
    setRoundDownMode();

    MEMSET(mMaskBuffer,0,mBufferWidth*mHeight*sizeof(SUBPIXEL_DATA));

    MATRIX2D transform = aTransformation;
    MATRIX2D_MULTIPLY(transform,mRemappingMatrix);

    ClipRectangle clipRect;
    clipRect.setClip(0,0,aTarget->mWidth,aTarget->mHeight,SUBPIXEL_COUNT);
    clipRect.intersect(mClipRect);

    int subPolyCount = aPolygon->getSubPolygonCount();
    int n;
    for (n = 0; n < subPolyCount; n++)
    {
        const SubPolygon &poly = aPolygon->getSubPolygon(n);
        int count = poly.getVertexCount();
        int p = 0;
        while (p < count)
        {
            PolygonEdge edges[20];
            int edgeCount = poly.getEdges(p,10,edges,transform,clipRect);
            int k;
            for (k = 0; k < edgeCount; k++)
                addEvenOddEdge(edges[k]);
            p += 10;
        }
    }

    fillEvenOdd(aTarget,aColor,clipRect);
}


//! Renders the polygon with non-zero winding fill.
/*! \param aTarget the target bitmap.
 *  \param aPolygon the polygon to render.
 *  \param aColor the color to be used for rendering.
 *  \param aTransformation the transformation matrix.
 */
void PolygonVersionB::renderNonZeroWinding(BitmapData *aTarget, const Polygon *aPolygon, unsigned long aColor, const MATRIX2D &aTransformation)
{
    if (!isSupported(aTarget->mFormat))
        return;

    // Sets the round down mode in case it has been modified.
    setRoundDownMode();

    MEMSET(mMaskBuffer,0,mBufferWidth*mHeight*sizeof(SUBPIXEL_DATA));
    MEMSET(mWindingBuffer,0,mBufferWidth*mHeight*SUBPIXEL_COUNT*sizeof(NON_ZERO_MASK_DATA_UNIT));

    MATRIX2D transform = aTransformation;
    MATRIX2D_MULTIPLY(transform,mRemappingMatrix);

    ClipRectangle clipRect;
    clipRect.setClip(0,0,aTarget->mWidth,aTarget->mHeight,SUBPIXEL_COUNT);
    clipRect.intersect(mClipRect);

    int subPolyCount = aPolygon->getSubPolygonCount();
    int n;
    for (n = 0; n < subPolyCount; n++)
    {
        const SubPolygon &poly = aPolygon->getSubPolygon(n);
        int count = poly.getVertexCount();
        int p = 0;
        while (p < count)
        {
            PolygonEdge edges[20];
            int edgeCount = poly.getEdges(p,10,edges,transform,clipRect);
            int k;
            for (k = 0; k < edgeCount; k++)
                addNonZeroEdge(edges[k]);
            p += 10;
        }
    }

    fillNonZero(aTarget,aColor,clipRect);
}


//! Sets the clip rectangle for the polygon filler.
void PolygonVersionB::setClipRect(unsigned int aX, unsigned int aY, unsigned int aWidth, unsigned int aHeight)
{
    unsigned int x = aX;
    unsigned int y = aY;

    if (x > mWidth)
        x = mWidth;

    if (y > mHeight)
        y = mHeight;

    unsigned int maxX = x + aWidth;
    unsigned int maxY = y + aHeight;

    if (maxX > mWidth)
        maxX = mWidth;

    if (maxY > mHeight)
        maxY = mHeight;

    mClipRect.setClip(x, y, maxX - x, maxY - y, SUBPIXEL_COUNT);
}


//! Adds an edge to be used with even-odd fill.
void PolygonVersionB::addEvenOddEdge(const PolygonEdge &aEdge)
{
    static const FIXED_POINT offsets[SUBPIXEL_COUNT] = SUBPIXEL_OFFSETS_FIXED;

    int y;
    FIXED_POINT x = rationalToFixed(aEdge.mX);
    FIXED_POINT slope = rationalToFixed(aEdge.mSlope);
    FIXED_POINT slopeFix = INT_TO_FIXED(0);

    if (aEdge.mLastLine - aEdge.mFirstLine >= SLOPE_FIX_STEP)
    {
        slopeFix = rationalToFixed(aEdge.mSlope * SLOPE_FIX_STEP) -
                   (slope << SLOPE_FIX_SHIFT);
    }

    for (y = aEdge.mFirstLine; y <= aEdge.mLastLine; y++)
    {
        int ySub = y & (SUBPIXEL_COUNT - 1);
        int xp = FIXED_TO_INT(x + offsets[ySub]);
        SUBPIXEL_DATA mask = 1 << ySub;
        int yLine = y >> SUBPIXEL_SHIFT;
        mMaskBuffer[yLine * mBufferWidth + xp] ^= mask;
        x += slope;
        if ((y & SLOPE_FIX_MASK) == 0)
             x += slopeFix;
    }
}


//! Renders the mask to the canvas with even-odd fill.
void PolygonVersionB::fillEvenOdd(BitmapData *aTarget, unsigned long aColor, const ClipRectangle &aClipRect)
{
    unsigned int x,y;

    unsigned int minX = aClipRect.getMinXi();
    unsigned int maxX = aClipRect.getMaxXi();

    unsigned int minY = aClipRect.getMinYi() >> SUBPIXEL_SHIFT;
    unsigned int maxY = aClipRect.getMaxYi() >> SUBPIXEL_SHIFT;

    unsigned long cs1 = aColor & 0xff00ff;
    unsigned long cs2 = (aColor >> 8) & 0xff00ff;

    unsigned long *target = aTarget->mData;
    unsigned int pitch = aTarget->mPitch / 4;

    for (y = minY; y < maxY; y++)
    {
        unsigned long *tp = &target[y * pitch + minX];

        SUBPIXEL_DATA mask = 0;
        for (x = minX; x <= maxX; x++)
        {
            mask ^= mMaskBuffer[y * mBufferWidth + x];
            unsigned long alpha = SUBPIXEL_COVERAGE(mask);

            // alpha is in range of 0 to SUBPIXEL_COUNT
            unsigned long invAlpha = SUBPIXEL_COUNT - alpha;

            unsigned long ct1 = (*tp & 0xff00ff) * invAlpha;
            unsigned long ct2 = ((*tp >> 8) & 0xff00ff) * invAlpha;

            ct1 = ((ct1 + cs1 * alpha) >> SUBPIXEL_SHIFT) & 0xff00ff;
            ct2 = ((ct2 + cs2 * alpha) << (8 - SUBPIXEL_SHIFT)) & 0xff00ff00;

            *tp++ = ct1 + ct2;
        }
    }
}


//! Adds an edge to be used with non-zero winding fill.
void PolygonVersionB::addNonZeroEdge(const PolygonEdge &aEdge)
{
    static const FIXED_POINT offsets[SUBPIXEL_COUNT] = SUBPIXEL_OFFSETS_FIXED;

    int y;
    FIXED_POINT x = rationalToFixed(aEdge.mX);
    FIXED_POINT slope = rationalToFixed(aEdge.mSlope);
    FIXED_POINT slopeFix = INT_TO_FIXED(0);

    if (aEdge.mLastLine - aEdge.mFirstLine >= SLOPE_FIX_STEP)
    {
        slopeFix = rationalToFixed(aEdge.mSlope * SLOPE_FIX_STEP) -
                   (slope << SLOPE_FIX_SHIFT);
    }

    NON_ZERO_MASK_DATA_UNIT winding = (NON_ZERO_MASK_DATA_UNIT)aEdge.mWinding;
    for (y = aEdge.mFirstLine; y <= aEdge.mLastLine; y++)
    {
        int ySub = y & (SUBPIXEL_COUNT - 1);
        int xp = FIXED_TO_INT(x + offsets[ySub]);
        SUBPIXEL_DATA mask = 1 << ySub;
        int yLine = y >> SUBPIXEL_SHIFT;
        mMaskBuffer[yLine * mBufferWidth + xp] |= mask;
        mWindingBuffer[(yLine * mBufferWidth + xp) * SUBPIXEL_COUNT + ySub] += winding;
        x += slope;
        if ((y & SLOPE_FIX_MASK) == 0)
             x += slopeFix;
    }
}


//! Renders the mask to the canvas with non-zero winding fill.
void PolygonVersionB::fillNonZero(BitmapData *aTarget, unsigned long aColor, const ClipRectangle &aClipRect)
{
    unsigned int x,y;

    unsigned int minX = aClipRect.getMinXi();
    unsigned int maxX = aClipRect.getMaxXi();

    unsigned int minY = aClipRect.getMinYi() >> SUBPIXEL_SHIFT;
    unsigned int maxY = aClipRect.getMaxYi() >> SUBPIXEL_SHIFT;

    unsigned long cs1 = aColor & 0xff00ff;
    unsigned long cs2 = (aColor >> 8) & 0xff00ff;

    unsigned long *target = aTarget->mData;
    unsigned int pitch = aTarget->mPitch / 4;

    NON_ZERO_MASK_DATA_UNIT values[SUBPIXEL_COUNT];
    int n;
    for (n = 0; n < SUBPIXEL_COUNT; n++)
        values[n] = 0;

    for (y = minY; y < maxY; y++)
    {
        unsigned long *tp = &target[y * pitch + minX];

        SUBPIXEL_DATA mask = 0;
        for (x = minX; x <= maxX; x++)
        {
            SUBPIXEL_DATA temp = mMaskBuffer[y * mBufferWidth + x];
            if (temp)
            {
                SUBPIXEL_DATA bit = 1;
                for (n = 0; n < SUBPIXEL_COUNT; n++)
                {
                    if (temp & bit)
                    {
                       NON_ZERO_MASK_DATA_UNIT t = values[n];
                       values[n] += mWindingBuffer[(y * mBufferWidth + x) * SUBPIXEL_COUNT + n];
                       if ((t == 0) ^ (values[n] == 0))
                          mask ^= bit;
                    }
                    bit <<= 1;
                }
            }

            unsigned long alpha = SUBPIXEL_COVERAGE(mask);

            // alpha is in range of 0 to SUBPIXEL_COUNT
            unsigned long invAlpha = SUBPIXEL_COUNT - alpha;

            unsigned long ct1 = (*tp & 0xff00ff) * invAlpha;
            unsigned long ct2 = ((*tp >> 8) & 0xff00ff) * invAlpha;

            ct1 = ((ct1 + cs1 * alpha) >> SUBPIXEL_SHIFT) & 0xff00ff;
            ct2 = ((ct2 + cs2 * alpha) << (8 - SUBPIXEL_SHIFT)) & 0xff00ff00;

            *tp++ = ct1 + ct2;
        }
    }
}


//! Checks if a surface format is supported.
bool PolygonVersionB::isSupported(BitmapData::BitmapFormat aFormat)
{
    // Only non-alpha target formats are supported.
    if (aFormat == BitmapData::BITMAP_FORMAT_XRGB ||
        aFormat == BitmapData::BITMAP_FORMAT_XBGR ||
        aFormat == BitmapData::BITMAP_FORMAT_RGBX ||
        aFormat == BitmapData::BITMAP_FORMAT_BGRX)
    {
        return true;
    }
    else
    {
        return false;
    }
}

