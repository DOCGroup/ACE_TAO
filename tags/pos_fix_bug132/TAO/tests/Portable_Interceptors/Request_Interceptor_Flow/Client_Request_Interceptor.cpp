// -*- C++ -*-

#include "Client_Request_Interceptor.h"
#include "testC.h"

ACE_RCSID (Request_Interceptor_Flow,
           Client_Request_Interceptor,
           "$Id$")

Client_Request_Interceptor::Client_Request_Interceptor (const char *name)
  : Request_Interceptor (name)
{
}

void
Client_Request_Interceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr ri
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  TAO_ENV_ARG_DEFN;

  CORBA::Boolean client_side =
    this->client_side_test (ri, ACE_TRY_ENV);
  ACE_CHECK;

  if (!client_side)
    return;  // Don't continue if the server side is being tested.

  ACE_DEBUG ((LM_INFO,
              "%s.send_request",
              this->name_.in ()));

  if (ACE_OS_String::strcmp ("CLIENT B", this->name_.in ()) == 0)
    {
      // Determine which test scenario we are in
      Dynamic::ParameterList_var paramlist =
        ri->arguments (ACE_TRY_ENV);
      ACE_CHECK;

      Test::TestScenario scenario;
      CORBA::ULong i = 0;  // index -- explicitly used to avoid
                           // overloaded operator ambiguity.
      if (paramlist[i].argument >>= scenario)
        {
          switch (scenario)
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
      else
        ACE_ERROR ((LM_ERROR,
                    "\nERROR: send_request - Could not extract "
                    "arguments.\n"));
    }

  ACE_DEBUG ((LM_DEBUG,
              "\n"));

  this->starting_interception_point_count_++;
}

void
Client_Request_Interceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr ri
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ENV_ARG_DEFN;

  CORBA::Boolean client_side =
    this->client_side_test (ri, ACE_TRY_ENV);
  ACE_CHECK;

  if (!client_side)
    return;  // Don't continue if the server side is being tested.

  ACE_DEBUG ((LM_INFO,
              "%s.send_poll\n",
              this->name_.in ()));

  this->starting_interception_point_count_++;
}

void
Client_Request_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr ri
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ENV_ARG_DEFN;

  CORBA::Boolean client_side =
    this->client_side_test (ri, ACE_TRY_ENV);
  ACE_CHECK;

  if (!client_side)
    return;  // Don't continue if the server side is being tested.

  ACE_DEBUG ((LM_INFO,
              "%s.receive_reply",
              this->name_.in ()));

  if (ACE_OS_String::strcmp ("CLIENT B", this->name_.in ()) == 0)
    {
      // Determine which test scenario we are in
      Dynamic::ParameterList_var paramlist =
        ri->arguments (ACE_TRY_ENV);
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
                    "\nERROR: receive_reply - Could not extract "
                    "arguments.\n"));
    }

  ACE_DEBUG ((LM_DEBUG,
              "\n"));

  this->ending_interception_point_count_++;
}

void
Client_Request_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr ri
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  TAO_ENV_ARG_DEFN;

  CORBA::Boolean client_side =
    this->client_side_test (ri, ACE_TRY_ENV);
  ACE_CHECK;

  if (!client_side)
    return;  // Don't continue if the server side is being tested.

  ACE_DEBUG ((LM_INFO,
              "%s.receive_exception",
              this->name_.in ()));

  if (ACE_OS_String::strcmp ("CLIENT B", this->name_.in ()) == 0)
    {
      // Determine which test scenario we are in
      Dynamic::ParameterList_var paramlist =
        ri->arguments (ACE_TRY_ENV);
      ACE_CHECK;

      Test::TestScenario scenario;
      CORBA::ULong i = 0;  // index -- explicitly used to avoid
                           // overloaded operator ambiguity.
      if (paramlist[i].argument >>= scenario)
        {
          CORBA::Any_var exception;
          CORBA::TypeCode_var tc;
          const char * id = 0;

          switch (scenario)
            {
            case 4:
              exception = ri->received_exception (ACE_TRY_ENV);
              ACE_CHECK;

              tc = exception->type ();

              id = tc->id (ACE_TRY_ENV);
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
                              "\nERROR: receive_exception - Received "
                              "unexpected exception!\n"));
                }
              break;
            default:
              break;
            }
        }
      else
        ACE_ERROR ((LM_ERROR,
                    "\nERROR: receive_exception - Could not extract "
                    "arguments.\n"));
    }

  ACE_DEBUG ((LM_DEBUG,
              "\n"));

  this->ending_interception_point_count_++;
}

void
Client_Request_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr ri
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  TAO_ENV_ARG_DEFN;

  CORBA::Boolean client_side =
    this->client_side_test (ri, ACE_TRY_ENV);
  ACE_CHECK;

  if (!client_side)
    return;  // Don't continue if the server side is being tested.

  ACE_DEBUG ((LM_INFO,
              "%s.receive_other\n",
              this->name_.in ()));

  this->ending_interception_point_count_++;
}

CORBA::Boolean
Client_Request_Interceptor::client_side_test (
  PortableInterceptor::ClientRequestInfo_ptr info,
  CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::String_var op = info->operation (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  return ACE_OS_String::strcmp (op.in (), "client_test") == 0 ? 1 : 0;
}
