// This may look like C, but it's really -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    any.h
//
// = DESCRIPTION
//     Header file for Win32 C/C++/COM interface to CORBA's "Any" type.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems, Inc.
// 
// ============================================================================

#if !defined (TAO_ANY_H)
#define TAO_ANY_H

extern const IID IID_CORBA_Any;

class CORBA_Any : public IUnknown
  // = TITLE
  //   Class "Any" can wrap values of any type, with the assistance
  //   of a TypeCode to describe that type.
  //
  // = DESCRIPTION
  //   XXX should find a way to make its memory allocation always go
  //   within the appropriate OLE heap...
{
public:
  // = Minor codes for exceptional returns
  enum 
  {
    uninitialised_type = 0xf000,
    value_without_type,
    unsupported_operation
  };

  // = Initialization and termination operations.
  CORBA_Any (void);
  // Default constructor.

  CORBA_Any (CORBA_TypeCode_ptr	type,
	     void *value = 0,
	     CORBA_Boolean orb_owns_data = CORBA_B_FALSE);
  // Constructor.

  CORBA_Any (const CORBA_Any &a);
  // Copy constructor.

  virtual ~CORBA_Any (void);
  // Destructor.

  // = ALLOCATION
  void *operator new (size_t, const void *p);
  // Placement new.
  void *operator new (size_t s);
  // Default new.
  void operator delete (void *p);
  // Default delete

  // = NOTE: 94-9-14 has assignment operator plus many insertion,

  void replace (CORBA_TypeCode_ptr type,
		const void *value,
		CORBA_Boolean orb_owns_data,
		CORBA_Environment &env);

  CORBA_TypeCode_ptr type (void) const;
  // Return <type> of <Any>.

  void *value (void) const;
  // Return <value> of <Any>.

  // = Methods required for COM <IUnknown> support.

  ULONG __stdcall AddRef (void);
  ULONG __stdcall Release (void);
  HRESULT __stdcall QueryInterface (REFIID riid,
				    void **ppv);

  // = Conversion to/from COM Variant types: 

  CORBA_Any (const VARIANT &src);
  // copy constructor,

  CORBA_Any &operator = (const VARIANT &src);
  // assignment operator

  operator VARIANT (void);
  // cast operator.

private:
  CORBA_TypeCode_ptr _type;
  // Typecode for the <Any>.

  void *_value;
  // Value for the <Any>.

  CORBA_Boolean _orb_owns_data;
  // Flag that indicates the ORB is responsible for deleting the data.

  u_int _refcnt;
  // Reference count the <Any> to reduce copying costs.

  ACE_Thread_Mutex lock_;
  // Serialize access to the reference count.

  // = NOT PROVIDED
  CORBA_Any &operator = (const CORBA_Any &a);

  // 94-9-14 hides unsigned char insert/extract 
};

#  if defined(__ACE_INLINE__)
#    include "any.i"
#  endif

#endif /* TAO_ANY_H */
