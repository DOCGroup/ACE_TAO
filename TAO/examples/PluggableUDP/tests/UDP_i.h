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
//
// = AUTHOR
//    Michael Kircher <Michael.Kircher@mchp.siemens.de>
//
// ============================================================================

#ifndef UDP_I_H
#define UDP_I_H

#include "UDPS.h"

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
                       CORBA::Long request_id,
                       CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (CORBA::Environment &env)
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
                            ACE_Thread_Mutex > request_id_table_;
};

#endif /* UDP_I_H */


