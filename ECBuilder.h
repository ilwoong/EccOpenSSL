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

#ifndef __ECC_EC_BUILDER_H__
#define __ECC_EC_BUILDER_H__

#include <vector>
#include <cstdint>

#include "BigNum.h"
#include "EllipticCurve.h"

namespace ecc
{
    class ECBuilder
    {
    public:
        size_t fieldSize;
        BigNum p;
        BigNum order;
        BigNum a;
        BigNum b;
        BigNum x;
        BigNum y;
        BigNum root;

    public:
        ECBuilder() = default;
        ~ECBuilder() = default;

        ECBuilder& FieldSize(size_t size);
        ECBuilder& Prime(const BigNum& p);
        ECBuilder& Irreducible(const BigNum& p);
        ECBuilder& Order(const BigNum& order);
        ECBuilder& A(const BigNum& a);
        ECBuilder& B(const BigNum& b);
        ECBuilder& X(const BigNum& x);
        ECBuilder& Y(const BigNum& y);
        ECBuilder& Root(const BigNum& root);

        EllipticCurve BuildGFp() const;
        EllipticCurve BuildGF2m() const;

    private:
        void CheckParams() const;
    };
}

#endif