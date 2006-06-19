//
// $Id$
//

#ifndef BLOBSERVER_H
#define BLOBSERVER_H
#include /**/ "ace/pre.h"

#include "testS.h"

class BlobServerImpl
  : public virtual POA_BlobServer
{
public:
  /// Constructor
  BlobServerImpl (CORBA::ORB_ptr orb);

  // = The skeleton methods
    virtual void test(
        BlobType1& blob1,
        BlobType2& blob2
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        ::CORBA::SystemException
      ));
  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* BLOBSERVER_H */
