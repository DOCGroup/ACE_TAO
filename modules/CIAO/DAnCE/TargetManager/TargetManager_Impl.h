// -*- C++ -*-
//
// $Id$


#ifndef TARGETMANAGERDAEMONI_H_
#define TARGETMANAGERDAEMONI_H_

#include "TargetManagerDaemonS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace DAnCE
{
class TARGETMANAGER_STUB_Export TargetManagerDaemon_i
  : public virtual POA_DAnCE::TargetManagerDaemon
{
public:
  // Constructor 
  TargetManagerDaemon_i (CORBA::ORB_ptr orb);
  
  // Destructor 
  virtual ~TargetManagerDaemon_i (void);
  
  void init (const ACE_TCHAR *file);
  
  virtual
  void shutdown (void);
  
  virtual
  ::Deployment::Domain * getAllResources (void);
  
  virtual
  ::Deployment::Domain * getAvailableResources (void);
  
  virtual
  ::Deployment::ResourceCommitmentManager_ptr createResourceCommitment (
    const ::Deployment::ResourceAllocations & resources);
  
  virtual
  void destroyResourceCommitment (
    ::Deployment::ResourceCommitmentManager_ptr manager);
  
  virtual
  void updateDomain (
    const ::CORBA::StringSeq & elements,
    const ::Deployment::Domain & domainSubset,
    ::Deployment::DomainUpdateKind updateKind);
  
 private:
  CORBA::ORB_var orb_;
};
}


#endif /* TARGETMANAGERDAEMONI_H_  */

