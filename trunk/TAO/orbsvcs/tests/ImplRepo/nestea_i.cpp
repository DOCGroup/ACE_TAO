// $Id$

#include "nestea_i.h"
#include "tao/corba.h"

ACE_RCSID(ImplRepo, nestea_i, "$Id$")

// Constructor

Nestea_i::Nestea_i (int shutdown)
: shutdown_ (shutdown),
  cans_ (0)
{
}


// Destructor

Nestea_i::~Nestea_i (void)
{
  // Nothing
}


// Add <cans> number of cans to the bookshelf.

void 
Nestea_i::drink (CORBA::Long cans,
                 CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Nestea_i::drink %d cans\n", cans));

  if (this->shutdown_ != 0)
    TAO_ORB_Core_instance ()->orb ()->shutdown ();

  this->cans_ += cans;
}


// Removes <cans> number of cans from the bookshelf.

void 
Nestea_i::crush (CORBA::Long cans,
                 CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Nestea_i::crush %d cans\n", cans));

  if (this->shutdown_ != 0)
    TAO_ORB_Core_instance ()->orb ()->shutdown ();

  this->cans_ -= cans;
  
  // Don't go below 0.
  if (this->cans_ < 0) 
    this->cans_ = 0;
}


// Returns the number of cans in the bookshelf.

CORBA::Long 
Nestea_i::bookshelf_size (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Nestea_i::bookshelf_size\n"));

  if (this->shutdown_ != 0)
    TAO_ORB_Core_instance ()->orb ()->shutdown ();

  return this->cans_;
}

// Returns comments about your collection.

char *
Nestea_i::get_praise (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Nestea_i::get_praise\n"));

  if (this->shutdown_ != 0)
    TAO_ORB_Core_instance ()->orb ()->shutdown ();

  if (this->cans_ > 500)
    return CORBA::string_dup ("Man, that is one excellent Nestea Collection!");
  else if (this->cans_ > 250)
    return CORBA::string_dup ("We are getting into the big leagues now!");
  else if (this->cans_ > 100)
    return CORBA::string_dup ("Things are looking up!");
  else if (this->cans_ > 0)
    return CORBA::string_dup ("Well, it is a start. Drink more Nestea!");
  else
    return CORBA::string_dup ("No cans, no praise.");
}
