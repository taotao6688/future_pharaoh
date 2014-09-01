
CC=g++

CFLAGS= -O2 -fPIC

LIB= -L ../KSTradeAPI/linux64 \
     -L ../KSMarketDataAPI/linux64 \
     -lkstradeapi \
     -lksmarketdataapi \
     -lkslkc64r

TARGET=Main

ifeq ($(vtype),32r)
    CFLAGS+=-m32
    LIB:=$(subst linux64,linux32,${LIB})
    LIB:=$(subst lkslkc64r,lkslkc32r,${LIB})
    TARGET:=Main
endif

ifeq ($(vtype),64r)
endif

all: ${TARGET}
	cp -f ${TARGET} ../run/

${TARGET}: event.o Socket.o ClientSocket.o MarketHandler.o MarketSubscriber.o Main.o
	${CC} ${CFLAGS} -o $@ $^  ${LIB} 

event.o:event.cpp
	${CC} ${CFLAGS} -o $@ -c $^  

Socket.o: Socket.cpp
	${CC} ${CFLAGS} -o $@ -c $^  

ClientSocket.o: ClientSocket.cpp
	${CC} ${CFLAGS} -o $@ -c $^  

MarketHandler.o: MarketHandler.cpp
	${CC} ${CFLAGS} -o $@ -c $^ 

MarketSubscriber.o: MarketSubscriber.cpp
	${CC} ${CFLAGS} -o $@ -c $^ 

Main.o: Main.cpp
	${CC} ${CFLAGS} -o $@ -c $^ 

clean:
	rm -f *.o Main
