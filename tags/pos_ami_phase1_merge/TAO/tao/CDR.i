// This may look like C, but it's really -*- C++ -*-
// $Id$

ACE_INLINE
TAO_OutputCDR::~TAO_OutputCDR (void)
{
}

// ****************************************************************

ACE_INLINE
TAO_InputCDR::TAO_InputCDR (const char *buf,
                            size_t bufsiz,
                            int byte_order,
                            TAO_ORB_Core* orb_core)
  : ACE_InputCDR (buf,
                  bufsiz,
                  byte_order),
    orb_core_ (orb_core)
{
}

ACE_INLINE
TAO_InputCDR::TAO_InputCDR (size_t bufsiz,
                            int byte_order,
                            TAO_ORB_Core* orb_core)
  : ACE_InputCDR (bufsiz,
                  byte_order),
    orb_core_ (orb_core)
{
}

ACE_INLINE
TAO_InputCDR::TAO_InputCDR (const ACE_Message_Block *data,
                            int byte_order,
                            TAO_ORB_Core* orb_core)
  : ACE_InputCDR (data,
                  byte_order),
    orb_core_ (orb_core)
{
}

ACE_INLINE
TAO_InputCDR::TAO_InputCDR (ACE_Data_Block *data,
                            int byte_order,
                            TAO_ORB_Core* orb_core)
  : ACE_InputCDR (data,
                  byte_order),
    orb_core_ (orb_core)
{
}

ACE_INLINE
TAO_InputCDR::TAO_InputCDR (const TAO_InputCDR& rhs,
                            size_t size,
                            ACE_CDR::Long offset)
  : ACE_InputCDR (rhs,
                  size,
                  offset),
    orb_core_ (rhs.orb_core_)
{
}

ACE_INLINE
TAO_InputCDR::TAO_InputCDR (const TAO_InputCDR& rhs,
                            size_t size)
  : ACE_InputCDR (rhs,
                  size),
    orb_core_ (rhs.orb_core_)
{
}

ACE_INLINE
TAO_InputCDR::TAO_InputCDR (const TAO_InputCDR& rhs)
  : ACE_InputCDR (rhs),
    orb_core_ (rhs.orb_core_)
{
}

ACE_INLINE
TAO_InputCDR::~TAO_InputCDR (void)
{
}

ACE_INLINE TAO_ORB_Core*
TAO_InputCDR::orb_core (void) const
{
  return this->orb_core_;
}

// ****************************************************************

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &os,
                                      CORBA::Short x)
{
  return ACE_static_cast(ACE_OutputCDR&,os) << x;
}

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &os,
                                      CORBA::UShort x)
{
  return ACE_static_cast(ACE_OutputCDR&,os) << x;
}

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &os,
                                      CORBA::Long x)
{
  return ACE_static_cast(ACE_OutputCDR&,os) << x;
}

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &os,
                                      CORBA::ULong x)
{
  return ACE_static_cast(ACE_OutputCDR&,os) << x;
}

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &os,
                                      CORBA::LongLong x)
{
  return ACE_static_cast(ACE_OutputCDR&,os) << x;
}

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &os,
                                      CORBA::ULongLong x)
{
  return ACE_static_cast(ACE_OutputCDR&,os) << x;
}

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR& os,
                                      CORBA::LongDouble x)
{
  return ACE_static_cast(ACE_OutputCDR&,os) << x;
}

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &os,
                                      CORBA::Float x)
{
  return ACE_static_cast(ACE_OutputCDR&,os) << x;
}

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &os,
                                      CORBA::Double x)
{
  return ACE_static_cast(ACE_OutputCDR&,os) << x;
}

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &os,
                                      const CORBA::Char* x)
{
  return ACE_static_cast(ACE_OutputCDR&,os) << x;
}

// ****************************************************************

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &is,
                                      CORBA::Short &x)
{
  return ACE_static_cast(ACE_InputCDR&,is) >> x;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &is,
                                      CORBA::UShort &x)
{
  return ACE_static_cast(ACE_InputCDR&,is) >> x;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &is,
                                      CORBA::Long &x)
{
  return ACE_static_cast(ACE_InputCDR&,is) >> x;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &is,
                                      CORBA::ULong &x)
{
  return ACE_static_cast(ACE_InputCDR&,is) >> x;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &is,
                                      CORBA::LongLong &x)
{
  return ACE_static_cast(ACE_InputCDR&,is) >> x;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &is,
                                      CORBA::ULongLong &x)
{
  return ACE_static_cast(ACE_InputCDR&,is) >> x;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &is,
                                      CORBA::LongDouble &x)
{
  return ACE_static_cast(ACE_InputCDR&,is) >> x;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &is,
                                      CORBA::Float &x)
{
  return ACE_static_cast(ACE_InputCDR&,is) >> x;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &is,
                                      CORBA::Double &x)
{
  return ACE_static_cast(ACE_InputCDR&,is) >> x;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &is,
                                      CORBA::Char* &x)
{
  return ACE_static_cast(ACE_InputCDR&,is) >> x;
}
