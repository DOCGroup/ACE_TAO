/* -*- C++ -*- */
// $Id$

#ifndef NAME_BINDING_H
#define NAME_BINDING_H

#include "ace/Auto_Ptr.h"
#include "ace/Name_Space.h"

// Listing 1 code/ch21
class Name_Binding
{
public:
  Name_Binding (ACE_Name_Binding *entry)
  {
    this->name_ = entry->name_.char_rep ();
    this->value_ = entry->value_.char_rep ();
    this->type_ = ACE_OS::strdup (entry->type_);
  }

  Name_Binding (const ACE_NS_WString &n,
                const ACE_NS_WString &v,
                const char *t)
  {
    this->name_ = n.char_rep ();
    this->value_ = v.char_rep ();
    this->type_ = ACE_OS::strdup (t);
  }

  ~Name_Binding ()
  {
    delete this->name_;
    delete this->value_;
    delete this->type_;
  }

  char *name (void)
  { return this->name_; }

  char *value (void)
  { return this->value_; }

  const char *type (void)
  { return this->type_; }

  int int_value (void)
  { return ACE_OS::atoi (this->value ()); }

private:
  char *name_;
  char *value_;
  const char *type_;
};

typedef auto_ptr<Name_Binding> Name_Binding_Ptr;
// Listing 1

#endif /* NAME_BINDING_H */
