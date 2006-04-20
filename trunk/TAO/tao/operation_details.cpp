//$Id$

#include "tao/operation_details.h"
#include "tao/Stub.h"
#include "tao/ORB_Constants.h"
#include "tao/Exception_Data.h"
#include "tao/SystemException.h"
#include "tao/Argument.h"
#include "tao/CDR.h"

#include "ace/OS_NS_string.h"

#if !defined (__ACE_INLINE__)
# include "tao/operation_details.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID (tao,
           operation_details,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Exception *
TAO_Operation_Details::corba_exception (const char *id
                                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  for (CORBA::ULong i = 0; i != this->ex_count_; ++i)
    {
      if (ACE_OS::strcmp (id,
                          this->ex_data_[i].id) != 0)
        {
          continue;
        }

      // Create an exception object
      CORBA::Exception *exception =
        this->ex_data_[i].alloc ();

      if (exception == 0)
        {
          ACE_THROW_RETURN (CORBA::NO_MEMORY (0,
                                              CORBA::COMPLETED_YES),
                            0);
        }

      // Return the exception object that we just created.
      return exception;
    }

  // If there are no matches return an unknown exception.
  ACE_THROW_RETURN (CORBA::UNKNOWN (0,
                                    CORBA::COMPLETED_YES),
                    0);
}

bool
TAO_Operation_Details::marshal_args (TAO_OutputCDR &cdr)
{
  for (CORBA::ULong i = 0; i != this->num_args_; ++i)
    {
      if (!((*this->args_[i]).marshal (cdr)))
        return false;
    }

  // Nothing else to fragment.  We're also guaranteed to have
  // data in the CDR stream since the operation was a marshaling
  // operation, not a fragmentation operation.
  cdr.more_fragments (false);

  return true;
}

bool
TAO_Operation_Details::demarshal_args (TAO_InputCDR &cdr)
{
  for (CORBA::ULong i = 0; i != this->num_args_; ++i)
    {
      if (!((*this->args_[i]).demarshal (cdr)))
        return false;
    }

  return true;
}

TAO_END_VERSIONED_NAMESPACE_DECL
