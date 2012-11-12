//
// $Id$
//

#ifndef HELLO_H
#define HELLO_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::TestLargeSequence interface
class BoundSequences
  : public virtual POA_Test::BoundSequences
{
public:
  /// Constructor
  BoundSequences (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual ::CORBA::ULong SendSequenceOf10Long (
    ::CORBA::ULong LengthSent,
    const ::Test::BoundSequences::SequenceOf10Long &inSeq
  );

  virtual void shutdown (void);

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* HELLO_H */
