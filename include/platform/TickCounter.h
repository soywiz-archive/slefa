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


#ifndef TICK_COUNTER_H_INCLUDED
#define TICK_COUNTER_H_INCLUDED

//! A tick counter implementation.
class TickCounter
{
public:
    //! Starts tick counting.
    void start();

    //! Ends tick counting.
    void end();

    //! Returns the amount of picoseconds spent between start() and end().
    int getTicks();

protected:
    __int64 mStartTick;
    __int64 mEndTick;
};

#endif // !TICK_COUNTER_H_INCLUDED
