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

#include "BasisConversion.h"
#include "GF2Polynomial.h"

using namespace ecc;

BasisConversion::BasisConversion(const GF2Polynomial& prime, const BigNum& root) : prime(prime)
{
    GF2Polynomial gamma(prime.Length(), root);

    for (auto i = 0; i <= prime.Length(); ++i) {
        auto row = gamma.Value();
        matrix.AddRow(row);
        
        gamma = gamma * gamma;
        gamma = gamma % prime;
    }
}

BigNum BasisConversion::Convert(const BigNum& num) const
{
    if (num.BitLength() > matrix.Rows()) {
        throw std::invalid_argument("length mismatch between BigNum and GF2Matrix");
    }

    auto poly = GF2Polynomial(matrix.Rows(), num);
    auto converted = poly * matrix;
    return converted.ToBigNum();
}

ECPoint BasisConversion::Convert(const ECPoint& point) const
{
    auto x = Convert(point.XCoord());
    auto y = Convert(point.YCoord());

    return ECPoint(point.Group(), x, y);
}
