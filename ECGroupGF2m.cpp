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

#include "ECGroupGF2m.h"
#include "BigNum.h"
#include <stdexcept>
#include <vector>
#include <openssl/err.h>

using namespace ecc;

static void handleErrors(const std::string& msg)
{
    auto err = ERR_get_error();
    throw std::runtime_error(msg + ": " + ERR_reason_error_string(err));
}

ECGroupGF2m::ECGroupGF2m(size_t fieldSize) : ECGroup(fieldSize)
{}

ECGroupGF2m::~ECGroupGF2m()
{}

bool ECGroupGF2m::SetParameters(const BigNum& p, const BigNum& order, const BigNum& a, const BigNum& b, const BigNum& x, const BigNum& y)
{
    BN_CTX *ctx;
    EC_POINT *generator;

    /* Set up the BN_CTX */
    if(NULL == (ctx = BN_CTX_new())) {
        handleErrors("BN_CTX_new");
        return false;
    }

    /* Create the curve */
    if(NULL == (group = EC_GROUP_new_curve_GF2m(p.RawPtr(), a.RawPtr(), b.RawPtr(), ctx))) {
        handleErrors("EC_GROUP_new_curve_GF2m");
        return false;
    }

    /* Create the generator */
    if(NULL == (generator = EC_POINT_new(group))) {
        handleErrors("EC_POINT_new");
        return false;
    }

    if(1 != EC_POINT_set_affine_coordinates_GF2m(group, generator, x.RawPtr(), y.RawPtr(), ctx)) {
        handleErrors("EC_POINT_set_affine_coordinates_GF2m");
        return false;
    }

    /* Set the generator and the order */
    if(1 != EC_GROUP_set_generator(group, generator, order.RawPtr(), NULL)) {
        handleErrors("EC_GROUP_set_generator");
        return false;
    }

    EC_POINT_free(generator);
    BN_CTX_free(ctx);

    return true;
}