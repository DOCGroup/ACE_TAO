/* -*- C++ -*- */
// $Id$

// ===========================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    QoS_Session_Impl.h
//
// = AUTHOR
//    Vishal Kachroo <vishal@cs.wustl.edu>
//
// ===========================================================================

#ifndef ACE_QOS_SESSION_IMPL_H
#define ACE_QOS_SESSION_IMPL_H
#include "ace/pre.h"

#include "ace/QoS_Session.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#if defined (ACE_HAS_RAPI)
#include "rapi_lib.h"

class ACE_Export ACE_RAPI_Session : public ACE_QoS_Session
{
  // = TITLE
  //    A RAPI QoS session object.
  //
  // = DESCRIPTION
  //    This class is a RAPI (RSVP API, an implementation of RSVP on UNIX) 
  //    implementation of the ACE_QoS_Session interface.
  
public:
  
  ~ACE_RAPI_Session (void);
  // Default destructor.

  static int rsvp_error;
  // Error handling for RSVP callback

  virtual int open (ACE_INET_Addr dest_addr,
                    ACE_Protocol_ID protocol_id);
  // Open a RAPI QoS session [dest IP, dest port, Protocol ID].

  virtual int close (void);
  // Close the RAPI QoS Session.
  
  virtual ACE_QoS qos (void) const;
  // Returns the QoS for this RAPI session.
  
  virtual int qos (ACE_SOCK *socket,
                   const ACE_QoS &ace_qos);
  // Set QoS for this RAPI session. The socket parameter is used to confirm if
  // this QoS session was subscribed to by the socket.

  virtual void qos (const ACE_QoS &ace_qos);
  // Sets the QoS for this session object to ace_qos. Does not interfere with the 
  // QoS in the underlying socket. This call is useful to update the QoS object 
  // when the underlying socket QoS is being set through a mechanism other than
  // the previous qos () method e.g. inside the dgram_mcast.subscribe () where the 
  // QoS for the socket is set through ACE_OS::join_leaf ().

  virtual int update_qos (void);
  // Calls rapi_dispatch () that further triggers the call back function.    
  // It is a mechanism of updating the QoS for this session asynchronously, as
  // RSVP events occur.

  virtual int session_id (void) const;
  // Get the RAPI session id.
  
  virtual void session_id (const int session_id);
  // Set the RAPI session id.

  virtual ACE_INET_Addr dest_addr (void) const;
  // Get the destination address for this RAPI session.

  virtual void dest_addr (const ACE_INET_Addr &dest_addr);
  // Set the destination address for this RAPI session.

  virtual int version ();
  // RAPI version. Returned value = 100 * major-version + minor-version.

  friend class ACE_QoS_Session_Factory;
  // The factory is a friend so it can create this object through
  // the only private constructor.
  
private:
  
  ACE_RAPI_Session (void);
  // Default constuctor. Constructor is defined private so that only 
  // the friend factory can instantiate this class.

  rapi_tspec_t *init_tspec_simplified (const ACE_Flow_Spec &flow_spec);
  // Construct a simplified RAPI Sender TSpec object 
  // from an ACE_Flow_Spec object. Used internally by this class.

  rapi_flowspec_t *init_flowspec_simplified(const ACE_Flow_Spec &flow_spec);
  // Construct a simplified RAPI Receiver FlowSpec object 
  // from an ACE_Flow_Spec object. Used internally by the class.

  int sending_qos (const ACE_QoS &ace_qos);
  // Set sending QoS for this RAPI session.
  
  int receiving_qos (const ACE_QoS &ace_qos);
  // Set receiving  QoS for this RAPI session.
  
};

#endif /* ACE_HAS_RAPI */

class ACE_Export ACE_GQoS_Session : public ACE_QoS_Session
{
  // = TITLE
  //    A GQoS session object.
  //
  // = DESCRIPTION
  //    This class is a GQoS (Generic QoS, an implementation of RSVP on 
  //    Win2K) implementation of the ACE_QoS_Session interface.

public:
  
  ~ACE_GQoS_Session (void);
  // Default destructor.

  static int GQoS_session_id;
  // This is a session ID generator. It does a lot more than expected 
  // from an int!.

  virtual int open (ACE_INET_Addr dest_addr,
                    ACE_Protocol_ID protocol_id);
  // Open a GQoS session [dest IP, dest port, Protocol ID].

  virtual int close (void);
  // Close the GQoS Session.
  
  virtual ACE_QoS qos (void) const;
  // Returns the QoS for this GQoS session.

  virtual int qos (ACE_SOCK *socket,
                   const ACE_QoS &ace_qos);
  // Set QoS for this GQoS session. The socket parameter is used to confirm if
  // this QoS session was subscribed to by the socket.
  
  virtual void qos (const ACE_QoS &ace_qos);
  // Sets the QoS for this session object to ace_qos. Does not interfere with the 
  // QoS in the underlying socket. This call is useful to update the QoS object 
  // when the underlying socket QoS is being set through a mechanism other than
  // the previous qos () method e.g. inside the dgram_mcast.subscribe () where the 
  // QoS for the socket is set through ACE_OS::join_leaf ().
  
  virtual int update_qos (void);
  // Calls the ioctl (ACE_SIO_GET_QOS). It is a mechanism of updating the 
  // QoS for this session asynchronously, as RSVP events occur.

  virtual ACE_INET_Addr dest_addr (void) const;
  // Get the destination address for this GQoS session.

  virtual void dest_addr (const ACE_INET_Addr &dest_addr);
  // Set the destination address for this GQoS session.

  virtual int session_id (void) const;
  // Get the GQoS session id.
  
  virtual void session_id (const int session_id);
  // Set the GQoS session id.

  virtual int version ();
  // GQoS version.

  friend class ACE_QoS_Session_Factory;
  // The factory is a friend so it can create this object through
  // the only private constructor.

private:
  
  ACE_GQoS_Session (void);
  // Default constructor. Constructor is defined private so that only 
  // the friend factory can instantiate this class.
  
};

#if defined (__ACE_INLINE__)
#include "ace/QoS_Session_Impl.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_QOS_SESSION_IMPL_H */
