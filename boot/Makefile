.PHONY: all clean 32bit 64bit

all: 32bit 64bit

32bit:
	@echo "Building 32-bit bootloader..."
	@$(MAKE) -C 32bit all

64bit:
	@echo "Building 64-bit bootloader..."
	@$(MAKE) -C 64bit all

clean:
	@echo "Cleaning 32-bit bootloader..."
	@$(MAKE) -C 32bit clean
	@echo "Cleaning 64-bit bootloader..."
	@$(MAKE) -C 64bit clean
