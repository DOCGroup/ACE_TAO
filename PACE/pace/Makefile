# $Id$ -*- Makefile -*-

# ==============================================================================
#
# = LIBRARY
#    pace
#
# = FILENAME
#    pace/Makefile
#
# = AUTHOR
#    Luther Baker
#
# ==============================================================================

#
# don't compile
# pace/sys/types.h
#

# ---------------- pace/*.*
SRC =	                \
 aio               \
 dirent            \
 fcntl             \
 grp               \
 mqueue            \
 netdb             \
 pthread           \
 pwd               \
 sched             \
 semaphore         \
 setjmp            \
 signal            \
 stdio             \
 stdlib            \
 string            \
 strings           \
 time              \
 unistd            \
 utime             \
 wait

# ---------------- pace/sys/*.*
SRC +=                  \
 sys/mman          \
 sys/socket        \
 sys/stat          \
 sys/termios       \
 sys/times         \
 sys/utsname

# ---------------- pace/netinet/*.*
SRC +=                  \
 netinet/in

# ---------------- pace/arpa/*.*
SRC += \
 arpa/inet

OBJS = $(addsuffix .o,$(SRC))

CFLAGS += -I$(PACE_ROOT) -g -w

LDFLAGS = -lsocket -lnsl -lrt

CC = cc

all:	$(OBJS)

clean:
	$(RM) -rf *.o *~ *.*~ .*~                        \
	arpa/*.o arpa/*~ arpa/*.*~ arpa/.*~              \
	netinet/*.o netinet/*~ netinet/*.*~ netinet/.*~  \
	sys/*.o sys/*~ sys/*.*~ sys/.*~                  \
	config/*.o config/*~ config/*.*~ config/.*~

# end of Makefile
