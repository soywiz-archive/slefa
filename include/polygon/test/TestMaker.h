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
 * \brief A utility class for constructing standard tests.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */


#ifndef TEST_MAKER_H_INCLUDED
#define TEST_MAKER_H_INCLUDED

//! A utility class for constructing standard tests.
class TestMaker
{
public:
    //! Creates a standard tester for given set of renderers.
    /*! Note that ownership of aRenderers and aRendererNames gets transferred.
     */
    static PolygonTester * createStandardTest(Renderer **aRenderers, char **aRendererNames, int aCount, PolygonFactory *aFactory, int aSingleTest = -1, const char *aSVGPath = NULL, bool aDump = false);

    //! Creates a square test set for given set of renderers.
    /*! Note that ownership of aRenderers and aRendererNames gets transferred.
     */
    static PolygonTester * createSquaresTest(Renderer **aRenderers, char **aRendererNames, int aCount, PolygonFactory *aFactory);

    //! Creates a quality test set for given set of renderers.
    /*! Note that ownership of aRenderers and aRendererNames gets transferred.
     */
    static PolygonTester * createQualityTest(Renderer **aRenderers, char **aRendererNames, int aCount, PolygonFactory *aFactory, const char *aSVGPath = NULL);

    //! Creates a test set with Zack Rusin's QT test polygons (from http://ktown.kde.org/~zrusin/complex.data)
    static PolygonTester * createQTPolygonTest(Renderer **aRenderers, char **aRendererNames, int aCount, PolygonFactory *aFactory);
};

#endif // !TEST_MAKER_H_INCLUDED
