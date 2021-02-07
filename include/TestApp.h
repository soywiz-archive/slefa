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
 * \brief Test application for the algorithm. Uses SDL.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */


#ifndef TESTAPP_H_INCLUDED
#define TESTAPP_H_INCLUDED

class PolygonTester;
class PolygonFiller;
class Polygon;

//! Test application for the algorithm. Uses SDL.
class TestApp
{
public:

    //! Constructor.
    TestApp();

    //! Destructor.
    virtual ~TestApp();

    //! Called upon application init.
    bool init();

    //! Called on application quit (cleanup).
    void deinit();

    //! Implements the application loop.
    void run();

    //! Called every time in the rendering loop.
    /*! Returns true if the program should keep on running. 
     */
    bool update();

protected:
    SDL_Surface *mScreen;
    PolygonTester *mPolygonTester;

#ifdef GATHER_STATISTICS
    class PolygonStatisticsCollection *mStatistics;
#endif

};

#endif // !TESTAPP_H_INCLUDED

