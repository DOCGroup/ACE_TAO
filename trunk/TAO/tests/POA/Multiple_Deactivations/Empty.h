//
// $Id$
//

#ifndef MULTIPLE_DEACTIVATIONS_EMPTY_H
#define MULTIPLE_DEACTIVATIONS_EMPTY_H
#include "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Empty interface
class Empty
  : public virtual POA_Test::Empty
{
public:
  /// Constructor
  Empty (void);

  /// Destructor
  virtual ~Empty (void);

  // = The skeleton methods

  virtual void _add_ref (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC (());
  virtual void _remove_ref (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC (());
  virtual void destroy (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Keep the reference count
  CORBA::ULong refcnt_;

  static CORBA::ULong in_request_count_;
};

#include "ace/post.h"
#endif /* MULTIPLE_DEACTIVATIONS_EMPTY_H */
