CC=gcc
CFLAGS=-I.

all :  tcp

tcp: tcpS tcpC 
udp: udpS udpC

tcpS : 
	$(CC) tcpserver.c -o tcpS $(CFLAGS)

tcpC :
	$(CC) tcpclient.c -o tcpC $(CFLAGS)

udpS : 
	$(CC) udpserver.c -o udpS $(CFLAGS)

udpC :
	$(CC) udpclient.c -o udpC $(CFLAGS)

clean:
	rm udpS udpC tcpS tcpC