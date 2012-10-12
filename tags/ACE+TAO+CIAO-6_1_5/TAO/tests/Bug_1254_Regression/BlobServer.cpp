//
// $Id$
//
#include "BlobServer.h"

BlobServerImpl::BlobServerImpl (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

/// Implement the BlobServer interface
void
BlobServerImpl::test(
            BlobType1& blob1,
            BlobType2& blob2
      )
{
    blob1.length(10);
    for(int i = 0; i < 10; i++)
    {
        blob1[i] = 'A' + i;
    }

    blob2.length(10);
    for(int i = 0; i < 10; i++)
    {
        blob2[i] = 'A' + i;
    }
}

void
BlobServerImpl::shutdown (void)
{
  this->orb_->shutdown (0);
}
