#include "tao/Service_Context.h"


ACE_RCSID(tao, Service_Context, "$Id$")

#if !defined (__ACE_INLINE__)
# include "tao/Service_Context.inl"
#endif /* ! __ACE_INLINE__ */


void
TAO_Service_Context::set_context_i (IOP::ServiceId id,
                                    TAO_OutputCDR &cdr)
{
  IOP::ServiceContext context;
  context.context_id = id;

  // Make a *copy* of the CDR stream...
  CORBA::ULong length = cdr.total_length ();
  context.context_data.length (length);
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
  CORBA::ULong length = cdr.total_length ();
  context.context_data.length (length);
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

void
TAO_Service_Context::set_context (IOP::ServiceContext &context)
{
  this->add_context_i (context);
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
          CORBA::ULong max = context.context_data.maximum ();
          CORBA::ULong len = context.context_data.length ();
          CORBA::Octet* buf = context.context_data.get_buffer (1);
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
  CORBA::ULong l = this->service_context_.length ();
  this->service_context_.length (l + 1);
  this->service_context_[l].context_id = context.context_id;
  CORBA::ULong max = context.context_data.maximum ();
  CORBA::ULong len = context.context_data.length ();
  CORBA::Octet* buf = context.context_data.get_buffer (1);
  this->service_context_[l].context_data.replace (max, len, buf, 1);
}

void
TAO_Service_Context::add_context_i (const IOP::ServiceContext& context)
{
  // @@ TODO Some contexts can show up multiple times, others
  //    can't find out and take appropiate action.
  CORBA::ULong l = this->service_context_.length ();
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
TAO_Service_Context::encode (TAO_OutputCDR& cdr) const
{
  return (cdr << this->service_context_);
}

int
TAO_Service_Context::decode (TAO_InputCDR& cdr)
{
  if ((cdr >> this->service_context_) == 0)
    return 0;

  return 1;
}
