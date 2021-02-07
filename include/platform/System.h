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
 * \brief Various system level function implementations.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */


#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

enum SYSTEM_PRIORITY
{
    SYSTEM_PRIORITY_REALTIME = 0,
    SYSTEM_PRIORITY_HIGH,
    SYSTEM_PRIORITY_NORMAL,
    SYSTEM_PRIORITY_LOW
};

//! Various system level function implementations.
class System
{
public:
    //! Sets the priority of the current process.
    static bool setPriority(SYSTEM_PRIORITY aPriority);
};

#endif // !SYSTEM__H_INCLUDED
