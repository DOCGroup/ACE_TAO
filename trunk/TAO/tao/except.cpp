// @(#)except.cpp	1.11 95/11/04
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// ORB:		Exception handling support
//
// THREADING NOTE:  calling thread handles mutual exclusion policy
// on all of these data structures.

#if 0
#include "ace/OS.h"    // WARNING! This MUST come before objbase.h on WIN32!
#include <objbase.h>
#include <initguid.h>

#include "ace/Log_Msg.h"

#include "tao/orb.h"
#include "tao/cdr.h"
#endif

#include "tao/corba.h"

#if defined (HAVE_WIDEC_H)
#  include <widec.h>
#endif /* HAVE_WIDEC_H */

// {77420082-F276-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_CORBA_Exception,
0x77420082, 0xf276, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);

// {77420083-F276-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_CORBA_UserException,
0x77420083, 0xf276, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);

// {77420084-F276-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_CORBA_SystemException,
0x77420084, 0xf276, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);


CORBA_Exception::CORBA_Exception (CORBA::TypeCode_ptr tc)
  : _type (tc),
    refcount_ (1)
{
  if (_type)
    _type->AddRef ();
  assert (_type != 0);
  assert (refcount_ > 0);
}

CORBA_Exception::CORBA_Exception (const CORBA_Exception	&src)
  : _type (src._type),
    refcount_ (1)
{
  if (_type)
    _type->AddRef ();
  assert (_type != 0);
  assert (refcount_ > 0);
}

// NOTE:  It's this code, not anything defined in a subclass, which
// is responsible for releasing any storage owned by the exception.
// It can do this because it's got the typecode.

CORBA_Exception::~CORBA_Exception (void)
{
  assert (refcount_ == 0);
  assert (_type != 0);

  assert (1 == 2);
}

CORBA_Exception &
CORBA_Exception::operator = (const CORBA_Exception &src)
{
  if (_type)
    _type->Release ();
  _type = src._type;
  if (_type)
    _type->AddRef ();
  assert (_type != 0);
  assert (refcount_ > 0);

  return *this;
}

TAO_CONST CORBA::String
CORBA_Exception::id (void) const
{
  CORBA::Environment env;

  assert (refcount_ > 0);
  if (_type)
    return _type->id (env);
  else
    return 0;
}

TAO_CONST CORBA::TypeCode_ptr
CORBA_Exception::type (void) const
{
  assert (refcount_ > 0);
  return _type;
}

// For COM -- IUnKnown operations

ULONG __stdcall
CORBA_Exception::AddRef (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, lock_, 0));

  assert (refcount_ > 0);
  return ++refcount_;
}

ULONG __stdcall
CORBA_Exception::Release (void)
{
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, lock_, 0));

    assert (refcount_ > 0);
    refcount_--;
    if (refcount_ != 0)
      return refcount_;

  }

  // CORBA::TypeCode_ptr		tc = _type->_duplicate ();

  CORBA::Any free_it_all (_type, this, CORBA::B_TRUE);

  // tc->Release ();

  return 0;
}

HRESULT __stdcall
CORBA_Exception::QueryInterface (REFIID	riid,
				 void **ppv)
{
  assert (refcount_ > 0);
  *ppv = 0;

  if (IID_CORBA_Exception == riid || IID_IUnknown == riid)
    *ppv = this;

  // XXX this approach needs modifying to enable returning
  // UserException, SystemException, and other kinds of pointers.

  if (*ppv == 0)
    return ResultFromScode (E_NOINTERFACE);

 (void) AddRef ();
  return NOERROR;
}

// Avoid zillions of not-quite-inlined copies of utilities.

CORBA_UserException::CORBA_UserException (CORBA::TypeCode_ptr tc)
  : CORBA_Exception (tc)
{
}

CORBA_UserException::~CORBA_UserException (void)
{
}

CORBA_SystemException::CORBA_SystemException (CORBA::TypeCode_ptr tc,
					      CORBA::ULong code,
					      CORBA::CompletionStatus completed)
  : CORBA_Exception (tc),
    _minor (code),
    _completed (completed)
{
}

CORBA_SystemException::~CORBA_SystemException (void)
{
}

#define	NUM_SYS_EXCEPTIONS 26		// update correctly!
#define	TC_BUFLEN 160		// preallocated tc buffer

static CORBA::TypeCode_ptr sys_exceptions [NUM_SYS_EXCEPTIONS];
CORBA::ExceptionList __system_exceptions;

// Make the TypeCode for a standard exception ... note that "buffer"
// holds the (unscoped) name originally, and is then overwritten.
//
// When used correctly, initializing system exceptions is only an
// exercise in CPU time; it allocates no new memory.

static void
make_standard_typecode (CORBA::TypeCode_ptr tcp,
			const char *name,
			unsigned char *buffer,
			size_t buflen,
			CORBA::Environment &env)
{
  static const char *minor = "minor";
  static const char *completion = "completion";

  static const unsigned long oc_completion_status [] =
  {
    1,		// byte order flag, tricky
    0, 0,		// type ID omitted
    3,		// three members
    0, 0,		// ... whose names are all omitted
    0, 0,
    0, 0
  };
  static CORBA::TypeCode
    tc_completion_status (CORBA::tk_enum,
			  sizeof oc_completion_status,
 (unsigned char *) &oc_completion_status,
			  CORBA::B_FALSE);

  static const CORBA::TypeCode_ptr completion_status = &tc_completion_status;

  // Create a CDR stream ... juggle the alignment here a bit, we
  // know it's good enough for tye typecode.

  CDR stream (0, buflen);

  stream.next = stream.buffer = buffer;

  // into CDR stream, stuff (in order):
  //	- byte order flag [4 bytes]
  //	- exception ID [27 + N bytes]
  //	- exception name [4 + N bytes ]
  //	- number of members (2) [4 bytes ]
  //	- foreach member, { name string, typecode } [~40 bytes]

  char	full_id [100], *strptr = (char *) &full_id;

 (void) ACE_OS::sprintf (full_id, "IDL:omg.org/CORBA/%s:1.0", name);
  assert (strlen (full_id) <= sizeof full_id);

  if (stream.put_byte (MY_BYTE_SEX) != CORBA::B_TRUE
      || stream.encode (CORBA::_tc_string,
		       &strptr, 0,
		       env) != CORBA::TypeCode::TRAVERSE_CONTINUE
      || stream.encode (CORBA::_tc_string,
		       &name, 0,
		       env) != CORBA::TypeCode::TRAVERSE_CONTINUE
      || stream.put_ulong (2L) != CORBA::B_TRUE
      || stream.encode (CORBA::_tc_string,
		       &minor, 0,
		       env) != CORBA::TypeCode::TRAVERSE_CONTINUE
      || stream.encode (CORBA::_tc_TypeCode,
		       &CORBA::_tc_ulong, 0,
		       env) != CORBA::TypeCode::TRAVERSE_CONTINUE
      || stream.encode (CORBA::_tc_string,
		       &completion, 0,
		       env) != CORBA::TypeCode::TRAVERSE_CONTINUE
      || stream.encode (CORBA::_tc_TypeCode,
		       &completion_status, 0,
		       env) != CORBA::TypeCode::TRAVERSE_CONTINUE) {
    env.exception (new CORBA_INITIALIZE (CORBA::COMPLETED_NO));
    return;
  }

  // OK, we stuffed the buffer we were given (or grew a bigger one;
  // hope to avoid that during initialization).  Now build and return
  // a TypeCode, saving it away in the list of ones that the ORB will
  // always accept as part of any operation response!

  sys_exceptions [__system_exceptions.length++]
    = new (tcp) CORBA::TypeCode (CORBA::tk_except,
				stream.next - stream.buffer,
				stream.buffer, CORBA::B_FALSE);

  assert (tcp->_length <= TC_BUFLEN);
  return;
}

// List of standard/system exceptions ... used to create static
// storage for their typecodes, then later to initialize that storage
// using the routine above. (It's just too painful to init these
// typecodes statically in all cases!)

#define	STANDARD_EXCEPTION_LIST \
    SYSEX (UNKNOWN) \
    SYSEX (BAD_PARAM) \
    SYSEX (NO_MEMORY) \
    SYSEX (IMP_LIMIT) \
    SYSEX (COMM_FAILURE) \
    SYSEX (INV_OBJREF) \
    SYSEX (OBJECT_NOT_EXIST) \
    SYSEX (NO_PERMISSION) \
    SYSEX (INTERNAL) \
    SYSEX (MARSHAL) \
    SYSEX (INITIALIZE) \
    SYSEX (NO_IMPLEMENT) \
    SYSEX (BAD_TYPECODE) \
    SYSEX (BAD_OPERATION) \
    SYSEX (NO_RESOURCES) \
    SYSEX (NO_RESPONSE) \
    SYSEX (PERSIST_STORE) \
    SYSEX (BAD_INV_ORDER) \
    SYSEX (TRANSIENT) \
    SYSEX (FREE_MEM) \
    SYSEX (INV_IDENT) \
    SYSEX (INV_FLAG) \
    SYSEX (INTF_REPOS) \
    SYSEX (BAD_CONTEXT) \
    SYSEX (OBJ_ADAPTER) \
    SYSEX (DATA_CONVERSION)

// Declare static storage for these ... the buffer is "naturally"
// aligned and overwritten.
//
// XXX this actually doesn't guarantee "natural" alignment, but
// it works that way in most systems.

#define	SYSEX(name) \
    static long tc_buf_ ## name [TC_BUFLEN / sizeof (long)]; \
    static CORBA::TypeCode tc_std_ ## name (CORBA::tk_except); \
    CORBA::TypeCode_ptr CORBA::_tc_ ## name = &tc_std_ ## name;
STANDARD_EXCEPTION_LIST
#undef	SYSEX

// Runtime initialization of all standard exception typecodes.  Called
// from CORBA::ORB::init ().

void
__TC_init_standard_exceptions (CORBA::Environment &env)
{
  // Initialize the list of system exceptions, used when
  // unmarshaling.
  __system_exceptions.length = 0;
  __system_exceptions.maximum = NUM_SYS_EXCEPTIONS;
  __system_exceptions.buffer = &sys_exceptions [0];

  // Initialize the typecodes.
#define	SYSEX(name) \
  if (env.exception () == 0) \
			       make_standard_typecode (&tc_std_ ## name, #name, \
 (unsigned char *) tc_buf_ ## name, \
						       sizeof tc_buf_ ## name, env);

  STANDARD_EXCEPTION_LIST
#undef	SYSEX
    }

#undef	STANDARD_EXCEPTION_LIST

// Static initialization of the two user-defined exceptions that
// are part of the ORB.

static CORBA::Octet tc_buf_Bounds [] =
{
  0, 0, 0, 0,		// big endian, padded
  0, 0, 0, 38,	// strlen (id) + 1
  'I', 'D', 'L', ':',
  'o', 'm', 'g', '.',
  'o', 'r', 'g', '/',
  'C', 'O', 'R', 'B',
  'A', '/', 'T', 'y',
  'p', 'e', 'C', 'o',
  'd', 'e', '/', 'B',
  'o', 'u', 'n', 'd',
  's', ':', '1', '.',
  '0', '\0', 0, 0,
  0, 0, 0, 0		// no members to this typecode
};

static CORBA::TypeCode tc_std_Bounds (CORBA::tk_except,
				     sizeof tc_buf_Bounds,
				     tc_buf_Bounds,
				     CORBA::B_FALSE);

CORBA::TypeCode_ptr CORBA::_tc_Bounds = &tc_std_Bounds;

static CORBA::Octet tc_buf_BadKind [] =
{
  0, 0, 0, 0,		// big endian, padded
  0, 0, 0, 39,	// strlen (id) + 1
  'I', 'D', 'L', ':',
  'o', 'm', 'g', '.',
  'o', 'r', 'g', '/',
  'C', 'O', 'R', 'B',
  'A', '/', 'T', 'y',
  'p', 'e', 'C', 'o',
  'd', 'e', '/', 'B',
  'a', 'd', 'K', 'i',
  'n', 'd', ':', '1',
  '.', '0', '\0', 0,
  0, 0, 0, 0		// no members to this typecode
};

static CORBA::TypeCode tc_std_BadKind (CORBA::tk_except,
				      sizeof tc_buf_BadKind,
				      tc_buf_BadKind,
				      CORBA::B_FALSE);
CORBA::TypeCode_ptr CORBA::_tc_BadKind = &tc_std_BadKind;

// Convenience -- say if the exception is a system exception or not.

CORBA::ExceptionType
CORBA::Environment::exception_type (void) const
{
  static char sysex_prefix [] = "IDL:omg.org/CORBA/";
  static char typecode_extra [] = "TypeCode/";

  if (!_exception)
    return CORBA::NO_EXCEPTION;

  // All exceptions currently (CORBA 2.0) defined in the CORBA
  // scope are system exceptions ... except for a couple that
  // are related to TypeCodes.

  char *id = _exception->id ();

  if (ACE_OS::strncmp (id, sysex_prefix, sizeof sysex_prefix - 1) == 0
      && ACE_OS::strncmp (id + sizeof sysex_prefix - 1,
			  typecode_extra, sizeof typecode_extra - 1) != 0)
    return CORBA::SYSTEM_EXCEPTION;

  return CORBA::USER_EXCEPTION;
}

// Diagnostic utility routine:  describe the exception onto
// the standard I/O stream passed as a parameter.
//
void
CORBA::Environment::print_exception (const char *info,
                                     FILE *) const
{
  CORBA::String id = this->_exception->id ();

  ACE_DEBUG ((LM_ERROR, "(%P|%t) EXCEPTION, %s\n", info));

  // XXX get rid of this logic, and rely on some member function
  // on Exception to say if it's user or system exception.

  if (ACE_OS::strncmp ((char *) id, "IDL:omg.org/CORBA/", 10) == 0
      && ACE_OS::strncmp ((char *) id, "IDL:omg.org/CORBA/TypeCode/", 19) != 0)
    {
      // XXX this should be a QueryInterface call instead
      CORBA::SystemException *x2 = (CORBA::SystemException *) this->_exception;

      // XXX there are a other few "user exceptions" in the CORBA
      // scope, they're not all standard/system exceptions ... really
      // need to either compare exhaustively against all those IDs
      // (yeech) or (preferably) to represent the exception type
      // directly in the exception value so it can be queried.

      ACE_DEBUG ((LM_ERROR, "(%P|%t) system exception, ID '%s'\n", id));
      ACE_DEBUG ((LM_ERROR,
		  "(%P|%t) minor code = %#lx, completed = %s",
		  x2->minor (),
                  (x2->completion () == CORBA::COMPLETED_YES) ? "YES" :
                  (x2->completion () == CORBA::COMPLETED_NO) ? "NO" :
                  (x2->completion () == CORBA::COMPLETED_MAYBE) ? "MAYBE" :
                  "garbage"));
    }
  else
    // XXX we can use the exception's typecode to dump all the data
    // held within it ...

    ACE_DEBUG ((LM_ERROR, "(%P|%t) user exception, ID '%s'\n", id));
}
