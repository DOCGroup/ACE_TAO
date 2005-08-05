#include "PICurrent_Impl.h"

#if TAO_HAS_INTERCEPTORS == 1

ACE_RCSID (tao,
           PICurrent,
           "$Id$")


#if !defined (__ACE_INLINE__)
# include "PICurrent_Impl.inl"
#endif /* __ACE_INLINE__ */

#include "TAO_Server_Request.h"
#include "PICurrent_Copy_Callback.h"
#include "SystemException.h"
#include "ace/Log_Msg.h"

TAO::PICurrent_Impl::PICurrent_Impl (void)
  : slot_table_ (),
    lc_slot_table_ (0),
    copy_callback_ (0),
    destruction_callback_ (0)
{
}

TAO::PICurrent_Impl::~PICurrent_Impl (void)
{
  // Break any existing ties with PICurrent to which our table was
  // logically copied since our table no longer exists once this
  // destructor completes.
  if (this->destruction_callback_ != 0)
    this->destruction_callback_->execute_destruction_callback (0);
}


CORBA::Any *
TAO::PICurrent_Impl::get_slot (PortableInterceptor::SlotId identifier
                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::InvalidSlot))
{
  // No need to check validity of SlotId.  It is validated before this
  // method is invoked.

  PICurrent_Impl::Table & table = this->current_slot_table ();

  ACE_ASSERT (this->lc_slot_table_ != &this->slot_table_);

  CORBA::Any * any = 0;

  if (identifier < table.size ())
    {
      ACE_NEW_THROW_EX (any,
                        CORBA::Any (table[identifier]), // Make a copy.
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            0,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_CHECK_RETURN (any);
    }
  else
    {
      // In accordance with the Portable Interceptor specification,
      // return an Any with a TCKind of tk_null.  A default
      // constructed Any has that TCKind.
      ACE_NEW_THROW_EX (any,
                        CORBA::Any,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            0,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_CHECK_RETURN (any);
    }

  return any;
}

void
TAO::PICurrent_Impl::set_slot (PortableInterceptor::SlotId identifier,
                               const CORBA::Any & data
                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::InvalidSlot))
{
  // No need to check validity of SlotId.  It is validated before this
  // method is invoked.

  // Perform deep copy of the logically copied slot table, if
  // necessary, before modifying our own slot table.
  if (this->copy_callback_ != 0
      && this->copy_callback_->execute () != 0)
    ACE_THROW (CORBA::INTERNAL ());

  // If the slot table array isn't large enough, then increase its
  // size.  We're guaranteed not to exceed the number of allocated
  // slots for the reason stated above.
  if (identifier >= this->slot_table_.size ()
      && this->slot_table_.size (identifier + 1) != 0)
    ACE_THROW (CORBA::INTERNAL ());

  this->slot_table_[identifier] = CORBA::Any (data);
}

void
TAO::PICurrent_Impl::execute_destruction_callback (
  TAO::PICurrent_Impl::Table * old_lc_slot_table)
{
  this->lc_slot_table_ = old_lc_slot_table;
}

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
