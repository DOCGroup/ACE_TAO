#include "Iterator_i.h"

#include "orbsvcs/Log/Log_Constraint_Interpreter.h"
#include "orbsvcs/Log/Log_Constraint_Visitors.h"
#include "orbsvcs/DsLogAdminC.h"

ACE_RCSID (Log,
           Iterator_i,
           "$Id$")

// iterator inactivity timeout
ACE_Time_Value
TAO_Iterator_i::timeout_(60 * 60);

TAO_Iterator_i::TAO_Iterator_i (ACE_Reactor* reactor,
				TAO_LogRecordStore::LOG_RECORD_STORE_ITER iter,
				TAO_LogRecordStore::LOG_RECORD_STORE_ITER iter_end,
                                CORBA::ULong start,
                                const char *constraint,
                                CORBA::ULong max_rec_list_len
                                )
  : iter_ (iter),
    iter_end_ (iter_end),
    current_position_(start),
    constraint_ (constraint),
    max_rec_list_len_ (max_rec_list_len),
    reactor_ (reactor)
{
   if (this->timeout_ != ACE_Time_Value::zero) 
     {
       this->timer_id_ = this->reactor_->schedule_timer (this, 0, this->timeout_);
     }
}


TAO_Iterator_i::~TAO_Iterator_i (void)
{
  // cancel timer
  if (this->timer_id_ != -1)
    {
      this->reactor_->cancel_timer (this->timer_id_); 
    }
}


DsLogAdmin::RecordList*
TAO_Iterator_i::get (CORBA::ULong position,
                 CORBA::ULong how_many
                 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidParam))
{
  // reset timer
  if (this->timer_id_ != -1) 
    {
      this->reactor_->cancel_timer (this->timer_id_);
      this->timer_id_ = this->reactor_->schedule_timer (this, 0, this->timeout_);
    }

  if (position < current_position_)
    {
      ACE_THROW_RETURN (DsLogAdmin::InvalidParam (), 0);
    }

  if (how_many == 0)
    {
      how_many = this->max_rec_list_len_;
    }

  // Use an Interpreter to build an expression tree.
  TAO_Log_Constraint_Interpreter interpreter (constraint_.in ()
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Sequentially iterate over all the records and pick the ones that
  // meet the constraints.

  // Allocate the list of <how_many> length.
  DsLogAdmin::RecordList* rec_list;
  ACE_NEW_THROW_EX (rec_list,
                    DsLogAdmin::RecordList (how_many),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  CORBA::ULong count = 0;
  CORBA::ULong current_position = this->current_position_;

  for ( ;
       ((this->iter_ != this->iter_end_) && (count < how_many));
       ++this->iter_)
    {
      // Use an evaluator.
      TAO_Log_Constraint_Visitor visitor ((*this->iter_).int_id_);

      // Does it match the constraint?
      if (interpreter.evaluate (visitor) == 1)
	{
          if (++current_position >= position)
	    {
	      (*rec_list)[count] = (*this->iter_).int_id_;
	      // copy the log record.
	      count++;
            }
	}
    }

  rec_list->length (count);
  this->current_position_ = current_position;

  if (count == 0 && this->iter_ == this->iter_end_)
    {
      // destroy this object..
      this->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (rec_list);
    }

  return rec_list;
}


void
TAO_Iterator_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::POA_ptr poa = this->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::ObjectId_var oid =
    poa->servant_to_id (this
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Goodbye cruel world...
  // deactivate from the poa.
  poa->deactivate_object (oid.in ()
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  return;
}


int
TAO_Iterator_i::handle_timeout(const ACE_Time_Value&, const void*)
{
  this->destroy();
  return 0;
}
