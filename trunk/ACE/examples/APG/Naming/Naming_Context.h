/* -*- C++ -*- */
// $Id$

#ifndef NAMING_CONTEXT_H
#define NAMING_CONTEXT_H

#include "ace/Naming_Context.h"
#include "ace/OS_NS_stdio.h"
#include "Name_Binding.h"

// Listing 1 code/ch21
class Naming_Context : public ACE_Naming_Context
{
public:
  typedef ACE_Naming_Context inherited;

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
  Name_Binding *fetch (const char *name)
  {
    ACE_NS_WString value;
    char *type = 0;

    if (this->resolve (name, value, type) != 0 ||
        value.length () < 1)
      {
        return 0;
      }

    Name_Binding *rval =
      new Name_Binding (ACE_NS_WString (name),
                        value,
                        type);
    delete [] type;

    return rval;
  }
// Listing 2
};

#endif /* NAMING_CONTEXT_H */
