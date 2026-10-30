#include "tao/DynamicInterface/Context.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/CORBA_String.h"
#include "tao/SystemException.h"
#include "ace/Guard_T.h"

#include <new>

#if !defined (__ACE_INLINE__)
# include "tao/DynamicInterface/Context.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::ULong
CORBA::Context::_incr_refcount ()
{
  return ++refcount_;
}

CORBA::ULong
CORBA::Context::_decr_refcount ()
{
  CORBA::ULong const new_count = --this->refcount_;

  if (new_count == 0)
    delete this;

  return new_count;
}

char const*
CORBA::Context::context_name () const
{
  throw ::CORBA::NO_IMPLEMENT (TAO::VMCID, CORBA::COMPLETED_NO);
}

CORBA::Context_ptr
CORBA::Context::parent () const
{
  throw ::CORBA::NO_IMPLEMENT (TAO::VMCID, CORBA::COMPLETED_NO);
}

void
CORBA::Context::create_child (char const* /* child_ctx_name */,
                              CORBA::Context_out /* child_ctx */)
{
  throw ::CORBA::NO_IMPLEMENT (TAO::VMCID, CORBA::COMPLETED_NO);
}

void
CORBA::Context::set_one_value (char const* /* propname */,
                               CORBA::Any const& /* propvalue */)
{
  throw ::CORBA::NO_IMPLEMENT (TAO::VMCID, CORBA::COMPLETED_NO);
}

void
CORBA::Context::set_values (CORBA::NVList_ptr)
{
  throw ::CORBA::NO_IMPLEMENT (TAO::VMCID, CORBA::COMPLETED_NO);
}

void
CORBA::Context::delete_values (char const* /* propname */)
{
  throw ::CORBA::NO_IMPLEMENT (TAO::VMCID, CORBA::COMPLETED_NO);
}

void
CORBA::Context::get_values (char const* /* start_scope */,
                            CORBA::Flags /* op_flags */,
                            char const* /* pattern */,
                            CORBA::NVList_ptr & /* values */)
{
  throw ::CORBA::NO_IMPLEMENT (TAO::VMCID, CORBA::COMPLETED_NO);
}

CORBA::ContextList::ContextList (CORBA::ULong len, char* *ctx_list)
{
  for (CORBA::ULong i=0; i < len; i++)
    {
      this->add (ctx_list [i]);
    }
}

CORBA::ContextList::~ContextList ()
{
  for (char *ctx : this->ctx_list_)
    {
      CORBA::string_free (ctx);
    }
}

void
CORBA::ContextList::add (char *ctx)
{
  char *duplicate = CORBA::string_dup (ctx);
  try
    {
      this->ctx_list_.push_back (duplicate);
    }
  catch (std::bad_alloc const&)
    {
      CORBA::string_free (duplicate);
      throw CORBA::NO_MEMORY ();
    }
}

void
CORBA::ContextList::add_consume (char *ctx)
{
  try
    {
      this->ctx_list_.push_back (ctx);
    }
  catch (std::bad_alloc const&)
    {
      CORBA::string_free (ctx);
      throw CORBA::NO_MEMORY ();
    }
}

char *
CORBA::ContextList::item (CORBA::ULong slot)
{
  if (slot >= this->ctx_list_.size ())
    {
      throw ::CORBA::TypeCode::Bounds ();
    }

  return CORBA::string_dup (this->ctx_list_[slot]);
}

void
CORBA::ContextList::remove (CORBA::ULong)
{
  throw ::CORBA::NO_IMPLEMENT ();
}

CORBA::ContextList_ptr
CORBA::ContextList::_duplicate ()
{
  ++this->refcount_;
  return this;
}

void
CORBA::ContextList::_destroy ()
{
  CORBA::ULong const current = --this->refcount_;

  if (current == 0)
    {
      delete this;
    }
}

void
CORBA::ContextList::_incr_refcount ()
{
  ++this->refcount_;
}

void
CORBA::ContextList::_decr_refcount ()
{
  --this->refcount_;

  if (this->refcount_ != 0)
    {
      delete this;
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
