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

ACE_RCSID(tao, Exception, "$Id$")

// Static initializers.

CORBA::ExceptionList *TAO_Exceptions::system_exceptions;

// TAO specific typecode
extern CORBA::TypeCode_ptr TC_completion_status;

// ****************************************************************

CORBA_Exception::CORBA_Exception (CORBA::TypeCode_ptr tc)
  : type_ (CORBA::TypeCode::_duplicate (tc)),
    refcount_ (0)
{
  assert (this->type_ != 0);
}

CORBA_Exception::CORBA_Exception (const CORBA_Exception &src)
  : type_ (CORBA::TypeCode::_duplicate (src.type_)),
    refcount_ (0)
{
  assert (this->type_ != 0);
}

// NOTE:  It's this code, not anything defined in a subclass, which
// is responsible for releasing any storage owned by the exception.
// It can do this because it's got the typecode.

CORBA_Exception::CORBA_Exception (void)
  :  type_ (0),
     refcount_ (1)
{
}

CORBA_Exception::~CORBA_Exception (void)
{
  assert (this->refcount_ == 0);
  assert (this->type_ != 0);
}

CORBA_Exception &
CORBA_Exception::operator= (const CORBA_Exception &src)
{
  if (this->type_)
    CORBA::release (this->type_);
  this->type_ = CORBA::TypeCode::_duplicate (src.type_);
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
  return (ACE_OS::strcmp (repository_id, "IDL:omg.org/CORBA/Exception:1.0")==0);
}

CORBA::ULong
CORBA_Exception::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->refcount_lock_, 0);
  return ++this->refcount_;
}

CORBA::ULong
CORBA_Exception::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->refcount_lock_, 0);
    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;

    // release the lock before destroying the object.
  }
  
  delete this;
  return 0;
}

// Avoid zillions of not-quite-inlined copies of utilities.

CORBA_UserException::CORBA_UserException (void)
{
}

CORBA_UserException::CORBA_UserException (CORBA::TypeCode_ptr tc)
  : CORBA_Exception (tc)
{
}

CORBA_UserException::~CORBA_UserException (void)
{
}

CORBA_UserException &
CORBA_UserException::operator= (const CORBA_UserException &src)
{
  this->CORBA_Exception::operator= (src);
  return *this;
}

int
CORBA_UserException::_is_a (const char* interface_id) const
{
  return ACE_OS::strcmp (interface_id,
                         "IDL:omg.org/CORBA/UserException:1.0") == 0
    || CORBA_Exception::_is_a (interface_id);
}

CORBA_UserException*
CORBA_UserException::_narrow (CORBA_Exception* exception)
{
  if (exception->_is_a ("IDL:omg.org/CORBA/UserException:1.0"))
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
CORBA_SystemException::operator= (const CORBA_SystemException &src)
{
  this->CORBA_Exception::operator= (src);

  this->minor_ = src.minor_;
  this->completed_ = src.completed_;

  return *this;
}

int
CORBA_SystemException::_is_a (const char* interface_id) const
{
  return ACE_OS::strcmp (interface_id,
                         "IDL:omg.org/CORBA/SystemException:1.0") == 0
    || CORBA_Exception::_is_a (interface_id);
}

CORBA_SystemException*
CORBA_SystemException::_narrow (CORBA_Exception* exception)
{
  if (exception->_is_a ("IDL:omg.org/CORBA/SystemException:1.0"))
    return ACE_dynamic_cast (CORBA_SystemException*,exception);
  return 0;
}

// ****************************************************************

CORBA_UnknownUserException::CORBA_UnknownUserException (void)
  : CORBA_UserException (CORBA::_tc_UnknownUserException),
    exception_ (0)
{
}

CORBA_UnknownUserException::CORBA_UnknownUserException (CORBA_Any &ex)
{
  this->exception_ = new CORBA_Any (ex);
}

CORBA_UnknownUserException::~CORBA_UnknownUserException (void)
{
  if (this->exception_ != 0)
    delete this->exception_;
}

CORBA_Any&
CORBA_UnknownUserException::exception (void)
{
  return *this->exception_;
}

int
CORBA_UnknownUserException::_is_a (const char* interface_id) const
{
  return ((ACE_OS::strcmp (interface_id,
			   "IDL:omg.org/CORBA/UnknownUserException:1.0") == 0)
	  || CORBA_UserException::_is_a (interface_id));
}

CORBA_UnknownUserException*
CORBA_UnknownUserException::_narrow (CORBA_Exception *ex)
{
  if (ex->_is_a ("IDL:omg.org/CORBA/UnknownUserException:1.0"))
    return ACE_dynamic_cast (CORBA_UnknownUserException*, ex);
  return 0;
}

void
CORBA_UnknownUserException::_raise (void)
{
  TAO_RAISE(*this);
}

// Note that "buffer" holds the (unscoped) name originally, and is
// then overwritten.

void
TAO_Exceptions::make_unknown_user_typecode (CORBA::TypeCode_ptr &tcp,
					    CORBA::Environment &env)
{
  // Create the TypeCode for the CORBA_UnknownUserException
  TAO_OutputCDR stream;

  const char* interface_id =
    "IDL:omg.org/CORBA/UnknownUserException:1.0";
  const char* name = "UnknownUserException";
  const char* field_name = "exception";

  if (stream.write_octet (TAO_ENCAP_BYTE_ORDER) != CORBA::B_TRUE
      || stream.encode (CORBA::_tc_string,
			&interface_id, 0,
			env) != CORBA::TypeCode::TRAVERSE_CONTINUE
      || stream.encode (CORBA::_tc_string,
			&name, 0,
			env) != CORBA::TypeCode::TRAVERSE_CONTINUE
      || stream.write_ulong (1L) != CORBA::B_TRUE
      || stream.encode (CORBA::_tc_string,
			&field_name, 0,
			env) != CORBA::TypeCode::TRAVERSE_CONTINUE
      || stream.encode (CORBA::_tc_TypeCode,
			&CORBA::_tc_any, 0,
			env) != CORBA::TypeCode::TRAVERSE_CONTINUE)
    {
      env.exception (new CORBA_INITIALIZE (CORBA::COMPLETED_NO));
      return;
    }

  tcp = new CORBA::TypeCode (CORBA::tk_except,
                             stream.length (),
                             stream.buffer (),
                             CORBA::B_TRUE);
}

void
TAO_Exceptions::make_standard_typecode (CORBA::TypeCode_ptr &tcp,
                                       const char *name,
                                       char *buffer,
                                       size_t buflen,
                                       CORBA::Environment &env)
{
  // This function must only be called ONCE, and with a global lock
  // held!  The <CORBA::ORB_init> method is responsible for ensuring
  // this.
  static const char *minor = "minor";
  static const char *completion = "completion";

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
                       &TC_completion_status, 0,
                       env) != CORBA::TypeCode::TRAVERSE_CONTINUE) {
    env.exception (new CORBA_INITIALIZE (CORBA::COMPLETED_NO));
    return;
  }

  // OK, we stuffed the buffer we were given (or grew a bigger one;
  // hope to avoid that during initialization).  Now build and return
  // a TypeCode, saving it away in the list of ones that the ORB will
  // always accept as part of any operation response!

  tcp = new CORBA::TypeCode (CORBA::tk_except,
                             stream.length (),
                             stream.buffer (),
                             CORBA::B_TRUE);
  
  TAO_Exceptions::system_exceptions->add (tcp);
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
    CORBA::TypeCode_ptr CORBA::_tc_ ## name;
STANDARD_EXCEPTION_LIST
#undef  TAO_SYSTEM_EXCEPTION

CORBA::TypeCode_ptr CORBA::_tc_UnknownUserException = 0;

//    static CORBA::TypeCode tc_std_ ## name (CORBA::tk_except);
//    CORBA::TypeCode_ptr CORBA::_tc_ ## name = &tc_std_ ## name;

#define POA_EXCEPTION_LIST \
  POA_EXCEPTION (AdapterAlreadyExists) \
  POA_EXCEPTION (AdapterInactive) \
  POA_EXCEPTION (AdapterNonExistent) \
  POA_EXCEPTION (InvalidPolicy) \
  POA_EXCEPTION (NoServant) \
  POA_EXCEPTION (ObjectAlreadyActive) \
  POA_EXCEPTION (ObjectNotActive) \
  POA_EXCEPTION (ServantAlreadyActive) \
  POA_EXCEPTION (ServantNotActive) \
  POA_EXCEPTION (WrongAdapter) \
  POA_EXCEPTION (WrongPolicy ) \

void
TAO_Exceptions::init (CORBA::Environment &env)
{
  // Initialize the list of system exceptions, used when unmarshaling.
  ACE_NEW (TAO_Exceptions::system_exceptions, CORBA::ExceptionList);

#define TAO_SYSTEM_EXCEPTION(name) \
  if (env.exception () == 0) \
    TAO_Exceptions::make_standard_typecode (CORBA::_tc_ ## name, #name, \
                                           (char *) tc_buf_ ## name, \
                                           sizeof tc_buf_ ## name, env);
  STANDARD_EXCEPTION_LIST
#undef  TAO_SYSTEM_EXCEPTION

  // Register POA exceptions as system exceptions
  TAO_Exceptions::system_exceptions->add (PortableServer::_tc_ForwardRequest);
  TAO_Exceptions::system_exceptions->add (PortableServer::POAManager::_tc_AdapterInactive);
  TAO_Exceptions::system_exceptions->add (PortableServer::Current::_tc_NoContext);

#define POA_EXCEPTION(name) \
  TAO_Exceptions::system_exceptions->add (PortableServer::POA::_tc_ ## name);
POA_EXCEPTION_LIST
#undef POA_EXCEPTION

  if (env.exception () == 0)
    TAO_Exceptions::make_unknown_user_typecode (CORBA::_tc_UnknownUserException,
						env);
}

CORBA_Exception*
TAO_Exceptions::create_system_exception (const char* id,
					 CORBA::Environment& env)
{
#define TAO_SYSTEM_EXCEPTION(name) \
  { \
    const char* xid = CORBA::_tc_ ## name ->id (env); \
    if (ACE_OS::strcmp (id, xid) == 0) \
      return new CORBA:: name; \
  }
  STANDARD_EXCEPTION_LIST
#undef TAO_SYSTEM_EXCEPTION
#define POA_EXCEPTION(name) \
  { \
    env.clear (); \
    const char* xid = PortableServer::POA::_tc_ ## name ->id (env); \
    if (env.exception () == 0 && ACE_OS::strcmp (id, xid) == 0) \
      return new PortableServer::POA:: name; \
  }
POA_EXCEPTION_LIST
#undef POA_EXCEPTION

  {
    env.clear ();
    const char* xid = PortableServer::_tc_ForwardRequest->id (env);
    if (env.exception () == 0 && ACE_OS::strcmp (id, xid) == 0)
      return new PortableServer::ForwardRequest;
  }
  {
    env.clear ();
    const char* xid = PortableServer::POAManager::_tc_AdapterInactive->id (env);
    if (env.exception () == 0 && ACE_OS::strcmp (id, xid) == 0)
      return new PortableServer::POAManager::AdapterInactive;
  }
  {
    env.clear ();
    const char* xid = PortableServer::Current::_tc_NoContext->id (env);
    if (env.exception () == 0 && ACE_OS::strcmp (id, xid) == 0)
      return new PortableServer::Current::NoContext;
  }

  return 0;
}

void
TAO_Exceptions::fini (void)
{
  delete TAO_Exceptions::system_exceptions;
#define TAO_SYSTEM_EXCEPTION(name) \
  CORBA::release (CORBA::_tc_ ## name); \
  CORBA::_tc_ ## name = 0;
  STANDARD_EXCEPTION_LIST
#undef TAO_SYSTEM_EXCEPTION

  delete CORBA::_tc_UnknownUserException;
}

#define TAO_SYSTEM_EXCEPTION(name) \
int \
CORBA_##name ::_is_a (const char* interface_id) const \
{ \
  return ((ACE_OS::strcmp (interface_id, "IDL:omg.org/CORBA/" #name ":1.0")==0) \
          || CORBA_SystemException::_is_a (interface_id)); \
}
STANDARD_EXCEPTION_LIST
#undef TAO_SYSTEM_EXCEPTION

#define TAO_SYSTEM_EXCEPTION(name) \
void \
CORBA_##name ::_raise (void) \
{ \
  TAO_RAISE(*this); \
}
STANDARD_EXCEPTION_LIST
#undef TAO_SYSTEM_EXCEPTION

#define TAO_SYSTEM_EXCEPTION(name) \
CORBA_##name * \
CORBA_##name ::_narrow (CORBA_Exception* exception) \
{ \
  if (exception->_is_a ("IDL:omg.org/CORBA/" #name ":1.0")) \
    return ACE_dynamic_cast (CORBA_##name *, exception); \
  return 0; \
}
STANDARD_EXCEPTION_LIST
#undef TAO_SYSTEM_EXCEPTION

#define TAO_SYSTEM_EXCEPTION(name) \
CORBA_##name :: CORBA_##name (void) \
  :  CORBA_SystemException (CORBA::_tc_ ## name, \
			    0xffff0000L, \
			    CORBA::COMPLETED_NO) \
{ \
}
STANDARD_EXCEPTION_LIST
#undef TAO_SYSTEM_EXCEPTION

#undef POA_EXCEPTION_LIST
#undef STANDARD_EXCEPTION_LIST

CORBA_ExceptionList::CORBA_ExceptionList (CORBA::ULong len,
                                          CORBA::TypeCode_ptr *tc_list)
{
  for (CORBA::ULong i=0; i < len; i++)
    this->add (tc_list [i]);
}

CORBA_ExceptionList::~CORBA_ExceptionList (void)
{
#if 1
  for (CORBA::ULong i = 0; i < this->count (); ++i)
    {
      CORBA::TypeCode_ptr *tc;
      if (this->tc_list_.get (tc, i) == -1)
	return;
      CORBA::release (*tc);
    }
#endif
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Node<CORBA::TypeCode_ptr>;
template class ACE_Unbounded_Queue<CORBA::TypeCode_ptr>;
template class ACE_Unbounded_Queue_Iterator<CORBA::TypeCode_ptr>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Node<CORBA::TypeCode_ptr>
#pragma instantiate ACE_Unbounded_Queue<CORBA::TypeCode_ptr>
#pragma instantiate ACE_Unbounded_Queue_Iterator<CORBA::TypeCode_ptr>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
