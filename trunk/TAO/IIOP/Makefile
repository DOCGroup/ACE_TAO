# @(#)Makefile	1.8 97/02/03
# Top level makefile for portable ORB

DIRS =		lib test

default:	$(DIRS)
all:		default

clean:		$(DIRS)
	-rm -rf proto Log core *.zip

install:	$(DIRS)
check:		$(DIRS)

clean	:= TARGET=clean
install	:= TARGET=install
check	:= TARGET=check


$(DIRS):	FRC
	@( cd $@ ; \
	  echo "** Builder's log, stardate" `date` ;\
	  echo "** Starting on " `uname -n` " which is a" `uname -m` ; \
	  echo "** Starting '$(TARGET)' build for $@" ;\
	  echo $(MAKE) $(MFLAGS) $(TARGET); \
	  $(MAKE) $(MFLAGS) $(TARGET); \
	  echo "** Finished '$(TARGET)' build for $@ at " `date` ;\
	  echo "" ;\
	)

#
# zip, unzip are widely available on UNIX and interoperate
# with pkzip, pkunzip, and similar tools under MS-Windows.
# compression is good; this is the new distribution format.
#
# http://quest.jpl.nasa.gov/Info-Zip/Info-Zip.html 
#
zip:
	-rm -f iiop.zip
	zip -r iiop.zip *
unzip:
	unzip iiop.zip

FRC:

