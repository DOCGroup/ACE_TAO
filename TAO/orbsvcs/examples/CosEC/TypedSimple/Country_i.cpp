// -*- C++ -*-
#include "Country_i.h"

Country_i::Country_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Country_i::update_population (char const *country,
                              CORBA::Long population)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Country %C has population %d\n"),
              country, population));
}

CORBA::Object_ptr
Country_i::get_typed_consumer ()
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Country_i::get_typed_consumer called...\n")));

  Country_var const ret = this->_this ();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("...returning CORBA::Object_ptr for Country_i\n")));

  return Country::_duplicate (ret.in ());
}

void
Country_i::push (CORBA::Any const & /* data */)
{
}

void
Country_i::disconnect_push_consumer ()
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Country_i::disconnect_push_consumer called...\n")));

  // Deactivate the impl
  PortableServer::POA_var const t_poa =
    this->_default_POA ();

  PortableServer::ObjectId_var const t_id =
    t_poa->servant_to_id (this);

  t_poa->deactivate_object (t_id.in ());

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Country_i::disconnect_push_consumer, ")
              ACE_TEXT ("calling ORB shutdown...\n")));
  orb_->shutdown (false);
}
