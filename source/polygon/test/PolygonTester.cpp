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

#include "polygon/base/Base.h"
#include "polygon/test/Test.h"

//! Constructor.
PolygonTester::PolygonTester()
{
    mVectorGraphics = NULL;
    mVectorGraphicCount = 0;
    mFillers = NULL;
    mFillerCount = 0;
    mTestCounts = NULL;
    mTestTimings = NULL;
    mIterationCount = 1000;
    mCurrentTest = -1;
    mPivot = VECTOR2D(INT_TO_RATIONAL(0),INT_TO_RATIONAL(0));
    mMovement = VECTOR2D(INT_TO_RATIONAL(0),INT_TO_RATIONAL(0));
    mRotationSpeed = INT_TO_RATIONAL(0);
    mScale = INT_TO_RATIONAL(1);
    mScales = NULL;
}


//! Initializer.
/*! Note that the ownership of aVectorGraphics, aFillers, aVectorGraphicNames and
 *  aFillerNames is transferred.
 */
bool PolygonTester::init(VectorGraphic **aVectorGraphics, char **aVectorGraphicNames, int aVectorGraphicCount,
                         Renderer **aFillers, char **aFillerNames, int aFillerCount)
{
    mVectorGraphics = aVectorGraphics;
    mVectorGraphicCount = aVectorGraphicCount;
    mFillers = aFillers;
    mFillerCount = aFillerCount;
    mFillerNames = aFillerNames;
    mVectorGraphicNames = aVectorGraphicNames;

    mTestCounts = new int[aFillerCount * mVectorGraphicCount];
    mTestTimings = new int[aFillerCount * mVectorGraphicCount];
    mScales = new RATIONAL[mVectorGraphicCount];

    if (mTestCounts == NULL || mTestTimings == NULL || mScales == NULL)
        return false;

    int n;
    for (n = 0; n < mVectorGraphicCount; n++)
        mScales[n] = INT_TO_RATIONAL(1);

    return true;
}


//! Destructor.
PolygonTester::~PolygonTester()
{
    int n;
    for (n = 0; n < mVectorGraphicCount; n++)
    {
        delete mVectorGraphics[n];
        delete[] mVectorGraphicNames[n];
    }
    delete[] mVectorGraphics;
    delete[] mVectorGraphicNames;
    for (n = 0; n < mFillerCount; n++)
    {
        delete mFillers[n];
        delete mFillerNames[n];
    }
    delete[] mFillers;
    delete[] mFillerNames;
    delete[] mTestCounts;
    delete[] mTestTimings;
    delete[] mScales;
}


//! Sets the iteration count for each test. The default value is 1000.
void PolygonTester::setIterationCount(int aIterationCount)
{
    mIterationCount = aIterationCount;
}


//! Sets the pivot (rotation center) for the polygon tester.
void PolygonTester::setPivot(const VECTOR2D &aPivot)
{
    mPivot = aPivot;
}


//! Sets the movement, i.e. the change of translation per tick for the tester.
void PolygonTester::setMovement(const VECTOR2D &aMovement)
{
    mMovement = aMovement;
}


//! Sets the rotation speed, i.e. the change of rotation per tick for the tester.
void PolygonTester::setRotationSpeed(RATIONAL aRotationSpeed)
{
    mRotationSpeed = aRotationSpeed;
}


//! Sets the scale for the transformations.
void PolygonTester::setScale(RATIONAL aScale)
{
    mScale = aScale;
}


//! Sets the scale for the transformation of the given graphic.
void PolygonTester::setScale(RATIONAL aScale, int aVectorGraphic)
{
    if (aVectorGraphic >= 0 && aVectorGraphic < mVectorGraphicCount)
        mScales[aVectorGraphic] = aScale;
}


//! Runs one iteration of the test. This renders the graphic to aTarget.
void PolygonTester::runTest(RenderTargetWrapper *aTarget)
{
    int maxCount = mFillerCount * mVectorGraphicCount;
    if (mCurrentTest >= maxCount)
    {
        aTarget->onRenderDone();
        return;
    }

    if (mCurrentTest < 0 || mTestCounts[mCurrentTest] > mIterationCount)
    {
        mCurrentTest++;
        if (mCurrentTest >= maxCount)
            return;
        beginTest(mCurrentTest);
    }

    int currentFiller = mCurrentTest / mVectorGraphicCount;
    int currentGraphic = mCurrentTest % mVectorGraphicCount;

    MATRIX2D transformation;
    updateTransformation(transformation);
    TICKCOUNTER counter;

    TICKCOUNTER_START(counter);
    mVectorGraphics[currentGraphic]->render(mFillers[currentFiller],aTarget,transformation);
    TICKCOUNTER_END(counter);

    mTestTimings[mCurrentTest] += TICKCOUNTER_GETTICKS(counter);
    mTestCounts[mCurrentTest]++;
}


//! Returns the amount of tests done.
int PolygonTester::getTestCount()
{
    int lastTest = mFillerCount * mVectorGraphicCount;
    return mCurrentTest < lastTest ? mCurrentTest + 1 : lastTest;
}


//! Returns the timing for a test at given index.
int PolygonTester::getTiming(int aIndex)
{
    if (aIndex <= getTestCount())
        return mTestTimings[aIndex];
    else
        return 0;
}


//! Returns the count for a test at given index.
int PolygonTester::getCount(int aIndex)
{
    if (aIndex <= getTestCount())
        return mTestCounts[aIndex];
    else
        return 0;
}


//! Returns true if the test is finsihed.
bool PolygonTester::isReady()
{
    int lastTest = mFillerCount * mVectorGraphicCount;
    return mCurrentTest == lastTest;
}


//! Returns the vector graphic name for a test at given index.
const char * PolygonTester::getVectorGraphicName(int aIndex)
{
    int graphic = aIndex % mVectorGraphicCount;
    return mVectorGraphicNames[graphic];
}


//! Returns the filler name for a test at given index.
const char * PolygonTester::getFillerName(int aIndex)
{
    int filler = aIndex / mVectorGraphicCount;
    return mFillerNames[filler];
}


//! Returns the vector graphic for a test at given index.
VectorGraphic * PolygonTester::getVectorGraphic(int aIndex)
{
    int graphic = aIndex % mVectorGraphicCount;
    return mVectorGraphics[graphic];
}


//! Returns the filler for a test at given index.
Renderer * PolygonTester::getFiller(int aIndex)
{
    int filler = aIndex / mVectorGraphicCount;
    return mFillers[filler];
}


//! Writes the results of the tests to a file with given name.
bool PolygonTester::saveResults(const char *aFileName)
{
    TEXTFILE_OBJECT *fp = TEXTFILE_OPEN_FOR_WRITE(aFileName);

    if (fp == NULL)
        return false;

    int count = getTestCount();
    int n;
    for (n = 0; n < count; n++)
    {
        RATIONAL ftime = (RATIONAL)getTiming(n) / (RATIONAL)(getCount(n) * 1000);
        const char *fillerName = getFillerName(n);
        const char *vgName = getVectorGraphicName(n);

        TEXTFILE_WRITE_STRING(fp,fillerName);
        TEXTFILE_WRITE_STRING(fp," ");
        TEXTFILE_WRITE_STRING(fp,vgName);
        TEXTFILE_WRITE_STRING(fp," ");
        TEXTFILE_WRITE_RATIONAL(fp,ftime);
        TEXTFILE_WRITE_STRING(fp,"\n");
    }

    TEXTFILE_CLOSE(fp);
    return true;
}


//! Restarts the tests.
void PolygonTester::restart()
{
    mCurrentTest = -1;
}


//! Updates the transformation and calculates it to a matrix.
void PolygonTester::updateTransformation(MATRIX2D &aTransformation)
{
    VECTOR2D negPivot(-VECTOR2D_GETX(mPivot),-VECTOR2D_GETY(mPivot));
    MATRIX2D_MAKETRANSLATION(aTransformation, negPivot);
    MATRIX2D_ROTATE(aTransformation, mCurrentRotation);
    MATRIX2D_TRANSLATE(aTransformation,mPivot);
    MATRIX2D_TRANSLATE(aTransformation,mCurrentTranslation);

    VECTOR2D scale;
    RATIONAL t = mScale * mScales[mCurrentTest % mVectorGraphicCount];
    VECTOR2D_SETX(scale,t);
    VECTOR2D_SETY(scale,t);
    MATRIX2D_SCALE(aTransformation,scale);

    VECTOR2D_SETX(mCurrentTranslation,VECTOR2D_GETX(mCurrentTranslation) + VECTOR2D_GETX(mMovement));
    VECTOR2D_SETY(mCurrentTranslation,VECTOR2D_GETY(mCurrentTranslation) + VECTOR2D_GETY(mMovement));
    mCurrentRotation += mRotationSpeed;
}


//! Begins a new test.
void PolygonTester::beginTest(int aTestIndex)
{
    if (aTestIndex < mFillerCount * mVectorGraphicCount)
    {
        mTestCounts[aTestIndex] = 0;
        mTestTimings[aTestIndex] = 0;
    }

    mCurrentIteration = 0;
    mCurrentTranslation = VECTOR2D(INT_TO_RATIONAL(5),INT_TO_RATIONAL(5));
    mCurrentRotation = INT_TO_RATIONAL(0);
}
