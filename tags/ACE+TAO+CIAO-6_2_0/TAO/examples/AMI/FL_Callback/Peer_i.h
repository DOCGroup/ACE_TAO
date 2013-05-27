
//=============================================================================
/**
 *  @file   Peer_i.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan
 */
//=============================================================================


#ifndef PEER_I_H
#define PEER_I_H

#include "testS.h"
#include "ace/Task.h"

class Peer_i;

class Peer_Handler_i : public POA_AMI_PeerHandler
{
public:
  /// The peer
  Peer_Handler_i (Peer_i *peer);

  virtual void request (CORBA::Long retval);

  virtual void request_excep (::Messaging::ExceptionHolder * excep_holder);

  virtual void start (void);

  virtual void shutdown (void);

private:
  /// The real implementation
  Peer_i *peer_;
};

class Peer_i : public POA_Peer
{
public:
  /// Constructor
  Peer_i (void);

  /// Destructor
  virtual ~Peer_i (void);

  void init (CORBA::ORB_ptr orb,
             Progress_ptr progress,
             const ACE_Time_Value &delay);

  /// Used by the Reply_Handler to indicate that a reply has been
  /// received.
  void reply (CORBA::Long result);

   // = See test.idl for an explanation of these methods.
  CORBA::Long request (CORBA::Long id);

  void start (const PeerSet& the_peers,
              CORBA::Long iterations);

  void shutdown (void);

private:
  /// The reply_handler
  Peer_Handler_i reply_handler_;

  /// The orb
  CORBA::ORB_var orb_;

  /// The interface to report back progress.
  Progress_var progress_;

  /// The delay on each request
  ACE_Time_Value delay_;

  /// The id assigned by the progress interface
  CORBA::Long id_;
};

/**
 * @class Peer_Task
 *
 * Run a "start" request on a separate thread.
 */
class Peer_Task : public ACE_Task_Base
{
public:
  Peer_Task (const PeerSet& the_peers,
             CORBA::Long iterations,
             Progress_ptr progress,
             AMI_PeerHandler_ptr handler,
             CORBA::Long id);

  /// The thread entry point
  virtual int svc (void);

private:
  /// The peers
  PeerSet the_peers_;

  /// The number of iterations
  CORBA::Long iterations_;

  /// To report progress
  Progress_var progress_;

  /// To issue async requests
  AMI_PeerHandler_var handler_;

  /// Our id
  CORBA::Long id_;
};

#endif /* PEER_I_H */
