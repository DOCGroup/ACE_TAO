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

char *
Top_i::shape (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
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

char *
Left_i::shape (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  return CORBA::string_dup ("the left line");
}

char *
Left_i::color (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
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

char *
Right_i::shape (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  return CORBA::string_dup ("the right line");
}

char *
Right_i::color (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  return CORBA::string_dup ("red");
  //
}

CORBA::Long
Right_i::width (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
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

char *
Buttom_i::shape (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  return CORBA::string_dup ("a diamond");
}

char *
Buttom_i::color (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  return CORBA::string_dup ("translucent");
}

CORBA::Long
Buttom_i::width (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  return 100;
}

char *
Buttom_i::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  return CORBA::string_dup ("Jubilee");
}
