// -*- C++ -*-

#include "Server_Request_Interceptor.h"

ACE_RCSID (Request_Interceptor_Flow,
           Server_Request_Interceptor,
           "$Id$")

#include "testC.h"

Server_Request_Interceptor::Server_Request_Interceptor (const char *name)
  : Request_Interceptor (name),
    scenario_ (0)
{
}

void
Server_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{

  CORBA::Boolean server_side =
    this->server_side_test (ri ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!server_side)
    return;  // Don't continue if the client side is being tested.

  ACE_DEBUG ((LM_INFO,
              "%s.receive_request_service_contexts",
              this->name_.in ()));

  if (ACE_OS_String::strcmp ("SERVER B", this->name_.in ()) == 0)
    {
      this->scenario_++;

      switch (this->scenario_)
        {
        case 2:
          // We can only throw a CORBA::SystemException or a
          // PortableInteceptor::ForwardRequest exception due to
          // the restricted exception specification dictated by
          // the IDL for request interceptors.  As such, an
          // arbitrary CORBA::SystemException was chosen
          // (NO_PERMISSION, in this case).
          ACE_DEBUG ((LM_DEBUG,
                      " raised CORBA::NO_PERMISSION exception\n"));
          ACE_THROW (CORBA::NO_PERMISSION ());  // Expected exception.

        default:
          break;
        }
    }

  ACE_DEBUG ((LM_DEBUG,
              "\n"));

  this->starting_interception_point_count_++;
}

void
Server_Request_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{

  CORBA::Boolean server_side =
    this->server_side_test (ri ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!server_side)
    return;  // Don't continue if the client side is being tested.

  ACE_DEBUG ((LM_INFO,
              "%s.receive_request\n",
              this->name_.in ()));
}

void
Server_Request_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  CORBA::Boolean server_side =
    this->server_side_test (ri ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!server_side)
    return;  // Don't continue if the client side is being tested.

  this->ending_interception_point_count_++;

  ACE_DEBUG ((LM_INFO,
              "%s.send_reply",
              this->name_.in ()));

  if (ACE_OS_String::strcmp ("SERVER B", this->name_.in ()) == 0)
    {
      // Determine which test scenario we are in
      Dynamic::ParameterList_var paramlist =
        ri->arguments (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      Test::TestScenario scenario;
      CORBA::ULong i = 0;  // index -- explicitly used to avoid
                           // overloaded operator ambiguity.
      if (paramlist[i].argument >>= scenario)
        {
          switch (scenario)
            {
            case 3:
              // We can only throw a CORBA::SystemException or a
              // PortableInteceptor::ForwardRequest exception due to
              // the restricted exception specification dictated by
              // the IDL for request interceptors.  As such, an
              // arbitrary CORBA::SystemException was chosen
              // (NO_PERMISSION, in this case).
              ACE_DEBUG ((LM_DEBUG,
                          " raised CORBA::NO_PERMISSION exception\n"));
              ACE_THROW (CORBA::NO_PERMISSION ());  // Expected exception.

            default:
              break;
            }
        }
      else
        ACE_ERROR ((LM_ERROR,
                    "\nERROR: send_reply - Could not extract "
                    "arguments.\n"));
    }

  ACE_DEBUG ((LM_DEBUG,
              "\n"));
}

void
Server_Request_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{

  CORBA::Boolean server_side =
    this->server_side_test (ri ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!server_side)
    return;  // Don't continue if the client side is being tested.

  this->ending_interception_point_count_++;

  ACE_DEBUG ((LM_INFO,
              "%s.send_exception",
              this->name_.in ()));

  if (ACE_OS_String::strcmp ("SERVER B", this->name_.in ()) == 0)
    {
      // Determine which test scenario we are in
      Dynamic::ParameterList_var paramlist =
        ri->arguments (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      Test::TestScenario scenario;
      CORBA::ULong i = 0;  // index -- explicitly used to avoid
                           // overloaded operator ambiguity.
      if (paramlist[i].argument >>= scenario)
        {
          CORBA::Any_var exception;
          CORBA::TypeCode_var tc;
          const char *id = 0;

          switch (scenario)
            {
            case 4:
              exception = ri->sending_exception (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_CHECK;

              tc = exception->type ();

              id = tc->id (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_CHECK;

              if (ACE_OS_String::strcmp (id, "IDL:Test/X:1.0") == 0)
                {
                  // We can only throw a CORBA::SystemException or a
                  // PortableInteceptor::ForwardRequest exception due
                  // to the restricted exception specification
                  // dictated by the IDL for request interceptors.  As
                  // such, an arbitrary CORBA::SystemException was
                  // chosen (NO_PERMISSION, in this case).
                  ACE_DEBUG ((LM_DEBUG,
                              " transformed Test::X exception to "
                              "CORBA::NO_PERMISSION exception\n"));

                  // Expected exception.
                  ACE_THROW (CORBA::NO_PERMISSION ());
                }
              else
                {
                  ACE_ERROR ((LM_ERROR,
                              "\nERROR: send_exception - Received "
                              "unexpected exception!\n"));
                }
              break;
            default:
              break;
            }
        }
      else
        ACE_ERROR ((LM_ERROR,
                    "\nERROR: send_exception - Could not extract "
                    "arguments.\n"));
    }

  ACE_DEBUG ((LM_DEBUG,
              "\n"));
}

void
Server_Request_Interceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{

  CORBA::Boolean server_side =
    this->server_side_test (ri ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!server_side)
    return;  // Don't continue if the client side is being tested.

  this->ending_interception_point_count_++;

  ACE_DEBUG ((LM_INFO,
              "%s.send_other",
              this->name_.in ()));
}

CORBA::Boolean
Server_Request_Interceptor::server_side_test (
  PortableInterceptor::ServerRequestInfo_ptr info
  ACE_ENV_ARG_DECL)
{
  CORBA::String_var op = info->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return ACE_OS_String::strcmp (op.in (), "server_test") == 0 ? 1 : 0;
}
