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

#ifndef __ECC_ELLIPTIC_CURVE_H__
#define __ECC_ELLIPTIC_CURVE_H__

#include "BigNum.h"
#include "ECGroup.h"
#include "ECPoint.h"
#include "BasisConversion.h"

#include <vector>
#include <memory>
#include <utility>

namespace ecc
{
    class EllipticCurve
    {
    public:
        std::shared_ptr<ECGroup> group;
        BasisConversion conversion;
        BigNum order;

    public:
        EllipticCurve() = default;
        ~EllipticCurve() = default;

        EllipticCurve(const EllipticCurve& other);
        EllipticCurve(const std::shared_ptr<ECGroup>& group, const BasisConversion& conversion, const BigNum& order);

        EllipticCurve& operator=(const EllipticCurve& other);

        BigNum RandomScalar();
        BigNum Normalize(const BigNum& value) const;

        ECPoint RandomPoint();
        ECPoint Multiply(const BigNum& k);

        ECPoint Point(const std::vector<uint8_t>& rawData);
        ECPoint Point(const std::vector<uint8_t>& x, uint8_t ybit);
        std::vector<uint8_t> Point2Vec(const ECPoint& point);
        std::vector<uint8_t> Point2VecCompressed(const ECPoint& point);

        BigNum Add(const BigNum& lhs, const BigNum& rhs) const;
        ECPoint Add(const ECPoint& lhs, const ECPoint& rhs) const;

        ECPoint Multiply(const BigNum& lhs, const ECPoint& rhs) const;

        BigNum ConvertNB(const BigNum& pb) const;
        BigNum ConvertPB(const BigNum& nb) const;

        std::pair<BigNum, BigNum> ConvertNB(const ECPoint& point) const;

        ECPoint ConvertPB(const std::vector<uint8_t>& x, uint8_t ybit) const;
        ECPoint ConvertPB(const BigNum& x, const BigNum& y) const;

        bool IsValidPoint(const ECPoint& point) const;
    };
}

#endif