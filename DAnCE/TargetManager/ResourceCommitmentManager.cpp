// $Id$
//
#include "DomainDataManager.h"
#include "ResourceCommitmentManager.h"

// Implementation skeleton constructor
CIAO::ResourceCommitmentManager_i::ResourceCommitmentManager_i (void)
{
}

// Implementation skeleton destructor
CIAO::ResourceCommitmentManager_i::~ResourceCommitmentManager_i (void)
{
}

void CIAO::ResourceCommitmentManager_i::commitResources (
    const ::Deployment::ResourceAllocations& resources)
{
  CIAO::DomainDataManager::get_data_manager ()->commitResourceAllocation (resources);

  // commit succesful .. add to commited resource
  this->add_to_commited_resource (resources);
  return;
}

void CIAO::ResourceCommitmentManager_i::releaseResources (
    const ::Deployment::ResourceAllocations & resources)
{
  ::Deployment::ResourceAllocations res;

  // if the resources set is null , use the already allocated resources ..
  if (resources.length () == 0)
    res = this->resources_;
  else
    res = resources;

  CIAO::DomainDataManager::get_data_manager ()->releaseResourceAllocation (res);
  return;
}

int CIAO::ResourceCommitmentManager_i::add_to_commited_resource (
    ::Deployment::ResourceAllocations res)
{
  CORBA::ULong current_length = this->resources_.length ();

  this->resources_.length (current_length + res.length ());

  for (CORBA::ULong i =0;i < res.length ();i++)
    this->resources_[current_length + i] = res[i];

  return 0;
}
