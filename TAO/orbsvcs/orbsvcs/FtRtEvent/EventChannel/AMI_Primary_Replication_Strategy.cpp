// $Id$

#include "orbsvcs/FtRtEvent/EventChannel/AMI_Primary_Replication_Strategy.h"
#include "ace/Synch_T.h"
#include "orbsvcs/FtRtEvent/EventChannel/GroupInfoPublisher.h"
#include "orbsvcs/FtRtEvent/EventChannel/Request_Context_Repository.h"
#include "orbsvcs/FtRtEvent/EventChannel/create_persistent_poa.h"
#include "orbsvcs/FtRtEvent/EventChannel/Update_Manager.h"
#include "tao/Utils/PolicyList_Destroyer.h"
#include "orbsvcs/FtRtEvent/EventChannel/ObjectGroupManagerHandler.h"
#include "tao/Utils/Implicit_Deactivator.h"
#include "../Utils/resolve_init.h"
#include "../Utils/ScopeGuard.h"
#include "../Utils/Log.h"

ACE_RCSID (EventChannel,
           AMI_Primary_Replication_Strategy,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

AMI_Primary_Replication_Strategy::AMI_Primary_Replication_Strategy(bool mt)
  : handler_(this),
    mutex_ (mt ? new ACE_SYNCH_RW_MUTEX : 0)
{
}

AMI_Primary_Replication_Strategy::~AMI_Primary_Replication_Strategy()
{
    running_= false;
    this->wait();
}

int  AMI_Primary_Replication_Strategy::acquire_read (void)
{
  return mutex_ ? mutex_->acquire_read() : 0;
}

int  AMI_Primary_Replication_Strategy::acquire_write (void)
{
  return mutex_ ? mutex_->acquire_write() : 0;
}

int  AMI_Primary_Replication_Strategy::release (void)
{
  return  mutex_ ? mutex_->release() : 0;
}

int AMI_Primary_Replication_Strategy::svc()
{
  ACE_TRY_NEW_ENV {
    int argc = 0;
    char** argv = 0;
    orb_ = CORBA::ORB_init (argc, argv);

    root_poa_ =  resolve_init<PortableServer::POA>(orb_.in(), "RootPOA"
      ACE_ENV_ARG_PARAMETER);

    // create POAManager
    mgr_ = root_poa_->the_POAManager();


    mgr_->activate();

    PortableServer::IdUniquenessPolicy_var id_uniqueness_policy =
      root_poa_->create_id_uniqueness_policy(PortableServer::MULTIPLE_ID
      ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK

    TAO::Utils::PolicyList_Destroyer policy_list(3);
    policy_list.length(1);
    policy_list[0] = PortableServer::IdUniquenessPolicy::_duplicate(
        id_uniqueness_policy.in()
      );
    poa_ = create_persistent_poa(root_poa_, mgr_, "AMI_Update", policy_list
      ACE_ENV_ARG_PARAMETER);

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
   Update_Manager* manager = 0;
   bool success = false;

    FTRT::TransactionDepth transaction_depth =
      Request_Context_Repository().get_transaction_depth();

   const FtRtecEventChannelAdmin::EventChannelList& backups =
     GroupInfoPublisher::instance()->backups();

   size_t num_backups = backups.length();

   if ((size_t)transaction_depth > num_backups) {
     TAO_FTRTEC::Log(3, "Throwing FTRT::TransactionDepthTooHigh\n");
     ACE_THROW(FTRT::TransactionDepthTooHigh());
   }

   ACE_NEW_THROW_EX(manager,
                    Update_Manager(event, backups.length(), transaction_depth-1, success),
                    CORBA::NO_MEMORY());


   Request_Context_Repository().set_transaction_depth(0 ACE_ENV_ARG_PARAMETER);

   for (size_t i = 0; i < num_backups; ++i)  {
      PortableServer::ObjectId oid;
      ACE_TRY_EX(block1) {
          FTRT::AMI_UpdateableHandler_ptr handler = handler_.activate(manager, i, oid
                                                                   ACE_ENV_ARG_PARAMETER);

          ACE_TRY_CHECK_EX(block1);
          FtRtecEventChannelAdmin::EventChannel_ptr obj = backups[i];
          // send set_update request to all the backup replicas

          obj->sendc_set_update(handler, state
                                      ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK_EX(block1);
       }
       ACE_CATCHANY {
         ACE_PRINT_EXCEPTION(ex, "AMI_Primary_Replication_Strategy::replicate_request : ");
         manager->handle_exception(i);
         if (oid.length())
           poa_->deactivate_object(oid);
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
     TAO_FTRTEC::Log(3, "Throwing FTRT::TransactionDepthTooHigh\n");
     ACE_THROW(FTRT::TransactionDepthTooHigh());
   }

}

void
AMI_Primary_Replication_Strategy::add_member(const FTRT::ManagerInfo & info,
                                             CORBA::ULong object_group_ref_version
                                             ACE_ENV_ARG_DECL)
{
  ACE_Auto_Event event;
  const FtRtecEventChannelAdmin::EventChannelList& backups =
     GroupInfoPublisher::instance()->backups();

  size_t num_backups = backups.length();
  ObjectGroupManagerHandler add_member_handler(event, num_backups+1);
  // The extra one is to prevent the event been signaled prematurely.

  PortableServer::ObjectId_var oid =
    root_poa_->activate_object(&add_member_handler ACE_ENV_ARG_PARAMETER);

  TAO::Utils::Implicit_Deactivator deactivator(&add_member_handler);

  CORBA::Object_var obj =
    root_poa_->id_to_reference(oid.in() ACE_ENV_ARG_PARAMETER);

  FTRT::AMI_ObjectGroupManagerHandler_var handler =
    FTRT::AMI_ObjectGroupManagerHandler::_narrow(obj.in() ACE_ENV_ARG_PARAMETER);

  for (unsigned i = 0; i < num_backups; ++i) {
    ACE_TRY {
      backups[i]->sendc_add_member(handler.in(),
                                   info,
                                   object_group_ref_version
                                   ACE_ENV_ARG_PARAMETER);
    }
    ACE_CATCHALL {
      add_member_handler.add_member_excep(0 ACE_ENV_ARG_PARAMETER);
    }
    ACE_ENDTRY;
  }
  // decrement the number of members so the event can be signaled once
  // all replys have been received.
  add_member_handler.add_member_excep(0 ACE_ENV_ARG_PARAMETER);

  event.wait();
}

TAO_END_VERSIONED_NAMESPACE_DECL
