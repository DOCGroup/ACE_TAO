// $Id$

#include "UDP_i.h"

ACE_RCSID(UDP, UDP_i, "")

// Constructor
UDP_i::UDP_i (void)
: messages_count_ (0),
  wrong_messages_count_ (0)
{
  // no-op
}

// Destructor

UDP_i::~UDP_i (void)
{
  // no-op
}

// Set the ORB pointer.

void
UDP_i::orb (CORBA::ORB_ptr orb)
{
  this->orb_ = CORBA::ORB::_duplicate (orb);
}


void
UDP_i::setResponseHandler (UDP_ptr udpHandler,
                           CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      this->responseHandler_ = UDP::_duplicate (udpHandler);
    }
  ACE_CATCHANY
    {
       ACE_DEBUG ((LM_DEBUG,
                   "UDP_i::svc: Received exception\n"));
    }
  ACE_ENDTRY;
}


void
UDP_i::invoke (const char * client_name,
               CORBA::Long request_id,
               CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      //ACE_DEBUG ((LM_DEBUG,
      //            "UDP_i::invoke: name = %s request id = %d\n",
      //            client_name,
      //            request_id));
      //ACE_DEBUG ((LM_DEBUG,
      //            "."));

      ++messages_count_;

      CORBA::Long last_request_id = 0;
      if (request_id_table_.find (client_name,
                                  last_request_id) != -1)
        {
          if (last_request_id + 1 != request_id)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "UDP_i::invoke: failure on %s expected id = %d, got %d\n",
                          client_name,
                          last_request_id+1,
                          request_id));

              wrong_messages_count_++;
            }
        }

      request_id_table_.rebind (client_name,
                                request_id);

      if (!CORBA::is_nil (responseHandler_))
        {
          responseHandler_->invoke (client_name,
                                    request_id,
                                    ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }

    }
  ACE_CATCHANY
    {
       ACE_DEBUG ((LM_DEBUG,
                   "UDP_i::invoke: Received exception\n"));
    }
  ACE_ENDTRY;
}


void
UDP_i::reset (const char * client_name,
              CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      // ACE_DEBUG ((LM_DEBUG,
      //            "UDP_i::reset: invoked\n"));

      request_id_table_.rebind (client_name,
                                0);
      if (!CORBA::is_nil (responseHandler_))
        {
          responseHandler_->reset (client_name,
                                   ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
       ACE_DEBUG ((LM_DEBUG,
                   "UDP_i::reset: Received exception\n"));
    }
  ACE_ENDTRY;
}

// Shutdown.

void 
UDP_i::shutdown (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "%s\n",
              "UDP_i is shutting down"));

  ACE_TRY
    {
      // Instruct the ORB to shutdown.
      this->orb_->shutdown (0, ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Catched exception: orb->run");
    }
  ACE_ENDTRY;
}


ACE_UINT32
UDP_i::getWrongMessagesCount ()
{
  ACE_UINT32 tmp = wrong_messages_count_;
  wrong_messages_count_ = 0;

  return tmp;
}


ACE_UINT32
UDP_i::getMessagesCount ()
{
  ACE_UINT32 tmp = messages_count_;
  messages_count_ = 0;

  return tmp;
}
