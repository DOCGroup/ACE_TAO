                  MPEG Video Software Decoder
                  (Version 2.0; Jan 27, 1993)

        Lawrence A. Rowe, Ketan Patel, and Brian Smith
 Computer Science Division-EECS, Univ. of Calif. at Berkeley

This directory contains a public domain MPEG video software
decoder. The decoder is implemented as a library that will
take a video stream and display it in an X window on an 8, 24
or 32 bit deep display.  The main routine is supplied to
demonstrate the use of the decoder library. Several dithering
algorithms are supplied based on the Floyd-Steinberg, ordered
dither, and half-toning algorithms that tradeoff quality and
performance. Neither the library nor the main routine handle
real-time synchronization or audio streams.

The decoder implements the standard described in the Committee 
Draft ISO/IEC CD 11172 dated December 6, 1991 which is
sometimes refered to as "Paris Format." The code has been
compiled and tested on the following platforms:

 HP PA-RISC (HP/UX 8.X, X11R4) (i.e., HP 9000/7XX and 9000/3XX)
 Sun Sparc (SunOS 4.X, X11R5)
 DECstation 5000 and Alpha
 IBM RS6000
 Silicon Graphics Indigo
 MIPS RISC/os 4.51
 Sequent Symmetry
 Sony NEWS
 and more than we can list here.

If you decide to port the code to a new architecture, please let
us know so that we can incorporate the changes into our sources.

This directory contains everything required to build and
display video. We have included source code, a makefile, an Imakefile,
installation instructions, and a man page. Data files can
be obtained from the same ftp site this was located in.
See the INSTALL file for instructions on how to
compile and run the decoder. 

The data files were produced by XING. XING data does not take
advantage of P or B frames (ie, frames with motion compensation). 
Performance of the player on XING data is significantly slower 
(half or less) than the performance when motion compensated MPEG 
data is decoded. We are very interested in running the software 
on other MPEG streams.  Please contact us if you have a stream 
that does not decode correctly. Also, please send us new streams
produced by others that do utilize P and B frames.

NOTE: One particular XING data file: raiders.mpg, is not a 
valid MPEG stream since it does not contain a sequence
header. 

We have established several mailing lists for messages about
the decoder:

mpeg-list-dist@CS.Berkeley.EDU 
   General information on the decoder for everyone interested 
   should be sent to this list.  This should become active after
   11/20/92

mpeg-list-request@CS.Berkeley.EDU
   Requests to join or leave the list should be sent to this 
   address. The subject line should contain the single word 
   ADD or DELETE.

mpeg-bugs@CS.Berkeley.EDU
   Problems, questions, or patches should be sent to this address.

Our future plans include porting the decoder to run on other
platforms, integrating it into a video playback system that
supports real-time synchronization and audio streams, and
further experiments to improve the performance of the
decoder. Vendors or other organizations interested in supporting 
this research or discussing other aspects of this project should 
contact Larry Rowe at Rowe@CS.Berkeley.EDU.

We also plan on producing an MPEG encoder. The encoder will NOT be
a real time digitizer, but will be intended for offline processing
of video data. 

ACKNOWLEDGEMENTS:
	We gratefully thank Hewlett-Packard, Fujitsu, the Semiconductor
	Research Corporation for financial support.

	We also want to thank the following people for their help:

	Tom Lane of the Independent JPEG Group provided us with
		the basic inverse DCT code used by our player.
		(tom_lane@g.gp.cs.cmu.edu)

	Reid Judd of Sun Microsystems provided advice and assistance.

	Todd Brunhoff of NVR provided advise and assistance.

	Toshihiko Kawai of Sony provided advise and assistance.








