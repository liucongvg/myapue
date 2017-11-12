targets = $(patsubst %.c,%,$(wildcard *.c))

all:$(targets)

debug:
	@echo $(targets)

%:%.c
	gcc -o $@ $^ 

.PHONY: clean

clean:
	rm -f $(targets)
