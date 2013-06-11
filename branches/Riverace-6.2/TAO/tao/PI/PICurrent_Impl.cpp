// $Id$

#include "tao/PI/PICurrent_Impl.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "tao/ORB_Core.h"

#if !defined (__ACE_INLINE__)
# include "tao/PI/PICurrent_Impl.inl"
#endif /* __ACE_INLINE__ */

#include "tao/TAO_Server_Request.h"
#include "ace/Log_Msg.h"
#include "tao/debug.h"
#include "ace/CORBA_macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Any *
TAO::PICurrent_Impl::get_slot (PortableInterceptor::SlotId identifier)
{
  // No need to check validity of SlotId.  It is validated before this
  // method is invoked.

  // The active slot table should never be a lazy copy of itself!
  if ( (0 != this->lazy_copy_)
      && (&this->lazy_copy_->current_slot_table () == &this->slot_table_))
  {
    if (TAO_debug_level > 0)
      TAOLIB_DEBUG ((LM_DEBUG, ACE_TEXT ("TAO (%P|%t) Lazy copy of self detected at %N,%l\n")));
    throw ::CORBA::INTERNAL ();
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
    }

  return any;
}

void
TAO::PICurrent_Impl::set_slot (PortableInterceptor::SlotId identifier,
                               const CORBA::Any & data)
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
    throw ::CORBA::INTERNAL ();

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

TAO::PICurrent_Impl::~PICurrent_Impl ()
{
  if (this->push_)
    {
      // We have YOUNGER stack members to REMOVE as well. HOWEVER we
      // don't want the entry above us coming back down and trying
      // to delete us again. (As we are currently doing just that.)
      this->push_->pop_= 0;

      delete this->push_;
    }
  else if (this->orb_core_)
    {
      // Since there are no entries above us, we must be the top of
      // the stack and since all are being deleted, the stack will
      // be empty.
      this->orb_core_->set_tss_resource (this->tss_slot_, 0);
    }

  // Break any existing ties that another PICurrent has with our table
  // since our table will no longer exists once this destructor completes.
  if (0 != this->impending_change_callback_)
    this->impending_change_callback_->convert_from_lazy_to_real_copy ();

  // If we have logically copied another table, ensure it is told about our
  // demise so that it will not call our non-existant
  // convert_from_lazy_to_real_copy() when it changes/destructs.
  if (0 != this->lazy_copy_)
    this->lazy_copy_->set_callback_for_impending_change (0);

  if (this->pop_)
    {
      // We have OLDER stack members to REMOVE as well. HOWEVER we
      // don't want multiple adjustments of the stack head pointer from
      // every older entry as they delete, as this requires multiple
      // set_tss_resource updates which would be slow and unnecessary.
      this->pop_->orb_core_= 0;

      // We also don't want double deletions of what used to be above these
      // since we have just completed the deletion of these ourselves.
      this->pop_->push_= 0;
      delete this->pop_;
    }
}

void
TAO::PICurrent_Impl::push (void)
{
  if (this->orb_core_) // We have a stack to adjust
    {
      TAO::PICurrent_Impl *const currentHead =
        static_cast<TAO::PICurrent_Impl *> (
          this->orb_core_->get_tss_resource (this->tss_slot_));
      if (!currentHead->push_)
        {
          // Since there is nothing younger above us, we need to create
          // a new entry.
          ACE_NEW_THROW_EX (currentHead->push_,
                            PICurrent_Impl (this->orb_core_, this->tss_slot_, currentHead),
                            CORBA::NO_MEMORY (
                              CORBA::SystemException::_tao_minor_code (
                                0,
                                ENOMEM),
                              CORBA::COMPLETED_NO));
        }
      this->orb_core_->set_tss_resource (this->tss_slot_, currentHead->push_);
    }
  else
    throw ::CORBA::INTERNAL (); // Should only call push if we have a stack
}

void
TAO::PICurrent_Impl::pop (void)
{
  if (this->orb_core_) // We have a stack to adjust
    {
      TAO::PICurrent_Impl *const currentHead =
        static_cast<TAO::PICurrent_Impl *> (
          this->orb_core_->get_tss_resource (this->tss_slot_));
      if (currentHead->pop_)
        orb_core_->set_tss_resource (tss_slot_, currentHead->pop_);
      else
        throw ::CORBA::INTERNAL (); // Too many pop's
    }
  else
    throw ::CORBA::INTERNAL (); // Should only call push if we have a stack
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
