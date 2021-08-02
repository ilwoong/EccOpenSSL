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
#include "ECGroupGF2m.h"

using namespace ecc;

ECBuilder& ECBuilder::SetIrreducible(const BigNum& p)
{
    this->p = p;
    return *this;
}

ECBuilder& ECBuilder::SetOrder(const BigNum& order)
{
    this->order = order;
    return *this;
}

ECBuilder& ECBuilder::SetA(const BigNum& a)
{
    this->a = a;
    return *this;
}

ECBuilder& ECBuilder::SetB(const BigNum& b)
{
    this->b = b;
    return *this;
}

ECBuilder& ECBuilder::SetX(const BigNum& x)
{
    this->x = x;
    return *this;
}

ECBuilder& ECBuilder::SetY(const BigNum& y)
{
    this->y = y;
    return *this;
}

EllipticCurve ECBuilder::Build() const
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

    auto group = std::make_shared<ECGroupGF2m>(409);
    group->SetParameters(p, order, a, b, x, y);
    return EllipticCurve(group);
}