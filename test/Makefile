CFLAGS   = -I.. -Wall -Wextra -Wpedantic -std=c99 -O

.PHONY: test clean

test: test.out test36_128.out bench36_128.out
	./test.out | node -
	./test36_128.out | node -
	time ./bench36_128.out

clean:
	$(RM) *.out

%.out: %.c ../*.h
	$(CC) $(CFLAGS) -o$@ $<
