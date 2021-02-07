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

#include <windows.h>
#include "platform/System.h"

//! Sets the priority of the current process.
bool System::setPriority(SYSTEM_PRIORITY aPriority)
{
    DWORD priorityClass;

    switch (aPriority)
    {
        case SYSTEM_PRIORITY_REALTIME:
            priorityClass = REALTIME_PRIORITY_CLASS;
            break;
        case SYSTEM_PRIORITY_HIGH:
            priorityClass = HIGH_PRIORITY_CLASS;
            break;
        case SYSTEM_PRIORITY_NORMAL:
            priorityClass = NORMAL_PRIORITY_CLASS;
            break;
        case SYSTEM_PRIORITY_LOW:
            priorityClass = IDLE_PRIORITY_CLASS;
            break;
        default:
            priorityClass = NORMAL_PRIORITY_CLASS;
            break;
    }

    return SetPriorityClass(GetCurrentProcess(),priorityClass) == TRUE;
}
