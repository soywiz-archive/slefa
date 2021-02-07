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
 * \brief A common include file for GDI+ headers.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */


#ifndef GDI_PLUS_H_INCLUDED
#define GDI_PLUS_H_INCLUDED

#define UNICODE
#include <windows.h>
#include <gdiplus.h>

using namespace Gdiplus;

#include "polygon/GDIplus/GDIplusPolygonWrapper.h"
#include "polygon/GDIplus/GDIplusPolygonFactory.h"
#include "polygon/GDIplus/GDIplusRenderTargetWrapper.h"
#include "polygon/GDIplus/GDIplusRenderer.h"

#endif // !GDI_PLUS_H_INCLUDED
