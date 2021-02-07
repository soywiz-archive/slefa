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
 * \brief PolygonTester is a class for running the tests for different polygon filler implementations.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */


#ifndef POLYGON_TESTER_H_INCLUDED
#define POLYGON_TESTER_H_INCLUDED

//! Runs the tests for different polygon filler implementations.
class PolygonTester
{
public:
    //! Constructor.
    PolygonTester();
    
    //! Initializer.
    /*! Note that the ownership of aVectorGraphics, aFillers, aVectorGraphicNames and
     *  aFillerNames is transferred.
     */
    bool init(VectorGraphic **aVectorGraphics, char **aVectorGraphicNames, int aVectorGraphicCount,
              Renderer **aFillers, char **aFillerNames, int aFillerCount);

    //! Destructor.
    ~PolygonTester();

    //! Returns the iteration count of the tester.
    int getIterationCount()
    {
        return mIterationCount;
    }

    //! Sets the iteration count for each test. The default value is 1000.
    void setIterationCount(int aIterationCount);

    //! Sets the pivot (rotation center) for the polygon tester.
    void setPivot(const VECTOR2D &aPivot);

    //! Sets the movement, i.e. the change of translation per tick for the tester.
    void setMovement(const VECTOR2D &aMovement);

    //! Sets the rotation speed, i.e. the change of rotation per tick for the tester.
    void setRotationSpeed(RATIONAL aRotationSpeed);

    //! Sets the scale for the transformations.
    void setScale(RATIONAL aScale);

    //! Sets the scale for the transformation of the given graphic.
    void setScale(RATIONAL aScale, int aVectorGraphic);

    //! Runs one iteration of the test. This renders the graphic to aTarget.
    void runTest(RenderTargetWrapper *aTarget);

    //! Returns the amount of tests done.
    int getTestCount();

    //! Returns the total amount of fillers.
    int getFillerCount()
    {
        return mFillerCount;
    }

    //! Returns the total amount of vector graphics.
    int getVectorGraphicCount()
    {
        return mVectorGraphicCount;
    }

    //! Returns the timing for a test at given index.
    int getTiming(int aIndex);

    //! Returns the count for a test at given index.
    int getCount(int aIndex);

    //! Returns true if the test is finsihed.
    bool isReady();

    //! Returns the vector graphic name for a test at given index.
    const char *getVectorGraphicName(int aIndex);

    //! Returns the filler name for a test at given index.
    const char *getFillerName(int aIndex);

    //! Returns the vector graphic for a test at given index.
    VectorGraphic *getVectorGraphic(int aIndex);

    //! Returns the filler for a test at given index.
    Renderer *getFiller(int aIndex);

    //! Writes the results of the tests to a file with given name.
    bool saveResults(const char *aFileName);

    //! Restarts the tests.
    void restart();

protected:
    //! Updates the transformation and calculates it to a matrix.
    void updateTransformation(MATRIX2D &aTransformation);

    //! Begins a new test.
    void beginTest(int aTestIndex);

    //! The vector graphics to be used in the test.
    VectorGraphic **mVectorGraphics;

    //! The names for the vector graphics.
    char **mVectorGraphicNames;

    //! Amount of vector graphics.
    int mVectorGraphicCount;

    //! The polygon fillers being tested.
    Renderer **mFillers;

    //! The names of the fillers.
    char **mFillerNames;

    //! Amount of fillers.
    int mFillerCount;

    //! Amount of iterations per test.
    int mIterationCount;

    //! Test result array.
    int *mTestTimings;

    //! Count of ticks per test.
    int *mTestCounts;

    //! Current test number.
    int mCurrentTest;

    //! Current iteration count.
    int mCurrentIteration;

    //! Pivot for the rotations.
    VECTOR2D mPivot;

    //! Movement, i.e. the change of rotation per tick for the tester.
    VECTOR2D mMovement;

    //! Rotation speed, i.e. the change of rotation per tick for the tester.
    RATIONAL mRotationSpeed;

    //! Current translation.
    VECTOR2D mCurrentTranslation;

    //! Current rotation.
    RATIONAL mCurrentRotation;

    //! Global scale factor.
    RATIONAL mScale;

    //! Scale factors for the vector graphics.
    RATIONAL *mScales;
};

#endif // !POLYGON_TESTER_H_INCLUDED
