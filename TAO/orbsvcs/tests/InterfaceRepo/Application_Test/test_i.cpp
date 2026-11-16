// -*- C++ -*-
#include "test_i.h"

#include <string>

CORBA::Boolean
inventory_i::getCDinfo (char const* artist,
                        char*& title,
                        CORBA::Float_out price)
{
  CORBA::Boolean in_stock {};
  price = 0.0f;
  std::string const name_key ("Beatles");
  std::string const name (artist);
  CORBA::String_var const tmp {title};
  ACE_UNUSED_ARG (tmp);

  if (name.find (name_key) != std::string::npos)
    {
      std::string const title_key ("Sgt. Pepper");
      std::string const working_title (title);

      if (working_title.find (title_key) != std::string::npos)
        {
          title =
            CORBA::string_dup ("Sgt. Pepper's Lonely Hearts Club Band");

          price = 13.49f;

          in_stock = true;
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
