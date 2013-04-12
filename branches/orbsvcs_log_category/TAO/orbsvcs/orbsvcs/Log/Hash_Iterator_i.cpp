// $Id$

#include "orbsvcs/Log/Hash_Iterator_i.h"
#include "orbsvcs/Log/Log_Constraint_Interpreter.h"
#include "orbsvcs/Log/Log_Constraint_Visitors.h"
#include "orbsvcs/DsLogAdminC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Hash_Iterator_i::TAO_Hash_Iterator_i (
  PortableServer::POA_ptr poa,
  ACE_Reactor* reactor,
  TAO_Hash_LogRecordStore* recordstore,
  TAO_Hash_LogRecordStore::LOG_RECORD_STORE_ITER iter,
  TAO_Hash_LogRecordStore::LOG_RECORD_STORE_ITER iter_end,
  CORBA::ULong start,
  const char *constraint,
  CORBA::ULong max_rec_list_len)
  : TAO_Iterator_i(poa, reactor),
    recordstore_ (recordstore),
    iter_ (iter),
    iter_end_ (iter_end),
    current_position_(start),
    constraint_ (constraint),
    max_rec_list_len_ (max_rec_list_len)
{
}


TAO_Hash_Iterator_i::~TAO_Hash_Iterator_i (void)
{
}


DsLogAdmin::RecordList*
TAO_Hash_Iterator_i::get (CORBA::ULong position, CORBA::ULong how_many)
{
  ACE_READ_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
                           guard,
                           this->recordstore_->lock (),
                           CORBA::INTERNAL ());

  if (position < current_position_)
    {
      throw DsLogAdmin::InvalidParam ();
    }

  if (how_many == 0)
    {
      how_many = this->max_rec_list_len_;
    }

  // Use an Interpreter to build an expression tree.
  TAO_Log_Constraint_Interpreter interpreter (constraint_.in ());

  // Sequentially iterate over all the records and pick the ones that
  // meet the constraints.

  // Allocate the list of <how_many> length.
  DsLogAdmin::RecordList* rec_list = 0;
  ACE_NEW_THROW_EX (rec_list,
                    DsLogAdmin::RecordList (how_many),
                    CORBA::NO_MEMORY ());
  rec_list->length (how_many);

  CORBA::ULong count = 0;
  CORBA::ULong current_position = this->current_position_;

  for ( ;
       ((this->iter_ != this->iter_end_) && (count < how_many));
       ++this->iter_)
    {
      // Use an evaluator.
      TAO_Log_Constraint_Visitor visitor (this->iter_->item ());

      // Does it match the constraint?
      if (interpreter.evaluate (visitor) == 1)
        {
          if (++current_position >= position)
            {
              (*rec_list)[count] = this->iter_->item ();
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
      this->destroy ();
    }

  return rec_list;
}

TAO_END_VERSIONED_NAMESPACE_DECL
