/**
 * @file SyncScope_Setup.cpp
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "SyncScope_Setup.h"
#include "RIR_Narrow.h"
#include "tao/AnyTypeCode/Any.h"

SyncScope_Setup::SyncScope_Setup (CORBA::ORB_ptr orb,
                                  Messaging::SyncScope value)
{
  this->init (orb, value);
}

SyncScope_Setup::SyncScope_Setup (CORBA::ORB_ptr orb)
{
  this->init (orb, Messaging::SYNC_WITH_TARGET);
}

void
SyncScope_Setup::init (CORBA::ORB_ptr orb,
                       Messaging::SyncScope value)
{
  CORBA::PolicyManager_var policy_manager =
    RIR_Narrow<CORBA::PolicyManager>::resolve (orb,
                                               "ORBPolicyManager");

  CORBA::PolicyList policy_list (1);
  policy_list.length (1);

  CORBA::Any sync_scope;
  sync_scope <<= value;

  // @@ We need a helper class that automatically calls the
  //    destroy() method on each policy...
  policy_list[0] =
    orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                        sync_scope);
  policy_manager->set_policy_overrides (policy_list,
                                        CORBA::ADD_OVERRIDE);

  policy_list[0]->destroy ();
}

SyncScope_Setup::~SyncScope_Setup ()
{
}
