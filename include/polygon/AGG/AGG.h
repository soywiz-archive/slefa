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
 * \brief A common include file for AGG headers.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */


#ifndef AGG_H_INCLUDED
#define AGG_H_INCLUDED

#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_path_storage.h"
#include "agg_conv_transform.h"
#include "agg_bounding_rect.h"

//#define AGG_GRAY16
//#define AGG_BGR24
//#define AGG_BGR48 
//#define AGG_RGB_AAA
#define AGG_BGRA32 
//#define AGG_RGBA32 
//#define AGG_ARGB32 
//#define AGG_ABGR32
//#define AGG_RGB565
//#define AGG_RGB555
#include "pixel_formats.h"

#include "polygon/AGG/AGGPolygonWrapper.h"
#include "polygon/AGG/AGGPolygonFactory.h"
#include "polygon/AGG/AGGRenderTargetWrapper.h"
#include "polygon/AGG/AGGRenderer.h"

#endif // !AGG_H_INCLUDED
