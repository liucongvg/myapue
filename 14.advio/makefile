targets = $(patsubst %.c,%,$(wildcard *.c))

all:$(targets)

debug:
	@echo $(targets)

%:%.c
	gcc -o $@ $^ -lrt 

.PHONY: clean

clean:
	rm -f $(targets)
