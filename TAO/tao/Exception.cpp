// $Id$

// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// ORB:         Exception handling support
//
// THREADING NOTE:  calling thread handles mutual exclusion policy
// on all of these data structures.

#include "tao/corba.h"

#if !defined (__ACE_INLINE__)
# include "tao/Exception.i"
#endif /* __ACE_INLINE__ */

// Static initializers.
CORBA::TypeCode_ptr
TAO_Exceptions::sys_exceptions[TAO_Exceptions::NUM_SYS_EXCEPTIONS];

CORBA::ExceptionList TAO_Exceptions::system_exceptions;

void
CORBA_Environment::exception (CORBA::Exception *ex)
{
  if (ex != this->exception_)
    {
      this->clear ();
      this->exception_ = ex;
      this->exception_->AddRef ();
    }
}

CORBA_Environment::~CORBA_Environment (void)
{
  this->clear ();
}

void
CORBA_Environment::clear (void)
{
  if (this->exception_)
    this->exception_->Release ();

  this->exception_ = 0;
}

void
CORBA_Exception::operator delete (void *p)
{
  ::operator delete (p);
}

CORBA_Environment::CORBA_Environment (void)
  : exception_ (0)
{
}

void *
CORBA_Exception::operator new (size_t s)
{
  return ::operator new (s);
}

CORBA_Exception::CORBA_Exception (CORBA::TypeCode_ptr tc)
  : type_ (tc),
    refcount_ (0)
{
  if (this->type_)
    this->type_->AddRef ();
  assert (this->type_ != 0);
}

CORBA_Exception::CORBA_Exception (const CORBA_Exception &src)
  : type_ (src.type_),
    refcount_ (0)
{
  if (this->type_)
    this->type_->AddRef ();
  assert (this->type_ != 0);
}

// NOTE:  It's this code, not anything defined in a subclass, which
// is responsible for releasing any storage owned by the exception.
// It can do this because it's got the typecode.

CORBA_Exception::~CORBA_Exception (void)
{
  assert (this->refcount_ == 0);
  assert (this->type_ != 0);

  assert (1 == 2);
}

CORBA_Exception &
CORBA_Exception::operator = (const CORBA_Exception &src)
{
  if (this->type_)
    this->type_->Release ();
  this->type_ = src.type_;
  if (this->type_)
    this->type_->AddRef ();
  assert (this->type_ != 0);

  return *this;
}

const char *
CORBA_Exception::_id (void) const
{
  CORBA::Environment env;

  if (this->type_)
    return this->type_->id (env);
  else
    return 0;
}

TAO_CONST CORBA::TypeCode_ptr
CORBA_Exception::_type (void) const
{
  return this->type_;
}

int
CORBA_Exception::_is_a (const char* repository_id) const
{
  return (ACE_OS::strcmp (repository_id, "IDL:CORBA/Exception:1.0")==0);
}

CORBA::ULong
CORBA_Exception::AddRef (void)
{
  return ++this->refcount_;
}

CORBA::ULong
CORBA_Exception::Release (void)
{
  this->refcount_--;
  if (this->refcount_ != 0)
    return this->refcount_;

  // CORBA::TypeCode_ptr                tc = type_->_duplicate ();

  CORBA::Any free_it_all (this->type_, this, CORBA::B_TRUE);

  // tc->Release ();

  return 0;
}

// Avoid zillions of not-quite-inlined copies of utilities.

CORBA_UserException::CORBA_UserException (CORBA::TypeCode_ptr tc)
  : CORBA_Exception (tc)
{
}

CORBA_UserException::~CORBA_UserException (void)
{
}

CORBA_UserException &
CORBA_UserException::operator = (const CORBA_UserException &src)
{
  if (this->type_)
    this->type_->Release ();
  this->type_ = src.type_;
  if (this->type_)
    this->type_->AddRef ();
  assert (this->type_ != 0);

  return *this;
}

int
CORBA_UserException::_is_a (const char* interface_id) const
{
  return ACE_OS::strcmp (interface_id,
                         "IDL:CORBA/UserException:1.0") == 0
    || CORBA_Exception::_is_a (interface_id);
}

CORBA_UserException*
CORBA_UserException::_narrow (CORBA_Exception* exception)
{
  if (exception->_is_a ("IDL:CORBA/UserException:1.0"))
    return ACE_dynamic_cast (CORBA_UserException *, exception);
  return 0;
}

CORBA_SystemException::CORBA_SystemException (CORBA::TypeCode_ptr tc,
                                              CORBA::ULong code,
                                              CORBA::CompletionStatus completed)
  : CORBA_Exception (tc),
    minor_ (code),
    completed_ (completed)
{
}

CORBA_SystemException::CORBA_SystemException (const CORBA_SystemException &src)
  : CORBA_Exception (src),
    minor_ (src.minor_),
    completed_ (src.completed_)
{
}

CORBA_SystemException::~CORBA_SystemException (void)
{
}

CORBA_SystemException &
CORBA_SystemException::operator = (const CORBA_SystemException &src)
{
  if (this->type_)
    this->type_->Release ();
  this->type_ = src.type_;
  if (this->type_)
    this->type_->AddRef ();

  this->minor_ = src.minor_;
  this->completed_ = src.completed_;

  assert (this->type_ != 0);

  return *this;
}

int
CORBA_SystemException::_is_a (const char* interface_id) const
{
  return ACE_OS::strcmp (interface_id,
                         "IDL:CORBA/SystemException:1.0") == 0
    || CORBA_Exception::_is_a (interface_id);
}

CORBA_SystemException*
CORBA_SystemException::_narrow (CORBA_Exception* exception)
{
  if (exception->_is_a ("IDL:CORBA/SystemException:1.0"))
    return ACE_dynamic_cast (CORBA_SystemException*,exception);
  return 0;
}

// Note that "buffer" holds the (unscoped) name originally, and is
// then overwritten.

void
TAO_Exceptions::make_standard_typecode (CORBA::TypeCode_ptr tcp,
                                       const char *name,
                                       char *buffer,
                                       size_t buflen,
                                       CORBA::Environment &env)
{
  static const char *minor = "minor";
  static const char *completion = "completion";

  static const CORBA::ULong oc_completion_status [] =
  {
    TAO_ENCAP_BYTE_ORDER, // byte order flag, tricky
    0, 0,                 // type ID omitted
    3,                    // three members
    0, 0,                 // ... whose names are all omitted
    0, 0,
    0, 0
  };
  static CORBA::TypeCode
    tc_completion_status (CORBA::tk_enum,
                          sizeof oc_completion_status,
                          (char *) &oc_completion_status,
                          CORBA::B_FALSE);

  static const CORBA::TypeCode_ptr completion_status =
    &tc_completion_status;

  // Create a CDR stream ... juggle the alignment here a bit, we know
  // it's good enough for the typecode.

  TAO_OutputCDR stream (buffer, buflen);

  // into CDR stream, stuff (in order):
  //    - byte order flag [4 bytes]
  //    - exception ID [27 + N bytes]
  //    - exception name [4 + N bytes ]
  //    - number of members (2) [4 bytes ]
  //    - foreach member, { name string, typecode } [~40 bytes]

  char full_id[100];
  char *strptr = full_id;

  (void) ACE_OS::sprintf (full_id,
                         "IDL:omg.org/CORBA/%s:1.0",
                         name);
  // @@ Should this really be an assert or should we deal with it via
  // exceptions?
  assert (ACE_OS::strlen (full_id) <= sizeof full_id);

  if (stream.write_octet (TAO_ENCAP_BYTE_ORDER) != CORBA::B_TRUE
      || stream.encode (CORBA::_tc_string,
                       &strptr, 0,
                       env) != CORBA::TypeCode::TRAVERSE_CONTINUE
      || stream.encode (CORBA::_tc_string,
                       &name, 0,
                       env) != CORBA::TypeCode::TRAVERSE_CONTINUE
      || stream.write_ulong (2L) != CORBA::B_TRUE
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

  CORBA::ULong l = TAO_Exceptions::system_exceptions.count ();
  TAO_Exceptions::system_exceptions.add (
                                         new (tcp) CORBA::TypeCode
                                         (CORBA::tk_except,
                                          stream.length (),
                                          stream.buffer (),
                                          CORBA::B_FALSE)
                                         );

  assert (tcp->length_ <= TAO_Exceptions::TC_BUFLEN);
  return;
}

// List of standard/system exceptions ... used to create static
// storage for their typecodes, then later to initialize that storage
// using the routine above. (It's just too painful to init these
// typecodes statically in all cases!)

#define STANDARD_EXCEPTION_LIST \
    TAO_SYSTEM_EXCEPTION (UNKNOWN) \
    TAO_SYSTEM_EXCEPTION (BAD_PARAM) \
    TAO_SYSTEM_EXCEPTION (NO_MEMORY) \
    TAO_SYSTEM_EXCEPTION (IMP_LIMIT) \
    TAO_SYSTEM_EXCEPTION (COMM_FAILURE) \
    TAO_SYSTEM_EXCEPTION (INV_OBJREF) \
    TAO_SYSTEM_EXCEPTION (OBJECT_NOT_EXIST) \
    TAO_SYSTEM_EXCEPTION (NO_PERMISSION) \
    TAO_SYSTEM_EXCEPTION (INTERNAL) \
    TAO_SYSTEM_EXCEPTION (MARSHAL) \
    TAO_SYSTEM_EXCEPTION (INITIALIZE) \
    TAO_SYSTEM_EXCEPTION (NO_IMPLEMENT) \
    TAO_SYSTEM_EXCEPTION (BAD_TYPECODE) \
    TAO_SYSTEM_EXCEPTION (BAD_OPERATION) \
    TAO_SYSTEM_EXCEPTION (NO_RESOURCES) \
    TAO_SYSTEM_EXCEPTION (NO_RESPONSE) \
    TAO_SYSTEM_EXCEPTION (PERSIST_STORE) \
    TAO_SYSTEM_EXCEPTION (BAD_INV_ORDER) \
    TAO_SYSTEM_EXCEPTION (TRANSIENT) \
    TAO_SYSTEM_EXCEPTION (FREE_MEM) \
    TAO_SYSTEM_EXCEPTION (INV_IDENT) \
    TAO_SYSTEM_EXCEPTION (INV_FLAG) \
    TAO_SYSTEM_EXCEPTION (INTF_REPOS) \
    TAO_SYSTEM_EXCEPTION (BAD_CONTEXT) \
    TAO_SYSTEM_EXCEPTION (OBJ_ADAPTER) \
    TAO_SYSTEM_EXCEPTION (DATA_CONVERSION)

// Declare static storage for these ... the buffer is "naturally"
// aligned and overwritten.
//
// @@ this actually doesn't guarantee "natural" alignment, but
// it works that way in most systems.

#define TAO_SYSTEM_EXCEPTION(name) \
    static CORBA::Long tc_buf_ ## name [TAO_Exceptions::TC_BUFLEN / sizeof (long)]; \
    static CORBA::TypeCode tc_std_ ## name (CORBA::tk_except); \
    CORBA::TypeCode_ptr CORBA::_tc_ ## name = &tc_std_ ## name;
STANDARD_EXCEPTION_LIST
#undef  TAO_SYSTEM_EXCEPTION

void
TAO_Exceptions::init_standard_exceptions (CORBA::Environment &env)
{
  // Initialize the list of system exceptions, used when
  // unmarshaling.

#define TAO_SYSTEM_EXCEPTION(name) \
  if (env.exception () == 0) \
    TAO_Exceptions::make_standard_typecode (&tc_std_ ## name, #name, \
                                           (char *) tc_buf_ ## name, \
                                           sizeof tc_buf_ ## name, env);
  STANDARD_EXCEPTION_LIST
#undef  TAO_SYSTEM_EXCEPTION
}

#define TAO_SYSTEM_EXCEPTION(name) \
int \
CORBA_##name ::_is_a (const char* interface_id) const \
{ \
  return ((ACE_OS::strcmp (interface_id, "IDL:CORBA/" #name "1.0")==0) \
          || CORBA_SystemException::_is_a (interface_id)); \
}
STANDARD_EXCEPTION_LIST
#undef TAO_SYSTEM_EXCEPTION

#define TAO_SYSTEM_EXCEPTION(name) \
CORBA_##name * \
CORBA_##name ::_narrow (CORBA_Exception* exception) \
{ \
  if (exception->_is_a ("IDL:CORBA/" #name "1.0")) \
    return ACE_dynamic_cast (CORBA_##name *, exception); \
  return 0; \
}
STANDARD_EXCEPTION_LIST
#undef TAO_SYSTEM_EXCEPTION

#undef  STANDARD_EXCEPTION_LIST

// Static initialization of the two user-defined exceptions that
// are part of the ORB.

static char tc_buf_Bounds [] =
{
  0, 0, 0, 0,           // big endian, padded
  0, 0, 0, 38,  // strlen (id) + 1
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
  0, 0, 0, 0            // no members to this typecode
};

static CORBA::TypeCode tc_std_Bounds (CORBA::tk_except,
                                      sizeof tc_buf_Bounds,
                                      tc_buf_Bounds,
                                      CORBA::B_FALSE);

CORBA::TypeCode_ptr CORBA::_tc_Bounds = &tc_std_Bounds;

static char tc_buf_BadKind [] =
{
  0, 0, 0, 0,           // big endian, padded
  0, 0, 0, 39,  // strlen (id) + 1
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
  0, 0, 0, 0            // no members to this typecode
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

  if (!this->exception_)
    return CORBA::NO_EXCEPTION;

  // All exceptions currently (CORBA 2.0) defined in the CORBA
  // scope are system exceptions ... except for a couple that
  // are related to TypeCodes.

  const char *id = this->exception_->_id ();

  if (ACE_OS::strncmp (id, sysex_prefix, sizeof sysex_prefix - 1) == 0
      && ACE_OS::strncmp (id + sizeof sysex_prefix - 1,
                          typecode_extra, sizeof typecode_extra - 1) != 0)
    return CORBA::SYSTEM_EXCEPTION;

  return CORBA::USER_EXCEPTION;
}

// Diagnostic utility routine: describe the exception onto the
// standard I/O stream passed as a parameter.

void
CORBA::Environment::print_exception (const char *info,
                                     FILE *) const
{
  const char *id = this->exception_->_id ();

  ACE_DEBUG ((LM_ERROR, "(%P|%t) EXCEPTION, %s\n", info));

  // @@ get rid of this logic, and rely on some member function on
  // Exception to say if it's user or system exception.

  if (ACE_OS::strncmp ((char *) id, "IDL:omg.org/CORBA/", 10) == 0
      && ACE_OS::strncmp ((char *) id, "IDL:omg.org/CORBA/TypeCode/", 19) != 0)
    {
      // @@ this should be a QueryInterface call instead.
      CORBA::SystemException *x2 =
        (CORBA::SystemException *) this->exception_;

      // @@ there are a other few "user exceptions" in the CORBA
      // scope, they're not all standard/system exceptions ... really
      // need to either compare exhaustively against all those IDs
      // (yeech) or (preferably) to represent the exception type
      // directly in the exception value so it can be queried.

      ACE_DEBUG ((LM_ERROR,
                  "(%P|%t) system exception, ID '%s'\n",
                  id));
      ACE_DEBUG ((LM_ERROR,
                  "(%P|%t) minor code = %x, completed = %s\n",
                  x2->minor (),
                  (x2->completion () == CORBA::COMPLETED_YES) ? "YES" :
                  (x2->completion () == CORBA::COMPLETED_NO) ? "NO" :
                  (x2->completion () == CORBA::COMPLETED_MAYBE) ? "MAYBE" :
                  "garbage"));
    }
  else
    // @@ we can use the exception's typecode to dump all the data
    // held within it ...

    ACE_DEBUG ((LM_ERROR,
                "(%P|%t) user exception, ID '%s'\n",
                id));
}

CORBA_ExceptionList::CORBA_ExceptionList (CORBA::ULong len,
                                          CORBA::TypeCode_ptr *tc_list)
{
  for (CORBA::ULong i=0; i < len; i++)
    this->add (tc_list [i]);
}

void
CORBA_ExceptionList::add (CORBA::TypeCode_ptr tc)
{
  this->tc_list_.enqueue_tail (CORBA::TypeCode::_duplicate (tc));
}

void
CORBA_ExceptionList::add_consume (CORBA::TypeCode_ptr tc)
{
  this->tc_list_.enqueue_tail (tc);
}

CORBA::TypeCode_ptr
CORBA_ExceptionList::item (CORBA::ULong index,
                           CORBA::Environment &env)
{
  CORBA::TypeCode_ptr *tc;
  env.clear ();
  if (this->tc_list_.get (tc, index) == -1)
    {
      env.exception (new CORBA::TypeCode::Bounds);
      return 0;
    }
  else
    {
      return CORBA::TypeCode::_duplicate (*tc);
    }
}
void
CORBA_ExceptionList::remove (CORBA::ULong index, CORBA::Environment &env)
{
  // unimplemented
  env.clear ();
}
