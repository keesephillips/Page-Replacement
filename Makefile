CFLAGS=-g
CC=/usr/bin/gcc

all: vm 

vm: vm.c vm.h pagetable.c disk.c list.c pt_core.c
		$(CC) $(CFLAGS) vm.c pagetable.c disk.c list.c pt_core.c -g -o vm

clean:
		rm -f vm input_gen list_test
