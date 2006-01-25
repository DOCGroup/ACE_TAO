// $Id$
//----------------------------------------------------------------------------------
/**
 * @file MonitorCB.cpp
 *
 * @brief The Monitor Update class
 *
 * The class which updates the resource data
 *
 * @author Nilabja Roy <nilabjar@dre.vanderbilt.edu>
 */
//----------------------------------------------------------------------------------

#include "MonitorCB.h"
#include "CIAO_common.h"


MonitorCB::MonitorCB (CORBA::ORB_ptr orb,
                      Deployment::TargetManager_ptr target,
                      int interval
                      ):orb_ (orb),
                        target_mgr_ (target),
                        interval_ (interval)
{
}

int MonitorCB::update_data (::Deployment::Domain& data)
{
  CORBA::StringSeq elements;
  elements.length (0);

  // data to be updated ...
  ::Deployment::DomainUpdateKind update_kind
      = ::Deployment::UpdateAvailable;

  // The copy of the domain variable
  ::Deployment::Domain_var domain
      = new ::Deployment::Domain (data);

  try
  {
    if (CIAO::debug_level () > 9)
      {
        ACE_DEBUG ((LM_DEBUG , "---Making a call to update\n"));
      }
    target_mgr_->updateDomain (elements , domain , update_kind);
  }
  catch (CORBA::Exception&)
    {
      ACE_DEBUG ((LM_DEBUG, "Unknown Exception"));
    }

  return 0;
}
