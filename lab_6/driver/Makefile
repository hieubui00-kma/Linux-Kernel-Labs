KDIR:= /lib/modules/`uname -r`/build

all:
	@make -C $(KDIR) M=`pwd` --no-print-directory

clean:
	@make -C $(KDIR) M=`pwd` clean --no-print-directory
