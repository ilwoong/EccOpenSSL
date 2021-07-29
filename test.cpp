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

#include "CustomCurve.h"

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

static void testAddition()
{
    auto curve = CustomCurve();

    auto p1 = curve.RandomPoint();
    auto p2 = curve.RandomPoint();
    auto p3 = curve.Add(p1, p2);

    print("Point Addition", curve.IsValidPoint(p3));
    print("p1", p1);
    print("p2", p2);
    print("p3", p3);
    std::cout << std::endl;
}

static void testMultiplication()
{
    auto curve = CustomCurve();

    auto k = curve.RandomScalar();
    auto p1 = curve.RandomPoint();    
    auto p2 = curve.Multiply(k, p1);

    print("Point Multiplication", curve.IsValidPoint(p2));
    print("p1", p1);
    print("p2", p2);
    std::cout << std::endl;
}

static void testCompression()
{
    auto curve = CustomCurve();

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
    print("p2", p2);
    print(curve.Point2Vec(p2));
    std::cout << std::endl;
}

int main(int argc, const char** argv)
{
    testAddition();
    testMultiplication();
    testCompression();
    
    return 0;
}