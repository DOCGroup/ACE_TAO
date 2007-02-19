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


CIAO::MonitorCB::MonitorCB (
  CORBA::ORB_ptr orb,
  Deployment::TargetManager_ptr target,
  int interval) :
    orb_ (orb),
    target_mgr_ (target),
    interval_ (interval)
{
}

int CIAO::MonitorCB::update_data (::Deployment::Domain& data)
{
  CORBA::StringSeq elements;
  elements.length (0);

  // data to be updated ...
  ::Deployment::DomainUpdateKind update_kind
      = ::Deployment::UpdateDynamic;

  // The copy of the domain variable
  ::Deployment::Domain_var domain
      = new ::Deployment::Domain (data);

  try
  {
    if (CIAO::debug_level () > 20)
      {
        ACE_DEBUG ((LM_DEBUG , "CIAO::NM::MonitorCB::Making a call to update\n"));
      }
    target_mgr_->updateDomain (elements , domain , update_kind);
  }
  catch (CORBA::Exception& )
    {
      //ACE_DEBUG ((LM_DEBUG, "CIAO::NM::MonitorCB::Unknown Exception\n"));
      //ex._tao_print_exception ("NM::MonitorCB::updateDomain");
    }
  catch (...)
    {
      // ignore
    }
  return 0;
}
