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

#ifndef VECTOR_GRAPHIC_H_INCLUDED
#define VECTOR_GRAPHIC_H_INCLUDED

//! A vector graphic image is a set of polygons. Each polygon is associated with a color.
class VectorGraphic
{
public:
    //! Constructor.
    VectorGraphic(PolygonWrapper **aPolygons, PaintWrapper **aColors, RENDERER_FILLMODE *aFillModes, int aPolygonCount);

    //! Destructor.
    ~VectorGraphic();

    //! Returns the polygon count of the graphic.
    inline int getPolygonCount()
    {
        return mPolygonCount;
    }

    //! Returns the polygon at given index.
    inline const PolygonWrapper *getPolygon(int aIndex)
    {
        return mPolygons[aIndex];
    }

    //! Returns the color at given index.
    inline const PaintWrapper *getColor(int aIndex)
    {
        return mColors[aIndex];
    }

    //! Returns the fill mode at given index.
    inline RENDERER_FILLMODE getFillMode(int aIndex)
    {
        return mFillModes[aIndex];
    }

    //! Renders the vector graphic with given renderer.
    /*! /param aRenderer the class to use for rendering.
     *  /param aTarget the render target.
     *  /param aTransformation the transformation to use in the rendering.
     */
    void render(Renderer *aRenderer, RenderTargetWrapper *aTarget, const MATRIX2D &aTransformation);

    //! Static method for creating the vector graphic from data.
    static VectorGraphic * create(const PolygonData * const *aPolygons, const unsigned long *aColors,
                                  const RENDERER_FILLMODE *aFillModes, int aPolygonCount,
                                  PolygonFactory *aFactory);

protected:
    int mPolygonCount;
    PolygonWrapper **mPolygons;
    PaintWrapper **mColors;
    RENDERER_FILLMODE *mFillModes;
};

#endif // !VECTOR_GRAPHIC_H_INCLUDED
