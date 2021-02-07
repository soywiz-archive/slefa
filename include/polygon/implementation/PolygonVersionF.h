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
 * \brief PolygonFiller implementation version F. Similar to E, but does the rendering one scanline at a time.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */


#ifndef POLYGON_VERSION_F_H_INCLUDED
#define POLYGON_VERSION_F_H_INCLUDED

#include "polygon/implementation/SpanExtents.h"

//! Version F of the polygon filler. Similar to E, but does the rendering one scanline at a time.
class PolygonVersionF : public PolygonFiller
{
public:

    //! Constructor.
    PolygonVersionF();

    //! Initializer.
    /*! aWidth and aHeight define the maximum output size for the filler.
     *  The filler will output to larger bitmaps as well, but the output will
     *  be cropped. aEdgeCount defines the initial amount of edges available for
     *  rendering.
     */
    bool init(unsigned int aWidth, unsigned int aHeight, unsigned int aEdgeCount);

    //! Virtual destructor.
    virtual ~PolygonVersionF();

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

    //! Adds an edge.
    bool addEdge(const PolygonEdge &aEdge);

    //! Renders the edges from the current vertical index using even-odd fill.
    inline void renderEvenOddEdges(PolygonScanEdge * &aActiveEdgeTable, SpanExtents &aEdgeExtents, int aCurrentLine);

    //! Renders the mask to the canvas with even-odd fill.
    void fillEvenOdd(BitmapData *aTarget, unsigned long aColor);

    //! Renders the edges from the current vertical index using non-zero winding fill.
    inline void renderNonZeroEdges(PolygonScanEdge * &aActiveEdgeTable, SpanExtents &aEdgeExtents, int aCurrentLine);

    //! Renders the mask to the canvas with non-zero winding fill.
    void fillNonZero(BitmapData *aTarget, unsigned long aColor, const ClipRectangle &aClipRect);

    //! Resets the fill extents.
    void resetExtents();

    //! Resizes the edge storage.
    bool resizeEdgeStorage(int aIncrement);

    //! Returns the amount of free edges in the edge storage.
    inline int getFreeEdgeCount()
    {
        return mEdgeCount - mCurrentEdge - 1;
    }

    SUBPIXEL_DATA *mMaskBuffer;
    NonZeroMask *mWindingBuffer;
    SpanExtents mVerticalExtents;

    PolygonScanEdge **mEdgeTable;
    PolygonScanEdge *mEdgeStorage;
    unsigned int mEdgeCount;
    unsigned int mCurrentEdge;

    unsigned int mWidth;
    unsigned int mBufferWidth;
    unsigned int mHeight;

    ClipRectangle mClipRect;
    MATRIX2D mRemappingMatrix;
};

#endif // !POLYGON_VERSION_F_H_INCLUDED
