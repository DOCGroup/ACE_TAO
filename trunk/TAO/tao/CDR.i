// This may look like C, but it's really -*- C++ -*-
// $Id$

// ****************************************************************

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
