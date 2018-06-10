CC:=gcc

SRC_DIR:=src
INC_DIR:=hdr
OBJ_DIR:=obj
BIN_DIR:=bin
APP_DIR:=app

CFLAGS:=-O3 -I.

ifeq ($(STDOUT),1)
  CFLAGS:=$(CFLAGS) -DSTDIO_OUT
endif

all: make_obj
	gcc -o $(BIN_DIR)/out.exe $(LDFLAGS) \
		$(OBJ_DIR)/main.o
		
make_obj: $(APP_DIR)/main.c
	gcc -c -o $(OBJ_DIR)/main.o $(CFLAGS) $(APP_DIR)/main.c
	
clean:
	rm -f $(BIN_DIR)/*.*
	rm -f $(OBJ_DIR)/*.*