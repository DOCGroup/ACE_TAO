#include "Context.h"


ACE_RCSID (DynamicInterface,
           Context,
           "$Id$")

#include "tao/Typecode.h"
#include "tao/NVList.h"


#if !defined (__ACE_INLINE__)
# include "Context.inl"
#endif /* ! __ACE_INLINE__ */


CORBA::Context::Context (void)
  : refcount_ (1)
{
}

CORBA::Context::~Context (void)
{
}

CORBA::ULong
CORBA::Context::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    ace_mon,
                    this->refcount_lock_,
                    0);

  return refcount_++;
}

CORBA::ULong
CORBA::Context::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                      ace_mon,
                      this->refcount_lock_,
                      0);

    this->refcount_--;

    if (this->refcount_ != 0)
      {
        return this->refcount_;
      }
  }

  delete this;
  return 0;
}

const char *
CORBA::Context::context_name (ACE_ENV_SINGLE_ARG_DECL) const
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                         CORBA::COMPLETED_NO),
                    0);
}

CORBA::Context_ptr
CORBA::Context::parent (ACE_ENV_SINGLE_ARG_DECL) const
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                         CORBA::COMPLETED_NO),
                    0);
}

void
CORBA::Context::create_child (const char * /* child_ctx_name */,
                              CORBA::Context_out /* child_ctx */
                              ACE_ENV_ARG_DECL)
{
  ACE_THROW (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                  CORBA::COMPLETED_NO));
}

void
CORBA::Context::set_one_value (const char * /* propname */,
                               const CORBA::Any & /* propvalue */
                               ACE_ENV_ARG_DECL)
{
  ACE_THROW (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                  CORBA::COMPLETED_NO));
}

void
CORBA::Context::set_values (CORBA::NVList_ptr
                            ACE_ENV_ARG_DECL)
{
  ACE_THROW (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                  CORBA::COMPLETED_NO));
}

void
CORBA::Context::delete_values (const char * /* propname */
                               ACE_ENV_ARG_DECL)
{
  ACE_THROW (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                  CORBA::COMPLETED_NO));
}

void
CORBA::Context::get_values (const char * /* start_scope */,
                            CORBA::Flags /* op_flags */,
                            const char * /* pattern */,
                            CORBA::NVList_ptr & /* values */
                            ACE_ENV_ARG_DECL)
{
  ACE_THROW (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                  CORBA::COMPLETED_NO));
}

CORBA::ContextList::ContextList (CORBA::ULong len,
                                 char* *ctx_list)
  : ref_count_ (1)
{
  for (CORBA::ULong i=0; i < len; i++)
    {
      this->add (ctx_list [i]);
    }
}

CORBA::ContextList::~ContextList (void)
{
  for (CORBA::ULong i = 0; i < this->count (); ++i)
    {
      char **ctx;

      if (this->ctx_list_.get (ctx, i) == -1)
        {
          return;
        }

      CORBA::string_free (*ctx);
    }
}

void
CORBA::ContextList::add (char *ctx)
{
  this->ctx_list_.enqueue_tail (CORBA::string_dup (ctx));
}

void
CORBA::ContextList::add_consume (char *ctx)
{
  this->ctx_list_.enqueue_tail (ctx);
}

char *
CORBA::ContextList::item (CORBA::ULong slot
                         ACE_ENV_ARG_DECL)
{
  char **ctx = 0;

  if (this->ctx_list_.get (ctx, slot) == -1)
    {
      ACE_THROW_RETURN (CORBA::TypeCode::Bounds (),
                        0);
    }
  else
    {
      return CORBA::string_dup (*ctx);
    }
}

void
CORBA::ContextList::remove (CORBA::ULong
                           ACE_ENV_ARG_DECL)
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

CORBA::ContextList_ptr
CORBA::ContextList::_duplicate (void)
{
  ++this->ref_count_;
  return this;
}

void
CORBA::ContextList::_destroy (void)
{
  CORBA::ULong current = --this->ref_count_;

  if (current == 0)
    {
      delete this;
    }
}

void
CORBA::ContextList::_incr_refcnt (void)
{
  this->ref_count_++;
}

void
CORBA::ContextList::_decr_refcnt (void)
{
  this->ref_count_--;

  if (this->ref_count_ != 0)
    {
      delete this;
    }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

  template class TAO_Pseudo_Var_T<CORBA::Context>;
  template class TAO_Pseudo_Out_T<CORBA::Context, CORBA::Context_var>;
  template class TAO_Pseudo_Var_T<CORBA::ContextList>;
  template class TAO_Pseudo_Out_T<CORBA::ContextList, CORBA::ContextList_var>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

# pragma instantiate TAO_Pseudo_Var_T<CORBA::Context>
# pragma instantiate TAO_Pseudo_Out_T<CORBA::Context, CORBA::Context_var>
# pragma instantiate TAO_Pseudo_Var_T<CORBA::ContextList>
# pragma instantiate TAO_Pseudo_Out_T<CORBA::ContextList, CORBA::ContextList_var>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

