// $Id$
//
#include "DomainDataManager.h"
#include "ResourceCommitmentManager.h"
#include "DAnCE/Logger/Log_Macros.h"

// Implementation skeleton constructor
DAnCE::ResourceCommitmentManager_i::ResourceCommitmentManager_i (void)
{
  DANCE_TRACE ("DAnCE::ResourceCommitmentManager_i");
}

// Implementation skeleton destructor
DAnCE::ResourceCommitmentManager_i::~ResourceCommitmentManager_i (void)
{
  DANCE_TRACE ("DAnCE::~ResourceCommitmentManager_i");
}

void DAnCE::ResourceCommitmentManager_i::commitResources (
    const ::Deployment::ResourceAllocations& resources)
{
  DANCE_TRACE ("DAnCE::ResourceCommitmentManager_i::commitResources");
  
  DOMAIN_DATA_MANAGER->commitResourceAllocation (resources);

  // commit succesful .. add to commited resource
  this->add_to_commited_resource (resources);
  return;
}

void DAnCE::ResourceCommitmentManager_i::releaseResources (
    const ::Deployment::ResourceAllocations & resources)
{
  DANCE_TRACE ("DAnCE::ResourceCommitmentManager_i::releaseResources");
  
  ::Deployment::ResourceAllocations res;
  
  // if the resources set is null , use the already allocated resources ..
  if (resources.length () == 0)
    res = this->resources_;
  else
    res = resources;

  DOMAIN_DATA_MANAGER->releaseResourceAllocation (res);
  return;
}

int DAnCE::ResourceCommitmentManager_i::add_to_commited_resource (
    ::Deployment::ResourceAllocations res)
{
  DANCE_TRACE ("DAnCE::ResourceCommitmentManager_i::add_to_commited_resource");
  
  CORBA::ULong const current_length = this->resources_.length ();

  this->resources_.length (current_length + res.length ());

  for (CORBA::ULong i = 0;i < res.length ();i++)
    this->resources_[current_length + i] = res[i];

  return 0;
}
