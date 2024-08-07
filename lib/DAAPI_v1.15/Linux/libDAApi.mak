
CC = gcc
CPP = g++
LINK = g++ -Wl,-rpath=.

CFLAGS   = -m64 -shared -fPIC -O3 -W -Wno-format -Wno-missing-field-initializers -finput-charset=GBK
CPPFLAGS = -m64 -shared -fPIC -O3 -W -Wno-format -Wno-missing-field-initializers -finput-charset=GBK -std=c++0x
LDFLAGS  = -shared -pthread -lrt -L./openSSL/a -lssl -lcrypto -L./zlib/a -ldl -lz

CORE_INCS = -I ./include -I ./DAApi -I ./openSSL/include -I ./zlib/include

ALL_HPP = include/DAMarketStruct.h\
	include/DAFutureStruct.h\
	include/DAStockStruct.h\
	DAApi/stdafx.h \

libDAApi.so: StaticFunction.o libDAApi.a
	$(LINK) -o libDAApi.so StaticFunction.o $(LDFLAGS)

libDAApi.a: StaticFunction.o
	ar crv libDAApi.a StaticFunction.o

StaticFunction.o: $(ALL_HPP)
	$(CPP) -c $(CPPFLAGS) $(CORE_INCS) DAApi/StaticFunction.cpp

clean:
	rm -f *.o *.so *.a







