CC = clang

APP = conv_sharp.exe
HEADERS = $(wildcard *.h)

all: $(APP)

clean:
	rm -f $(APP)

run: $(APP)
	./$(APP)

#%.exe: %.oo Makefile
#	$(CC) -o $@ -O3 $(filter %.oo,$^) -lm
#
#%.oo: %.ss Makefile
#	$(CC) -c -o $@ -x assembler $(filter %.ss,$^)
#
#%.ss: %.c Makefile
#	$(CC) -S -o $@ -O3 $(filter %.c,$^)

%.exe: %.c $(HEADERS) Makefile
	$(CC) -o $@ -O3 $(filter %.c,$^) -lm

.PHONY: all clean run

#.PRECIOUS: %.ss %.oo