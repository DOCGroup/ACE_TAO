// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/examples/Simple/UDP
//
// = FILENAME
//    UDP_i.h
//
// = DESCRIPTION
//    This class implements the server functionality of the UDP test.
//    We support only single threaded servers, as we have a null mutex
//    for locking our state.
//
// = AUTHOR
//    Michael Kircher <Michael.Kircher@mchp.siemens.de>
//
// ============================================================================

#ifndef UDP_I_H
#define UDP_I_H

#include "UDPS.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Synch.h"

class UDP_i: public POA_UDP
{
  // = TITLE
  //    UDP Object Implementation
  //
  // = DESCRIPTION
  //    This class implements the UDP object, which accepts two methods.
  //    The first, <invoke> accepts a string identifying the client
  //    and a request_id so that the server can verify if the requests
  //    arrived in order.
public:
  // = Initialization and termination methods.
  UDP_i (void);
  // Constructor

  ~UDP_i (void);
  // Destructor

  virtual void invoke (const char *client_name,
                       UDP_ptr udpHandler,
                       CORBA::Long request_id
                       TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Shutdown the server.

  void orb (CORBA::ORB_ptr o);
  // Set the ORB pointer.

private:
  CORBA::ORB_var orb_;
  // ORB pointer.

  ACE_Hash_Map_Manager_Ex < CORBA::String_var,
                            CORBA::Long,
                            ACE_Hash < const char * >,
                            ACE_Equal_To < const char * >,
                            ACE_Null_Mutex > request_id_table_;
};

#endif /* UDP_I_H */
