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
 * \brief Test application for AGG
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */

#include <windows.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

#include "polygon/base/Base.h"
#include "polygon/test/Test.h"
#include "polygon/AGG/AGG.h"

#include "ctrl/agg_slider_ctrl.h"
#include "platform/agg_platform_support.h"

enum flip_y_e { flip_y = false };

#define GLOBAL_SCALE 2
#define WINDOW_WIDTH (320*GLOBAL_SCALE)
#define WINDOW_HEIGHT (240*GLOBAL_SCALE)

#define STANDARD_TEST
// #define QUALITY_TEST
// #define SQUARE_TEST
// #define QT_POLYGON_TEST // Enable also "INCLUDE_QT_DATA" define in TestPattern.c!

// Toggles on saving of bitmaps.
// #define SAVE_BITMAPS

// Toggles on saving of results.
// #define SAVE_RESULTS

// Defines the svg path
#define SVG_PATH "../../../SVG/"

//! Test application for the AGG library.
class TestAppAGG : public agg::platform_support
{
    PolygonTester * mTester;
    AGGRenderTargetWrapper * mTarget;

public:
    TestAppAGG(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y)
    {
        create_img(0, WINDOW_WIDTH, WINDOW_HEIGHT);

        mTarget = new AGGRenderTargetWrapper(&rbuf_img(0));

        const int fillerCount = 1;

        Renderer **fillers = new Renderer*[fillerCount];
        fillers[0] = new AGGRenderer(mTarget);

        char **fillerNames = new char *[fillerCount];
        fillerNames[0] = strdup("AGG filler");

        AGGPolygonFactory factory(((AGGRenderer *)fillers[0])->getPathStorage());

#ifdef STANDARD_TEST
        mTester = TestMaker::createStandardTest(fillers,fillerNames,fillerCount,&factory,-1,SVG_PATH);
#else
#ifdef QUALITY_TEST
        mTester = TestMaker::createQualityTest(fillers,fillerNames,fillerCount,&factory,SVG_PATH);
#else
#ifdef SQUARE_TEST
        mTester = TestMaker::createSquaresTest(fillers,fillerNames,fillerCount,&factory,SVG_PATH);
#else
#ifdef QT_POLYGON_TEST
        mTester = TestMaker::createQTPolygonTest(fillers,fillerNames,fillerCount,&factory);
#endif
#endif
#endif
#endif
        mTester->setScale(GLOBAL_SCALE);

        /*
        mTester->setMovement(VECTOR2D(FLOAT_TO_RATIONAL(0.0f),FLOAT_TO_RATIONAL(0.0f)));
        mTester->setRotationSpeed(FLOAT_TO_RATIONAL(0.0f));
        */
    }

    virtual ~TestAppAGG()
    {
#ifdef SAVE_RESULTS
#if(GLOBAL_SCALE == 4)
        static const char *fileName = "AGG Test Results 1280x960.txt";
#elif(GLOBAL_SCALE == 2)
        static const char *fileName = "AGG Test Results 640x480.txt";
#elif(GLOBAL_SCALE == 1)
        static const char *fileName = "AGG Test Results 320x240.txt";
#else
        static const char *fileName = "AGG Test Results.txt";
#endif
        mTester->saveResults(fileName);
#endif
        delete mTester;
        delete mTarget;
    }

    virtual void on_resize(int cx, int cy)
    {
        pixfmt pf(rbuf_window());
        agg::renderer_base<pixfmt> r(pf);
        r.clear(agg::rgba(1, 1, 1));
    }

    virtual void on_draw()
    {
        int testCount = mTester->getTestCount();

        // if (testCount >= 30)
        //     ::PostQuitMessage(0);

        pixfmt pf(rbuf_img(0));
        agg::renderer_base<pixfmt> r(pf);
        r.clear(agg::rgba(1, 1, 1));

        mTester->runTest(mTarget);
        copy_img_to_window(0);

#ifdef SAVE_BITMAPS
        int newCount = mTester->getTestCount();
        if (testCount != newCount)
        {
            // The count has changed -> this is the first frame of a new test.
            char buffer[32];
            sprintf(buffer,"Test%03d",testCount);
            save_img(0,buffer);
        }
#endif

        pixfmt pfWin(rbuf_window());
        agg::renderer_base<pixfmt> rWin(pfWin);

        agg::renderer_scanline_aa_solid<agg::renderer_base<pixfmt> > ren(rWin);
        agg::scanline_p8 sl;
        agg::gsv_text txt;
        txt.flip(true);
        agg::conv_stroke<agg::gsv_text> txt_stroke(txt);
        agg::rasterizer_scanline_aa<> ras;
        txt_stroke.width(1.0);
        txt_stroke.line_cap(agg::round_cap);
        txt.size(8.0);
        ren.color(agg::rgba(1.0, 0.0, 0.0));

        char buffer[200];
        int count = mTester->getTestCount();

        int yOffset = 15;
        int maxY = 16*count;
        if (maxY > WINDOW_HEIGHT)
            yOffset = WINDOW_HEIGHT - maxY + 15;

        int n;
        for (n = 0; n < count; n++)
        {
            float ftime = (float)mTester->getTiming(n) / (float)(mTester->getCount(n) * 1000);
            const char *fillerName = mTester->getFillerName(n);
            const char *vgName = mTester->getVectorGraphicName(n);

            if (fillerName && vgName)
            {
                sprintf(buffer,"%s: %s %f ms",fillerName,vgName,ftime);

                txt.start_point(5, yOffset + 16*n);
                txt.text(buffer);
                ras.reset();
                ras.add_path(txt_stroke);
                agg::render_scanlines(ras, sl, ren);
            }
        }
    
    }

    virtual void on_post_draw(void* /*raw_handler*/)
    {
        force_redraw();
    }

};


int agg_main(int argc, char* argv[])
{
    TestAppAGG app(pix_format, flip_y);
    app.caption("AGG Performance Test");

    if(app.init(WINDOW_WIDTH, WINDOW_HEIGHT, agg::window_resize))
    {
        return app.run();
    }
    return 1;
}
