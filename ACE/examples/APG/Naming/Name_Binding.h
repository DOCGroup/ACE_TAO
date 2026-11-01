/* -*- C++ -*- */
#ifndef NAME_BINDING_H
#define NAME_BINDING_H

#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_string.h"
#include "ace/Name_Space.h"
#include <memory>

struct Name_Binding_Type_Deleter
{
  void operator () (char *type) const
  {
    ACE_OS::free (type);
  }
};

// Listing 1 code/ch21
class Name_Binding
{
public:
  explicit Name_Binding (ACE_Name_Binding *entry)
    : name_ (entry->name_.char_rep ()),
      value_ (entry->value_.char_rep ()),
      type_ (ACE_OS::strdup (entry->type_))
  { }

  Name_Binding (ACE_NS_WString const &n,
                ACE_NS_WString const &v,
                char const *t)
    : name_ (n.char_rep ()), value_ (v.char_rep ()), type_ (ACE_OS::strdup (t))
  { }

  ~Name_Binding () = default;

  Name_Binding (Name_Binding const &) = delete;
  Name_Binding &operator = (Name_Binding const &) = delete;
  Name_Binding (Name_Binding &&) = delete;
  Name_Binding &operator = (Name_Binding &&) = delete;

  char *name () const
  { return this->name_.get (); }

  char *value () const
  { return this->value_.get (); }

  char const *type () const
  { return this->type_.get (); }

  int int_value () const
  { return ACE_OS::atoi (this->value ()); }

private:
  std::unique_ptr<char[]> name_;
  std::unique_ptr<char[]> value_;
  std::unique_ptr<char, Name_Binding_Type_Deleter> type_;
};

using Name_Binding_Ptr = std::unique_ptr<Name_Binding>;
// Listing 1

#endif /* NAME_BINDING_H */
