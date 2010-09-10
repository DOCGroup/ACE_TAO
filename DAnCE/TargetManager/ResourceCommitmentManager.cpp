// $Id$
//
#include "ResourceCommitmentManager.h"
#include "DomainDataManager.h"
#include "Logger/Log_Macros.h"

DAnCE::ResourceCommitmentManager_i::ResourceCommitmentManager_i (void)
{
  DANCE_TRACE ("DAnCE::ResourceCommitmentManager_i");
}

DAnCE::ResourceCommitmentManager_i::~ResourceCommitmentManager_i (void)
{
  DANCE_TRACE ("DAnCE::~ResourceCommitmentManager_i");
}

void
DAnCE::ResourceCommitmentManager_i::commitResources (
    const ::Deployment::ResourceAllocations& resources)
{
  DANCE_TRACE ("DAnCE::ResourceCommitmentManager_i::commitResources");

  DOMAIN_DATA_MANAGER->commitResourceAllocation (resources);

  // Commit succesful .. add to commited resource, we get an exception
  // if the method above fails
  this->add_to_committed_resource (resources);
}

void
DAnCE::ResourceCommitmentManager_i::releaseResources (
    const ::Deployment::ResourceAllocations & resources)
{
  DANCE_TRACE ("DAnCE::ResourceCommitmentManager_i::releaseResources");

  // If the resources set is null, use the already allocated resources ..
  if (resources.length () == 0)
    {
      DOMAIN_DATA_MANAGER->releaseResourceAllocation (this->resources_);
    }
  else
    {
      DOMAIN_DATA_MANAGER->releaseResourceAllocation (resources);
    }
}

void
DAnCE::ResourceCommitmentManager_i::add_to_committed_resource (
    ::Deployment::ResourceAllocations res)
{
  DANCE_TRACE ("DAnCE::ResourceCommitmentManager_i::add_to_commited_resource");

  CORBA::ULong const current_length = this->resources_.length ();

  this->resources_.length (current_length + res.length ());

  for (CORBA::ULong i = 0;i < res.length ();i++)
    {
      this->resources_[current_length + i] = res[i];
    }
}
