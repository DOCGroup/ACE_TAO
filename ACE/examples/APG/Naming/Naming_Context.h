/* -*- C++ -*- */
#ifndef NAMING_CONTEXT_H
#define NAMING_CONTEXT_H

#include "ace/Naming_Context.h"
#include "ace/OS_NS_stdio.h"
#include "Name_Binding.h"
#include <memory>

// Listing 1 code/ch21
class Naming_Context : public ACE_Naming_Context
{
public:
  using inherited = ACE_Naming_Context;

  int rebind (const char *name_in,
              const char *value_in,
              const char *type_in = "")
  {
    return this->inherited::rebind (name_in, value_in, type_in);
  }

  int rebind (const char *name_in,
              float value_in,
              const char *type_in = "")
  {
    char buf[BUFSIZ];
    ACE_OS::sprintf (buf, "%2f", value_in);
    return this->inherited::rebind (name_in,
                                    (const char *)buf,
                                    type_in);
  }

  int rebind (const char *name_in,
              int value_in,
              const char *type_in = "")
  {
    char buf[BUFSIZ];
    ACE_OS::sprintf (buf, "%d", value_in );
    return this->inherited::rebind (name_in,
                                    (const char *)buf,
                                    type_in);
  }
  // Listing 1

  // Listing 2 code/ch21
  Name_Binding_Ptr fetch (char const *name)
  {
    ACE_NS_WString value;
    char *type = nullptr;

    int const status = this->resolve (name, value, type);
    std::unique_ptr<char[]> type_owner (type);

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
