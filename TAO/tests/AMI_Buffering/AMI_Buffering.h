//
// $Id$
//

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

  // = The skeleton methods
  virtual void receive_data (const Test::Payload &the_payload);

  virtual void flush (void);

  virtual void sync (void);

  virtual void shutdown (void);

  /// internal implementation of shutdown. This
  void try_shutdown (void);

  class Nest_Guard
    {
    public:
      Nest_Guard (AMI_Buffering &);
      ~Nest_Guard (void);
    private:
      AMI_Buffering &target_;
    };

  /// Use an ORB reference to shutdown the application.
  CORBA::ORB_var orb_;

  /// Report request progress to this interface
  Test::AMI_Buffering_Admin_var admin_;

  /// nesting depth count for receive_data processing
  int nest_;

  /// maximum nesting depth reached during run
  int max_nest_;

  /// flag indicating that a shutdown is required as soon
  /// as the nest count reaches 0.
  bool must_shutdown_;
};

#include /**/ "ace/post.h"
#endif /* AMI_BUFFERING_H */
