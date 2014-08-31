
CC=g++

CFLAGS= -O2 -fPIC

LIB= -L ../KSTradeAPI/linux64 \
     -lkstradeapi \
     -lkslkc64r

TARGET=servant_instrument

ifeq ($(vtype),32r)
    CFLAGS+=-m32
    LIB:=$(subst linux64,linux32,${LIB})
    LIB:=$(subst lkslkc64r,lkslkc32r,${LIB})
    TARGET:=main_32
endif

ifeq ($(vtype),64r)
endif

all: ${TARGET}
	cp -f ${TARGET} ../run/

${TARGET}: event.o Socket.o ClientSocket.o servant_instrument.o
	${CC} ${CFLAGS} -o $@ $^  ${LIB} 

event.o:event.cpp
	${CC} ${CFLAGS} -o $@ -c $^  

Socket.o: Socket.cpp
	${CC} ${CFLAGS} -o $@ -c $^  

ClientSocket.o: ClientSocket.cpp
	${CC} ${CFLAGS} -o $@ -c $^  

servant_instrument.o: servant_instrument.cpp
	${CC} ${CFLAGS} -o $@ -c $^ 

clean:
	rm -f *.o servant_instrument
