/*
 * fMBT, free Model Based Testing tool
 * Copyright (c) 2012, Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 2.1, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */


#define ERROR_CANNOT_OPEN_IMAGEFILE -3
#define ERROR_CANNOT_OPEN_ICONFILE -4

#if defined(__MINGW32__) || defined(_MSC_VER)

typedef signed int int32_t;
#define EXPORT __declspec(dllexport)

#else

#define EXPORT

#include <sys/types.h>

#endif

extern "C" {

    EXPORT void initeye4graphics();

    typedef struct _bbox {
        int32_t left, top, right, bottom;
        int32_t error;
    } BoundingBox;

    typedef struct _rgb888 {
        unsigned char red, green, blue;
    } rgb888;

    /*
     * rgb888at - read color value at coordinates
     *
     * Parameters:
     *   - color (out)  - out: pixel color value
     *   - image        - opened image
     *   - x, y         - coordinates
     */

    EXPORT
    int rgb888at(rgb888* color,
                 void* image,
                 const int x,
                 const int y);

    /*
     * findNextColor
     *
     * Parameters:
     *   - bbox (in/out)- in: bounding box of previously found pixel
     *                    out: bounding box of next pixel
     *   - image        - opened image
     *   - color        - color to be searched for.
     *   - colorMatch   - 0.0 - 1.0, required color match.
     *   - opacityLimit - skip comparing pixels with opacity < opacityLimit
     *                    0.0 (default) compares all pixels without reading
     *                    opacity values.
     *   - invertMatch  - if non-zero, find a pixel that does not match the
     *                    color.
     *   - searchArea   - bounding box of area in the imagefile to be searched.
     *
     * Return value:
     *    0: not found
     *    1: found
     */

    EXPORT
    int findNextColor(BoundingBox* bbox,
                      void* image,
                      const rgb888* color,
                      const double colorMatch,
                      const double opacityLimit,
                      const int invertMatch,
                      const BoundingBox* searchArea);

    /*
     * findSingleIcon
     *
     * Parameters:
     *   - bbox (out)   - bounding box of matching area
     *   - imagefile    - name of image to be searched from (haystack)
     *   - iconfile     - name of icon to be search for (needle)
     *   - threshold    - max. acceptable error.
     *       0: perfect match
     *       9: big error allowed
     *   - colorMatch   - 0.0 - 1.0, required color match
     *   - opacityLimit - skip comparing pixels with opacity < opacityLimit
     *                    0.0 (default) compares all pixels without reading
     *                    opacity values
     *   - searchArea   - bounding box of area in the imagefile to be searched
     *
     * Return value:
     *    0: success
     *   -1: nothing like icon has been found
     *   -2: found an icon but it has too big error
     *   -3: cannot open imagefile
     *   -4: cannot open iconfile
     */

    EXPORT
    int findSingleIcon(BoundingBox* bbox,
                       const char* imagefile,
                       const char* iconfile,
                       const int threshold,
                       const double colorMatch,
                       const double opacityLimit,
                       const BoundingBox* searchArea);

    /*
     * findNextIcon
     *
     * Parameters:
     * - bbox (in/out)  - in: bounding box of previously found icon
     *                    out: bounding box of next icon
     * - image          - opened image (returned by openImage)
     * - icon           - opened icon (returned by openImage)
     * - threshold, colorMatch, opacityLimit, searchArea - see findSingleIcon
     * - continueOpts   - if 0, find the first match (ignore bbox value)
     *                    if non-zero, find the next match
     * - xscale         - X axis scaling for icon
     * - yscale         - Y axis scaling for icon
     * - neeRectSize    - area on icon, counter part must be in image
     * - hayRectSize    - area on image, counter part for area in icon
     *
     * Return value:
     *     see findSingleIcon
     */

    EXPORT
    int findNextIcon(BoundingBox* bbox,
                     void* image,
                     void* icon,
                     const int threshold,
                     const double colorMatch,
                     const double opacityLimit,
                     const BoundingBox* searchArea,
                     const int continueOpts,
                     const float xscale,
                     const float yscale,
                     const int neeRectSize,
                     const int hayRectSize);

    /*
     * findNextDiff
     *
     * Parameters:
     *   - bbox (out)   - bounding box of differing pixel
     *   - imageA       - the first compared image
     *   - imageB       - the second compared image
     *   - colorMatch   - 0.0 - 1.0, required color match, the default is 1.0
     *   - opacityLimit - skip comparing pixels with opacity < opacityLimit
     *   - searchAreaA  - unused
     *   - searchAreaB  - unused
     *   - continueOpts - 0: fresh start, 1: continue from current bbox
     *
     * Return value:
     *    1: a differing pixel found and stored to bbox
     *    0: no differing pixels found
     */
    EXPORT
    int findNextDiff(BoundingBox* bbox,
                     void* imageA,
                     void* imageB,
                     const double colorMatch,
                     const double opacityLimit,
                     const BoundingBox* searchAreaA,
                     const BoundingBox* searchAreaB,
                     const int continueOpts);



    EXPORT
    int findNextHighErrorBlock(BoundingBox* bbox,
                               void* image,
                               const int columns,
                               const int rows,
                               const double threshold,
                               const BoundingBox* searchArea);

    /*
     * imageDimensions
     *
     * Parameters:
     *   - bbox for returning dimensions of the image
     *   - imagefile - name of the image
     *
     * Return value:
     *    0: success
     *   -3: cannot open image file
     *
     *       on success, bbox.right is the width, and bbox.bottom is
     *       the height of the image.
     */
    EXPORT
    int imageDimensions(BoundingBox* bbox,
                        const char* imagefile);

    EXPORT
    int openedImageDimensions(BoundingBox* bbox, void* image);

    EXPORT
    int openedImageIsBlank(void* image);

    EXPORT
    void* openImage(const char* imagefile);

    EXPORT
    void* openBlob(const void* blob, const char* pixelorder, int x, int y);

    EXPORT
    void closeImage(void* image);

    /*
     * bgrx2rgb - convert 4-bytes-per-pixel bitmap data (BGRx) to RGB.
     *
     * Return value:
     *    0: all pixels are black
     *    > 0: there is at least one non-black pixel
     */
    EXPORT
    int bgrx2rgb(char* data, int width, int height);

    /*
     * rgbx2rgb - convert 4-bytes-per-pixel bitmap data (RGBx) to RGB.
     *
     * Return value:
     *    0: all pixels are black
     *    > 0: there is at least one non-black pixel
     */
    EXPORT
    int rgbx2rgb(char* data, int width, int height);


    /*
     * rgb5652rgb - convert 16-bit RGB (5-6-5) to 24-bit RGB (8-8-8).
     *
     * Return value:
     *    0: all pixels are black
     *    > 0: there is at least one non-black pixel
     */
    int rgb5652rgb(unsigned short* data_in, int width, int height, char* data_out);

    /*
     * bgr2rgb - convert bitmap data BGR to RGB.
     *
     * Return value:
     *    0: all pixels are black
     *    > 0: there is at least one non-black pixel
     */
    EXPORT
    int bgr2rgb(char* data, int width, int height);

    /*
     * wbgr2rgb - convert Windows GetDIBits (BGR, mirrored Y) image to RGB.
     */
    EXPORT
    int wbgr2rgb(char* data, int width, int height);
}
