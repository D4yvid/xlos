# Architecture-specific toolchains are defined in their respective Makefiles:
# - boot/32bit/Makefile uses i686-elf-* toolchain for 32-bit protected mode
# - boot/64bit/Makefile uses x86_64-elf-* toolchain for 64-bit long mode
#
# Required toolchains:
# - i686-elf-gcc, i686-elf-ld, i686-elf-as (32-bit)
# - x86_64-elf-gcc, x86_64-elf-ld, x86_64-elf-as (64-bit)
