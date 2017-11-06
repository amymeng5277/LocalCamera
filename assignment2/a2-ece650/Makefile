object = main.o util.o queue.o graph.o base.o
all : $(object)
	cc -o a2-ece650 $(object)
util.o : util.c util.h
	cc -c util.c
graph.o : graph.c graph.h queue.h
	cc -c graph.c
queue.o : queue.c queue.h
	cc -c queue.c
base.o : base.c base.h util.h graph.h
	cc -c base.c
main.o : main.c  util.h  queue.h  graph.h  base.h

clean:
	 rm a2-ece650 $(object)
