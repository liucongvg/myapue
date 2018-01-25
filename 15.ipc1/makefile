targets = $(patsubst %.c,%,$(wildcard *.c))

all:$(targets)

debug:
	@echo $(targets)

%:%.c
	gcc -o $@ $^ -lrt -pthread 

.PHONY: clean

clean:
	rm -f $(targets)
