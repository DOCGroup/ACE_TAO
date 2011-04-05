//$Id$

#include "tao/operation_details.h"
#include "tao/Stub.h"
#include "tao/ORB_Constants.h"
#include "tao/Exception_Data.h"
#include "tao/SystemException.h"
#include "tao/Argument.h"
#include "tao/CDR.h"
#include "tao/debug.h"

#include "ace/OS_NS_string.h"

#if !defined (__ACE_INLINE__)
# include "tao/operation_details.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Exception *
TAO_Operation_Details::corba_exception (const char *id) const
{
  for (CORBA::ULong i = 0; i != this->ex_count_; ++i)
    {
      if (ACE_OS::strcmp (id, this->ex_data_[i].id) != 0)
        {
          continue;
        }

      // Create an exception object
      CORBA::Exception *exception = this->ex_data_[i].alloc ();

      if (exception == 0)
        {
          throw ::CORBA::NO_MEMORY (0, CORBA::COMPLETED_YES);
        }

      // Return the exception object that we just created.
      return exception;
    }

  // If there are no matches return an unknown exception.
  throw ::CORBA::UNKNOWN (0, CORBA::COMPLETED_YES);
}

bool
TAO_Operation_Details::has_exception (::CORBA::Exception& ex) const
{
  for (CORBA::ULong i = 0; i != this->ex_count_; ++i)
    {
      if (ACE_OS::strcmp (ex._rep_id (), this->ex_data_[i].id) == 0)
        {
          return true;
        }
    }
  return false;
}

bool
TAO_Operation_Details::marshal_args (TAO_OutputCDR &cdr)
{
  try {
    for (CORBA::ULong i = 0; i != this->num_args_; ++i)
      {
      if (!((*this->args_[i]).marshal (cdr)))
        return false;
      }

    // Nothing else to fragment.  We're also guaranteed to have
    // data in the CDR stream since the operation was a marshaling
    // operation, not a fragmentation operation.
    cdr.more_fragments (false);
#ifdef TAO_HAS_VALUETYPE_OUT_INDIRECTION
    cdr.reset_vt_indirect_maps ();
#endif
    }
  catch (...) {
#ifdef TAO_HAS_VALUETYPE_OUT_INDIRECTION
    cdr.reset_vt_indirect_maps ();
#endif
    throw;
  }
  return true;
}

bool
TAO_Operation_Details::demarshal_args (TAO_InputCDR &cdr)
{
  try {
    for (CORBA::ULong i = 0; i != this->num_args_; ++i)
      {
        if (!((*this->args_[i]).demarshal (cdr)))
          return false;
      }

    cdr.reset_vt_indirect_maps ();
  }
  catch (...) {
    cdr.reset_vt_indirect_maps ();
    throw;
  }
  return true;
}

TAO_END_VERSIONED_NAMESPACE_DECL
