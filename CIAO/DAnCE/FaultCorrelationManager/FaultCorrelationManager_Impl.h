// -*- C++ -*-

//=============================================================================
/**
 *  @file    FaultCorrelationManager_Impl.h
 *
 *  $Id$
 *
 *  @author Friedhelm Wolf <fwolf@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef FAULTCORRELATIONMANAGER_IMPL_H_
#define FAULTCORRELATIONMANAGER_IMPL_H_

#include "tao/ORB.h"
#include "FaultCorrelationManager_export.h"
#include "Deployment/FaultCorrelationManagerS.h"
#include "Deployment/Deployment_common.h"

namespace DAnCE
{
  /**
   *  @brief Implementation of a manager to provide fail-stop behavior
   *         for component deployments.
   */
  class FAULTCORRELATIONMANAGER_Export FaultCorrelationManager_Impl : 
    public virtual POA_FTDeployment::FaultCorrelationManager
  {
  public:
    // the fixed listener port is caused by the usage of CCM Object locator
    FaultCorrelationManager_Impl (CORBA::ORB_ptr orb,
                                  const PROPERTY_MAP &options);

    virtual ~FaultCorrelationManager_Impl();

    virtual void stop_failver_unit (const char * fou_id);

    // methods from inherited interfaces
    virtual void proc_failure (const char *,
                               const char *);

  private:
    CORBA::ORB_var orb_;
    PROPERTY_MAP properties_;
  };
}

#endif /* FAULTCORRELATIONMANAGER_IMPL_H_ */
