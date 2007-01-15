//
// $Id$
//
#include "BlobServer.h"

ACE_RCSID(BlobServerImpl, BlobServerImpl, "$Id$")

BlobServerImpl::BlobServerImpl (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

/// Implement the BlobServer interface
void 
BlobServerImpl::test(
            BlobType1& blob1,
            BlobType2& blob2
            ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        ::CORBA::SystemException
      ))
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
BlobServerImpl::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
