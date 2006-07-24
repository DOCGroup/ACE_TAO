// $Id$

#include "UDP_i.h"

ACE_RCSID(UDP, UDP_i, "")

// Constructor
UDP_i::UDP_i (void)
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
UDP_i::orb (CORBA::ORB_ptr o)
{
  this->orb_ = CORBA::ORB::_duplicate (o);
}


void
UDP_i::invoke (const char * client_name,
               UDP_ptr udpHandler,
               CORBA::Long request_id
               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      ACE_DEBUG ((LM_DEBUG,
                  "UDP_i::invoke: name = %s request id = %d\n",
                  client_name,
                  request_id));
      ACE_DEBUG ((LM_DEBUG,
                  "."));

      CORBA::Long last_request_id = 0;
      if (request_id_table_.find (client_name,
                                  last_request_id) != -1)
        {
          if (last_request_id + 1 != request_id)
            ACE_DEBUG ((LM_DEBUG,
                        "UDP_i::invoke: failure on %s expected id = %d, got %d\n",
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
                              request_id
                              ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

    }
  ACE_CATCHANY
    {
       ACE_DEBUG ((LM_DEBUG,
                   "UDP_i::svc: Received exception\n"));
    }
  ACE_ENDTRY;
}

// Shutdown.

void
UDP_i::shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "%s\n",
              "UDP_i is shutting down"));

  // Instruct the ORB to shutdown.
  this->orb_->shutdown ();
}

