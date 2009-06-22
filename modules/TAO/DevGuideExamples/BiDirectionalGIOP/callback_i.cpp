// $Id$

#include "bidir_giop_pch.h"

#include "callback_i.h"
#include <iostream>

Callback_i::Callback_i (CORBA::ORB_ptr orb)
: orb_(CORBA::ORB::_duplicate(orb))
{
}

Callback_i::~Callback_i (void)
{
}

void Callback_i::shutdown ()
{
  std::cout << "Performing clean shutdown." << std::endl;
  const int wait = 0;
  orb_->shutdown(wait);
}

void Callback_i::callback_method ()
{
  std::cout << "Callback method called." << std::endl;
}

