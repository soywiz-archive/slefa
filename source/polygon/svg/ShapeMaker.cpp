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
 * \brief A utility class for building polygons out of SVG commands.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */

#include "polygon/base/Base.h"
#include "polygon/svg/ShapeMaker.h"


// Constructor.
ShapeMaker::ShapeMaker()
{
    mShapeData = NULL;
    MATRIX2D_MAKEIDENTITY(mTransformation);
    mCurrentColor = 0;
    mCurrentFillMode = RENDERER_FILLMODE_EVENODD;
    mBezierSteps = 32;
}


// Destructor.
ShapeMaker::~ShapeMaker()
{
    delete mShapeData;
}


// Sets the transformation matrix.
void ShapeMaker::setTransformation(const MATRIX2D &aTransformation)
{
    mTransformation = aTransformation;
}


//! Sets the current color.
/*! Current color is applied to the path at beginPath().
 */
void ShapeMaker::setColor(unsigned int aColor)
{
    mCurrentColor = aColor;
}


//! Sets the current fillrule.
/*! Current fillrule is applied to the path at beginPath().
 */
void ShapeMaker::setFillRule(RENDERER_FILLMODE aFillMode)
{
    mCurrentFillMode = aFillMode;
}


//! Starts a new shape and clears the shape data collected this far.
/*! This must be called as the first thing before assigning any commands to
 *  the shape maker.
 */
bool ShapeMaker::newShape()
{
    delete mShapeData;
    mShapeData = new ShapeMaker::ShapeData();
    if (mShapeData == NULL)
        return false;
    return true;
}


//! Begins a new path.
bool ShapeMaker::beginPath()
{
    return mShapeData->beginPolygon(mCurrentColor,mCurrentFillMode);
}


//! Ends the path.
bool ShapeMaker::endPath()
{
    return mShapeData->endPolygon();
}


// Moves the draw head to an absolute position.
bool ShapeMaker::moveToAbsolute(const VECTOR2D &aPosition)
{
    mCurrentPoint = aPosition;
    mCurrentControlPoint = aPosition;
    mStartPoint = aPosition;

    VECTOR2D position;
    MATRIX2D_TRANSFORM(mTransformation,aPosition,position);

    return moveTo(position);
}


// Moves the draw head to a position relative to the previous one.
bool ShapeMaker::moveToRelative(const VECTOR2D &aPosition)
{
    VECTOR2D position;
    VECTOR2D_SETX(position,VECTOR2D_GETX(mCurrentPoint) + VECTOR2D_GETX(aPosition));
    VECTOR2D_SETY(position,VECTOR2D_GETY(mCurrentPoint) + VECTOR2D_GETY(aPosition));

    return moveToAbsolute(position);
}


// Draws a line to an absolute position.
bool ShapeMaker::lineToAbsolute(const VECTOR2D &aPosition)
{
    mCurrentPoint = aPosition;
    mCurrentControlPoint = aPosition;

    VECTOR2D position;
    MATRIX2D_TRANSFORM(mTransformation,aPosition,position);

    return edgeTo(position);    
}


// Draws a line to a position relative to the previous one.
bool ShapeMaker::lineToRelative(const VECTOR2D &aPosition)
{
    VECTOR2D position;
    VECTOR2D_SETX(position,VECTOR2D_GETX(mCurrentPoint) + VECTOR2D_GETX(aPosition));
    VECTOR2D_SETY(position,VECTOR2D_GETY(mCurrentPoint) + VECTOR2D_GETY(aPosition));

    return lineToAbsolute(position);
}


// Draws a horizontal line to the new x coordinate.
bool ShapeMaker::horizontalLineToAbsolute(RATIONAL aX)
{
    VECTOR2D newPoint;
    VECTOR2D_SETX(newPoint,aX);
    VECTOR2D_SETY(newPoint,VECTOR2D_GETY(mCurrentPoint));

    return lineToAbsolute(newPoint);
}


// Draws a horizontal line to the new x coordinate, relative to the previous point.
bool ShapeMaker::horizontalLineToRelative(RATIONAL aX)
{
    VECTOR2D newPoint;
    VECTOR2D_SETX(newPoint,aX);
    VECTOR2D_SETY(newPoint,INT_TO_RATIONAL(0));

    return lineToRelative(newPoint);
}


// Draws a vertical line to the new y coordinate.
bool ShapeMaker::verticalLineToAbsolute(RATIONAL aY)
{
    VECTOR2D newPoint;
    VECTOR2D_SETX(newPoint,VECTOR2D_GETX(mCurrentPoint));
    VECTOR2D_SETY(newPoint,aY);

    return lineToAbsolute(newPoint);
}


// Draws a vertical line to the new y coordinate, relative to the previous point.
bool ShapeMaker::verticalLineToRelative(RATIONAL aY)
{
    VECTOR2D newPoint;
    VECTOR2D_SETX(newPoint,INT_TO_RATIONAL(0));
    VECTOR2D_SETY(newPoint,aY);

    return lineToRelative(newPoint);
}


// Draws a cubic bezier curve to point aPosition, using aPoint1 and aPoint2 as control points.
bool ShapeMaker::curveToAbsolute(const VECTOR2D &aPoint1, const VECTOR2D &aPoint2, const VECTOR2D &aPosition)
{
    // P(t) = (1 - t)^3P0 + 3(1-t)^2tP1 + 3(1-t)t^2P2 + t^3P3

    VECTOR2D p0, p1, p2, p3;
    MATRIX2D_TRANSFORM(mTransformation,mCurrentPoint,p0);
    MATRIX2D_TRANSFORM(mTransformation,aPoint1,p1);
    MATRIX2D_TRANSFORM(mTransformation,aPoint2,p2);
    MATRIX2D_TRANSFORM(mTransformation,aPosition,p3);

    RATIONAL step = INT_TO_RATIONAL(1) / mBezierSteps;
    RATIONAL t = step;
    RATIONAL it = INT_TO_RATIONAL(1) - t;
    int n;
    for (n = 0; n < mBezierSteps - 1; n++)
    {
        VECTOR2D pos;
        RATIONAL it2 = it * it;
        RATIONAL it3 = it2 * it;
        RATIONAL t2 = t * t;
        RATIONAL t3 = t2 * t;

        VECTOR2D_SETX(pos,it3 * VECTOR2D_GETX(p0) +
                           INT_TO_RATIONAL(3) * it2 * t * VECTOR2D_GETX(p1) +
                           INT_TO_RATIONAL(3) * it * t2 * VECTOR2D_GETX(p2) +
                           t3 * VECTOR2D_GETX(p3));
        VECTOR2D_SETY(pos,it3 * VECTOR2D_GETY(p0) + 
                           INT_TO_RATIONAL(3) * it2 * t * VECTOR2D_GETY(p1) +
                           INT_TO_RATIONAL(3) * it * t2 * VECTOR2D_GETY(p2) +
                           t3 * VECTOR2D_GETY(p3));

        if (!edgeTo(pos))
            return false;

        t += step;
        it -= step;
    }

    if (!edgeTo(p3))
        return false;

    mCurrentPoint = aPosition;
    mCurrentControlPoint = aPoint2;

    return true;
}


// Draws a cubic bezier curve to point aPosition, using aPoint1 and aPoint2 as control points.
bool ShapeMaker::curveToRelative(const VECTOR2D &aPoint1, const VECTOR2D &aPoint2, const VECTOR2D &aPosition)
{
    VECTOR2D p1, p2, p3;
    VECTOR2D_SETX(p1, VECTOR2D_GETX(mCurrentPoint) + VECTOR2D_GETX(aPoint1));
    VECTOR2D_SETY(p1, VECTOR2D_GETY(mCurrentPoint) + VECTOR2D_GETY(aPoint1));
    VECTOR2D_SETX(p2, VECTOR2D_GETX(mCurrentPoint) + VECTOR2D_GETX(aPoint2));
    VECTOR2D_SETY(p2, VECTOR2D_GETY(mCurrentPoint) + VECTOR2D_GETY(aPoint2));
    VECTOR2D_SETX(p3, VECTOR2D_GETX(mCurrentPoint) + VECTOR2D_GETX(aPosition));
    VECTOR2D_SETY(p3, VECTOR2D_GETY(mCurrentPoint) + VECTOR2D_GETY(aPosition));

    return curveToAbsolute(p1,p2,p3);
}


// Draws a cubic bezier curve to point aPosition, mirroring the first control point
// from the previous curveTo command and using aPoint2 as second control point.
bool ShapeMaker::smoothCurveToAbsolute(const VECTOR2D &aPoint2, const VECTOR2D &aPosition)
{
    VECTOR2D point1;
    VECTOR2D_SETX(point1, INT_TO_RATIONAL(2) * VECTOR2D_GETX(mCurrentPoint) -
                           VECTOR2D_GETX(mCurrentControlPoint));
    VECTOR2D_SETY(point1, INT_TO_RATIONAL(2) * VECTOR2D_GETY(mCurrentPoint) -
                           VECTOR2D_GETY(mCurrentControlPoint));

    return curveToAbsolute(point1,aPoint2,aPosition);
}


// Draws a cubic bezier curve to point aPosition, mirroring the first control point
// from the previous curveTo command and using aPoint2 as second control point.
bool ShapeMaker::smoothCurveToRelative(const VECTOR2D &aPoint2, const VECTOR2D &aPosition)
{
    VECTOR2D p2, p3;
    VECTOR2D_SETX(p2, VECTOR2D_GETX(mCurrentPoint) + VECTOR2D_GETX(aPoint2));
    VECTOR2D_SETY(p2, VECTOR2D_GETY(mCurrentPoint) + VECTOR2D_GETY(aPoint2));
    VECTOR2D_SETX(p3, VECTOR2D_GETX(mCurrentPoint) + VECTOR2D_GETX(aPosition));
    VECTOR2D_SETY(p3, VECTOR2D_GETY(mCurrentPoint) + VECTOR2D_GETY(aPosition));

    return smoothCurveToAbsolute(p2,p3);
}


// Draws a quadratic bezier curve to point aPosition, using aPoint as the control point.
bool ShapeMaker::quadraticCurveToAbsolute(const VECTOR2D &aPoint, const VECTOR2D &aPosition)
{
    // P(t) = (1 - t)^2P0 + 2(1-t)tP1 + t^2P2

    VECTOR2D p0, p1, p2;
    MATRIX2D_TRANSFORM(mTransformation,mCurrentPoint,p0);
    MATRIX2D_TRANSFORM(mTransformation,aPoint,p1);
    MATRIX2D_TRANSFORM(mTransformation,aPosition,p2);

    RATIONAL step = INT_TO_RATIONAL(1) / mBezierSteps;
    RATIONAL t = step;
    RATIONAL it = INT_TO_RATIONAL(1) - t;
    int n;
    for (n = 0; n < mBezierSteps - 1; n++)
    {
        VECTOR2D pos;
        RATIONAL it2 = it * it;
        RATIONAL t2 = t * t;

        VECTOR2D_SETX(pos,it2 * VECTOR2D_GETX(p0) +
                           INT_TO_RATIONAL(2) * it * t * VECTOR2D_GETX(p1) +
                           t2 * VECTOR2D_GETX(p2));
        VECTOR2D_SETY(pos,it2 * VECTOR2D_GETY(p0) +
                           INT_TO_RATIONAL(2) * it * t * VECTOR2D_GETY(p1) +
                           t2 * VECTOR2D_GETY(p2));

        if (!edgeTo(pos))
            return false;

        t += step;
        it -= step; 
    }

    if (!edgeTo(p2))
        return false;

    mCurrentPoint = aPosition;
    mCurrentControlPoint = aPoint;

    return true;
}


// Draws a quadratic bezier curve to point aPosition, using aPoint as the control point.
bool ShapeMaker::quadraticCurveToRelative(const VECTOR2D &aPoint, const VECTOR2D &aPosition)
{
    VECTOR2D p1, p2;
    VECTOR2D_SETX(p1, VECTOR2D_GETX(mCurrentPoint) + VECTOR2D_GETX(aPoint));
    VECTOR2D_SETY(p1, VECTOR2D_GETY(mCurrentPoint) + VECTOR2D_GETY(aPoint));
    VECTOR2D_SETX(p2, VECTOR2D_GETX(mCurrentPoint) + VECTOR2D_GETX(aPosition));
    VECTOR2D_SETY(p2, VECTOR2D_GETY(mCurrentPoint) + VECTOR2D_GETY(aPosition));

    return quadraticCurveToAbsolute(p1,p2);
}


// Draws a quadratic bezier curve to point aPosition, mirroring the control point from the 
// previous curveTo command.
bool ShapeMaker::smoothQuadraticCurveToAbsolute(const VECTOR2D &aPosition)
{
    VECTOR2D point;
    VECTOR2D_SETX(point, INT_TO_RATIONAL(2) * VECTOR2D_GETX(mCurrentPoint) -
                          VECTOR2D_GETX(mCurrentControlPoint));
    VECTOR2D_SETY(point, INT_TO_RATIONAL(2) * VECTOR2D_GETY(mCurrentPoint) -
                          VECTOR2D_GETY(mCurrentControlPoint));

    return quadraticCurveToAbsolute(point,aPosition);
}


// Draws a quadratic bezier curve to point aPosition, mirroring the control point from the
// previous curveTo command.
bool ShapeMaker::smoothQuadraticCurveToRelative(const VECTOR2D &aPosition)
{
    VECTOR2D p;
    VECTOR2D_SETX(p, VECTOR2D_GETX(mCurrentPoint) + VECTOR2D_GETX(aPosition));
    VECTOR2D_SETY(p, VECTOR2D_GETY(mCurrentPoint) + VECTOR2D_GETY(aPosition));

    return smoothQuadraticCurveToAbsolute(p);
}


// Closes the path being drawn.
bool ShapeMaker::closePath()
{
    mCurrentPoint = mStartPoint;
    mCurrentControlPoint = mStartPoint;

    return mShapeData->endSubPolygon();
}


// Sets the amount of bezier steps.
void ShapeMaker::setBezierSteps(int aSteps)
{
    mBezierSteps = aSteps;
}


//! Constructor.
ShapeMaker::ShapeData::ShapeData()
{
    mPolygonCount = 0;
    mShapes = NULL;
    mPolygons = NULL;
    mColors = NULL;
    mFillModes = NULL;
}


//! Destructor.
ShapeMaker::ShapeData::~ShapeData()
{
    int n;
    for (n = 0; n < mPolygonCount; n++)
    {
        delete mShapes[n];
        delete mPolygons[n];
    }
    delete[] mShapes;
    delete[] mPolygons;
    delete[] mColors;
    delete[] mFillModes;
}


//! Begins a new polygon.
bool ShapeMaker::ShapeData::beginPolygon(unsigned long aColor, RENDERER_FILLMODE aFillMode)
{
    int polygonCount = mPolygonCount + 1;

    ShapeEntry **shapes = new ShapeEntry*[polygonCount];
    PolygonData **polygons = new PolygonData*[polygonCount];
    unsigned long *colors = new unsigned long[polygonCount];
    RENDERER_FILLMODE *fillModes = new RENDERER_FILLMODE[polygonCount];
    ShapeEntry *currentShape = new ShapeEntry();

    if (shapes == NULL || polygons == NULL || colors == NULL || fillModes == NULL || currentShape == NULL)
    {
        delete shapes;
        delete polygons;
        delete colors;
        delete fillModes;
        delete currentShape;
        return false;
    }

    int n;
    for (n = 0; n < mPolygonCount; n++)
    {
        shapes[n] = mShapes[n];
        polygons[n] = mPolygons[n];
        colors[n] = mColors[n];
        fillModes[n] = mFillModes[n];
    }

    delete[] mShapes;
    delete[] mPolygons;
    delete[] mColors;
    delete[] mFillModes;

    mShapes = shapes;
    mPolygons = polygons;
    mColors = colors;
    mFillModes = fillModes;

    mShapes[mPolygonCount] = currentShape;
    mPolygons[mPolygonCount] = NULL;
    mColors[mPolygonCount] = aColor;
    mFillModes[mPolygonCount] = aFillMode;

    mPolygonCount = polygonCount;

    return true;
}


//! Ends the polygon.
bool ShapeMaker::ShapeData::endPolygon()
{
    // endPolygon() already called?
    if (mPolygons[mPolygonCount-1] != NULL)
        return false;

    mPolygons[mPolygonCount-1] = mShapes[mPolygonCount-1]->createPolygonData();
    if (mPolygons[mPolygonCount-1] == NULL)
        return false;

    return true;
}


//! Starts a new sub polygon.
bool ShapeMaker::ShapeData::beginSubPolygon()
{
    return mShapes[mPolygonCount-1]->beginSubPolygon();
}


//! Ends the sub polygon.
bool ShapeMaker::ShapeData::endSubPolygon()
{
    return mShapes[mPolygonCount-1]->endSubPolygon();
}


//! Adds a vertex to current sub polygon.
bool ShapeMaker::ShapeData::addVertex(const VECTOR2D &aVertex)
{
    return mShapes[mPolygonCount-1]->addVertex(aVertex);
}


//! Returns a pointer to the polygon data.
const PolygonData * const * ShapeMaker::ShapeData::getPolygons() const
{
    return mPolygons;
}


//! Returns a pointer to the color data.
const unsigned long * ShapeMaker::ShapeData::getColors() const
{
    return mColors;
}


//! Returns a pointer to the fill mode data.
const RENDERER_FILLMODE * ShapeMaker::ShapeData::getFillModes() const
{
    return mFillModes;
}


//! Returns the polygon count.
int ShapeMaker::ShapeData::getPolygonCount() const
{
    return mPolygonCount;
}


//! Constructor.
ShapeMaker::ShapeData::ShapeEntry::ShapeEntry()
{
    mVertexData = NULL;
    mVertexCounts = NULL;
    mSubPolygonCount = 0;
}


//! Destructor.
ShapeMaker::ShapeData::ShapeEntry::~ShapeEntry()
{
    int n;
    for (n = 0; n < mSubPolygonCount; n++)
    {
        delete[] mVertexData[n];
    }
    delete[] mVertexData;
    delete[] mVertexCounts;
}


//! Creates a PolygonData from the contents of the shape entry.
/*! Note that the ShapeEntry owns the data the PolygonData refers to.
 */
PolygonData * ShapeMaker::ShapeData::ShapeEntry::createPolygonData() const
{
    return new PolygonData(mVertexData, mVertexCounts, mSubPolygonCount);
}


//! Starts a new sub polygon.
bool ShapeMaker::ShapeData::ShapeEntry::beginSubPolygon()
{
    int subPolygonCount = mSubPolygonCount + 1;
    RATIONAL **vertexData = new RATIONAL*[subPolygonCount];
    int *vertexCounts = new int[subPolygonCount];

    if (vertexData == NULL || vertexCounts == NULL)
    {
        delete[] vertexData;
        delete[] vertexCounts;
        return false;
    }

    int n;
    for (n = 0; n < mSubPolygonCount; n++)
    {
        vertexData[n] = mVertexData[n];
        vertexCounts[n] = mVertexCounts[n];
    }

    delete[] mVertexData;
    delete[] mVertexCounts;

    mVertexData = vertexData;
    mVertexCounts = vertexCounts;

    mVertexData[mSubPolygonCount] = NULL;
    mVertexCounts[mSubPolygonCount] = 0;

    mSubPolygonCount = subPolygonCount;

    return true;
}


//! Ends the sub polygon.
bool ShapeMaker::ShapeData::ShapeEntry::endSubPolygon()
{    
    int index = mSubPolygonCount - 1;
    // Does nothing if endSubPolygon is called with no data.
    if (index < 0 || mVertexCounts[index] == 0)
        return true;

    // Crop the vertex data to actual size.
    RATIONAL * vertices = new RATIONAL[mVertexCounts[index] * 2];
    if (vertices == NULL)
        return false;

    int n;
    RATIONAL * srcVertices = mVertexData[index];
    for (n = 0; n < mVertexCounts[index] * 2; n++)
    {
        vertices[n] = srcVertices[n];
    }

    delete[] srcVertices;
    mVertexData[index] = vertices;

    return true;
}


//! Adds a vertex to the shape entry.
bool ShapeMaker::ShapeData::ShapeEntry::addVertex(const VECTOR2D &aVertex)
{
    int index = mSubPolygonCount - 1;
    if (index < 0)
        return false;

    int vertexCount = mVertexCounts[index];
    RATIONAL *vertices = mVertexData[index];

    // Resize the vertex array in chunks.
    // The size will be cropped to actual size at endSubPolygon().
    if ((vertexCount & 0xff) == 0)
    {
        RATIONAL *newVertices = new RATIONAL[(vertexCount + 0x100) * 2];
        if (newVertices == NULL)
            return false;

        int n;
        for (n = 0; n < vertexCount * 2; n++)
        {
            newVertices[n] = vertices[n];
        }

        delete[] vertices;

        mVertexData[index] = newVertices;
        vertices = newVertices;
    }

    mVertexCounts[index]++;

    vertices[vertexCount * 2] = VECTOR2D_GETX(aVertex);
    vertices[vertexCount * 2 + 1] = VECTOR2D_GETY(aVertex);

    return true;
}


//! Returns a pointer to the shape data gathered since the last call to newShape().
const ShapeMaker::ShapeData * ShapeMaker::getShapeData() const
{
    return mShapeData;
}


//! Moves the internal cursor to a new position.
/*! This uses the transformed coordinates.
 */
bool ShapeMaker::moveTo(const VECTOR2D &aPosition)
{
    mInternalCursorPos = aPosition;
    mInternalCursorMove = true;
    return true;
}


//! Draws an edge from the previous position to a new position.
/*! This uses the transformed coordinates.
 */
bool ShapeMaker::edgeTo(const VECTOR2D &aPosition)
{
    if (mInternalCursorMove)
    {
        if (!mShapeData->endSubPolygon() ||
            !mShapeData->beginSubPolygon() ||
            !mShapeData->addVertex(mInternalCursorPos))
            return false;
        mInternalCursorMove = false;
    }

    if (!mShapeData->addVertex(aPosition))
        return false;

    return true;
}
