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
 * \brief PolygonFiller implementation version D. Similar to C, but uses mask tracking.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */

#include "polygon/base/Base.h"
#include "polygon/implementation/Implementation.h"

/*
Implementation with looping and compares
filler D star even-odd         0.082825
filler D star non-zero         0.263189
filler D siemens star even-odd 0.972188
filler D siemens star non-zero 3.158569
filler D circles even-odd      3.187550
filler D circles non-zero      6.109601
filler D random even-odd       0.531068
filler D random non-zero       1.731718

Implementation with looping and bit operations (no compare)
filler D star even-odd         0.082816
filler D star non-zero         0.248362
filler D siemens star even-odd 0.971455
filler D siemens star non-zero 2.905812
filler D circles even-odd      3.211994
filler D circles non-zero      5.609789
filler D random even-odd       0.533210
filler D random non-zero       1.480293

Implementation with NonZeroMask
filler D star even-odd         0.081080
filler D star non-zero         0.185329
filler D siemens star even-odd 0.974903
filler D siemens star non-zero 2.035923
filler D circles even-odd      3.191562
filler D circles non-zero      4.263064
filler D random even-odd       0.530713
filler D random non-zero       1.047195
*/

//! Constructor.
PolygonVersionD::PolygonVersionD()
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
bool PolygonVersionD::init(unsigned int aWidth, unsigned int aHeight)
{
    // The buffer used for filling needs to be three pixels wider than the bitmap.
    // This is because of three reasons: first, the buffer used for filling needs
    // to be one pixel wider because the end flag that turns the fill off is the
    // first pixel after the actually drawn edge. Second, the edge tracking needs
    // to add the maximum sample offset (i.e. 1) to the end of the edge. This
    // requires one pixel more in the buffer to avoid reading of data from the
    // left edge. Third, the rendering is stopped with an edge marker that is placed
    // after the data, thus requiring one pixel more for the maximum case.
    unsigned int bufferWidth = aWidth + 3;

    mMaskBuffer = new SUBPIXEL_DATA[bufferWidth * aHeight];
    if (mMaskBuffer == NULL)
        return false;
    MEMSET(mMaskBuffer,0,bufferWidth*aHeight*sizeof(SUBPIXEL_DATA));

    mWindingBuffer = new NonZeroMask[bufferWidth * aHeight];
    if (mWindingBuffer == NULL)
        return false;
    MEMSET(mWindingBuffer,0,bufferWidth*aHeight*sizeof(NonZeroMask));

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
PolygonVersionD::~PolygonVersionD()
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
void PolygonVersionD::renderEvenOdd(BitmapData *aTarget, const Polygon *aPolygon, unsigned long aColor, const MATRIX2D &aTransformation)
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
void PolygonVersionD::renderNonZeroWinding(BitmapData *aTarget, const Polygon *aPolygon, unsigned long aColor, const MATRIX2D &aTransformation)
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
void PolygonVersionD::setClipRect(unsigned int aX, unsigned int aY, unsigned int aWidth, unsigned int aHeight)
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
void PolygonVersionD::addEvenOddEdge(const PolygonEdge &aEdge)
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
void PolygonVersionD::fillEvenOdd(BitmapData *aTarget, unsigned long aColor)
{
    int y;

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
            SUBPIXEL_DATA *end = &maskBuffer[maxX + 1];

            // Place a marker data at the end position.
            // Since marker data is placed after the last entry to be rendered (thus mask is 0),
            // it forces a state change. This drops the control to the outer while loop, which
            // terminates because of the compare.
            *end = SUBPIXEL_FULL_COVERAGE;

            SUBPIXEL_DATA mask = *mb;
            *mb++ = 0;

            while (mb <= end)
            {
                // The straightforward implementation reads in the value, modifies the mask with it,
                // calculates the coverage from the mask and renders the result with it.

                // Here the mask value is fetched in the previous round. Therefore the write operation
                // needs to be done before the new mask value is changed. This also means that the
                // end marker will never be rendered, as the loop terminates before that.
                if (mask == 0)
                {
                    // Mask is empty, scan forward until mask changes.
                    SUBPIXEL_DATA *sb = mb;
                    do
                    {
                        mask = *mb++;
                    }
                    while (mask == 0);
                    mb[-1] = 0;
                    tp += mb - sb;
                }
                else if (mask == SUBPIXEL_FULL_COVERAGE)
                {
                    // Mask has full coverage, fill with aColor until mask changes.
                    SUBPIXEL_DATA temp;
                    do
                    {
                        *tp++ = aColor;
                        temp = *mb++;
                    }
                    while (temp == 0);
                    mb[-1] = 0;
                    mask ^= temp;
                }
                else
                {
                    // Mask is semitransparent.
                    do
                    {
                        unsigned long alpha = SUBPIXEL_COVERAGE(mask);

                        // alpha is in range of 0 to SUBPIXEL_COUNT
                        unsigned long invAlpha = SUBPIXEL_COUNT - alpha;

                        unsigned long ct1 = (*tp & 0xff00ff) * invAlpha;
                        unsigned long ct2 = ((*tp >> 8) & 0xff00ff) * invAlpha;

                        ct1 = ((ct1 + cs1 * alpha) >> SUBPIXEL_SHIFT) & 0xff00ff;
                        ct2 = ((ct2 + cs2 * alpha) << (8 - SUBPIXEL_SHIFT)) & 0xff00ff00;

                        *tp++ = ct1 + ct2;

                        mask ^= *mb;
                        *mb++ = 0;
                    }
                    while (!(mask == 0 || mask == SUBPIXEL_FULL_COVERAGE));
                }
            }
        }

        target += pitch;
        maskBuffer += mBufferWidth;
    }
}


//! Adds an edge to be used with non-zero winding fill.
void PolygonVersionD::addNonZeroEdge(const PolygonEdge &aEdge)
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

        NonZeroMask *windingBuffer = &mWindingBuffer[firstLine * mBufferWidth];

        SUBPIXEL_DATA mask = 1 << ys;
        int ySub;
        for (ySub = ys; ySub <= ye; ySub++)
        {
            int xp = FIXED_TO_INT(x + offsets[ySub]);
            windingBuffer[xp].mMask |= mask;
            windingBuffer[xp].mBuffer[ySub] += winding;
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

        NonZeroMask *windingBuffer = &mWindingBuffer[firstLine * mBufferWidth];

        SUBPIXEL_DATA mask = 1 << ys;
        int ySub;
        for (ySub = ys; ySub < SUBPIXEL_COUNT; ySub++)
        {
            int xp = FIXED_TO_INT(x + offsets[ySub]);
            windingBuffer[xp].mMask |= mask;
            windingBuffer[xp].mBuffer[ySub] += winding;
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
            windingBuffer = windingBuffer + mBufferWidth;

            mask = 1;
            for (ySub = 0; ySub < SUBPIXEL_COUNT; ySub++)
            {
                int xp = FIXED_TO_INT(x + offsets[ySub]);
                windingBuffer[xp].mMask |= mask;
                windingBuffer[xp].mBuffer[ySub] += winding;
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
        windingBuffer = windingBuffer + mBufferWidth;

        mask = 1;
        for (ySub = 0; ySub <= ye; ySub++)
        {
            int xp = FIXED_TO_INT(x + offsets[ySub]);
            windingBuffer[xp].mMask |= mask;
            windingBuffer[xp].mBuffer[ySub] += winding;
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
void PolygonVersionD::fillNonZero(BitmapData *aTarget, unsigned long aColor, const ClipRectangle &aClipRect)
{
    int y;

    int minY = mVerticalExtents.mMinimum;
    int maxY = mVerticalExtents.mMaximum;

    unsigned long cs1 = aColor & 0xff00ff;
    unsigned long cs2 = (aColor >> 8) & 0xff00ff;

    unsigned int pitch = aTarget->mPitch / 4;
    unsigned long *target = &aTarget->mData[minY * pitch];
    NonZeroMask *windingBuffer = &mWindingBuffer[minY * mBufferWidth];

    NonZeroMask values;

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
            NonZeroMask *wb = &windingBuffer[minX];
            NonZeroMask *end = &windingBuffer[maxX + 1];

            // Place a marker data at the end position.
            // Since marker data is placed after the last entry to be rendered (thus mask is 0),
            // it forces a state change. This drops the control to the outer while loop, which
            // terminates because of the compare.
            MEMSET(end,0xff,sizeof(NonZeroMask));

            NonZeroMask *temp = wb++;
            NonZeroMaskC::reset(*temp,values);

            while (wb <= end)
            {
                // The straightforward implementation reads in the value, modifies the mask with it,
                // calculates the coverage from the mask and renders the result with it.

                // Here the mask value is fetched in the previous round. Therefore the write operation
                // needs to be done before the new mask value is changed. This also means that the
                // end marker will never be rendered, as the loop terminates before that.
                if (values.mMask == 0)
                {
                    do
                    {
                        // Mask is empty, scan forward until mask changes.
                        NonZeroMask *sb = wb;
                        do
                        {
                            temp = wb++;
                        }
                        while (temp->mMask == 0 && wb <= end);

                        int count = wb - sb;
                        tp += count;

                        NonZeroMaskC::init(*temp,values);
                    }
                    while (values.mMask == 0); // && wb <= end);
                }
                else if (values.mMask == SUBPIXEL_FULL_COVERAGE)
                {
                    do
                    {
                        // Mask has full coverage, fill with aColor until mask changes.
                        *tp++ = aColor;
                        temp = wb++;

                        if (temp->mMask)
                        {
                            NonZeroMaskC::apply(*temp,values);

                            // A safeguard is needed if the winding value overflows and end marker is not
                            // detected correctly
                            if (wb > end)
                                break;
                        }
                    }
                    while (values.mMask == SUBPIXEL_FULL_COVERAGE); // && wb <= end);
                }
                else
                {
                    // Mask is semitransparent.
                    unsigned long alpha = SUBPIXEL_COVERAGE(values.mMask);

                    // alpha is in range of 0 to SUBPIXEL_COUNT
                    unsigned long invAlpha = SUBPIXEL_COUNT - alpha;
                    unsigned long cs1a = cs1 * alpha;
                    unsigned long cs2a = cs2 * alpha;

                    do
                    {
                        unsigned long ct1 = (*tp & 0xff00ff) * invAlpha;
                        unsigned long ct2 = ((*tp >> 8) & 0xff00ff) * invAlpha;

                        ct1 = ((ct1 + cs1a) >> SUBPIXEL_SHIFT) & 0xff00ff;
                        ct2 = ((ct2 + cs2a) << (8 - SUBPIXEL_SHIFT)) & 0xff00ff00;

                        *tp++ = ct1 + ct2;

                        temp = wb++;
                        if (temp->mMask)
                        {
                            NonZeroMaskC::apply(*temp,values);

                            alpha = SUBPIXEL_COVERAGE(values.mMask);
                            invAlpha = SUBPIXEL_COUNT - alpha;
                            cs1a = cs1 * alpha;
                            cs2a = cs2 * alpha;

                            // A safeguard is needed if the winding value overflows and end marker is not
                            // detected correctly
                            if (wb > end)
                                break;
                        }
                    }
                    while (values.mMask != 0 && values.mMask != SUBPIXEL_FULL_COVERAGE); // && wb <= end);
                }
            }
        }

        target += pitch;
        windingBuffer += mBufferWidth;
    }

}


//! Checks if a surface format is supported.
bool PolygonVersionD::isSupported(BitmapData::BitmapFormat aFormat)
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
void PolygonVersionD::resetExtents()
{
    unsigned int n;
    for (n = 0; n < mHeight; n++)
        mEdgeExtents[n].reset();
    mVerticalExtents.reset();
}
