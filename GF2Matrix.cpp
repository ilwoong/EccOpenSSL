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
#include <sstream>
#include <iomanip>

using namespace ecc;

GF2Matrix::GF2Matrix(const GF2Matrix& other) : elements(other.elements)
{}

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

const std::string GF2Matrix::ToString() const
{
    std::ostringstream oss;

    for (auto i = 0; i < Rows(); ++i) {
        for (auto j = 0; j < Cols(); ++j) {
            oss << std::hex << std::setfill('0') << std::setw(8) << +elements[i][j] << " ";
        }
        oss << std::endl;
    }

    return oss.str();
}

GF2Matrix GF2Matrix::Invert() const
{
    auto tmpElements = std::vector<std::vector<uint32_t>>(elements);
    auto invElements = UnitElements();

    for (auto i = 0; i < Rows(); ++i) {
        int q = i >> 5;
        int bitMask = 1 << (i & 0x1f);

        if ((tmpElements[i][q] & bitMask) == 0) {
            bool foundNonZero = false;

            for (int j = i + 1; j < Rows(); ++j) {
                if ((tmpElements[j][q] & bitMask) != 0) {
                    foundNonZero = true;
                    SwapRows(tmpElements, i, j);
                    SwapRows(invElements, i, j);
                    j = Rows();
                    continue;
                }
            }

            if (!foundNonZero) {
                throw std::logic_error("The matrix is not invertible.");
            }
        }

        for (int j = Rows() - 1; j >= 0; --j) {
            if ((j != i) && ((tmpElements[j][q] & bitMask) != 0)) {
                AddToRow(tmpElements[j], tmpElements[i], q);
                AddToRow(invElements[j], invElements[i], 0);
            }
        }
    }

    GF2Matrix result;
    for (auto row : invElements) {
        result.AddRow(row);
    }

    return result;
}

std::vector<std::vector<uint32_t>> GF2Matrix::UnitElements() const
{
    auto unitElements = std::vector<std::vector<uint32_t>>();
    for (auto row : elements) {
        unitElements.push_back(std::vector<uint32_t>(Cols()));
    }
    for (int i = Rows() - 1; i >= 0; --i) {
        int q = i >> 5;
        int r = i & 0x1f;
        unitElements[i][q] = 1 << r;
    }

    return unitElements;
}

void GF2Matrix::SwapRows(std::vector<std::vector<uint32_t>>& elements, size_t from, size_t to) const
{
    elements[from].swap(elements[to]);
}

void GF2Matrix::AddToRow(std::vector<uint32_t>& dst, const std::vector<uint32_t>& src, size_t startIdx) const
{
    for (int i = dst.size() - 1; i >= static_cast<int>(startIdx); --i) {
        dst[i] = src[i] ^ dst[i];
    }
}

GF2Polynomial ecc::operator*(const GF2Polynomial& lhs, const GF2Matrix& rhs)
{
    if (lhs.Length() != rhs.Rows()) {
        throw std::invalid_argument("length mismatch");
    }

    auto res = std::vector<uint32_t>(rhs.Cols(), 0);

    auto q = rhs.Rows() >> 5;
    auto r = 1 << (rhs.Rows() & 0x1f);

    size_t row = 0;
    for (auto i = 0; i < q; ++i) {
        uint32_t bitmask = 1;
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