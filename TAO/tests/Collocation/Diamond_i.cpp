#include "Diamond_i.h"

Top_i::Top_i ()
{
}

Top_i::~Top_i ()
{
}

char *
Top_i::shape (void)
{
  return CORBA::string_dup ("a point.");
}

Left_i::Left_i ()
{
}

Left_i::~Left_i ()
{
}

char *
Left_i::shape (void)
{
  return CORBA::string_dup ("the left line");
}

char *
Left_i::color (void)
{
  return CORBA::string_dup ("black");
}

Right_i::Right_i ()
{
}

Right_i::~Right_i ()
{
}

char *
Right_i::shape (void)
{
  return CORBA::string_dup ("the right line");
}

char *
Right_i::color (void)
{
  return CORBA::string_dup ("red");
  //
}

CORBA::Long
Right_i::width (void)
{
  return 0;
}

Buttom_i::Buttom_i ()
{
}

Buttom_i::~Buttom_i ()
{
}

char *
Buttom_i::shape (void)
{
  return CORBA::string_dup ("a diamond");
}

char *
Buttom_i::color (void)
{
  return CORBA::string_dup ("translucent");
}

CORBA::Long
Buttom_i::width (void)
{
  return 100;
}

char *
Buttom_i::name (void)
{
  return CORBA::string_dup ("Jubilee");
}
