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

ACE_INLINE CORBA_Boolean
operator<< (TAO_OutputCDR& cdr, CORBA::ULong x)
{
  return operator<< (cdr, x);
}

ACE_INLINE CORBA_Boolean
operator<< (TAO_OutputCDR& cdr, const CORBA::Any &x)
{
  TAO_TRY
    {
      CORBA::TypeCode::traverse_status status =
        TAO_MARSHAL_ANY::instance ()->encode (0, 
                                              &x, 
                                              0, 
                                              &cdr, 
                                              TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (status== CORBA::TypeCode::TRAVERSE_CONTINUE)
        return 1;
      // else return 0 at the end of the function
    }
  TAO_CATCH (CORBA_Exception, ex)
    {
      return 0;
    }
  TAO_ENDTRY;
  return 0;
}

ACE_INLINE CORBA_Boolean
operator<< (TAO_OutputCDR& cdr, const CORBA::Object *x)
{
  TAO_TRY
    {
      CORBA::TypeCode::traverse_status status =
        TAO_MARSHAL_OBJREF::instance ()->encode (0, 
                                                 &x, 
                                                 0, 
                                                 &cdr, 
                                                 TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (status == CORBA::TypeCode::TRAVERSE_CONTINUE)
        return 1;
      // else return 0 at the end of the function
    }
  TAO_CATCH (CORBA_Exception, ex)
    {
      return 0;
    }
  TAO_ENDTRY;
  return 0;
}

ACE_INLINE CORBA_Boolean
operator<< (TAO_OutputCDR& cdr, const CORBA::TypeCode *x)
{
  TAO_TRY
    {
      CORBA::TypeCode::traverse_status status =
        TAO_MARSHAL_TYPECODE::instance ()->encode (0, 
                                                   &x, 
                                                   0, 
                                                   &cdr, 
                                                   TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (status == CORBA::TypeCode::TRAVERSE_CONTINUE)
        return 1;
      // else return 0 at the end of the function
    }
  TAO_CATCH (CORBA_Exception, ex)
    {
      return 0;
    }
  TAO_ENDTRY;
  return 0;
}

// ****************************************************************

ACE_INLINE CORBA_Boolean
operator>> (TAO_InputCDR& cdr, CORBA::ULong x)
{
  return operator>> (cdr, x);
}

ACE_INLINE CORBA_Boolean
operator>> (TAO_InputCDR& cdr, CORBA::Any &x)
{
  TAO_TRY
    {
      CORBA::TypeCode::traverse_status status =
        TAO_MARSHAL_ANY::instance ()->decode (0, 
                                              &x, 
                                              0, 
                                              &cdr, 
                                              TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (status != CORBA::TypeCode::TRAVERSE_CONTINUE)
        return 0;
    }
  TAO_CATCH (CORBA_Exception, ex)
    {
      return 0;
    }
  TAO_ENDTRY;

  return 1;
}

ACE_INLINE CORBA_Boolean
operator>> (TAO_InputCDR& cdr, CORBA::Object *&x)
{
  TAO_TRY
    {
      CORBA::TypeCode::traverse_status status =
        TAO_MARSHAL_OBJREF::instance ()->decode (0, 
                                                 &x, 
                                                 0, 
                                                 &cdr, 
                                                 TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (status != CORBA::TypeCode::TRAVERSE_CONTINUE)
        return 0;
    }
  TAO_CATCH (CORBA_Exception, ex)
    {
      return 0;
    }
  TAO_ENDTRY;

  return 1;
}

ACE_INLINE CORBA_Boolean
operator>> (TAO_InputCDR& cdr, CORBA::TypeCode *&x)
{
  TAO_TRY
    {
      CORBA::TypeCode::traverse_status status =
        TAO_MARSHAL_TYPECODE::instance ()->decode (0, 
                                                   &x, 
                                                   0, 
                                                   &cdr, 
                                                   TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (status != CORBA::TypeCode::TRAVERSE_CONTINUE)
        return 0;
    }
  TAO_CATCH (CORBA_Exception, ex)
    {
      return 0;
    }
  TAO_ENDTRY;

  return 1;
}

// ***************************************************************************
// We must define this method here because it uses the "read_*" inlined
// methods of the ACE_InputCDR class
// ***************************************************************************

ACE_INLINE CORBA::TypeCode::traverse_status
TAO_OutputCDR::append (CORBA::TypeCode_ptr tc,
                       TAO_InputCDR *src,
                       CORBA::Environment &TAO_IN_ENV)
{
  TAO_Marshal_Object *mobj =
    TAO_MARSHAL_FACTORY::instance ()->make_marshal_object (tc, 
                                                           TAO_IN_ENV);
  TAO_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

  if (mobj == 0)
    return CORBA::TypeCode::TRAVERSE_STOP;

  return mobj->append (tc, src, this, TAO_IN_ENV);
}
