# Le Makefile qui met la pression, gaaaaarçon ! *wesh wesh*

SRC = \
foliage/overload_new_delete.cpp \
foliage/foliage.cpp \
foliage/timer.cpp \
foliage/memory.cpp \
foliage/fastmath.cpp \
foliage/basic_types.cpp \
foliage/leaf_graphic/screen.cpp \
foliage/leaf_graphic/surface.cpp \
foliage/leaf_graphic/sprite.cpp \
foliage/leaf_graphic/bitmap_loader.cpp \
foliage/leaf_graphic/instancizator.cpp \
foliage/leaf_io/leaf_io.cpp \
foliage/leaf_sound/leaf_sound.cpp \
foliage/leaf_sound/sound.cpp \
game/start.cpp \
game/bullet.cpp \
game/font.cpp \
game/star.cpp 

all: boot.me

clean:
	rm -f executable.elf boot.me disassembly.txt

boot.me: executable.elf
	powerpc-eabi-objcopy -O srec executable.elf boot.me

executable.elf: $(SRC) 
	powerpc-eabi-gcc -O3 $(SRC) -o executable.elf -I../ppc405_0/include/ -I./foliage/ \
	  -L../ppc405_0/lib/ -lxil -lstdc++ -pipe -Wall -Wl,-T -Wl,foliage_linker_script.ld -Wl,-O -Wl,-S -Wl,--gc-sections
	@echo ===================================================================
	@powerpc-eabi-size executable.elf
	@echo ===================================================================

copy_on_cf: boot.me
	cp -f boot.me /cygdrive/d
	@echo Done!
	
disassemble: executable.elf
	powerpc-eabi-objdump -D executable.elf > disassembly.txt
	@echo Done!
