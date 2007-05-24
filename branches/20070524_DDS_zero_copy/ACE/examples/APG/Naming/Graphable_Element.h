/* -*- C++ -*- */
// $Id$

#ifndef GRAPHABLE_ELEMENT_H
#define GRAPHABLE_ELEMENT_H

#include "Name_Binding.h"
#include <list>

// A helper class that knows how to sort two ACE_Name_Binding objects
// which contain temperature metrics. The value stored in the binding
// is expected to be of the format "time|temp".
//
// Listing 1 code/ch21
class Graphable_Element : public Name_Binding
{
public:
  Graphable_Element (ACE_Name_Binding *entry)
    : Name_Binding(entry)
  {
    sscanf (this->value (), "%d|%f", &this->when_, &this->temp_);
  }
  // Listing 1

  // Listing 2 code/ch21
  inline int when (void) const
  {
    return this->when_;
  }

  inline float temp (void)
  {
    return this->temp_;
  }
  // Listing 2

  // Listing 3 code/ch21
  inline bool operator< (const Graphable_Element &other) const
  {
    return this->when () < other.when ();
  }
  // Listing 3

  // Listing 4 code/ch21
private:
  int when_;
  float temp_;
};

typedef std::list<Graphable_Element> Graphable_Element_List;
// Listing 4

#endif /* GRAPHABLE_ELEMENT_H */
