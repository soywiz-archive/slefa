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
 * \brief NonZeroMaskA is an implementation for tracking the mask data of non-zero winding buffer.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */


#ifndef NON_ZERO_MASK_A_H_INCLUDED
#define NON_ZERO_MASK_A_H_INCLUDED

//! An implementation for tracking the mask data of non-zero winding buffer.
class NonZeroMaskA
{
public:
    static inline void reset(NonZeroMask &aSource, NonZeroMask &aDestination)
    {
        aDestination.mMask = 0;
        SUBPIXEL_DATA bit = 1;
        int n;
        for (n = 0; n < SUBPIXEL_COUNT; n++)
        {
            if (aSource.mMask & bit)
            {
               aDestination.mBuffer[n] = aSource.mBuffer[n];
               aSource.mBuffer[n] = 0;
               if (aDestination.mBuffer[n])
                  aDestination.mMask |= bit;
            }
            else
                aDestination.mBuffer[n] = 0;
            bit <<= 1;
        }
        aSource.mMask = 0;
    }

    static inline void init(NonZeroMask &aSource, NonZeroMask &aDestination)
    {
        SUBPIXEL_DATA bit = 1;
        int n;
        for (n = 0; n < SUBPIXEL_COUNT; n++)
        {
            if (aSource.mMask & bit)
            {
               aDestination.mBuffer[n] = aSource.mBuffer[n];
               aSource.mBuffer[n] = 0;
               if (aDestination.mBuffer[n])
                  aDestination.mMask |= bit;
            }
            bit <<= 1;
        }
        aSource.mMask = 0;
    }

    static inline void apply(NonZeroMask &aSource, NonZeroMask &aDestination)
    {
        SUBPIXEL_DATA bit = 1;
        int n;
        for (n = 0; n < SUBPIXEL_COUNT; n++)
        {
            if (aSource.mMask & bit)
            {
                NON_ZERO_MASK_DATA_UNIT t = aDestination.mBuffer[n];
                aDestination.mBuffer[n] += aSource.mBuffer[n];
                aSource.mBuffer[n] = 0;
                if ((t == 0) ^ (aDestination.mBuffer[n] == 0))
                    aDestination.mMask ^= bit;
            }
            bit <<= 1;
        }
        aSource.mMask = 0;
    }

};

#endif // !NON_ZERO_MASK_A_H_INCLUDED
