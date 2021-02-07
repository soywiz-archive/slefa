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
 * \brief A utility class for constructing test patterns.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */


#ifndef TEST_PATTERN_H_INCLUDED
#define TEST_PATTERN_H_INCLUDED

//! A utility class for constructing test patterns.
class TestPattern
{
public:
    //! Creates a siemens star test pattern.
    static VectorGraphic * siemensStar(RATIONAL aSize, int aSegments, RATIONAL aRotation, const VECTOR2D &aCenter,
                                       RENDERER_FILLMODE aFillMode, unsigned long aColor, PolygonFactory *aFactory,
                                       const char *aSVGDumpName, const char *aBinaryDumpName);

    //! Creates a star test pattern with a fill in the middle.
    static VectorGraphic * filledStar(RATIONAL aSize, int aSegments, RATIONAL aFillSize, RATIONAL aRotation, const VECTOR2D &aCenter,
                                      RENDERER_FILLMODE aFillMode, unsigned long aColor, PolygonFactory *aFactory,
                                      const char *aSVGDumpName, const char *aBinaryDumpName);

    //! Creates a test pattern of concentric circles.
    /*! Note that the winding direction for all circles is the same, so non-zero winding fill rule
     *  should produce just one large disk.
     */
    static VectorGraphic * circles(RATIONAL aSize, RATIONAL aWidth, int aSegments, int aCount, const VECTOR2D &aCenter, 
                                   RENDERER_FILLMODE aFillMode, unsigned long aColor, PolygonFactory *aFactory,
                                   const char *aSVGDumpName, const char *aBinaryDumpName);

    //! Creates a test pattern of random polygon.
    static VectorGraphic * randomPolygon(RATIONAL aSize, int aVertexCount, int aSeed, RATIONAL aMinStep, RATIONAL aMaxStep, const VECTOR2D &aCenter, 
                                         RENDERER_FILLMODE aFillMode, unsigned long aColor, PolygonFactory *aFactory,
                                         const char *aSVGDumpName, const char *aBinaryDumpName);

    //! Creates a test pattern of a star.
    static VectorGraphic * star(RATIONAL aSize, int aSpikeCount, const VECTOR2D &aCenter,
                                RENDERER_FILLMODE aFillMode, unsigned long aColor, PolygonFactory *aFactory,
                                const char *aSVGDumpName, const char *aBinaryDumpName);

    //! Creates a test pattern of a checker board.
    static VectorGraphic * checkers(RATIONAL aSize, int aCount, const VECTOR2D &aCenter,
                                    RENDERER_FILLMODE aFillMode, unsigned long aColor, PolygonFactory *aFactory,
                                    const char *aSVGDumpName, const char *aBinaryDumpName);

    //! Creates a test pattern of a set of squares.
    static VectorGraphic * squares(RATIONAL aSquareSize, RATIONAL aGridCellSize, int aGridSteps,
                                   int aSubPolygonCount, RATIONAL aSizeDelta, int aSegmentCount,
                                   const VECTOR2D &aCenter, RENDERER_FILLMODE aFillMode, unsigned long aColor,
                                   unsigned long aColorDelta, PolygonFactory *aFactory, const char *aSVGDumpName,
                                   const char *aBinaryDumpName);

    //! Creates a test polygon with Zack Rusin's QT test polygons (from http://ktown.kde.org/~zrusin/complex.data)
    /*! aIndex should be in the range of 0 to 2.
     */
    static VectorGraphic * qtPolygon(int aIndex,
                                     RENDERER_FILLMODE aFillMode, unsigned long aColor, PolygonFactory *aFactory,
                                     const char *aSVGDumpName, const char *aBinaryDumpName);
protected:
    //! Creates a set of vertices for a star.
    static RATIONAL * starVertices(RATIONAL aSize, int aCount, const VECTOR2D &aCenter, RATIONAL aStartAngle, RATIONAL aAngleDelta);

};

#endif // !TEST_PATTERN_H_INCLUDED
