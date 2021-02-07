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

#include "polygon/base/Base.h"
#include "polygon/test/Test.h"
#include "polygon/svg/SVG.h"

//! Creates a standard tester for given set of renderers.
/*! Note that ownership of aRenderers and aRendererNames gets transferred.
 */
PolygonTester * TestMaker::createStandardTest(Renderer **aRenderers, char **aRendererNames, int aCount, PolygonFactory *aFactory, int aSingleTest, const char *aSVGPath, bool aDump)
{
    static const int initialCount = 29;

    int vgsCount = initialCount;

    int specialScaleIndex[initialCount];
    RATIONAL specialScale[initialCount];
    int specialScaleCount = 0;

    if (aSingleTest >= vgsCount)
        return NULL;

    if (aSingleTest >= 0)
        vgsCount = 1;

    VectorGraphic **vgs = NULL;
    char **vgsNames = NULL;
    PolygonTester *tester = NULL;
    int n,p;

    bool error = false;

    vgs = new VectorGraphic*[vgsCount];
    if (vgs == NULL)
        error = true;

    if (!error)
    {
        for (p = 0; p < vgsCount; p++)
            vgs[p] = NULL;
    }

    if (!error)
    {
        p = 0;
        n = 0;

        if (aSingleTest < 0 || aSingleTest == n)
            vgs[p++] = TestPattern::star(INT_TO_RATIONAL(110),5,
                                    VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),
                                    RENDERER_FILLMODE_EVENODD, 0xff000000,
                                    aFactory,
                                    aDump ? "star-even-odd-dump.svg" : NULL,
                                    aDump ? "star-even-odd-dump.psh" : NULL);
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgs[p++] = TestPattern::star(INT_TO_RATIONAL(110),5,
                                    VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),
                                    RENDERER_FILLMODE_NONZERO, 0xff000000,
                                    aFactory,
                                    aDump ? "star-non-zero-dump.svg" : NULL,
                                    aDump ? "star-non-zero-dump.psh" : NULL);
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgs[p++] = TestPattern::siemensStar(INT_TO_RATIONAL(110),90,INT_TO_RATIONAL(0),
                                            VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),
                                            RENDERER_FILLMODE_EVENODD, 0xff000000,
                                            aFactory,
                                            aDump ? "siemens-star-even-odd-dump.svg" : NULL,
                                            aDump ? "siemens-star-even-odd-dump.psh" : NULL);
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgs[p++] = TestPattern::siemensStar(INT_TO_RATIONAL(110),90,INT_TO_RATIONAL(0),
                                            VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),
                                            RENDERER_FILLMODE_NONZERO, 0xff000000,
                                            aFactory,
                                            aDump ? "siemens-star-non-zero-dump.svg" : NULL,
                                            aDump ? "siemens-star-non-zero-dump.psh" : NULL);
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgs[p++] = TestPattern::circles(INT_TO_RATIONAL(108), FLOAT_TO_RATIONAL(1.0f), 100, 36, 
                                        VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),
                                        RENDERER_FILLMODE_EVENODD, 0xff000000,
                                        aFactory,
                                        aDump ? "circles-even-odd-dump.svg" : NULL,
                                        aDump ? "circles-even-odd-dump.psh" : NULL);
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgs[p++] = TestPattern::circles(INT_TO_RATIONAL(108), FLOAT_TO_RATIONAL(1.0f), 100, 36,
                                        VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),
                                        RENDERER_FILLMODE_NONZERO, 0xff000000,
                                        aFactory,
                                        aDump ? "circles-non-zero-dump.svg" : NULL,
                                        aDump ? "circles-non-zero-dump.psh" : NULL);
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgs[p++] = TestPattern::randomPolygon(INT_TO_RATIONAL(110), 100, 75, INT_TO_RATIONAL(100),INT_TO_RATIONAL(101),
                                                VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),
                                                RENDERER_FILLMODE_EVENODD, 0xff000000,
                                                aFactory,
                                                aDump ? "random-polygon-even-odd-dump.svg" : NULL,
                                                aDump ? "random-polygon-even-odd-dump.psh" : NULL);
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgs[p++] = TestPattern::randomPolygon(INT_TO_RATIONAL(110), 100, 75, INT_TO_RATIONAL(100),INT_TO_RATIONAL(101),
                                                VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),
                                                RENDERER_FILLMODE_NONZERO, 0xff000000,
                                                aFactory,
                                                aDump ? "random-polygon-non-zero-dump.svg" : NULL,
                                                aDump ? "random-polygon-non-zero-dump.psh" : NULL);
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgs[p++] = TestPattern::checkers(INT_TO_RATIONAL(110),50,
                                            VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),
                                            RENDERER_FILLMODE_EVENODD, 0xff000000,
                                            aFactory,
                                            aDump ? "checkers-even-odd-dump.svg" : NULL,
                                            aDump ? "checkers-even-odd-dump.psh" : NULL);
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgs[p++] = TestPattern::checkers(INT_TO_RATIONAL(110),50,
                                            VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),
                                            RENDERER_FILLMODE_NONZERO, 0xff000000,
                                            aFactory,
                                            aDump ? "checkers-non-zero-dump.svg" : NULL,
                                            aDump ? "checkers-non-zero-dump.psh" : NULL);
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgs[p++] = TestPattern::squares(FLOAT_TO_RATIONAL(109.875f),INT_TO_RATIONAL(0),1,1,INT_TO_RATIONAL(0),9,
                                            VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),RENDERER_FILLMODE_EVENODD,
                                            0x000000,0x0f0f0f,aFactory,
                                            aDump ? "square-basic-evenodd-dump.svg" : NULL,
                                            aDump ? "square-basic-evenodd-dump.psh" : NULL);
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgs[p++] = TestPattern::squares(FLOAT_TO_RATIONAL(109.875f),INT_TO_RATIONAL(0),1,1,INT_TO_RATIONAL(0),9,
                                            VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),RENDERER_FILLMODE_NONZERO,
                                            0x000000,0x0f0f0f,aFactory,
                                            aDump ? "square-basic-nonzero-dump.svg" : NULL,
                                            aDump ? "square-basic-nonzero-dump.psh" : NULL);
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgs[p++] = TestPattern::squares(FLOAT_TO_RATIONAL(109.875f),INT_TO_RATIONAL(0),1,1,INT_TO_RATIONAL(0),900,
                                            VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),RENDERER_FILLMODE_EVENODD,
                                            0x000000,0x0f0f0f,aFactory,
                                            aDump ? "square-many-edges-evenodd-dump.svg" : NULL,
                                            aDump ? "square-many-edges-evenodd-dump.psh" : NULL);
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgs[p++] = TestPattern::squares(FLOAT_TO_RATIONAL(109.875f),INT_TO_RATIONAL(0),1,1,INT_TO_RATIONAL(0),900,
                                            VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),RENDERER_FILLMODE_NONZERO,
                                            0x000000,0x0f0f0f,aFactory,
                                            aDump ? "square-many-edges-nonzero-dump.svg" : NULL,
                                            aDump ? "square-many-edges-nonzero-dump.psh" : NULL);
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgs[p++] = TestPattern::squares(FLOAT_TO_RATIONAL(109.875f),INT_TO_RATIONAL(0),1,9,INT_TO_RATIONAL(0),1,
                                            VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),RENDERER_FILLMODE_EVENODD,
                                            0x000000,0x0f0f0f,aFactory,
                                            aDump ? "square-long-edges-evenodd-dump.svg" : NULL,
                                            aDump ? "square-long-edges-evenodd-dump.psh" : NULL);
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgs[p++] = TestPattern::squares(FLOAT_TO_RATIONAL(109.875f),INT_TO_RATIONAL(0),1,9,INT_TO_RATIONAL(0),1,
                                            VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),RENDERER_FILLMODE_NONZERO,
                                            0x000000,0x0f0f0f,aFactory,
                                            aDump ? "square-long-edges-nonzero-dump.svg" : NULL,
                                            aDump ? "square-long-edges-nonzero-dump.psh" : NULL);
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgs[p++] = TestPattern::squares(FLOAT_TO_RATIONAL(90.5f),FLOAT_TO_RATIONAL(39.5f/8.0f),8,1,FLOAT_TO_RATIONAL(0.0f),1, 
                                            VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),RENDERER_FILLMODE_EVENODD,
                                            0x000000,0xbadfab,aFactory,
                                            aDump ? "squares-overdraw-evenodd-dump.svg" : NULL,
                                            aDump ? "squares-overdraw-evenodd-dump.psh" : NULL);
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgs[p++] = TestPattern::squares(FLOAT_TO_RATIONAL(90.5f),FLOAT_TO_RATIONAL(39.5f/8.0f),8,1,FLOAT_TO_RATIONAL(0.0f),1, 
                                            VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),RENDERER_FILLMODE_NONZERO,
                                            0x000000,0xbadfab,aFactory,
                                            aDump ? "squares-overdraw-nonzero-dump.svg" : NULL,
                                            aDump ? "squares-overdraw-nonzero-dump.psh" : NULL);
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgs[p++] = TestPattern::squares(FLOAT_TO_RATIONAL(3.0f),FLOAT_TO_RATIONAL(106.0f/16.0f),32,2,FLOAT_TO_RATIONAL(-4.0f),9, 
                                            VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),RENDERER_FILLMODE_EVENODD,
                                            0x000000,0x010101,aFactory,
                                            aDump ? "squares-small-evenodd-dump.svg" : NULL,
                                            aDump ? "squares-small-evenodd-dump.psh" : NULL);
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgs[p++] = TestPattern::squares(FLOAT_TO_RATIONAL(3.0f),FLOAT_TO_RATIONAL(106.0f/16.0f),32,2,FLOAT_TO_RATIONAL(-4.0f),9, 
                                            VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),RENDERER_FILLMODE_NONZERO,
                                            0x000000,0x010101,aFactory,
                                            aDump ? "squares-small-nonzero-dump.svg" : NULL,
                                            aDump ? "squares-small-nonzero-dump.psh" : NULL);
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgs[p++] = loadSVGtoVectorGraphic(aFactory,aSVGPath,"froggy.svg",
                                              aDump ? "froggy-dump.svg" : NULL,
                                              aDump ? "froggy-dump.psh" : NULL);
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgs[p++] = loadSVGtoVectorGraphic(aFactory,aSVGPath,"froggy-simple.svg",
                                              aDump ? "froggy-simple-dump.svg" : NULL,
                                              aDump ? "froggy-simple-dump.psh" : NULL);
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgs[p++] = loadSVGtoVectorGraphic(aFactory,aSVGPath,"fire_engine.svg",
                                              aDump ? "fire_engine-dump.svg" : NULL,
                                              aDump ? "fire_engine-dump.psh" : NULL);
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgs[p++] = loadSVGtoVectorGraphic(aFactory,aSVGPath,"burning_car.svg",
                                              aDump ? "burning_car-dump.svg" : NULL,
                                              aDump ? "burning_car-dump.psh" : NULL);
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgs[p++] = loadSVGtoVectorGraphic(aFactory,aSVGPath,"human_head.svg",
                                              aDump ? "human_head-dump.svg" : NULL,
                                              aDump ? "human_head-dump.psh" : NULL);
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgs[p++] = loadSVGtoVectorGraphic(aFactory,aSVGPath,"human_anatomy.svg",
                                              aDump ? "human_anatomy-dump.svg" : NULL,
                                              aDump ? "human_anatomy-dump.psh" : NULL);
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgs[p++] = loadSVGtoVectorGraphic(aFactory,aSVGPath,"antiloop2.svg",
                                              aDump ? "antiloop-dump.svg" : NULL,
                                              aDump ? "antiloop-dump.psh" : NULL);
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgs[p++] = loadSVGtoVectorGraphic(aFactory,aSVGPath,"lorem_ipsum_compound.svg",
                                              aDump ? "lorem_ipsum_compound-dump.svg" : NULL,
                                              aDump ? "lorem_ipsum_compound-dump.psh" : NULL);
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgs[p++] = loadSVGtoVectorGraphic(aFactory,aSVGPath,"lorem_ipsum_separate.svg",
                                              aDump ? "lorem_ipsum_separate-dump.svg" : NULL,
                                              aDump ? "lorem_ipsum_separate-dump.psh" : NULL);
        n++;

        /*
        // Just some extra test files, these are not distributed with the source package so they
        // are commented out...
        if (aSingleTest < 0 || aSingleTest == n)
        {
            specialScaleIndex[specialScaleCount] = p;
            specialScale[specialScaleCount] = FLOAT_TO_RATIONAL(0.5f);
            specialScaleCount++;
            vgs[p++] = loadSVGtoVectorGraphic(aFactory,aSVGPath,"clinton.svg",
                                              aDump ? "clinton-dump.svg" : NULL,
                                              aDump ? "clinton-dump.psh" : NULL);
        }
        n++;

        if (aSingleTest < 0 || aSingleTest == n)
        {
            specialScaleIndex[specialScaleCount] = p;
            specialScale[specialScaleCount] = FLOAT_TO_RATIONAL(0.5f);
            specialScaleCount++;
            vgs[p++] = loadSVGtoVectorGraphic(aFactory,aSVGPath,"tiger.svg",
                                              aDump ? "tiger-dump.svg" : NULL,
                                              aDump ? "tiger-dump.psh" : NULL);
        }
        n++;

        if (aSingleTest < 0 || aSingleTest == n)
        {
            specialScaleIndex[specialScaleCount] = p;
            specialScale[specialScaleCount] = FLOAT_TO_RATIONAL(0.6f);
            specialScaleCount++;
            vgs[p++] = loadSVGtoVectorGraphic(aFactory,aSVGPath,"tiger_small.svg",
                                              aDump ? "tiger_small-dump.svg" : NULL,
                                              aDump ? "tiger_small-dump.psh" : NULL);
        }
        n++;

        if (aSingleTest < 0 || aSingleTest == n)
        {
            specialScaleIndex[specialScaleCount] = p;
            specialScale[specialScaleCount] = FLOAT_TO_RATIONAL(0.25f);
            specialScaleCount++;
            vgs[p++] = loadSVGtoVectorGraphic(aFactory,aSVGPath,"lion.svg",
                                              aDump ? "lion-dump.svg" : NULL,
                                              aDump ? "lion-dump.psh" : NULL);
        }
        n++;
        */

        for (n = 0; n < vgsCount; n++)
        {
            if (vgs[n] == NULL)
                error = true;
        }
    }

    if (!error)
    {
        vgsNames = new char *[vgsCount];
        if (vgsNames == NULL)
            error = true;
    }

    if (!error)
    {
        for (p = 0; p < vgsCount; p++)
            vgsNames[p] = NULL;
    }

    if (!error)
    {
        p = 0;
        n = 0;
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("star even-odd        ");
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("star non-zero        ");
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("siemens star even-odd");
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("siemens star non-zero");
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("circles even-odd     ");
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("circles non-zero     ");
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("random even-odd      ");
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("random non-zero      ");
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("checkers even-odd    ");
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("checkers non-zero    ");
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("square basic evenodd ");
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("square basic nonzero ");
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("square many edges e/o");
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("square many edges n/z");
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("square long edges e/o");
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("square long edges n/z");
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("squares overdraw e/o ");
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("squares overdraw n/z ");
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("squares small evenodd");
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("squares small nonzero");
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("froggy               ");
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("froggy simple        ");
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("fire engine          ");
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("burning car          ");
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("human head           ");
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("human anatomy        ");
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("antiloop             ");
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("lorem ipsum compound ");
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("lorem ipsum separate ");
        n++;
        /*
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("clinton              ");
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("tiger                ");
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("tiger small          ");
        n++;
        if (aSingleTest < 0 || aSingleTest == n)
            vgsNames[p++] = STRDUP("lion                 ");
        n++;
        */

        for (n = 0; n < vgsCount; n++)
        {
            if (vgsNames[n] == NULL)
                error = true;
        }
    }

    if (!error)
    {
        tester = new PolygonTester();
        if (tester == NULL || !tester->init(vgs, vgsNames, vgsCount, aRenderers, aRendererNames, aCount))
            error = true;
    }


    if (!error)
    {
        tester->setIterationCount(200);
        tester->setPivot(VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)));
        tester->setMovement(VECTOR2D(FLOAT_TO_RATIONAL(0.09f),FLOAT_TO_RATIONAL(0.0f)));
        tester->setRotationSpeed(FLOAT_TO_RATIONAL(0.001f));
        for (n = 0; n < specialScaleCount; n++)
            tester->setScale(specialScale[n],specialScaleIndex[n]);
    }
    else
    {
        for (n = 0; n < vgsCount; n++)
        {
            if (vgs)
                delete vgs[n];
            if (vgsNames)
                delete vgsNames[n];
        }
        delete vgs;
        delete vgsNames;
        delete tester;
        vgs = NULL;
        vgsNames = NULL;
        tester = NULL;
    }

    return tester;
}


//! Creates a square test set for given set of renderers.
/*! Note that ownership of aRenderers and aRendererNames gets transferred.
 */
PolygonTester * TestMaker::createSquaresTest(Renderer **aRenderers, char **aRendererNames, int aCount, PolygonFactory *aFactory)
{
    const int vgsCount = 22;

    VectorGraphic **vgs = NULL;
    char **vgsNames = NULL;
    PolygonTester *tester = NULL;
    int n,p;

    bool error = false;

    vgs = new VectorGraphic*[vgsCount];
    if (vgs == NULL)
        error = true;

    /*
    - basic has 27 edges per side, 1x overlap
    - long edges variants:
      - 9 edges per side, 3x overlap
      - 3 edges per side, 9x overlap
      - 1 edge per side, 27x overlap
    - many edges variants:
      - 81 edges per side, 1x overlap
      - 243 edges per side, 1x overlap
      - 729 edges per side, 1x overlap
      - 2187 edges per side, 1x overlap
    - overdraw variants:
      - 27 edges per side, 1x overlap, 9x overdraw
      - 27 edges per side, 1x overlap, 36x overdraw
      - 27 edges per side, 1x overlap, 81x overdraw
    */

    if (!error)
    {
        p = 0;

        vgs[p++] = TestPattern::squares(FLOAT_TO_RATIONAL(109.875f),INT_TO_RATIONAL(0),1,1,INT_TO_RATIONAL(0),27,
                                        VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),RENDERER_FILLMODE_EVENODD,
                                        0x000000,0x0f0f0f,aFactory,NULL,NULL);

        vgs[p++] = TestPattern::squares(FLOAT_TO_RATIONAL(109.875f),INT_TO_RATIONAL(0),1,3,INT_TO_RATIONAL(0),9,
                                        VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),RENDERER_FILLMODE_EVENODD,
                                        0x000000,0x0f0f0f,aFactory,NULL,NULL);
        vgs[p++] = TestPattern::squares(FLOAT_TO_RATIONAL(109.875f),INT_TO_RATIONAL(0),1,9,INT_TO_RATIONAL(0),3,
                                        VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),RENDERER_FILLMODE_EVENODD,
                                        0x000000,0x0f0f0f,aFactory,NULL,NULL);
        vgs[p++] = TestPattern::squares(FLOAT_TO_RATIONAL(109.875f),INT_TO_RATIONAL(0),1,27,INT_TO_RATIONAL(0),1,
                                        VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),RENDERER_FILLMODE_EVENODD,
                                        0x000000,0x0f0f0f,aFactory,NULL,NULL);

        vgs[p++] = TestPattern::squares(FLOAT_TO_RATIONAL(109.875f),INT_TO_RATIONAL(0),1,1,INT_TO_RATIONAL(0),81,
                                        VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),RENDERER_FILLMODE_EVENODD,
                                        0x000000,0x0f0f0f,aFactory,NULL,NULL);
        vgs[p++] = TestPattern::squares(FLOAT_TO_RATIONAL(109.875f),INT_TO_RATIONAL(0),1,1,INT_TO_RATIONAL(0),243,
                                        VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),RENDERER_FILLMODE_EVENODD,
                                        0x000000,0x0f0f0f,aFactory,NULL,NULL);
        vgs[p++] = TestPattern::squares(FLOAT_TO_RATIONAL(109.875f),INT_TO_RATIONAL(0),1,1,INT_TO_RATIONAL(0),729,
                                        VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),RENDERER_FILLMODE_EVENODD,
                                        0x000000,0x0f0f0f,aFactory,NULL,NULL);
        vgs[p++] = TestPattern::squares(FLOAT_TO_RATIONAL(109.875f),INT_TO_RATIONAL(0),1,1,INT_TO_RATIONAL(0),2187,
                                        VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),RENDERER_FILLMODE_EVENODD,
                                        0x000000,0x0f0f0f,aFactory,NULL,NULL);

        vgs[p++] = TestPattern::squares(FLOAT_TO_RATIONAL(109.875f),INT_TO_RATIONAL(0),3,1,INT_TO_RATIONAL(0),27,
                                        VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),RENDERER_FILLMODE_EVENODD,
                                        0x000000,0x000000,aFactory,NULL,NULL);
        vgs[p++] = TestPattern::squares(FLOAT_TO_RATIONAL(109.875f),INT_TO_RATIONAL(0),6,1,INT_TO_RATIONAL(0),27,
                                        VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),RENDERER_FILLMODE_EVENODD,
                                        0x000000,0x000000,aFactory,NULL,NULL);
        vgs[p++] = TestPattern::squares(FLOAT_TO_RATIONAL(109.875f),INT_TO_RATIONAL(0),9,1,INT_TO_RATIONAL(0),27,
                                        VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),RENDERER_FILLMODE_EVENODD,
                                        0x000000,0x000000,aFactory,NULL,NULL);

        vgs[p++] = TestPattern::squares(FLOAT_TO_RATIONAL(109.875f),INT_TO_RATIONAL(0),1,1,INT_TO_RATIONAL(0),27,
                                        VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),RENDERER_FILLMODE_NONZERO,
                                        0x000000,0x0f0f0f,aFactory,NULL,NULL);

        vgs[p++] = TestPattern::squares(FLOAT_TO_RATIONAL(109.875f),INT_TO_RATIONAL(0),1,3,INT_TO_RATIONAL(0),9,
                                        VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),RENDERER_FILLMODE_NONZERO,
                                        0x000000,0x0f0f0f,aFactory,NULL,NULL);
        vgs[p++] = TestPattern::squares(FLOAT_TO_RATIONAL(109.875f),INT_TO_RATIONAL(0),1,9,INT_TO_RATIONAL(0),3,
                                        VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),RENDERER_FILLMODE_NONZERO,
                                        0x000000,0x0f0f0f,aFactory,NULL,NULL);
        vgs[p++] = TestPattern::squares(FLOAT_TO_RATIONAL(109.875f),INT_TO_RATIONAL(0),1,27,INT_TO_RATIONAL(0),1,
                                        VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),RENDERER_FILLMODE_NONZERO,
                                        0x000000,0x0f0f0f,aFactory,NULL,NULL);

        vgs[p++] = TestPattern::squares(FLOAT_TO_RATIONAL(109.875f),INT_TO_RATIONAL(0),1,1,INT_TO_RATIONAL(0),81,
                                        VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),RENDERER_FILLMODE_NONZERO,
                                        0x000000,0x0f0f0f,aFactory,NULL,NULL);
        vgs[p++] = TestPattern::squares(FLOAT_TO_RATIONAL(109.875f),INT_TO_RATIONAL(0),1,1,INT_TO_RATIONAL(0),243,
                                        VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),RENDERER_FILLMODE_NONZERO,
                                        0x000000,0x0f0f0f,aFactory,NULL,NULL);
        vgs[p++] = TestPattern::squares(FLOAT_TO_RATIONAL(109.875f),INT_TO_RATIONAL(0),1,1,INT_TO_RATIONAL(0),729,
                                        VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),RENDERER_FILLMODE_NONZERO,
                                        0x000000,0x0f0f0f,aFactory,NULL,NULL);
        vgs[p++] = TestPattern::squares(FLOAT_TO_RATIONAL(109.875f),INT_TO_RATIONAL(0),1,1,INT_TO_RATIONAL(0),2187,
                                        VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),RENDERER_FILLMODE_NONZERO,
                                        0x000000,0x0f0f0f,aFactory,NULL,NULL);

        vgs[p++] = TestPattern::squares(FLOAT_TO_RATIONAL(109.875f),INT_TO_RATIONAL(0),3,1,INT_TO_RATIONAL(0),27,
                                        VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),RENDERER_FILLMODE_NONZERO,
                                        0x000000,0x000000,aFactory,NULL,NULL);
        vgs[p++] = TestPattern::squares(FLOAT_TO_RATIONAL(109.875f),INT_TO_RATIONAL(0),6,1,INT_TO_RATIONAL(0),27,
                                        VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),RENDERER_FILLMODE_NONZERO,
                                        0x000000,0x000000,aFactory,NULL,NULL);
        vgs[p++] = TestPattern::squares(FLOAT_TO_RATIONAL(109.875f),INT_TO_RATIONAL(0),9,1,INT_TO_RATIONAL(0),27,
                                        VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),RENDERER_FILLMODE_NONZERO,
                                        0x000000,0x000000,aFactory,NULL,NULL);

        for (n = 0; n < vgsCount; n++)
        {
            if (vgs[n] == NULL)
                error = true;
        }
    }

    if (!error)
    {
        vgsNames = new char *[vgsCount];
        if (vgsNames == NULL)
            error = true;
    }

    if (!error)
    {
        p = 0;
        vgsNames[p++] = STRDUP("1x1x27 evenodd       ");
        vgsNames[p++] = STRDUP("1x3x9 evenodd        ");
        vgsNames[p++] = STRDUP("1x9x3 evenodd        ");
        vgsNames[p++] = STRDUP("1x27x1 evenodd       ");
        vgsNames[p++] = STRDUP("1x1x81 evenodd       ");
        vgsNames[p++] = STRDUP("1x1x243 evenodd      ");
        vgsNames[p++] = STRDUP("1x1x729 evenodd      ");
        vgsNames[p++] = STRDUP("1x1x2187 evenodd     ");
        vgsNames[p++] = STRDUP("9x1x27 evenodd       ");
        vgsNames[p++] = STRDUP("36x1x27 evenodd      ");
        vgsNames[p++] = STRDUP("81x1x27 evenodd      ");

        vgsNames[p++] = STRDUP("1x1x27 nonzero       ");
        vgsNames[p++] = STRDUP("1x3x9 nonzero        ");
        vgsNames[p++] = STRDUP("1x9x3 nonzero        ");
        vgsNames[p++] = STRDUP("1x27x1 nonzero       ");
        vgsNames[p++] = STRDUP("1x1x81 nonzero       ");
        vgsNames[p++] = STRDUP("1x1x243 nonzero      ");
        vgsNames[p++] = STRDUP("1x1x729 nonzero      ");
        vgsNames[p++] = STRDUP("1x1x2187 nonzero     ");
        vgsNames[p++] = STRDUP("9x1x27 nonzero       ");
        vgsNames[p++] = STRDUP("36x1x27 nonzero      ");
        vgsNames[p++] = STRDUP("81x1x27 nonzero      ");


        for (n = 0; n < vgsCount; n++)
        {
            if (vgsNames[n] == NULL)
                error = true;
        }
    }

    if (!error)
    {
        tester = new PolygonTester();
        if (tester == NULL || !tester->init(vgs, vgsNames, vgsCount, aRenderers, aRendererNames, aCount))
            error = true;
    }


    if (!error)
    {
        tester->setIterationCount(200);
        tester->setPivot(VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)));
        tester->setMovement(VECTOR2D(FLOAT_TO_RATIONAL(0.09f),FLOAT_TO_RATIONAL(0.0f)));
        tester->setRotationSpeed(FLOAT_TO_RATIONAL(0.001f));
    }
    else
    {
        for (n = 0; n < vgsCount; n++)
        {
            if (vgs)
                delete vgs[n];
            if (vgsNames)
                delete vgsNames[n];
        }
        delete vgs;
        delete vgsNames;
        delete tester;
        vgs = NULL;
        vgsNames = NULL;
        tester = NULL;
    }

    return tester;
}


//! Creates a quality tester for given set of renderers.
/*! Note that ownership of aRenderers and aRendererNames gets transferred.
 */
PolygonTester * TestMaker::createQualityTest(Renderer **aRenderers, char **aRendererNames, int aCount, PolygonFactory *aFactory, const char *aSVGPath)
{
    const int vgsCount = 4;

    VectorGraphic **vgs = NULL;
    char **vgsNames = NULL;
    PolygonTester *tester = NULL;
    int n,p;

    bool error = false;

    vgs = new VectorGraphic*[vgsCount];
    if (vgs == NULL)
        error = true;

    if (!error)
    {
        p = 0;

        vgs[p++] = TestPattern::filledStar(INT_TO_RATIONAL(110),60,INT_TO_RATIONAL(15),FLOAT_TO_RATIONAL(0.0f*3.1415926535897932384626433832795f/180.0f),
                                          VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)),
                                          RENDERER_FILLMODE_EVENODD, 0xff000000,
                                          aFactory,NULL,NULL);
        vgs[p++] = loadSVGtoVectorGraphic(aFactory,aSVGPath,"froggy.svg",NULL,NULL);
        vgs[p++] = loadSVGtoVectorGraphic(aFactory,aSVGPath,"froggy-simple.svg",NULL,NULL);
        vgs[p++] = loadSVGtoVectorGraphic(aFactory,aSVGPath,"lorem_ipsum_separate.svg",NULL,NULL);

        for (n = 0; n < vgsCount; n++)
        {
            if (vgs[n] == NULL)
                error = true;
        }
    }

    if (!error)
    {
        vgsNames = new char *[vgsCount];
        if (vgsNames == NULL)
            error = true;
    }

    if (!error)
    {
        p = 0;
        vgsNames[p++] = STRDUP("filled star even-odd ");
        vgsNames[p++] = STRDUP("froggy               ");
        vgsNames[p++] = STRDUP("froggy simple        ");
        vgsNames[p++] = STRDUP("lorem ipsum separate ");

        for (n = 0; n < vgsCount; n++)
        {
            if (vgsNames[n] == NULL)
                error = true;
        }
    }

    if (!error)
    {
        tester = new PolygonTester();
        if (tester == NULL || !tester->init(vgs, vgsNames, vgsCount, aRenderers, aRendererNames, aCount))
            error = true;
    }


    if (!error)
    {
        tester->setIterationCount(10);
        tester->setPivot(VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)));
        tester->setMovement(VECTOR2D(FLOAT_TO_RATIONAL(0.0f),FLOAT_TO_RATIONAL(0.0f)));
        tester->setRotationSpeed(FLOAT_TO_RATIONAL(0.0f));
    }
    else
    {
        for (n = 0; n < vgsCount; n++)
        {
            if (vgs)
                delete vgs[n];
            if (vgsNames)
                delete vgsNames[n];
        }
        delete vgs;
        delete vgsNames;
        delete tester;
        vgs = NULL;
        vgsNames = NULL;
        tester = NULL;
    }

    return tester;
}


//! Creates a test set with Zack Rusin's QT test polygons (from http://ktown.kde.org/~zrusin/complex.data)
PolygonTester * TestMaker::createQTPolygonTest(Renderer **aRenderers, char **aRendererNames, int aCount, PolygonFactory *aFactory)
{
    const int vgsCount = 6;

    VectorGraphic **vgs = NULL;
    char **vgsNames = NULL;
    PolygonTester *tester = NULL;
    int n,p;

    bool error = false;

    vgs = new VectorGraphic*[vgsCount];
    if (vgs == NULL)
        error = true;

    if (!error)
    {
        p = 0;

        vgs[p++] = TestPattern::qtPolygon(0,RENDERER_FILLMODE_EVENODD,0xff000000,aFactory,NULL,NULL);
        vgs[p++] = TestPattern::qtPolygon(0,RENDERER_FILLMODE_NONZERO,0xff000000,aFactory,NULL,NULL);
    
        vgs[p++] = TestPattern::qtPolygon(1,RENDERER_FILLMODE_EVENODD,0xff000000,aFactory,NULL,NULL);
        vgs[p++] = TestPattern::qtPolygon(1,RENDERER_FILLMODE_NONZERO,0xff000000,aFactory,NULL,NULL);

        vgs[p++] = TestPattern::qtPolygon(2,RENDERER_FILLMODE_EVENODD,0xff000000,aFactory,NULL,NULL);
        vgs[p++] = TestPattern::qtPolygon(2,RENDERER_FILLMODE_NONZERO,0xff000000,aFactory,NULL,NULL);

        for (n = 0; n < vgsCount; n++)
        {
            if (vgs[n] == NULL)
                error = true;
        }
    }

    if (!error)
    {
        vgsNames = new char *[vgsCount];
        if (vgsNames == NULL)
            error = true;
    }

    if (!error)
    {
        p = 0;
        vgsNames[p++] = STRDUP("QT Polygon 1 e/o fill");
        vgsNames[p++] = STRDUP("QT Polygon 1 n/z fill");
        vgsNames[p++] = STRDUP("QT Polygon 2 e/o fill");
        vgsNames[p++] = STRDUP("QT Polygon 2 n/z fill");
        vgsNames[p++] = STRDUP("QT Polygon 3 e/o fill");
        vgsNames[p++] = STRDUP("QT Polygon 3 n/z fill");

        for (n = 0; n < vgsCount; n++)
        {
            if (vgsNames[n] == NULL)
                error = true;
        }
    }

    if (!error)
    {
        tester = new PolygonTester();
        if (tester == NULL || !tester->init(vgs, vgsNames, vgsCount, aRenderers, aRendererNames, aCount))
            error = true;
    }

    for (n = 0; n < vgsCount; n++)
    {
        tester->setScale(FLOAT_TO_RATIONAL(0.5f),n);
    }

    if (!error)
    {
        tester->setIterationCount(200);
        tester->setPivot(VECTOR2D(INT_TO_RATIONAL(110),INT_TO_RATIONAL(110)));
        tester->setMovement(VECTOR2D(FLOAT_TO_RATIONAL(0.09f),FLOAT_TO_RATIONAL(0.0f)));
        tester->setRotationSpeed(FLOAT_TO_RATIONAL(0.001f));
    }
    else
    {
        for (n = 0; n < vgsCount; n++)
        {
            if (vgs)
                delete vgs[n];
            if (vgsNames)
                delete vgsNames[n];
        }
        delete vgs;
        delete vgsNames;
        delete tester;
        vgs = NULL;
        vgsNames = NULL;
        tester = NULL;
    }

    return tester;
}

