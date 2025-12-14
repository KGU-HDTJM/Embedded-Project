KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

obj-m += digitCtrl.o

USER_SRC := proj2.c
USER_BIN := proj2

all: module user

module:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

user:
	$(CC) $(USER_SRC) -o $(USER_BIN)

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -f $(USER_BIN)
