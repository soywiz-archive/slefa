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


#ifndef GDI_PLUS_RENDERER_H_INCLUDED
#define GDI_PLUS_RENDERER_H_INCLUDED

//! A class for wrapping the GDI+ rendering implementation for the tests.
class GDIplusRenderer : public Renderer
{
public:
    //! Renders a polygon.
    /*! /param aTarget the render target for rendering the polygon.
     *  /param aPolygon the polygon to render.
     *  /param aColor the rendering color.
     *  /param aFillMode the fill mode for rendering.
     *  /param aTransformation the transformation for rendering.
     */
    virtual void render(RenderTargetWrapper *aTarget, PolygonWrapper *aPolygon, PaintWrapper *aColor,
                        RENDERER_FILLMODE aFillMode, const MATRIX2D &aTransformation);
};

#endif // !GDI_PLUS_RENDER_TARGET_H_INCLUDED
