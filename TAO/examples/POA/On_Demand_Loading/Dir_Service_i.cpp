// $Id$
//=============================================================================
//
// = FILENAME
//     Dir_Service_i.cpp
//
// = DESCRIPTION
//     Servant - implementation of the Directory Service object.
//     Provides directory information.
//
// = AUTHOR
//     Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
//=============================================================================

#define ACE_BUILD_SVC_EXPORT
 
#include "Dir_Service_i.h"
#include "ace/OS.h"

ACE_RCSID(On_Demand_Loading, Dir_Service_i, "$Id$")

// Initialization.

Dir_Service_i::Dir_Service_i (CORBA::ORB_ptr orb,
                              PortableServer::POA_ptr poa)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa))
   {}

// Wrapping up, cleaning up if any.

Dir_Service_i::~Dir_Service_i (void)
{}

// Return the Default POA of this Servant

PortableServer::POA_ptr 
Dir_Service_i::_default_POA (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

// The telephone number information is provided.

CORBA::Long
Dir_Service_i::tele_number (const char *name,
                            CORBA::Environment &env)
{
  ACE_UNUSED_ARG (name);
  ACE_UNUSED_ARG (env);

  ACE_DEBUG ((LM_DEBUG,
              "Providing required information...\n"));

  // The telephone number corresponding to the name is returned.
   return 9354215;
}

// Ending the session with a word of thanks.

void 
Dir_Service_i::end_note (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  ACE_DEBUG ((LM_DEBUG,
              "Thanks for using Directory Service!\n"));
}

// The area code information is displayed.

void 
Dir_Service_i::area_codes_info (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  ACE_DEBUG ((LM_DEBUG,
              "Some area_codes:\n"
              " Indianapolis  812\n"
               "St.Louis  314\n"
               "Sunnyvale  408\n"));

}

// Shuts down the servant.

void 
Dir_Service_i::shutdown (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  this->orb_->shutdown ();
}

// This is the point of entry into this library.

extern "C" ACE_Svc_Export PortableServer::Servant create_dir_service (CORBA::ORB_ptr orb,
                                                                       PortableServer::POA_ptr poa);

// The servant pointer is returned which will be of Base class type. The binding to Dir_Service 
//servant will happen at run-time.

PortableServer::Servant
create_dir_service (CORBA::ORB_ptr orb,
                    PortableServer::POA_ptr poa)
{
  PortableServer::Servant servant;

  ACE_NEW_RETURN (servant,
                  Dir_Service_i (orb,
                                 poa),
                  0);
  return servant;
}


