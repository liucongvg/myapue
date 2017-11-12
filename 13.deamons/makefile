targets = $(patsubst %.c,%,$(wildcard *.c))

all:$(targets)

debug:
	@echo $(targets)

%:%.c
	gcc -o $@ $^ -pthread 

.PHONY: clean

clean:
	rm -f $(targets)
