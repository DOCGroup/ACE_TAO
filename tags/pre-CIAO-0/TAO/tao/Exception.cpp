// $Id$


// THREADING NOTE:  calling thread handles mutual exclusion policy
// on all of these data structures.

#include "ace/streams.h"
#include "ace/Dynamic_Service.h"
#include "tao/Exception.h"
#include "tao/Typecode.h"
#include "tao/Environment.h"
#include "tao/Any.h"
#include "tao/CDR.h"
#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "tao/Dynamic_Adapter.h"

#if defined(ACE_MVS)
#include "ace/Codeset_IBM1047.h"
#endif /* ACE_MVS */

#if !defined (__ACE_INLINE__)
# include "tao/Exception.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (TAO,
           Exception,
           "$Id$")


// Static initializers.

ACE_Allocator *TAO_Exceptions::global_allocator_;

// Flag that denotes that the TAO TypeCode constants have been
// initialized.
int TAO_Exceptions::initialized_ = 0;

// TAO specific typecode.
extern CORBA::TypeCode_ptr TC_completion_status;

// ****************************************************************

CORBA_Exception::CORBA_Exception (const char *repository_id)
  : id_ (CORBA::string_dup (repository_id))
{
  ACE_ASSERT (this->id_ != 0);
}

CORBA_Exception::CORBA_Exception (const CORBA_Exception &src)
  : id_ (CORBA::string_dup (src.id_))
{
  ACE_ASSERT (this->id_ != 0);
}

// NOTE: It's this code, not anything defined in a subclass, which is
// responsible for releasing any storage owned by the exception.  It
// can do this because it's got the typecode.

CORBA_Exception::CORBA_Exception (void)
  :  id_ (0)
{
}

CORBA_Exception::~CORBA_Exception (void)
{
  CORBA::string_free (this->id_);
}

CORBA_Exception &
CORBA_Exception::operator= (const CORBA_Exception &src)
{
  if (this->id_)
    CORBA::string_free (this->id_);
  this->id_ = CORBA::string_dup (src.id_);
  ACE_ASSERT (this->id_ != 0);

  return *this;
}

const char *
CORBA_Exception::_id (void) const
{
  return this->id_;
}

CORBA::TypeCode_ptr
CORBA_Exception::_type (void) const
{
  return CORBA::TypeCode::_nil ();
}

int
CORBA_Exception::_is_a (const char* repository_id) const
{
  return ACE_OS_String::strcmp (repository_id,
                                "IDL:omg.org/CORBA/Exception:1.0") == 0;
}

void
CORBA_Exception::_tao_print_exception (const char *user_provided_info,
                                       FILE *) const
{
  ACE_DEBUG ((LM_ERROR,
              ACE_LIB_TEXT("(%P|%t) EXCEPTION, %s\n")
              ACE_LIB_TEXT("%s\n"),
              ACE_TEXT_CHAR_TO_TCHAR(user_provided_info),
              ACE_TEXT_CHAR_TO_TCHAR(this->_info ().c_str ())));
}

#if defined (ACE_USES_WCHAR)
void
CORBA_Exception::_tao_print_exception (const ACE_WCHAR_T *info,
                                       FILE *f) const
{
    // Even though this call causes additional type conversions,
    // this is better for the maintenance.  Plus, this will occur
    // only on exception anyway.
    this->_tao_print_exception(ACE_TEXT_ALWAYS_CHAR(info), f);
}
#endif  // ACE_USES_WCHAR

void
CORBA_Exception::_tao_any_destructor (void *x)
{
  CORBA_Exception *tmp = ACE_static_cast (CORBA_Exception *, x);
  delete tmp;
}

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)

// Convenient ostrean operator.
ostream& operator<< (ostream &os,
                     const CORBA_Exception &e)
{
  const char *p = 0;

  CORBA::TypeCode_ptr tc = e._type ();

  if (tc != CORBA::TypeCode::_nil ())
    {
      p = tc->name ();
    }

  if (p != 0 && *p != '\0')
    {
      os << p << " (" << e._id () << ')';
    }
  else
    {
      os << e._id ();
    }

  return os;
}

#endif /* (ACE_LACKS_IOSTREAM_TOTALLY) */

// Avoid zillions of not-quite-inlined copies of utilities.

CORBA_UserException::CORBA_UserException (void)
{
}

CORBA_UserException::CORBA_UserException (const char *repository_id)
  : CORBA_Exception (repository_id)
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
  return ACE_OS_String::strcmp (interface_id,
                                "IDL:omg.org/CORBA/UserException:1.0") == 0
    || CORBA_Exception::_is_a (interface_id);
}

CORBA_UserException*
CORBA_UserException::_downcast (CORBA_Exception* exception)
{
  if (exception->_is_a ("IDL:omg.org/CORBA/UserException:1.0"))
    return ACE_dynamic_cast (CORBA_UserException *,
                             exception);
  return 0;
}

ACE_CString
CORBA_UserException::_info (void) const
{
  // @@ we can use the exception's typecode to dump all the data held
  // within it ...

  ACE_CString user_exception_info = "user exception, ID '";
  user_exception_info += this->_id ();
  user_exception_info += "'";
  return user_exception_info;
}

// ****************************************************************

TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (CORBA)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_UnknownUserException, 0)
TAO_NAMESPACE_END

// ****************************************************************

CORBA_SystemException::CORBA_SystemException (void)
{
}

CORBA_SystemException::CORBA_SystemException (const char *repository_id,
                                              CORBA::ULong code,
                                              CORBA::CompletionStatus completed)
  : CORBA_Exception (repository_id),
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
  return ACE_OS_String::strcmp (interface_id,
                                "IDL:omg.org/CORBA/SystemException:1.0") == 0
    || CORBA_Exception::_is_a (interface_id);
}

CORBA_SystemException*
CORBA_SystemException::_downcast (CORBA_Exception* exception)
{
  if (exception->_is_a ("IDL:omg.org/CORBA/SystemException:1.0"))
    return ACE_dynamic_cast (CORBA_SystemException *,
                             exception);
  return 0;
}

void
CORBA_SystemException::_tao_encode (TAO_OutputCDR &cdr
                                    ACE_ENV_ARG_DECL) const
{
  if (cdr.write_string (this->_id ())
      && cdr.write_ulong (this->minor ())
      && cdr.write_ulong (this->completed ()))
    return;
  ACE_THROW (CORBA::MARSHAL ());
}

void
CORBA_SystemException::_tao_decode (TAO_InputCDR &cdr
                                    ACE_ENV_ARG_DECL)
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
    case EPIPE:
      return TAO_EPIPE_MINOR_CODE;
    case ECONNREFUSED:
      return TAO_ECONNREFUSED_MINOR_CODE;
    case ENOENT:
      return TAO_ENOENT_MINOR_CODE;

#if !defined (ACE_HAS_WINCE)
    case EMFILE:
      return TAO_EMFILE_MINOR_CODE;
    case EBADF:
      return TAO_EBADF_MINOR_CODE;
    case EPERM:
      return TAO_EPERM_MINOR_CODE;
    case EINVAL:
      return TAO_EINVAL_MINOR_CODE;
#endif  // ACE_HAS_WINCE

#if (ENOSYS != EFAULT)
    case ENOSYS:
      return TAO_ENOSYS_MINOR_CODE;
#endif /* ENOSYS != EFAULT */
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
      return errno_value & 0x7FU;
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
              ACE_LIB_TEXT("(%P|%t) system exception, ID '%s'\n"),
              ACE_TEXT_CHAR_TO_TCHAR(this->_info ().c_str ())));
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
      // @@ Move the following code to a subroutine, it is too long already!
      const char *location;
      switch (this->minor () & 0x00000F80u)
        {
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
        case TAO_POA_INACTIVE:
          location = "poa in inactive state";
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
        case TAO_IMPLREPO_MINOR_CODE:
          location = "implrepo server exception";
          break;
        case TAO_ACCEPTOR_REGISTRY_OPEN_LOCATION_CODE:
          location = "endpoint initialization failure in Acceptor Registry";
          break;
        case TAO_ORB_CORE_INIT_LOCATION_CODE:
          location = "ORB Core initialization failed";
          break;
        case TAO_POLICY_NARROW_CODE:
          location = "Failure when narrowing a Policy";
          break;
        case TAO_GUARD_FAILURE:
          location = "Failure when trying to acquire a guard/monitor";
          break;
        case TAO_POA_BEING_DESTROYED:
          location = "POA has been destroyed or is currently being destroyed";
          break;
        default:
          location = "unknown location";
        }

      const char *errno_indication;
      char unknown_errno [32];
      CORBA::ULong minor_code = this->minor () & 0x7FU;
      switch (minor_code)
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
          errno_indication = "ENOTSUP";
          break;
        default:
          {
            // 7 bits of some other errno.
            ACE_OS::sprintf (unknown_errno,
                             "low 7 bits of errno: %3u",
                             minor_code);

            errno_indication = unknown_errno;
          }
        }

      char buffer[BUFSIZ];
      ACE_OS::sprintf (buffer,
                       "TAO exception, "
                       "minor code = %x (%s; %s), "
                       "completed = %s\n",
                       minor_code,
                       location,
                       errno_indication,
                       (completed () == CORBA::COMPLETED_YES) ? "YES" :
                       (completed () == CORBA::COMPLETED_NO) ? "NO" :
                       (completed () == CORBA::COMPLETED_MAYBE) ? "MAYBE" :
                       "garbage");

      info += buffer;
    }
  else if (VMCID == TAO_OMG_VMCID)
    {
      CORBA::ULong minor_code = this->minor () & 0xFFFU;

      const char *minor_description = 0;

      if (minor_code > 0)
          minor_description =
            CORBA::SystemException::_tao_get_omg_exception_description (
              *this,
              minor_code);
      else
        minor_description = "*unknown description*";

      char buffer[BUFSIZ];
      ACE_OS::sprintf (buffer,
                       "OMG minor code (%d), "
                       "described as '%s', "
                       "completed = %s\n",
                       minor_code,
                       minor_description,
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
                       "Unknown vendor minor code id (%x), "
                       "minor code = %x, completed = %s\n",
                       VMCID,
                       this->minor (),  // Use the raw minor code
                       (completed () == CORBA::COMPLETED_YES) ? "YES" :
                       (completed () == CORBA::COMPLETED_NO) ? "NO" :
                       (completed () == CORBA::COMPLETED_MAYBE) ? "MAYBE" :
                       "garbage");

      info += buffer;
    }

  return info;
}

const char *
CORBA_SystemException::_tao_get_omg_exception_description (
  const CORBA::SystemException &exc,
  CORBA::ULong minor_code)
{
#ifndef ACE_NDEBUG

  static const char *UNKNOWN_TABLE[] =
    {
        "Unlisted user exception received by client.",    // 1
        "Non-standard System Exception not supported."    // 2
    };

  static const char *BAD_PARAM_TABLE[] =
    {
        "Failure to register, unregister, or lookup value factory.", // 1
        "RID already defined in IFR.",                               // 2
        "Name already used in the context in IFR.",                  // 3
        "Target is not a valid container.",                          // 4
        "Name clash in inherited context.",                          // 5
        "Incorrect type for abstract interface.",                    // 6
        "string_to_object conversion failed due to a bad scheme name.", // 7
        "string_to_object conversion failed due to a bad address.",  // 8
        "string_to_object conversion failed due to a bad schema specific part.",// 9
        "string_to_object conversion failed due to non specific reason.", // 10
        "Attempt to derive abstract interface from non-abstract base interface in the Interface Repository.", // 11
        "Attempt to let a ValueDef support more than one non-abstract interface in the Interface Repository.", // 12
        "Attempt to use an incomplete TypeCode as a parameter.",     // 13
        "Invalid object id passed to POA::create_reference_by_id.",  // 14
        "Bad name argument in TypeCode operation.",                  // 15
        "Bad RepositoryId argument in TypeCode operation.",          // 16
        "Invalid member namein TypeCode operation.",                 // 17
        "Duplicate label value in create_union_tc.",                 // 18
        "Incompatible TypeCode of label and discriminator in create_union_tc.", // 19
        "Supplied discriminator type illegitimate in create_union_tc.", // 20
        "Any passed to ServerRequest::set_exception does not contain an exception.", // 21
        "Unlisted user exception passed to ServerRequest::set_exception", // 22
        "wchar transmission code set not in service context.",       // 23
        "Service context is not in OMG-defined range.",              // 24
        "Enum value out of range.",                                  // 25
        "Invalid service context Id in portable interceptor.",       // 26
        "Attempt to call register_initial_reference with a null Object.", // 27
        "Invalid component Id in portable interceptor.",             // 28
        "Invalid profile Id in portable interceptor.",               // 29
        "Two or more Policy objects with the same PolicyType value supplied to Object::set_policy_overrides or PolicyManager::set_policy_overrides." // 30
    };

  static const char *IMP_LIMIT_TABLE[] =
    {
        "Unable to use any profile in IOR." // 1
    };

  static const char *INV_OBJREF_TABLE[] =
    {
        "wchar Code Set support not specified.", // 1
        "Codeset component required for type using wchar or wstring data." // 2
    };

  static const char *MARSHAL_TABLE[] =
    {
        "Unable to locate value factory.",  // 1
        "ServerRequest::set_result called before ServerRequest::ctx when the operation IDL contains a context clause.", // 2
        "NVList passed to ServerRequest::arguments does not describe all parameters passed by client.", // 3
        "Attempt to marshal Local object.", // 4
        "wchar or wstring data erroneously sent by client over GIOP 1.0 connection.", // 5
        "wchar or wstring data erroneously returned by server over GIOP 1.0 connection." //6
    };

  static const char *BAD_TYPECODE_TABLE[] =
    {
        "Attempt to marshal incomplete TypeCode.",             // 1
        "Member type code illegitimate in TypeCode operation." // 2
    };

  static const char *NO_IMPLEMENT_TABLE[] =
    {
        "Missing local value implementation.",        // 1
        "Incompatible value implementation version.", // 2
        "Unable to use any profile in IOR.",          // 3
        "Attempt to use DII on Local object."         // 4
    };

  static const char *NO_RESOURCE_TABLE[] =
    {
        "Portable Interceptor operation not support in this binding." // 1
    };

  static const char *BAD_INV_ORDER_TABLE[] =
    {
        "Dependency exists in IFR preventing destruction of this object", // 1
        "Attempt to destroy indestructible objects in IFR.", // 2
        "Operation would deadlock.",                         // 3
        "ORB has shutdown.",                                 // 4
        "Attempt to invoke \"send\" or \"invoke\" operation of the same \"Request\" object more than once.", // 5
        "Attempt to set a servant manager after one has already been set.", // 6
        "ServerRequest::arguments called more than once or after a call to ServerRequest::set_exception.", // 7
        "ServerRequest::ctx called more than once or before ServerRequest::arguments or after ServerRequest::ctx, ServerRequest::set_result or ServerRequest::set_exception.", // 8
        "ServerRequest::result called more than once or before ServerRequest::arguments or after ServerRequest::set_result or ServerRequest::set_exception.", // 9
        "Attempt to send a DII request after it was sent previously.", // 10
        "Attempt to poll a DII request or to retrieve its result before the request was sent.", // 11
        "Attempt to poll a DII request or to retrieve its result after the result was retrieved previously.", // 12
        "Attempt to poll a synchronous DII request or to retrieve results from a synchronous DII request.", // 13
        "Invalid portable interceptor call",                 // 14
        "Service context add failed in portable interceptor because a service context with the given id already exists.", // 15
        "Registration of PolicyFactory failed because a factory already exists for the given type.", // 16
        "POA cannot create POAs while undergoing destruction." // 17
    };

  static const char *TRANSIENT_TABLE[] =
    {
        "Request discarded due to resource exhaustion in POA, or because POA is in discarding state.", // 1
        "No usable profile in IOR.",                            // 2
        "Request cancelled.",                                   // 3
        "POA destroyed."                                        // 4
    };

  static const char *OBJ_ADAPTER_TABLE[] =
    {
        "System exception in POA::unknown_adapter.",              // 1
        "Servant not found [ServantManager].",                    // 2
        "No default servant available [POA policy].",             // 3
        "No servant manager available [POA policy].",             // 4
        "Violation of POA policy by ServantActivator::incarnate." // 5
    };

  static const char *DATA_CONVERSION_TABLE[] =
    {
        "Character does not map to negotiated transmission code set." // 1
    };

  static const char *OBJECT_NOT_EXIST_TABLE[] =
    {
        "Attempt to pass an unactivated (unregistered) value as an object reference.", // 1
        "POAManager::incarnate failed to create POA." // 2
    };

  static const char *INV_POLICY_TABLE[] =
    {
        "Unable to reconcile IOR specified policy with the effective policy override." // 1
        "Invalid PolicyType.", // 2
        "No PolicyFactory has been registered for the given PolicyType." // 3
    };

  if (minor_code == 0)
    return "*unknown description*";

  minor_code--;  // Adjust to match table offset.

  if (exc._is_a ("IDL:omg.org/CORBA/UNKNOWN:1.0")
      && minor_code < sizeof UNKNOWN_TABLE / sizeof (char *))
    return UNKNOWN_TABLE[minor_code];

  if (exc._is_a ("IDL:omg.org/CORBA/BAD_PARAM:1.0")
      && minor_code < sizeof BAD_PARAM_TABLE / sizeof (char *))
    return BAD_PARAM_TABLE[minor_code];

  if (exc._is_a ("IDL:omg.org/CORBA/IMP_LIMIT:1.0")
      && minor_code < sizeof IMP_LIMIT_TABLE / sizeof (char *))
    return IMP_LIMIT_TABLE[minor_code];

  if (exc._is_a ("IDL:omg.org/CORBA/INV_OBJREF:1.0")
      && minor_code < sizeof INV_OBJREF_TABLE / sizeof (char *))
    return INV_OBJREF_TABLE[minor_code];

  if (exc._is_a ("IDL:omg.org/CORBA/MARSHAL:1.0")
      && minor_code < sizeof MARSHAL_TABLE / sizeof (char *))
    return MARSHAL_TABLE[minor_code];

  if (exc._is_a ("IDL:omg.org/CORBA/BAD_TYPECODE:1.0")
      && minor_code < sizeof BAD_TYPECODE_TABLE / sizeof (char *))
    return BAD_TYPECODE_TABLE[minor_code];

  if (exc._is_a ("IDL:omg.org/CORBA/NO_IMPLEMENT:1.0")
      && minor_code < sizeof NO_IMPLEMENT_TABLE / sizeof (char *))
    return NO_IMPLEMENT_TABLE[minor_code];

  if (exc._is_a ("IDL:omg.org/CORBA/NO_RESOURCE:1.0")
      && minor_code < sizeof NO_RESOURCE_TABLE / sizeof (char *))
    return NO_RESOURCE_TABLE[minor_code];

  if (exc._is_a ("IDL:omg.org/CORBA/BAD_INV_ORDER:1.0")
      && minor_code < sizeof BAD_INV_ORDER_TABLE / sizeof (char *))
    return BAD_INV_ORDER_TABLE[minor_code];

  if (exc._is_a ("IDL:omg.org/CORBA/TRANSIENT:1.0")
      && minor_code < sizeof TRANSIENT_TABLE / sizeof (char *))
    return TRANSIENT_TABLE[minor_code];

  if (exc._is_a ("IDL:omg.org/CORBA/OBJ_ADAPTER:1.0")
      && minor_code < sizeof OBJ_ADAPTER_TABLE / sizeof (char *))
    return OBJ_ADAPTER_TABLE[minor_code];

  if (exc._is_a ("IDL:omg.org/CORBA/DATA_CONVERSION:1.0")
      && minor_code < sizeof DATA_CONVERSION_TABLE / sizeof (char *))
    return DATA_CONVERSION_TABLE[minor_code];

  if (exc._is_a ("IDL:omg.org/CORBA/OBJECT_NOT_EXIST:1.0")
      && minor_code < sizeof OBJECT_NOT_EXIST_TABLE / sizeof (char *))
    return OBJECT_NOT_EXIST_TABLE[minor_code];

  if (exc._is_a ("IDL:omg.org/CORBA/INV_POLICY:1.0")
      && minor_code < sizeof INV_POLICY_TABLE / sizeof (char *))
    return INV_POLICY_TABLE[minor_code];

#else
  ACE_UNUSED_ARG (exc);
  ACE_UNUSED_ARG (minor_code);
#endif  /* !ACE_NDEBUG */

  return "*unknown description*";
}

// Note that "buffer" holds the (unscoped) name originally, and is
// then overwritten.

void
TAO_Exceptions::make_unknown_user_typecode (CORBA::TypeCode_ptr &tcp
                                            ACE_ENV_ARG_DECL)
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
                        TAO_Exceptions::global_allocator_,
                        ACE_DEFAULT_CDR_MEMCPY_TRADEOFF,
                        &translator);
#else
  TAO_OutputCDR stream (0,
                        ACE_CDR_BYTE_ORDER,
                        TAO_Exceptions::global_allocator_,
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
    || stream.write_string (field_name) == 0;
  if (result)
    ACE_THROW (CORBA_INITIALIZE ());

  if (!(stream << CORBA::_tc_any))
    ACE_THROW (CORBA_INITIALIZE ());

  ACE_NEW_THROW_EX (tcp,
                    CORBA::TypeCode (CORBA::tk_except,
                                     stream.length (),
                                     stream.buffer (),
                                     1,
                                     sizeof (CORBA_UserException)),
                    CORBA_INITIALIZE ());
}

void
TAO_Exceptions::make_standard_typecode (CORBA::TypeCode_ptr &tcp,
                                        const char *name,
                                        char *buffer,
                                        size_t buflen
                                        ACE_ENV_ARG_DECL)
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
                        TAO_Exceptions::global_allocator_,
                        ACE_DEFAULT_CDR_MEMCPY_TRADEOFF,
                        &translator);
#else
  TAO_OutputCDR stream (buffer, buflen,
                        ACE_CDR_BYTE_ORDER,
                        TAO_Exceptions::global_allocator_,
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
                         + ACE_OS_String::strlen (name)
                         + sizeof suffix);

  ACE_OS_String::strcpy (full_id, prefix);
  ACE_OS_String::strcat (full_id, name);
  ACE_OS_String::strcat (full_id, suffix);

  CORBA::Boolean result = stream.write_octet (TAO_ENCAP_BYTE_ORDER) == 0
    || stream.write_string (full_id) == 0
    || stream.write_string (name) == 0
    || stream.write_ulong (2L) != 1
    || stream.write_string (minor) == 0;

  result = result || !(stream << CORBA::_tc_ulong);

  CORBA::string_free (full_id);  // No longer need the string

  result = result || stream.write_string (completed) == 0;
  result = result || !(stream << TC_completion_status);

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
                    CORBA_INITIALIZE ());
  ACE_CHECK;

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
    TAO_SYSTEM_EXCEPTION (INVALID_TRANSACTION) \
    TAO_SYSTEM_EXCEPTION (CODESET_INCOMPATIBLE) \
    TAO_SYSTEM_EXCEPTION (BAD_QOS)

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

void
TAO_Exceptions::init (ACE_ENV_SINGLE_ARG_DECL)
{
  // This routine should only be called once.

  // Not thread safe.  Caller must provide synchronization.

  if (TAO_Exceptions::initialized_ != 0)
    return;

  TAO_Exceptions::initialized_ = 1;

  // Initialize the start up allocator.
  ACE_NEW (TAO_Exceptions::global_allocator_,
           ACE_New_Allocator);

#define TAO_SYSTEM_EXCEPTION(name) \
  TAO_Exceptions::make_standard_typecode (CORBA::_tc_ ## name, \
                                          #name, \
                                          (char*)tc_buf_##name, \
                                          sizeof(tc_buf_##name) \
                                           ACE_ENV_ARG_PARAMETER); \
  ACE_CHECK;
  STANDARD_EXCEPTION_LIST
#undef  TAO_SYSTEM_EXCEPTION

  TAO_Exceptions::make_unknown_user_typecode (CORBA::_tc_UnknownUserException
                                               ACE_ENV_ARG_PARAMETER);
}

CORBA_SystemException *
TAO_Exceptions::create_system_exception (const char *id
                                         ACE_ENV_ARG_DECL_NOT_USED)
{
#define TAO_SYSTEM_EXCEPTION(name) \
  { \
    const char* xid = "IDL:omg.org/CORBA/" #name ":1.0"; \
    if (ACE_OS_String::strcmp (id, xid) == 0) \
      return new CORBA:: name; \
  }
  STANDARD_EXCEPTION_LIST
#undef TAO_SYSTEM_EXCEPTION

  return 0;
}

void
TAO_Exceptions::fini (void)
{
#define TAO_SYSTEM_EXCEPTION(name) \
  CORBA::release (CORBA::_tc_ ## name); \
  CORBA::_tc_ ## name = 0;
  STANDARD_EXCEPTION_LIST
#undef TAO_SYSTEM_EXCEPTION

  CORBA::release (CORBA::_tc_UnknownUserException);
  CORBA::_tc_UnknownUserException = 0;

  delete TAO_Exceptions::global_allocator_;
  TAO_Exceptions::global_allocator_ = 0;
}

#define TAO_SYSTEM_EXCEPTION(name) \
int \
CORBA_##name ::_is_a (const char* interface_id) const \
{ \
  return ((ACE_OS_String::strcmp ( \
               interface_id, \
               "IDL:omg.org/CORBA/" #name ":1.0") == 0) \
          || CORBA_SystemException::_is_a (interface_id)); \
}
STANDARD_EXCEPTION_LIST
#undef TAO_SYSTEM_EXCEPTION

#define TAO_SYSTEM_EXCEPTION(name) \
CORBA_##name * \
CORBA_##name ::_downcast (CORBA_Exception* exception) \
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
  :  CORBA_SystemException ("IDL:omg.org/CORBA/" #name ":1.0", \
                            TAO_DEFAULT_MINOR_CODE, \
                            CORBA::COMPLETED_NO) \
{ \
}
STANDARD_EXCEPTION_LIST
#undef TAO_SYSTEM_EXCEPTION

#define TAO_SYSTEM_EXCEPTION(name) \
CORBA::TypeCode_ptr \
CORBA_##name ::_type (void) const \
{ \
  return CORBA::_tc_ ## name; \
}
STANDARD_EXCEPTION_LIST
#undef TAO_SYSTEM_EXCEPTION

#define TAO_SYSTEM_EXCEPTION(name) \
void \
CORBA_##name ::_tao_any_destructor (void *x) \
{ \
  CORBA_##name *tmp = ACE_static_cast (CORBA_##name *, x); \
  delete tmp; \
}
STANDARD_EXCEPTION_LIST
#undef TAO_SYSTEM_EXCEPTION

#define TAO_SYSTEM_EXCEPTION(name) \
CORBA_Exception * \
CORBA_##name ::_tao_duplicate (void) const \
{ \
  CORBA_Exception *result; \
  ACE_NEW_RETURN (result, CORBA_##name (*this), 0); \
  return result; \
}
STANDARD_EXCEPTION_LIST
#undef TAO_SYSTEM_EXCEPTION

static void
tao_insert_for_insertion_system_exception (CORBA::Any &any,
                                           const CORBA::SystemException &ex,
                                           const char *msg)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      TAO_OutputCDR stream;
      ex._tao_encode (stream ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      any._tao_replace (ex._type (),
                        TAO_ENCAP_BYTE_ORDER,
                        stream.begin ());
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (
          ACE_ANY_EXCEPTION,
          msg);
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

#define TAO_SYSTEM_EXCEPTION(name) \
void operator<<= (CORBA::Any &any, const CORBA_##name &ex) \
{ \
  tao_insert_for_insertion_system_exception (any, ex, \
            "\tCORBA::Any insertion (non-copy) of CORBA_" #name "\n" \
                               ); \
}
STANDARD_EXCEPTION_LIST
#undef TAO_SYSTEM_EXCEPTION

static void
tao_insert_system_exception (CORBA::Any &any,
                             CORBA::SystemException *ex,
                             CORBA::Any::_tao_destructor destructor,
                             const char *msg)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      TAO_OutputCDR stream;
      ex->_tao_encode (stream ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      any._tao_replace (ex->_type (),
                        TAO_ENCAP_BYTE_ORDER,
                        stream.begin (),
                        1,
                        ex,
                        destructor);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (
          ACE_ANY_EXCEPTION,
          msg
        );
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

#define TAO_SYSTEM_EXCEPTION(name) \
void operator<<= (CORBA::Any &any, CORBA_##name *ex) \
{ \
  tao_insert_system_exception (any, ex, \
          CORBA_##name ::_tao_any_destructor, \
          "\tCORBA::Any insertion (non-copy) of CORBA_" #name "\n" \
                               ); \
}

STANDARD_EXCEPTION_LIST
#undef TAO_SYSTEM_EXCEPTION

#define TAO_SYSTEM_EXCEPTION(name) \
static CORBA_SystemException* _tao_allocator_##name (void) \
{ \
  return new CORBA_##name; \
}
STANDARD_EXCEPTION_LIST
#undef TAO_SYSTEM_EXCEPTION

static CORBA::Boolean
tao_insert_in_extractor_system_exception (
        const CORBA::Any &any,
        CORBA::SystemException *&tmp,
        CORBA::SystemException * (*allocator)(void),
        CORBA::TypeCode_ptr tc_name,
        CORBA::Any::_tao_destructor destructor,
        const char *compare_IR_Id,
        const char *msg)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::TypeCode_var type = any.type ();
      CORBA::Boolean equiv =
        type->equivalent (tc_name ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (!equiv)
        return 0;
      if (any.any_owns_data ())
        {
          tmp = (CORBA_SystemException *)any.value ();
          return 1;
        }
      else
        {
          TAO_InputCDR stream (
              any._tao_get_cdr (),
              any._tao_byte_order ()
            );
          CORBA::String_var interface_repository_id;
          if (!(stream >> interface_repository_id.out ()))
            return 0;
          if (ACE_OS_String::strcmp (interface_repository_id.in (),
                                     compare_IR_Id))
            return 0;
          CORBA::SystemException *tmp = allocator ();
          tmp->_tao_decode (stream ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          ((CORBA::Any *)&any)->_tao_replace (
              tc_name,
              1,
              tmp,
              destructor
            );
          return 1;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (
          ACE_ANY_EXCEPTION,
          msg
        );
    }
  ACE_ENDTRY;
  return 0;
}

#define TAO_SYSTEM_EXCEPTION(name) \
CORBA::Boolean operator>>= (const CORBA::Any &any, \
                            const CORBA_##name *&ex) \
{ \
  ex = 0; \
  CORBA_SystemException *tmp; \
  if (tao_insert_in_extractor_system_exception (any, \
       tmp, \
       _tao_allocator_##name, \
       CORBA::_tc_##name, \
       CORBA_##name ::_tao_any_destructor, \
       "IDL:omg.org/CORBA/" #name ":1.0", \
       "\tCORBA::Any extraction of CORBA_" #name "\n") == 0) \
         { \
             ex = 0; \
         return 0; \
         } \
 ex = (CORBA_##name*)tmp; \
 return 1; \
}

STANDARD_EXCEPTION_LIST
#undef TAO_SYSTEM_EXCEPTION

#undef STANDARD_EXCEPTION_LIST

#if defined (TAO_DONT_CATCH_DOT_DOT_DOT)
TAO_DONT_CATCH::TAO_DONT_CATCH (void)
{}
#endif /* TAO_DONT_CATCH_DOT_DOT_DOT */
