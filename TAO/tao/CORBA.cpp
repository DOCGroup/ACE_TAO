// $Id$

#include "tao/corba.h"

// String utility support; this can need to be integrated with the
// ORB's own memory allocation subsystem.

CORBA::String
CORBA::string_copy (const CORBA::Char *str)
{
  if (!str)
    return 0;

  CORBA::String retval =
    CORBA::string_alloc (ACE_OS::strlen (str));

  // clear the contents of the allocated string
  ACE_OS::memset(retval, '\0', ACE_OS::strlen (str));

  return ACE_OS::strcpy (retval, str);
}

CORBA::String_var &
CORBA::String_var::operator= (char *p)
{
  if (this->ptr_ != p)
    {
      if (this->ptr_ != 0)
        CORBA::string_free (this->ptr_);
      this->ptr_ = p;
    }
  return *this;
}

CORBA::String_var &
CORBA::String_var::operator= (const char *p)
{
  if (this->ptr_ != 0)
    CORBA::string_free (this->ptr_);

  this->ptr_ = CORBA::string_dup (p);
  return *this;
}

CORBA::String_var &
CORBA::String_var::operator= (const CORBA::String_var& r)
{
  if (this != &r)
    {
      if (this->ptr_ != 0)
        CORBA::string_free (this->ptr_);
      this->ptr_ = CORBA::string_dup (r.ptr_);
    }
  return *this;
}

// Wide Character string utility support; this can need to be
// integrated with the ORB's own memory allocation subsystem.

CORBA::WString
CORBA::wstring_alloc (CORBA::ULong len)
{
  return new CORBA::WChar [(size_t) (len + 1)];
}

static
inline
CORBA::WChar *
wscpy (CORBA::WChar *dest,
       const CORBA::WChar *src)
{
  CORBA::WChar  *retval = dest;

  while ((*dest++ = *src++) != 0)
    continue;
  return retval;
}

CORBA::WString
CORBA::wstring_copy (const CORBA::WChar *const str)
{
  if (*str)
    return 0;

  CORBA::WString retval = CORBA::wstring_alloc (ACE_WString::wstrlen (str));
  return wscpy (retval, str);
}

void
CORBA::wstring_free (CORBA::WChar *const str)
{
  delete [] str;
}

// ORB initialisation, per OMG document 94-9-46.
//
// XXX in addition to the "built in" Internet ORB, there will be ORBs
// which are added separately, e.g. through a DLL listed in the
// registry.  Registry will be used to assign orb names and to
// establish which is the default.

CORBA::ORB_ptr
CORBA::ORB_init (int &argc,
                 char *const *argv,
                 const char * /* orb_name */,
                 CORBA::Environment &env)
{
  // Using ACE_Static_Object_Lock::instance() precludes ORB_init from
  // being called within a static object CTOR.
  ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard,
                            *ACE_Static_Object_Lock::instance (), 0));

  env.clear ();

  // @@ We need to make sure it's ok for the following 3
  // initialization routines to be called multiple times.  Or better
  // yet, ensure that we just call them the first time, e.g., by
  // putting them in some type of TAO_Object_Manager, along with the
  // Typecode_Constants...

  // Put these initializations here so that exceptions are enabled
  // immediately.
  TAO_Marshal::initialize ();
  TAO_Exceptions::init_standard_exceptions (env);
  TAO_IIOP_Interpreter::init_table ();

  if (env.exception () != 0)
    return 0;

  // Verify some of the basic implementation requirements.  This test
  // gets optimized away by a decent compiler (or else the rest of the
  // routine does).
  //
  // NOTE:  we still "just" assume that native floating point is IEEE.

  if (sizeof (CORBA::Short) != 2
      || sizeof (CORBA::Long) != 4
      || sizeof (CORBA::LongLong) != 8
      || sizeof (CORBA::Float) != 4
      || sizeof (CORBA::Double) != 8
      || sizeof (CORBA::LongDouble) != 16
      || sizeof (CORBA::WChar) < 2
      || sizeof (void *) != ACE_SIZEOF_VOID_P)
    {
      ACE_DEBUG ((LM_DEBUG, "%s; ERROR: unexpected basic type size; "
                            "s:%d l:%d ll:%d f:%d d:%d ld:%d wc:%d v:%d\n",
                  sizeof (CORBA::Short),
                  sizeof (CORBA::Long),
                  sizeof (CORBA::LongLong),
                  sizeof (CORBA::Float),
                  sizeof (CORBA::Double),
                  sizeof (CORBA::LongDouble),
                  sizeof (CORBA::WChar),
                  sizeof (void *)));

      env.exception (new CORBA::INITIALIZE (CORBA::COMPLETED_NO));
      return 0;
    }

  // Initialize the ORB Core instance.
  int result = TAO_ORB_Core_instance ()->init (argc, (char **)argv);
 
  // check for errors and return 0 if error.
  if (result == -1)
    {
      env.exception (new CORBA::BAD_PARAM (CORBA::COMPLETED_NO));
      return 0;
    }
 
  return TAO_ORB_Core_instance()->orb ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class TAO_Unbounded_Sequence<CORBA::Octet>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate TAO_Unbounded_Sequence<CORBA::Octet>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
