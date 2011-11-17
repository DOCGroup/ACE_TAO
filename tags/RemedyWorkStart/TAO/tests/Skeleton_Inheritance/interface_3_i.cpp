// $Id$
#include "interface_3_i.h"

interface_3_i::interface_3_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

// INTERFACE 1
char *
interface_3_i::operation_1 (void)
{
  return CORBA::string_dup ("Invoked operation one");
}

::CORBA::Long
interface_3_i::attribute_1 (void)
{
  return 10;
}

void
interface_3_i::attribute_1 (::CORBA::Long attribute_1)
{
  ACE_UNUSED_ARG (attribute_1);
}

// INTERFACE 2

char *
interface_3_i::operation_2 (void)
{
  return CORBA::string_dup ("Invoked operation two");
}

char *
interface_3_i::attribute_2 (void)
{
  return CORBA::string_dup ("Returning attribute_2");
}

//INTERFACE 3

char *
interface_3_i::operation_3 (void)
{
  return CORBA::string_dup ("Invoked operation three");
}

void
interface_3_i::shutdown (void)
{
  this->orb_->shutdown ();
}

::CORBA::Short
interface_3_i::attribute_3 (void)
{
  return 10;
}

void
interface_3_i::attribute_3 (::CORBA::Short attribute_3)
{
  ACE_UNUSED_ARG (attribute_3);
}


