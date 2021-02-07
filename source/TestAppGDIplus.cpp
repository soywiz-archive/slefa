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
 * \brief Test application for GDI+
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */

#define UNICODE
#include <stdio.h>
#include <windows.h>
#include <gdiplus.h>

#include "polygon/base/Base.h"
#include "polygon/test/Test.h"
#include "polygon/GDIplus/GDIplus.h"

static bool isDying = false;
static Bitmap * doubleBuffer = NULL;
static PolygonTester * tester = NULL;

#define ANTIALIASED_RENDERING

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

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
   UINT  num = 0;          // number of image encoders
   UINT  size = 0;         // size of the image encoder array in bytes

   ImageCodecInfo* pImageCodecInfo = NULL;

   GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;  // Failure

   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1;  // Failure

   GetImageEncoders(num, size, pImageCodecInfo);

   for(UINT j = 0; j < num; ++j)
   {
      if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;  // Success
      }    
   }

   free(pImageCodecInfo);
   return -1;  // Failure
}


// VOID OnTimer(HWND hWnd)
VOID OnPaint(HWND hWnd)
{
    HDC       hDC;

    if(hDC = GetDC(hWnd))
    {
        int testCount = tester->getTestCount();

        // if (testCount >= 30)
        // {
        //    isDying = true;
        //    ::PostQuitMessage(0);
        // }

        Graphics doubleBufferGraphics(doubleBuffer);
        Graphics graphics(hDC);
    
#ifdef ANTIALIASED_RENDERING
        doubleBufferGraphics.SetSmoothingMode(SmoothingModeHighQuality);
#else
        doubleBufferGraphics.SetSmoothingMode(SmoothingModeHighSpeed);
#endif

        doubleBufferGraphics.Clear(Color(255, 255, 255, 255));    

        // Run the tests
        GDIplusRenderTargetWrapper wrapper(&doubleBufferGraphics);
        tester->runTest(&wrapper);

#ifdef SAVE_BITMAPS
        int newCount = tester->getTestCount();
        if (testCount != newCount)
        {
            // The count has changed -> this is the first frame of a new test.
            WCHAR buffer[32];
            swprintf(buffer,L"Test%03d.bmp",testCount);

            CLSID bmpClsid;
            GetEncoderClsid(L"image/bmp", &bmpClsid);
            doubleBuffer->Save(buffer, &bmpClsid, NULL);
        }
#endif

        // Scale the output to the rendered windows. Don't downscale, keep the aspect ratio.
        RECT rect;
        GetClientRect(hWnd,&rect);

        graphics.SetInterpolationMode(InterpolationModeNearestNeighbor );

        int w = rect.bottom * WINDOW_WIDTH / WINDOW_HEIGHT;
        int h;

        if (w > rect.right)
        {
            h = rect.right * WINDOW_HEIGHT / WINDOW_WIDTH;
            w = h * WINDOW_WIDTH / WINDOW_HEIGHT;
        }
        else
        {
            h = w * WINDOW_HEIGHT / WINDOW_WIDTH;
        }

        if (w < WINDOW_WIDTH || h < WINDOW_HEIGHT)
        {
            w = WINDOW_WIDTH;
            h = WINDOW_HEIGHT;
        }

        int x = (rect.right - w) / 2;
        int y = (rect.bottom - h) / 2;

        FontFamily  fontFamily(L"Courier New");
        Font        font(&fontFamily, 12, FontStyleBold, UnitPixel);
        SolidBrush  solidBrush(Color(255, 255, 0, 0));

        doubleBufferGraphics.ResetTransform();
        doubleBufferGraphics.SetTextRenderingHint(TextRenderingHintAntiAlias);

        char buffer[200];
        unsigned short bufferL[200];
        int count = tester->getTestCount();

        int yOffset = 15;
        int maxY = 16*count;
        if (maxY > WINDOW_HEIGHT)
            yOffset = WINDOW_HEIGHT - maxY + 15;

        int n;
        for (n = 0; n < count; n++)
        {
            float ftime = (float)tester->getTiming(n) / (float)(tester->getCount(n) * 1000);
            const char *fillerName = tester->getFillerName(n);
            const char *vgName = tester->getVectorGraphicName(n);

            if (fillerName && vgName)
            {
                sprintf(buffer,"%s: %s %f ms",fillerName,vgName,ftime);
                char *p = buffer;
                unsigned short *pL = bufferL;
                while (*p)
                    *pL++ = *p++;
                *pL = 0;

//            printer.print(NULL,5,yOffset + 16*n,buffer);
                PointF      pointF(5.0f, (float)(yOffset + 16*n));
                doubleBufferGraphics.DrawString(bufferL, -1, &font, pointF, &solidBrush);
            }
        }
        
//        doubleBufferGraphics.DrawString(L"Hello", -1, &font, pointF, &solidBrush);

        graphics.DrawImage(doubleBuffer,x,y,w,h);
    }
    ReleaseDC(hWnd,hDC);
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
    HWND                hWnd;
    MSG                 msg;
    WNDCLASS            wndClass;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    
    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    
    wndClass.style          = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc    = WndProc;
    wndClass.cbClsExtra     = 0;
    wndClass.cbWndExtra     = 0;
    wndClass.hInstance      = hInstance;
    wndClass.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName   = NULL;
    wndClass.lpszClassName  = TEXT("PerformanceTest");
    
    RegisterClass(&wndClass);

    doubleBuffer = new Bitmap(WINDOW_WIDTH,WINDOW_HEIGHT);

    const int fillerCount = 1;

    Renderer **fillers = new Renderer*[fillerCount];
    fillers[0] = new GDIplusRenderer();

    char **fillerNames = new char *[fillerCount];
    fillerNames[0] = strdup("GDI filler");

    GDIplusPolygonFactory factory;
#ifdef STANDARD_TEST
    tester = TestMaker::createStandardTest(fillers,fillerNames,fillerCount,&factory,-1,SVG_PATH);
#else
#ifdef QUALITY_TEST
    tester = TestMaker::createQualityTest(fillers,fillerNames,fillerCount,&factory,SVG_PATH);
#else
#ifdef SQUARE_TEST
    tester = TestMaker::createSquaresTest(fillers,fillerNames,fillerCount,&factory,SVG_PATH);
#else
#ifdef QT_POLYGON_TEST
    tester = TestMaker::createQTPolygonTest(fillers,fillerNames,fillerCount,&factory);
#endif
#endif
#endif
#endif
    tester->setScale(GLOBAL_SCALE);

    /*
    tester->setMovement(VECTOR2D(FLOAT_TO_RATIONAL(0.0f),FLOAT_TO_RATIONAL(0.0f)));
    tester->setRotationSpeed(FLOAT_TO_RATIONAL(0.0f));
    */

    hWnd = CreateWindow(
        TEXT("PerformanceTest"),   // window class name
        TEXT("GDI+ Performance Test"),  // window caption
        WS_OVERLAPPEDWINDOW,      // window style
        CW_USEDEFAULT,            // initial x position
        CW_USEDEFAULT,            // initial y position
        WINDOW_WIDTH,             // initial x size
        WINDOW_HEIGHT,            // initial y size
        NULL,                     // parent window handle
        NULL,                     // window menu handle
        hInstance,                // program instance handle
        NULL);                    // creation parameters
    
//    SetTimer(hWnd,1,10,NULL);
    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);
    
    while (!isDying)
    {
        // while(GetMessage(&msg, NULL, 0, 0))
        while (PeekMessage (&msg,NULL,0,0,PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        InvalidateRgn(hWnd,0,0);
    }

    delete doubleBuffer;
    doubleBuffer = NULL;

#ifdef SAVE_RESULTS
#ifdef ANTIALIASED_RENDERING
#if GLOBAL_SCALE == 4
    static const char *fileName = "GDI+ Test Results 1280x960 AA on.txt";
#elif GLOBAL_SCALE == 2
    static const char *fileName = "GDI+ Test Results 640x480 AA on.txt";
#elif GLOBAL_SCALE == 1
    static const char *fileName = "GDI+ Test Results 320x240 AA on.txt";
#else // GLOBAL_SCALE != [1,2,4]
    static const char *fileName = "GDI+ Test Results AA on.txt";
#endif // GLOBAL_SCALE
#else // !ANTIALIASED_RENDERING
#if GLOBAL_SCALE == 4
    static const char *fileName = "GDI+ Test Results 1280x960 AA off.txt";
#elif GLOBAL_SCALE == 2
    static const char *fileName = "GDI+ Test Results 640x480 AA off.txt";
#elif GLOBAL_SCALE == 1
    static const char *fileName = "GDI+ Test Results 320x240 AA off.txt";
#else // GLOBAL_SCALE != [1,2,4]
    static const char *fileName = "GDI+ Test Results AA off.txt";
#endif // GLOBAL_SCALE
#endif // ANTIALIASED_RENDERING

    tester->saveResults(fileName);
#endif

    delete tester;
    tester = NULL;

    GdiplusShutdown(gdiplusToken);

    return msg.wParam;
}  // WinMain

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, 
                         WPARAM wParam, LPARAM lParam)
{
    HDC          hdc;
    PAINTSTRUCT  ps;
    
    switch(message)
    {
/*    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        return 0;
    case WM_TIMER:
        OnTimer(hWnd);
        return 0;*/
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        OnPaint(hWnd);
        EndPaint(hWnd, &ps);
        return 0;
    case WM_DESTROY:
        isDying = true;
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
} // WndProc
