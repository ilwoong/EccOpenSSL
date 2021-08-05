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

#include <iostream>
#include <iomanip>
#include <cstring>
#include <vector>

using namespace ecc;

static void print(const std::string& title, bool result) {
    std::cout << title << ": ";
    std::cout << std::boolalpha << result << std::noboolalpha << std::endl;
}

static void print(const std::vector<uint8_t>& vec) 
{
    std::cout << "(" << vec.size() << ") ";
    std::cout << std::hex;
    for (auto letter : vec) {
        std::cout << std::setfill('0') << std::setw(2) << +letter;
    }
    std::cout << std::dec << std::endl;
}

static void print(const std::string& name, const ECPoint& p) {
    std::cout << name << ": " <<  p.ToString() << std::endl;
}

static EllipticCurve SecgK409Curve()
{
    /* Binary data for the curve parameters for SECG K-409 curve*/
    std::vector<uint8_t> a = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
    };
    std::vector<uint8_t> b = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x01,
    };

    // if a prime field is used, p is the prime number
    // if a binary field is used, p is the irreducible polynomial
    std::vector<uint8_t> p = {
        0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x01,
    };
    std::vector<uint8_t> order = {
        0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFE, 0x5F, 0x83, 0xB2, 0xD4, 0xEA, 0x20, 0x40, 0x0E, 0xC4,
        0x55, 0x7D, 0x5E, 0xD3, 0xE3, 0xE7, 0xCA, 0x5B, 0x4B, 0x5C, 0x83, 0xB8,
        0xE0, 0x1E, 0x5F, 0xCF 
    };
    std::vector<uint8_t> x = {
        0x00, 0x60, 0xF0, 0x5F, 0x65, 0x8F, 0x49, 0xC1, 0xAD, 0x3A, 0xB1, 0x89,
        0x0F, 0x71, 0x84, 0x21, 0x0E, 0xFD, 0x09, 0x87, 0xE3, 0x07, 0xC8, 0x4C,
        0x27, 0xAC, 0xCF, 0xB8, 0xF9, 0xF6, 0x7C, 0xC2, 0xC4, 0x60, 0x18, 0x9E,
        0xB5, 0xAA, 0xAA, 0x62, 0xEE, 0x22, 0x2E, 0xB1, 0xB3, 0x55, 0x40, 0xCF,
        0xE9, 0x02, 0x37, 0x46,
    };
    std::vector<uint8_t> y ={
        0x01, 0xE3, 0x69, 0x05, 0x0B, 0x7C, 0x4E, 0x42, 0xAC, 0xBA, 0x1D, 0xAC,
        0xBF, 0x04, 0x29, 0x9C, 0x34, 0x60, 0x78, 0x2F, 0x91, 0x8E, 0xA4, 0x27,
        0xE6, 0x32, 0x51, 0x65, 0xE9, 0xEA, 0x10, 0xE3, 0xDA, 0x5F, 0x6C, 0x42,
        0xE9, 0xC5, 0x52, 0x15, 0xAA, 0x9C, 0xA2, 0x7A, 0x58, 0x63, 0xEC, 0x48,
        0xD8, 0xE0, 0x28, 0x6B,
    };

    auto builder = ECBuilder();

    builder.FieldSize(409).Irreducible(p).Order(order).A(a).B(b).X(x).Y(y);

    return builder.BuildGF2m();
}

static void testAddition(EllipticCurve& curve)
{
    auto p1 = curve.RandomPoint();
    auto p2 = curve.RandomPoint();    
    auto p3 = p1 + p2;

    print("Point Addition", curve.IsValidPoint(p3));
    print("p1", p1);
    print("p2", p2);
    print("p3", p3);
    std::cout << std::endl;
}

static void testMultiplication(EllipticCurve& curve)
{
    auto k = curve.RandomScalar();
    auto p1 = curve.RandomPoint();    
    auto p2 = k * p1;
    
    print("Point Multiplication", curve.IsValidPoint(p2));
    print("p1", p1);
    print("p2", p2);
    std::cout << std::endl;
}

static void testCompression(EllipticCurve& curve)
{
    auto p1 = curve.RandomPoint();    
    auto buf = curve.Point2VecCompressed(p1);
    std::vector<uint8_t> x;
    x.assign(buf.begin() + 1, buf.end());

    // buf[0] == 0x02 = 0; // compressed (defined in SEC 1: Elliptic Curve Cryptography, Version 2.0)
    // buf[0] == 0x03 = 1; // compressed (defined in SEC 1: Elliptic Curve Cryptography, Version 2.0)
    // buf[0] == 0x04 = uncomressed  (defined in SEC 1: Elliptic Curve Cryptography, Version 2.0)
    auto p2 = curve.Point(x, buf[0]);

    print("Point Compression", curve.IsValidPoint(p2));
    print(buf);
    print("p1", p1);
    print("compressed", p2);
    print(curve.Point2Vec(p2));
    std::cout << std::endl;
}

#include "BasisConvertMatrix.h"

int main(int argc, const char** argv)
{
    auto curve = SecgK409Curve();

    testAddition(curve);
    testMultiplication(curve);
    testCompression(curve);

    auto matrix = BasisConvertMatrix(431, 5, BigNum());

    std::cout << matrix[0][0] << std::endl;
    
    return 0;
}