// -*- C++ -*-
// $Id$

#include "tao/Service_Context.h"
#include "tao/CDR.h"

#if !defined (__ACE_INLINE__)
# include "tao/Service_Context.inl"
#endif /* ! __ACE_INLINE__ */

#include "ace/OS_NS_string.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_Service_Context::set_context_i (IOP::ServiceId id, TAO_OutputCDR &cdr)
{
  IOP::ServiceContext context;
  context.context_id = id;

  // Make a *copy* of the CDR stream...
  size_t const length = cdr.total_length ();
  context.context_data.length (static_cast<CORBA::ULong> (length));
  CORBA::Octet *buf = context.context_data.get_buffer ();

  for (const ACE_Message_Block *i = cdr.begin ();
       i != 0;
       i = i->cont ())
    {
      ACE_OS::memcpy (buf, i->rd_ptr (), i->length ());
      buf += i->length ();
    }

  this->set_context_i (context);
}

void
TAO_Service_Context::set_context_i (IOP::ServiceContext &context,
                                    TAO_OutputCDR &cdr)
{
  // Make a *copy* of the CDR stream...
  size_t const length = cdr.total_length ();
  context.context_data.length (static_cast<CORBA::ULong> (length));
  CORBA::Octet *buf = context.context_data.get_buffer ();

  for (const ACE_Message_Block *i = cdr.begin ();
       i != 0;
       i = i->cont ())
    {
      ACE_OS::memcpy (buf, i->rd_ptr (), i->length ());
      buf += i->length ();
    }
}

void
TAO_Service_Context::set_context (const IOP::ServiceContext &context)
{
  this->add_context_i (context);
}

int
TAO_Service_Context::set_context (const IOP::ServiceContext &context,
                                  CORBA::Boolean replace)
{
  for (CORBA::ULong i = 0; i != this->service_context_.length (); ++i)
    {
      if (context.context_id == this->service_context_[i].context_id)
        {
          if (replace)
            {
              this->service_context_[i] = context;
              return 1;
            }
          else
            {
              return 0;
            }
        }
    }

  this->add_context_i (context);
  return 1;
}

void
TAO_Service_Context::set_context_i (const IOP::ServiceContext& context)
{
  // @@ TODO Some contexts can show up multiple times, others
  //    can't find out and take appropiate action.
  for (CORBA::ULong i = 0; i != this->service_context_.length (); ++i)
    {
      if (context.context_id == this->service_context_[i].context_id)
        {
          this->service_context_[i] = context;
          return;
        }
    }

  this->add_context_i (context);
}

void
TAO_Service_Context::set_context_i (IOP::ServiceContext& context)
{
  for (CORBA::ULong i = 0; i != this->service_context_.length (); ++i)
    {
      if (context.context_id == this->service_context_[i].context_id)
        {
          CORBA::ULong const max = context.context_data.maximum ();
          CORBA::ULong const len = context.context_data.length ();
          CORBA::Octet * const buf = context.context_data.get_buffer (1);
          this->service_context_[i].context_data.replace (max, len, buf, 1);
          return;
        }
    }

  this->add_context_i (context);
}

void
TAO_Service_Context::add_context_i (IOP::ServiceContext& context)
{
  // @@ TODO Some contexts can show up multiple times, others
  //    can't find out and take appropiate action.
  CORBA::ULong const l = this->service_context_.length ();
  this->service_context_.length (l + 1);
  this->service_context_[l].context_id = context.context_id;
  CORBA::ULong const max = context.context_data.maximum ();
  CORBA::ULong const len = context.context_data.length ();
  CORBA::Octet* const buf = context.context_data.get_buffer (1);
  this->service_context_[l].context_data.replace (max, len, buf, 1);
}

void
TAO_Service_Context::add_context_i (const IOP::ServiceContext& context)
{
  // @@ TODO Some contexts can show up multiple times, others
  //    can't find out and take appropiate action.
  CORBA::ULong const l = this->service_context_.length ();
  this->service_context_.length (l + 1);
  this->service_context_[l] = context;
}

int
TAO_Service_Context::get_context (IOP::ServiceContext& context) const
{
  for (CORBA::ULong i = 0; i != this->service_context_.length (); ++i)
    {
      if (context.context_id == this->service_context_[i].context_id)
        {
          context = this->service_context_[i];
          return 1;
        }
    }

  return 0;
}

int
TAO_Service_Context::get_context (IOP::ServiceId id,
                                  const IOP::ServiceContext **context) const
{
  for (CORBA::ULong i = 0; i != this->service_context_.length (); ++i)
    {
      if (id == this->service_context_[i].context_id)
        {
          *context = &this->service_context_[i];
          return 1;
        }
    }

  return 0;
}

int
TAO_Service_Context::get_context (IOP::ServiceId id,
                                  IOP::ServiceContext_out context)
{
  CORBA::ULong const len = this->service_context_.length ();

  for (CORBA::ULong i = 0; i < len; ++i)
    {
      if (id == this->service_context_[i].context_id)
        {
          ACE_NEW_RETURN (context, IOP::ServiceContext, 0);

          *(context.ptr ()) = this->service_context_[i];

          return 1;
        }
    }

  return 0;
}

int
TAO_Service_Context::encode (TAO_OutputCDR& cdr) const
{
  return (cdr << this->service_context_);
}

int
TAO_Service_Context::decode (TAO_InputCDR& cdr)
{
  if (!(cdr >> this->service_context_))
    {
      return 0;
    }

  return 1;
}

TAO_END_VERSIONED_NAMESPACE_DECL
