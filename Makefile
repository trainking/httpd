all: httpd

OUT_PATH = out
$(shell mkdir -p $(OUT_PATH)/)
BIN_PATH = bin
$(shell mkdir -p $(BIN_PATH)/)
SRC_PATH = src
CC = gcc
CFLAGS = -g -W -Wall
LIBS = -lpthread -lm

httpd: $(OUT_PATH)/httpd.o $(OUT_PATH)/response.o $(OUT_PATH)/request.o $(OUT_PATH)/common.o
	$(CC) $(CFLAGS) $(OUT_PATH)/httpd.o $(OUT_PATH)/response.o $(OUT_PATH)/request.o $(OUT_PATH)/common.o -o $(BIN_PATH)/httpd $(LIBS)
$(OUT_PATH)/httpd.o: $(SRC_PATH)/httpd.c
	$(CC) $(CFLAGS) $(LIBS) -c $(SRC_PATH)/httpd.c -o $(OUT_PATH)/httpd.o
$(OUT_PATH)/response.o: $(SRC_PATH)/response.c
	$(CC) $(CFLAGS) -c $(SRC_PATH)/response.c -o $(OUT_PATH)/response.o
$(OUT_PATH)/request.o: $(SRC_PATH)/request.c
	$(CC) $(CFLAGS) -c $(SRC_PATH)/request.c -o $(OUT_PATH)/request.o
$(OUT_PATH)/common.o: $(SRC_PATH)/common.c
	$(CC) $(CFLAGS) -c $(SRC_PATH)/common.c -o $(OUT_PATH)/common.o
clean:
	rm -rf $(OUT_PATH)/ && rm -f $(BIN_PATH)/httpd
test:
	$(BIN_PATH)/httpd --version