/* -*- C++ -*- */

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    params.h
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

// This is a quick hack to avoid having to unravel the intracacies of
// the all the hairy order interdepencies that currently exist in TAO.
#if ! defined (__ACE_INLINE__)
#define LOCAL_INLINE
#else
#define LOCAL_INLINE ACE_INLINE
#endif /* ! __ACE_INLINE__ */

class ACE_Svc_Export TAO_ORB_Parameters
// = TITLE
//    Parameters that are specific to the ORB.  These parameters can be
//    for the client, the server, or for both.
{
public:
  LOCAL_INLINE TAO_ORB_Parameters (void);
  // Constructor
  
  LOCAL_INLINE ~TAO_ORB_Parameters (void);
  // Destructor
  
  LOCAL_INLINE void addr (ACE_INET_Addr &addr);
  // Set the address on which we're listening.

  LOCAL_INLINE ACE_INET_Addr addr (void);
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
  LOCAL_INLINE TAO_OA_Parameters (void);
  // Constructor

  LOCAL_INLINE ~TAO_OA_Parameters (void);
  // Destructor
  
  void demux_strategy (const char *strategy);
  // Specify the demultiplexing strategy to be used via <{strategy}>.
  // Valid values are one of (case matters) "linear", "dynamic_hash",
  // "user_def", or "active_demux".  If the value is not valid, then
  // <Dynamic Hash> is used as a default.
  
  LOCAL_INLINE void demux_strategy (TAO_Demux_Strategy s);
  // Specify the demultiplexing strategy to be used.
  
  LOCAL_INLINE TAO_Demux_Strategy demux_strategy (void);
  // Return the demultiplexing strategy being used.

  LOCAL_INLINE void userdef_lookup_strategy (TAO_Object_Table *&ot);
  // provide a way for user defined object key lookup strategies to be plugged
  // in 

  LOCAL_INLINE TAO_Object_Table *userdef_lookup_strategy (void);
  // return the lookup strategy

  LOCAL_INLINE void tablesize (CORBA::ULong tablesize);
  // set the table size for lookup table

  LOCAL_INLINE CORBA::ULong tablesize (void);
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
