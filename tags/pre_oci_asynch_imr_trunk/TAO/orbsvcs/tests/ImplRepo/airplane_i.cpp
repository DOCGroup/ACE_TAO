// $Id$

#include "airplane_i.h"

#include "tao/debug.h"
#include "ace/OS_NS_time.h"

// Constructor
Airplane_i::Airplane_i ()
{
  // Seed the random number generator
  ACE_OS::srand (static_cast<u_int> (ACE_OS::time (0L)));
}


// Destructor

Airplane_i::~Airplane_i (void)
{
  // Nothing
}


// Returns a random plane and page number

char *
Airplane_i::get_plane (void)
{
  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Airplane_i::get_plane\n"));

  switch (ACE_OS::rand () % 24)
    {
    case 0:
      return CORBA::string_dup ("Seagull, page 2");
    case 1:
      return CORBA::string_dup ("Albatross, page 7");
    case 2:
      return CORBA::string_dup ("Owl, page 13");
    case 3:
      return CORBA::string_dup ("Falcon, page 19");
    case 4:
      return CORBA::string_dup ("Sparrow, page 24");
    case 5:
      return CORBA::string_dup ("Condor, page 31");
    case 6:
      return CORBA::string_dup ("Basic wing-flapper, page 36");
    case 7:
      return CORBA::string_dup ("Butterfly, page 42");
    case 8:
      return CORBA::string_dup ("Moth, page 49");
    case 9:
      return CORBA::string_dup ("Bat, page 54");
    case 10:
      return CORBA::string_dup ("Wind rider, page 62");
    case 11:
      return CORBA::string_dup ("Bobber, page 67");
    case 12:
      return CORBA::string_dup ("Thunderbird, page 72");
    case 13:
      return CORBA::string_dup ("Oddbird, page 77");
    case 14:
      return CORBA::string_dup ("Another oddbird, page 81");
    case 15:
      return CORBA::string_dup ("Asymmetric plane, page 86");
    case 16:
      return CORBA::string_dup ("Flying spectacles, page 90");
    case 17:
      return CORBA::string_dup ("UFO, page 100");
    case 18:
      return CORBA::string_dup ("Space fighter #1, page 105");
    case 19:
      return CORBA::string_dup ("Space fighter #2, page 110");
    case 20:
      return CORBA::string_dup ("Pinwheel, page 120");
    case 21:
      return CORBA::string_dup ("Flying saucer, page 128");
    case 22:
      return CORBA::string_dup ("Flying teacup, page 134");
    case 23:
      return CORBA::string_dup ("Tumbler, page 138");
    }

  return 0;
}
