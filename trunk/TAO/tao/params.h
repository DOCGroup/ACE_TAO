/* -*- C++ -*- */

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    params.hh
//
// = AUTHOR
//    Chris Cleeland
// 
// ============================================================================

#if !defined (TAO_PARAMS_H)
#  define TAO_PARAMS_H

#if 0
#  include "ace/OS.h"

#  include "tao/orb.h" // get types
#  include "tao/boa.h" // Need the DSI Handler
#  include "tao/sequence.h" // for CORBA::OctetSeq
#endif /* 0 */

// Forward decls.

class ROA;
typedef ROA* ROA_ptr;
class TAO_OA_Connection_Handler;

class ACE_Svc_Export TAO_ORB_Parameters
// = TITLE
//    Parameters that are specific to the ORB.  These parameters can be
//    for the client, the server, or for both.
{
public:
  void addr (ACE_INET_Addr &addr);
  // Set the address on which we're listening.

  ACE_INET_Addr addr (void);
  // Get the address on which we're listening.

private:
  ACE_INET_Addr addr_;          
  // host + port number we are listening on
};

typedef enum 
  {
    TAO_NONE,
    TAO_LINEAR,
    TAO_DYNAMIC_HASH,
    TAO_ACTIVE_DEMUX,
    TAO_USER_DEFINED
  } TAO_Demux_Strategy;

class ACE_Svc_Export TAO_OA_Parameters
// = TITLE
//    Parameters specific to an Object Adapter.  By definition, this
//    is only on the server side, since a client does not have an object
//    adapter.
//
// = NOTES
//    This can be subclassed in order to have OA-specific parameters, e.g.,
//    the Realtime Object Adapter might subclass this and add its own
//    parameters.
{
public:
  // This should move to TAO_ORB_Parameters
  void demux_strategy (const char *strategy);
  // Specify the demultiplexing strategy to be used via <{strategy}>.
  // Valid values are one of (case matters) "linear", "dynamic_hash",
  // "user_def", or "active_demux".  If the value is not valid, then
  // <Dynamic Hash> is used as a default.
  
  void demux_strategy (TAO_Demux_Strategy s);
  // Specify the demultiplexing strategy to be used.
  
  TAO_Demux_Strategy demux_strategy (void);
  // Return the demultiplexing strategy being used.

  void userdef_lookup_strategy (TAO_Object_Table *&ot);
  // provide a way for user defined object key lookup strategies to be plugged
  // in 

  TAO_Object_Table *userdef_lookup_strategy (void);
  // return the lookup strategy

  void tablesize (CORBA::ULong tablesize);
  // set the table size for lookup table

  CORBA::ULong tablesize (void);
  // get the table size for the lookup table

private:
  TAO_Demux_Strategy demux_;  
  // demux strategy

  CORBA::ULong tablesize_;       
  // size of object lookup table

  TAO_Object_Table *ot_;
  // concrete lookup table instance
};

#endif /* TAO_PARAMS_H */
