//
// Header file for Win32 C/C++/COM interface to CORBA's "Any" type.
//
// Class "Any" can wrap values of any type, with the assistance
// of a TypeCode to describe that type.
//
// XXX should find a way to make its memory allocation always go
// within the appropriate OLE heap...
//
typedef CORBA_Any		*CORBA_Any_ptr;

extern const IID	IID_CORBA_Any;

class CORBA_Any : public IUnknown
{
public:
  // minor codes for exceptional returns
  enum {
    uninitialised_type = 0xf000,
    value_without_type,
    unsupported_operation
  };

  CORBA_Any ();
  CORBA_Any (
	     CORBA_TypeCode_ptr	type,
	     void			*value = 0,
	     CORBA_Boolean		orb_owns_data
	     = CORBA_B_FALSE
	     );
  CORBA_Any (const CORBA_Any &a);
  virtual ~CORBA_Any ();

  void			*operator new (size_t, const void *p)
  { return (void *) p; }
  void			*operator new (size_t s)
  { return ::operator new (s); }
  void			operator delete (void *p)
  { ::operator delete (p); }

  //
  // NOTE:  94-9-14 has assignment operator plus many insertion,
  // extraction operators, various from_xx and to_xx helper classes.
  //

  void			replace (
				 CORBA_TypeCode_ptr	type,
				 const void		*value,
				 CORBA_Boolean	orb_owns_data,
				 CORBA_Environment	&env
				 );

  CORBA_TypeCode_ptr		type () const;
  void			*value () const;

  //
  // Stuff required for COM IUnknown support
  //
  ULONG __stdcall		AddRef ();
  ULONG __stdcall		Release ();
  HRESULT __stdcall           QueryInterface (
					      REFIID	riid,
					      void	**ppv
					      );

  //
  // Conversion to/from COM Variant types:  copy constructor,
  // assignment operator, cast.
  //
  CORBA_Any (const VARIANT &src);
  CORBA_Any			&operator = (const VARIANT &src);
  operator VARIANT ();
    
private:
  CORBA_TypeCode_ptr		_type;
  void			*_value;
  CORBA_Boolean		_orb_owns_data;

  ACE_Thread_Mutex lock_;
  unsigned			_refcnt;

  // NOT PROVIDED
  CORBA_Any			&operator = (const CORBA_Any &a);

  //
  // 94-9-14 hides unsigned char insert/extract
  //
};
