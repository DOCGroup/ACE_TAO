// -*- C++ -*-
// $Id$

#include "tao/Messaging/Messaging.h"
#include "tao/Messaging/Messaging_Loader.h"

#include "tao/Exception_Data.h"
#include "tao/debug.h"
#include "tao/ORB_Constants.h"
#include "tao/CDR.h"

#include "ace/Auto_Ptr.h"
#include "ace/OS_NS_string.h"


ACE_RCSID (Messaging,
           Messaging,
           "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_Messaging_Initializer::init (void)
{
  return ACE_Service_Config::process_directive (ace_svc_desc_TAO_Messaging_Loader);
}

void TAO_Messaging_Helper::
exception_holder_raise (TAO::Exception_Data *exception_data,
                        CORBA::ULong exception_count,
                        CORBA::Octet *marshaled_data,
                        CORBA::ULong marshaled_data_length,
                        CORBA::Boolean byte_order,
                        CORBA::Boolean is_system_exception
                        )
{
  TAO_InputCDR _tao_in ((const char*) marshaled_data,
                        marshaled_data_length,
                        byte_order);

  CORBA::String_var type_id;

  if ((_tao_in >> type_id.inout ()) == 0)
    {
      // Could not demarshal the exception id, raise a local
      // CORBA::MARSHAL
      throw ( ::CORBA::MARSHAL (TAO::VMCID,
                                 CORBA::COMPLETED_YES));
    }

  if (is_system_exception)
    {
      CORBA::ULong minor = 0;
      CORBA::ULong completion = 0;
      if ((_tao_in >> minor) == 0 ||
          (_tao_in >> completion) == 0)
        throw ( ::CORBA::MARSHAL (TAO::VMCID,
                                   CORBA::COMPLETED_MAYBE));

      CORBA::SystemException* exception =
        TAO::create_system_exception (type_id.in ());

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
      ACE_Auto_Basic_Ptr<CORBA::SystemException> e_ptr(exception);
      exception->_raise ();

      return;
    }

  // Match the exception interface repository id with the
  // exception in the exception list.
  // This is important to decode the exception.
  for (CORBA::ULong i = 0; i != exception_count; ++i)
    {
      if (ACE_OS::strcmp (type_id.in (), exception_data[i].id) != 0)
        continue;

      CORBA::Exception * const exception = exception_data[i].alloc ();

      if (exception == 0)
        throw ( ::CORBA::NO_MEMORY (TAO::VMCID,
                                     CORBA::COMPLETED_YES));
      exception->_tao_decode (_tao_in);

      // Raise the exception.
      ACE_Auto_Basic_Ptr<CORBA::Exception> e_ptr (exception);
      exception->_raise ();

      return;
    }

  // If we couldn't find the right exception, report it as
  // CORBA::UNKNOWN.

  // @@ It would seem like if the remote exception is a
  //    UserException we can assume that the request was
  //    completed.
  throw ( ::CORBA::UNKNOWN (TAO::VMCID,
                             CORBA::COMPLETED_YES));
}

TAO_END_VERSIONED_NAMESPACE_DECL
