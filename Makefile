CC = g++

all : test

test : test.cpp EllipticCurve.cpp ECBuilder.cpp ECGroup.cpp ECGroupGFp.cpp ECGroupGF2m.cpp ECPoint.cpp BigNum.cpp
	$(CC) $^ -o $@ -lcrypto

clean:
	rm -rf test