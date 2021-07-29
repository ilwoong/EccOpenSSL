CC = g++

all : test

test : test.cpp CustomCurve.cpp ECGroup.cpp ECPoint.cpp BigNum.cpp
	$(CC) $^ -o $@ -lcrypto

clean:
	rm -rf test