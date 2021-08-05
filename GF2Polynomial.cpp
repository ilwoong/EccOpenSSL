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

#include "GF2Polynomial.h"
#include <array>

using namespace ecc;

static std::array<uint32_t, 33> BITMASK = {
    0x00000001, 0x00000002, 0x00000004, 0x00000008, 0x00000010, 0x00000020, 0x00000040, 0x00000080,
    0x00000100, 0x00000200, 0x00000400, 0x00000800, 0x00001000, 0x00002000, 0x00004000, 0x00008000,
    0x00010000, 0x00020000, 0x00040000, 0x00080000, 0x00100000, 0x00200000, 0x00400000, 0x00800000, 
    0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000,
    0x00000000, 
};

static std::array<uint32_t, 33> REVERSE_RIGHT_MASK = {
    0x00000000, 0x00000001, 0x00000003, 0x00000007, 0x0000000f, 0x0000001f, 0x0000003f, 0x0000007f, 
    0x000000ff, 0x000001ff, 0x000003ff, 0x000007ff, 0x00000fff, 0x00001fff, 0x00003fff, 0x00007fff, 
    0x0000ffff, 0x0001ffff, 0x0003ffff, 0x0007ffff, 0x000fffff, 0x001fffff, 0x003fffff, 0x007fffff, 
    0x00ffffff, 0x01ffffff, 0x03ffffff, 0x07ffffff, 0x0fffffff, 0x1fffffff, 0x3fffffff, 0x7fffffff,
    0xffffffff
};

GF2Polynomial::GF2Polynomial() : GF2Polynomial(0)
{}

GF2Polynomial::GF2Polynomial(size_t length)
{
    if (length < 1) {
        this->length = 1;
    }

    value = std::vector<uint32_t>((this->length + 31) >> 5);
}

GF2Polynomial::GF2Polynomial(size_t length, const std::vector<uint32_t>& data) : GF2Polynomial(length)
{
    auto bytes = (length + 31) >> 5;
    auto bits = length % 32;
    auto min = (bytes < data.size()) ? bytes : data.size();
    std::copy(data.begin(), data.begin() + min, value.begin());

    ZeroUnusedBits();
}

GF2Polynomial::GF2Polynomial(size_t length, const BigNum& num) : GF2Polynomial(length)
{
    auto val = num.ToByteVector();

    auto i = 0;
    for (; i <= (val.size() - 4) >> 2; ++i) {
        auto m = val.size() - 1 - (i << 2);
        value[i] = (val[m]) & 0x000000ff;
        value[i] |= (val[m - 1]) & 0x0000ff00;
        value[i] |= (val[m - 2]) & 0x00ff0000;
        value[i] |= (val[m - 3]) & 0xff000000;
    }

    if ((length & 0x1f) != 0) {
        value[i] &= REVERSE_RIGHT_MASK[length & 0x1f];
    }
}

GF2Polynomial::GF2Polynomial(const GF2Polynomial& other) : GF2Polynomial(other.length, other.value)
{}

GF2Polynomial::~GF2Polynomial()
{}

size_t GF2Polynomial::Length() const 
{
    return length;
}

std::vector<uint32_t> GF2Polynomial::Value() const
{
    return value;
}

bool GF2Polynomial::IsZero() const
{
    for (auto iter = value.rbegin(); iter != value.rend(); ++iter) {
        if (*iter != 0) {
            return false;
        }
    }

    return true;
}

uint8_t GF2Polynomial::GetBit(size_t idx) const
{
    auto byteIdx = (idx >> 5);
    auto bitIdx = idx % 32;
    return (value[byteIdx] >> bitIdx) & 0x1;
}

void GF2Polynomial::SetBit(size_t idx)
{
    if (length <= idx) {
        throw std::out_of_range("idx is out of range");
    }

    auto byteIdx = (idx >> 5);
    auto bitIdx = idx % 32;
    value[byteIdx] |= BITMASK[bitIdx];
}

void GF2Polynomial::SetBit(const std::initializer_list<size_t>& idxList)
{
    for (auto idx : idxList) {
        SetBit(idx);
    }
}

GF2Polynomial GF2Polynomial::Expand(size_t length) const
{
    if (this->length >= length) {
        return GF2Polynomial(*this);
    }

    return GF2Polynomial(length, value);
}

GF2Polynomial GF2Polynomial::Reduce() const
{
    int64_t len = length;
    for (auto iter = value.rbegin(); iter != value.rend(); ++iter) {
        auto block = *iter;
        if (block != 0) {
            for (auto i = 0; i < 32; ++i) {
                if ((block & 0x80000000) == 0x80000000) {
                    break;
                }
                block <<= 1;
                len -= 1;
            }
            break;
        }

        len -= 32;
    }

    if (len < 0) {
        len = 0;
    }

    return GF2Polynomial(len, value);
}

GF2Polynomial GF2Polynomial::ShiftLeft() const
{
    auto result = GF2Polynomial(length + 1, value);

    for (auto i = value.size() - 1; i >= 1; --i) {
        result.value[i] <<= 1;
        result.value[i] |= value[i - 1] >> 31;
    }
    result.value[0] <<= 1;

    return result;
}

GF2Polynomial GF2Polynomial::ShiftLeft(size_t numBits) const
{
    auto result = GF2Polynomial(*this);

    if (numBits >= 32) {
        result = result.ShiftBlocksLeft(numBits >> 5);
    }

    result.Expand(numBits);

    auto remaining = numBits % 32;

    if (remaining > 0) {
        for (auto i = result.value.size() - 1; i >= 1; --i) {
            result.value[i] <<= remaining;
            result.value[i] |= value[i - 1] >> (32 - remaining);
        }
        result.value[0] <<= 1;
    }

    return result;
}

GF2Polynomial GF2Polynomial::ShiftBlocksLeft() const
{
    auto result = GF2Polynomial(*this);

    result.value.insert(result.value.begin(), 0);

    return result;
}

GF2Polynomial GF2Polynomial::ShiftBlocksLeft(size_t numBlocks) const
{
    auto result = GF2Polynomial(*this);
    std::vector<uint32_t> zeros(numBlocks);

    result.value.insert(result.value.begin(), zeros.begin(), zeros.end());

    return result;
}

GF2Polynomial& GF2Polynomial::operator=(const GF2Polynomial& other)
{
    length = other.length;
    value = other.value;
    return *this;
}

GF2Polynomial& GF2Polynomial::operator^=(const GF2Polynomial& other)
{
    if (other.length > length) {
        throw std::invalid_argument("other should be shorter than this object");
    }

    for (auto i = 0; i < other.value.size(); ++i) {
        value[i] ^= other.value[i];
    }

    return *this;
}

GF2Polynomial GF2Polynomial::operator^(const GF2Polynomial& other) const
{
    GF2Polynomial lhs = GF2Polynomial(*this);
    GF2Polynomial rhs = other;
    auto min = lhs.value.size() < rhs.value.size() ? lhs.value.size() : rhs.value.size();

    lhs = lhs.Expand(rhs.Length());
    for (auto i = 0; i < min; ++i) {
        lhs.value[i] ^= rhs.value[i];
    }

    lhs.ZeroUnusedBits();

    return lhs;
}

GF2Polynomial GF2Polynomial::operator+(const GF2Polynomial& other) const
{
    return *this ^ other;
}

GF2Polynomial GF2Polynomial::operator-(const GF2Polynomial& other) const
{    
    return *this ^ other;
}

// classic multiplication, if speedup needed, consider Karatsuba algorithm 
GF2Polynomial GF2Polynomial::operator*(const GF2Polynomial& other) const
{
    auto max = length > other.length ? length : other.length;
    auto result = GF2Polynomial(max << 1);
    auto m = std::array<GF2Polynomial, 32>();

    m[0] = GF2Polynomial(*this);
    for (auto i = 1; i < 32; ++i) {
        m[i] = m[i - 1].ShiftLeft();
    }

    for (auto i = 0; i < other.value.size(); ++i) {
        for (auto j = 0; j < 32; ++j) {
            if ((other.value[i] & BITMASK[j]) != 0) {
                result ^= m[j];
            }
        }
        for (auto j = 0; j < 32; ++j) {
            m[j].ShiftBlocksLeft();
        }
    }

    return result;
}

GF2Polynomial GF2Polynomial::operator%(const GF2Polynomial& other) const
{
    if (other.IsZero()) {
        throw std::invalid_argument("devide by zero is not allowed");
    }

    auto lhs = GF2Polynomial(*this);
    auto rhs = GF2Polynomial(other);

    lhs = lhs.Reduce();
    rhs = rhs.Reduce();

    if (lhs.length < rhs.length) {
        return lhs;
    }

    while (lhs.length >= rhs.length) {
        auto tmp = rhs.ShiftLeft(lhs.length - rhs.length);
        lhs = lhs - tmp;        
        lhs = lhs.Reduce();
    }

    return lhs;
}

void GF2Polynomial::ZeroUnusedBits()
{
    if ((length & 0x1f) != 0) {
        value[value.size() - 1] &= REVERSE_RIGHT_MASK[length & 0x1f];
    }
}