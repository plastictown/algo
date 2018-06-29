CC:=gcc

SRC_DIR:=src
INC_DIR:=hdr
OBJ_DIR:=obj
BIN_DIR:=bin
APP_DIR:=app

CFLAGS:=-O3 -I. -I$(INC_DIR)
LDFLAGS:=-static

ifeq ($(STDOUT),1)
  CFLAGS:=$(CFLAGS) -DSTDIO_OUT
endif

all: make_obj
	gcc -o $(BIN_DIR)/out.exe $(LDFLAGS) \
		$(OBJ_DIR)/main.o \
		$(OBJ_DIR)/util.o \
		$(OBJ_DIR)/sort.o \
		$(OBJ_DIR)/listd.o
		
make_obj:
	gcc -c -o $(OBJ_DIR)/main.o $(CFLAGS) $(APP_DIR)/main.c
	gcc -c -o $(OBJ_DIR)/util.o $(CFLAGS) $(SRC_DIR)/util.c
	gcc -c -o $(OBJ_DIR)/sort.o $(CFLAGS) $(SRC_DIR)/sort.c
	gcc -c -o $(OBJ_DIR)/listd.o $(CFLAGS) $(SRC_DIR)/listd.c
	
clean:
	rm -f $(BIN_DIR)/*.*
	rm -f $(OBJ_DIR)/*.*