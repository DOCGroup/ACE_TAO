//
// $Id$
//
#include "Factory.h"
#include "Simple.h"

Factory::Factory (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

Test::Simple_ptr
Factory::create_simple_object (void)
{
  Simple *simple_impl;
  ACE_NEW_THROW_EX (simple_impl,
                    Simple,
                    CORBA::NO_MEMORY ());

  PortableServer::ServantBase_var owner_transfer(simple_impl);

  return simple_impl->_this ();
}

void
Factory::shutdown (void)
{
  this->orb_->shutdown (0);
}
