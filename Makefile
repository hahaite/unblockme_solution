CFLAGS += -I/home/hahaite/myTest/cppStudy/util

CFLAGS += -W -Wall
#CFLAGS += -Werror
CFLAGS += -Wno-unused-variable
CFLAGS += -Wno-unused-parameter
CFLAGS += -Wno-sign-compare
CFLAGS += -Wno-format

main : main.cpp unblockme.cpp
	g++ -o $@ $^ $(CFLAGS)

clean :
	rm -rf main *.o
