CC     = g++
INCDIR = include
CFLAGS = -std=c++11

SRCDIR = src
TESTDIR = test

BINDIR = bin
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(BINDIR)/%.o, $(SRCS))

print:
	@echo $(SRCS)
	@echo $(OBJS)
	@echo $(TEST_SRCS)
	@echo $(TEST_OBJS)

shell: clean $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o myShell

all: shell
	./myShell

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	rm -rf myShell $(BINDIR)/*.o