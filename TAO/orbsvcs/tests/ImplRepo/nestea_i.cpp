// $Id$

#include "nestea_i.h"
#include "tao/corba.h"
#include "ace/FILE_Connector.h"

const size_t MAX_UINT32_STR_LEN = 11;  // Largest UINT32 is 8589934591 + NUL is 11 characters

ACE_RCSID(ImplRepo, nestea_i, "$Id$")

// Constructor

Nestea_i::Nestea_i (const char *filename, int shutdown)
: shutdown_ (shutdown),
  cans_ (0)
{
  this->data_filename_ = ACE::strnew (filename);
  
  // @@ This should probably be called from somewhere else
  this->load_data ();
}


// Destructor

Nestea_i::~Nestea_i (void)
{
  delete this->data_filename_;
}


// Add <cans> number of cans to the bookshelf.

void 
Nestea_i::drink (CORBA::Long cans,
                 CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Nestea_i::drink %d cans\n", cans));

  this->cans_ += cans;

  if (this->shutdown_ != 0)
    TAO_ORB_Core_instance ()->orb ()->shutdown ();

  this->save_data ();
}


// Removes <cans> number of cans from the bookshelf.

void 
Nestea_i::crush (CORBA::Long cans,
                 CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Nestea_i::crush %d cans\n", cans));

  if (cans > this->cans_)
    this->cans_ = 0;
  else
    this->cans_ -= cans;
  
  if (this->shutdown_ != 0)
    TAO_ORB_Core_instance ()->orb ()->shutdown ();

  this->save_data ();
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


// Saves bookshelf data to a file.

int 
Nestea_i::save_data (void)
{
  ACE_FILE_IO file;
  ACE_FILE_Addr filename (this->data_filename_);

  char str[MAX_UINT32_STR_LEN];

  ACE_OS::sprintf (str, "%d", this->cans_);

  return file.send_n (str, ACE_OS::strlen (str) + 1);
}


// Loads bookshelf data from a file.

int 
Nestea_i::load_data (void)
{
  ACE_FILE_IO file;
  ACE_FILE_Addr filename ("nestea.dat");

  char str[12];

  int len = file.recv (str, ACE_OS::strlen (str) + 1);
  str[len] = 0;

  if (len > 0)
    this->cans_ = ACE_OS::atoi (str);
  else
    this->cans_ = 0;
  return 0;
}
