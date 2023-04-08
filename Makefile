.ONESHELL:

include ./config.mk

boot:
	$(MAKE) -C boot

kernel:
	$(MAKE) -C kernel

all:
	@echo "Nothing"

.PHONY: all check boot kernel
