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
      );
  virtual void shutdown (void);

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* BLOBSERVER_H */
