// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/Concurrency_Service
//
// = FILENAME
//    Concurrency_Utils.cpp
//
// = AUTHOR
//    Torben Worm <tworm@cs.wustl.edu>
//
// ============================================================================

#include "ace/streams.h"
#include "orbsvcs/CosConcurrencyControlC.h"
#include "tao/corba.h"
#include "tao/debug.h"
#include "Concurrency_Utils.h"

ACE_RCSID (Concurrency,
           Concurrency_Utils,
           "$Id$")

// Default constructor

TAO_Concurrency_Server::TAO_Concurrency_Server (void)
{
}

// Constructor which takes an ORB and POA.

TAO_Concurrency_Server::TAO_Concurrency_Server (CORBA::ORB_ptr orb,
                                                PortableServer::POA_ptr poa)
{
  CORBA::Object_var obj = this->init (orb, poa);
}

// Function to initialize the concurrency server object under the
// passed orb and poa.

CORBA::Object_ptr
TAO_Concurrency_Server::init (CORBA::ORB_ptr orb,
                              PortableServer::POA_ptr poa)
{
  CORBA::Object_var obj;

  ACE_TRY_NEW_ENV
    {
      // @@ Huh?!?

      this->poa_= PortableServer::POA::_duplicate (poa);

      // Get the naming context ptr to NameService.
      //      ACE_TRY_CHECK;

      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId ("ConcurrencyService");

      poa->activate_object_with_id (id.in (),
                                    &this->lock_set_factory_
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Stringify the objref we'll be implementing, and print it to
      // stdout.  Someone will take that string and give it to a
      // client.  Then release the object.
      obj =
        poa->id_to_reference (id.in ()
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var str =
        orb->object_to_string (obj.in ()
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      
      ACE_DEBUG ((LM_DEBUG,
                  "listening as object <%s>\n",
                  str.in ()));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Concurrency Service");
      return CORBA::Object::_nil ();
    }
  ACE_ENDTRY;

  return obj._retn ();
}

int
TAO_Concurrency_Server::fini (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      PortableServer::ObjectId_var id =
        this->poa_->servant_to_id (&this->lock_set_factory_);

      this->poa_->deactivate_object (id.in ()
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level > 0)
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               ACE_TEXT ("TAO_Concurrency_Server"));
        }

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

CC_LockSetFactory *
TAO_Concurrency_Server::GetLockSetFactory(void)
{
  return &this->lock_set_factory_;
}

TAO_Concurrency_Server::~TAO_Concurrency_Server (void)
{
}
