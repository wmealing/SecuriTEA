obj-m := canary-exposed.o

# NOSTACKPROTECTION=-fno-stack-protector 
# I do know the evil here.. we'll get to this. 
# KBUILD_CFLAGS += $(NOSTACKPROTECTION)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD)

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD)
	rm -rf *.o *.ko  modules.order Module.symvers  *.mod.c  .* 
