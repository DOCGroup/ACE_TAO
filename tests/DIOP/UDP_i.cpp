// $Id$

#include "UDP_i.h"

// Constructor
UDP_i::UDP_i (CORBA::ORB_ptr o) :
  orb_(CORBA::ORB::_duplicate (o))
{
  // no-op
}

// Destructor

UDP_i::~UDP_i (void)
{
  // no-op
}

void
UDP_i::invoke (const char * client_name,
               UDP_ptr udpHandler,
               CORBA::Long request_id)
{
  try
    {
      ACE_DEBUG ((LM_DEBUG,
                  "UDP_i::invoke: name = %C request id = %d.\n",
                  client_name,
                  request_id));

      CORBA::Long last_request_id = 0;
      if (request_id_table_.find (client_name,
                                  last_request_id) != -1)
        {
          if (last_request_id + 1 != request_id)
            ACE_ERROR ((LM_ERROR,
                        "UDP_i::invoke: failure on %C expected id = %d, got %d\n",
                        client_name,
                        last_request_id+1,
                        request_id));
        }

      request_id_table_.rebind (client_name,
                                request_id);

      if (!CORBA::is_nil (udpHandler))
        {
          udpHandler->invoke (client_name,
                              UDP::_nil (),
                              request_id);
        }

    }
  catch (const CORBA::Exception&)
    {
       ACE_ERROR ((LM_ERROR,
                   "UDP_i::svc: Received exception\n"));
    }
}

// Shutdown.

void
UDP_i::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "UDP_i is shutting down\n"));

  // Instruct the ORB to shutdown.
  this->orb_->shutdown ();
}

