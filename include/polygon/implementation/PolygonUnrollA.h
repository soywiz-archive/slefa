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
 * \brief PolygonUnrollA.h contains defines for unrolled loops. This version uses immediate data for everything.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */


#ifndef POLYGON_UNROLL_A_H_INCLUDED
#define POLYGON_UNROLL_A_H_INCLUDED

#define EVENODD_LINE_UNROLL_INIT() \
        SUBPIXEL_DATA __evenodd_line_mask__ = 1; \
        int __evenodd_line_xp__;

#define EVENODD_LINE_UNROLL_0(aScanline,aX,aSlope,aOffsets) \
        __evenodd_line_xp__ = aX + aOffsets[0]; \
        aX += aSlope; \
        __evenodd_line_xp__ = FIXED_TO_INT(__evenodd_line_xp__); \
        aScanline[__evenodd_line_xp__] ^= __evenodd_line_mask__; \
        __evenodd_line_mask__ <<= 1;

#define EVENODD_LINE_UNROLL_1(aScanline,aX,aSlope,aOffsets) \
        __evenodd_line_xp__ = aX + aOffsets[1]; \
        aX += aSlope; \
        __evenodd_line_xp__ = FIXED_TO_INT(__evenodd_line_xp__); \
        aScanline[__evenodd_line_xp__] ^= __evenodd_line_mask__; \
        __evenodd_line_mask__ <<= 1;

#define EVENODD_LINE_UNROLL_2(aScanline,aX,aSlope,aOffsets) \
        __evenodd_line_xp__ = aX + aOffsets[2]; \
        aX += aSlope; \
        __evenodd_line_xp__ = FIXED_TO_INT(__evenodd_line_xp__); \
        aScanline[__evenodd_line_xp__] ^= __evenodd_line_mask__; \
        __evenodd_line_mask__ <<= 1;

#define EVENODD_LINE_UNROLL_3(aScanline,aX,aSlope,aOffsets) \
        __evenodd_line_xp__ = aX + aOffsets[3]; \
        aX += aSlope; \
        __evenodd_line_xp__ = FIXED_TO_INT(__evenodd_line_xp__); \
        aScanline[__evenodd_line_xp__] ^= __evenodd_line_mask__; \
        __evenodd_line_mask__ <<= 1;

#define EVENODD_LINE_UNROLL_4(aScanline,aX,aSlope,aOffsets) \
        __evenodd_line_xp__ = aX + aOffsets[4]; \
        aX += aSlope; \
        __evenodd_line_xp__ = FIXED_TO_INT(__evenodd_line_xp__); \
        aScanline[__evenodd_line_xp__] ^= __evenodd_line_mask__; \
        __evenodd_line_mask__ <<= 1;

#define EVENODD_LINE_UNROLL_5(aScanline,aX,aSlope,aOffsets) \
        __evenodd_line_xp__ = aX + aOffsets[5]; \
        aX += aSlope; \
        __evenodd_line_xp__ = FIXED_TO_INT(__evenodd_line_xp__); \
        aScanline[__evenodd_line_xp__] ^= __evenodd_line_mask__; \
        __evenodd_line_mask__ <<= 1;

#define EVENODD_LINE_UNROLL_6(aScanline,aX,aSlope,aOffsets) \
        __evenodd_line_xp__ = aX + aOffsets[6]; \
        aX += aSlope; \
        __evenodd_line_xp__ = FIXED_TO_INT(__evenodd_line_xp__); \
        aScanline[__evenodd_line_xp__] ^= __evenodd_line_mask__; \
        __evenodd_line_mask__ <<= 1;

#define EVENODD_LINE_UNROLL_7(aScanline,aX,aSlope,aOffsets) \
        __evenodd_line_xp__ = aX + aOffsets[7]; \
        aX += aSlope; \
        __evenodd_line_xp__ = FIXED_TO_INT(__evenodd_line_xp__); \
        aScanline[__evenodd_line_xp__] ^= __evenodd_line_mask__; \
        __evenodd_line_mask__ <<= 1;

#if SUBPIXEL_COUNT > 8

#define EVENODD_LINE_UNROLL_8(aScanline,aX,aSlope,aOffsets) \
        __evenodd_line_xp__ = aX + aOffsets[8]; \
        aX += aSlope; \
        __evenodd_line_xp__ = FIXED_TO_INT(__evenodd_line_xp__); \
        aScanline[__evenodd_line_xp__] ^= __evenodd_line_mask__; \
        __evenodd_line_mask__ <<= 1;

#define EVENODD_LINE_UNROLL_9(aScanline,aX,aSlope,aOffsets) \
        __evenodd_line_xp__ = aX + aOffsets[9]; \
        aX += aSlope; \
        __evenodd_line_xp__ = FIXED_TO_INT(__evenodd_line_xp__); \
        aScanline[__evenodd_line_xp__] ^= __evenodd_line_mask__; \
        __evenodd_line_mask__ <<= 1;

#define EVENODD_LINE_UNROLL_10(aScanline,aX,aSlope,aOffsets) \
        __evenodd_line_xp__ = aX + aOffsets[10]; \
        aX += aSlope; \
        __evenodd_line_xp__ = FIXED_TO_INT(__evenodd_line_xp__); \
        aScanline[__evenodd_line_xp__] ^= __evenodd_line_mask__; \
        __evenodd_line_mask__ <<= 1;

#define EVENODD_LINE_UNROLL_11(aScanline,aX,aSlope,aOffsets) \
        __evenodd_line_xp__ = aX + aOffsets[11]; \
        aX += aSlope; \
        __evenodd_line_xp__ = FIXED_TO_INT(__evenodd_line_xp__); \
        aScanline[__evenodd_line_xp__] ^= __evenodd_line_mask__; \
        __evenodd_line_mask__ <<= 1;

#define EVENODD_LINE_UNROLL_12(aScanline,aX,aSlope,aOffsets) \
        __evenodd_line_xp__ = aX + aOffsets[12]; \
        aX += aSlope; \
        __evenodd_line_xp__ = FIXED_TO_INT(__evenodd_line_xp__); \
        aScanline[__evenodd_line_xp__] ^= __evenodd_line_mask__; \
        __evenodd_line_mask__ <<= 1;

#define EVENODD_LINE_UNROLL_13(aScanline,aX,aSlope,aOffsets) \
        __evenodd_line_xp__ = aX + aOffsets[13]; \
        aX += aSlope; \
        __evenodd_line_xp__ = FIXED_TO_INT(__evenodd_line_xp__); \
        aScanline[__evenodd_line_xp__] ^= __evenodd_line_mask__; \
        __evenodd_line_mask__ <<= 1;

#define EVENODD_LINE_UNROLL_14(aScanline,aX,aSlope,aOffsets) \
        __evenodd_line_xp__ = aX + aOffsets[14]; \
        aX += aSlope; \
        __evenodd_line_xp__ = FIXED_TO_INT(__evenodd_line_xp__); \
        aScanline[__evenodd_line_xp__] ^= __evenodd_line_mask__; \
        __evenodd_line_mask__ <<= 1;

#define EVENODD_LINE_UNROLL_15(aScanline,aX,aSlope,aOffsets) \
        __evenodd_line_xp__ = aX + aOffsets[15]; \
        aX += aSlope; \
        __evenodd_line_xp__ = FIXED_TO_INT(__evenodd_line_xp__); \
        aScanline[__evenodd_line_xp__] ^= __evenodd_line_mask__; \
        __evenodd_line_mask__ <<= 1;

#endif // !SUBPIXEL_COUNT > 8

#if SUBPIXEL_COUNT > 16

#define EVENODD_LINE_UNROLL_16(aScanline,aX,aSlope,aOffsets) \
        __evenodd_line_xp__ = aX + aOffsets[16]; \
        aX += aSlope; \
        __evenodd_line_xp__ = FIXED_TO_INT(__evenodd_line_xp__); \
        aScanline[__evenodd_line_xp__] ^= __evenodd_line_mask__; \
        __evenodd_line_mask__ <<= 1;

#define EVENODD_LINE_UNROLL_17(aScanline,aX,aSlope,aOffsets) \
        __evenodd_line_xp__ = aX + aOffsets[17]; \
        aX += aSlope; \
        __evenodd_line_xp__ = FIXED_TO_INT(__evenodd_line_xp__); \
        aScanline[__evenodd_line_xp__] ^= __evenodd_line_mask__; \
        __evenodd_line_mask__ <<= 1;

#define EVENODD_LINE_UNROLL_18(aScanline,aX,aSlope,aOffsets) \
        __evenodd_line_xp__ = aX + aOffsets[18]; \
        aX += aSlope; \
        __evenodd_line_xp__ = FIXED_TO_INT(__evenodd_line_xp__); \
        aScanline[__evenodd_line_xp__] ^= __evenodd_line_mask__; \
        __evenodd_line_mask__ <<= 1;

#define EVENODD_LINE_UNROLL_19(aScanline,aX,aSlope,aOffsets) \
        __evenodd_line_xp__ = aX + aOffsets[19]; \
        aX += aSlope; \
        __evenodd_line_xp__ = FIXED_TO_INT(__evenodd_line_xp__); \
        aScanline[__evenodd_line_xp__] ^= __evenodd_line_mask__; \
        __evenodd_line_mask__ <<= 1;

#define EVENODD_LINE_UNROLL_20(aScanline,aX,aSlope,aOffsets) \
        __evenodd_line_xp__ = aX + aOffsets[20]; \
        aX += aSlope; \
        __evenodd_line_xp__ = FIXED_TO_INT(__evenodd_line_xp__); \
        aScanline[__evenodd_line_xp__] ^= __evenodd_line_mask__; \
        __evenodd_line_mask__ <<= 1;

#define EVENODD_LINE_UNROLL_21(aScanline,aX,aSlope,aOffsets) \
        __evenodd_line_xp__ = aX + aOffsets[21]; \
        aX += aSlope; \
        __evenodd_line_xp__ = FIXED_TO_INT(__evenodd_line_xp__); \
        aScanline[__evenodd_line_xp__] ^= __evenodd_line_mask__; \
        __evenodd_line_mask__ <<= 1;

#define EVENODD_LINE_UNROLL_22(aScanline,aX,aSlope,aOffsets) \
        __evenodd_line_xp__ = aX + aOffsets[22]; \
        aX += aSlope; \
        __evenodd_line_xp__ = FIXED_TO_INT(__evenodd_line_xp__); \
        aScanline[__evenodd_line_xp__] ^= __evenodd_line_mask__; \
        __evenodd_line_mask__ <<= 1;

#define EVENODD_LINE_UNROLL_23(aScanline,aX,aSlope,aOffsets) \
        __evenodd_line_xp__ = aX + aOffsets[23]; \
        aX += aSlope; \
        __evenodd_line_xp__ = FIXED_TO_INT(__evenodd_line_xp__); \
        aScanline[__evenodd_line_xp__] ^= __evenodd_line_mask__; \
        __evenodd_line_mask__ <<= 1;

#define EVENODD_LINE_UNROLL_24(aScanline,aX,aSlope,aOffsets) \
        __evenodd_line_xp__ = aX + aOffsets[24]; \
        aX += aSlope; \
        __evenodd_line_xp__ = FIXED_TO_INT(__evenodd_line_xp__); \
        aScanline[__evenodd_line_xp__] ^= __evenodd_line_mask__; \
        __evenodd_line_mask__ <<= 1;

#define EVENODD_LINE_UNROLL_25(aScanline,aX,aSlope,aOffsets) \
        __evenodd_line_xp__ = aX + aOffsets[25]; \
        aX += aSlope; \
        __evenodd_line_xp__ = FIXED_TO_INT(__evenodd_line_xp__); \
        aScanline[__evenodd_line_xp__] ^= __evenodd_line_mask__; \
        __evenodd_line_mask__ <<= 1;

#define EVENODD_LINE_UNROLL_26(aScanline,aX,aSlope,aOffsets) \
        __evenodd_line_xp__ = aX + aOffsets[26]; \
        aX += aSlope; \
        __evenodd_line_xp__ = FIXED_TO_INT(__evenodd_line_xp__); \
        aScanline[__evenodd_line_xp__] ^= __evenodd_line_mask__; \
        __evenodd_line_mask__ <<= 1;

#define EVENODD_LINE_UNROLL_27(aScanline,aX,aSlope,aOffsets) \
        __evenodd_line_xp__ = aX + aOffsets[27]; \
        aX += aSlope; \
        __evenodd_line_xp__ = FIXED_TO_INT(__evenodd_line_xp__); \
        aScanline[__evenodd_line_xp__] ^= __evenodd_line_mask__; \
        __evenodd_line_mask__ <<= 1;

#define EVENODD_LINE_UNROLL_28(aScanline,aX,aSlope,aOffsets) \
        __evenodd_line_xp__ = aX + aOffsets[28]; \
        aX += aSlope; \
        __evenodd_line_xp__ = FIXED_TO_INT(__evenodd_line_xp__); \
        aScanline[__evenodd_line_xp__] ^= __evenodd_line_mask__; \
        __evenodd_line_mask__ <<= 1;

#define EVENODD_LINE_UNROLL_29(aScanline,aX,aSlope,aOffsets) \
        __evenodd_line_xp__ = aX + aOffsets[29]; \
        aX += aSlope; \
        __evenodd_line_xp__ = FIXED_TO_INT(__evenodd_line_xp__); \
        aScanline[__evenodd_line_xp__] ^= __evenodd_line_mask__; \
        __evenodd_line_mask__ <<= 1;

#define EVENODD_LINE_UNROLL_30(aScanline,aX,aSlope,aOffsets) \
        __evenodd_line_xp__ = aX + aOffsets[30]; \
        aX += aSlope; \
        __evenodd_line_xp__ = FIXED_TO_INT(__evenodd_line_xp__); \
        aScanline[__evenodd_line_xp__] ^= __evenodd_line_mask__; \
        __evenodd_line_mask__ <<= 1;

#define EVENODD_LINE_UNROLL_31(aScanline,aX,aSlope,aOffsets) \
        __evenodd_line_xp__ = aX + aOffsets[31]; \
        aX += aSlope; \
        __evenodd_line_xp__ = FIXED_TO_INT(__evenodd_line_xp__); \
        aScanline[__evenodd_line_xp__] ^= __evenodd_line_mask__; \
        __evenodd_line_mask__ <<= 1;

#endif // !SUBPIXEL_COUNT > 16


// Macros for non-zero fill.

#define NONZERO_LINE_UNROLL_INIT() \
        SUBPIXEL_DATA __nonzero_line_mask__ = 1; \
        int __nonzero_line_xp__;

#define NONZERO_LINE_UNROLL_0(aWindingBuffer,aX,aSlope,aOffsets,aWinding) \
        __nonzero_line_xp__ = aX + aOffsets[0]; \
        aX += aSlope; \
        __nonzero_line_xp__ = FIXED_TO_INT(__nonzero_line_xp__); \
        aWindingBuffer[__nonzero_line_xp__].mMask |= __nonzero_line_mask__; \
        aWindingBuffer[__nonzero_line_xp__].mBuffer[0] += aWinding; \
        __nonzero_line_mask__ <<= 1;

#define NONZERO_LINE_UNROLL_1(aWindingBuffer,aX,aSlope,aOffsets,aWinding) \
        __nonzero_line_xp__ = aX + aOffsets[1]; \
        aX += aSlope; \
        __nonzero_line_xp__ = FIXED_TO_INT(__nonzero_line_xp__); \
        aWindingBuffer[__nonzero_line_xp__].mMask |= __nonzero_line_mask__; \
        aWindingBuffer[__nonzero_line_xp__].mBuffer[1] += aWinding; \
        __nonzero_line_mask__ <<= 1;

#define NONZERO_LINE_UNROLL_2(aWindingBuffer,aX,aSlope,aOffsets,aWinding) \
        __nonzero_line_xp__ = aX + aOffsets[2]; \
        aX += aSlope; \
        __nonzero_line_xp__ = FIXED_TO_INT(__nonzero_line_xp__); \
        aWindingBuffer[__nonzero_line_xp__].mMask |= __nonzero_line_mask__; \
        aWindingBuffer[__nonzero_line_xp__].mBuffer[2] += aWinding; \
        __nonzero_line_mask__ <<= 1;

#define NONZERO_LINE_UNROLL_3(aWindingBuffer,aX,aSlope,aOffsets,aWinding) \
        __nonzero_line_xp__ = aX + aOffsets[3]; \
        aX += aSlope; \
        __nonzero_line_xp__ = FIXED_TO_INT(__nonzero_line_xp__); \
        aWindingBuffer[__nonzero_line_xp__].mMask |= __nonzero_line_mask__; \
        aWindingBuffer[__nonzero_line_xp__].mBuffer[3] += aWinding; \
        __nonzero_line_mask__ <<= 1;

#define NONZERO_LINE_UNROLL_4(aWindingBuffer,aX,aSlope,aOffsets,aWinding) \
        __nonzero_line_xp__ = aX + aOffsets[4]; \
        aX += aSlope; \
        __nonzero_line_xp__ = FIXED_TO_INT(__nonzero_line_xp__); \
        aWindingBuffer[__nonzero_line_xp__].mMask |= __nonzero_line_mask__; \
        aWindingBuffer[__nonzero_line_xp__].mBuffer[4] += aWinding; \
        __nonzero_line_mask__ <<= 1;

#define NONZERO_LINE_UNROLL_5(aWindingBuffer,aX,aSlope,aOffsets,aWinding) \
        __nonzero_line_xp__ = aX + aOffsets[5]; \
        aX += aSlope; \
        __nonzero_line_xp__ = FIXED_TO_INT(__nonzero_line_xp__); \
        aWindingBuffer[__nonzero_line_xp__].mMask |= __nonzero_line_mask__; \
        aWindingBuffer[__nonzero_line_xp__].mBuffer[5] += aWinding; \
        __nonzero_line_mask__ <<= 1;

#define NONZERO_LINE_UNROLL_6(aWindingBuffer,aX,aSlope,aOffsets,aWinding) \
        __nonzero_line_xp__ = aX + aOffsets[6]; \
        aX += aSlope; \
        __nonzero_line_xp__ = FIXED_TO_INT(__nonzero_line_xp__); \
        aWindingBuffer[__nonzero_line_xp__].mMask |= __nonzero_line_mask__; \
        aWindingBuffer[__nonzero_line_xp__].mBuffer[6] += aWinding; \
        __nonzero_line_mask__ <<= 1;

#define NONZERO_LINE_UNROLL_7(aWindingBuffer,aX,aSlope,aOffsets,aWinding) \
        __nonzero_line_xp__ = aX + aOffsets[7]; \
        aX += aSlope; \
        __nonzero_line_xp__ = FIXED_TO_INT(__nonzero_line_xp__); \
        aWindingBuffer[__nonzero_line_xp__].mMask |= __nonzero_line_mask__; \
        aWindingBuffer[__nonzero_line_xp__].mBuffer[7] += aWinding; \
        __nonzero_line_mask__ <<= 1;

#if SUBPIXEL_COUNT > 8

#define NONZERO_LINE_UNROLL_8(aWindingBuffer,aX,aSlope,aOffsets,aWinding) \
        __nonzero_line_xp__ = aX + aOffsets[8]; \
        aX += aSlope; \
        __nonzero_line_xp__ = FIXED_TO_INT(__nonzero_line_xp__); \
        aWindingBuffer[__nonzero_line_xp__].mMask |= __nonzero_line_mask__; \
        aWindingBuffer[__nonzero_line_xp__].mBuffer[8] += aWinding; \
        __nonzero_line_mask__ <<= 1;

#define NONZERO_LINE_UNROLL_9(aWindingBuffer,aX,aSlope,aOffsets,aWinding) \
        __nonzero_line_xp__ = aX + aOffsets[9]; \
        aX += aSlope; \
        __nonzero_line_xp__ = FIXED_TO_INT(__nonzero_line_xp__); \
        aWindingBuffer[__nonzero_line_xp__].mMask |= __nonzero_line_mask__; \
        aWindingBuffer[__nonzero_line_xp__].mBuffer[9] += aWinding; \
        __nonzero_line_mask__ <<= 1;

#define NONZERO_LINE_UNROLL_10(aWindingBuffer,aX,aSlope,aOffsets,aWinding) \
        __nonzero_line_xp__ = aX + aOffsets[10]; \
        aX += aSlope; \
        __nonzero_line_xp__ = FIXED_TO_INT(__nonzero_line_xp__); \
        aWindingBuffer[__nonzero_line_xp__].mMask |= __nonzero_line_mask__; \
        aWindingBuffer[__nonzero_line_xp__].mBuffer[10] += aWinding; \
        __nonzero_line_mask__ <<= 1;

#define NONZERO_LINE_UNROLL_11(aWindingBuffer,aX,aSlope,aOffsets,aWinding) \
        __nonzero_line_xp__ = aX + aOffsets[11]; \
        aX += aSlope; \
        __nonzero_line_xp__ = FIXED_TO_INT(__nonzero_line_xp__); \
        aWindingBuffer[__nonzero_line_xp__].mMask |= __nonzero_line_mask__; \
        aWindingBuffer[__nonzero_line_xp__].mBuffer[11] += aWinding; \
        __nonzero_line_mask__ <<= 1;

#define NONZERO_LINE_UNROLL_12(aWindingBuffer,aX,aSlope,aOffsets,aWinding) \
        __nonzero_line_xp__ = aX + aOffsets[12]; \
        aX += aSlope; \
        __nonzero_line_xp__ = FIXED_TO_INT(__nonzero_line_xp__); \
        aWindingBuffer[__nonzero_line_xp__].mMask |= __nonzero_line_mask__; \
        aWindingBuffer[__nonzero_line_xp__].mBuffer[12] += aWinding; \
        __nonzero_line_mask__ <<= 1;

#define NONZERO_LINE_UNROLL_13(aWindingBuffer,aX,aSlope,aOffsets,aWinding) \
        __nonzero_line_xp__ = aX + aOffsets[13]; \
        aX += aSlope; \
        __nonzero_line_xp__ = FIXED_TO_INT(__nonzero_line_xp__); \
        aWindingBuffer[__nonzero_line_xp__].mMask |= __nonzero_line_mask__; \
        aWindingBuffer[__nonzero_line_xp__].mBuffer[13] += aWinding; \
        __nonzero_line_mask__ <<= 1;

#define NONZERO_LINE_UNROLL_14(aWindingBuffer,aX,aSlope,aOffsets,aWinding) \
        __nonzero_line_xp__ = aX + aOffsets[14]; \
        aX += aSlope; \
        __nonzero_line_xp__ = FIXED_TO_INT(__nonzero_line_xp__); \
        aWindingBuffer[__nonzero_line_xp__].mMask |= __nonzero_line_mask__; \
        aWindingBuffer[__nonzero_line_xp__].mBuffer[14] += aWinding; \
        __nonzero_line_mask__ <<= 1;

#define NONZERO_LINE_UNROLL_15(aWindingBuffer,aX,aSlope,aOffsets,aWinding) \
        __nonzero_line_xp__ = aX + aOffsets[15]; \
        aX += aSlope; \
        __nonzero_line_xp__ = FIXED_TO_INT(__nonzero_line_xp__); \
        aWindingBuffer[__nonzero_line_xp__].mMask |= __nonzero_line_mask__; \
        aWindingBuffer[__nonzero_line_xp__].mBuffer[15] += aWinding; \
        __nonzero_line_mask__ <<= 1;

#endif // !SUBPIXEL_COUNT > 8

#if SUBPIXEL_COUNT > 16

#define NONZERO_LINE_UNROLL_16(aWindingBuffer,aX,aSlope,aOffsets,aWinding) \
        __nonzero_line_xp__ = aX + aOffsets[16]; \
        aX += aSlope; \
        __nonzero_line_xp__ = FIXED_TO_INT(__nonzero_line_xp__); \
        aWindingBuffer[__nonzero_line_xp__].mMask |= __nonzero_line_mask__; \
        aWindingBuffer[__nonzero_line_xp__].mBuffer[16] += aWinding; \
        __nonzero_line_mask__ <<= 1;

#define NONZERO_LINE_UNROLL_17(aWindingBuffer,aX,aSlope,aOffsets,aWinding) \
        __nonzero_line_xp__ = aX + aOffsets[17]; \
        aX += aSlope; \
        __nonzero_line_xp__ = FIXED_TO_INT(__nonzero_line_xp__); \
        aWindingBuffer[__nonzero_line_xp__].mMask |= __nonzero_line_mask__; \
        aWindingBuffer[__nonzero_line_xp__].mBuffer[17] += aWinding; \
        __nonzero_line_mask__ <<= 1;

#define NONZERO_LINE_UNROLL_18(aWindingBuffer,aX,aSlope,aOffsets,aWinding) \
        __nonzero_line_xp__ = aX + aOffsets[18]; \
        aX += aSlope; \
        __nonzero_line_xp__ = FIXED_TO_INT(__nonzero_line_xp__); \
        aWindingBuffer[__nonzero_line_xp__].mMask |= __nonzero_line_mask__; \
        aWindingBuffer[__nonzero_line_xp__].mBuffer[18] += aWinding; \
        __nonzero_line_mask__ <<= 1;

#define NONZERO_LINE_UNROLL_19(aWindingBuffer,aX,aSlope,aOffsets,aWinding) \
        __nonzero_line_xp__ = aX + aOffsets[19]; \
        aX += aSlope; \
        __nonzero_line_xp__ = FIXED_TO_INT(__nonzero_line_xp__); \
        aWindingBuffer[__nonzero_line_xp__].mMask |= __nonzero_line_mask__; \
        aWindingBuffer[__nonzero_line_xp__].mBuffer[19] += aWinding; \
        __nonzero_line_mask__ <<= 1;

#define NONZERO_LINE_UNROLL_20(aWindingBuffer,aX,aSlope,aOffsets,aWinding) \
        __nonzero_line_xp__ = aX + aOffsets[20]; \
        aX += aSlope; \
        __nonzero_line_xp__ = FIXED_TO_INT(__nonzero_line_xp__); \
        aWindingBuffer[__nonzero_line_xp__].mMask |= __nonzero_line_mask__; \
        aWindingBuffer[__nonzero_line_xp__].mBuffer[20] += aWinding; \
        __nonzero_line_mask__ <<= 1;

#define NONZERO_LINE_UNROLL_21(aWindingBuffer,aX,aSlope,aOffsets,aWinding) \
        __nonzero_line_xp__ = aX + aOffsets[21]; \
        aX += aSlope; \
        __nonzero_line_xp__ = FIXED_TO_INT(__nonzero_line_xp__); \
        aWindingBuffer[__nonzero_line_xp__].mMask |= __nonzero_line_mask__; \
        aWindingBuffer[__nonzero_line_xp__].mBuffer[21] += aWinding; \
        __nonzero_line_mask__ <<= 1;

#define NONZERO_LINE_UNROLL_22(aWindingBuffer,aX,aSlope,aOffsets,aWinding) \
        __nonzero_line_xp__ = aX + aOffsets[22]; \
        aX += aSlope; \
        __nonzero_line_xp__ = FIXED_TO_INT(__nonzero_line_xp__); \
        aWindingBuffer[__nonzero_line_xp__].mMask |= __nonzero_line_mask__; \
        aWindingBuffer[__nonzero_line_xp__].mBuffer[22] += aWinding; \
        __nonzero_line_mask__ <<= 1;

#define NONZERO_LINE_UNROLL_23(aWindingBuffer,aX,aSlope,aOffsets,aWinding) \
        __nonzero_line_xp__ = aX + aOffsets[23]; \
        aX += aSlope; \
        __nonzero_line_xp__ = FIXED_TO_INT(__nonzero_line_xp__); \
        aWindingBuffer[__nonzero_line_xp__].mMask |= __nonzero_line_mask__; \
        aWindingBuffer[__nonzero_line_xp__].mBuffer[23] += aWinding; \
        __nonzero_line_mask__ <<= 1;

#define NONZERO_LINE_UNROLL_24(aWindingBuffer,aX,aSlope,aOffsets,aWinding) \
        __nonzero_line_xp__ = aX + aOffsets[24]; \
        aX += aSlope; \
        __nonzero_line_xp__ = FIXED_TO_INT(__nonzero_line_xp__); \
        aWindingBuffer[__nonzero_line_xp__].mMask |= __nonzero_line_mask__; \
        aWindingBuffer[__nonzero_line_xp__].mBuffer[24] += aWinding; \
        __nonzero_line_mask__ <<= 1;

#define NONZERO_LINE_UNROLL_25(aWindingBuffer,aX,aSlope,aOffsets,aWinding) \
        __nonzero_line_xp__ = aX + aOffsets[25]; \
        aX += aSlope; \
        __nonzero_line_xp__ = FIXED_TO_INT(__nonzero_line_xp__); \
        aWindingBuffer[__nonzero_line_xp__].mMask |= __nonzero_line_mask__; \
        aWindingBuffer[__nonzero_line_xp__].mBuffer[25] += aWinding; \
        __nonzero_line_mask__ <<= 1;

#define NONZERO_LINE_UNROLL_26(aWindingBuffer,aX,aSlope,aOffsets,aWinding) \
        __nonzero_line_xp__ = aX + aOffsets[26]; \
        aX += aSlope; \
        __nonzero_line_xp__ = FIXED_TO_INT(__nonzero_line_xp__); \
        aWindingBuffer[__nonzero_line_xp__].mMask |= __nonzero_line_mask__; \
        aWindingBuffer[__nonzero_line_xp__].mBuffer[26] += aWinding; \
        __nonzero_line_mask__ <<= 1;

#define NONZERO_LINE_UNROLL_27(aWindingBuffer,aX,aSlope,aOffsets,aWinding) \
        __nonzero_line_xp__ = aX + aOffsets[27]; \
        aX += aSlope; \
        __nonzero_line_xp__ = FIXED_TO_INT(__nonzero_line_xp__); \
        aWindingBuffer[__nonzero_line_xp__].mMask |= __nonzero_line_mask__; \
        aWindingBuffer[__nonzero_line_xp__].mBuffer[27] += aWinding; \
        __nonzero_line_mask__ <<= 1;

#define NONZERO_LINE_UNROLL_28(aWindingBuffer,aX,aSlope,aOffsets,aWinding) \
        __nonzero_line_xp__ = aX + aOffsets[28]; \
        aX += aSlope; \
        __nonzero_line_xp__ = FIXED_TO_INT(__nonzero_line_xp__); \
        aWindingBuffer[__nonzero_line_xp__].mMask |= __nonzero_line_mask__; \
        aWindingBuffer[__nonzero_line_xp__].mBuffer[28] += aWinding; \
        __nonzero_line_mask__ <<= 1;

#define NONZERO_LINE_UNROLL_29(aWindingBuffer,aX,aSlope,aOffsets,aWinding) \
        __nonzero_line_xp__ = aX + aOffsets[29]; \
        aX += aSlope; \
        __nonzero_line_xp__ = FIXED_TO_INT(__nonzero_line_xp__); \
        aWindingBuffer[__nonzero_line_xp__].mMask |= __nonzero_line_mask__; \
        aWindingBuffer[__nonzero_line_xp__].mBuffer[29] += aWinding; \
        __nonzero_line_mask__ <<= 1;

#define NONZERO_LINE_UNROLL_30(aWindingBuffer,aX,aSlope,aOffsets,aWinding) \
        __nonzero_line_xp__ = aX + aOffsets[30]; \
        aX += aSlope; \
        __nonzero_line_xp__ = FIXED_TO_INT(__nonzero_line_xp__); \
        aWindingBuffer[__nonzero_line_xp__].mMask |= __nonzero_line_mask__; \
        aWindingBuffer[__nonzero_line_xp__].mBuffer[30] += aWinding; \
        __nonzero_line_mask__ <<= 1;

#define NONZERO_LINE_UNROLL_31(aWindingBuffer,aX,aSlope,aOffsets,aWinding) \
        __nonzero_line_xp__ = aX + aOffsets[31]; \
        aX += aSlope; \
        __nonzero_line_xp__ = FIXED_TO_INT(__nonzero_line_xp__); \
        aWindingBuffer[__nonzero_line_xp__].mMask |= __nonzero_line_mask__; \
        aWindingBuffer[__nonzero_line_xp__].mBuffer[31] += aWinding; \
        __nonzero_line_mask__ <<= 1;

#endif // !SUBPIXEL_COUNT > 16

#endif // !POLYGON_UNROLL_A_H_INCLUDED
