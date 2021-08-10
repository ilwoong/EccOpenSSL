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

#include "ECPoint.h"

using namespace ecc;

ECPoint::ECPoint(const ECPoint& other) : ECPoint(other.group, EC_POINT_dup(other.point, other.group->RawPtr()))
{}

ECPoint::ECPoint(const std::shared_ptr<ECGroup>& group) : ECPoint(group, EC_POINT_new(group->RawPtr()))
{}

ECPoint::ECPoint(const std::shared_ptr<ECGroup>& group, EC_POINT* point) : group(group), point(point), x(BigNum(BN_new())), y(BigNum(BN_new()))
{
    EC_POINT_get_affine_coordinates_GF2m(group->RawPtr(), point, x.RawPtr(), y.RawPtr(), NULL);
}

ECPoint::ECPoint(const std::shared_ptr<ECGroup>& group, const BigNum& x, const BigNum& y) : group(group), point(EC_POINT_new(group->RawPtr()))
{
    EC_POINT_set_affine_coordinates_GF2m(group->RawPtr(), point, x.RawPtr(), y.RawPtr(), NULL);
}

ECPoint::~ECPoint()
{
    if (point != nullptr) {
        EC_POINT_free(point);
        point = nullptr;
    }
}

ECPoint& ECPoint::operator=(const ECPoint& other)
{
    group = other.group;
    if (point != nullptr) {
        EC_POINT_free(point);
        point = nullptr;
    }

    point = EC_POINT_dup(point, group->RawPtr());
    x = other.x;
    y = other.y;

    return *this;
}

ECPoint ECPoint::operator+(const ECPoint& other) const
{
    if (group != other.Group()) {
        throw std::invalid_argument("ECPoint add: two points are not on the same curve");
    }

    EC_POINT* result = EC_POINT_new(group->RawPtr());
    EC_POINT_add(group->RawPtr(), result, point, other.point, NULL);
    return ECPoint(group, result);
}

ECPoint ECPoint::operator*(const BigNum& num) const
{
    EC_POINT* result = EC_POINT_new(group->RawPtr());
    EC_POINT_mul(group->RawPtr(), result, NULL, point, num.RawPtr(), NULL);
    return ECPoint(group, result);
}

std::shared_ptr<ECGroup> ECPoint::Group() const
{
    return group;
}

EC_POINT* ECPoint::RawPtr()
{
    return point;
}

const EC_POINT* ECPoint::RawPtr() const
{
    return point;
}

BigNum ECPoint::XCoord() const
{
    return x;
}

BigNum ECPoint::YCoord() const
{
    return y;
}

const std::string ECPoint::ToString() const
{
    return "(" + x.ToString() + ", " + y.ToString() + ")";
}

ECPoint ecc::operator*(const BigNum& num, const ECPoint& point)
{
    return point * num;
}