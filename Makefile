# all: httpd
# LIBS = -lpthread #-lsocket
# httpd: httpd.c
# 	gcc -g -W -Wall $(LIBS) -o $@ $<
# clean:
# 	rm httpd

CC = gcc
CFLAGS = -g -W -Wall
LIBS = -lpthread
httpd: httpd2.out response.out
	$(CC) $(LIBS) httpd2.out response.out -o httpd
httpd2.out: httpd2.c
	$(CC) $(CFLAGS) $(LIBS) -c httpd2.c -o httpd2.out
response.out: response.c
	$(CC) $(CFLAGS) -c response.c -o response.out
clean:
	rm -rf *.out