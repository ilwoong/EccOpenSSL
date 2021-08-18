CC = g++
CPPFLAGS = -std=c++11 -O2
SRC = \
	EllipticCurve.cpp \
	ECBuilder.cpp \
	ECGroup.cpp \
	ECGroupGFp.cpp \
	ECGroupGF2m.cpp \
	ECPoint.cpp \
	BigNum.cpp \
	GF2Polynomial.cpp \
	GF2Matrix.cpp \
	BasisConversion.cpp \

.PHONY: all clean

all : libecc test

libecc : $(SRC)
	$(CC) $(CPPFLAGS) $^ -o $@.so -shared -fPIC -lcrypto

test : test.cpp
	$(CC) $(CPPFLAGS) $^ -o $@ -L. -lecc

clean:
	rm -rf libecc.so test