TARGET=netlink_pong
OBJS=netlink_pong.o
MDIR=drivers/misc

EXTRA_CFLAGS = -DEXPORT_SYMTAB
CURRENT = $(shell uname -r)
KDIR = /lib/modules/$(CURRENT)/build
PWD = $(shell pwd) 
PWD += ../ 
DEST = /lib/modules/$(CURRENT)/kernel/$(MDIR)

obj-m      := $(TARGET).o
uds-objs   := $(OBJS)
default:
	make -C $(KDIR) SUBDIRS=$(PWD) modules

$(TARGET).o: $(OBJS)
	$(LD) $(LD_RFLAG) -r -o $@ $(OBJS)

ifneq (,$(findstring 2.4.,$(CURRENT)))
install:
	su -c "cp -v $(TARGET).o $(DEST) && /sbin/depmod -a"
else
install:
	su -c "cp -v $(TARGET).ko $(DEST) && /sbin/depmod -a"
endif

clean:
	-rm -f *.o *.ko .*.cmd .*.flags *.mod.c

-include $(KDIR)/Rules.make
