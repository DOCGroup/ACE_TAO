// -*- C++ -*-
// $Id$

#include "Messaging.h"
#include "Invocation.h"

ACE_RCSID(tao, Messaging, "$Id$")

#if (TAO_HAS_AMI_CALLBACK == 1)

void TAO_Messaging_Helper::
exception_holder_raise (TAO_Exception_Data *exception_data,
                        CORBA::ULong exception_count,
                        CORBA::Octet *marshaled_data,
                        CORBA::ULong marshaled_data_length,
                        CORBA::Boolean byte_order,
                        CORBA::Boolean is_system_exception
                        TAO_ENV_ARG_DECL)
{
  TAO_InputCDR _tao_in ((const char*) marshaled_data,
                        marshaled_data_length,
                        byte_order);

  CORBA::String_var type_id;

  if ((_tao_in >> type_id.inout ()) == 0)
    {
      // Could not demarshal the exception id, raise a local
      // CORBA::MARSHAL
      ACE_THROW (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                 CORBA::COMPLETED_YES));
    }
  
  if (is_system_exception)
    {
      CORBA::ULong minor = 0;
      CORBA::ULong completion = 0;
      if ((_tao_in >> minor) == 0 ||
          (_tao_in >> completion) == 0)
        ACE_THROW (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                   CORBA::COMPLETED_MAYBE));
      CORBA::SystemException* exception =
        TAO_Exceptions::create_system_exception (type_id.in ()
                                                 TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (exception == 0)
        {
          // @@ We should raise a CORBA::NO_MEMORY, but we ran out
          //    of memory already. We need a pre-allocated, TSS,
          //    CORBA::NO_MEMORY instance
          ACE_NEW (exception, CORBA::UNKNOWN);
        }
      exception->minor (minor);
      exception->completed (CORBA::CompletionStatus (completion));
      
      // Raise the exception.
      TAO_ENV_RAISE (exception);

      return;
    }

  // Match the exception interface repository id with the
  // exception in the exception list.
  // This is important to decode the exception.
  for (CORBA::ULong i = 0; i != exception_count; ++i)
    {
      if (ACE_OS::strcmp (type_id.in (), exception_data[i].id) != 0)
        continue;
          
      CORBA::Exception *exception = exception_data[i].alloc ();
          
      if (exception == 0)
        ACE_THROW (CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                     CORBA::COMPLETED_YES));
      exception->_tao_decode (_tao_in TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // Raise the exception.
      TAO_ENV_RAISE (exception);
          
      return;
    }
      
  // If we couldn't find the right exception, report it as
  // CORBA::UNKNOWN.
      
  // @@ It would seem like if the remote exception is a
  //    UserException we can assume that the request was
  //    completed.
  ACE_THROW (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE,
                             CORBA::COMPLETED_YES));
}

#endif /* TAO_HAS_AMI_CALLBACK */
