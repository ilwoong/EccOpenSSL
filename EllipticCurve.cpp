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

#include "EllipticCurve.h"
#include "ECGroupGF2m.h"

using namespace ecc;

EllipticCurve::EllipticCurve() : EllipticCurve(nullptr)
{}

EllipticCurve::EllipticCurve(const std::shared_ptr<ECGroup>& group) : group(group)
{
    ctx = BN_CTX_new();
}

EllipticCurve::~EllipticCurve()
{
    if (ctx != nullptr) {
        BN_CTX_free(ctx);
        ctx = nullptr;
    }
}

BigNum EllipticCurve::RandomScalar()
{
    BIGNUM *k = BN_new();

    BN_pseudo_rand(k, group->FieldSize(), 0, 0);

    return BigNum(k);
}

BigNum EllipticCurve::Normalize(const BigNum& num) const
{
    return num;
}

ECPoint EllipticCurve::RandomPoint()
{
    auto k = RandomScalar();
    return RandomPoint(k);
}

ECPoint EllipticCurve::RandomPoint(const BigNum& k)
{
    EC_POINT* point = EC_POINT_new(group->Group());

    EC_POINT_mul(group->Group(), point, k.Data(), NULL, NULL, ctx);

    return ECPoint(group, point);
}

ECPoint EllipticCurve::Point(const std::vector<uint8_t>& rawData)
{
    EC_POINT* point = EC_POINT_new(group->Group());

    EC_POINT_oct2point(group->Group(), point, rawData.data(), rawData.size(), ctx);

    return ECPoint(group, point);
}

// ybit = 0 -> 0x02
// ybit = 1 -> 0x03
ECPoint EllipticCurve::Point(const std::vector<uint8_t>& x, uint8_t ybit)
{
    auto bnx = BigNum(x);
    EC_POINT* point = EC_POINT_new(group->Group());

    EC_POINT_set_compressed_coordinates(group->Group(), point, bnx.Data(), ybit & 0x1, ctx);

    return ECPoint(group, point);
}

std::vector<uint8_t> EllipticCurve::Point2Vec(const ECPoint& point)
{
    auto len = group->FieldSizeInBytes() * 2 + 1;
    std::vector<uint8_t> vec(len);
    EC_POINT_point2oct(group->Group(), point.Point(), point_conversion_form_t::POINT_CONVERSION_UNCOMPRESSED, vec.data(), len, ctx);

    return vec;
}

std::vector<uint8_t> EllipticCurve::Point2VecCompressed(const ECPoint& point)
{
    auto len = group->FieldSizeInBytes() + 1;
    std::vector<uint8_t> vec(len);
    EC_POINT_point2oct(group->Group(), point.Point(), point_conversion_form_t::POINT_CONVERSION_COMPRESSED, vec.data(), len, ctx);

    return vec;
}

bool EllipticCurve::IsValidPoint(const ECPoint& point) const
{
    return 1 == EC_POINT_is_on_curve(group->Group(), point.Point(), ctx);
}