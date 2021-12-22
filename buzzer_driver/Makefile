obj-m += buzzer_driver.o
KDIR = ~/working/kernel

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
