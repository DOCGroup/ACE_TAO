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
UDP_i::setResponseHandler (UDP_ptr udpHandler
                           ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (CORBA::is_nil (udpHandler))
    ACE_DEBUG ((LM_DEBUG,
                "response handler is nil\n"));

  this->responseHandler_ = UDP::_duplicate (udpHandler);
}

void
UDP_i::invoke (const char * client_name,
               CORBA::Long request_id
               ACE_ENV_ARG_DECL)
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

      if (!CORBA::is_nil (responseHandler_.in ()))
        {
          responseHandler_->invoke (client_name,
                                    request_id
                                    ACE_ENV_ARG_PARAMETER);
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
UDP_i::reset (const char * client_name
              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      // ACE_DEBUG ((LM_DEBUG,
      //            "UDP_i::reset: invoked\n"));

      request_id_table_.rebind (client_name,
                                0);
      if (!CORBA::is_nil (responseHandler_.in ()))
        {
          responseHandler_->reset (client_name
                                   ACE_ENV_ARG_PARAMETER);
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
UDP_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "%s\n",
              "UDP_i is shutting down"));

  ACE_TRY
    {
      // Instruct the ORB to shutdown.
      this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception: orb->run");
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Manager_Ex<CORBA::String_var, int, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<CORBA::String_var, int, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<CORBA::String_var, int, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<CORBA::String_var, int, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<CORBA::String_var, int>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Manager_Ex<CORBA::String_var, int, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<CORBA::String_var, int, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<CORBA::String_var, int, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<CORBA::String_var, int, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<CORBA::String_var, int>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
