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

#ifndef SHAPEMAKER_H_INCLUDED
#define SHAPEMAKER_H_INCLUDED


//! A utility class for building polygons out of SVG commands.
class ShapeMaker
{
public:
    //! Constructor.
    ShapeMaker();

    //! Destructor.
    ~ShapeMaker();

    //! Sets the transformation matrix.
    void setTransformation(const MATRIX2D &aTransformation);

    //! Sets the current color.
    /*! Current color is applied to the path at beginPath().
     */
    void setColor(unsigned int aColor);

    //! Sets the current fillrule.
    /*! Current fillrule is applied to the path at beginPath().
     */
    void setFillRule(RENDERER_FILLMODE aFillMode);

    //! Starts a new shape and clears the shape data collected this far.
    /*! This must be called as the first thing before assigning any commands to
     *  the shape maker.
     */
    bool newShape();

    //! Begins a new path.
    bool beginPath();

    //! Ends the path.
    bool endPath();

    //! Moves the draw head to an absolute position.
    bool moveToAbsolute(const VECTOR2D &aPosition);

    //! Moves the draw head to a position relative to the previous one.
    bool moveToRelative(const VECTOR2D &aPosition);

    //! Draws a line to an absolute position.
    bool lineToAbsolute(const VECTOR2D &aPosition);

    //! Draws a line to a position relative to the previous one.
    bool lineToRelative(const VECTOR2D &aPosition);

    //! Draws a horizontal line to the new x coordinate.
    bool horizontalLineToAbsolute(RATIONAL aX);

    //! Draws a horizontal line to the new x coordinate, relative to the previous point.
    bool horizontalLineToRelative(RATIONAL aX);

    //! Draws a vertical line to the new y coordinate.
    bool verticalLineToAbsolute(RATIONAL aY);

    //! Draws a vertical line to the new y coordinate, relative to the previous point.
    bool verticalLineToRelative(RATIONAL aY);

    //! Draws a cubic bezier curve to point aPosition, using aPoint1 and aPoint2 as control points.
    bool curveToAbsolute(const VECTOR2D &aPoint1, const VECTOR2D &aPoint2, const VECTOR2D &aPosition);

    //! Draws a cubic bezier curve to point aPosition, using aPoint1 and aPoint2 as control points.
    bool curveToRelative(const VECTOR2D &aPoint1, const VECTOR2D &aPoint2, const VECTOR2D &aPosition);

    //! Draws a cubic bezier curve to point aPosition, mirroring the first control point from the previous curveTo command and using aPoint2 as second control point.
    bool smoothCurveToAbsolute(const VECTOR2D &aPoint2, const VECTOR2D &aPosition);

    //! Draws a cubic bezier curve to point aPosition, mirroring the first control point from the previous curveTo command and using aPoint2 as second control point.
    bool smoothCurveToRelative(const VECTOR2D &aPoint2, const VECTOR2D &aPosition);

    //! Draws a quadratic bezier curve to point aPosition, using aPoint as the control point.
    bool quadraticCurveToAbsolute(const VECTOR2D &aPoint, const VECTOR2D &aPosition);

    //! Draws a quadratic bezier curve to point aPosition, using aPoint as the control point.
    bool quadraticCurveToRelative(const VECTOR2D &aPoint, const VECTOR2D &aPosition);

    //! Draws a quadratic bezier curve to point aPosition, mirroring the control point from the previous curveTo command.
    bool smoothQuadraticCurveToAbsolute(const VECTOR2D &aPosition);

    //! Draws a quadratic bezier curve to point aPosition, mirroring the control point from the previous curveTo command.
    bool smoothQuadraticCurveToRelative(const VECTOR2D &aPosition);

    //! Closes the path being drawn.
    bool closePath();

    //! Sets the amount of bezier steps.
    void setBezierSteps(int aSteps);

    //! Inner class for handling the path data as a set of polygons.
    class ShapeData
    {
    public:
        //! Constructor.
        ShapeData();

        //! Destructor.
        ~ShapeData();

        //! Begins a new polygon.
        bool beginPolygon(unsigned long aColor, RENDERER_FILLMODE aFillMode);

        //! Ends the polygon.
        bool endPolygon();

        //! Starts a new sub polygon.
        bool beginSubPolygon();

        //! Ends the sub polygon.
        bool endSubPolygon();

        //! Adds a vertex to current sub polygon.
        bool addVertex(const VECTOR2D &aVertex);

        //! Returns a pointer to the polygon data.
        const PolygonData * const * getPolygons() const;

        //! Returns a pointer to the color data.
        const unsigned long * getColors() const;

        //! Returns a pointer to the fill mode data.
        const RENDERER_FILLMODE * getFillModes() const;

        //! Returns the polygon count.
        int getPolygonCount() const;

    protected:
        //! Inner class for handling a single polygon in the ShapeData.
        class ShapeEntry
        {
        public:
            //! Constructor.
            ShapeEntry();

            //! Destructor.
            ~ShapeEntry();

            //! Creates a PolygonData from the contents of the shape entry.
            /*! Note that the ShapeEntry owns the data the PolygonData refers to.
             */
            PolygonData * createPolygonData() const;

            //! Starts a new sub polygon.
            bool beginSubPolygon();

            //! Ends the sub polygon.
            bool endSubPolygon();

            //! Adds a vertex to the shape entry.
            bool addVertex(const VECTOR2D &aVertex);

        protected:
            RATIONAL **mVertexData;
            int *mVertexCounts;
            int mSubPolygonCount;
        };

        ShapeEntry **mShapes;
        PolygonData **mPolygons;
        unsigned long *mColors;
        RENDERER_FILLMODE *mFillModes;
        int mPolygonCount;
    };

    //! Returns a pointer to the shape data gathered since the last call to newShape().
    const ShapeMaker::ShapeData * getShapeData() const;

protected:
    //! Moves the internal cursor to a new position.
    /*! This uses the transformed coordinates.
     */
    bool moveTo(const VECTOR2D &aPosition);

    //! Draws an edge from the previous position to a new position.
    /*! This uses the transformed coordinates.
     */
    bool edgeTo(const VECTOR2D &aPosition);

    ShapeData *mShapeData;
    MATRIX2D mTransformation;
    unsigned int mCurrentColor;
    RENDERER_FILLMODE mCurrentFillMode;
    int mBezierSteps;
    VECTOR2D mCurrentPoint;
    VECTOR2D mCurrentControlPoint;
    VECTOR2D mStartPoint;
    VECTOR2D mInternalCursorPos;
    bool mInternalCursorMove;
};

#endif // SHAPEMAKER_H_INCLUDED
