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

#ifndef __ECC_GF2_MATRIX_H__
#define __ECC_GF2_MATRIX_H__

#include "BigNum.h"
#include "GF2Polynomial.h"

namespace ecc
{
    class GF2Matrix
    {
    private:
        std::vector<std::vector<uint32_t>> elements;

    public:
        GF2Matrix() = default;
        ~GF2Matrix() = default;

        size_t Rows() const;
        size_t Cols() const;

        void AddRow(const std::vector<uint32_t>& row);
        const std::vector<uint32_t>& operator[](size_t idx) const;
    };

    GF2Polynomial operator*(const GF2Polynomial& lhs, const GF2Matrix& rhs);
}

#endif