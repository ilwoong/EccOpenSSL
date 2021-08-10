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

#ifndef __ECC_EC_POINT_H__
#define __ECC_EC_POINT_H__

#include "BigNum.h"
#include "ECGroup.h"

#include <string>
#include <memory>
#include <openssl/ec.h>

namespace ecc
{
    class ECPoint
    {
    private:
        std::shared_ptr<ECGroup> group;
        EC_POINT *point;
        BigNum x;
        BigNum y;

    public:
        ECPoint(const ECPoint& other);
        ECPoint(const std::shared_ptr<ECGroup>& group);
        ECPoint(const std::shared_ptr<ECGroup>& group, EC_POINT* point);
        ECPoint(const std::shared_ptr<ECGroup>& group, const BigNum& x, const BigNum& y);
        ~ECPoint();

        ECPoint& operator=(const ECPoint& other);
        ECPoint operator+(const ECPoint& other) const;
        ECPoint operator*(const BigNum& num) const;

        std::shared_ptr<ECGroup> Group() const;

        EC_POINT* RawPtr();
        const EC_POINT* RawPtr() const;

        BigNum XCoord() const;
        BigNum YCoord() const;

        const std::string ToString() const;
    };

    ECPoint operator*(const BigNum& num, const ECPoint& point);
}

#endif