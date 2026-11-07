#include "Diamond_i.h"

char *
Top_i::shape ()
{
  return CORBA::string_dup ("a point.");
}

char *
Left_i::shape ()
{
  return CORBA::string_dup ("the left line");
}

char *
Left_i::color ()
{
  return CORBA::string_dup ("black");
}

char *
Right_i::shape ()
{
  return CORBA::string_dup ("the right line");
}

CORBA::Long
Right_i::width ()
{
  return 0;
}

char *
Buttom_i::shape ()
{
  return CORBA::string_dup ("a diamond");
}

char *
Buttom_i::color ()
{
  return CORBA::string_dup ("translucent");
}

CORBA::Long
Buttom_i::width ()
{
  return 100;
}

char *
Buttom_i::name ()
{
  return CORBA::string_dup ("Jubilee");
}
