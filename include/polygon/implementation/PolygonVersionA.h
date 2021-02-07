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
 * \brief PolygonFiller implementation version A. This is the basic implementation.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */


#ifndef POLYGON_VERSION_A_H_INCLUDED
#define POLYGON_VERSION_A_H_INCLUDED

//! Version A of the polygon filler. This is the basic implementation.
class PolygonVersionA : public PolygonFiller
{
public:

    //! Constructor.
    PolygonVersionA();

    //! Initializer.
    /*! aWidth and aHeight define the maximum output size for the filler.
     *  The filler will output to larger bitmaps as well, but the output will
     *  be cropped.
     */
    bool init(unsigned int aWidth, unsigned int aHeight);

    //! Virtual destructor.
    virtual ~PolygonVersionA();

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
    void fillEvenOdd(BitmapData *aTarget, unsigned long aColor, const ClipRectangle &aClipRect);

    //! Adds an edge to be used with non-zero winding fill.
    void addNonZeroEdge(const PolygonEdge &aEdge);

    //! Renders the mask to the canvas with non-zero winding fill.
    void fillNonZero(BitmapData *aTarget, unsigned long aColor, const ClipRectangle &aClipRect);

    SUBPIXEL_DATA *mMaskBuffer;
    NON_ZERO_MASK_DATA_UNIT *mWindingBuffer;

    unsigned int mWidth;
    unsigned int mBufferWidth;
    unsigned int mHeight;

    ClipRectangle mClipRect;
    MATRIX2D mRemappingMatrix;
};

#endif // !POLYGON_VERSION_A_H_INCLUDED
