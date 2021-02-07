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
 * \brief A vector graphic image is a set of polygons. Each polygon is associated with a color.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */

#include "polygon/base/Base.h"

//! Constructor.
VectorGraphic::VectorGraphic(PolygonWrapper **aPolygons, PaintWrapper **aColors, RENDERER_FILLMODE *aFillModes, int aPolygonCount) :
    mPolygons(aPolygons), mColors(aColors), mFillModes(aFillModes), mPolygonCount(aPolygonCount)
{
  
}


//! Destructor.
VectorGraphic::~VectorGraphic()
{
    int n;
    for (n = 0; n < mPolygonCount; n++)
    {
        delete mPolygons[n];
        delete mColors[n];
    }
    delete[] mPolygons;
    delete[] mColors;
    delete[] mFillModes;
}


//! Renders the vector graphic with given renderer.
/*! /param aRenderer the class to use for rendering.
 *  /param aTarget the render target.
 *  /param aTransformation the transformation to use in the rendering.
 */
void VectorGraphic::render(Renderer *aRenderer, RenderTargetWrapper *aTarget, const MATRIX2D &aTransformation)
{
    int n;
    for (n = 0; n < mPolygonCount; n++)
    {
        aRenderer->render(aTarget,mPolygons[n],mColors[n],mFillModes[n],aTransformation);
    }

    aTarget->onRenderDone();
}


//! Static method for creating the vector graphic from data.
VectorGraphic * VectorGraphic::create(const PolygonData * const *aPolygons, const unsigned long *aColors,
                                      const RENDERER_FILLMODE *aFillModes, int aPolygonCount, PolygonFactory *aFactory)
{
    PolygonWrapper **polygons = new PolygonWrapper*[aPolygonCount];
    PaintWrapper **colors = new PaintWrapper*[aPolygonCount];
    RENDERER_FILLMODE *fillModes = new RENDERER_FILLMODE[aPolygonCount];

    if (polygons == NULL || colors == NULL || fillModes == NULL)
    {
        delete[] polygons;
        delete[] colors;
        delete[] fillModes;
        return NULL;
    }

    aFactory->beginGraphic();
    int n;
    for (n = 0; n < aPolygonCount; n++)
    {
        polygons[n] = aFactory->createPolygonWrapper(aPolygons[n]);
        colors[n] = aFactory->createPaintWrapper(aColors[n]);
        if (polygons[n] == NULL || colors[n] == NULL)
        {
            int p;
            for (p = 0; p < n; p++)
            {
                delete polygons[p];
                delete colors[n];
            }
            delete[] polygons;
            delete[] colors;
            delete[] fillModes;
            return NULL;
        }        

        fillModes[n] = aFillModes[n];
    }
    aFactory->endGraphic();

    VectorGraphic *vg = new VectorGraphic(polygons,colors,fillModes,aPolygonCount);

    if (vg == NULL)
    {
        for (n = 0; n < aPolygonCount; n++)
        {
            delete polygons[n];
            delete colors[n];
        }
        delete[] polygons;
        delete[] colors;
        delete[] fillModes;
        return NULL;
    }

    return vg;
}
