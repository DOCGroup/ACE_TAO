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

// factory method
//
// Based on the kind of the typecode, return the appropriate marshal object
ACE_INLINE TAO_Marshal_Object * 
TAO_Marshal_Factory::make_marshal_object (CORBA::TypeCode_ptr tc,
                                         CORBA::Environment &env)
{
  if (tc && tc->_kind >= 0 && tc->_kind < CORBA::TC_KIND_COUNT)
    return this->mobj_table_[tc->_kind].obj_;
  else
    {
      env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
      return 0;
    }
}

// the deep_copy methods

// deep_copy for any
ACE_INLINE CORBA::TypeCode::traverse_status
TAO_Marshal_Any::deep_copy (CORBA::TypeCode_ptr,
                            const void *source,
			    const void *dest,
			    CORBA::Environment &)
{
  (void) new (dest) CORBA::Any (*(CORBA::Any *) source);
  return CORBA::TypeCode::TRAVERSE_CONTINUE;
}

// deep_copy for TypeCode
ACE_INLINE CORBA::TypeCode::traverse_status
TAO_Marshal_TypeCode::deep_copy (CORBA::TypeCode_ptr,
				 const void *source,
				 const void *dest,
				 CORBA::Environment &)
{
  if ((*(CORBA::TypeCode_ptr *) source) != 0) 
    dest = source;
  else
    dest = CORBA::_tc_null;

  ((CORBA::TypeCode_ptr) dest)->AddRef ();
  return CORBA::TypeCode::TRAVERSE_CONTINUE;
}

// deep_copy for ObjRef
ACE_INLINE CORBA::TypeCode::traverse_status
TAO_Marshal_ObjRef::deep_copy (CORBA::TypeCode_ptr,
			       const void *source,
			       const void *dest,
			       CORBA::Environment &)
{
  *(CORBA::Object_ptr *) dest = CORBA::Object::_duplicate (*(CORBA::Object_ptr *)
							 source); 
  return CORBA::TypeCode::TRAVERSE_CONTINUE;
}

// deep_copy for string
ACE_INLINE CORBA::TypeCode::traverse_status
TAO_Marshal_String::deep_copy (CORBA::TypeCode_ptr,
			       const void *source,
			       const void *dest,
			       CORBA::Environment &)
{
  *(CORBA::String *) dest = CORBA::string_copy (*(CORBA::String *) source);
  return CORBA::TypeCode::TRAVERSE_CONTINUE;
}

// deep_copy for wstring
ACE_INLINE CORBA::TypeCode::traverse_status
TAO_Marshal_WString::deep_copy (CORBA::TypeCode_ptr,
				const void *source,
				const void *dest,
				CORBA::Environment &)
{
  *(CORBA::WString *) dest = CORBA::wstring_copy (*(CORBA::WString *) source);
  return CORBA::TypeCode::TRAVERSE_CONTINUE;
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
  if ((*(CORBA::TypeCode_ptr *) source) != 0) 
    (*(CORBA::TypeCode_ptr *) source)->Release ();
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
  CORBA::release (*(CORBA::Object_ptr *) source);
  return CORBA::TypeCode::TRAVERSE_CONTINUE;
}

// deep_free for string 
ACE_INLINE CORBA::TypeCode::traverse_status
TAO_Marshal_String::deep_free (CORBA::TypeCode_ptr,
			       const void *source,
			       const void *,
			       CORBA::Environment &)
{
  CORBA::string_free (*(CORBA::String *) source);
  return CORBA::TypeCode::TRAVERSE_CONTINUE;
}

// deep_free for wstring 
ACE_INLINE CORBA::TypeCode::traverse_status
TAO_Marshal_WString::deep_free (CORBA::TypeCode_ptr,
				const void *source,
				const void *,
				CORBA::Environment &)
{
  CORBA::wstring_free (*(CORBA::WString *) source);
  return CORBA::TypeCode::TRAVERSE_CONTINUE;
}

