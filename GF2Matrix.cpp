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

#include "GF2Matrix.h"
#include <stdexcept>

using namespace ecc;

size_t GF2Matrix::Rows() const
{
    return elements.size();
}

size_t GF2Matrix::Cols() const
{
    return elements[0].size();
}

void GF2Matrix::AddRow(const std::vector<uint32_t>& row)
{
    elements.push_back(row);
}

const std::vector<uint32_t>& GF2Matrix::operator[](size_t idx) const
{
    return elements[idx];
}

GF2Polynomial ecc::operator*(const GF2Polynomial& lhs, const GF2Matrix& rhs)
{
    if (lhs.Length() != rhs.Rows()) {
        throw std::invalid_argument("length mismatch");
    }

    auto res = std::vector<uint32_t>(rhs.Cols());

    auto q = lhs.Length() >> 5;
    auto r = 1 << (lhs.Length() & 0x1f);

    size_t row = 0;
    for (auto i = 0; i < q; ++i) {
        int bitmask = 1;
        do {
            auto b = lhs[i] & bitmask;
            if (b != 0) {
                for (auto col = 0; col < rhs.Cols(); ++col) {
                    res[col] ^= rhs[row][col];
                }
            }

            row += 1;
            bitmask <<= 1;
        } while (bitmask != 0);
    }

    uint32_t bitmask = 1;
    while (bitmask != r) {
        auto b = lhs[q] & bitmask;
        if (b != 0) {
            for (auto col = 0; col < rhs.Cols(); ++col) {
                res[col] ^= rhs[row][col];
            }
        }

        row += 1;
        bitmask <<= 1;
    }

    return GF2Polynomial(lhs.Length(), res);
}