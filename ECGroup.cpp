/** 
 * MIT License
 * 
 * Copyright (c) 2021 Ilwoong Jeong (https://github.com/ilwoong)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "ECGroup.h"
#include "BigNum.h"
#include <vector>

using namespace ecc;

static void handleErrors()
{

}

ECGroup::ECGroup() : group(nullptr)
{
    BN_CTX *ctx;
    BigNum a, b, p, order, x, y;
    EC_POINT *generator;

    /* Binary data for the curve parameters */
    std::vector<uint8_t> a_bin = {
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFE
    };
    std::vector<uint8_t> b_bin = {
        0xB4, 0x05, 0x0A, 0x85, 0x0C, 0x04, 0xB3, 0xAB,
        0xF5, 0x41, 0x32, 0x56, 0x50, 0x44, 0xB0, 0xB7,
        0xD7, 0xBF, 0xD8, 0xBA, 0x27, 0x0B, 0x39, 0x43,
        0x23, 0x55, 0xFF, 0xB4
    };
    std::vector<uint8_t> p_bin = {
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x01
    };
    std::vector<uint8_t> order_bin = {
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x16, 0xA2, 
        0xE0, 0xB8, 0xF0, 0x3E, 0x13, 0xDD, 0x29, 0x45, 
        0x5C, 0x5C, 0x2A, 0x3D 
    };
    std::vector<uint8_t> x_bin = {
        0xB7, 0x0E, 0x0C, 0xBD, 0x6B, 0xB4, 0xBF, 0x7F, 
        0x32, 0x13, 0x90, 0xB9, 0x4A, 0x03, 0xC1, 0xD3, 
        0x56, 0xC2, 0x11, 0x22, 0x34, 0x32, 0x80, 0xD6, 
        0x11, 0x5C, 0x1D, 0x21
    };
    std::vector<uint8_t> y_bin ={
        0xbd, 0x37, 0x63, 0x88, 0xb5, 0xf7, 0x23, 0xfb, 
        0x4c, 0x22, 0xdf, 0xe6, 0xcd, 0x43, 0x75, 0xa0, 
        0x5a, 0x07, 0x47, 0x64, 0x44, 0xd5, 0x81, 0x99, 
        0x85, 0x00, 0x7e, 0x34
    };

    /* Set up the BN_CTX */
    if(NULL == (ctx = BN_CTX_new())) handleErrors();

    /* Set the values for the various parameters */
    a = BigNum(a_bin);
    b = BigNum(b_bin);
    p = BigNum(p_bin);
    order = BigNum(order_bin);
    x = BigNum(x_bin);
    y = BigNum(y_bin);

    /* Create the curve */
    if(NULL == (group = EC_GROUP_new_curve_GFp(p.num, a.num, b.num, ctx))) handleErrors();

    /* Create the generator */
    if(NULL == (generator = EC_POINT_new(group))) handleErrors();
    if(1 != EC_POINT_set_affine_coordinates_GFp(group, generator, x.num, y.num, ctx))
        handleErrors();

    /* Set the generator and the order */
    if(1 != EC_GROUP_set_generator(group, generator, order.num, NULL))
        handleErrors();

    bitCount = BN_num_bits(order.num);

    EC_POINT_free(generator);
    BN_CTX_free(ctx);
}

ECGroup::ECGroup(const ECGroup& other)
{
    bitCount = other.bitCount;
    group = EC_GROUP_dup(other.group);
}

ECGroup::~ECGroup()
{
    if (group != nullptr) {
        EC_GROUP_free(group);
        group = nullptr;
    }
}

size_t ECGroup::BitCount() const
{
    return bitCount;
}

EC_GROUP* ECGroup::Group()
{
    return group;
}