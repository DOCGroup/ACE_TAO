// $Id$

#include "Context.h"

ACE_RCSID(DynamicInterface, Context, "$Id$")

#include "tao/Typecode.h"
#include "tao/Environment.h"
#include "tao/NVList.h"
#include "tao/ORB.h"

#if !defined (__ACE_INLINE__)
# include "Context.inl"
#endif /* ! __ACE_INLINE__ */

CORBA_Context::CORBA_Context (void)
  : refcount_ (1)
{
}

CORBA_Context::~CORBA_Context (void)
{
}

CORBA::ULong
CORBA_Context::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    ace_mon,
                    this->refcount_lock_,
                    0);

  return refcount_++;
}

CORBA::ULong
CORBA_Context::_decr_refcnt (void)
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
CORBA_Context::context_name (ACE_ENV_SINGLE_ARG_DECL) const
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                         CORBA::COMPLETED_NO),
                    0);
}

CORBA_Context_ptr
CORBA_Context::parent (ACE_ENV_SINGLE_ARG_DECL) const
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                         CORBA::COMPLETED_NO),
                    0);
}

void
CORBA_Context::create_child (const char * /* child_ctx_name */,
                             CORBA_Context_out /* child_ctx */
                             ACE_ENV_ARG_DECL)
{
  ACE_THROW (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                  CORBA::COMPLETED_NO));
}

void
CORBA_Context::set_one_value (const char * /* propname */,
                              const CORBA_Any & /* propvalue */
                              ACE_ENV_ARG_DECL)
{
  ACE_THROW (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                  CORBA::COMPLETED_NO));
}

void
CORBA_Context::set_values (CORBA::NVList_ptr
                           ACE_ENV_ARG_DECL)
{
  ACE_THROW (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                  CORBA::COMPLETED_NO));
}

void
CORBA_Context::delete_values (const char * /* propname */
                              ACE_ENV_ARG_DECL)
{
  ACE_THROW (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                  CORBA::COMPLETED_NO));
}

void
CORBA_Context::get_values (const char * /* start_scope */,
                           CORBA::Flags /* op_flags */,
                           const char * /* pattern */,
                           CORBA::NVList_ptr & /* values */
                           ACE_ENV_ARG_DECL)
{
  ACE_THROW (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                  CORBA::COMPLETED_NO));
}

CORBA_ContextList::CORBA_ContextList (CORBA::ULong len,
                                      char* *ctx_list)
  : ref_count_ (1)
{
  for (CORBA::ULong i=0; i < len; i++)
    {
      this->add (ctx_list [i]);
    }
}

CORBA_ContextList::~CORBA_ContextList (void)
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
CORBA_ContextList::add (char *ctx)
{
  this->ctx_list_.enqueue_tail (CORBA::string_dup (ctx));
}

void
CORBA_ContextList::add_consume (char *ctx)
{
  this->ctx_list_.enqueue_tail (ctx);
}

char *
CORBA_ContextList::item (CORBA::ULong slot
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
CORBA_ContextList::remove (CORBA::ULong
                           ACE_ENV_ARG_DECL)
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

CORBA_ContextList_ptr
CORBA_ContextList::_duplicate (void)
{
  ++this->ref_count_;
  return this;
}

void
CORBA_ContextList::_destroy (void)
{
  CORBA::ULong current = --this->ref_count_;

  if (current == 0)
    {
      delete this;
    }
}

void
CORBA_ContextList::_incr_refcnt (void)
{
  this->ref_count_++;
}

void
CORBA_ContextList::_decr_refcnt (void)
{
  this->ref_count_--;

  if (this->ref_count_ != 0)
    {
      delete this;
    }
}


