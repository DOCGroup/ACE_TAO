//
// $Id$
//
#include "Test_i.h"

One_Impl::One_Impl (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
One_Impl::get_string (void)
{
  return CORBA::string_dup ("One_Impl there!");
}

Two_Impl::Two_Impl (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
Two_Impl::get_string (void)
{
  return CORBA::string_dup ("One_Impl there!");
}


Three_Impl::Three_Impl (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
Three_Impl::get_string (void)
{
  return CORBA::string_dup ("Three_Impl there!");
}
