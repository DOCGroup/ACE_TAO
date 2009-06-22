// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/examples/AMI/FL_Callback
//
// = FILENAME
//   Peer_i.h
//
// = AUTHOR
//   Carlos O'Ryan
//
// ============================================================================

#ifndef PEER_I_H
#define PEER_I_H

#include "testS.h"
#include "ace/Task.h"

class Peer_i;

class Peer_Handler_i : public POA_AMI_PeerHandler
{
public:
  Peer_Handler_i (Peer_i *peer);
  // The peer

  virtual void request (CORBA::Long retval);

  virtual void request_excep (::Messaging::ExceptionHolder * excep_holder);

  virtual void start (void);

  virtual void shutdown (void);

private:
  Peer_i *peer_;
  // The real implementation
};

class Peer_i : public POA_Peer
{
public:
  Peer_i (void);
  // Constructor

  virtual ~Peer_i (void);
  // Destructor

  void init (CORBA::ORB_ptr orb,
             Progress_ptr progress,
             const ACE_Time_Value &delay);

  void reply (CORBA::Long result);
  // Used by the Reply_Handler to indicate that a reply has been
  // received.

   // = See test.idl for an explanation of these methods.
  CORBA::Long request (CORBA::Long id);

  void start (const PeerSet& the_peers,
              CORBA::Long iterations);

  void shutdown (void);

private:
  Peer_Handler_i reply_handler_;
  // The reply_handler

  CORBA::ORB_var orb_;
  // The orb

  Progress_var progress_;
  // The interface to report back progress.

  ACE_Time_Value delay_;
  // The delay on each request

  CORBA::Long id_;
  // The id assigned by the progress interface
};

class Peer_Task : public ACE_Task_Base
{
  // = DESCRIPTION
  //   Run a "start" request on a separate thread.
public:
  Peer_Task (const PeerSet& the_peers,
             CORBA::Long iterations,
             Progress_ptr progress,
             AMI_PeerHandler_ptr handler,
             CORBA::Long id);

  virtual int svc (void);
  // The thread entry point

private:
  PeerSet the_peers_;
  // The peers

  CORBA::Long iterations_;
  // The number of iterations

  Progress_var progress_;
  // To report progress

  AMI_PeerHandler_var handler_;
  // To issue async requests

  CORBA::Long id_;
  // Our id
};

#endif /* PEER_I_H */
