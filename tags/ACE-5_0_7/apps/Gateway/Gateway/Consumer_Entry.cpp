// Defines an entry in the Consumer Map.
// $Id$

#include "Consumer_Entry.h"

Consumer_Entry::Consumer_Entry (void)
{
  ACE_NEW (this->destinations_, Consumer_Entry::ENTRY_SET);
}

Consumer_Entry::~Consumer_Entry (void)
{
  delete this->destinations_;
}

// Get the associated set of destinations.

Consumer_Entry::ENTRY_SET *
Consumer_Entry::destinations (void)
{
  return this->destinations_;
}

// Set the associated set of destinations.

void 
Consumer_Entry::destinations (Consumer_Entry::ENTRY_SET *s)
{
  this->destinations_ = s;
}

