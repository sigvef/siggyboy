CC = gcc
CFLAGS = -g -O2 -std=gnu99
OBJECTS = CPU.o
TESTS = tests/CPU.test.o
ODIR = obj
TESTDIR = tests


emulator : $(OBJECTS)
	ar rcs lib/libgbc.a $(patsubst %, $(ODIR)/%,$(OBJECTS))

test : emulator $(TESTS)
	python tests/testrunner.py

tests/%.o : tests/%.c
	$(CC) $(CFLAGS) $< -Llib -o $(patsubst %.c, bin/%, $<) -lgbc -lrt

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $(patsubst %.c, $(ODIR)/%.o, $<)

clean:
	rm -f lib/*.a
	rm -f $(ODIR)/*.o
	rm -f $(ODIR)/tests/*.o
	rm -rf bin/*
	mkdir bin/tests
