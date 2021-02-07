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


#ifndef POLYGON_VERSION_C_H_INCLUDED
#define POLYGON_VERSION_C_H_INCLUDED

#include "polygon/implementation/SpanExtents.h"

//! Version C of the polygon filler. Similar to B, but uses edge tracking.
class PolygonVersionC : public PolygonFiller
{
public:

    //! Constructor.
    PolygonVersionC();

    //! Initializer.
    /*! aWidth and aHeight define the maximum output size for the filler.
     *  The filler will output to larger bitmaps as well, but the output will
     *  be cropped.
     */
    bool init(unsigned int aWidth, unsigned int aHeight);

    //! Virtual destructor.
    virtual ~PolygonVersionC();

    //! Renders the polygon with even-odd fill.
    /*! \param aTarget the target bitmap.
     *  \param aPolygon the polygon to render.
     *  \param aColor the color to be used for rendering.
     *  \param aTransformation the transformation matrix.
     */
    virtual void renderEvenOdd(BitmapData *aTarget, const Polygon *aPolygon, unsigned long aColor, const MATRIX2D &aTransformation);

    //! Renders the polygon with non-zero winding fill.
    /*! \param aTarget the target bitmap.
     *  \param aPolygon the polygon to render.
     *  \param aColor the color to be used for rendering.
     *  \param aTransformation the transformation matrix.
     */
    virtual void renderNonZeroWinding(BitmapData *aTarget, const Polygon *aPolygon, unsigned long aColor, const MATRIX2D &aTransformation);

    //! Sets the clip rectangle for the polygon filler.
    virtual void setClipRect(unsigned int aX, unsigned int aY, unsigned int aWidth, unsigned int aHeight);

protected:
    //! Checks if a surface format is supported.
    bool isSupported(BitmapData::BitmapFormat aFormat);

    //! Adds an edge to be used with even-odd fill.
    void addEvenOddEdge(const PolygonEdge &aEdge);

    //! Renders the mask to the canvas with even-odd fill.
    void fillEvenOdd(BitmapData *aTarget, unsigned long aColor);

    //! Adds an edge to be used with non-zero winding fill.
    void addNonZeroEdge(const PolygonEdge &aEdge);

    //! Renders the mask to the canvas with non-zero winding fill.
    void fillNonZero(BitmapData *aTarget, unsigned long aColor, const ClipRectangle &aClipRect);

    //! Resets the fill extents.
    void resetExtents();

    SUBPIXEL_DATA *mMaskBuffer;
    NON_ZERO_MASK_DATA_UNIT *mWindingBuffer;
    SpanExtents *mEdgeExtents;
    SpanExtents mVerticalExtents;

    unsigned int mWidth;
    unsigned int mBufferWidth;
    unsigned int mHeight;

    ClipRectangle mClipRect;
    MATRIX2D mRemappingMatrix;
};

#endif // !POLYGON_VERSION_C_H_INCLUDED
