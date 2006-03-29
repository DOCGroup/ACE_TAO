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
                                   CORBA::Long population
                                   ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))

{
  //Add your implementation here
  ACE_DEBUG ((LM_DEBUG, "Country %s has population %d\n",
              country, population));  
}
  
CORBA::Object_ptr
Country_i::get_typed_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))

{
  //Add your implementation here
  ACE_DEBUG ((LM_DEBUG, "Country_i::get_typed_consumer called...\n"));

  Country_var ret = this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  ACE_DEBUG ((LM_DEBUG, "...returning CORBA::Object_ptr for Country_i\n"));

  return Country::_duplicate(ret.in ());
}
  
void
Country_i::push (const CORBA::Any & /* data */
                 ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosEventComm::Disconnected))

{
  //Add your implementation here
}
  
void
Country_i::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))

{
  //Add your implementation here
  ACE_DEBUG ((LM_DEBUG, "Country_i::disconnect_push_consumer called...\n"));

  // Deactivate the impl
  PortableServer::POA_var t_poa =
    this->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  
  PortableServer::ObjectId_var t_id =
    t_poa->servant_to_id (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  t_poa->deactivate_object (t_id.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Country_i::disconnect_push_consumer, ")
              ACE_TEXT ("calling ORB shutdown...\n")));
  orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
