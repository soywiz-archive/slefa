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
 * \brief A base class for defining renderers. The actual implementation of the renderer
 *        depends on the platform and fillers.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */


#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

#define RENDERER_FILLMODE unsigned char
#define RENDERER_FILLMODE_EVENODD 0
#define RENDERER_FILLMODE_NONZERO 1

//! A base class for defining renderers.
/*! The actual implementation of the renderer depends on the platform and fillers.
 */
class Renderer
{
public:
    //! Virtual destructor.
    virtual ~Renderer() {}

    //! Renders a polygon.
    /*! /param aTarget the render target for rendering the polygon.
     *  /param aPolygon the polygon to render.
     *  /param aColor the rendering color.
     *  /param aFillMode the fill mode for rendering.
     *  /param aTransformation the transformation for rendering.
     */
    virtual void render(RenderTargetWrapper *aTarget, PolygonWrapper *aPolygon, PaintWrapper *aColor,
                        RENDERER_FILLMODE aFillMode, const MATRIX2D &aTransformation) = 0;
};

#endif // !RENDER_TARGET_H_INCLUDED
