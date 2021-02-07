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


#ifndef MATRIX2D_H_INCLUDED
#define MATRIX2D_H_INCLUDED

//! A simple 2d matrix class.
class Matrix2d
{
public:
    // Makes a rotation matrix.
    void makeRotation(RATIONAL aAngle);

    // Makes a translation matrix.
    void makeTranslation(const Vector2d &aTranslation);
    
    // Makes a scaling matrix.
    void makeScaling(const Vector2d &aScale);

    // Makes an identity matrix.
    void makeIdentity();

    // Rotates the matrix by an angle
    void rotate(RATIONAL rotate);

    // Translates the matrix with a vector.
    void translate(const Vector2d &aTranslation);

    // Scales the matrix with a vector.
    void scale(const Vector2d &aScale);

    // Multiplies the matrix with another matrix.
    void multiply(const Matrix2d &aMatrix);

    // Transforms a point with the matrix.
    void transform(const Vector2d &aSource, Vector2d &aTarget) const;

    RATIONAL mMatrix[3][2];
};

#endif // !MATRIX2D_H_INCLUDED
