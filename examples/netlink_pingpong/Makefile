MAKE=make
CXX=g++

CFG_INC += -I/usr/local/include/stlport
#CFG_LIB += -L/usr/local/lib  -lstlport_gcc
CFG_LIB += -L/usr/local/lib

OUTDIR=Debug
OUTFILE=$(OUTDIR)/netlink_pingpong
CFG_INC+=-I$(ACE_ROOT)
	
#CFG_LIB+=-L$(ACE_ROOT)/ace/ -lstlport_gcc -lACE
CFG_LIB+=-L$(ACE_ROOT)/ace/  -lACE
CFG_OBJ=
COMMON_OBJ=$(OUTDIR)/netlink_pingpong.o \
	   $(OUTDIR)/netlinkgateway.o 

OBJ=$(COMMON_OBJ) $(CFG_OBJ)
ALL_OBJ=$(OUTDIR)/*.o

COMPILE=$(CXX) -c  -g $(CFG_INC) -O2 -pthread \
			-DACE_NDEBUG \
			-DACE_USE_RCSID=0 \
			-DACE_HAS_EXCEPTIONS \
			-D__ACE_INLINE__  \
			$(CFG_INC) \
		 	 -o "$(OUTDIR)/$(*F).o"  "$<"

LINK=$(CXX) -g  $(OUTDIR)/*.o $(CFG_LIB)  -o "$(OUTFILE)"

# Pattern rules
$(OUTDIR)/%.o : %.cpp
	$(COMPILE)

# Build rules
all: $(OUTFILE)

$(OUTFILE): $(OUTDIR)  $(OBJ)
	$(LINK)

$(OUTDIR):
	mkdir -p "$(OUTDIR)"

# Rebuild this project
rebuild: cleanall all

# Clean this project
clean:
	rm -f $(OUTFILE)
	rm -f $(OBJ)

