//$Id$
#include "Current.h"
#include "tao/ORB_Core.h"
#include "Distributable_Thread.h"
#include "tao/RTCORBA/Priority_Mapping_Manager.h"
#include "tao/RTCORBA/RT_Current.h"
#include "ace/ACE.h"

//#include "ThreadAction.h"

ACE_Atomic_Op<TAO_SYNCH_MUTEX, long> guid_counter;

u_long
TAO_DTId_Hash::operator () (const IdType &id) const
{
  return ACE::hash_pjw ((const char *) id.get_buffer (),
                        id.length ());
}

TAO_RTScheduler_Current::TAO_RTScheduler_Current (void)
{

}


void
TAO_RTScheduler_Current::init (TAO_ORB_Core* orb 
			                         ACE_ENV_ARG_DECL)
{
  this->orb_ = orb;
  
  // Create the RT_Current.
  RTCORBA::Current_ptr current;
  ACE_NEW_THROW_EX (current,
                    TAO_RT_Current (orb),
                    CORBA::NO_MEMORY (
				      CORBA::SystemException::_tao_minor_code (
				       TAO_DEFAULT_MINOR_CODE,
				       ENOMEM),
				      CORBA::COMPLETED_NO));
  this->rt_current_ = current;
}

void
TAO_RTScheduler_Current::rt_current (RTCORBA::Current_ptr rt_current)
{
  this->rt_current_ = RTCORBA::Current::_duplicate (rt_current);
}

TAO_ORB_Core* 
TAO_RTScheduler_Current::orb (void)
{
  return this->orb_;
}

DT_Hash_Map*
TAO_RTScheduler_Current::dt_hash (void)
{
  return &this->dt_hash_;
}


void
TAO_RTScheduler_Current::begin_scheduling_segment(const char * name,
						  CORBA::Policy_ptr sched_param,
						  CORBA::Policy_ptr implicit_sched_param
						  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   RTScheduling::Current::UNSUPPORTED_SCHEDULING_DISCIPLINE))
{
  TAO_RTScheduler_Current_i *impl = this->implementation ();
  
  if (impl == 0)
    {
      ACE_NEW_THROW_EX (impl,
			TAO_RTScheduler_Current_i (this->orb_,
						   &this->dt_hash_),
			CORBA::NO_MEMORY (
					  CORBA::SystemException::_tao_minor_code (
					  TAO_DEFAULT_MINOR_CODE,
					  ENOMEM),
					  CORBA::COMPLETED_NO));
      ACE_CHECK;

      this->implementation (impl);
    }
  
  impl->begin_scheduling_segment (name,
				  sched_param,
				  implicit_sched_param
				  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}


void 
TAO_RTScheduler_Current::update_scheduling_segment (const char * name,
						    CORBA::Policy_ptr sched_param,
						    CORBA::Policy_ptr implicit_sched_param
						    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   RTScheduling::Current::UNSUPPORTED_SCHEDULING_DISCIPLINE))
{
  TAO_RTScheduler_Current_i *impl = this->implementation ();
  
  if (impl == 0)
    ACE_THROW (CORBA::BAD_INV_ORDER ());
  
  impl->update_scheduling_segment (name,
				   sched_param,
				   implicit_sched_param
				   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void 
TAO_RTScheduler_Current::end_scheduling_segment (const char * name
						 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_RTScheduler_Current_i *impl = this->implementation ();
  
  if (impl == 0)
    {
      ACE_ERROR ((LM_ERROR,
		  "Missing scheduling context OR DT cancelled\n"));
      ACE_THROW (CORBA::BAD_INV_ORDER ());
      return;
    }
  
  impl->end_scheduling_segment (name
				ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

RTScheduling::DistributableThread_ptr 
TAO_RTScheduler_Current::lookup(const RTScheduling::Current::IdType & id
				ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  RTScheduling::DistributableThread_var DT;
  int result = this->dt_hash_.find (id,
				    DT);
  if (result == 0)
    return DT._retn ();
  else return RTScheduling::DistributableThread::_nil ();
}

// returns a null reference if
// the distributable thread is
// not known to the local scheduler

RTScheduling::DistributableThread_ptr
TAO_RTScheduler_Current::spawn (RTScheduling::ThreadAction_ptr start,
				CORBA::VoidData data,
				const char* name,
				CORBA::Policy_ptr sched_param,
				CORBA::Policy_ptr implicit_sched_param,
				CORBA::ULong stack_size,
				RTCORBA::Priority base_priority
				ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_RTScheduler_Current_i *impl = this->implementation ();
  
  if (impl == 0)
    ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (), 0);
  
  return impl->spawn (start,
		      data,
		      name,
		      sched_param,
		      implicit_sched_param,
		      stack_size,
		      base_priority
		      ACE_ENV_ARG_PARAMETER);
}

RTScheduling::Current::IdType * 
TAO_RTScheduler_Current::id (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_RTScheduler_Current_i *impl = this->implementation ();
  
  if (impl == 0)
    ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (), 0);

  return impl->id (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Policy_ptr 
TAO_RTScheduler_Current::scheduling_parameter (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  TAO_RTScheduler_Current_i *impl = this->implementation ();
  
  if (impl == 0)
    ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (), 0);

  return impl->scheduling_parameter (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Policy_ptr 
TAO_RTScheduler_Current::implicit_scheduling_parameter (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_RTScheduler_Current_i *impl = this->implementation ();
  
  if (impl == 0)
    ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (), 0);

  return impl->implicit_scheduling_parameter (ACE_ENV_SINGLE_ARG_PARAMETER);
}

RTScheduling::Current::NameList * 
TAO_RTScheduler_Current::current_scheduling_segment_names (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_RTScheduler_Current_i *impl = this->implementation ();
  
  if (impl == 0)
    ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (), 0);
  
  return impl->current_scheduling_segment_names (ACE_ENV_SINGLE_ARG_PARAMETER);
}

RTCORBA::Priority 
TAO_RTScheduler_Current::the_priority (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->rt_current_->the_priority (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void 
TAO_RTScheduler_Current::the_priority (RTCORBA::Priority the_priority
				       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->rt_current_->the_priority(the_priority
				  ACE_ENV_ARG_PARAMETER);
}

TAO_RTScheduler_Current_i*
TAO_RTScheduler_Current::implementation (TAO_RTScheduler_Current_i* new_current)
{
  TAO_TSS_Resources *tss =
    TAO_TSS_RESOURCES::instance ();
  
  TAO_RTScheduler_Current_i *old =
    ACE_static_cast (TAO_RTScheduler_Current_i *,
                     tss->rtscheduler_current_impl_);
  tss->rtscheduler_current_impl_ = new_current;

  return old;
}

TAO_RTScheduler_Current_i*
TAO_RTScheduler_Current::implementation (void)
{
  TAO_TSS_Resources *tss =
    TAO_TSS_RESOURCES::instance ();
  
  TAO_RTScheduler_Current_i* impl =
    ACE_static_cast (TAO_RTScheduler_Current_i *,
		     tss->rtscheduler_current_impl_);
  return impl;
}

TAO_ORB_Core* 
TAO_RTScheduler_Current_i::orb (void)
{
  return this->orb_;
}

DT_Hash_Map*
TAO_RTScheduler_Current_i::dt_hash (void)
{
  return this->dt_hash_;
}

 RTScheduling::Scheduler_ptr
TAO_RTScheduler_Current_i::scheduler (void)
{
  return RTScheduling::Scheduler::_duplicate (this->scheduler_.in ());
}

TAO_RTScheduler_Current_i::TAO_RTScheduler_Current_i (TAO_ORB_Core* orb,
						      DT_Hash_Map* dt_hash
						      ACE_ENV_ARG_DECL)
  :orb_ (orb),
   dt_ (RTScheduling::DistributableThread::_nil ()),
   previous_current_ (0),
   dt_hash_ (dt_hash)
{
  CORBA::Object_ptr scheduler_obj = this->orb_->object_ref_table ().resolve_initial_references ("RTScheduler"
												ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  this->scheduler_ = RTScheduling::Scheduler::_narrow (scheduler_obj
						       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

TAO_RTScheduler_Current_i::TAO_RTScheduler_Current_i (TAO_ORB_Core* orb,
						      DT_Hash_Map* dt_hash,
						      RTScheduling::Current::IdType guid,
						      const char * name,
						      CORBA::Policy_ptr sched_param,
						      CORBA::Policy_ptr implicit_sched_param,
						      RTScheduling::DistributableThread_ptr dt,
						      TAO_RTScheduler_Current_i* prev_current
						      ACE_ENV_ARG_DECL)
  : orb_ (orb),
    guid_ (guid),
    name_ (CORBA::string_dup (name)),
    sched_param_ (sched_param),
    implicit_sched_param_ (implicit_sched_param),
    dt_ (RTScheduling::DistributableThread::_duplicate (dt)),
    previous_current_ (prev_current),
    dt_hash_ (dt_hash)
{
  CORBA::Object_ptr scheduler_obj = orb->object_ref_table ().resolve_initial_references ("RTScheduler"
											 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  this->scheduler_ = RTScheduling::Scheduler::_narrow (scheduler_obj
						       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_RTScheduler_Current_i::begin_scheduling_segment(const char * name,
						    CORBA::Policy_ptr sched_param,
						    CORBA::Policy_ptr implicit_sched_param
						    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   RTScheduling::Current::UNSUPPORTED_SCHEDULING_DISCIPLINE))
{
  // Check if it is a new Scheduling Segmnet
  if (this->guid_.length () == 0) 
    {
      //Generate GUID
      long temp = ++guid_counter;
      this->guid_.length (sizeof(long));
      ACE_OS::memcpy (this->guid_.get_buffer (),
		      &temp,
		      sizeof(long));
      
      int guid;
      ACE_OS::memcpy (&guid,
		      this->guid_.get_buffer (),
		      this->guid_.length ());
      
      
      // Inform the scheduler of the new scheduling segment.
      this->scheduler_->begin_new_scheduling_segment (this->guid_,
						      name,
						      sched_param,
						      implicit_sched_param
						      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      
      if (CORBA::is_nil (this->dt_.in ()))
	//Create new DT.
	this->dt_ = TAO_DistributableThread_Factory::create_DT ();
      
      //Add new DT to map
      int result = this->dt_hash_->bind (this->guid_,
					 this->dt_);

      // Error in binding to the map - cancel thread.
      if (result != 0)
	{
	  this->cancel_thread (ACE_ENV_SINGLE_ARG_PARAMETER);
	  ACE_CHECK;
	}
      
      // Remember parameters for the scheduling segment.
      this->name_ = CORBA::string_dup (name);
      this->sched_param_ = CORBA::Policy::_duplicate (sched_param);
      this->implicit_sched_param_ = CORBA::Policy::_duplicate (implicit_sched_param);      
      
    }
  else //Nested segment
    {
      // Check current DT state.
      if (this->dt_->state () == RTScheduling::DistributableThread::CANCELLED)
	{
	  this->cancel_thread (ACE_ENV_SINGLE_ARG_PARAMETER);
	  ACE_CHECK;
	}
      
      // Inform scheduler of start of nested scheduling segment.
      this->scheduler_->begin_nested_scheduling_segment
	(this->guid_,
	 name,
	 sched_param,
	 implicit_sched_param
	 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      
      TAO_TSS_Resources *tss =
	TAO_TSS_RESOURCES::instance ();
      
      TAO_RTScheduler_Current_i* new_current;
	ACE_NEW_THROW_EX (new_current,
			  TAO_RTScheduler_Current_i (this->orb_,
						     this->dt_hash_,
						     this->guid_,
						     name,
						     sched_param,
						     implicit_sched_param,
						     this->dt_.in (),
						     this),
			  CORBA::NO_MEMORY (
					    CORBA::SystemException::_tao_minor_code (
					  TAO_DEFAULT_MINOR_CODE,
					  ENOMEM),
					  CORBA::COMPLETED_NO));
	ACE_CHECK;
      
      tss->rtscheduler_current_impl_ = new_current;
    }
}

void 
TAO_RTScheduler_Current_i::update_scheduling_segment (const char * name,
						      CORBA::Policy_ptr sched_param,
						      CORBA::Policy_ptr implicit_sched_param
						      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   RTScheduling::Current::UNSUPPORTED_SCHEDULING_DISCIPLINE))
{
  // Check if DT has been cancelled
  if (this->dt_->state () == RTScheduling::DistributableThread::CANCELLED)
    {
      this->cancel_thread (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
     
  // Let scheduler know of the updates.
  this->scheduler_->update_scheduling_segment (this->guid_,
					       name,
					       sched_param,
					       implicit_sched_param
					       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  // Remember the new values.
  this->name_ = CORBA::string_dup (name);
  this->sched_param_ = CORBA::Policy::_duplicate (sched_param);
  this->implicit_sched_param_ = CORBA::Policy::_duplicate (implicit_sched_param);      
}

void 
TAO_RTScheduler_Current_i::end_scheduling_segment (const char * name
						   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Check if DT has been cancelled
  if (this->dt_->state () == RTScheduling::DistributableThread::CANCELLED)
    {
      this->cancel_thread (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }

  if (this->previous_current_ == 0)
    {
      // Let the scheduler know that the DT is
      // terminating.
      this->scheduler_->end_scheduling_segment(this->guid_,
					       name
					       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
	  
      // Cleanup DT.
      this->cleanup_DT ();
      
      // Cleanup current.
      this->cleanup_current ();
      
      // A Nested segment.
    } else { 
      
      // Inform scheduler of end of nested
      // scheduling segment.
      this->scheduler_->end_nested_scheduling_segment (this->guid_,
						       name,
						       this->previous_current_->sched_param_
						       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
	  
      // Cleanup current.
      this->cleanup_current ();
    }
}


// returns a null reference if
// the distributable thread is
// not known to the local scheduler
RTScheduling::DistributableThread_ptr
TAO_RTScheduler_Current_i::spawn (RTScheduling::ThreadAction_ptr start,
				  CORBA::VoidData data,
				  const char* name,
				  CORBA::Policy_ptr sched_param,
				  CORBA::Policy_ptr implicit_sched_param,
				  CORBA::ULong stack_size,
				  RTCORBA::Priority base_priority
				  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Check if DT has been cancelled.
  if (this->dt_->state () == RTScheduling::DistributableThread::CANCELLED)
    this->cancel_thread (ACE_ENV_SINGLE_ARG_PARAMETER);
  
  // Create new task for new DT.
  DTTask *dttask;
    
  // If no scheduling parameter is specified then use the current
  // implicit scheduling parameter as the scheduling parameter
  if (sched_param == 0)
    sched_param = this->implicit_sched_param_;

  RTScheduling::DistributableThread_var dt = TAO_DistributableThread_Factory::create_DT ();
  TAO_RTScheduler_Current_i *new_current;
  
  ACE_NEW_RETURN (new_current,
		  TAO_RTScheduler_Current_i (this->orb_,
					     this->dt_hash_),
		  0);

  new_current->DT (dt.in ());

  ACE_NEW_RETURN (dttask,
		  DTTask (//thread_manager_,
			  this->orb_,
			  this->dt_hash_, 	
			  new_current,
			  start,
			  data,
			  name,
			  sched_param,
			  implicit_sched_param),
		  0);
 
  if (dttask->activate_task (base_priority,
			     stack_size) == -1)
    {
      ACE_ERROR((LM_ERROR,
		 "Error in Spawning\n"));
      
      RTScheduling::DistributableThread::_nil ();
    }
  
  return dt._retn ();
}

int
DTTask::activate_task (RTCORBA::Priority base_priority,
		       CORBA::ULong stack_size
		       ACE_ENV_ARG_DECL)
{
  // Activate thread.
  long default_flags = THR_NEW_LWP | THR_JOINABLE;
  long flags =
    default_flags |
    this->orb_->orb_params ()->scope_policy () |
    this->orb_->orb_params ()->sched_policy ();
  
  CORBA::Object_var object = this->orb_->object_ref_table ().resolve_initial_references ("PriorityMappingManager"
											 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  
  RTCORBA::PriorityMappingManager_var mapping_manager =
    RTCORBA::PriorityMappingManager::_narrow (object.in ()
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  
  RTCORBA::PriorityMapping *pm =
    mapping_manager->mapping ();
  
  RTCORBA::NativePriority native_priority;
  pm->to_native (base_priority,
		 native_priority);
  
  size_t stack [1];
  stack [0] = stack_size;
  if (this->activate (flags,
		      1,
		      0,//force_active
		      native_priority,//priority
		      -1,//grp_id
		      0,//ACE_Task_Base
		      0,//thread_handles
		      0,//stack
		      stack//stack_size
		      ) == -1)
    {
      if (ACE_OS::last_error () == EPERM)
	ACE_ERROR_RETURN ((LM_ERROR,
			   ACE_TEXT ("Insufficient privilege to run this test.\n")),
			  -1);
    }
  return 0;
}

DTTask::DTTask (//ACE_Thread_Manager *manager,
		TAO_ORB_Core *orb,
		DT_Hash_Map *dt_hash,
		TAO_RTScheduler_Current_i* new_current,		
		RTScheduling::ThreadAction_ptr start,
		CORBA::VoidData data,
		const char *name,
		CORBA::Policy_ptr sched_param,
		CORBA::Policy_ptr implicit_sched_param)
  ://manager_ (manager),
   orb_ (orb),
   dt_hash_ (dt_hash),
   current_ (new_current),
   start_ (RTScheduling::ThreadAction::_duplicate (start)),
   data_ (data),
   name_ (CORBA::string_dup (name)),
   sched_param_ (CORBA::Policy::_duplicate (sched_param)),
   implicit_sched_param_ (CORBA::Policy::_duplicate (implicit_sched_param))
{
}

int 
DTTask::svc (void)
{
  ACE_TRY_NEW_ENV
    {

      TAO_TSS_Resources *tss =
	TAO_TSS_RESOURCES::instance ();
      
      tss->rtscheduler_current_impl_ = this->current_;
      
      this->current_->begin_scheduling_segment (this->name_.in (),
						this->sched_param_.in (),
						this->implicit_sched_param_.in ()
						ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      
      // Invoke entry point into new DT.
      this->start_->_cxx_do (this->data_
			     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      
      this->current_->end_scheduling_segment (this->name_.in ()
					     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return -1;
    }
  ACE_ENDTRY; 
  
  return 0;
}


RTScheduling::Current::IdType * 
TAO_RTScheduler_Current_i::id (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  
  RTScheduling::Current::IdType_var guid = this->guid_;
  return guid._retn ();
}


CORBA::Policy_ptr 
TAO_RTScheduler_Current_i::scheduling_parameter (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::Policy::_duplicate (this->sched_param_);
}

CORBA::Policy_ptr 
TAO_RTScheduler_Current_i::implicit_scheduling_parameter (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::Policy::_duplicate (this->implicit_sched_param_);
}

RTScheduling::Current::NameList * 
TAO_RTScheduler_Current_i::current_scheduling_segment_names (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  RTScheduling::Current::NameList* name_list;
  ACE_NEW_RETURN (name_list,
		  RTScheduling::Current::NameList,
		  0);

  TAO_RTScheduler_Current_i* current = this;

  for (int index = 0; current != 0; index++)
    {
      name_list->length (index+1);
      (*name_list) [index] = current->name ();
      current = current->previous_current_;
    }

  return name_list;
}

const char*
TAO_RTScheduler_Current_i::name (void)
{
  return CORBA::string_dup (this->name_.in ());
}

void
TAO_RTScheduler_Current_i::cancel_thread (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  int guid;
  ACE_OS::memcpy (&guid,
		  this->guid_.get_buffer (),
		  this->guid_.length ());
  
  ACE_DEBUG ((LM_DEBUG,
	      "Distributable Thread - %d is cancelled\n",
	      guid));
 
  // Let the scheduler know that the thread has
  // been cancelled.
  this->scheduler_->cancel (this->guid_
			    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  this->cleanup_DT ();
  
  // Remove all related nested currents.
  this->delete_all_currents ();
  
  // Throw exception.
  ACE_THROW (CORBA::THREAD_CANCELLED ());
}

void
TAO_RTScheduler_Current_i::cleanup_DT (void)
{
  // Remove DT from map.
  this->dt_hash_->unbind (this->guid_);
}

void
TAO_RTScheduler_Current_i::cleanup_current (void)
{
  TAO_TSS_Resources *tss =
    TAO_TSS_RESOURCES::instance ();
  
  tss->rtscheduler_current_impl_ = this->previous_current_;  

  // Delete this current.
  delete this;
}

void
TAO_RTScheduler_Current_i::delete_all_currents (void)
{
  TAO_RTScheduler_Current_i* current = this;
  
  while (current != 0)
    {
      TAO_RTScheduler_Current_i* prev_current = current->previous_current_;
      current->cleanup_current ();
      current = prev_current;
    }

  TAO_TSS_Resources *tss =
    TAO_TSS_RESOURCES::instance ();
  
  tss->rtscheduler_current_impl_ = tss->rtscheduler_previous_current_impl_;  
}

void
TAO_RTScheduler_Current_i::id (RTScheduling::Current::IdType guid)
{
  this->guid_ = guid;
}

void 
TAO_RTScheduler_Current_i::name (char * name)
{
  this->name_ = CORBA::string_dup (name);
}

RTScheduling::DistributableThread_ptr 
TAO_RTScheduler_Current_i::DT (void)
{
  return this->dt_._retn ();
}

void 
TAO_RTScheduler_Current_i::DT (RTScheduling::DistributableThread_ptr dt)
{
  this->dt_ = RTScheduling::DistributableThread::_duplicate (dt);
}

void 
TAO_RTScheduler_Current_i::scheduling_parameter (CORBA::Policy_ptr sched_param)
{
  this->sched_param_ = CORBA::Policy::_duplicate (sched_param);
}

void 
TAO_RTScheduler_Current_i::implicit_scheduling_parameter (CORBA::Policy_ptr implicit_sched_param)
{
  this->implicit_sched_param_ = CORBA::Policy::_duplicate (implicit_sched_param);
}
  

// *************************************************************

// *************************************************************
// Operations for class TAO_RTScheduler_Current_var
// *************************************************************

TAO_RTScheduler_Current_var::TAO_RTScheduler_Current_var (void) // default constructor
  : ptr_ (TAO_RTScheduler_Current::_nil ())
{}

::TAO_RTScheduler_Current_ptr
TAO_RTScheduler_Current_var::ptr (void) const
{
  return this->ptr_;
}

TAO_RTScheduler_Current_var::TAO_RTScheduler_Current_var (const ::TAO_RTScheduler_Current_var &p)
  : TAO_Base_var (),
    ptr_ (TAO_RTScheduler_Current::_duplicate (p.ptr ()))
{}

TAO_RTScheduler_Current_var::~TAO_RTScheduler_Current_var (void) // destructor
{
  CORBA::release (this->ptr_);
}

TAO_RTScheduler_Current_var &
TAO_RTScheduler_Current_var::operator= (TAO_RTScheduler_Current_ptr p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

TAO_RTScheduler_Current_var &
TAO_RTScheduler_Current_var::operator= (const ::TAO_RTScheduler_Current_var &p)
{
  if (this != &p)
  {
    CORBA::release (this->ptr_);
    this->ptr_ = ::TAO_RTScheduler_Current::_duplicate (p.ptr ());
  }
  return *this;
}

TAO_RTScheduler_Current_var::operator const ::TAO_RTScheduler_Current_ptr &() const
{
  return this->ptr_;
}

TAO_RTScheduler_Current_var::operator ::TAO_RTScheduler_Current_ptr &()
{
  return this->ptr_;
}

TAO_RTScheduler_Current_ptr
TAO_RTScheduler_Current_var::operator-> (void) const
{
  return this->ptr_;
}

TAO_RTScheduler_Current_ptr
TAO_RTScheduler_Current_var::in (void) const
{
  return this->ptr_;
}

TAO_RTScheduler_Current_ptr &
TAO_RTScheduler_Current_var::inout (void)
{
  return this->ptr_;
}

TAO_RTScheduler_Current_ptr &
TAO_RTScheduler_Current_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = ::TAO_RTScheduler_Current::_nil ();
  return this->ptr_;
}

TAO_RTScheduler_Current_ptr
TAO_RTScheduler_Current_var::_retn (void)
{
  // yield ownership of managed obj reference
  ::TAO_RTScheduler_Current_ptr val = this->ptr_;
  this->ptr_ = ::TAO_RTScheduler_Current::_nil ();
  return val;
}

TAO_RTScheduler_Current_ptr
TAO_RTScheduler_Current_var::duplicate (TAO_RTScheduler_Current_ptr p)
{
  return ::TAO_RTScheduler_Current::_duplicate (p);
}

void
TAO_RTScheduler_Current_var::release (TAO_RTScheduler_Current_ptr p)
{
  CORBA::release (p);
}

TAO_RTScheduler_Current_ptr
TAO_RTScheduler_Current_var::nil (void)
{
  return ::TAO_RTScheduler_Current::_nil ();
}

TAO_RTScheduler_Current_ptr
TAO_RTScheduler_Current_var::narrow (
    CORBA::Object *p
    ACE_ENV_ARG_DECL
  )
{
  return ::TAO_RTScheduler_Current::_narrow (p ACE_ENV_ARG_PARAMETER);
}

CORBA::Object *
TAO_RTScheduler_Current_var::upcast (void *src)
{
  TAO_RTScheduler_Current **tmp =
    ACE_static_cast (TAO_RTScheduler_Current **, src);
  return *tmp;
}

TAO_RTScheduler_Current_ptr TAO_RTScheduler_Current::_narrow (
    CORBA::Object_ptr obj
    ACE_ENV_ARG_DECL
  )
{
  return TAO_RTScheduler_Current::_unchecked_narrow (obj ACE_ENV_ARG_PARAMETER);
}

TAO_RTScheduler_Current_ptr TAO_RTScheduler_Current::_unchecked_narrow (
    CORBA::Object_ptr obj
    ACE_ENV_ARG_DECL_NOT_USED
  )
{
  if (CORBA::is_nil (obj))
    return TAO_RTScheduler_Current::_nil ();
  return
      ACE_reinterpret_cast
        (
          TAO_RTScheduler_Current_ptr,
            obj->_tao_QueryInterface
              (
                ACE_reinterpret_cast
                  (
                    ptr_arith_t,
                    &TAO_RTScheduler_Current::_narrow
                  )
              )
        );
}

TAO_RTScheduler_Current_ptr
TAO_RTScheduler_Current::_duplicate (TAO_RTScheduler_Current_ptr obj)
{
  if (!CORBA::is_nil (obj))
    obj->_add_ref ();
  return obj;
}

void *TAO_RTScheduler_Current::_tao_QueryInterface (ptr_arith_t type)
{
  void *retv = 0;
  if (type == ACE_reinterpret_cast
      (ptr_arith_t,
       &TAO_RTScheduler_Current::_narrow))
    retv = ACE_reinterpret_cast (void*, this);
  else if (type == ACE_reinterpret_cast (
					 ptr_arith_t,
					 &ACE_NESTED_CLASS (::RTScheduling, Current)::_tao_class_id)
	   )
    {
      retv = ACE_reinterpret_cast (void*, this);
    }
  else if (type == ACE_reinterpret_cast (
              ptr_arith_t,
              &::RTCORBA::Current::_tao_class_id)
            )
    {
      retv =
        ACE_reinterpret_cast (
            void *,
            ACE_static_cast (
                RTCORBA::Current_ptr,
                this
              )
          );
    }
  else if (type == ACE_reinterpret_cast (
              ptr_arith_t,
              &::CORBA::Current::_tao_class_id)
            )
    {
      retv =
        ACE_reinterpret_cast (
            void *,
            ACE_static_cast (
                CORBA::Current_ptr,
                this
              )
          );
    }
  else if (type == ACE_reinterpret_cast (
               ptr_arith_t,
               &CORBA::Object::_tao_class_id)
             )
    {
      retv =
        ACE_reinterpret_cast (
            void *,
            ACE_static_cast (CORBA::Object_ptr, this)
          );
    }

//   else if (type == ACE_reinterpret_cast
//     (ptr_arith_t,
//       &::RTScheduling::Current::_narrow))
//     retv = ACE_reinterpret_cast
//       (
//         void *,
//         ACE_static_cast
//           (
//             RTScheduling::Current_ptr,
//             this
//           )
//       );
//   else if (type == ACE_reinterpret_cast (ptr_arith_t, &CORBA::Object::_narrow))
//     retv = ACE_reinterpret_cast (void *,
//       ACE_static_cast (CORBA::Object_ptr, this));
  
  if (retv)
    this->_add_ref ();
  return retv;
}

const char* TAO_RTScheduler_Current::_interface_repository_id (void) const
{
  return "IDL:TAO_RTScheduler_Current:1.0";
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Equal_To<IdType>;
template class ACE_Hash_Map_Manager_Ex<IdType, RTScheduling::DistributableThread_var, TAO_DTId_Hash, ACE_Equal_To<IdType>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<IdType, RTScheduling::DistributableThread_var, TAO_DTId_Hash, ACE_Equal_To<IdType>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Entry<IdType, RTScheduling::DistributableThread_var>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<IdType, RTScheduling::DistributableThread_var, TAO_DTId_Hash, ACE_Equal_To<IdType>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<IdType, RTScheduling::DistributableThread_var, TAO_DTId_Hash, ACE_Equal_To<IdType>, TAO_SYNCH_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Equal_To<IdType>;
#pragma instantiate ACE_Hash_Map_Manager_Ex<IdType, RTScheduling::DistributableThread_var, TAO_DTId_Hash, ACE_Equal_To<IdType>, TAO_SYNCH_MUTEX>;
#pragma instantiate ACE_Hash_Map_Iterator_Ex<IdType, RTScheduling::DistributableThread_var, TAO_DTId_Hash, ACE_Equal_To<IdType>, TAO_SYNCH_MUTEX>;
#pragma instantiate ACE_Hash_Map_Entry<IdType, RTScheduling::DistributableThread_var>;
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<IdType, RTScheduling::DistributableThread_var, TAO_DTId_Hash, ACE_Equal_To<IdType>, TAO_SYNCH_MUTEX>;
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<IdType, RTScheduling::DistributableThread_var, TAO_DTId_Hash, ACE_Equal_To<IdType>, TAO_SYNCH_MUTEX>;

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
