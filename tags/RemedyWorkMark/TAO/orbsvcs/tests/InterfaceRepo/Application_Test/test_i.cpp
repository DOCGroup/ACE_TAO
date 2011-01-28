// -*- C++ -*-
// $Id$

#include "test_i.h"

#include "ace/SString.h"

CORBA::Boolean
inventory_i::getCDinfo (const char * artist,
                        char *& title,
                        CORBA::Float_out price)
{
  CORBA::Boolean in_stock = 0;
  price = 0.0f;
  ACE_CString name_key ("Beatles");
  ACE_CString name (artist);
  CORBA::String_var tmp = title;
  ACE_UNUSED_ARG (tmp);

  if (name.strstr (name_key) != ACE_CString::npos)
    {
      ACE_CString title_key ("Sgt. Pepper");
      ACE_CString working_title (title);

      if (working_title.strstr (title_key) != ACE_CString::npos)
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
