// This may look like C, but it's really -*- C++ -*-
// $Id$

// ****************************************************************

ACE_INLINE CORBA::TypeCode::traverse_status
TAO_OutputCDR::encode (CORBA::TypeCode_ptr tc,
                       const void *data,
                       const void *data2,
                       CORBA::Environment &TAO_IN_ENV)
{
  TAO_Marshal_Object *mobj =
    TAO_MARSHAL_FACTORY::instance ()->make_marshal_object (tc, 
                                                           TAO_IN_ENV);
  TAO_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

  if (!mobj)
    return CORBA::TypeCode::TRAVERSE_STOP;

  return mobj->encode (tc, data, data2, this, TAO_IN_ENV);
}

// ****************************************************************

ACE_INLINE CORBA::TypeCode::traverse_status
TAO_InputCDR::decode (CORBA::TypeCode_ptr tc,
                      const void *data,
                      const void *data2,
                      CORBA::Environment &TAO_IN_ENV)
{
  TAO_Marshal_Object *mobj =
    TAO_MARSHAL_FACTORY::instance ()->make_marshal_object (tc, 
                                                           TAO_IN_ENV);
  TAO_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

  if (!mobj)
    return CORBA::TypeCode::TRAVERSE_STOP;

  return mobj->decode (tc, data, data2, this, TAO_IN_ENV);
}

ACE_INLINE CORBA::TypeCode::traverse_status
TAO_InputCDR::skip (CORBA::TypeCode_ptr tc,
                    CORBA::Environment &TAO_IN_ENV)
{
  TAO_Marshal_Object *mobj =
    TAO_MARSHAL_FACTORY::instance ()->make_marshal_object (tc, 
                                                           TAO_IN_ENV);
  TAO_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

  if (mobj == 0)
    return CORBA::TypeCode::TRAVERSE_STOP;

  return mobj->skip (tc, this, TAO_IN_ENV);
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
