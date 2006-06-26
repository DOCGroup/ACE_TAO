#include "tao/PI/PICurrent_Impl.h"

#if TAO_HAS_INTERCEPTORS == 1

ACE_RCSID (tao,
           PICurrent,
           "$Id$")


#if !defined (__ACE_INLINE__)
# include "tao/PI/PICurrent_Impl.inl"
#endif /* __ACE_INLINE__ */

#include "tao/TAO_Server_Request.h"
#include "tao/SystemException.h"
#include "ace/Log_Msg.h"
#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Any *
TAO::PICurrent_Impl::get_slot (PortableInterceptor::SlotId identifier
                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::InvalidSlot))
{
  // No need to check validity of SlotId.  It is validated before this
  // method is invoked.

  // The active slot table should never be a lazy copy of itself!
  if ( (0 != this->lazy_copy_)
      && (&this->lazy_copy_->current_slot_table () == &this->slot_table_))
  {
    if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TAO (%P|%t) Lazy copy of self detected at %N,%l\n")));
    ACE_THROW (CORBA::INTERNAL ());
  }

  // Get the slot table that is currently active
  PICurrent_Impl::Table & table = this->current_slot_table ();
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

  // Break any existing ties that another PICurrent has with our table
  // since our table is changing.
  if (0 != this->impending_change_callback_)
    this->impending_change_callback_->convert_from_lazy_to_real_copy ();

  // Ensure that we have a real physical copy of the table before
  // making any changes to it.
  this->convert_from_lazy_to_real_copy ();

  // If the slot table array isn't large enough, then increase its
  // size.  We're guaranteed not to exceed the number of allocated
  // slots for the reason stated above.
  if (identifier >= this->slot_table_.size ()
      && this->slot_table_.size (identifier + 1) != 0)
    ACE_THROW (CORBA::INTERNAL ());

  this->slot_table_[identifier] = CORBA::Any (data);
}

void
TAO::PICurrent_Impl::take_lazy_copy (
  TAO::PICurrent_Impl * p)
{
  // Check that we are being told to actually change which table we are
  // copying from. (If it is the same as before OR it would ultimately be
  // the same table, we are already correctly setup and we do nothing.)
  if ( (p != this->lazy_copy_)
      && ((0 == p) || (&p->current_slot_table () != &this->current_slot_table ()))
      )
    {
      // Break any existing ties that another PICurrent has with our table
      // since our table is changing.
      if (0 != this->impending_change_callback_)
        this->impending_change_callback_->convert_from_lazy_to_real_copy ();

      // If we have previously logically copied another table, ensure it is
      // told that we are no longer interested in it so that it will not
      // call our conver_from_lazy_to_real_copy() when it changes/destructs.
      if (0 != this->lazy_copy_)
        this->lazy_copy_->set_callback_for_impending_change (0);

      // Are we being asked to copy ourself (or nothing)
      if ((0 == p) || (this == p))
        {
          this->lazy_copy_ = 0; // Use our own physical slot_table_
        }
      else
        {
          this->lazy_copy_ = p;

          // Must tell the newly copied PICurrent_Impl that we want to
          // be told when/if it is going to be changed or destroyed.
          this->lazy_copy_->set_callback_for_impending_change (this);
        }
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
