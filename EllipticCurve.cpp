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

EllipticCurve::EllipticCurve(const std::shared_ptr<ECGroup>& group, const BasisConversion& conversion, const BigNum& order) : group(group), conversion(conversion), order(order)
{
}

EllipticCurve::EllipticCurve(const EllipticCurve& other) : EllipticCurve(other.group, other.conversion, other.order)
{}

EllipticCurve& EllipticCurve::operator=(const EllipticCurve& other)
{
    this->group = other.group;
    this->conversion = other.conversion;
    this->order = other.order;

    return *this;
}

BigNum EllipticCurve::RandomScalar()
{
    BIGNUM *k = BN_new();

    BN_pseudo_rand(k, group->FieldSize(), 0, 0);

    return BigNum(k);
}

BigNum EllipticCurve::Normalize(const BigNum& num) const
{
    return num % order;
}

ECPoint EllipticCurve::RandomPoint()
{
    auto k = RandomScalar();
    return Multiply(k);
}

ECPoint EllipticCurve::Multiply(const BigNum& k)
{
    EC_POINT* point = EC_POINT_new(group->RawPtr());

    EC_POINT_mul(group->RawPtr(), point, k.RawPtr(), nullptr, nullptr, nullptr);

    return ECPoint(group, point);
}

ECPoint EllipticCurve::Point(const std::vector<uint8_t>& rawData)
{
    EC_POINT* point = EC_POINT_new(group->RawPtr());

    EC_POINT_oct2point(group->RawPtr(), point, rawData.data(), rawData.size(), nullptr);

    return ECPoint(group, point);
}

// ybit = 0 -> 0x02
// ybit = 1 -> 0x03
ECPoint EllipticCurve::Point(const std::vector<uint8_t>& x, uint8_t ybit)
{
    auto bnx = BigNum(x);
    EC_POINT* point = EC_POINT_new(group->RawPtr());

    EC_POINT_set_compressed_coordinates(group->RawPtr(), point, bnx.RawPtr(), ybit & 0x1, nullptr);

    return ECPoint(group, point);
}

std::vector<uint8_t> EllipticCurve::Point2Vec(const ECPoint& point)
{
    auto len = group->FieldSizeInBytes() * 2 + 1;
    std::vector<uint8_t> vec(len);
    EC_POINT_point2oct(group->RawPtr(), point.RawPtr(), point_conversion_form_t::POINT_CONVERSION_UNCOMPRESSED, vec.data(), len, nullptr);

    return vec;
}

std::vector<uint8_t> EllipticCurve::Point2VecCompressed(const ECPoint& point)
{
    auto len = group->FieldSizeInBytes() + 1;
    std::vector<uint8_t> vec(len);
    EC_POINT_point2oct(group->RawPtr(), point.RawPtr(), point_conversion_form_t::POINT_CONVERSION_COMPRESSED, vec.data(), len, nullptr);

    return vec;
}

BigNum EllipticCurve::Add(const BigNum& lhs, const BigNum& rhs) const
{
    return (lhs + rhs) % order;
}

ECPoint EllipticCurve::Add(const ECPoint& lhs, const ECPoint& rhs) const
{
    return (lhs + rhs);
}

ECPoint EllipticCurve::Multiply(const BigNum& lhs, const ECPoint& rhs) const
{
    return (lhs * rhs);
}

bool EllipticCurve::IsValidPoint(const ECPoint& point) const
{
    return 1 == EC_POINT_is_on_curve(group->RawPtr(), point.RawPtr(), nullptr);
}

BigNum EllipticCurve::ConvertPB(const BigNum& nb) const
{
    return conversion.ConvertPB(nb);
}

BigNum EllipticCurve::ConvertNB(const BigNum& pb) const
{
    return conversion.ConvertNB(pb);
}

std::pair<BigNum, BigNum> EllipticCurve::ConvertNB(const ECPoint& point) const
{
    auto nbX = conversion.ConvertNB(point.XCoord());
    auto nbY = conversion.ConvertNB(point.YCoord());

    return std::make_pair<>(nbX, nbY);
}

ECPoint EllipticCurve::ConvertPB(const std::vector<uint8_t>& nbX, uint8_t ybit) const
{
    auto nbY = conversion.ConvertPB(nbX);
    return ConvertPB(nbX, nbY);
}

ECPoint EllipticCurve::ConvertPB(const BigNum& nbX, const BigNum& nbY) const
{
    auto x = conversion.ConvertPB(nbX);
    auto y = conversion.ConvertPB(nbY);
    return ECPoint(group, x, y);
}
