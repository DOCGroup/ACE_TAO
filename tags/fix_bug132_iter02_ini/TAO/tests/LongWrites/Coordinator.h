//
// $Id$
//

#ifndef LONGWRITES_COORDINATOR_H
#define LONGWRITES_COORDINATOR_H
#include "ace/pre.h"

#include "TestS.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::Coordinator interface
class Coordinator
  : public virtual POA_Test::Coordinator
  , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  Coordinator (void);

  /// Destructor
  virtual ~Coordinator (void);

  /// Return 1 after <shutdown> is invoked
  int shutdown_called (void);

  // = The skeleton methods
  virtual void add_pair (Test::Receiver_ptr receiver,
                         Test::Sender_ptr sender,
                         CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void start (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void shutdown (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  struct Pair {
    Test::Receiver_var receiver;
    Test::Sender_var sender;
  };

private:
  ACE_SYNCH_MUTEX mutex_;

  int shutdown_called_;

  size_t pairs_count_;
  size_t pairs_length_;
  Pair* pairs_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* LONGWRITES_COORDINATOR_H */
