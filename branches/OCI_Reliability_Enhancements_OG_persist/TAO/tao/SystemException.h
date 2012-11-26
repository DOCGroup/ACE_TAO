// -*- C++ -*-

//=============================================================================
/**
 *  @file    SystemException.h
 *
 *  $Id$
 *
 *  CORBA::SystemException class header.
 *
 *  @author DOC Group at Vanderbilt U, Wash U, and UCI
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_SYSTEM_EXCEPTION_H
#define TAO_SYSTEM_EXCEPTION_H

#include /**/ "ace/pre.h"

// Do not try removing this. If you remove this for subsetting lots of
// things go wrong in TAO.
#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Exception.h"

#ifndef TAO_Exception_Export
# if (__GNUC__ > 3)
// Exceptions found in DLL A but potentially thrown in DLL B must
// always be exported.
#  define TAO_Exception_Export ACE_Proper_Export_Flag
# else
#  define TAO_Exception_Export TAO_Export
# endif  /* __GNUC__ > 3*/
#endif  /* !TAO_Exception_Export */

#ifdef THREAD_CANCELLED
# undef THREAD_CANCELLED
#endif /* THREAD_CANCELLED */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_OutputCDR;
class TAO_InputCDR;

// This is already done in orbconf.h. But this file is totally
// decoupled from its contents that we have to do this here. Including
// orbconf.h is probably going to be a overhead.
#if defined (minor)
#undef minor
#endif /* minor */

namespace CORBA
{
  class SystemException;
}

namespace TAO
{
  typedef CORBA::SystemException* (*excp_factory)(void);
}

namespace CORBA
{
  /**
   * @enum CompletionStatus
   *
   * @brief Completion status for CORBA system exceptions
   *
   * Enumerations that denote how an operation was (not) completed
   * when a @c CORBA::SystemException is thrown.
   */
  enum CompletionStatus
  {
    COMPLETED_YES,     ///< Successful or exceptional completion.
    COMPLETED_NO,      ///< Didn't change any state; retry is OK.
    COMPLETED_MAYBE    ///< Can't say what happened; retry unsafe.
  };

  /**
   * @class SystemException
   *
   * @brief SystemException
   *
   * System exceptions are those defined in the CORBA specification;
   * OMG-IDL defines these.
   */
  class TAO_Export SystemException : public Exception
  {
  public:

    /// Copy constructor.
    /**
     * @note This constructor should be protected, but VC7.1 at
     *       warning level 4 complains about the inaccessible copy
     *       constructor preventing it from being caught.  However,
     *       that probably isn't true for most cases since CORBA
     *       exceptions are typically caught by reference, not by
     *       copy.
     */
    SystemException (const SystemException & src);

    /// Destructor.
    virtual ~SystemException (void);

    /// Get the minor status.
    ULong minor (void) const;

    /// Set the minor status.
    void minor (ULong m);

    /// Get the completion status.
    CORBA::CompletionStatus completed (void) const;

    /// Set the operation completion status.
    void completed (CORBA::CompletionStatus c);

    /// Narrow to a SystemException.
    static SystemException *_downcast (CORBA::Exception *exception);

    /// The const version of narrow operation to a SystemException
    static const SystemException *_downcast(const CORBA::Exception *exception);

    virtual void _raise (void) const = 0;

    // = TAO-specific extension.

    /// Print the system exception @c ex to output determined by @c f.
    /// This function is not CORBA compliant.
    void _tao_print_system_exception (FILE *f = stdout) const;

    /// Returns a string containing information about the exception. This
    /// function is not CORBA compliant.
    virtual ACE_CString _info (void) const;

    virtual void _tao_encode (TAO_OutputCDR &cdr) const;

    virtual void _tao_decode (TAO_InputCDR &cdr);

    /// Helper to create a minor status value.
    static CORBA::ULong _tao_minor_code (u_int location, int errno_value);

    /// Helper to translate a platform-specific errno to a TAO errno
    /// value.
    static CORBA::ULong _tao_errno (int errno_value);

    /// Deep copy
    virtual CORBA::Exception *_tao_duplicate (void) const;

  protected:

    /// Default constructor.
    SystemException (void);

    /// Assignment operator.
    SystemException & operator= (const SystemException &src);

    /// Constructor using a repository id.
    SystemException (CORBA::ULong code, CORBA::CompletionStatus completed);

    /// Constructor using a repository id.
    SystemException (const char *repository_id,
                     const char *local_name,
                     CORBA::ULong code,
                     CORBA::CompletionStatus completed);

    /// Return the exception description associated with the given OMG
    /// minor code.
    static const char *_tao_get_omg_exception_description (
      const CORBA::SystemException &exc, CORBA::ULong minor_code);

  private:
    /// Minor code.
    CORBA::ULong minor_;

    /// Completion status.
    CORBA::CompletionStatus completed_;
  };

  // Declarations for all of the CORBA standard exceptions.
  //
  // @@ - shouldn't have a default minor code, at least for code that's
  // inside the ORB.  All minor codes should be symbolically catalogued.

#define TAO_SYSTEM_EXCEPTION(name) \
    class TAO_Exception_Export name : public SystemException \
    { \
    public: \
      name (void); \
      name (CORBA::ULong code, \
            CORBA::CompletionStatus completed); \
      static name * _downcast (CORBA::Exception* exception); \
      static name const * _downcast (CORBA::Exception const * exception); \
      virtual void _raise (void) const; \
      virtual CORBA::TypeCode_ptr _tao_type (void) const; \
      static void _tao_any_destructor (void*); \
      virtual CORBA::Exception *_tao_duplicate (void) const; \
      static CORBA::SystemException *_tao_create (void); \
    }; \

  TAO_SYSTEM_EXCEPTION(UNKNOWN)          // the unknown exception
  TAO_SYSTEM_EXCEPTION(BAD_PARAM)        // an invalid parameter was passed
  TAO_SYSTEM_EXCEPTION(NO_MEMORY)        // memory allocation failure
  TAO_SYSTEM_EXCEPTION(IMP_LIMIT)        // violated implementation limit
  TAO_SYSTEM_EXCEPTION(COMM_FAILURE)     // communication failure
  TAO_SYSTEM_EXCEPTION(INV_OBJREF)       // invalid object reference
  TAO_SYSTEM_EXCEPTION(OBJECT_NOT_EXIST) // no such object
  TAO_SYSTEM_EXCEPTION(NO_PERMISSION)    // no permission for operation
  TAO_SYSTEM_EXCEPTION(INTERNAL)         // ORB internal error
  TAO_SYSTEM_EXCEPTION(MARSHAL)          // error marshaling param/result
  TAO_SYSTEM_EXCEPTION(INITIALIZE)       // ORB initialization failure
  TAO_SYSTEM_EXCEPTION(NO_IMPLEMENT)     // implementation unavailable
  TAO_SYSTEM_EXCEPTION(BAD_TYPECODE)     // bad typecode
  TAO_SYSTEM_EXCEPTION(BAD_OPERATION)    // invalid operation
  TAO_SYSTEM_EXCEPTION(NO_RESOURCES)     // out of resources for request
  TAO_SYSTEM_EXCEPTION(NO_RESPONSE)      // response not yet available
  TAO_SYSTEM_EXCEPTION(PERSIST_STORE)    // persistent storage failure
  TAO_SYSTEM_EXCEPTION(BAD_INV_ORDER)    // routine invocations out of order
  TAO_SYSTEM_EXCEPTION(TRANSIENT)        // transient error, try again later
  TAO_SYSTEM_EXCEPTION(FREE_MEM)         // cannot free memory
  TAO_SYSTEM_EXCEPTION(INV_IDENT)        // invalid identifier syntax
  TAO_SYSTEM_EXCEPTION(INV_FLAG)         // invalid flag was specified
  TAO_SYSTEM_EXCEPTION(INTF_REPOS)       // interface repository unavailable
  TAO_SYSTEM_EXCEPTION(BAD_CONTEXT)      // error processing context object
  TAO_SYSTEM_EXCEPTION(OBJ_ADAPTER)      // object adapter failure
  TAO_SYSTEM_EXCEPTION(DATA_CONVERSION)  // data conversion error
  TAO_SYSTEM_EXCEPTION(INV_POLICY)       // invalid policies present
  TAO_SYSTEM_EXCEPTION(REBIND)           // rebind needed
  TAO_SYSTEM_EXCEPTION(TIMEOUT)          // operation timed out
  TAO_SYSTEM_EXCEPTION(TRANSACTION_UNAVAILABLE) // no transaction
  TAO_SYSTEM_EXCEPTION(TRANSACTION_MODE)        // invalid transaction mode
  TAO_SYSTEM_EXCEPTION(TRANSACTION_REQUIRED)    // operation needs transaction
  TAO_SYSTEM_EXCEPTION(TRANSACTION_ROLLEDBACK)  // operation was a no-op
  TAO_SYSTEM_EXCEPTION(INVALID_TRANSACTION)     // invalid TP context passed
  TAO_SYSTEM_EXCEPTION(CODESET_INCOMPATIBLE)    // incompatible code set
  TAO_SYSTEM_EXCEPTION(BAD_QOS)          // bad quality of service
  TAO_SYSTEM_EXCEPTION(INVALID_ACTIVITY)
  TAO_SYSTEM_EXCEPTION(ACTIVITY_COMPLETED)
  TAO_SYSTEM_EXCEPTION(ACTIVITY_REQUIRED)
  TAO_SYSTEM_EXCEPTION(THREAD_CANCELLED)

#undef TAO_SYSTEM_EXCEPTION

} // End CORBA namespace

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/SystemException.inl"
#endif /* __ACE_INLINE__ */

#include /**/"ace/post.h"

#endif /* TAO_SYSTEM_EXCEPTION_H */
