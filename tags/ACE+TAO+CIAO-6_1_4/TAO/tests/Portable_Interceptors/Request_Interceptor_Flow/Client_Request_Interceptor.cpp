// -*- C++ -*-
// $Id$

#include "Client_Request_Interceptor.h"
#include "testC.h"

#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/AnyTypeCode/AnyTypeCode_Adapter_Impl.h"
#include "ace/OS_NS_string.h"

Client_Request_Interceptor::Client_Request_Interceptor (const char *name)
  : Request_Interceptor (name)
{
}

void
Client_Request_Interceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  CORBA::Boolean client_side =
    this->client_side_test (ri);

  if (!client_side)
    return;  // Don't continue if the server side is being tested.

  ACE_DEBUG ((LM_INFO,
              "%C.send_request",
              this->name_.in ()));

  if (ACE_OS::strcmp ("CLIENT B", this->name_.in ()) == 0)
    {
      // Determine which test scenario we are in
      Dynamic::ParameterList_var paramlist =
        ri->arguments ();

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
              throw CORBA::NO_PERMISSION ();  // Expected exception.

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
    PortableInterceptor::ClientRequestInfo_ptr ri)
{

  CORBA::Boolean client_side =
    this->client_side_test (ri);

  if (!client_side)
    return;  // Don't continue if the server side is being tested.

  ACE_DEBUG ((LM_INFO,
              "%C.send_poll\n",
              this->name_.in ()));

  this->starting_interception_point_count_++;
}

void
Client_Request_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{

  CORBA::Boolean client_side =
    this->client_side_test (ri);

  if (!client_side)
    return;  // Don't continue if the server side is being tested.

  this->ending_interception_point_count_++;

  ACE_DEBUG ((LM_INFO,
              "%C.receive_reply",
              this->name_.in ()));

  if (ACE_OS::strcmp ("CLIENT B", this->name_.in ()) == 0)
    {
      // Determine which test scenario we are in
      Dynamic::ParameterList_var paramlist =
        ri->arguments ();

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
              throw CORBA::NO_PERMISSION ();  // Expected exception.

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
}

void
Client_Request_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{

  CORBA::Boolean client_side =
    this->client_side_test (ri);

  if (!client_side)
    return;  // Don't continue if the server side is being tested.

  this->ending_interception_point_count_++;

  ACE_DEBUG ((LM_INFO,
              "%C.receive_exception",
              this->name_.in ()));

  if (ACE_OS::strcmp ("CLIENT B", this->name_.in ()) == 0)
    {
      // Determine which test scenario we are in
      Dynamic::ParameterList_var paramlist =
        ri->arguments ();

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
              exception = ri->received_exception ();

              tc = exception->type ();

              id = tc->id ();

              if (ACE_OS::strcmp (id, "IDL:Test/X:1.0") == 0)
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
                  throw CORBA::NO_PERMISSION ();
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
}

void
Client_Request_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{

  CORBA::Boolean client_side =
    this->client_side_test (ri);

  if (!client_side)
    return;  // Don't continue if the server side is being tested.

  this->ending_interception_point_count_++;

  ACE_DEBUG ((LM_INFO,
              "%C.receive_other\n",
              this->name_.in ()));
}

CORBA::Boolean
Client_Request_Interceptor::client_side_test (
  PortableInterceptor::ClientRequestInfo_ptr info)
{
  CORBA::String_var op = info->operation ();

  return ACE_OS::strcmp (op.in (), "client_test") == 0 ? 1 : 0;
}
