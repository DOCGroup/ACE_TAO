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

// factory method
//
// Based on the kind of the typecode, return the appropriate marshal object
ACE_INLINE TAO_Marshal_Object * 
TAO_Marshal_Factory::make_marshal_object (CORBA_TypeCode_ptr tc,
                                         CORBA_Environment &env)
{
  if (tc && tc->_kind >= 0 && tc->_kind < TC_KIND_COUNT)
    return this->mobj_table_[tc->_kind].obj_;
  else
    {
      env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
      return 0;
    }
}

// the deep_copy methods

// deep_copy for any
ACE_INLINE CORBA_TypeCode::traverse_status
TAO_Marshal_Any::deep_copy (CORBA_TypeCode_ptr,
                            const void *source,
			    const void *dest,
			    CORBA_Environment &)
{
  (void) new (dest) CORBA_Any (*(CORBA_Any *) source);
  return CORBA_TypeCode::TRAVERSE_CONTINUE;
}

// deep_copy for TypeCode
ACE_INLINE CORBA_TypeCode::traverse_status
TAO_Marshal_TypeCode::deep_copy (CORBA_TypeCode_ptr,
				 const void *source,
				 const void *dest,
				 CORBA_Environment &)
{
  if ((*(CORBA_TypeCode_ptr *) source) != 0) 
    dest = source;
  else
    dest = _tc_CORBA_Null;

  ((CORBA_TypeCode_ptr) dest)->AddRef ();
  return CORBA_TypeCode::TRAVERSE_CONTINUE;
}

// deep_copy for ObjRef
ACE_INLINE CORBA_TypeCode::traverse_status
TAO_Marshal_ObjRef::deep_copy (CORBA_TypeCode_ptr,
			       const void *source,
			       const void *dest,
			       CORBA_Environment &)
{
  *(CORBA_Object_ptr *) dest = CORBA_Object::_duplicate (*(CORBA_Object_ptr *)
							 source); 
  return CORBA_TypeCode::TRAVERSE_CONTINUE;
}

// deep_copy for string
ACE_INLINE CORBA_TypeCode::traverse_status
TAO_Marshal_String::deep_copy (CORBA_TypeCode_ptr,
			       const void *source,
			       const void *dest,
			       CORBA_Environment &)
{
  *(CORBA_String *) dest = CORBA_string_copy (*(CORBA_String *) source);
  return CORBA_TypeCode::TRAVERSE_CONTINUE;
}

// deep_copy for wstring
ACE_INLINE CORBA_TypeCode::traverse_status
TAO_Marshal_WString::deep_copy (CORBA_TypeCode_ptr,
				const void *source,
				const void *dest,
				CORBA_Environment &)
{
  *(CORBA_WString *) dest = CORBA_wstring_copy (*(CORBA_WString *) source);
  return CORBA_TypeCode::TRAVERSE_CONTINUE;
}

// *************** deep_free methods ******************

// deep_free for Any
ACE_INLINE CORBA_TypeCode::traverse_status
TAO_Marshal_Any::deep_free (CORBA_TypeCode_ptr,
			    const void *source,
			    const void *,
			    CORBA_Environment &)
{
 ((CORBA_Any *) source)->~CORBA_Any ();
  return CORBA_TypeCode::TRAVERSE_CONTINUE;
}

// deep_free for TypeCode
ACE_INLINE CORBA_TypeCode::traverse_status
TAO_Marshal_TypeCode::deep_free (CORBA_TypeCode_ptr,
                                 const void *source,
                                 const void *,
                                 CORBA_Environment &)
{
  if ((*(CORBA_TypeCode_ptr *) source) != 0) 
    (*(CORBA_TypeCode_ptr *) source)->Release ();
  return CORBA_TypeCode::TRAVERSE_CONTINUE;
}

// deep_free for Principal
ACE_INLINE CORBA_TypeCode::traverse_status
TAO_Marshal_Principal::deep_free (CORBA_TypeCode_ptr,
				  const void *value,
				  const void *,
				  CORBA_Environment &)
{
  CORBA_release (*(CORBA_Principal_ptr *) value);
  return CORBA_TypeCode::TRAVERSE_CONTINUE;
}

// deep_free for ObjRef
ACE_INLINE CORBA_TypeCode::traverse_status
TAO_Marshal_ObjRef::deep_free (CORBA_TypeCode_ptr,
			       const void *source,
			       const void *,
			       CORBA_Environment &)
{
  CORBA_release (*(CORBA_Object_ptr *) source);
  return CORBA_TypeCode::TRAVERSE_CONTINUE;
}

// deep_free for string 
ACE_INLINE CORBA_TypeCode::traverse_status
TAO_Marshal_String::deep_free (CORBA_TypeCode_ptr,
			       const void *source,
			       const void *,
			       CORBA_Environment &)
{
  CORBA_string_free (*(CORBA_String *) source);
  return CORBA_TypeCode::TRAVERSE_CONTINUE;
}

// deep_free for wstring 
ACE_INLINE CORBA_TypeCode::traverse_status
TAO_Marshal_WString::deep_free (CORBA_TypeCode_ptr,
				const void *source,
				const void *,
				CORBA_Environment &)
{
  CORBA_wstring_free (*(CORBA_WString *) source);
  return CORBA_TypeCode::TRAVERSE_CONTINUE;
}

