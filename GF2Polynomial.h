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

#ifndef __ECC_GF2_POLYNOMIAL_H__
#define __ECC_GF2_POLYNOMIAL_H__

#include "BigNum.h"
#include <vector>
#include <cstdint>

namespace ecc 
{
    class GF2Polynomial 
    {
    public:
        size_t length;
        std::vector<uint32_t> value;
    
    public:
        GF2Polynomial();
        GF2Polynomial(size_t length);
        GF2Polynomial(size_t length, const std::vector<uint32_t>& data);
        GF2Polynomial(size_t length, const BigNum& num);
        GF2Polynomial(const GF2Polynomial& other);
        ~GF2Polynomial();

        size_t Length() const;
        std::vector<uint32_t> Value() const;

        bool IsZero() const;
        uint8_t GetBit(size_t idx) const;
        void SetBit(size_t idx);
        void SetBit(const std::initializer_list<size_t>& idxList);
        
        GF2Polynomial Expand(size_t length) const;
        GF2Polynomial Reduce() const;
        
        GF2Polynomial ShiftLeft() const;
        GF2Polynomial ShiftLeft(size_t numBits) const;
        GF2Polynomial ShiftBlocksLeft() const;
        GF2Polynomial ShiftBlocksLeft(size_t numBlocks) const;
        
        GF2Polynomial& operator=(const GF2Polynomial& other);
        GF2Polynomial& operator^=(const GF2Polynomial& other);

        GF2Polynomial operator^(const GF2Polynomial& other) const;
        GF2Polynomial operator+(const GF2Polynomial& other) const;
        GF2Polynomial operator-(const GF2Polynomial& other) const;
        GF2Polynomial operator*(const GF2Polynomial& other) const;
        GF2Polynomial operator%(const GF2Polynomial& other) const;

    private:
        void ZeroUnusedBits();
    };
}

#endif