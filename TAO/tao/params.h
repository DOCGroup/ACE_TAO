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

// Forward decls.

class TAO_Server_Connection_Handler;

// This is a quick hack to avoid having to unravel the intracacies of
// the all the hairy order interdepencies that currently exist in TAO.
#if ! defined (__ACE_INLINE__)
#define TAO_LOCAL_INLINE
#else
#define TAO_LOCAL_INLINE ACE_INLINE
#endif /* ! __ACE_INLINE__ */

class TAO_Export TAO_ORB_Parameters
  // = TITLE
  //    Parameters that are specific to the ORB.  These parameters can
  //    be for the client, the server, or for both.
  //
  // = NOTE
  // Some of the functions have inline definitions in the class header
  // below.  Please do not move them back to the .i file.
  // cygnus-2.7.2-960126, distributed with Tornado 1.0.1, can't deal
  // with returing a const reference here.  It raises an internal
  // compiler error at ace/Svc_Handler.cpp:46, of all places.
{
public:
  TAO_ORB_Parameters (void);
  // Constructor.
  
  ~TAO_ORB_Parameters (void);
  // Destructor.
  
  void addr (const ACE_INET_Addr &addr);
  // Set the address on which we're listening.

  const ACE_INET_Addr &addr (void);
  // Get the address on which we're listening.

  void name_service_ior (CORBA::String ns);
  // Set the IOR of our name service.

  CORBA::String name_service_ior (void);
  // Get the IOR of our name service.

  void name_service_port (CORBA::UShort port);
  // Set the port of our name service.

  CORBA::UShort name_service_port (void);
  // Get the port of our name service.

  // @@ TODO: This shouldn't be needed, they were added until the
  // Naming Service works correctly.
  CORBA::String schedule_service_ior (void);
  // Get the IOR of the 

  void schedule_service_ior (CORBA::String ns);
  // Set the IOR of our schedule service.

  CORBA::String event_service_ior (void);
  // Get the IOR of the event service

  void event_service_ior (CORBA::String ns);
  // Set the IOR of our event service.

private:
  ACE_INET_Addr addr_;          
  // host + port number we are listening on

  CORBA::String name_service_ior_;
  // The IOR of our configured Naming Service.

  CORBA::UShort name_service_port_;
  // The port number of our configured Naming Service.

  CORBA::String schedule_service_ior_;
  // The IOR of our configured Scheduling Service.

  CORBA::String event_service_ior_;
  // The IOR of our configured Event Service.
};

typedef enum 
{
  TAO_NONE,
  TAO_LINEAR,
  TAO_DYNAMIC_HASH,
  TAO_ACTIVE_DEMUX,
  TAO_USER_DEFINED
} TAO_Demux_Strategy;

class TAO_Export TAO_OA_Parameters
  // = TITLE
  //    Parameters specific to an Object Adapter.  By definition, this
  //    is only on the server side, since a client does not have an
  //    object adapter.
  //
  // = NOTES
  //    This can be subclassed in order to have OA-specific
  //    parameters, e.g., the Realtime Object Adapter might subclass
  //    this and add its own parameters.
{
public:
  TAO_LOCAL_INLINE TAO_OA_Parameters (void);
  // Constructor

  TAO_LOCAL_INLINE ~TAO_OA_Parameters (void);
  // Destructor
  
  void demux_strategy (const char *strategy);
  // Specify the demultiplexing strategy to be used via <{strategy}>.
  // Valid values are one of (case matters) "linear", "dynamic_hash",
  // "user_def", or "active_demux".  If the value is not valid, then
  // <Dynamic Hash> is used as a default.
  
  TAO_LOCAL_INLINE void demux_strategy (TAO_Demux_Strategy s);
  // Specify the demultiplexing strategy to be used.
  
  TAO_LOCAL_INLINE TAO_Demux_Strategy demux_strategy (void);
  // Return the demultiplexing strategy being used.

  TAO_LOCAL_INLINE void userdef_lookup_strategy (TAO_Object_Table *&ot);
  // Provide a way for user defined object key lookup strategies to be
  // plugged in.

  TAO_LOCAL_INLINE TAO_Object_Table *userdef_lookup_strategy (void);
  // return the lookup strategy

  TAO_LOCAL_INLINE void tablesize (CORBA::ULong tablesize);
  // set the table size for lookup table

  TAO_LOCAL_INLINE CORBA::ULong tablesize (void);
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
