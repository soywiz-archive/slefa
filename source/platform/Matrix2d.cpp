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
 * \brief A simple 2d matrix class.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */

#include <math.h>
#include "polygon/base/Platform.h"
#include "platform/Vector2d.h"
#include "platform/Matrix2d.h"

// Makes a rotation matrix.
void Matrix2d::makeRotation(RATIONAL aAngle)
{
    RATIONAL sinRot = (RATIONAL)sin(aAngle);
    RATIONAL cosRot = (RATIONAL)cos(aAngle);

    mMatrix[0][0] = cosRot;
    mMatrix[0][1] = sinRot;
    mMatrix[1][0] = -sinRot;
    mMatrix[1][1] = cosRot;
    mMatrix[2][0] = INT_TO_RATIONAL(0);
    mMatrix[2][1] = INT_TO_RATIONAL(0);
}


// Makes a translation matrix.
void Matrix2d::makeTranslation(const Vector2d &aTranslation)
{
    mMatrix[0][0] = INT_TO_RATIONAL(1);
    mMatrix[0][1] = INT_TO_RATIONAL(0);
    mMatrix[1][0] = INT_TO_RATIONAL(0);
    mMatrix[1][1] = INT_TO_RATIONAL(1);
    mMatrix[2][0] = aTranslation.mX;
    mMatrix[2][1] = aTranslation.mY;
}


// Makes a scaling matrix.
void Matrix2d::makeScaling(const Vector2d &aScale)
{
    mMatrix[0][0] = aScale.mX;
    mMatrix[0][1] = INT_TO_RATIONAL(0);
    mMatrix[1][0] = INT_TO_RATIONAL(0);
    mMatrix[1][1] = aScale.mY;
    mMatrix[2][0] = INT_TO_RATIONAL(0);
    mMatrix[2][1] = INT_TO_RATIONAL(0);
}


// Makes an identity matrix.
void Matrix2d::makeIdentity()
{
    mMatrix[0][0] = INT_TO_RATIONAL(1);
    mMatrix[0][1] = INT_TO_RATIONAL(0);
    mMatrix[1][0] = INT_TO_RATIONAL(0);
    mMatrix[1][1] = INT_TO_RATIONAL(1);
    mMatrix[2][0] = INT_TO_RATIONAL(0);
    mMatrix[2][1] = INT_TO_RATIONAL(0);
}


// Rotates the matrix by an angle
void Matrix2d::rotate(RATIONAL rotate)
{
    Matrix2d tmp;
    tmp.makeRotation(rotate);
    multiply(tmp);
}


// Translates the matrix with a vector.
void Matrix2d::translate(const Vector2d &aTranslation)
{
    Matrix2d tmp;
    tmp.makeTranslation(aTranslation);
    multiply(tmp);
}


// Scales the matrix with a vector.
void Matrix2d::scale(const Vector2d &aScale)
{
    Matrix2d tmp;
    tmp.makeScaling(aScale);
    multiply(tmp);
}


// Multiplies the matrix with another matrix.
void Matrix2d::multiply(const Matrix2d &aMatrix)
{
    Matrix2d tmp = *this;
    mMatrix[0][0] = tmp.mMatrix[0][0] * aMatrix.mMatrix[0][0] +
                    tmp.mMatrix[0][1] * aMatrix.mMatrix[1][0];
    mMatrix[0][1] = tmp.mMatrix[0][0] * aMatrix.mMatrix[0][1] +
                    tmp.mMatrix[0][1] * aMatrix.mMatrix[1][1];
    mMatrix[1][0] = tmp.mMatrix[1][0] * aMatrix.mMatrix[0][0] +
                    tmp.mMatrix[1][1] * aMatrix.mMatrix[1][0];
    mMatrix[1][1] = tmp.mMatrix[1][0] * aMatrix.mMatrix[0][1] +
                    tmp.mMatrix[1][1] * aMatrix.mMatrix[1][1];
    mMatrix[2][0] = tmp.mMatrix[2][0] * aMatrix.mMatrix[0][0] +
                    tmp.mMatrix[2][1] * aMatrix.mMatrix[1][0] +
                    aMatrix.mMatrix[2][0];
    mMatrix[2][1] = tmp.mMatrix[2][0] * aMatrix.mMatrix[0][1] +
                    tmp.mMatrix[2][1] * aMatrix.mMatrix[1][1] +
                    aMatrix.mMatrix[2][1];
}


// Transforms a point with the matrix.
void Matrix2d::transform(const Vector2d &aSource, Vector2d &aTarget) const
{
    aTarget.mX = mMatrix[0][0] * aSource.mX +
                 mMatrix[1][0] * aSource.mY +
                 mMatrix[2][0];
                
    aTarget.mY = mMatrix[0][1] * aSource.mX +
                 mMatrix[1][1] * aSource.mY +
                 mMatrix[2][1];
}
