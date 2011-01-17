// -*- C++ -*-
// $Id$

#include "Server_Request_Interceptor.h"
#include "testC.h"
#include "tao/AnyTypeCode/AnyTypeCode_Adapter_Impl.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "ace/OS_NS_string.h"

Server_Request_Interceptor::Server_Request_Interceptor (const char *name)
  : Request_Interceptor (name),
    scenario_ (0)
{
}

void
Server_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{

  CORBA::Boolean server_side =
    this->server_side_test (ri);

  if (!server_side)
    return;  // Don't continue if the client side is being tested.

  ACE_DEBUG ((LM_INFO,
              "%C.receive_request_service_contexts",
              this->name_.in ()));

  if (ACE_OS::strcmp ("SERVER B", this->name_.in ()) == 0)
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
          throw CORBA::NO_PERMISSION ();  // Expected exception.

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
    PortableInterceptor::ServerRequestInfo_ptr ri)
{

  CORBA::Boolean server_side =
    this->server_side_test (ri);

  if (!server_side)
    return;  // Don't continue if the client side is being tested.

  ACE_DEBUG ((LM_INFO,
              "%C.receive_request\n",
              this->name_.in ()));
}

void
Server_Request_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{

  CORBA::Boolean server_side =
    this->server_side_test (ri);

  if (!server_side)
    return;  // Don't continue if the client side is being tested.

  this->ending_interception_point_count_++;

  ACE_DEBUG ((LM_INFO,
              "%C.send_reply",
              this->name_.in ()));

  if (ACE_OS::strcmp ("SERVER B", this->name_.in ()) == 0)
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
                    "\nERROR: send_reply - Could not extract "
                    "arguments.\n"));
    }

  ACE_DEBUG ((LM_DEBUG,
              "\n"));
}

void
Server_Request_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{

  CORBA::Boolean server_side =
    this->server_side_test (ri);

  if (!server_side)
    return;  // Don't continue if the client side is being tested.

  this->ending_interception_point_count_++;

  ACE_DEBUG ((LM_INFO,
              "%C.send_exception",
              this->name_.in ()));

  if (ACE_OS::strcmp ("SERVER B", this->name_.in ()) == 0)
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
          const char *id = 0;

          switch (scenario)
            {
            case 4:
              exception = ri->sending_exception ();

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
    PortableInterceptor::ServerRequestInfo_ptr ri)
{

  CORBA::Boolean server_side =
    this->server_side_test (ri);

  if (!server_side)
    return;  // Don't continue if the client side is being tested.

  this->ending_interception_point_count_++;

  ACE_DEBUG ((LM_INFO,
              "%C.send_other",
              this->name_.in ()));
}

CORBA::Boolean
Server_Request_Interceptor::server_side_test (
  PortableInterceptor::ServerRequestInfo_ptr info)
{
  CORBA::String_var op = info->operation ();

  return ACE_OS::strcmp (op.in (), "server_test") == 0 ? 1 : 0;
}
