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


#include "polygon/base/Base.h"
#include "polygon/AGG/AGG.h"

//! Constructor.
AGGRenderer::AGGRenderer(RenderTargetWrapper *aTarget) :
    mCachedTarget(aTarget),
    mPixf(*((AGGRenderTargetWrapper *)aTarget)->getBuffer()),
    mRendererBase(mPixf),
    mRenderer(mRendererBase),
    mMatrix(1.0,0.0,0.0,1.0,0.0,0.0),
    mConvTransformation(mPath, mMatrix)
{
    MATRIX2D_MAKEIDENTITY(mCachedTransformation);
}


//! Renders a polygon.
/*! /param aTarget the render target for rendering the polygon.
 *  /param aPolygon the polygon to render.
 *  /param aColor the rendering color.
 *  /param aFillMode the fill mode for rendering.
 *  /param aTransformation the transformation for rendering.
 */
void AGGRenderer::render(RenderTargetWrapper *aTarget, PolygonWrapper *aPolygon, PaintWrapper *aColor,
                             RENDERER_FILLMODE aFillMode, const MATRIX2D &aTransformation)
{
    unsigned int color = ((DefaultPaintWrapper *)aColor)->getColor();

    if (aTarget != mCachedTarget)
    {
        mPixf = pixfmt(*((AGGRenderTargetWrapper *)aTarget)->getBuffer());
        mRendererBase = agg::renderer_base<pixfmt>(mPixf);
        mRenderer = agg::renderer_scanline_aa_solid<agg::renderer_base<pixfmt> >(mRendererBase);
    }

    mMatrix = agg::trans_affine(MATRIX2D_GET_M11(aTransformation),
                                MATRIX2D_GET_M12(aTransformation),
                                MATRIX2D_GET_M21(aTransformation),
                                MATRIX2D_GET_M22(aTransformation),
                                MATRIX2D_GET_DX(aTransformation),
                                MATRIX2D_GET_DY(aTransformation));

    mRasterizer.reset();

    if (aFillMode == RENDERER_FILLMODE_NONZERO)
        mRasterizer.filling_rule(agg::fill_non_zero);
    else
        mRasterizer.filling_rule(agg::fill_even_odd);

    mRasterizer.add_path(mConvTransformation,((AGGPolygonWrapper *)aPolygon)->getPathIndex());

    agg::rgba8 col;
    col.r = (unsigned char)((color >> 16) & 0xff);
    col.g = (unsigned char)((color >> 8) & 0xff);
    col.b = (unsigned char)(color & 0xff);
    col.a = 255;

    mRenderer.color(col);

    agg::render_scanlines(mRasterizer, mScanline, mRenderer);
}
