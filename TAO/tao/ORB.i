// -*- c++ -*-
// $Id$

// String utility support.  Since these are static methods we need to
// explicitly export them from the DLL.

ACE_INLINE TAO_Export CORBA::String
CORBA::string_alloc (CORBA::ULong len)
{
  // Allocate 1 + strlen to accomodate the null terminating character.
  return new CORBA::Char[size_t (len + 1)];
}

ACE_INLINE TAO_Export void
CORBA::string_free (CORBA::Char *str)
{
  delete [] str;
}

// ----------------------------------------------------------------------
// String_var type
// ----------------------------------------------------------------------

ACE_INLINE
CORBA::String_var::String_var (void)
{
  this->ptr_ = 0;
}

ACE_INLINE
CORBA::String_var::String_var (const char *p)
  : ptr_ (CORBA::string_dup ((char *) p))
{
}

ACE_INLINE CORBA::Char &
CORBA::String_var::operator[] (CORBA::ULong index)
{
  // We need to verify bounds else raise some exception.
  return this->ptr_[index];
}

ACE_INLINE CORBA::Char
CORBA::String_var::operator[] (CORBA::ULong index) const
{
  // We need to verify bounds else raise some exception.
  return this->ptr_[index];
}

ACE_INLINE
CORBA::String_var::operator char *()
{
  return this->ptr_;
}

ACE_INLINE
CORBA::String_var::operator const char *() const
{
  return this->ptr_;
}

ACE_INLINE const char *
CORBA::String_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE char *&
CORBA::String_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE char *&
CORBA::String_var::out (void)
{
  CORBA::string_free (this->ptr_);
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE char *
CORBA::String_var::_retn (void)
{
  char *temp = this->ptr_;
  this->ptr_ = 0;
  return temp;
}

// ----------------------------------------------------
//  String_out type
// ----------------------------------------------------

ACE_INLINE
CORBA::String_out::String_out (char *&s)
  : ptr_ (s)
{
  this->ptr_ = 0;
}

ACE_INLINE
CORBA::String_out::String_out (CORBA::String_var &s)
  : ptr_ (s.out ())
{
}

ACE_INLINE
CORBA::String_out::String_out (CORBA::String_out &s)
  : ptr_ (s.ptr_)
{
}

ACE_INLINE CORBA::String_out &
CORBA::String_out::operator= (CORBA::String_out &s)
{
  this->ptr_ = s.ptr_;
  return *this;
}

ACE_INLINE CORBA::String_out &
CORBA::String_out::operator= (char *s)
{
  this->ptr_ = s;
  return *this;
}

ACE_INLINE CORBA::String_out &
CORBA::String_out::operator= (const char *s)
{
  this->ptr_ = CORBA::string_dup (s);
  return *this;
}

ACE_INLINE
CORBA::String_out::operator char *&()
{
  return this->ptr_;
}

ACE_INLINE char *&
CORBA::String_out::ptr (void)
{
  return this->ptr_;
}

// ----------------------------------------------------------------------
// Wide strings
// ----------------------------------------------------------------------

ACE_INLINE TAO_Export CORBA::WString
CORBA::wstring_dup (const WChar *const str)
{
  return CORBA::wstring_copy (str);
}

// CORBA dup/release build on top of COM's (why not).

ACE_INLINE void
CORBA::release (CORBA::ORB_ptr obj)
{
  if (obj)
    obj->Release ();
}

// ---------------------------------------------------------------------------
//  ORB specific
// ---------------------------------------------------------------------------

ACE_INLINE ULONG
CORBA_ORB::AddRef (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, lock_, 0));

  return refcount_++;
}

ACE_INLINE CORBA::ORB_ptr
CORBA_ORB::_duplicate (CORBA::ORB_ptr obj)
{
  if (obj)
    obj->AddRef ();
  return obj;
}

// Null pointers represent nil objects.

ACE_INLINE CORBA::ORB_ptr
CORBA_ORB::_nil (void)
{
  return 0;
}

ACE_INLINE CORBA::Boolean
CORBA::is_nil (CORBA::ORB_ptr obj)
{
  return (CORBA::Boolean) (obj == 0);
}

ACE_INLINE CORBA::Boolean
CORBA_ORB::work_pending (void)
{
  // There's ALWAYS work to do ;-)
  return CORBA::B_TRUE;
}

ACE_INLINE CORBA::Boolean
CORBA_ORB::orb_free_resources (void)
{
  return !CORBA_ORB::orb_init_count_;
}

// *************************************************************
// Inline operations for class CORBA_ORB_var
// *************************************************************

ACE_INLINE
CORBA_ORB_var::CORBA_ORB_var (void) // default constructor
  : ptr_ (CORBA_ORB::_nil ())
{
}

ACE_INLINE
CORBA_ORB_var::CORBA_ORB_var (CORBA::ORB_ptr p)
        : ptr_ (p)
{
}

ACE_INLINE CORBA::ORB_ptr
CORBA_ORB_var::ptr (void) const
{
  return this->ptr_;
}

ACE_INLINE
CORBA_ORB_var::CORBA_ORB_var (const CORBA_ORB_var &p) // copy constructor
  : ptr_ (CORBA_ORB::_duplicate (p.ptr ()))
{
}

ACE_INLINE
CORBA_ORB_var::~CORBA_ORB_var (void) // destructor
{
  CORBA::release (this->ptr_);
}

ACE_INLINE CORBA_ORB_var &
CORBA_ORB_var::operator= (CORBA::ORB_ptr p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

ACE_INLINE CORBA_ORB_var &
CORBA_ORB_var::operator= (const CORBA_ORB_var &p)
{
  if (this != &p)
    {
      CORBA::release (this->ptr_);
      this->ptr_ = CORBA_ORB::_duplicate (p.ptr ());
    }
  return *this;
}

ACE_INLINE
CORBA_ORB_var::operator const CORBA::ORB_ptr &() const // cast
{
  return this->ptr_;
}

ACE_INLINE
CORBA_ORB_var::operator CORBA::ORB_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE CORBA::ORB_ptr
CORBA_ORB_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA::ORB_ptr
CORBA_ORB_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE CORBA::ORB_ptr &
CORBA_ORB_var::inout (void)
{
  return this->ptr_;
}

ACE_INLINE CORBA::ORB_ptr &
CORBA_ORB_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA_ORB::_nil ();
  return this->ptr_;
}

ACE_INLINE CORBA::ORB_ptr
CORBA_ORB_var::_retn (void)
{
  // yield ownership of managed obj reference
  CORBA::ORB_ptr val = this->ptr_;
  this->ptr_ = CORBA_ORB::_nil ();
  return val;
}

// *************************************************************
// Inline operations for class CORBA_ORB_out
// *************************************************************

ACE_INLINE
CORBA_ORB_out::CORBA_ORB_out (CORBA::ORB_ptr &p)
        : ptr_ (p)
{
  this->ptr_ = CORBA_ORB::_nil ();
}

ACE_INLINE
CORBA_ORB_out::CORBA_ORB_out (CORBA_ORB_var &p) // constructor from _var
        : ptr_ (p.out ())
{
  CORBA::release (this->ptr_);
  this->ptr_ = CORBA_ORB::_nil ();
}

ACE_INLINE
CORBA_ORB_out::CORBA_ORB_out (CORBA_ORB_out &p) // copy constructor
        : ptr_ (p.ptr_)
{}

ACE_INLINE CORBA_ORB_out &
CORBA_ORB_out::operator= (CORBA_ORB_out &p)
{
  this->ptr_ = p.ptr_;
  return *this;
}

ACE_INLINE CORBA_ORB_out &
CORBA_ORB_out::operator= (const CORBA_ORB_var &p)
{
  this->ptr_ = CORBA_ORB::_duplicate (p.ptr ());
  return *this;
}

ACE_INLINE CORBA_ORB_out &
CORBA_ORB_out::operator= (CORBA::ORB_ptr p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
CORBA_ORB_out::operator CORBA::ORB_ptr &() // cast
{
  return this->ptr_;
}

ACE_INLINE CORBA::ORB_ptr &
CORBA_ORB_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE CORBA::ORB_ptr
CORBA_ORB_out::operator-> (void)
{
  return this->ptr_;
}
