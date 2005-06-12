#include "Iterator_i.h"

#include "orbsvcs/Log/Log_Constraint_Interpreter.h"
#include "orbsvcs/Log/Log_Constraint_Visitors.h"
#include "orbsvcs/DsLogAdminC.h"

ACE_RCSID (Log,
           Iterator_i,
           "$Id$")

TAO_Iterator_i::TAO_Iterator_i (TAO_LogRecordStore::LOG_RECORD_STORE &store,
                                CORBA::ULong start,
                                const char *constraint,
                                CORBA::ULong max_rec_list_len
                                )
  :iter_ (store),
   constraint_ (constraint),
   max_rec_list_len_ (max_rec_list_len)
{
  // Advance the iterator to the starting point.
  for (CORBA::ULong i = 0;
       i < start && iter_.advance () != -1;
       ++i)
    {
      ;
    }
}

TAO_Iterator_i::~TAO_Iterator_i (void)
{
  // Do nothing
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

DsLogAdmin::RecordList*
TAO_Iterator_i::get (CORBA::ULong position,
                 CORBA::ULong how_many
                 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidParam))
{
  how_many = how_many ? 0 : 1;

  if (how_many > this->max_rec_list_len_ - position)
    ACE_THROW_RETURN (DsLogAdmin::InvalidParam (), 0);

  CORBA::ULong i = 0;
  // move the iterator to "position"
  for (;
       i < position && iter_.advance () != -1;
       ++i)
    {
      ;
    }

  // Use an Interpreter to build an expression tree.
  TAO_Log_Constraint_Interpreter interpreter (constraint_
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Sequentially iterate over all the records and pick the ones that
  // meet the constraints.

  // Iterate over and populate the list.
  TAO_LogRecordStore::LOG_RECORD_HASH_MAP_ENTRY *hash_entry = 0;

  DsLogAdmin::RecordList* rec_list = 0;
  // Figure out the length of the list.

  // Allocate the list of <max_rec_list_len_> length.
  ACE_NEW_THROW_EX (rec_list,
                    DsLogAdmin::RecordList (this->max_rec_list_len_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  CORBA::ULong count = 0;

  CORBA::Boolean done = 0;

  for ( i = 0;
        i < how_many && count < this->max_rec_list_len_;
        ++i)
    {
      if (iter_.next (hash_entry) == -1 || iter_.advance () == -1)
        {
          done = 1;
          break;
        }

      // Use an evaluator.
      TAO_Log_Constraint_Visitor visitor (hash_entry->int_id_);

      // Does it match the constraint?
      if (interpreter.evaluate (visitor) == 1)
      {
        (*rec_list)[count] = hash_entry->int_id_;
        // copy the log record.
        count++;
      }
    }

  rec_list->length (count);

  if (done == 1)
    {
      // destroy this object..
      this->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (rec_list);
    }

  return rec_list;
}
