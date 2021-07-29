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

ECPoint::ECPoint(const ECPoint& other) : ECPoint(other.group, EC_POINT_dup(other.point, other.group->Group()))
{}

ECPoint::ECPoint(const std::shared_ptr<ECGroup>& group) : ECPoint(group, EC_POINT_new(group->Group()))
{}

ECPoint::ECPoint(const std::shared_ptr<ECGroup>& group, EC_POINT* point) : group(group), point(point), x(BigNum(BN_new())), y(BigNum(BN_new()))
{
    EC_POINT_get_affine_coordinates_GFp(group->Group(), point, x.num, y.num, NULL);
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

    point = EC_POINT_dup(point, group->Group());
    x = other.x;
    y = other.y;

    return *this;
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