// ============================================================================
//
// = LIBRARY
//    CORBA Benchmark/Marshal
// 
// = FILENAME
//    marshal_options.h
//
// = AUTHOR
//    Aniruddha Gokhale
// 
// ============================================================================

#if !defined (_CORBA_MARSHAL_OPTIONS_H_)
#define _CORBA_MARSHAL_OPTIONS_H_

#include "ace/OS.h"
#include "ace/SString.h"

class CORBA_Marshal_Options
{
  // = TITLE
  //    CORBA_Marshal_Options
  // = DESCRIPTION
  //    An encapsulation of all the CORBA_MARSHAL test parameters.  This is
  //    used to communicate between the ACE_CORBA_Marshal_Driver and all
  //    specializations of CORBA_Marshal_Proxy. It must be noted that not all
  //    options will be valid for a given specialization of CORBA_Marshal_Proxy. This is
  //    a "all-encompassing" class.
public:
  // The following data types will be tested
  enum DATATYPE 
  {
    NO_DATATYPE,
    SHORT,
    LONG,
    DOUBLE,
    CHAR,
    OCTET,
    STRUCT,
    UNION,
    ANY,
    SEQUENCE,
    RECURSIVE
  };

  // The following CORBA implementations will be tested
  enum PROXY_TYPE 
  {
    NO_PROXY,
    TAO, 
    ORBIX, 
    VISIBROKER
  };

  // Invocation policy
  enum POLICY 
  {
    NO_POLICY,
    STATIC,  // SII, SSI
    DYNAMIC  // DII, DSI
  };

  // what role do we play?
  enum ROLE 
  {
    NO_ROLE,
    SENDER,
    RECEIVER
  };

  CORBA_Marshal_Options (void);
  // Construction.  Sets default values.

  ~CORBA_Marshal_Options (void);
  // Destructor

  int parse_args (int argc, char *argv[]);
  // Parse command line arguments.  This decides what type of CORBA_MARSHAL
  // test to run.

  // All the command line options

  //= Get/Set the data type to be sent/received
  CORBA_Marshal_Options::DATATYPE data_type (void);
  void data_type (CORBA_Marshal_Options::DATATYPE);

  //= Get/set the proxy type
  CORBA_Marshal_Options::PROXY_TYPE proxy_type (void);
  void proxy_type (CORBA_Marshal_Options::PROXY_TYPE);

  //= Get/Set the policy used by clients/servers for communication
  CORBA_Marshal_Options::POLICY policy (void);
  void policy (CORBA_Marshal_Options::POLICY);

  //= Get/Set whether we are client or server (sender/receiver)
  CORBA_Marshal_Options::ROLE role (void);
  void role (CORBA_Marshal_Options::ROLE);

  //= Get/Set the hostname client connects to or the interface on which the
  //server listens to
  const char *hostname (void);
  void hostname (const char *);

  //= Get/set the port number on which the server listens
  short port (void);
  void port (short);
  
  //= Get/Set the object name (to which to connect to)
  const char *object_name (void);
  void object_name (const char *);

  //= Get/Set the service name (for impl is ready)
  const char *service_name (void);
  void service_name (const char *);

  // = Get/set receive sockbufsize.
  int rcv_socket_queue_size (void);
  void rcv_socket_queue_size (int);

  // = Get/set send sockbufsize.
  int snd_socket_queue_size (void);
  void snd_socket_queue_size (int);

  // = Get/set whether to use TCP_NODELAY or not
  unsigned short disable_nagle (void);
  void disable_nagle (unsigned short);

  // = Get/set whether to print debug messages.
  unsigned short debug (void);
  void debug (unsigned short);

  // =Get/Set filename to store results
  const char *filename (void);
  void filename (const char *fname);

  // =Get/set number of iterations
  int iterations (void);
  void iterations (int iter);

  int Usage (void);
  // prints a usage message and always returns -1

  int orb_init_args (int &argc, char * argv []);
  // return a string that will be passed to the ORB init method based on the
  // other options and proxy type

  int boa_init_args (int &argc, char * argv []);
  // return a string that will be passed to the BOA init method based on the
  // other options and proxy type

private:
  const char *object_name_;
  // name of the object.
  
  const char *service_name_;
  // name of the service.
  
  CORBA_Marshal_Options::DATATYPE data_type_;
  // data type to send/receive

  CORBA_Marshal_Options::PROXY_TYPE proxy_type_;
  // the proxy type, e.g., ACE, Orbix, VisiBroker, etc

  CORBA_Marshal_Options::POLICY policy_;
  // communications policy used by the communicating entity

  CORBA_Marshal_Options::ROLE role_;
  // Are we creating a sender or receiver?

  int rcv_socket_queue_size_;
  // Size of socket queue.

  int snd_socket_queue_size_;
  // Size of socket queue.

  unsigned short disable_nagle_;
  // Nagle must be stopped.

  unsigned short debug_;
  // Should we print debug messages?

  short port_;
  // Port for socket connections to use.

  const  char *hostname_;
  // Host for senders to connect to. and servers to listen on

  const char *filename_;
  // filename in which to store the results

  int iterations_;
  // number of times to run the same test

  ACE_CString orb_init_args_;
  // arguments to orb_init

  ACE_CString boa_init_args_;
  // arguments to boa_init

};

#if defined (__ACE_INLINE__)
#include "benchmark/marshal_options.i"
#endif /* __ACE_INLINE__ */

#endif
