#include "BlobServer.h"

BlobServerImpl::BlobServerImpl (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

/// Implement the BlobServer interface
void
BlobServerImpl::test (BlobType1 &blob1,
                      BlobType2 &blob2)
{
  blob1.length (10);
  for (CORBA::ULong i {}; i < 10; ++i)
    {
      blob1[i] = static_cast<CORBA::Octet> ('A' + i);
    }

  blob2.length (10);
  for (CORBA::ULong i {}; i < 10; ++i)
    {
      blob2[i] = static_cast<CORBA::Octet> ('A' + i);
    }
}

void
BlobServerImpl::shutdown ()
{
  this->orb_->shutdown (false);
}
