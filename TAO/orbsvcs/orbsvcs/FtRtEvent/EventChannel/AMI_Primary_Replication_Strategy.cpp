// $Id$

#include "AMI_Primary_Replication_Strategy.h"
#include "ace/Synch_T.h"
#include "GroupInfoPublisher.h"
#include "../Utils/ScopeGuard.h"
#include "Request_Context_Repository.h"
#include "../Utils/resolve_init.h"
#include "create_persistent_poa.h"
#include "Update_Manager.h"
#include "tao/Utils/PolicyList_Destroyer.h"

ACE_RCSID (EventChannel,
           AMI_Primary_Replication_Strategy,
           "$Id$")

AMI_Primary_Replication_Strategy::AMI_Primary_Replication_Strategy()
: handler_(this)
{
}

AMI_Primary_Replication_Strategy::~AMI_Primary_Replication_Strategy()
{
    running_= false;
    this->wait();
}

int  AMI_Primary_Replication_Strategy::acquire_read (void)
{
  return mutex_.acquire_read();
}

int  AMI_Primary_Replication_Strategy::acquire_write (void)
{
  return mutex_.acquire_write();
}

int  AMI_Primary_Replication_Strategy::release (void)
{
  return mutex_.release();
}


int AMI_Primary_Replication_Strategy::init()
{
    return this->activate();
}

int AMI_Primary_Replication_Strategy::svc()
{
  ACE_TRY_NEW_ENV {
    int argc = 0;
    char** argv = 0;
    orb_ = CORBA::ORB_init (argc, argv);
    ACE_TRY_CHECK;

    PortableServer::POA_var
      root_poa =  resolve_init<PortableServer::POA>(orb_.in(), "RootPOA"
      ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    // create POAManager
    mgr_ = root_poa->the_POAManager(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;


    mgr_->activate(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    PortableServer::IdUniquenessPolicy_var id_uniqueness_policy =
      root_poa->create_id_uniqueness_policy(PortableServer::MULTIPLE_ID
      ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK

    TAO::Utils::PolicyList_Destroyer policy_list(3);
    policy_list.length(1);
    policy_list[0] = PortableServer::IdUniquenessPolicy::_duplicate(
        id_uniqueness_policy.in()
      );
    poa_ = create_persistent_poa(root_poa, mgr_, "AMI_Update", policy_list
      ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    id_uniqueness_policy->destroy();

    running_ = true;
    while (running_) {
      if (orb_->work_pending())
        orb_->perform_work();
    }
    orb_->destroy();
  }
  ACE_CATCHANY {
    ACE_PRINT_EXCEPTION(ex, "AMI_Primary_Replication_Strategy::svc");
    running_ = false;
    return -1;
  }
  ACE_ENDTRY;
  running_ = false;
  return 0;
}

void
AMI_Primary_Replication_Strategy::replicate_request(
  const FTRT::State& state,
  RollbackOperation rollback,
  const FtRtecEventChannelAdmin::ObjectId& oid
  ACE_ENV_ARG_DECL)
{
   ACE_Auto_Event event;
   Update_Manager* manager;
   bool success;

    FTRT::TransactionDepth transaction_depth =
      Request_Context_Repository().get_transaction_depth();

   const FtRtecEventChannelAdmin::EventChannelList& backups =
     GroupInfoPublisher::instance()->backups();

   size_t num_backups = backups.length();

   if ((size_t)transaction_depth > num_backups)
     ACE_THROW(FTRT::TransactionDepthTooHigh());

   ACE_NEW_THROW_EX(manager,
                    Update_Manager(event, backups.length(), transaction_depth-1, success),
                    CORBA::NO_MEMORY());


   Request_Context_Repository().set_transaction_depth(0 ACE_ENV_ARG_PARAMETER);
   ACE_CHECK;

   for (size_t i = 0; i < num_backups; ++i)  {
       ACE_TRY_EX(block1) {
         PortableServer::ObjectId oid;
          FTRT::AMI_UpdateableHandler_ptr handler = handler_.activate(manager, i, oid
                                                                   ACE_ENV_ARG_PARAMETER);

          ScopeGuard guard = MakeObjGuard(*poa_.in(),
                                          &PortableServer::POA::deactivate_object,
                                          oid);
          ACE_TRY_CHECK_EX(block1);
          FtRtecEventChannelAdmin::EventChannel_ptr obj = backups[i];
          // send set_update request to all the backup replicas

          obj->sendc_set_update(handler, state
                                      ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK_EX(block1);
          guard.Dismiss();
       }
       ACE_CATCHANY {
         ACE_PRINT_EXCEPTION(ex, "AMI_Primary_Replication_Strategy::replicate_request : ");
         manager->handle_exception(i);
       }
       ACE_ENDTRY;
   }
   // wait until the first transaction_depth replicas replied.
   event.wait();

   if (!success) { // replication failed, transaction depth too high
     for (size_t i =0; i < num_backups; ++i)  {
       ACE_TRY_EX(block2) {
         (backups[i]->*rollback)(oid ACE_ENV_ARG_PARAMETER);
         ACE_TRY_CHECK_EX(block2);
       }
       ACE_CATCHALL {
       }
       ACE_ENDTRY;
     }

     ACE_THROW(FTRT::TransactionDepthTooHigh());
   }

}
