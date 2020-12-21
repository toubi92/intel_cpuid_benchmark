BASEDIR ?= $(PWD)
BUILD ?= obj
PREFIX ?= /usr
LIBDIR ?= lib
includedir = $(PREFIX)/include
libdir = $(PREFIX)/$(LIBDIR)
KERNEL_SRC_DIR = /lib/modules/`uname -r`/build
KERNEL_CFLAGS += -g -DDEBUG

obj-m += bm_mod.o
bm_mod-objs = $(patsubst ${BASEDIR}/%.c,%.o,$(wildcard ${BASEDIR}/mod/*.c))

lib-obj-y = $(patsubst %.c,%.o,$(wildcard lib/*.c))
lib-objs = $(addprefix $(BUILD)/, $(lib-obj-y))

ccflags-y += -I$(PWD)/include
libbm.so: ccflags-y += -fPIC -Iinclude
libbm.so: LDFLAGS += -shared

.PHONY: all install uninstall clean

all: clean bm_mod.ko libbm.so

bm_mod.ko:
	@make -C $(KERNEL_SRC_DIR) M=`pwd` modules EXTRA_CFLAGS="$(KERNEL_CFLAGS)"

libbm.so: $(lib-objs)
	@mkdir -p $(dir $@)
	$(CC) $(lib-objs) -o $@ $(LDFLAGS) $(ccflags-y) $(LIBS)

$(BUILD)/%.o: %.c
	@mkdir -p "$(dir $@)"
	$(CC) -c $< -o $@ $(ccflags-y) -MT $@ -MMD -MP -MF $(@:.o=.d)

install:
	install -m 755 -d "$(DESTDIR)$(libdir)"
	install -m 644 include/bm_kit.h "$(DESTDIR)$(includedir)/"
	install -m 644 libbm.so "$(DESTDIR)$(libdir)/libbm.so"

uninstall:
	rm "$(DESTDIR)$(libdir)/libbm.so"
	rm "$(DESTDIR)$(includedir)/bm_kit.h"

clean: 
	make -C /lib/modules/`uname -r`/build M=$(PWD) clean
	rm -rf $(BUILD)
	rm -rf libbm.so

