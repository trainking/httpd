CC = gcc
CFLAGS = -g -W -Wall
LIBS = -lpthread
httpd: out/httpd.o out/response.o out/request.o out/common.o
	$(CC) $(LIBS) out/httpd.o out/response.o out/request.o out/common.o -o httpd
out/httpd.o: src/httpd.c
	$(CC) $(CFLAGS) $(LIBS) -c src/httpd.c -o out/httpd.o
out/response.o: src/response.c
	$(CC) $(CFLAGS) -c src/response.c -o out/response.o
out/request.o: src/request.c
	$(CC) $(CFLAGS) -c src/request.c -o out/request.o
out/common.o: src/common.c
	$(CC) $(CFLAGS) -c src/common.c -o out/common.o
clean:
	rm -rf out/*.o