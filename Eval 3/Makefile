#reference: AoP Appendix D.1
CPPFLAGS=-ggdb3 -Wall -Werror -pedantic -std=gnu++98
all: cyoa-step1 cyoa-step2 cyoa-step3 cyoa-step4
cyoa-step%: cyoa-step%.o story.o page.o
	g++ -o $@ $^
%.o: %.cpp
	g++ $(CPPFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o *~ cyoa-step1 cyoa-step2 cyoa-step3 cyoa-step4
