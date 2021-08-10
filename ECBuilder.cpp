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

#include "ECBuilder.h"
#include "ECGroupGFp.h"
#include "ECGroupGF2m.h"

using namespace ecc;

ECBuilder& ECBuilder::FieldSize(size_t size)
{
    fieldSize = size;
    return *this;
}

ECBuilder& ECBuilder::Prime(const BigNum& p)
{
    this->p = p;
    return *this;
}

ECBuilder& ECBuilder::Irreducible(const BigNum& p)
{
    this->p = p;
    return *this;
}

ECBuilder& ECBuilder::Order(const BigNum& order)
{
    this->order = order;
    return *this;
}

ECBuilder& ECBuilder::A(const BigNum& a)
{
    this->a = a;
    return *this;
}

ECBuilder& ECBuilder::B(const BigNum& b)
{
    this->b = b;
    return *this;
}

ECBuilder& ECBuilder::X(const BigNum& x)
{
    this->x = x;
    return *this;
}

ECBuilder& ECBuilder::Y(const BigNum& y)
{
    this->y = y;
    return *this;
}

EllipticCurve ECBuilder::BuildGFp() const
{
    CheckParams();

    auto group = std::make_shared<ECGroupGFp>(fieldSize);
    group->SetParameters(p, order, a, b, x, y);
    return EllipticCurve(group);
}

EllipticCurve ECBuilder::BuildGF2m() const
{
    CheckParams();

    auto group = std::make_shared<ECGroupGF2m>(fieldSize);
    group->SetParameters(p, order, a, b, x, y);
    return EllipticCurve(group);
}

void ECBuilder::CheckParams() const
{
    if (p.Empty()) {
        throw std::invalid_argument("ECBuilder: curve parameter p is empty");
    }

    if (order.Empty()) {
        throw std::invalid_argument("ECBuilder: curve parameter order is empty");
    }

    if (a.Empty()) {
        throw std::invalid_argument("ECBuilder: curve parameter a is empty");
    }

    if (b.Empty()) {
        throw std::invalid_argument("ECBuilder: curve parameter b is empty");
    }

    if (x.Empty()) {
        throw std::invalid_argument("ECBuilder: curve parameter x is empty");
    }

    if (y.Empty()) {
        throw std::invalid_argument("ECBuilder: curve parameter y is empty");
    }
}