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

#ifndef __ECC_BASIS_CONVERSION_H__
#define __ECC_BASIS_CONVERSION_H__

#include "GF2Matrix.h"
#include "ECPoint.h"

namespace ecc 
{
    class BasisConversion 
    {
    private:
        GF2Polynomial prime;
        GF2Matrix matrix;

    public:
        BasisConversion(const GF2Polynomial& prime, const BigNum& root);
        ~BasisConversion() = default;

        BigNum Convert(const BigNum& num) const;
        ECPoint Convert(const ECPoint& point) const;
    };
}

#endif