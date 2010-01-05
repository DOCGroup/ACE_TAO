// -*- C++ -*-
//
// $Id$

#include "Country_i.h"

// Implementation skeleton constructor
Country_i::Country_i (CORBA::ORB_ptr orb) :
  orb_ (CORBA::ORB::_duplicate (orb))
{
}

// Implementation skeleton destructor
Country_i::~Country_i (void)
{
}

void Country_i::update_population (const char * country,
                                   CORBA::Long population)

{
  //Add your implementation here
  ACE_DEBUG ((LM_DEBUG, "Country %s has population %d\n",
              country, population));
}

CORBA::Object_ptr
Country_i::get_typed_consumer (void)

{
  //Add your implementation here
  ACE_DEBUG ((LM_DEBUG, "Country_i::get_typed_consumer called...\n"));

  Country_var ret = this->_this ();

  ACE_DEBUG ((LM_DEBUG, "...returning CORBA::Object_ptr for Country_i\n"));

  return Country::_duplicate(ret.in ());
}

void
Country_i::push (const CORBA::Any & /* data */
                 )

{
  //Add your implementation here
}

void
Country_i::disconnect_push_consumer (void)

{
  //Add your implementation here
  ACE_DEBUG ((LM_DEBUG, "Country_i::disconnect_push_consumer called...\n"));

  // Deactivate the impl
  PortableServer::POA_var t_poa =
    this->_default_POA ();

  PortableServer::ObjectId_var t_id =
    t_poa->servant_to_id (this);

  t_poa->deactivate_object (t_id.in ());

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Country_i::disconnect_push_consumer, ")
              ACE_TEXT ("calling ORB shutdown...\n")));
  orb_->shutdown (0);
}
