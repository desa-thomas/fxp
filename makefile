CC = gcc
ODIR = objs
  
LIBS = -lm
#debugging flags
CFLAGS = -g -Wall -Wextra -pedantic -MMD -MP

main.exe: $(ODIR)/fxlib.o
	$(CC) -LIBS -o $@ $^

$(ODIR)/fxlib.o: fxlib.c
	$(CC) -c $(CFLAGS) -o $@ $<
