PWD := $(shell pwd)
KDIR := /lib/modules/$(shell uname -r)/build

obj-m += param.o

SUBDIRS := $(PWD)
COMMON_OPS = -C $(KDIR) M='$(SUBDIRS)'

deafult: 
	$(MAKE) $(COMMON_OPS) modules

clean:
	rm -rf *.o *.ko *.cmd *.mod.c *.mod .*.cmd .tmp_versions modules.order Module.symvers *~