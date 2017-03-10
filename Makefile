#
# Makefile for BigXionFlasher for Linux and Windows (MinGW)
# Copyright (c) 2011-2017 by Thomas König <info@bigxionflasher.org>.
# All rights reserved.
#

TINY_CAN_API = tinycan

CC = gcc
LD = gcc
INCLUDE = -I$(TINY_CAN_API)
LDFLAGS = -Wall

UNAME := $(shell uname)

ifeq ($(UNAME),Linux)
 API_DRV = can_drv_linux
 LIB = -ldl -lc
 CFLAGS = -Wall -pthread -g0 -O2 
else
 API_DRV = can_drv_win
 LIB =
 CFLAGS = -Wall -g0 -O2
endif

OBJS = obj/bigXionFlasher.o obj/$(API_DRV).o

all: $(OBJS)
	$(LD) $(LDFLAGS) -o bigXionFlasher $(OBJS) $(LIB)

obj/bigXionFlasher.o: src/bigXionFlasher.c
	$(CC) $(CFLAGS) $(INCLUDE) -o obj/bigXionFlasher.o -c src/bigXionFlasher.c

obj/$(API_DRV).o: $(TINY_CAN_API)/$(API_DRV).c
	$(CC) $(CFLAGS) $(INCLUDE) -o obj/$(API_DRV).o -c $(TINY_CAN_API)/$(API_DRV).c

clean:	
	rm -f obj/*.o *~ ./bigXionFlasher ./bigXionFlasher.exe
