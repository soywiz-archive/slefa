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
 * \brief A class for wrapping the GDI+ rendering implementation for the tests.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */


#include "polygon/base/Base.h"
#include "polygon/GDIplus/GDIplus.h"


//! Renders a polygon.
/*! /param aTarget the render target for rendering the polygon.
 *  /param aPolygon the polygon to render.
 *  /param aColor the rendering color.
 *  /param aFillMode the fill mode for rendering.
 *  /param aTransformation the transformation for rendering.
 */
void GDIplusRenderer::render(RenderTargetWrapper *aTarget, PolygonWrapper *aPolygon, PaintWrapper *aColor,
                             RENDERER_FILLMODE aFillMode, const MATRIX2D &aTransformation)
{
    unsigned int color = ((DefaultPaintWrapper *)aColor)->getColor();
    SolidBrush brush(Color(255, (unsigned char)((color >> 16) & 0xff),
                                (unsigned char)((color >> 8) & 0xff),
                                (unsigned char)(color & 0xff)));

    GraphicsPath *path = ((GDIplusPolygonWrapper *)aPolygon)->getPath();

    if (aFillMode == RENDERER_FILLMODE_EVENODD)
        path->SetFillMode(FillModeAlternate);
    else
        path->SetFillMode(FillModeWinding);

    Matrix transformation(MATRIX2D_GET_M11(aTransformation),
                          MATRIX2D_GET_M12(aTransformation),
                          MATRIX2D_GET_M21(aTransformation),
                          MATRIX2D_GET_M22(aTransformation),
                          MATRIX2D_GET_DX(aTransformation) - 0.625f,
                          MATRIX2D_GET_DY(aTransformation) - 0.625f);

    Graphics * graphics = ((GDIplusRenderTargetWrapper *)aTarget)->getGraphics();

    graphics->SetTransform(&transformation);
    graphics->FillPath(&brush,path);
}
