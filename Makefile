C=gcc
CXX=g++
RM=rm -f

SRCS=main.cpp Project.cpp InputParse.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: tool

tool: $(OBJS)
	$(CXX) -o city_plan $(OBJS)

clean:
	$(RM) $(OBJS)