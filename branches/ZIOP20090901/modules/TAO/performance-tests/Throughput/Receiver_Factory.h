//
// $Id$
//

#ifndef THROUGHPUT_RECEIVER_FACTORY_H
#define THROUGHPUT_RECEIVER_FACTORY_H
#include /**/ "ace/pre.h"

#include "TestS.h"

#if defined (_MSC_VER)
# pragma warning(push)
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::Receiver_Factory interface
class Receiver_Factory
  : public virtual POA_Test::Receiver_Factory
{
public:
  /// Constructor
  Receiver_Factory (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual Test::Receiver_ptr create_receiver (void);

  virtual void shutdown (void);

private:
  /// Keep a reference to the ORB in order to shutdown the app
  CORBA::ORB_var orb_;
};

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* THROUGHPUT_RECEIVER_FACTORY_H */
