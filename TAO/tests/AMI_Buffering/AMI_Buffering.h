
#ifndef AMI_BUFFERING_H
#define AMI_BUFFERING_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::AMI_Buffering interface
class AMI_Buffering
  : public virtual POA_Test::AMI_Buffering
{
public:
  /// Constructor
  AMI_Buffering (CORBA::ORB_ptr orb,
                 Test::AMI_Buffering_Admin_ptr admin);
  AMI_Buffering (AMI_Buffering const &) = delete;
  AMI_Buffering &operator= (AMI_Buffering const &) = delete;
  AMI_Buffering (AMI_Buffering &&) = delete;
  AMI_Buffering &operator= (AMI_Buffering &&) = delete;

  // = The skeleton methods
  void receive_data (Test::Payload const &the_payload) override;

  void flush () override;

  void sync () override;

  void shutdown () override;

  /// internal implementation of shutdown. This
  void try_shutdown ();

  class Nest_Guard
    {
    public:
      explicit Nest_Guard (AMI_Buffering &);
      ~Nest_Guard ();
      Nest_Guard (Nest_Guard const &) = delete;
      Nest_Guard &operator= (Nest_Guard const &) = delete;
      Nest_Guard (Nest_Guard &&) = delete;
      Nest_Guard &operator= (Nest_Guard &&) = delete;
    private:
      AMI_Buffering &target_;
    };

  /// Use an ORB reference to shutdown the application.
  CORBA::ORB_var orb_;

  /// Report request progress to this interface
  Test::AMI_Buffering_Admin_var admin_;

  /// nesting depth count for receive_data processing
  int nest_ {};

  /// maximum nesting depth reached during run
  int max_nest_ {};

  /// flag indicating that a shutdown is required as soon
  /// as the nest count reaches 0.
  bool must_shutdown_ {};
};

#include /**/ "ace/post.h"
#endif /* AMI_BUFFERING_H */
