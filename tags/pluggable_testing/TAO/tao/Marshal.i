//$Id$
ACE_INLINE
TAO_Marshal_Object::TAO_Marshal_Object (void)
{
}

ACE_INLINE
TAO_Marshal_Object::~TAO_Marshal_Object (void)
{
}

ACE_INLINE
TAO_Marshal_Primitive::TAO_Marshal_Primitive (void)
{
}

ACE_INLINE
TAO_Marshal_Primitive::~TAO_Marshal_Primitive (void)
{
}

ACE_INLINE
TAO_Marshal_Any::TAO_Marshal_Any (void)
{
}

ACE_INLINE
TAO_Marshal_Any::~TAO_Marshal_Any (void)
{
}

ACE_INLINE
TAO_Marshal_Principal::TAO_Marshal_Principal (void)
{
}

ACE_INLINE
TAO_Marshal_Principal::~TAO_Marshal_Principal (void)
{
}

ACE_INLINE
TAO_Marshal_TypeCode::TAO_Marshal_TypeCode (void)
{
}

ACE_INLINE
TAO_Marshal_TypeCode::~TAO_Marshal_TypeCode (void)
{
}

ACE_INLINE
TAO_Marshal_ObjRef::TAO_Marshal_ObjRef (void)
{
}

ACE_INLINE
TAO_Marshal_ObjRef::~TAO_Marshal_ObjRef (void)
{
}

ACE_INLINE
TAO_Marshal_Struct::TAO_Marshal_Struct (void)
{
}

ACE_INLINE
TAO_Marshal_Struct::~TAO_Marshal_Struct (void)
{
}

ACE_INLINE
TAO_Marshal_Union::TAO_Marshal_Union (void)
{
}

ACE_INLINE
TAO_Marshal_Union::~TAO_Marshal_Union (void)
{
}

ACE_INLINE
TAO_Marshal_String::TAO_Marshal_String (void)
{
}

ACE_INLINE
TAO_Marshal_String::~TAO_Marshal_String (void)
{
}

ACE_INLINE
TAO_Marshal_Sequence::TAO_Marshal_Sequence (void)
{
}

ACE_INLINE
TAO_Marshal_Sequence::~TAO_Marshal_Sequence (void)
{
}

ACE_INLINE
TAO_Marshal_Array::TAO_Marshal_Array (void)
{
}

ACE_INLINE
TAO_Marshal_Array::~TAO_Marshal_Array (void)
{
}

ACE_INLINE
TAO_Marshal_Alias::TAO_Marshal_Alias (void)
{
}

ACE_INLINE
TAO_Marshal_Alias::~TAO_Marshal_Alias (void)
{
}

ACE_INLINE
TAO_Marshal_Except::TAO_Marshal_Except (void)
{
}

ACE_INLINE
TAO_Marshal_Except::~TAO_Marshal_Except (void)
{
}

ACE_INLINE
TAO_Marshal_WString::TAO_Marshal_WString (void)
{
}

ACE_INLINE
TAO_Marshal_WString::~TAO_Marshal_WString (void)
{
}

// *************** deep_free methods ******************

// deep_free for Any
ACE_INLINE CORBA::TypeCode::traverse_status
TAO_Marshal_Any::deep_free (CORBA::TypeCode_ptr,
                            const void *source,
                            const void *,
                            CORBA::Environment &)
{
  ((CORBA::Any *) source)->~CORBA_Any ();
  return CORBA::TypeCode::TRAVERSE_CONTINUE;
}

// deep_free for TypeCode
ACE_INLINE CORBA::TypeCode::traverse_status
TAO_Marshal_TypeCode::deep_free (CORBA::TypeCode_ptr,
                                 const void *source,
                                 const void *,
                                 CORBA::Environment &)
{
  CORBA::release (*(CORBA::TypeCode_ptr *) source);
  return CORBA::TypeCode::TRAVERSE_CONTINUE;
}

// deep_free for Principal
ACE_INLINE CORBA::TypeCode::traverse_status
TAO_Marshal_Principal::deep_free (CORBA::TypeCode_ptr,
                                  const void *value,
                                  const void *,
                                  CORBA::Environment &)
{
  CORBA::release (*(CORBA::Principal_ptr *) value);
  return CORBA::TypeCode::TRAVERSE_CONTINUE;
}

// deep_free for ObjRef
ACE_INLINE CORBA::TypeCode::traverse_status
TAO_Marshal_ObjRef::deep_free (CORBA::TypeCode_ptr,
                               const void *source,
                               const void *,
                               CORBA::Environment &)
{
  CORBA::Object_ptr obj = *(CORBA::Object_ptr *)source;
  CORBA::release (obj);
  return CORBA::TypeCode::TRAVERSE_CONTINUE;
}

// deep_free for string
ACE_INLINE CORBA::TypeCode::traverse_status
TAO_Marshal_String::deep_free (CORBA::TypeCode_ptr,
                               const void *source,
                               const void *,
                               CORBA::Environment &)
{
  CORBA::string_free (*(CORBA::Char* *) source);
  *(CORBA::Char* *)source = 0;
  return CORBA::TypeCode::TRAVERSE_CONTINUE;
}

// deep_free for wstring
ACE_INLINE CORBA::TypeCode::traverse_status
TAO_Marshal_WString::deep_free (CORBA::TypeCode_ptr,
                                const void *source,
                                const void *,
                                CORBA::Environment &)
{
  CORBA::wstring_free (*(CORBA::WChar* *) source);
  *(CORBA::WChar* *)source = 0;
  return CORBA::TypeCode::TRAVERSE_CONTINUE;
}
