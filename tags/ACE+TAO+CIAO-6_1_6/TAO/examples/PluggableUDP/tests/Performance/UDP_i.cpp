// $Id$

#include "UDP_i.h"



// Constructor
UDP_i::UDP_i (void)
  : messages_count_ (0)
  , wrong_messages_count_ (0)
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
UDP_i::setResponseHandler (UDP_ptr udpHandler)
{
  if (CORBA::is_nil (udpHandler))
    ACE_DEBUG ((LM_DEBUG,
                "response handler is nil\n"));

  this->responseHandler_ = UDP::_duplicate (udpHandler);
}

void
UDP_i::invoke (const char * client_name,
               CORBA::Long request_id)
{
  try
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
                          "(%P) UDP_i::invoke: failure on %s expected id = %d, got %d\n",
                          client_name,
                          last_request_id+1,
                          request_id));

              wrong_messages_count_++;
            }
        }

      request_id_table_.rebind (client_name,
                                request_id);

      if (!CORBA::is_nil (responseHandler_.in ()))
        {
          responseHandler_->invoke (client_name,
                                    request_id);
        }

    }
  catch (const CORBA::Exception&)
    {
       ACE_DEBUG ((LM_DEBUG,
                   "UDP_i::invoke: Received exception\n"));
    }
}

void
UDP_i::reset (const char * client_name)
{
  try
    {
      // ACE_DEBUG ((LM_DEBUG,
      //            "UDP_i::reset: invoked\n"));

      request_id_table_.rebind (client_name,
                                0);
      if (!CORBA::is_nil (responseHandler_.in ()))
        {
          responseHandler_->reset (client_name);
        }
    }
  catch (const CORBA::Exception&)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "UDP_i::reset: Received exception\n"));
    }
}

// Shutdown.

void
UDP_i::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "%s\n",
              "UDP_i is shutting down"));

  try
    {
      // Instruct the ORB to shutdown.
      this->orb_->shutdown (0);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception: orb->shutdown");
    }
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

