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
 * \brief A tick counter implementation.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */

#include <windows.h>
#include "platform/TickCounter.h"

//! Starts tick counting.
void TickCounter::start()
{
    LARGE_INTEGER t;
    QueryPerformanceCounter(&t);
    mStartTick = t.QuadPart;
}


//! Ends tick counting.
void TickCounter::end()
{
    LARGE_INTEGER t;
    QueryPerformanceCounter(&t);
    mEndTick = t.QuadPart;
}


//! Returns the amount of picoseconds spent between start() and end().
int TickCounter::getTicks()
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

    __int64 tickDiff = mEndTick - mStartTick;

    __int64 result = (tickDiff * 1000000) / frequency.QuadPart;
    return (int)result;
}
