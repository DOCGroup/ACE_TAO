/* -*- C++ -*- */
#ifndef NAMING_CONTEXT_H
#define NAMING_CONTEXT_H

#include <cstdio>
#include <memory>

#include "ace/Naming_Context.h"
#include "Name_Binding.h"

// Listing 1 code/ch21
class Naming_Context : public ACE_Naming_Context
{
public:
  using inherited = ACE_Naming_Context;

  int rebind (char const *name_in,
              char const *value_in,
              char const *type_in = "")
  {
    return this->inherited::rebind (name_in, value_in, type_in);
  }

  int rebind (char const *name_in,
              float value_in,
              char const *type_in = "")
  {
    char buffer[BUFSIZ] {};
    std::snprintf (buffer, sizeof (buffer), "%2f", value_in);
    return this->inherited::rebind (name_in, buffer, type_in);
  }

  int rebind (char const *name_in,
              int value_in,
              char const *type_in = "")
  {
    char buffer[BUFSIZ] {};
    std::snprintf (buffer, sizeof (buffer), "%d", value_in);
    return this->inherited::rebind (name_in, buffer, type_in);
  }
  // Listing 1

  // Listing 2 code/ch21
  Name_Binding_Ptr fetch (char const *name)
  {
    ACE_NS_WString value {};
    char *type {};

    int const status = this->resolve (name, value, type);
    std::unique_ptr<char, Name_Binding_Type_Deleter> type_owner {type};

    if (status != 0 || value.length () < 1)
      {
        return nullptr;
      }

    return std::make_unique<Name_Binding> (ACE_NS_WString (name),
                                           value,
                                           type_owner.get ());
  }
// Listing 2
};

#endif /* NAMING_CONTEXT_H */
