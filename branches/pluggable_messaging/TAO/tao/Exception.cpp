// $Id$

// THREADING NOTE:  calling thread handles mutual exclusion policy
// on all of these data structures.

#include "ace/streams.h"
#include "tao/Exception.h"
#include "tao/Typecode.h"
#include "tao/Environment.h"
#include "tao/Any.h"
#include "tao/CDR.h"

#if defined(ACE_MVS)
#include "ace/Codeset_IBM1047.h"
#endif /* ACE_MVS */

#if !defined (__ACE_INLINE__)
# include "tao/Exception.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Exception, "$Id$")

// Static initializers.

CORBA::ExceptionList *TAO_Exceptions::system_exceptions;
ACE_Allocator *TAO_Exceptions::global_allocator_;

// TAO specific typecode
extern CORBA::TypeCode_ptr TC_completion_status;

// ****************************************************************

CORBA_Exception::CORBA_Exception (CORBA::TypeCode_ptr tc)
  : type_ (CORBA::TypeCode::_duplicate (tc)),
    refcount_ (0)
{
  ACE_ASSERT (this->type_ != 0);
}

CORBA_Exception::CORBA_Exception (const CORBA_Exception &src)
  : type_ (CORBA::TypeCode::_duplicate (src.type_)),
    refcount_ (0)
{
  ACE_ASSERT (this->type_ != 0);
}

// NOTE: It's this code, not anything defined in a subclass, which is
// responsible for releasing any storage owned by the exception.  It
// can do this because it's got the typecode.

CORBA_Exception::CORBA_Exception (void)
  :  type_ (0),
     refcount_ (0)
{
}

CORBA_Exception::~CORBA_Exception (void)
{
  ACE_ASSERT (this->refcount_ == 0);
  ACE_ASSERT (this->type_ != 0);
  CORBA::release (this->type_);
}

CORBA_Exception &
CORBA_Exception::operator= (const CORBA_Exception &src)
{
  if (this->type_)
    CORBA::release (this->type_);
  this->type_ = CORBA::TypeCode::_duplicate (src.type_);
  ACE_ASSERT (this->type_ != 0);

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

CORBA::TypeCode_ptr
CORBA_Exception::_type (void) const
{
  return this->type_;
}

int
CORBA_Exception::_is_a (const char* repository_id) const
{
  return ACE_OS::strcmp (repository_id,
                         "IDL:omg.org/CORBA/Exception:1.0") == 0;
}

void
CORBA_Exception::_tao_print_exception (const char *user_provided_info,
                                       FILE *) const
{
  ACE_DEBUG ((LM_ERROR,
              ASYS_TEXT ("(%P|%t) EXCEPTION, %s\n")
              ASYS_TEXT ("%s\n"),
              user_provided_info,
              this->_info ().c_str ()));
}

ACE_CString
CORBA_Exception::_info (void) const
{
  CORBA::SystemException *system_exception =
    CORBA_SystemException::_narrow (ACE_const_cast (CORBA_Exception *,
                                                    this));

  if (system_exception != 0)
    return system_exception->_info ();

  // @@ we can use the exception's typecode to dump all the data held
  // within it ...

  ACE_CString user_exception_info = "user exception, ID '";
  user_exception_info += this->_id ();
  user_exception_info += "'";
  return user_exception_info;
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

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)

// Convenient ostrean operator.
ostream& operator<< (ostream &os,
                     const CORBA_Exception &e)
{
  CORBA::Any tmp;
  tmp <<= e;

  CORBA::TypeCode_var tc = tmp.type ();
  const char *p = tc->name ();

  if (*p != '\0')
    {
      os << p << " (" << tc->id () << ')';
    }
  else
    {
      os << tc->id ();
    }

  return os;
}

#endif // (ACE_LACKS_IOSTREAM_TOTALLY)

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
    return ACE_dynamic_cast (CORBA_UserException *,
                             exception);
  return 0;
}

CORBA_SystemException::CORBA_SystemException (void)
{
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
    return ACE_dynamic_cast (CORBA_SystemException *,
                             exception);
  return 0;
}

void
CORBA_SystemException::_raise (void)
{
  TAO_RAISE (*this);
}

void
CORBA_SystemException::_tao_encode (TAO_OutputCDR &cdr,
                                    CORBA::Environment &ACE_TRY_ENV) const
{
  if (cdr.write_string (this->_id ())
      && cdr.write_ulong (this->minor ())
      && cdr.write_ulong (this->completed ()))
    return;
  ACE_THROW (CORBA::MARSHAL ());
}

void
CORBA_SystemException::_tao_decode (TAO_InputCDR &cdr,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  // The string is read by the caller, to determine the exact type of
  // the exception.  We just decode the fields...
  // cdr.read_string (this->id ());
  CORBA::ULong tmp;

  if (cdr.read_ulong (this->minor_)
      && cdr.read_ulong (tmp))
    {
      this->completed_ = CORBA::CompletionStatus (tmp);
      return;
    }
  ACE_THROW (CORBA::MARSHAL ());
}

CORBA::ULong
CORBA_SystemException::_tao_errno (int errno_value)
{
  switch (errno_value)
    {
    case 0:
      return TAO_UNSPECIFIED_MINOR_CODE;
    case ETIMEDOUT:
      return TAO_ETIMEDOUT_MINOR_CODE;
    case ENFILE:
      return TAO_ENFILE_MINOR_CODE;
    case EMFILE:
      return TAO_EMFILE_MINOR_CODE;
    case EPIPE:
      return TAO_EPIPE_MINOR_CODE;
    case ECONNREFUSED:
      return TAO_ECONNREFUSED_MINOR_CODE;
    case ENOENT:
      return TAO_ENOENT_MINOR_CODE;
    case EBADF:
      return TAO_EBADF_MINOR_CODE;
#if (ENOSYS != EFAULT)
    case ENOSYS:
      return TAO_ENOSYS_MINOR_CODE;
#endif /* ENOSYS != EFAULT */
    case EPERM:
      return TAO_EPERM_MINOR_CODE;
    case EAFNOSUPPORT:
      return TAO_EAFNOSUPPORT_MINOR_CODE;
    case EAGAIN:
      return TAO_EAGAIN_MINOR_CODE;
    case ENOMEM:
      return TAO_ENOMEM_MINOR_CODE;
    case EACCES:
      return TAO_EACCES_MINOR_CODE;
    case EFAULT:
      return TAO_EFAULT_MINOR_CODE;
    case EBUSY:
      return TAO_EBUSY_MINOR_CODE;
    case EEXIST:
      return TAO_EEXIST_MINOR_CODE;
    case EINVAL:
      return TAO_EINVAL_MINOR_CODE;
    case ECOMM:
      return TAO_ECOMM_MINOR_CODE;
    case ECONNRESET:
      return TAO_ECONNRESET_MINOR_CODE;
#if (ENOTSUP != ENOSYS)
    case ENOTSUP:
      return TAO_ENOTSUP_MINOR_CODE;
#endif /* ENOSYS != EFAULT */
    default:
      // Mask off bottom 7 bits and return them.
      return errno_value & 0x7F;
    }
}

CORBA::ULong
CORBA_SystemException::_tao_minor_code (u_int location,
                                        int errno_value)
{
  return
    TAO_DEFAULT_MINOR_CODE
    | location
    | _tao_errno (errno_value);
}

void
CORBA_SystemException::_tao_print_system_exception (FILE *) const
{
  ACE_DEBUG ((LM_ERROR,
              ASYS_TEXT ("(%P|%t) system exception, ID '%s'\n"),
              this->_info ().c_str ()));
}

ACE_CString
CORBA_SystemException::_info (void) const
{
  // @@ there are a other few "user exceptions" in the CORBA scope,
  // they're not all standard/system exceptions ... really need to
  // either compare exhaustively against all those IDs (yeech) or
  // (preferably) to represent the exception type directly in the
  // exception value so it can be queried.

  ACE_CString info = "system exception, ID '";
  info += this->_id ();
  info += "'\n";

  CORBA::ULong VMCID =
    this->minor () & 0xFFFFF000u;

  if (VMCID == TAO_DEFAULT_MINOR_CODE)
    {
      const char *location;
      switch (this->minor () & 0x00000F80u)
        {
        case TAO_INVOCATION_CONNECT_MINOR_CODE:
          location = "invocation connect failed";
          break;
        case TAO_INVOCATION_LOCATION_FORWARD_MINOR_CODE:
          location = "location forward failed";
          break;
        case TAO_INVOCATION_SEND_REQUEST_MINOR_CODE:
          location = "send request failed";
          break;
        case TAO_POA_DISCARDING:
          location = "poa in discarding state";
          break;
        case TAO_POA_HOLDING:
          location = "poa in holding state";
          break;
        case TAO_UNHANDLED_SERVER_CXX_EXCEPTION:
          location = "unhandled c++ exception in server side";
          break;
        case TAO_INVOCATION_RECV_REQUEST_MINOR_CODE:
          location = "failed to recv request response";
          break;
        case TAO_CONNECTOR_REGISTRY_NO_USABLE_PROTOCOL:
          location = "all protocols failed to parse the IOR";
          break;
        case TAO_NULL_POINTER_MINOR_CODE:
          location = "attempt to use null pointer";
          break;
        case TAO_MPROFILE_CREATION_ERROR:
          location = "error during MProfile creation";
          break;
        case TAO_TIMEOUT_CONNECT_MINOR_CODE:
          location = "timeout during connect";
          break;
        case TAO_TIMEOUT_SEND_MINOR_CODE:
          location = "timeout during send";
          break;
        case TAO_TIMEOUT_RECV_MINOR_CODE:
          location = "timeout during recv";
          break;
        case TAO_IMPLREPO_SERVER_MANUAL_ACTIVATION:
          location = "implrepo server specified manual startup";
          break;
        default:
          location = "unknown location";
        }

      const char *errno_indication;
      char unknown_errno [32];
      switch (this->minor () & 0x7FU)
        {
        case TAO_UNSPECIFIED_MINOR_CODE:
          errno_indication = "unspecified errno";
          break;
        case TAO_ETIMEDOUT_MINOR_CODE:
          errno_indication = "ETIMEOUT";
          break;
        case TAO_ENFILE_MINOR_CODE:
          errno_indication = "ENFILE";
          break;
        case TAO_EMFILE_MINOR_CODE:
          errno_indication = "EMFILE";
          break;
        case TAO_EPIPE_MINOR_CODE:
          errno_indication = "EPIPE";
          break;
        case TAO_ECONNREFUSED_MINOR_CODE:
          errno_indication = "ECONNREFUSED";
          break;
        case TAO_ENOENT_MINOR_CODE:
          errno_indication = "ENOENT";
          break;
        case TAO_EBADF_MINOR_CODE:
          errno_indication = "EBADF";
          break;
        case TAO_ENOSYS_MINOR_CODE:
          errno_indication = "ENOSYS";
          break;
        case TAO_EPERM_MINOR_CODE:
          errno_indication = "EPERM";
          break;
        case TAO_EAFNOSUPPORT_MINOR_CODE:
          errno_indication = "EAFNOSUPPORT";
          break;
        case TAO_EAGAIN_MINOR_CODE:
          errno_indication = "EAGAIN";
          break;
        case TAO_ENOMEM_MINOR_CODE:
          errno_indication = "ENOMEM";
          break;
        case TAO_EACCES_MINOR_CODE:
          errno_indication = "EACCES";
          break;
        case TAO_EFAULT_MINOR_CODE:
          errno_indication = "EFAULT";
          break;
        case TAO_EBUSY_MINOR_CODE:
          errno_indication = "EBUSY";
          break;
        case TAO_EEXIST_MINOR_CODE:
          errno_indication = "EEXIST";
          break;
        case TAO_EINVAL_MINOR_CODE:
          errno_indication = "EINVAL";
          break;
        case TAO_ECOMM_MINOR_CODE:
          errno_indication = "ECOMM";
          break;
        case TAO_ECONNRESET_MINOR_CODE:
          errno_indication = "ECONNRESET";
          break;
        case TAO_ENOTSUP_MINOR_CODE:
          errno_indication = "EENOTSUP";
          break;
        default:
          {
            // 7 bits of some other errno.
            ACE_OS::sprintf (unknown_errno,
                             "low 7 bits of errno: %3u",
                             this->minor () & 0x7FU);

            errno_indication = unknown_errno;
          }
        }

      char buffer[BUFSIZ];
      ACE_OS::sprintf (buffer,
                       "TAO exception, "
                       "minor code = %x (%s; %s), "
                       "completed = %s\n",
                       this->minor (),
                       location,
                       errno_indication,
                       (completed () == CORBA::COMPLETED_YES) ? "YES" :
                       (completed () == CORBA::COMPLETED_NO) ? "NO" :
                       (completed () == CORBA::COMPLETED_MAYBE) ? "MAYBE" :
                       "garbage");

      info += buffer;
    }
  else
    {
      char buffer[BUFSIZ];
      ACE_OS::sprintf (buffer,
                       "non-TAO exception, "
                       "minor code = %x, completed = %s\n",
                       this->minor (),
                       (completed () == CORBA::COMPLETED_YES) ? "YES" :
                       (completed () == CORBA::COMPLETED_NO) ? "NO" :
                       (completed () == CORBA::COMPLETED_MAYBE) ? "MAYBE" :
                       "garbage");

      info += buffer;
    }

  return info;
}

CORBA_UnknownUserException::CORBA_UnknownUserException (void)
  : CORBA_UserException (CORBA::_tc_UnknownUserException),
    exception_ (0)
{
}

CORBA_UnknownUserException::CORBA_UnknownUserException (CORBA_Any &ex)
  : CORBA_UserException (CORBA::_tc_UnknownUserException)
{
  ACE_NEW (this->exception_,
           CORBA_Any (ex));
}

CORBA_UnknownUserException::CORBA_UnknownUserException (
      const CORBA_UnknownUserException& e)
  : CORBA_UserException (e._type ())
{
  ACE_NEW (this->exception_,
           CORBA_Any (*e.exception_));
}

CORBA_UnknownUserException::~CORBA_UnknownUserException (void)
{
  delete this->exception_;
}

CORBA_Any &
CORBA_UnknownUserException::exception (void)
{
  return *this->exception_;
}

int
CORBA_UnknownUserException::_is_a (const char *interface_id) const
{
  return ((ACE_OS::strcmp (interface_id,
                           "IDL:omg.org/CORBA/UnknownUserException:1.0") == 0)
          || CORBA_UserException::_is_a (interface_id));
}

CORBA_UnknownUserException*
CORBA_UnknownUserException::_narrow (CORBA_Exception *ex)
{
  if (ex->_is_a ("IDL:omg.org/CORBA/UnknownUserException:1.0"))
    return ACE_dynamic_cast (CORBA_UnknownUserException *,
                             ex);
  return 0;
}

void
CORBA_UnknownUserException::_raise (void)
{
  TAO_RAISE (*this);
}

void
CORBA_UnknownUserException::_tao_encode (TAO_OutputCDR &,
                                         CORBA::Environment &ACE_TRY_ENV) const
{
  ACE_THROW (CORBA::MARSHAL ());
}

void
CORBA_UnknownUserException::_tao_decode (TAO_InputCDR &,
                                         CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::MARSHAL ());
}

// Note that "buffer" holds the (unscoped) name originally, and is
// then overwritten.

void
TAO_Exceptions::make_unknown_user_typecode (CORBA::TypeCode_ptr &tcp,
                                            CORBA::Environment &ACE_TRY_ENV)
{
  // Create the TypeCode for the CORBA_UnknownUserException.

#if defined(ACE_MVS)
  // @@ We need to use a translator to make sure that all TypeCodes
  // are stored in ISO8859 form, the problem is that this hack does
  // not scale as more native sets have to be supported

  ACE_IBM1047_ISO8859 translator;
  TAO_OutputCDR stream (0,
                        ACE_CDR_BYTE_ORDER,
                        TAO_Exceptions::global_allocator_,
                        TAO_Exceptions::global_allocator_,
                        ACE_DEFAULT_CDR_MEMCPY_TRADEOFF,
                        &translator);
#else
  TAO_OutputCDR stream (0,
                        ACE_CDR_BYTE_ORDER,
                        TAO_Exceptions::global_allocator_,
                        TAO_Exceptions::global_allocator_,
                        ACE_DEFAULT_CDR_MEMCPY_TRADEOFF);
#endif /* ACE_MVS */

  const char *interface_id =
    "IDL:omg.org/CORBA/UnknownUserException:1.0";
  const char *name = "UnknownUserException";
  const char *field_name = "exception";

  CORBA::Boolean result = stream.write_octet (TAO_ENCAP_BYTE_ORDER) == 0
    || stream.write_string (interface_id) == 0
    || stream.write_string (name) == 0
    || stream.write_ulong (1L) == 0
    || stream.write_string (field_name) == 0
    || stream.encode (CORBA::_tc_TypeCode,
                      &CORBA::_tc_any, 0,
                      ACE_TRY_ENV) != CORBA::TypeCode::TRAVERSE_CONTINUE;
  ACE_CHECK;

  if (result)
    ACE_THROW (CORBA_INITIALIZE ());

  ACE_NEW_THROW_EX (tcp,
                    CORBA::TypeCode (CORBA::tk_except,
                                     stream.length (),
                                     stream.buffer (),
                                     1,
                                     sizeof (CORBA_UserException)),
                    CORBA_NO_MEMORY ());
}

void
TAO_Exceptions::make_standard_typecode (CORBA::TypeCode_ptr &tcp,
                                        const char *name,
                                        char *buffer,
                                        size_t buflen,
                                        CORBA::Environment &ACE_TRY_ENV)
{
  // This function must only be called ONCE, and with a global lock
  // held!  The <CORBA::ORB_init> method is responsible for ensuring
  // this.
  static const char *minor = "minor";
  static const char *completed = "completed";

  // Create a CDR stream ... juggle the alignment here a bit, we know
  // it's good enough for the typecode.

#if defined(ACE_MVS)
  // @@ We need to use a translator to make sure that all TypeCodes
  // are stored in ISO8859 form, the problem is that this hack does
  // not scale as more native sets have to be supported

  ACE_IBM1047_ISO8859 translator;
  TAO_OutputCDR stream (buffer, buflen,
                        ACE_CDR_BYTE_ORDER,
                        TAO_Exceptions::global_allocator_,
                        TAO_Exceptions::global_allocator_,
                        ACE_DEFAULT_CDR_MEMCPY_TRADEOFF,
                        &translator);
#else
  TAO_OutputCDR stream (buffer, buflen,
                        ACE_CDR_BYTE_ORDER,
                        TAO_Exceptions::global_allocator_,
                        TAO_Exceptions::global_allocator_,
                        ACE_DEFAULT_CDR_MEMCPY_TRADEOFF);
#endif /* ACE_MVS */

  // into CDR stream, stuff (in order):
  //    - byte order flag [4 bytes]
  //    - exception ID [27 + N bytes]
  //    - exception name [4 + N bytes ]
  //    - number of members (2) [4 bytes ]
  //    - foreach member, { name string, typecode } [~40 bytes]

  const char prefix[] = "IDL:omg.org/CORBA/";
  const char suffix[] = ":1.0";
  char * full_id =
    CORBA::string_alloc (sizeof prefix
                         + ACE_OS::strlen (name)
                         + sizeof suffix);

  ACE_OS::strcpy (full_id, prefix);
  ACE_OS::strcat (full_id, name);
  ACE_OS::strcat (full_id, suffix);

  CORBA::Boolean result = stream.write_octet (TAO_ENCAP_BYTE_ORDER) == 0
    || stream.write_string (full_id) == 0
    || stream.write_string (name) == 0
    || stream.write_ulong (2L) != 1
    || stream.write_string (minor) == 0
    || stream.encode (CORBA::_tc_TypeCode,
                      &CORBA::_tc_ulong, 0,
                      ACE_TRY_ENV) != CORBA::TypeCode::TRAVERSE_CONTINUE;
  ACE_CHECK; // @@ Maybe we should transform this exception

  CORBA::string_free (full_id);  // No longer need the string

  result = result || stream.write_string (completed) == 0
    || stream.encode (CORBA::_tc_TypeCode,
                      &TC_completion_status, 0,
                      ACE_TRY_ENV) != CORBA::TypeCode::TRAVERSE_CONTINUE;
  ACE_CHECK; // @@ Maybe we should transform this exception

  if (result)
    ACE_THROW (CORBA::INITIALIZE ());

  // @@ It is possible to throw an exception at this point?
  //    What if the exception typecode has not been initialized yet?

  // OK, we stuffed the buffer we were given (or grew a bigger one;
  // hope to avoid that during initialization).  Now build and return
  // a TypeCode, saving it away in the list of ones that the ORB will
  // always accept as part of any operation response!

  ACE_NEW_THROW_EX (tcp,
                    CORBA::TypeCode (CORBA::tk_except,
                                     stream.length (),
                                     stream.buffer (),
                                     1,
                                     sizeof (CORBA_SystemException)),
                    CORBA_NO_MEMORY ());
  ACE_CHECK;

  TAO_Exceptions::system_exceptions->add (tcp);

  ACE_ASSERT (tcp->length_ <= buflen);
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
    TAO_SYSTEM_EXCEPTION (DATA_CONVERSION) \
    TAO_SYSTEM_EXCEPTION (INV_POLICY) \
    TAO_SYSTEM_EXCEPTION (REBIND) \
    TAO_SYSTEM_EXCEPTION (TIMEOUT) \
    TAO_SYSTEM_EXCEPTION (TRANSACTION_UNAVAILABLE) \
    TAO_SYSTEM_EXCEPTION (TRANSACTION_MODE) \
    TAO_SYSTEM_EXCEPTION (TRANSACTION_REQUIRED) \
    TAO_SYSTEM_EXCEPTION (TRANSACTION_ROLLEDBACK) \
    TAO_SYSTEM_EXCEPTION (INVALID_TRANSACTION)

// Declare static storage for these ... the buffer is "naturally"
// aligned and overwritten.
//
// @@ this actually doesn't guarantee "natural" alignment, but
// it works that way in most systems.

#define TAO_TC_BUF_LEN 256

#define TAO_SYSTEM_EXCEPTION(name) \
    static CORBA::Long tc_buf_##name[TAO_TC_BUF_LEN/sizeof(CORBA::Long)]; \
    TAO_NAMESPACE_TYPE(CORBA::TypeCode_ptr) \
    TAO_NAMESPACE_BEGIN (CORBA) \
    TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_##name, 0) \
    TAO_NAMESPACE_END
  STANDARD_EXCEPTION_LIST
#undef  TAO_SYSTEM_EXCEPTION
#undef TAO_TC_BUF_LEN

TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (CORBA)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_UnknownUserException, 0)
TAO_NAMESPACE_END

void
TAO_Exceptions::init (CORBA::Environment &ACE_TRY_ENV)
{
  // This routine should only be called once.
  // Initialize the start up allocator.
  ACE_NEW (TAO_Exceptions::global_allocator_,
           ACE_New_Allocator);

  // Initialize the list of system exceptions, used when unmarshaling.
  ACE_NEW (TAO_Exceptions::system_exceptions,
           CORBA::ExceptionList);

#define TAO_SYSTEM_EXCEPTION(name) \
  TAO_Exceptions::make_standard_typecode (CORBA::_tc_ ## name, \
                                          #name, \
                                          (char*)tc_buf_##name, \
                                          sizeof(tc_buf_##name), \
                                          ACE_TRY_ENV); \
  ACE_CHECK;
  STANDARD_EXCEPTION_LIST
#undef  TAO_SYSTEM_EXCEPTION

  TAO_Exceptions::make_unknown_user_typecode (CORBA::_tc_UnknownUserException,
                                              ACE_TRY_ENV);
}

CORBA_SystemException *
TAO_Exceptions::create_system_exception (const char *id,
                                         CORBA::Environment &ACE_TRY_ENV)
{
#define TAO_SYSTEM_EXCEPTION(name) \
  { \
    const char* xid = CORBA::_tc_ ## name ->id (ACE_TRY_ENV); \
    ACE_CHECK_RETURN (0); \
    if (ACE_OS::strcmp (id, xid) == 0) \
      return new CORBA:: name; \
  }
  STANDARD_EXCEPTION_LIST
#undef TAO_SYSTEM_EXCEPTION

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
  delete TAO_Exceptions::global_allocator_;
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
void \
CORBA_##name ::_raise (void) \
{ \
  TAO_RAISE (*this); \
}
STANDARD_EXCEPTION_LIST
#undef TAO_SYSTEM_EXCEPTION

#define TAO_SYSTEM_EXCEPTION(name) \
CORBA_##name :: CORBA_##name (void) \
  :  CORBA_SystemException (CORBA::_tc_ ## name, \
                            TAO_DEFAULT_MINOR_CODE, \
                            CORBA::COMPLETED_NO) \
{ \
}
STANDARD_EXCEPTION_LIST
#undef TAO_SYSTEM_EXCEPTION

#undef STANDARD_EXCEPTION_LIST

CORBA_ExceptionList::CORBA_ExceptionList (CORBA::ULong len,
                                          CORBA::TypeCode_ptr *tc_list)
  : ref_count_ (1)
{
  for (CORBA::ULong i = 0;
       i < len;
       i++)
    this->add (tc_list [i]);
}

CORBA_ExceptionList::~CORBA_ExceptionList (void)
{
  for (CORBA::ULong i = 0;
       i < this->count ();
       ++i)
    {
      CORBA::TypeCode_ptr *tc;

      if (this->tc_list_.get (tc, i) == -1)
        return;

      CORBA::release (*tc);
    }
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
CORBA_ExceptionList::item (CORBA::ULong slot,
                           CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::TypeCode_ptr *tc;

  if (this->tc_list_.get (tc,
                          slot) == -1)
    ACE_THROW_RETURN (CORBA::TypeCode::Bounds (), CORBA::TypeCode::_nil ());
  else
    return CORBA::TypeCode::_duplicate (*tc);
}

void
CORBA_ExceptionList::remove (CORBA::ULong, CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

CORBA_ExceptionList_ptr
CORBA_ExceptionList::_duplicate (void)
{
  ++this->ref_count_;
  return this;
}

void
CORBA_ExceptionList::_destroy (void)
{
  CORBA::ULong current = --this->ref_count_;

  if (current == 0)
    delete this;
}

void
CORBA_ExceptionList::_incr_refcnt (void)
{
  this->ref_count_++;
}

void
CORBA_ExceptionList::_decr_refcnt (void)
{
  this->ref_count_--;
  if (this->ref_count_ == 0)
    delete this;

}

#if defined (TAO_DONT_CATCH_DOT_DOT_DOT)
TAO_DONT_CATCH::TAO_DONT_CATCH (void)
{}
#endif /* TAO_DONT_CATCH_DOT_DOT_DOT */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Node<CORBA::TypeCode_ptr>;
template class ACE_Unbounded_Queue<CORBA::TypeCode_ptr>;
template class ACE_Unbounded_Queue_Iterator<CORBA::TypeCode_ptr>;
template class ACE_Atomic_Op<ACE_SYNCH_MUTEX, CORBA::ULong>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Node<CORBA::TypeCode_ptr>
#pragma instantiate ACE_Unbounded_Queue<CORBA::TypeCode_ptr>
#pragma instantiate ACE_Unbounded_Queue_Iterator<CORBA::TypeCode_ptr>
#pragma instantiate ACE_Atomic_Op<ACE_SYNCH_MUTEX, CORBA::ULong>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
