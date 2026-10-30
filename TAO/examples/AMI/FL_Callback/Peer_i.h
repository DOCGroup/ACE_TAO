
//=============================================================================
/**
 *  @file   Peer_i.h
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
  explicit Peer_Handler_i (Peer_i *peer);

  ~Peer_Handler_i () override = default;

  Peer_Handler_i (Peer_Handler_i const &) = delete;
  Peer_Handler_i &operator= (Peer_Handler_i const &) = delete;
  Peer_Handler_i (Peer_Handler_i &&) = delete;
  Peer_Handler_i &operator= (Peer_Handler_i &&) = delete;

  void request (CORBA::Long retval) override;

  void request_excep (::Messaging::ExceptionHolder *excep_holder) override;

  void start ();

  void shutdown ();

private:
  /// The real implementation
  Peer_i *peer_ {};
};

class Peer_i : public POA_Peer
{
public:
  /// Constructor
  Peer_i ();

  /// Destructor
  ~Peer_i () override = default;

  Peer_i (Peer_i const &) = delete;
  Peer_i &operator= (Peer_i const &) = delete;
  Peer_i (Peer_i &&) = delete;
  Peer_i &operator= (Peer_i &&) = delete;

  void init (CORBA::ORB_ptr orb,
             Progress_ptr progress,
             ACE_Time_Value const& delay);

  /// Used by the Reply_Handler to indicate that a reply has been
  /// received.
  void reply (CORBA::Long result);

   // = See test.idl for an explanation of these methods.
  CORBA::Long request (CORBA::Long id) override;

  void start (PeerSet const& the_peers,
              CORBA::Long iterations) override;

  void shutdown () override;

private:
  /// The reply_handler
  Peer_Handler_i reply_handler_;

  /// The orb
  CORBA::ORB_var orb_ {};

  /// The interface to report back progress.
  Progress_var progress_ {};

  /// The delay on each request
  ACE_Time_Value delay_ {};

  /// The id assigned by the progress interface
  CORBA::Long id_ {};
};

/**
 * @class Peer_Task
 *
 * Run a "start" request on a separate thread.
 */
class Peer_Task : public ACE_Task_Base
{
public:
  Peer_Task (PeerSet const& the_peers,
             CORBA::Long iterations,
             Progress_ptr progress,
             AMI_PeerHandler_ptr handler,
             CORBA::Long id);

  /// The thread entry point
  int svc () override;

  Peer_Task (Peer_Task const &) = delete;
  Peer_Task &operator= (Peer_Task const &) = delete;
  Peer_Task (Peer_Task &&) = delete;
  Peer_Task &operator= (Peer_Task &&) = delete;

private:
  /// The peers
  PeerSet the_peers_;

  /// The number of iterations
  CORBA::Long iterations_ {};

  /// To report progress
  Progress_var progress_;

  /// To issue async requests
  AMI_PeerHandler_var handler_;

  /// Our id
  CORBA::Long id_ {};
};

#endif /* PEER_I_H */
