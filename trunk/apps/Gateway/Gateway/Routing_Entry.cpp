// Defines an entry in the Routing Table.
// @(#)Routing_Entry.cpp	1.1	10/18/96

#include "Routing_Entry.h"

Routing_Entry::Routing_Entry (int validity_interval)
  : validity_interval_ (validity_interval)
{
  ACE_NEW (this->destinations_, Routing_Entry::ENTRY_SET);
}

Routing_Entry::~Routing_Entry (void)
{
  delete this->destinations_;
}

// Get the associated set of destinations.

Routing_Entry::ENTRY_SET *
Routing_Entry::destinations (void)
{
  return this->destinations_;
}

// Set the associated set of destinations.

void 
Routing_Entry::destinations (Routing_Entry::ENTRY_SET *s)
{
  this->destinations_ = s;
}

// Get the current validity interval for this route.

int
Routing_Entry::validity_interval (void)
{
  return this->validity_interval_;
}

// Set the current validity interval for this route.

void
Routing_Entry::validity_interval (int vi)
{
  this->validity_interval_ = vi;
}
