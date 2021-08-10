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

#include "BigNum.h"
#include "ECPoint.h"

#include <sstream>
#include <iomanip>

using namespace ecc;

BigNum::BigNum() : BigNum(nullptr)
{}

BigNum::BigNum(const BigNum& other) : BigNum(BN_dup(other.num))
{}

BigNum::BigNum(const std::vector<uint8_t>& data) : BigNum(BN_bin2bn(data.data(), data.size(), NULL))
{}

BigNum::BigNum(BIGNUM* bn) : num(bn)
{}

BigNum::~BigNum()
{
    if (num != nullptr) {
        BN_free(num);
        num = nullptr;
    }
}

bool BigNum::Empty() const
{
    return num == nullptr;
}

size_t BigNum::BitLength() const
{
    return BN_num_bits(num);
}

BIGNUM* BigNum::Data() const
{
    return num;
}

BigNum& BigNum::operator=(const BigNum& other)
{
    if (num != nullptr) {
        BN_free(num);
        num = nullptr;
    }

    if (other.num != nullptr) {
        num = BN_dup(other.num);
    }

    return *this;
}

const std::string BigNum::ToString() const
{
    auto vec = ToByteVector();
    
    std::ostringstream oss;
    
    oss << std::hex;
    for (auto letter : vec) {
        oss << std::setfill('0') << std::setw(2) << +letter;
    }
    oss << std::dec;

    return oss.str();
}

std::vector<uint8_t> BigNum::ToByteVector() const
{
    auto len = BN_num_bytes(num);
    std::vector<uint8_t> result(len);
    BN_bn2bin(num, result.data());

    return result;
}