//=============================================================================
/**
 *  @file    Concurrency_Utils.cpp
 *
 *  $Id$
 *
 *  @author Torben Worm <tworm@cs.wustl.edu>
 */
//=============================================================================


#include "orbsvcs/Log_Macros.h"
#include "ace/streams.h"
#include "orbsvcs/CosConcurrencyControlC.h"
#include "tao/debug.h"
#include "orbsvcs/Concurrency/Concurrency_Utils.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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

  try
    {
      // @@ Huh?!?

      this->poa_= PortableServer::POA::_duplicate (poa);

      // Get the naming context ptr to NameService.

      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId ("ConcurrencyService");

      poa->activate_object_with_id (id.in (),
                                    &this->lock_set_factory_);

      // Stringify the objref we'll be implementing, and print it to
      // stdout.  Someone will take that string and give it to a
      // client.  Then release the object.
      obj =
        poa->id_to_reference (id.in ());

      CORBA::String_var str =
        orb->object_to_string (obj.in ());

      ORBSVCS_DEBUG ((LM_DEBUG,
                  "listening as object <%s>\n",
                  str.in ()));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Concurrency Service");
      return CORBA::Object::_nil ();
    }

  return obj._retn ();
}

int
TAO_Concurrency_Server::fini (void)
{
  try
    {
      PortableServer::ObjectId_var id =
        this->poa_->servant_to_id (&this->lock_set_factory_);

      this->poa_->deactivate_object (id.in ());
    }
  catch (const CORBA::Exception& ex)
    {
      if (TAO_debug_level > 0)
        {
          ex._tao_print_exception (ACE_TEXT ("TAO_Concurrency_Server"));
        }

      return -1;
    }

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

TAO_END_VERSIONED_NAMESPACE_DECL
