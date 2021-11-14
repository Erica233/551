CPPFLAGS=-std=gnu++98 -pedantic -Wall -Werror -ggdb3
cyoa-step1: cyoa-step1.cpp
	g++ $(CFLAGS) -o cyoa-step1 cyoa-step1.cpp
clean:
	rm -f $(OBJS) $(PROGRAM) *~
	
	
	
	
CPPFLAGS=-std=gnu++98 -pedantic -Wall -Werror -ggdb3
PROGS=cyoa-step1 cyoa-step2 cyoa-step3 cyoa-step4
OBJS=$(patsubst %,%.o,$(PROGS)) page.o
all: $(PROGS)
cyoa-step%: cyoa-step%.o
	g++ -o $@ $^
%.o: %.c page.hpp
	g++ $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *~ $(PROGS) $(OBJS)