// -*- C++ -*-

#include "PICurrent.h"

#if TAO_HAS_INTERCEPTORS == 1

ACE_RCSID (tao,
           PICurrent,
           "$Id$")


#if !defined (__ACE_INLINE__)
# include "PICurrent.inl"
#endif /* __ACE_INLINE__ */


#include "ORB_Core.h"
#include "TAO_Server_Request.h"
#include "debug.h"


TAO_PICurrent::TAO_PICurrent (TAO_ORB_Core *orb_core)
  : orb_core_ (orb_core),
    slot_count_ (0)
{
}

TAO_PICurrent::~TAO_PICurrent (void)
{
}

CORBA::Any *
TAO_PICurrent::get_slot (PortableInterceptor::SlotId id
                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::InvalidSlot))
{
  this->check_validity (id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  TAO_PICurrent_Impl *impl = this->tsc ();

  if (impl == 0)
    {
      ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 14,
                                              CORBA::COMPLETED_NO),
                        0);
    }

  return impl->get_slot (id ACE_ENV_ARG_PARAMETER);
}

void
TAO_PICurrent::set_slot (PortableInterceptor::SlotId id,
                         const CORBA::Any & data
                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::InvalidSlot))
{
  this->check_validity (id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  TAO_PICurrent_Impl *impl = this->tsc ();

  if (impl == 0)
    {
      ACE_THROW (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 14,
                                       CORBA::COMPLETED_NO));
    }

  // -------------------------------------------
  // CLIENT SIDE STUFF
  // -------------------------------------------
  // If the TSC was logically copied to the RSC, then deep copy the
  // contents of the TSC to the RSC before modifying the RSC.  The RSC
  // should not be altered by modifications to the TSC.
  TAO_PICurrent_Impl *rsc = impl->pi_peer ();
  if (rsc != 0)
    rsc->copy (*impl, 1);  // Deep copy
  // -------------------------------------------

  impl->set_slot (id, data ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

PortableInterceptor::SlotId
TAO_PICurrent::allocate_slot_id (void)
{
  // No need to acquire a lock.  This only gets called during ORB
  // initialization.  ORB initialization is already atomic.
  return this->slot_count_++;
}

TAO_PICurrent_Impl *
TAO_PICurrent::tsc (void)
{
  TAO_ORB_Core_TSS_Resources *tss =
    this->orb_core_->get_tss_resources ();

  return &tss->pi_current_;
}

// ------------------------------------------------------------------

TAO_PICurrent_Impl::TAO_PICurrent_Impl (void)
  : pi_peer_ (0),
    slot_table_ (),
    lc_slot_table_ (0),
    dirty_ (0)
{
}

TAO_PICurrent_Impl::~TAO_PICurrent_Impl (void)
{
  size_t len = this->slot_table_.size ();
  for (size_t i = 0; i < len; ++i)
    delete (CORBA::Any *) this->slot_table_[i];
}

CORBA::Any *
TAO_PICurrent_Impl::get_slot (PortableInterceptor::SlotId id
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::InvalidSlot))
{
  // No need to check validity of SlotId.  It is validated before this
  // method is invoked.

  TAO_PICurrent_Impl::Table &table =
    this->lc_slot_table_ == 0 ? this->slot_table_ : *this->lc_slot_table_;

  CORBA::Any * any = 0;

  if (id >= table.size () || table[id] == 0)
    {
      // In accordance with the Portable Interceptor specification,
      // return an Any with a TCKind of tk_null.  A default
      // constructed Any has that TCKind.
      ACE_NEW_THROW_EX (any,
                        CORBA::Any,
                        CORBA::NO_MEMORY (
                          CORBA_SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_CHECK_RETURN (0);

      return any;
    }

  const CORBA::Any *data = (CORBA::Any *) table[id];

  ACE_NEW_THROW_EX (any,
                    CORBA::Any (*data), // Make a copy.
                    CORBA::NO_MEMORY (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  return any;
}

void
TAO_PICurrent_Impl::set_slot (PortableInterceptor::SlotId id,
                              const CORBA::Any & data
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::InvalidSlot))
{
  // No need to check validity of SlotId.  It is validated before this
  // method is invoked.

  // Copy the contents of the logically copied slot table before
  // modifying our own slot table.
  if (this->lc_slot_table_ != 0)
    {
      // Deep copy

      Table &table = *this->lc_slot_table_;

      size_t new_size = table.size ();
      this->slot_table_.size (new_size);

      for (size_t i = 0; i < new_size; ++i)
        {
          if (i == id)
            continue;  // Avoid copying data twice.

          const CORBA::Any *lc_data = (CORBA::Any *) table[i];

          CORBA::Any *any = 0;
          ACE_NEW_THROW_EX (any,
                            CORBA::Any (*lc_data), // Make a copy.
                            CORBA::NO_MEMORY (
                              CORBA_SystemException::_tao_minor_code (
                                TAO_DEFAULT_MINOR_CODE,
                                ENOMEM),
                              CORBA::COMPLETED_NO));
          ACE_CHECK;

          this->slot_table_[i] = any;
        }

      // Break all ties with the logically copied slot table.
      this->lc_slot_table_ = 0;
    }

  // If the slot table array isn't large enough, then increase its
  // size.  We're guaranteed not to exceed the number of allocated
  // slots for the reason stated above.
  size_t old_size = this->slot_table_.size ();
  size_t new_size = id + 1;
  if (id >= old_size && this->slot_table_.size (new_size) != 0)
    ACE_THROW (CORBA::INTERNAL ());

  // Initialize intermediate array elements to zero, since they
  // haven't been initialized yet.  This ensures that garbage is not
  // returned when accessing any of those elements at a later point in
  // time.
  for (size_t i = old_size; i < id; ++i)
    this->slot_table_[i] = 0;

  // Now copy the data into the slot table.
  CORBA::Any * any = 0;
  ACE_NEW_THROW_EX (any,
                    CORBA::Any (data),  // Make a copy.
                    CORBA::NO_MEMORY (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  this->slot_table_[id] = any;

  // Mark the table as being modified.
  this->dirty_ = 1;
}

void
TAO_PICurrent_Impl::copy (TAO_PICurrent_Impl &rhs, CORBA::Boolean deep_copy)
{
  if (!rhs.dirty ())
    return;  // Nothing to copy

  if (deep_copy)
    {
      size_t new_size = rhs.slot_table ().size ();

      this->slot_table_.size (new_size);

      ACE_DECLARE_NEW_CORBA_ENV;
      ACE_TRY
        {
          for (size_t i = 0; i < new_size; ++i)
            {
              this->slot_table_[i] =
                rhs.get_slot (i ACE_ENV_ARG_PARAMETER);  // Deep copy
              ACE_TRY_CHECK;
            }
        }
      ACE_CATCHANY
        {
          if (TAO_debug_level > 0)
            ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                                 "(%P|%t) Error during PICurrent "
                                 "slot table copy.");
        }
      ACE_ENDTRY;
      ACE_CHECK;

      rhs.dirty (0);

      // Break all ties with the PICurrent peer.
      rhs.pi_peer (0);
      this->lc_slot_table_ = 0;
    }
  else
    {
      this->lc_slot_table_ = &rhs.slot_table ();  // Shallow copy

      this->pi_peer_ = &rhs;
    }
}

// ------------------------------------------------------------------

TAO_PICurrent_Guard::TAO_PICurrent_Guard (TAO_ServerRequest &server_request,
                                          CORBA::Boolean tsc_to_rsc)
  : src_ (0),
    dest_ (0)
{
  // This constructor is used on the server side.

  // Retrieve the thread scope current (no TSS access incurred yet).
  TAO_PICurrent *pi_current = server_request.orb_core ()->pi_current ();

  // If the slot count is zero, then there is nothing to copy.
  // Prevent any copying (and hence TSS accesses) from occurring.
  if (pi_current->slot_count () != 0)
    {
      // Retrieve the request scope current.
      TAO_PICurrent_Impl *rsc = &server_request.rs_pi_current ();

      // Retrieve the thread scope current.
      TAO_PICurrent_Impl *tsc = pi_current->tsc ();

      if (tsc_to_rsc)
        {
          // TSC to RSC copy.
          // Occurs after receive_request() interception point and
          // upcall.
          this->src_  = tsc;
          this->dest_ = rsc;
        }
      else
        {
          // RSC to TSC copy.
          // Occurs after receive_request_service_contexts()
          // interception point.
          this->src_  = rsc;
          this->dest_ = tsc;
        }
    }
}

TAO_PICurrent_Guard::~TAO_PICurrent_Guard (void)
{
  if (this->src_ != 0 && this->dest_ != 0)
    {
      // This copy better be exception-safe!
      this->dest_->copy (*this->src_, 0);    // Logical copy
    }
}

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
