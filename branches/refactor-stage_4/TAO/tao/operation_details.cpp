//$Id$
#include "tao/operation_details.h"
#include "tao/Stub.h"

#if !defined (__ACE_INLINE__)
# include "tao/operation_details.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID (tao,
           operation_details,
           "$Id$")

CORBA::Exception *
TAO_Operation_Details::corba_exception (const char *id
                                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  for (CORBA::ULong i = 0; i < this->ex_count_; ++i)
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
          ACE_THROW_RETURN (CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                              CORBA::COMPLETED_YES),
                            0);
        }
    }

  ACE_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE,
                                    CORBA::COMPLETED_YES),
                    0);
}
