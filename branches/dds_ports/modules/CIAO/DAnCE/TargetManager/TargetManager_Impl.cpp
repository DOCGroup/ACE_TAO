// -*- C++ -*-
//
// $Id$

#include "TargetManager_Impl.h"
#include "DomainDataManager.h"

namespace DAnCE
{
// Implementation skeleton constructor
TargetManagerDaemon_i::TargetManagerDaemon_i (CORBA::ORB_ptr orb)
  : orb_ (::CORBA::ORB::_duplicate (orb))
{
}

// Implementation skeleton destructor
TargetManagerDaemon_i::~TargetManagerDaemon_i (void)
{
}

void TargetManagerDaemon_i::init (const ACE_TCHAR *file)
{
  DAnCE::DOMAIN_DATA_MANAGER->init (this->orb_.in (),
                                    this->_this (),
                                    file);
}

void TargetManagerDaemon_i::shutdown (void)
{
  // Add your implementation here
}

::Deployment::Domain * TargetManagerDaemon_i::getAllResources (void)
{
  return DAnCE::DOMAIN_DATA_MANAGER->get_initial_domain ();
}

::Deployment::Domain * TargetManagerDaemon_i::getAvailableResources (void)
{
  return DAnCE::DOMAIN_DATA_MANAGER->get_current_domain ();
}

::Deployment::ResourceCommitmentManager_ptr TargetManagerDaemon_i::createResourceCommitment (
  const ::Deployment::ResourceAllocations & resources)
{
  return DAnCE::DOMAIN_DATA_MANAGER->commitResources (resources);
}

void TargetManagerDaemon_i::destroyResourceCommitment (
  ::Deployment::ResourceCommitmentManager_ptr manager)
{
  return DAnCE::DOMAIN_DATA_MANAGER->releaseResources (manager);
}

void TargetManagerDaemon_i::updateDomain (
  const ::CORBA::StringSeq & elements,
  const ::Deployment::Domain & domainSubset,
  ::Deployment::DomainUpdateKind updateKind)
{
  DAnCE::DOMAIN_DATA_MANAGER->update_domain (elements, domainSubset, updateKind);
}


}
