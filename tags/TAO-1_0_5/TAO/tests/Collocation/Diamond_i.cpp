// $Id$

#include "Diamond_i.h"

Top_i::Top_i ()
{
  // no-op.
}

Top_i::~Top_i ()
{
  // No-op.
}

CORBA::String
Top_i::shape (CORBA::Environment &)
{
  return CORBA::string_dup ("a point.");
}

Left_i::Left_i ()
{
  // No-op.
}

Left_i::~Left_i ()
{
  // No-op.
}

CORBA::String
Left_i::shape (CORBA::Environment &)
{
  return CORBA::string_dup ("the left line");
}

CORBA::String
Left_i::color (CORBA::Environment &)
{
  return CORBA::string_dup ("black");
};

Right_i::Right_i ()
{
  // no-op.
}

Right_i::~Right_i ()
{
  // no-op.
}

CORBA::String
Right_i::shape (CORBA::Environment &)
{
  return CORBA::string_dup ("the right line");
}

CORBA::String
Right_i::color (CORBA::Environment &)
{
  return CORBA::string_dup ("red");
  //
}

CORBA::Long
Right_i::width (CORBA::Environment &)
{
  return 0;
}

Buttom_i::Buttom_i ()
{
  // no-op.
}

Buttom_i::~Buttom_i ()
{
  // no-op.
}

CORBA::String
Buttom_i::shape (CORBA::Environment &)
{
  return CORBA::string_dup ("a diamond");
}

CORBA::String
Buttom_i::color (CORBA::Environment &)
{
  return CORBA::string_dup ("translucent");
}

CORBA::Long
Buttom_i::width (CORBA::Environment &)
{
  return 100;
}

CORBA::String
Buttom_i::name (CORBA::Environment &)
{
  return CORBA::string_dup ("Jubilee");
}
