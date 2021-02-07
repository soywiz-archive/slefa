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

#include "polygon/base/Base.h"
#include "polygon/test/Test.h"
#include "polygon/svg/DumpSVG.h"

// #define INCLUDE_QT_DATA

#pragma warning(disable:4305)
#ifdef INCLUDE_QT_DATA
#include "complex_data.inc"
#else
#define QT_POLYGON_A_SIZE 8
#define QT_POLYGON_B_SIZE 8
#define QT_POLYGON_C_SIZE 8

const RATIONAL gQTPolygonA[QT_POLYGON_A_SIZE] = {
    0.0f,0.0f,400.0f,0.0f,400.0f,400.0f,0.0f,400.0f
};

const RATIONAL gQTPolygonB[QT_POLYGON_B_SIZE] = {
    0.0f,0.0f,400.0f,0.0f,400.0f,400.0f,0.0f,400.0f
};

const RATIONAL gQTPolygonC[QT_POLYGON_C_SIZE] = {
    0.0f,0.0f,400.0f,0.0f,400.0f,400.0f,0.0f,400.0f
};
#endif

//! Creates a siemens star test pattern.
VectorGraphic * TestPattern::siemensStar(RATIONAL aSize, int aSegments, RATIONAL aRotation, const VECTOR2D &aCenter,
                                         RENDERER_FILLMODE aFillMode, unsigned long aColor, PolygonFactory *aFactory,
                                         const char *aSVGDumpName, const char *aBinaryDumpName)
{
    if (aSegments < 2)
        return NULL;

    int vertexCount = aSegments * 3;

    RATIONAL *vertices = new RATIONAL[2 * vertexCount];
    if (!vertices)
        return NULL;

    int current = 0;

    RATIONAL step = FLOAT_TO_RATIONAL(3.1415926535897932384626433832795f) / aSegments;
    RATIONAL angle = aRotation;

    int n;
    for (n = 0; n < aSegments; n++)
    {
        vertices[current++] = VECTOR2D_GETX(aCenter) + RATIONAL_SIN(angle) * aSize;
        vertices[current++] = VECTOR2D_GETY(aCenter) + RATIONAL_COS(angle) * aSize;

        angle += step;

        vertices[current++] = VECTOR2D_GETX(aCenter) + RATIONAL_SIN(angle) * aSize;
        vertices[current++] = VECTOR2D_GETY(aCenter) + RATIONAL_COS(angle) * aSize;

        angle += step;

        vertices[current++] = VECTOR2D_GETX(aCenter);
        vertices[current++] = VECTOR2D_GETY(aCenter);
    }

    RATIONAL *vertexData[1] = { vertices };
    int vertexCounts[1] = { vertexCount };

    PolygonData pdata(vertexData, vertexCounts, 1);
    PolygonData *polygons[1] = { &pdata };

    unsigned long colors[1] = { aColor };
    RENDERER_FILLMODE fillmodes[1] = { aFillMode };

    if (aSVGDumpName)
    {
        DumpSVG::write(aSVGDumpName,polygons,colors,fillmodes,1);
    }

    if (aBinaryDumpName)
    {
        DumpSVG::writeBinary(aBinaryDumpName,polygons,colors,fillmodes,1);
    }

    VectorGraphic *vg = VectorGraphic::create(polygons,colors,fillmodes,1,aFactory);

    delete[] vertices;

    return vg;
}


//! Creates a star test pattern with a fill in the middle.
VectorGraphic * TestPattern::filledStar(RATIONAL aSize, int aSegments, RATIONAL aFillSize, RATIONAL aRotation, const VECTOR2D &aCenter,
                                        RENDERER_FILLMODE aFillMode, unsigned long aColor, PolygonFactory *aFactory,
                                        const char *aSVGDumpName, const char *aBinaryDumpName)
{
    if (aSegments < 2)
        return NULL;

    int vertexCount = aSegments * 3;

    RATIONAL *vertices = new RATIONAL[2 * vertexCount];
    if (!vertices)
        return NULL;

    int current = 0;

    RATIONAL step = FLOAT_TO_RATIONAL(3.1415926535897932384626433832795f) / aSegments;
    RATIONAL angle = aRotation;

    RATIONAL middleSize = step * aFillSize;

    int n;
    for (n = 0; n < aSegments; n++)
    {
        vertices[current++] = VECTOR2D_GETX(aCenter) + RATIONAL_SIN(angle) * aSize;
        vertices[current++] = VECTOR2D_GETY(aCenter) + RATIONAL_COS(angle) * aSize;

        angle += step;

        vertices[current++] = VECTOR2D_GETX(aCenter) + RATIONAL_SIN(angle) * aSize;
        vertices[current++] = VECTOR2D_GETY(aCenter) + RATIONAL_COS(angle) * aSize;

        angle += step/2;

        vertices[current++] = VECTOR2D_GETX(aCenter) + RATIONAL_SIN(angle) * aFillSize;
        vertices[current++] = VECTOR2D_GETY(aCenter) + RATIONAL_COS(angle) * aFillSize;

        angle += step/2;
    }

    RATIONAL *vertexData[1] = { vertices };
    int vertexCounts[1] = { vertexCount };

    PolygonData pdata(vertexData, vertexCounts, 1);
    PolygonData *polygons[1] = { &pdata };

    unsigned long colors[1] = { aColor };
    RENDERER_FILLMODE fillmodes[1] = { aFillMode };

    if (aSVGDumpName)
    {
        DumpSVG::write(aSVGDumpName,polygons,colors,fillmodes,1);
    }

    if (aBinaryDumpName)
    {
        DumpSVG::writeBinary(aBinaryDumpName,polygons,colors,fillmodes,1);
    }

    VectorGraphic *vg = VectorGraphic::create(polygons,colors,fillmodes,1,aFactory);

    delete[] vertices;

    return vg;
}


//! Creates a test pattern of concentric circles.
/*! Note that the winding direction for all circles is the same, so non-zero winding fill rule
 *  should produce just one large disk.
 */
VectorGraphic * TestPattern::circles(RATIONAL aSize, RATIONAL aWidth, int aSegments, int aCount, const VECTOR2D &aCenter, 
                                     RENDERER_FILLMODE aFillMode, unsigned long aColor, PolygonFactory *aFactory,
                                     const char *aSVGDumpName, const char *aBinaryDumpName)
{
    if (aCount < 1)
        return NULL;

    if (aSegments < 3)
        return NULL;

    bool error = false;

    int count = aCount * 2;
    int n;
    int *vertexCounts = new int[count];
    if (vertexCounts)
    {
        for (n = 0; n < count; n++)
            vertexCounts[n] = aSegments;
    }

    RATIONAL **vertexData = new RATIONAL *[count];

    if (vertexData)
    {
        for (n = 0; n < count; n++)
            vertexData[n] = new RATIONAL[2 * aSegments];
    }

    if (vertexData == NULL || vertexCounts == NULL)
        error = true;

    if (!error)
    {
        for (n = 0; n < count; n++)
        {
            if (vertexData[n] == NULL)
                error = true;
        }
    }

    VectorGraphic *vg = NULL;

    if (!error)
    {
        RATIONAL step = FLOAT_TO_RATIONAL(2*3.1415926535897932384626433832795f) / aSegments;
        RATIONAL angle = INT_TO_RATIONAL(0);

        // For every line, make two circles.
        for (n = 0; n < aCount; n++)
        {
            RATIONAL outerSize = (RATIONAL)(n + 1) * aSize / (RATIONAL)aCount;
            RATIONAL innerSize = outerSize - aWidth;

            int current = 0;
            RATIONAL *innerVertices = vertexData[n*2];
            RATIONAL *outerVertices = vertexData[n*2 + 1];
            int p;
            for (p = 0; p < aSegments; p++)
            {
                RATIONAL sinVal = RATIONAL_SIN(angle);
                RATIONAL cosVal = RATIONAL_COS(angle);

                innerVertices[current] = VECTOR2D_GETX(aCenter) + sinVal * innerSize;
                outerVertices[current++] = VECTOR2D_GETX(aCenter) + sinVal * outerSize;

                innerVertices[current] = VECTOR2D_GETY(aCenter) + cosVal * innerSize;
                outerVertices[current++] = VECTOR2D_GETY(aCenter) + cosVal * outerSize;

                angle += step;
            }
        }

        PolygonData pdata(vertexData, vertexCounts, count);
        PolygonData *polygons[1] = { &pdata };

        unsigned long colors[1] = { aColor };
        RENDERER_FILLMODE fillmodes[1] = { aFillMode };

        if (aSVGDumpName)
        {
            DumpSVG::write(aSVGDumpName,polygons,colors,fillmodes,1);
        }

        if (aBinaryDumpName)
        {
            DumpSVG::writeBinary(aBinaryDumpName,polygons,colors,fillmodes,1);
        }

        vg = VectorGraphic::create(polygons,colors,fillmodes,1,aFactory);
    }

    if (vertexData)
    {
        for (n = 0; n < count; n++)
            delete[] vertexData[n];
        delete[] vertexData;
    }

    delete vertexCounts;

    return vg;
}


//! Creates a test pattern of random polygon.
VectorGraphic * TestPattern::randomPolygon(RATIONAL aSize, int aVertexCount, int aSeed, RATIONAL aMinStep, RATIONAL aMaxStep, const VECTOR2D &aCenter, 
                                           RENDERER_FILLMODE aFillMode, unsigned long aColor, PolygonFactory *aFactory,
                                           const char *aSVGDumpName, const char *aBinaryDumpName)
{
    if (aVertexCount < 3)
        return NULL;

    RATIONAL *vertices = new RATIONAL[2 * aVertexCount];
    if (!vertices)
        return NULL;

    int rnd = aSeed;

    #define RAND_MULTIPLIER 1103515245
    #define RAND_ADDER 12345
    #define UPDATE_RAND(a) RAND_MULTIPLIER * a + RAND_ADDER

    rnd = UPDATE_RAND(rnd);
    int xi = rnd >> 16;
    rnd = UPDATE_RAND(rnd);
    int yi = rnd >> 16;

    RATIONAL x = (RATIONAL)xi * aSize / INT_TO_RATIONAL(0x7fff);
    RATIONAL y = (RATIONAL)yi * aSize / INT_TO_RATIONAL(0x7fff);

    RATIONAL minSq = aMinStep * aMinStep;
    RATIONAL maxSq = aMaxStep * aMaxStep;
    RATIONAL sizeSq = aSize * aSize;

    int current = 0;
    int n;
    for (n = 0; n < aVertexCount; n++)
    {
        RATIONAL xs, xt, ys, yt, lsq;
        do
        {
            do
            {
                rnd = UPDATE_RAND(rnd);
                xi = rnd >> 16;
                rnd = UPDATE_RAND(rnd);
                yi = rnd >> 16;

                xs = (RATIONAL)xi * aMaxStep / INT_TO_RATIONAL(0x7fff);
                ys = (RATIONAL)yi * aMaxStep / INT_TO_RATIONAL(0x7fff);

                lsq = xs * xs + ys * ys;
            }
            while (lsq > maxSq || lsq < minSq);

            xt = x + xs;
            yt = y + ys;

            lsq = xt * xt + yt * yt;
        }
        while (lsq > sizeSq);

        x = xt;
        y = yt;

        vertices[current++] = VECTOR2D_GETX(aCenter) + x;
        vertices[current++] = VECTOR2D_GETY(aCenter) + y;
    }

    RATIONAL *vertexData[1] = { vertices };
    int vertexCounts[1] = { aVertexCount };

    PolygonData pdata(vertexData, vertexCounts, 1);
    PolygonData *polygons[1] = { &pdata };

    unsigned long colors[1] = { aColor };
    RENDERER_FILLMODE fillmodes[1] = { aFillMode };

    if (aSVGDumpName)
    {
        DumpSVG::write(aSVGDumpName,polygons,colors,fillmodes,1);
    }

    if (aBinaryDumpName)
    {
        DumpSVG::writeBinary(aBinaryDumpName,polygons,colors,fillmodes,1);
    }

    VectorGraphic *vg = VectorGraphic::create(polygons,colors,fillmodes,1,aFactory);

    delete[] vertices;

    return vg;
}


//! Creates a test pattern of a star.
VectorGraphic * TestPattern::star(RATIONAL aSize, int aSpikeCount, const VECTOR2D &aCenter,
                                  RENDERER_FILLMODE aFillMode, unsigned long aColor, PolygonFactory *aFactory,
                                  const char *aSVGDumpName, const char *aBinaryDumpName)
{
    if (aSpikeCount < 5)
        return NULL;

    if (aSpikeCount & 1)
    {
        RATIONAL angle = FLOAT_TO_RATIONAL(2.0f * 3.1415926535897932384626433832795f) / (RATIONAL)aSpikeCount;
        RATIONAL startAngle = FLOAT_TO_RATIONAL(1.0f * 3.1415926535897932384626433832795f);
        RATIONAL angleDelta = FLOAT_TO_RATIONAL(3.1415926535897932384626433832795f) + angle / INT_TO_RATIONAL(2);

        RATIONAL *vertices = starVertices(aSize,aSpikeCount,aCenter,startAngle,angleDelta);
        if (vertices == NULL)
            return NULL;
        
        RATIONAL *vertexData[1] = { vertices };
        int vertexCounts[1] = { aSpikeCount };

        PolygonData pdata(vertexData, vertexCounts, 1);
        PolygonData *polygons[1] = { &pdata };

        unsigned long colors[1] = { aColor };
        RENDERER_FILLMODE fillmodes[1] = { aFillMode };

        if (aSVGDumpName)
        {
            DumpSVG::write(aSVGDumpName,polygons,colors,fillmodes,1);
        }

        if (aBinaryDumpName)
        {
            DumpSVG::writeBinary(aBinaryDumpName,polygons,colors,fillmodes,1);
        }

        VectorGraphic *vg = VectorGraphic::create(polygons,colors,fillmodes,1,aFactory);

        delete[] vertices;

        return vg;
    }
    else if (aSpikeCount & 2)
    {
        int count = aSpikeCount / 2;

        RATIONAL angle = FLOAT_TO_RATIONAL(2.0f * 3.1415926535897932384626433832795f) / (RATIONAL)aSpikeCount;
        RATIONAL startAngleA = FLOAT_TO_RATIONAL(0.5f * 3.1415926535897932384626433832795f);
        RATIONAL startAngleB = FLOAT_TO_RATIONAL(-0.5f * 3.1415926535897932384626433832795f);
        RATIONAL angleDelta = FLOAT_TO_RATIONAL(3.1415926535897932384626433832795f) + angle;

        RATIONAL *verticesA = starVertices(aSize,count,aCenter,startAngleA,angleDelta);
        if (verticesA == NULL)
            return NULL;

        RATIONAL *verticesB = starVertices(aSize,count,aCenter,startAngleB,angleDelta);
        if (verticesB == NULL)
        {
            delete[] verticesA;
            return NULL;
        }
        
        RATIONAL *vertexData[2] = { verticesA, verticesB };
        int vertexCounts[2] = { count, count };

        PolygonData pdata(vertexData, vertexCounts, 2);
        PolygonData *polygons[1] = { &pdata };

        unsigned long colors[1] = { aColor };
        RENDERER_FILLMODE fillmodes[1] = { aFillMode };

        if (aSVGDumpName)
        {
            DumpSVG::write(aSVGDumpName,polygons,colors,fillmodes,1);
        }

        if (aBinaryDumpName)
        {
            DumpSVG::writeBinary(aBinaryDumpName,polygons,colors,fillmodes,1);
        }

        VectorGraphic *vg = VectorGraphic::create(polygons,colors,fillmodes,1,aFactory);

        delete[] verticesA;
        delete[] verticesB;

        return vg;
    }
    else
    {
        RATIONAL angle = FLOAT_TO_RATIONAL(2.0f * 3.1415926535897932384626433832795f) / (RATIONAL)aSpikeCount;
        RATIONAL startAngle = FLOAT_TO_RATIONAL(0.5f * 3.1415926535897932384626433832795f);
        RATIONAL angleDelta = FLOAT_TO_RATIONAL(3.1415926535897932384626433832795f) + angle;// / INT_TO_RATIONAL(2);

        RATIONAL *vertices = starVertices(aSize,aSpikeCount,aCenter,startAngle,angleDelta);
        if (vertices == NULL)
            return NULL;
        
        RATIONAL *vertexData[1] = { vertices };
        int vertexCounts[1] = { aSpikeCount };

        PolygonData pdata(vertexData, vertexCounts, 1);
        PolygonData *polygons[1] = { &pdata };

        unsigned long colors[1] = { aColor };
        RENDERER_FILLMODE fillmodes[1] = { aFillMode };

        if (aSVGDumpName)
        {
            DumpSVG::write(aSVGDumpName,polygons,colors,fillmodes,1);
        }

        if (aBinaryDumpName)
        {
            DumpSVG::writeBinary(aBinaryDumpName,polygons,colors,fillmodes,1);
        }

        VectorGraphic *vg = VectorGraphic::create(polygons,colors,fillmodes,1,aFactory);

        delete[] vertices;

        return vg;
    }
}


//! Creates a set of vertices for a star.
RATIONAL * TestPattern::starVertices(RATIONAL aSize, int aCount, const VECTOR2D &aCenter, RATIONAL aStartAngle, RATIONAL aAngleDelta)
{
    RATIONAL *vertices = new RATIONAL[2 * aCount];
    if (!vertices)
        return NULL;

    RATIONAL angle = aStartAngle;

    int current = 0;
    int n;
    for (n = 0; n < aCount; n++)
    {
        vertices[current++] = VECTOR2D_GETX(aCenter) + aSize * RATIONAL_SIN(angle);
        vertices[current++] = VECTOR2D_GETY(aCenter) + aSize * RATIONAL_COS(angle);

        angle += aAngleDelta;
    }

    return vertices;
}


//! Creates a test pattern of a checker board.
VectorGraphic * TestPattern::checkers(RATIONAL aSize, int aCount, const VECTOR2D &aCenter,
                                      RENDERER_FILLMODE aFillMode, unsigned long aColor, PolygonFactory *aFactory,
                                      const char *aSVGDumpName, const char *aBinaryDumpName)
{
    if (aCount < 1)
        return NULL;

    // The checker board is build up from rectangles so that there is equal amount of
    // horizontal and vertical overlapping rectangles.

    bool error = false;

    int rectCount = aCount * 2;
    int n;
    int *vertexCounts = new int[rectCount];
    if (vertexCounts)
    {
        for (n = 0; n < rectCount; n++)
            vertexCounts[n] = 4;
    }

    RATIONAL **vertexData = new RATIONAL *[rectCount];

    if (vertexData)
    {
        for (n = 0; n < rectCount; n++)
            vertexData[n] = new RATIONAL[8];
    }

    if (vertexData == NULL || vertexCounts == NULL)
        error = true;

    if (!error)
    {
        for (n = 0; n < rectCount; n++)
        {
            if (vertexData[n] == NULL)
                error = true;
        }
    }

    VectorGraphic *vg = NULL;

    if (!error)
    {
        int gridSize = aCount * 2 + 1;

        RATIONAL step = aSize * INT_TO_RATIONAL(2) / gridSize;

        RATIONAL minx = VECTOR2D_GETX(aCenter) - aSize;
        RATIONAL miny = VECTOR2D_GETY(aCenter) - aSize;
        RATIONAL maxx = VECTOR2D_GETX(aCenter) + aSize;
        RATIONAL maxy = VECTOR2D_GETY(aCenter) + aSize;

        RATIONAL x = minx + step;
        RATIONAL y = miny + step;

        for (n = 0; n < aCount; n++)
        {
            RATIONAL *hVertices = vertexData[n*2];
            RATIONAL *vVertices = vertexData[n*2 + 1];

            hVertices[0] = x;
            hVertices[1] = miny;

            hVertices[2] = x + step;
            hVertices[3] = miny;

            hVertices[4] = x + step;
            hVertices[5] = maxy;

            hVertices[6] = x;
            hVertices[7] = maxy;

            vVertices[0] = minx;
            vVertices[1] = y;

            vVertices[2] = maxx;
            vVertices[3] = y;

            vVertices[4] = maxx;
            vVertices[5] = y + step;

            vVertices[6] = minx;
            vVertices[7] = y + step;

            x += step * INT_TO_RATIONAL(2);
            y += step * INT_TO_RATIONAL(2);
        }

        PolygonData pdata(vertexData, vertexCounts, rectCount);
        PolygonData *polygons[1] = { &pdata };

        unsigned long colors[1] = { aColor };
        RENDERER_FILLMODE fillmodes[1] = { aFillMode };

        if (aSVGDumpName)
        {
            DumpSVG::write(aSVGDumpName,polygons,colors,fillmodes,1);
        }

        if (aBinaryDumpName)
        {
            DumpSVG::writeBinary(aBinaryDumpName,polygons,colors,fillmodes,1);
        }

        vg = VectorGraphic::create(polygons,colors,fillmodes,1,aFactory);
    }

    if (vertexData)
    {
        for (n = 0; n < rectCount; n++)
            delete[] vertexData[n];
        delete[] vertexData;
    }

    delete vertexCounts;

    return vg;
}


/*
More synthetic tests:
- lots of small shapes
- large shape with edge consisting of very small segments
  - Configurable squares rendering so that it's possible to have
    - always same coverage
    - same total length, more segments
    - same edge count, longer total length
- overdraw test

All can be done with the same configurable test generator.
- Squares in a nxn grid
- Each square has size
- Each square has p subpolygons, with size delta
- Each edge of the square has q segments
*/

//! Creates a test pattern of a set of squares.
VectorGraphic * TestPattern::squares(RATIONAL aSquareSize, RATIONAL aGridCellSize, int aGridSteps,
                                     int aSubPolygonCount, RATIONAL aSizeDelta, int aSegmentCount,
                                     const VECTOR2D &aCenter, RENDERER_FILLMODE aFillMode, unsigned long aColor,
                                     unsigned long aColorDelta, PolygonFactory *aFactory, const char *aSVGDumpName,
                                     const char *aBinaryDumpName)
{
    int objectCount = aGridSteps * aGridSteps;

    PolygonData **polygons = new PolygonData*[objectCount];
    unsigned long *colors = new unsigned long[objectCount];
    int **vertexCounts = new int*[objectCount];
    RATIONAL ***vertexDatas = new RATIONAL **[objectCount];

    RENDERER_FILLMODE *fillmodes = new RENDERER_FILLMODE[objectCount];

    if (polygons == NULL || colors == NULL || fillmodes == NULL ||
        vertexCounts == NULL || vertexDatas == NULL)
    {
        delete[] polygons;
        delete[] colors;
        delete[] fillmodes;
        delete[] vertexCounts;
        delete[] vertexDatas;
        return NULL;
    }

    int n;

    int rCol = (aColor & 0xff0000) >> 16;
    int gCol = (aColor & 0x00ff00) >> 8;
    int bCol = (aColor & 0x0000ff);
    int rDelta = (int)((char)((aColorDelta & 0xff0000) >> 16));
    int gDelta = (int)((char)((aColorDelta & 0x00ff00) >> 8));
    int bDelta = (int)((char)(aColorDelta & 0x0000ff));

    for (n = 0; n < objectCount; n++)
    {
        polygons[n] = NULL;
        colors[n] = (rCol << 16) | (gCol << 8) | bCol;

        rCol += rDelta;
        rCol &= 0xff;

        gCol += gDelta;
        gCol &= 0xff;

        bCol += bDelta;
        bCol &= 0xff;

        fillmodes[n] = aFillMode;
        vertexCounts[n] = NULL;
        vertexDatas[n] = NULL;
    }

    RATIONAL gridRadius = aGridCellSize * aGridSteps / INT_TO_RATIONAL(2);

    RATIONAL xMin = VECTOR2D_GETX(aCenter) - gridRadius;
    RATIONAL yMin = VECTOR2D_GETY(aCenter) - gridRadius;

    RATIONAL yPos = yMin;

    bool error = false;
    int x,y;
    n = 0;
    y = 0;
    while (y < aGridSteps && !error)
    {
        x = 0;
        RATIONAL xPos = xMin;
        while (x < aGridSteps && !error)
        {
            int *vc = new int[aSubPolygonCount];
            int p;
            if (vc)
            {
                for (p = 0; p < aSubPolygonCount; p++)
                    vc[p] = aSegmentCount * 4;
                vertexCounts[n] = vc;
            }

            RATIONAL **vd = new RATIONAL *[aSubPolygonCount];

            if (vd)
            {
                for (p = 0; p < aSubPolygonCount; p++)
                {
                    vd[p] = new RATIONAL[aSegmentCount * 8];
                    if (vd[p] == NULL)
                        error = true;
                }
                vertexDatas[n] = vd;
            }

            if (vd == NULL || vc == NULL)
                error = true;

            if (!error)
            {
                RATIONAL squareSize = aSquareSize * INT_TO_RATIONAL(2);
                for (p = 0; p < aSubPolygonCount; p++)
                {
                    RATIONAL *v = vd[p];

                    RATIONAL segmentStep = squareSize / (RATIONAL)aSegmentCount;
                    RATIONAL squareOffset = squareSize / INT_TO_RATIONAL(2);

                    RATIONAL xp = xPos - squareOffset;
                    RATIONAL yp = yPos - squareOffset;

                    static const RATIONAL xDir[4] = {INT_TO_RATIONAL(0),
                                                     INT_TO_RATIONAL(1),
                                                     INT_TO_RATIONAL(0),
                                                     INT_TO_RATIONAL(-1)};

                    static const RATIONAL yDir[4] = {INT_TO_RATIONAL(1),
                                                     INT_TO_RATIONAL(0),
                                                     INT_TO_RATIONAL(-1),
                                                     INT_TO_RATIONAL(0)};

                    int r,s;
                    for (r = 0; r < 4; r++)
                    {
                        for (s = 0; s < aSegmentCount; s++)
                        {
                            *v++ = xp;
                            *v++ = yp;

                            xp += xDir[r] * segmentStep;
                            yp += yDir[r] * segmentStep;
                        }
                    }

                    squareSize += aSizeDelta;
                }

                polygons[n] = new PolygonData(vd,vc,aSubPolygonCount);
                if (polygons[n] == NULL)
                    error = true;
            }

            xPos += aGridCellSize;
            n++;
            x++;
        }

        yPos += aGridCellSize;
        y++;
    }

    VectorGraphic *vg = NULL;

    if (!error)
    {
        if (aSVGDumpName)
            DumpSVG::write(aSVGDumpName,polygons,colors,fillmodes,objectCount);

        if (aBinaryDumpName)
            DumpSVG::writeBinary(aBinaryDumpName,polygons,colors,fillmodes,1);

        vg = VectorGraphic::create(polygons,colors,fillmodes,objectCount,aFactory);
    }

    if (vertexCounts)
    {
        for (n = 0; n < objectCount; n++)
        {
            int *vc = vertexCounts[n];
            delete[] vc;
        }

        delete[] vertexCounts;
    }

    if (vertexDatas)
    {
        for (n = 0; n < objectCount; n++)
        {
            RATIONAL **vd = vertexDatas[n];
            if (vd)
            {
                int p;
                for (p = 0; p < aSubPolygonCount; p++)
                    delete[] vd[p];
                delete[] vd;
            }
        }

        delete[] vertexDatas;
    }

    if (polygons)
    {
        for (n = 0; n < objectCount; n++)
        {
            delete polygons[n];
        }
        delete[] polygons;
    }

    delete[] colors;
    delete[] fillmodes;

    return vg;
}


//! Creates a test polygon with Zack Rusin's QT test polygons (from http://ktown.kde.org/~zrusin/complex.data)
/*! aIndex should be in the range of 0 to 2.
 */
VectorGraphic * TestPattern::qtPolygon(int aIndex,
                                       RENDERER_FILLMODE aFillMode, unsigned long aColor, PolygonFactory *aFactory,
                                       const char *aSVGDumpName, const char *aBinaryDumpName)
{
    // Data from complex_data.inc (complex.data converted to C arrays).
    static const RATIONAL *datas[] = { gQTPolygonA, gQTPolygonB, gQTPolygonC };
    static const int dataSizes[] = { QT_POLYGON_A_SIZE, QT_POLYGON_B_SIZE, QT_POLYGON_C_SIZE };

    const RATIONAL *data = datas[aIndex];
    int size = dataSizes[aIndex];

    const RATIONAL *vertexData[1] = { data };
    int vertexCounts[1] = { size/2 };

    PolygonData pdata(vertexData, vertexCounts, 1);
    PolygonData *polygons[1] = { &pdata };

    unsigned long colors[1] = { aColor };
    RENDERER_FILLMODE fillmodes[1] = { aFillMode };

    if (aSVGDumpName)
    {
        DumpSVG::write(aSVGDumpName,polygons,colors,fillmodes,1);
    }

    if (aBinaryDumpName)
    {
        DumpSVG::writeBinary(aBinaryDumpName,polygons,colors,fillmodes,1);
    }

    VectorGraphic *vg = VectorGraphic::create(polygons,colors,fillmodes,1,aFactory);

    return vg;
}
