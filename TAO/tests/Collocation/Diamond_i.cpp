#include "Diamond_i.h"

Top_i::Top_i () = default;

Top_i::~Top_i () = default;

char *
Top_i::shape ()
{
  return CORBA::string_dup ("a point.");
}

Left_i::Left_i () = default;

Left_i::~Left_i () = default;

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

Right_i::Right_i () = default;

Right_i::~Right_i () = default;

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

Buttom_i::Buttom_i () = default;

Buttom_i::~Buttom_i () = default;

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
