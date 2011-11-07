//
// $Id$
//

#ifndef ONEWAY_BUFFERING_H
#define ONEWAY_BUFFERING_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Oneway_Buffering interface
class Oneway_Buffering
  : public virtual POA_Test::Oneway_Buffering
{
public:
  /// Constructor
  Oneway_Buffering (CORBA::ORB_ptr orb,
                    Test::Oneway_Buffering_Admin_ptr admin);

  // = The skeleton methods
  virtual void receive_data (const Test::Payload &the_payload);

  virtual void flush (void);

  virtual void sync (void);

  virtual void shutdown (void);

private:
  /// Use an ORB reference to shutdown the application.
  CORBA::ORB_var orb_;

  /// Report request progress to this interface
  Test::Oneway_Buffering_Admin_var admin_;
};

#include /**/ "ace/post.h"
#endif /* ONEWAY_BUFFERING_H */
