CC		= /bin/cc
CXX		= /usr/bin/CC
CFLAGS		= $(DCFLAGS) -Aa -z
CCFLAGS		= $(CFLAGS) -pta -ptb
DCFLAGS		+= -g
DLD		= $(CXX)
LD		= $(CXX) 
PIC		= +Z
ARFLAGS		= ruv
RANLIB		= echo
LDFLAGS		= -L$(WRAPPER_ROOT)/ace -Wl,+s
LIBS		+= -lxti
SOFLAGS		= -b
SOEXT		= sl
SOBUILD     = @echo ""; \
  echo "$(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<"; \
  $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
  $(RM) -rf $(VSHDIR)pt$*; mkdir $(VSHDIR)pt$*; \
  echo "int main() { return 0; }" >dummy.C; \
  echo "$(CXX) $(CCFLAGS) $(PIC) $(CPPFLAGS) $(LDFLAGS) -ptr$(VSHDIR)pt$* -ptr./ptrepository dummy.C $<";\
  YYZ="`$(CXX) $(CCFLAGS) $(PIC) $(CPPFLAGS) $(LDFLAGS) -ptr$(VSHDIR)pt$* -ptr./ptrepository dummy.C $<`"; \
  $(RM) -rf a.out dummy.*; \
  YYZ="`echo $(VSHDIR)pt$*/*.o`"; \
  echo "$(SOLINK.cc) -o $@ ./$(VSHDIR)$*.o $(YYZ)"; \
  $(SOLINK.cc) -o $@ ./$(VSHDIR)$*.o $(YYZ)
