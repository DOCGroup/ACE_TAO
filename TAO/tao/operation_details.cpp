//$Id$
#include "operation_details.h"
#include "Stub.h"
#include "Argument.h"
#include "DynamicC.h"
#include "ace/streams.h"

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
          ACE_THROW_RETURN (CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                              CORBA::COMPLETED_YES),
                            0);
        }

      // Return the exception object that we just created.
      return exception;
    }

  // If there are no matches return an unknown exception.
  ACE_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE,
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

bool
TAO_Operation_Details::parameter_list (Dynamic::ParameterList &param_list)
{
  // Account for the return type that could be in the argument list.
  param_list.length (this->num_args_ - 1);

   for (CORBA::ULong i = 1; i != this->num_args_; ++i)
     this->args_[i]->interceptor_param (param_list[i - 1]);

   return true;
}


bool
TAO_Operation_Details::exception_list (Dynamic::ExceptionList &exception_list)
{
  if (this->ex_count_)
    {
      exception_list.length (this->ex_count_);

      for (CORBA::ULong i = 0;
           i != this->ex_count_;
           ++i)
        {
          CORBA::TypeCode_ptr tcp = this->ex_data_[i].tc_ptr;
          TAO_Pseudo_Object_Manager<CORBA::TypeCode> tcp_object (&tcp, 1);
          exception_list[i] = tcp_object;
        }
    }
  return true;
}


bool
TAO_Operation_Details::result (CORBA::Any *any)
{
  for (CORBA::ULong i = 0; i != this->num_args_; ++i)
    (*this->args_[i]).interceptor_result (any);

  return true;
}
