//$Id$
#include "operation_details.h"
#include "Stub.h"
#include "Argument.h"

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


bool
TAO_Operation_Details::marshal_args (TAO_OutputCDR &cdr)
{
  for (int i = 0; i != this->num_args_; ++i)
    {
      if (!((*this->args_[i]).marshal (cdr)))
        return false;
    }

  return true;
}

bool
TAO_Operation_Details::demarshal_args (TAO_InputCDR &cdr)
{
  for (int i = 0; i != this->num_args_; ++i)
    {
      if (!((*this->args_[i]).demarshal (cdr)))
        return false;
    }

  return true;
}

bool
TAO_Operation_Details::parameter_list (Dynamic::ParameterList &list)
{
  list.length (this->num_args_);

   for (int i = 0; i != this->num_args_; ++i)
     (*this->args_[i]).interceptor_param (list[i]);

   return true;
}


bool
TAO_Operation_Details::exception_list (Dynamic::ExceptionList &list)
{
  /*if (this->ex_count_)
    {
      list.length (this->ex_count_);

      for (int i = 0; i != this->ex_count_; ++i)
        {
          if (!((*this->args_[i]).interceptor_param (list[i])))
            return false;
        }
        }*/
  return true;
}
