// $Id$

// ============================================================================
//
// = LIBRARY
//    CORBA Benchmark
// 
// = FILENAME
//    marshal_options.cpp
//
// = AUTHOR
//    Aniruddha Gokhale
// 
// ============================================================================

#include "benchmark/marshal_options.h"
#include "ace/Get_Opt.h"

#if !defined (__ACE_INLINE__)
#include "benchmark/marshal_options.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(benchmark, marshal_options, "$Id$")

static char *usage_string = 
"Marshal_Test available options: \n\
-d                      : set debug ON \n\
-D                      : set TCP NO DELAY on \n\
-n <service name>       : set the service name\n\
-s <snd sock bufsize>   : set sender sock buf \n\
-r <rcv sock bufsize>   : set recv sock buf \n\
-h <hostname>           : hostname to listen/connect \n\
-p <portnum>            : port num to listen \n\
-i <iters>              : number of iters \n\
-R <ROLE>               : s(ender)/r(eceiver) \n\
-T <datatype>           : s(hort)/l(ong)/d(ouble)/o(ctet)/c(har)/\n\
                          S(truct)/U(nion)/A(ny)/(se)Q/R(ecursive)\n\
-P <policy>             : S(II/SI)/D(II/SI) depending on role \n\
-I <CORBA impl>         : O(rbix)/V(isiBroker)/T(AO)\n\
-o <obj name>           : obj to bind to \n\
-f <filename>           : filename to store results \n\
";

int
CORBA_Marshal_Options::Usage (void)
{
  ACE_DEBUG ((LM_DEBUG, "%s\n", usage_string));
  return -1;
}

int
CORBA_Marshal_Options::parse_args (int argc, char *argv [])
{
  int c; // option letter used
  ACE_Get_Opt get_opt (argc, argv, "dDn:s:r:h:p:i:R:T:P:I:o:f:");

  //  ACE_DEBUG ((LM_DEBUG, "CORBA_Marshal_Options::parse_args\n"));

  while ((c = get_opt ()) != -1)
    {
      switch (c)
	{
	case 'd': // debug
	  this->debug (1);
	  break;
	case 'D': // TCP NODELAY
	  this->disable_nagle (1);
	  break;
	case 'n':
	  this->service_name (get_opt.optarg);
	  break;
	case 's': // sender socket size
	  this->snd_socket_queue_size (ACE_OS::atoi (get_opt.optarg));
	  break;
	case 'r': // receive socket size
	  this->rcv_socket_queue_size (ACE_OS::atoi (get_opt.optarg));
	  break;
	case 'h': // hostname
	  this->hostname (ACE_OS::strdup (get_opt.optarg));
	  break;
	case 'p': // port number
	  this->port (ACE_OS::atoi (get_opt.optarg));
	  break;
	case 'i': // port number
	  this->iterations (ACE_OS::atoi (get_opt.optarg));
	  break;
	case 'R': // role
	  switch (*get_opt.optarg)
	    {
	    case 'S': // sender
	    case 's': // sender
	      this->role (SENDER);
	      break;
	    case 'R': // receiver
	    case 'r': // receiver
	      this->role (RECEIVER);
	      break;
	    default:
	      return this->Usage ();
	    }
	  break;
	case 'T': // data type to test
	  switch (*get_opt.optarg)
	    {
	    case 's': // short
	      this->data_type (SHORT);
	      break;
	    case 'd': // double
	      this->data_type (DOUBLE);
	      break;
	    case 'o': // octet
	      this->data_type (OCTET);
	      break;
	    case 'c': // char
	      this->data_type (CHAR);
	      break;
	    case 'S': // Struct
	      this->data_type (STRUCT);
	      break;
	    case 'U': // Union
	      this->data_type (UNION);
	      break;
	    case 'A': // Any
	      this->data_type (ANY);
	      break;
	    case 'Q': // Sequence
	      this->data_type (SEQUENCE);
	      break;
	    case 'R': // Recursive
	      this->data_type (RECURSIVE);
	      break;
	    case 'l': // long, also the default
	    default:
	      this->data_type (LONG);
	      break;
	    }
	  break;
	case 'P': // policy
	  switch (*get_opt.optarg)
	    {
	    case 'S':
	    case 's':
	      this->policy (STATIC);
	      break;
	    case 'D':
	    case 'd':
	      this->policy (DYNAMIC);
	      break;
	    default:
	      return this->Usage ();
	    }
	  break;
	case 'I': // impl
	  switch (*get_opt.optarg)
	    {
	    case 'O': // Orbix
	      this->proxy_type (ORBIX);
	      break;
	    case 'V': // VisiBroker
	      this->proxy_type (VISIBROKER);
	      break;
	    case 'T': // TAO
	      this->proxy_type (TAO);
	      break;
	    }
	  break;
	case 'o': // obj name
	  this->object_name (ACE_OS::strdup (get_opt.optarg));
	  break;
	case 'f': // file name to save results
	  this->filename (ACE_OS::strdup (get_opt.optarg));
	  break;
	default:  // error
	  return this->Usage ();
	}
    }
  return 0;
}

int
CORBA_Marshal_Options::orb_init_args (int &argc, char *argv [])
{
  char buffer [200];

  argc = 0;
  switch (this->proxy_type_)
    {
    case ORBIX:
      break;
    case VISIBROKER:
      {
	ACE_OS::memset (buffer, '\0', 200);
	if (snd_socket_queue_size_ > 0)
	  {
	    ACE_OS::sprintf (buffer, "-ORBsendbufsize %d", snd_socket_queue_size_);
	    orb_init_args_ += ACE_CString (buffer);
	    argc ++;
	  }

	ACE_OS::memset (buffer, '\0', 200);
	if (rcv_socket_queue_size_ > 0)
	  {
	    ACE_OS::sprintf (buffer, "-ORBrcvbufsize %d", rcv_socket_queue_size_);
	    orb_init_args_ += ACE_CString (buffer);
	    argc ++;
	  }
      }
      break;
    case TAO:
      break;
    }
  ACE_OS::memset (buffer, '\0', 200);
  ACE_OS::memcpy (buffer, boa_init_args_.rep (), boa_init_args_.length ());
  return 0;
}

static char buffer [20][200];

int
CORBA_Marshal_Options::boa_init_args (int &argc, char *argv [])
{
  argc = 0;
  switch (this->proxy_type_)
    {
    case ORBIX:
      break;
    case VISIBROKER:
      {
	ACE_OS::memset (buffer[argc], '\0', 200);
	if (snd_socket_queue_size_ > 0)
	  {
	    ACE_OS::sprintf (buffer[argc], "-OAsendbufsize %d", snd_socket_queue_size_);
	    //	    boa_init_args_ += ACE_CString (buffer[argc]);
	    argc ++;
	  }
	ACE_OS::memset (buffer[argc], '\0', 200);
	if (rcv_socket_queue_size_ > 0)
	  {
	    ACE_OS::sprintf (buffer[argc], "-OArcvbufsize %d", rcv_socket_queue_size_);
	    //	    boa_init_args_ += ACE_CString (buffer[argc]);
	    argc ++;
	  }
	ACE_OS::memset (buffer[argc], '\0', 200);
	if (port_ > 0)
	  {
	    ACE_OS::sprintf (buffer[argc], "-OAport %d", port_);
	    //	    boa_init_args_ += ACE_CString (buffer[argc]);
	    argc ++;
	  }
	ACE_OS::memset (buffer[argc], '\0', 200);
	if (hostname_ != 0 && role_ == RECEIVER)
	  {
	    ACE_OS::sprintf (buffer[argc], "-OAipaddr %s", hostname_);
	    //	    boa_init_args_ += ACE_CString (buffer[argc]);
	    argc ++;
	  }
      }
      break;
    case TAO:
      {
	ACE_OS::memset (buffer[argc], '\0', 200);
	if (port_ > 0)
	  {
	    ACE_OS::sprintf (buffer[argc], "-OAport %d", port_);
	    //	    boa_init_args_ += ACE_CString (buffer);
	    argc ++;
	  }
	ACE_OS::memset (buffer[argc], '\0', 200);
	if (hostname_ != 0 && role_ == RECEIVER)
	  {
	    ACE_OS::sprintf (buffer[argc], "-OAhostname %s", hostname_);
	    //	    boa_init_args_ += ACE_CString (buffer);
	    argc ++;
	  }
      }
      break;
    }
  //  ACE_OS::memset (buffer, '\0', 200);
  //  ACE_OS::memcpy (buffer, boa_init_args_.rep (), boa_init_args_.length ());
  return 0;
}

