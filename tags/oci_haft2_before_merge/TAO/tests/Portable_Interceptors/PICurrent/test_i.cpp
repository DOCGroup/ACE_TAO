// -*- C++ -*-

#include "test_i.h"

ACE_RCSID (PICurrent,
           test_i,
           "$Id$")

test_i::test_i (PortableInterceptor::Current_ptr current,
                PortableInterceptor::SlotId id,
                CORBA::ORB_ptr orb)
  : current_ (PortableInterceptor::Current::_duplicate (current)),
    slot_id_ (id),
    orb_ (CORBA::ORB::_duplicate (orb))
{
}

test_i::~test_i (void)
{
}

void
test_i::invoke_me (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "Test method invoked.\n"));

  // @note "TSC" is "thread scope current"
  //       "RSC" is "request scope current"

  // ----------------------------------------------------

  // Verify that the following RSC->TSC->RSC copying scenario works:
  //
  // 1.    ServerRequestInterceptor::receive_request_service_contexts()
  //   a.       ServerRequestInfo::set_slot()
  //   b.       RSC->TSC shallow copy 
  // 2.    servant implementation invokes method on another server
  //   a.       TSC->RSC shallow copy
  //   b.       ClientRequestInterceptor::send_request()
  //     i.          ClientRequestInfo::get_slot()

  // By this point all of step 1 has occurred.  Step 2 will now
  // occur.
  PICurrentTest::test_var my_ref =
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Note that the invocation must occur through the object
  // reference to force the client request interceptor
  // (ClientRequestInterceptor2) to be invoked.  This assumes that
  // DIRECT collocation (and possibly THRU_POA collocation) is
  // disabled.
  my_ref->invoke_you (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // ----------------------------------------------------

  CORBA::Any_var retrieved_any;

  ACE_TRY
    {
      // Retrieve data placed into RSC PICurrent by the
      // receive_request_service_contexts() interception point, and
      // then copied into the TSC current.
      retrieved_any =
        this->current_->get_slot (this->slot_id_
                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (PortableInterceptor::InvalidSlot, ex)
    {
      ACE_PRINT_EXCEPTION (ex,
                           "Exception thrown in "
                           "test_i::invoke_me() when calling "
                           "Current::get_slot\n");

      ACE_DEBUG ((LM_DEBUG,
                  "Invalid slot: %u\n",
                  this->slot_id_));

      ACE_TRY_THROW (CORBA::INTERNAL ());
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unexpected exception\n");
    }
  ACE_ENDTRY;
  ACE_CHECK;

  CORBA::Long retrieved;
  if (retrieved_any.in() >>= retrieved)
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) Retrieved number <%d> from TSC.\n",
                retrieved));
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Problem extracting data from "
                  "CORBA::Any retrieved from TSC.\n"));

      ACE_THROW (CORBA::INTERNAL ());
    }
  // ----------------------------------------------------

  // Insert some data into the TSC PICurrent object.
  const char str[] = "Drink milk!";

  CORBA::Any data;

  data <<= str;

  ACE_TRY_EX (foo)
    {
      this->current_->set_slot (this->slot_id_,
                                data
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK_EX (foo);
    }
  ACE_CATCH (PortableInterceptor::InvalidSlot, ex)
    {
      ACE_PRINT_EXCEPTION (ex,
                           "Exception thrown in "
                           "test_i::invoke_me() when calling "
                           "Current::set_slot\n");

      ACE_DEBUG ((LM_DEBUG,
                  "Invalid slot: %u\n",
                  this->slot_id_));

      ACE_TRY_THROW_EX (CORBA::INTERNAL (), foo);
    }
  ACE_ENDTRY;
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) String \"%s\" inserted into TSC.\n",
              str));
}

void
test_i::invoke_you (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Nothing to be tested here.  This method is here just so that we
  // have a different method
}

void
test_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Server is shutting down.\n"));

  this->orb_->shutdown (0
                        ACE_ENV_ARG_PARAMETER);
}
