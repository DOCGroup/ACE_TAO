// -*- C++ -*-
// $Id$

#include "test_i.h"

ACE_RCSID(Application_Test, test_i, "$Id$")

CORBA::Boolean
inventory_i::getCDinfo (const char * artist,
                        char *& title,
                        CORBA::Float_out price
                        TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Boolean in_stock = 0;
  price = 0.0f;
  ACE_CString name_key ("Beatles");
  ACE_CString name (artist);

  if (name.strstr (name_key) != -1)
    {
      ACE_CString title_key ("Sgt. Pepper");
      ACE_CString working_title (title);

      if (working_title.strstr (title_key) != -1)
        {
          title =
            CORBA::string_dup ("Sgt. Pepper's Lonely Hearts Club Band");

          price = 13.49f;

          in_stock = 1;
        }
      else
        {
          title = CORBA::string_dup ("not found");
        }
    }
  else
    {
      title = CORBA::string_dup ("not found");
    }

  return in_stock;
}

