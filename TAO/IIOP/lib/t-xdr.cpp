// XDR record stream ... test encode performance against CDR, using
// normal kinds of inlining performance hacks
//
// XXX as of 3-Nov-95 XDR_stream should only be relied on to marshal the
// simplest primitives ... not objrefs, typecodes, etc.  Also, the
// handling of sequences of chars/octets/shorts/wchars is wrong.
//
// It's suitable only for a simple performance test just now ...
//


#include	<string.h>
#include	<unistd.h>

#include	<sys/types.h>
#include	<sys/time.h>

#include	<orb.h>

#if defined(HAVE_WIDEC_H)
#  include	<widec.h>
#endif

#include	"cdr.h"
#include	"debug.h"

#include	"xdr.h"


// A structure that's somewhat representative of an IIOP message
// in terms of overall complexity, so that its encoding cost is
// "interesting"

typedef CORBA_SEQUENCE <CORBA_Octet> opaque;

struct interesting {

// A call that's mostly going to be "on the mark" for IIOP-ish
// messages with small numbers of parameters:

// 4 chars magic
// 2 bytes version
// 1 byte byte-order
// 1 byte message type		---> 8 bytes "pure header"
    CORBA_Char		hdr_bytes [8];
// 1 word message size		--=> end of GIOP::MessageHeader
    CORBA_ULong		hdr_len;

// Service Context (1 word min)
    CORBA_ULong		empty_ctx;
// 1 word request id
    CORBA_ULong		request_id;
// 1 byte response-expected flag
    CORBA_Boolean	response_expected;
// [ CDR:  3 bytes padding ]
// opaque object key (1 word min; typically less than 16 bytes)
    opaque		object_key;
// string operation name (non-empty ... often less than 10 bytes)
    CORBA_String	opname;
// Principal client (1 word min)
    opaque		client_id;

//				--=> END OF GIOP::RequestHeader

// ... small number of parameters with any significance

// 2 word parameters
    CORBA_ULong		param1;
    CORBA_ULong		param2;
// 1 string parameter
    CORBA_String	param3;

};

// XXX declare CDR typecode for above type ... initialize and use
// one instance in the test below

extern CORBA_TypeCode	TC_opaque;

static void
do_test (
    int				use_XDR,
    CORBA_TypeCode_ptr		tc,
    void			*data
)
{
    unsigned			loopcount = 100 * 1000;
    unsigned			i;
    unsigned			error_count = 0;
    ACE_Time_Value		before, after;
    CORBA_String		opname = "kill_husband";
    opaque			key;
    CORBA_Boolean		status;

    key.buffer = (CORBA_Octet *) "jacqueline";
    key.length = key.maximum = 10;

    before = ACE_OS::gettimeofday ();

    if (use_XDR) {

	// Using XDR APIs and encoding rules ... 
	// encode the structure  repeatedly

	for (i = 0; i < loopcount; i++) {
	    CORBA_Environment	env;
	    XDR_stream		stream (-1);

	    // GIOP header plus most of request header
	    status = status
		    && stream.put_long ('GIOP')		// magic
		    && stream.put_long ('\01\01\01\01')	// version etc
		    && stream.put_long (99)		// msg len
		    && stream.put_long (0)		// no svc ctx
		    && stream.put_long (42)		// request ID
		    && stream.put_boolean (CORBA_B_TRUE)// response?
		    ;

	    if (status)
		status = XDR_stream::encoder (&TC_opaque, &key, 0, &stream, env)
		    == CORBA_TypeCode::TRAVERSE_CONTINUE;

	    if (status)
		status = XDR_stream::encoder (_tc_CORBA_String, &opname,
			0, &stream, env)
		    == CORBA_TypeCode::TRAVERSE_CONTINUE;

	    /*
	    if (status)
		status = XDR_stream::encoder (_tc_CORBA_Principal, &key,
			0, &stream, env)
		    == CORBA_TypeCode::TRAVERSE_CONTINUE;
	    */

	    // Parameters:  two longs, a string
	    status = status
		    && stream.put_long (99)
		    && stream.put_long (-3455);
	    if (status)
		status = XDR_stream::encoder (_tc_CORBA_String, &opname,
			0, &stream, env)
		    == CORBA_TypeCode::TRAVERSE_CONTINUE;

	    // Gratuitous extra "interesting" data
	    status = XDR_stream::encoder (tc, data, 0, &stream, env)
		    == CORBA_TypeCode::TRAVERSE_CONTINUE;

	    
	    if (status != CORBA_B_TRUE)
		error_count++;
	}

    } else {

	// This branch is the same, but using CDR APIs and encoding ...
	// encode the structure  repeatedly

	for (i = 0; i < loopcount; i++) {
	    CORBA_Environment	env;
	    unsigned char	buffer [CDR::DEFAULT_BUFSIZE];
	    CDR			stream (buffer, sizeof buffer);

	    // GIOP header plus most of request header
	    status = status
		    && stream.put_long ('GIOP')		// magic
		    && stream.put_long ('\01\01\01\01')	// version etc
		    && stream.put_long (99)		// msg len
		    && stream.put_long (0)		// no svc ctx
		    && stream.put_long (42)		// request ID
		    && stream.put_boolean (CORBA_B_TRUE)// response?
		    ;

	    if (status)
		status = CDR::encoder (&TC_opaque, &key, 0, &stream, env)
		    == CORBA_TypeCode::TRAVERSE_CONTINUE;

	    if (status)
		status = CDR::encoder (_tc_CORBA_String, &opname,
			0, &stream, env)
		    == CORBA_TypeCode::TRAVERSE_CONTINUE;

	    /*
	    if (status)
		status = CDR::encoder (_tc_CORBA_Principal, &key,
			0, &stream, env)
		    == CORBA_TypeCode::TRAVERSE_CONTINUE;
	    */

	    // Parameters:  two longs, a string
	    status = status
		    && stream.put_long (99)
		    && stream.put_long (-3455);
	    if (status)
		status = CDR::encoder (_tc_CORBA_String, &opname,
			0, &stream, env)
		    == CORBA_TypeCode::TRAVERSE_CONTINUE;

	    // Gratuitous extra "interesting" data
	    status = CDR::encoder (tc, data, 0, &stream, env)
		    == CORBA_TypeCode::TRAVERSE_CONTINUE;


	    if (status != CORBA_B_TRUE)
		error_count++;
	}

    }

    after = ACE_OS::gettimeofday ();

    if (loopcount > 0) {
      if (error_count == 0) {
	ACE_Time_Value diff = after - before;
	unsigned long	usecs = diff.sec() * 1000 * 1000 + diff.usec();

	ACE_OS::printf ("%s average encode time\t= %ld.%.03ldms, \t"
			"%ld calls/second\n",
			use_XDR ? "XDR" : "CDR",
			usecs / 1000, usecs % 1000,
			1000000L / usecs);
      }

      ACE_OS::printf ("%d calls, %d errors\n", loopcount, error_count);
    }
}


int
main (
    int			argc,
    char		**argv
)
{
    int			c;
    int			use_XDR = 1;
    CORBA_TypeCode_ptr	tc = _tc_CORBA_TypeCode;
    void		*data = tc;

    while ((c = ACE_OS::getopt (argc, argv, "cx")) != EOF) {
	switch (c) {
	  case 'c':
	    use_XDR = 0;
	    continue;

	  case 'x':
	    use_XDR = 1;
	    continue;

	  case '?':
	  default:
// usage:
	    ACE_OS::fprintf (stderr, "usage:  %s"
			, " [-cx]"
			, "\n"
			, argv [0]
			);
	}
    }

    do_test (1, tc, data);	// XDR-ish
    do_test (0, tc, data);	// CDR

    return 0;
}

