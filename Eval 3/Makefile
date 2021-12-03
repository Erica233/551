CPPFLAGS=-ggdb3 -Wall -Werror -pedantic -std=gnu++98
all: cyoa-step1 cyoa-step2 cyoa-step3 cyoa-step4
cyoa-step1: cyoa-step1.o page.o
	g++ -o cyoa-step1 cyoa-step1.o page.o
cyoa-step1.o: cyoa-step1.cpp
	g++ $(CFLAGS) -c cyoa-step1.cpp
cyoa-step2: cyoa-step2.o story.o page.o
	g++ -o cyoa-step2 cyoa-step2.o story.o page.o
cyoa-step2.o: cyoa-step2.cpp
	g++ $(CFLAGS) -c cyoa-step2.cpp
cyoa-step3: cyoa-step3.o story.o page.o
	g++ -o cyoa-step3 cyoa-step3.o story.o page.o
cyoa-step3.o: cyoa-step3.cpp
	g++ $(CFLAGS) -c cyoa-step3.cpp
cyoa-step4: cyoa-step4.o story.o page.o
	g++ -o cyoa-step4 cyoa-step4.o story.o page.o
cyoa-step4.o: cyoa-step4.cpp
	g++ $(CFLAGS) -c cyoa-step4.cpp
story.o: story.cpp
	g++ $(CFLAGS) -c story.cpp
page.o: page.cpp
	g++ $(CFLAGS) -c page.cpp



CPPFLAGS=-std=gnu++98 -pedantic -Wall -Werror -ggdb3
PROGS=cyoa-step1 cyoa-step2 cyoa-step3 cyoa-step4
OBJS=$(patsubst %,%.o,$(PROGS)) page.o story.o
all: $(PROGS)
cyoa-step%: cyoa-step%.o
	g++ -o $@ $^
%.o: %.c page.hpp story.hpp
	g++ $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *~ $(PROGS) $(OBJS)
