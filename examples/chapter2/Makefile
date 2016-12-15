XEN_TARGET_ARCH = x86_64
ARCH_CFLAGS := -m64
ARCH_ASFLAGS := -m64
ARCH_LDFLAGS := -m elf_x86_64 -T x86_64.lds
ARCH_OBJS := bootstrap.x86_64.o

CPPFLAGS += -I../xen/xen/include/public $(ARCH_CPPFLAGS)
LDFLAGS  += -nostdlib $(ARCH_LDFLAGS)
CFLAGS   += -Wall -Werror -std=c99 $(ARCH_CFLAGS)
ASFLAGS   = -D__ASSEMBLY__ $(ARCH_ASFLAGS)

.PHONY: all clean run

all: testkernel

testkernel: $(ARCH_OBJS) kernel.o
	$(LD) $(LDFLAGS) $^ -o testkernel

clean:
	rm -f *.o 
	rm -f testkernel
