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
 * \brief A class for wrapping the AGG rendering implementation for the tests.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */


#ifndef AGG_RENDERER_H_INCLUDED
#define AGG_RENDERER_H_INCLUDED

//! A class for wrapping the AGG rendering implementation for the tests.
class AGGRenderer : public Renderer
{
public:
    //! Constructor.
    AGGRenderer(RenderTargetWrapper *aTarget);

    //! Renders a polygon.
    /*! /param aTarget the render target for rendering the polygon.
     *  /param aPolygon the polygon to render.
     *  /param aColor the rendering color.
     *  /param aFillMode the fill mode for rendering.
     *  /param aTransformation the transformation for rendering.
     */
    virtual void render(RenderTargetWrapper *aTarget, PolygonWrapper *aPolygon, PaintWrapper *aColor,
                        RENDERER_FILLMODE aFillMode, const MATRIX2D &aTransformation);

    //! Returns a pointer to the path storage of the renderer.
    agg::path_storage *getPathStorage()
    {
        return &mPath;
    }

protected:
    agg::rasterizer_scanline_aa<> mRasterizer;
    agg::scanline_p8  mScanline;
    agg::path_storage mPath;

    RenderTargetWrapper * mCachedTarget;
    pixfmt mPixf;
    agg::renderer_base<pixfmt> mRendererBase;
    agg::renderer_scanline_aa_solid<agg::renderer_base<pixfmt> > mRenderer;

    MATRIX2D mCachedTransformation;
    agg::trans_affine mMatrix;
    agg::conv_transform<agg::path_storage, agg::trans_affine> mConvTransformation;
};

#endif // !AGG_RENDER_TARGET_H_INCLUDED
