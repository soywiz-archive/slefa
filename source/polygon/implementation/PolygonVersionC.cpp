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
 * \brief PolygonFiller implementation version C. Similar to B, but uses edge tracking.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */

#include "polygon/base/Base.h"
#include "polygon/implementation/Implementation.h"

//! Constructor.
PolygonVersionC::PolygonVersionC()
{
    mMaskBuffer = NULL;
    mWindingBuffer = NULL;
    mEdgeExtents = NULL;
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
bool PolygonVersionC::init(unsigned int aWidth, unsigned int aHeight)
{
    // The buffer used for filling needs to be two pixels wider than the bitmap.
    // This is because of two reasons: first, the buffer used for filling needs
    // to be one pixel wider because the end flag that turns the fill off is the
    // first pixel after the actually drawn edge. Second, the edge tracking needs
    // to add the maximum sample offset (i.e. 1) to the end of the edge. This
    // requires one pixel more in the buffer to avoid reading of data from the
    // left edge.
    unsigned int bufferWidth = aWidth + 2;

    mMaskBuffer = new SUBPIXEL_DATA[bufferWidth * aHeight];
    if (mMaskBuffer == NULL)
        return false;
    MEMSET(mMaskBuffer,0,bufferWidth*aHeight*sizeof(SUBPIXEL_DATA));

    mWindingBuffer = new NON_ZERO_MASK_DATA_UNIT[bufferWidth * aHeight * SUBPIXEL_COUNT];
    if (mWindingBuffer == NULL)
        return false;
    MEMSET(mWindingBuffer,0,bufferWidth*aHeight*SUBPIXEL_COUNT*sizeof(NON_ZERO_MASK_DATA_UNIT));

    mEdgeExtents = new SpanExtents[aHeight];
    if (mEdgeExtents == NULL)
        return false;

    mWidth = aWidth;
    mBufferWidth = bufferWidth;
    mHeight = aHeight;

    mClipRect.setClip(0,0,mWidth,mHeight,SUBPIXEL_COUNT);

    return true;
}


//! Virtual destructor.
PolygonVersionC::~PolygonVersionC()
{
    delete[] mMaskBuffer;
    delete[] mWindingBuffer;
    delete[] mEdgeExtents;
}


//! Renders the polygon with even-odd fill.
/*! \param aTarget the target bitmap.
 *  \param aPolygon the polygon to render.
 *  \param aColor the color to be used for rendering.
 *  \param aTransformation the transformation matrix.
 */
void PolygonVersionC::renderEvenOdd(BitmapData *aTarget, const Polygon *aPolygon, unsigned long aColor, const MATRIX2D &aTransformation)
{
    if (!isSupported(aTarget->mFormat))
        return;

    // Sets the round down mode in case it has been modified.
    setRoundDownMode();

    resetExtents();

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

    fillEvenOdd(aTarget,aColor);
}


//! Renders the polygon with non-zero winding fill.
/*! \param aTarget the target bitmap.
 *  \param aPolygon the polygon to render.
 *  \param aColor the color to be used for rendering.
 *  \param aTransformation the transformation matrix.
 */
void PolygonVersionC::renderNonZeroWinding(BitmapData *aTarget, const Polygon *aPolygon, unsigned long aColor, const MATRIX2D &aTransformation)
{
    if (!isSupported(aTarget->mFormat))
        return;

    // Sets the round down mode in case it has been modified.
    setRoundDownMode();

    resetExtents();

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
void PolygonVersionC::setClipRect(unsigned int aX, unsigned int aY, unsigned int aWidth, unsigned int aHeight)
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
void PolygonVersionC::addEvenOddEdge(const PolygonEdge &aEdge)
{
    static const FIXED_POINT offsets[SUBPIXEL_COUNT] = SUBPIXEL_OFFSETS_FIXED;

    FIXED_POINT x = rationalToFixed(aEdge.mX);
    FIXED_POINT slope = rationalToFixed(aEdge.mSlope);
    FIXED_POINT slopeFix = INT_TO_FIXED(0);

    if (aEdge.mLastLine - aEdge.mFirstLine >= SLOPE_FIX_STEP)
    {
        slopeFix = rationalToFixed(aEdge.mSlope * SLOPE_FIX_STEP) -
                   (slope << SLOPE_FIX_SHIFT);
    }

    // The plotting is divided to three stages: from start y to the end of the
    // full scanline, then completely full scanlines, and finally from the beginning
    // of the scanline to the end y.

    int firstLine = aEdge.mFirstLine >> SUBPIXEL_SHIFT;
    int lastLine = aEdge.mLastLine >> SUBPIXEL_SHIFT;

    mVerticalExtents.mark(firstLine,lastLine);

    // The case where start y and end y are within the same full scanline is treated
    // separately.
    if (firstLine == lastLine)
    {
        // First x value rounded down.
        int xs = FIXED_TO_INT(x);

        int ys = aEdge.mFirstLine & (SUBPIXEL_COUNT - 1);
        int ye = aEdge.mLastLine & (SUBPIXEL_COUNT - 1);

        SUBPIXEL_DATA *scanline = &mMaskBuffer[firstLine * mBufferWidth];

        SUBPIXEL_DATA mask = 1 << ys;
        int ySub;
        for (ySub = ys; ySub <= ye; ySub++)
        {
            int xp = FIXED_TO_INT(x + offsets[ySub]);
            scanline[xp] ^= mask;
            mask <<= 1;
            x += slope;
        }

        // Last x-value rounded down.
        int xe = FIXED_TO_INT(x - slope);

        // Mark the span.
        mEdgeExtents[firstLine].markWithSort(xs,xe);
    }
    else
    {
        // Process the first line.

        // First x value rounded down.
        int xs = FIXED_TO_INT(x);

        int ys = aEdge.mFirstLine & (SUBPIXEL_COUNT - 1);

        SUBPIXEL_DATA *scanline = &mMaskBuffer[firstLine * mBufferWidth];

        SUBPIXEL_DATA mask = 1 << ys;
        int ySub;
        for (ySub = ys; ySub < SUBPIXEL_COUNT; ySub++)
        {
            int xp = FIXED_TO_INT(x + offsets[ySub]);
            scanline[xp] ^= mask;
            mask <<= 1;
            x += slope;
        }

        // Last x-value rounded down.
        int xe = FIXED_TO_INT(x - slope);

        // Mark the span.
        mEdgeExtents[firstLine].markWithSort(xs,xe);

        // Process the lines between first and last.

        int y;
        for (y = firstLine + 1; y < lastLine; y++)
        {
            // First x value rounded down.
            xs = FIXED_TO_INT(x);

            // Next scanline.
            scanline = scanline + mBufferWidth;

            mask = 1;
            for (ySub = 0; ySub < SUBPIXEL_COUNT; ySub++)
            {
                int xp = FIXED_TO_INT(x + offsets[ySub]);
                scanline[xp] ^= mask;
                mask <<= 1;
                x += slope;
            }

            // Last x-value rounded down.
            xe = FIXED_TO_INT(x - slope);

            // Mark the span.
            mEdgeExtents[y].markWithSort(xs,xe);

            // Add the slope fix to keep the fixed point math accurate.
            if ((y & SLOPE_FIX_SCANLINE_MASK) == 0)
                x += slopeFix;
        }

        // Process the last line

        // First x value rounded down.
        xs = FIXED_TO_INT(x);

        int ye = aEdge.mLastLine & (SUBPIXEL_COUNT - 1);

        // Next scanline.
        scanline = scanline + mBufferWidth;

        mask = 1;
        for (ySub = 0; ySub <= ye; ySub++)
        {
            int xp = FIXED_TO_INT(x + offsets[ySub]);
            scanline[xp] ^= mask;
            mask <<= 1;
            x += slope;
        }

        // Last x-value rounded down.
        xe = FIXED_TO_INT(x - slope);

        // Mark the span.
        mEdgeExtents[lastLine].markWithSort(xs,xe);
    }
}


//! Renders the mask to the canvas with even-odd fill.
void PolygonVersionC::fillEvenOdd(BitmapData *aTarget, unsigned long aColor)
{
    int x,y;

    int minY = mVerticalExtents.mMinimum;
    int maxY = mVerticalExtents.mMaximum;

    unsigned long cs1 = aColor & 0xff00ff;
    unsigned long cs2 = (aColor >> 8) & 0xff00ff;

    unsigned int pitch = aTarget->mPitch / 4;
    unsigned long *target = &aTarget->mData[minY * pitch];
    SUBPIXEL_DATA *maskBuffer = &mMaskBuffer[minY * mBufferWidth];

    for (y = minY; y <= maxY; y++)
    {
        int minX = mEdgeExtents[y].mMinimum;
        // Offset values are not taken into account when calculating the extents, so add
        // one to the maximum. This makes sure that full spans are included, as offset
        // values are in the range of 0 to 1.
        int maxX = mEdgeExtents[y].mMaximum + 1;

        if (minX < maxX)
        {
            unsigned long *tp = &target[minX];
            SUBPIXEL_DATA *mb = &maskBuffer[minX];

            SUBPIXEL_DATA mask = 0;
            for (x = minX; x <= maxX; x++)
            {
                SUBPIXEL_DATA temp = *mb++;
                if (temp)
                {
                    mb[-1] = 0;
                    mask ^= temp;
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

        target += pitch;
        maskBuffer += mBufferWidth;
    }
}


//! Adds an edge to be used with non-zero winding fill.
void PolygonVersionC::addNonZeroEdge(const PolygonEdge &aEdge)
{
    static const FIXED_POINT offsets[SUBPIXEL_COUNT] = SUBPIXEL_OFFSETS_FIXED;

    FIXED_POINT x = rationalToFixed(aEdge.mX);
    FIXED_POINT slope = rationalToFixed(aEdge.mSlope);
    FIXED_POINT slopeFix = INT_TO_FIXED(0);

    if (aEdge.mLastLine - aEdge.mFirstLine >= SLOPE_FIX_STEP)
    {
        slopeFix = rationalToFixed(aEdge.mSlope * SLOPE_FIX_STEP) -
                   (slope << SLOPE_FIX_SHIFT);
    }

    NON_ZERO_MASK_DATA_UNIT winding = (NON_ZERO_MASK_DATA_UNIT)aEdge.mWinding;

    // The plotting is divided to three stages: from start y to the end of the
    // full scanline, then completely full scanlines, and finally from the beginning
    // of the scanline to the end y.

    int firstLine = aEdge.mFirstLine >> SUBPIXEL_SHIFT;
    int lastLine = aEdge.mLastLine >> SUBPIXEL_SHIFT;

    mVerticalExtents.mark(firstLine,lastLine);

    // The case where start y and end y are within the same full scanline is treated
    // separately.
    if (firstLine == lastLine)
    {
        // First x value rounded down.
        int xs = FIXED_TO_INT(x);

        int ys = aEdge.mFirstLine & (SUBPIXEL_COUNT - 1);
        int ye = aEdge.mLastLine & (SUBPIXEL_COUNT - 1);

        SUBPIXEL_DATA *scanline = &mMaskBuffer[firstLine * mBufferWidth];
        NON_ZERO_MASK_DATA_UNIT *windingbuffer = &mWindingBuffer[firstLine * mBufferWidth * SUBPIXEL_COUNT];

        SUBPIXEL_DATA mask = 1 << ys;
        int ySub;
        for (ySub = ys; ySub <= ye; ySub++)
        {
            int xp = FIXED_TO_INT(x + offsets[ySub]);
            scanline[xp] |= mask;
            windingbuffer[xp * SUBPIXEL_COUNT + ySub] += winding;
            mask <<= 1;
            x += slope;
        }

        // Last x-value rounded down.
        int xe = FIXED_TO_INT(x - slope);

        // Mark the span.
        mEdgeExtents[firstLine].markWithSort(xs,xe);
    }
    else
    {
        // Process the first line.

        // First x value rounded down.
        int xs = FIXED_TO_INT(x);

        int ys = aEdge.mFirstLine & (SUBPIXEL_COUNT - 1);

        SUBPIXEL_DATA *scanline = &mMaskBuffer[firstLine * mBufferWidth];
        NON_ZERO_MASK_DATA_UNIT *windingbuffer = &mWindingBuffer[firstLine * mBufferWidth * SUBPIXEL_COUNT];

        SUBPIXEL_DATA mask = 1 << ys;
        int ySub;
        for (ySub = ys; ySub < SUBPIXEL_COUNT; ySub++)
        {
            int xp = FIXED_TO_INT(x + offsets[ySub]);
            scanline[xp] |= mask;
            windingbuffer[xp * SUBPIXEL_COUNT + ySub] += winding;
            mask <<= 1;
            x += slope;
        }

        // Last x-value rounded down.
        int xe = FIXED_TO_INT(x - slope);

        // Mark the span.
        mEdgeExtents[firstLine].markWithSort(xs,xe);

        // Process the lines between first and last.

        int y;
        for (y = firstLine + 1; y < lastLine; y++)
        {
            // First x value rounded down.
            xs = FIXED_TO_INT(x);

            // Next scanline.
            scanline = scanline + mBufferWidth;
            windingbuffer = windingbuffer + (mBufferWidth * SUBPIXEL_COUNT);

            mask = 1;
            for (ySub = 0; ySub < SUBPIXEL_COUNT; ySub++)
            {
                int xp = FIXED_TO_INT(x + offsets[ySub]);
                scanline[xp] |= mask;
                windingbuffer[xp * SUBPIXEL_COUNT + ySub] += winding;
                mask <<= 1;
                x += slope;
            }

            // Last x-value rounded down.
            xe = FIXED_TO_INT(x - slope);

            // Mark the span.
            mEdgeExtents[y].markWithSort(xs,xe);

            // Add the slope fix to keep the fixed point math accurate.
            if ((y & SLOPE_FIX_SCANLINE_MASK) == 0)
                x += slopeFix;
        }

        // Process the last line

        // First x value rounded down.
        xs = FIXED_TO_INT(x);

        int ye = aEdge.mLastLine & (SUBPIXEL_COUNT - 1);

        // Next scanline.
        scanline = scanline + mBufferWidth;
        windingbuffer = windingbuffer + (mBufferWidth * SUBPIXEL_COUNT);

        mask = 1;
        for (ySub = 0; ySub <= ye; ySub++)
        {
            int xp = FIXED_TO_INT(x + offsets[ySub]);
            scanline[xp] |= mask;
            windingbuffer[xp * SUBPIXEL_COUNT + ySub] += winding;
            mask <<= 1;
            x += slope;
        }

        // Last x-value rounded down.
        xe = FIXED_TO_INT(x - slope);

        // Mark the span.
        mEdgeExtents[lastLine].markWithSort(xs,xe);
    }
}


//! Renders the mask to the canvas with non-zero winding fill.
void PolygonVersionC::fillNonZero(BitmapData *aTarget, unsigned long aColor, const ClipRectangle &aClipRect)
{
    int x,y;

    int minY = mVerticalExtents.mMinimum;
    int maxY = mVerticalExtents.mMaximum;

    unsigned long cs1 = aColor & 0xff00ff;
    unsigned long cs2 = (aColor >> 8) & 0xff00ff;

    unsigned int pitch = aTarget->mPitch / 4;
    unsigned long *target = &aTarget->mData[minY * pitch];
    SUBPIXEL_DATA *maskBuffer = &mMaskBuffer[minY * mBufferWidth];
    NON_ZERO_MASK_DATA_UNIT *windingBuffer = &mWindingBuffer[minY * mBufferWidth * SUBPIXEL_COUNT];

    NON_ZERO_MASK_DATA_UNIT values[SUBPIXEL_COUNT];
    int n;
    for (n = 0; n < SUBPIXEL_COUNT; n++)
        values[n] = 0;

    for (y = minY; y <= maxY; y++)
    {
        int minX = mEdgeExtents[y].mMinimum;
        // Offset values are not taken into account when calculating the extents, so add
        // one to the maximum. This makes sure that full spans are included, as offset
        // values are in the range of 0 to 1.
        int maxX = mEdgeExtents[y].mMaximum + 1;

        if (minX < maxX)
        {
            unsigned long *tp = &target[minX];
            SUBPIXEL_DATA *mb = &maskBuffer[minX];
            NON_ZERO_MASK_DATA_UNIT *wb = &windingBuffer[minX * SUBPIXEL_COUNT];

            SUBPIXEL_DATA mask = 0;
            for (x = minX; x <= maxX; x++)
            {
                SUBPIXEL_DATA temp = *mb++;
                if (temp)
                {
                    mb[-1] = 0; // Clear mask buffer
                    SUBPIXEL_DATA bit = 1;
                    for (n = 0; n < SUBPIXEL_COUNT; n++)
                    {
                        if (temp & bit)
                        {
                           NON_ZERO_MASK_DATA_UNIT t = values[n];
                           values[n] += wb[n];
                           wb[n] = 0; // Clear winding buffer
                           if ((t == 0) ^ (values[n] == 0))
                              mask ^= bit;
                        }
                        bit <<= 1;
                    }
                }
                wb = wb + SUBPIXEL_COUNT;

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

        target += pitch;
        maskBuffer += mBufferWidth;
        windingBuffer += mBufferWidth * SUBPIXEL_COUNT;
    }
}


//! Checks if a surface format is supported.
bool PolygonVersionC::isSupported(BitmapData::BitmapFormat aFormat)
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


//! Resets the fill extents.
void PolygonVersionC::resetExtents()
{
    unsigned int n;
    for (n = 0; n < mHeight; n++)
        mEdgeExtents[n].reset();
    mVerticalExtents.reset();
}
