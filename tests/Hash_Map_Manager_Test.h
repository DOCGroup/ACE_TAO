/* -*- C++ -*- */
// $Id$

// ============================================================================
// = FILENAME
//    Hash_Map_Manager_Test.h
//
// = DESCRIPTION
//    This file contains the definition of Dumb_String.  Some compilers
//    need it in a .h file for template instantiation (such as AIX C Set ++)
//
// = AUTHOR
//    James Hu, Steve Huston
// 
// ============================================================================

#if !defined (HASH_MAP_MANAGER_TEST_H)
#define HASH_MAP_MANAGER_TEST_H

class Dumb_String
{
  // = DESCRIPTION
  //     Desperate times call for desperate measures.  Bug your
  //     compiler vendor to support template specialization.
public:
  Dumb_String (char *s = 0);
  // Default constructor

  Dumb_String (const Dumb_String &ds);
  // Copy constructor

  ~Dumb_String (void);
  // Default destructor

  u_long hash (void) const;
  // To satisfy Hash_Map_Manager

  int operator== (const Dumb_String &ds) const;
  // To satisfy Hash_Map_Manager

  char *operator= (const Dumb_String &ds);
  // To satisfy Hash_Map_Manager

  int operator== (char const * s) const;
  operator char * (void) const;
  // These make life a little easier

private:
  char *s_;
  // The string.

  int &copy_;
  // Reference counter.

  int junk_;
  // default reference counter initializer.
};

#endif /* HASH_MAP_MANAGER_TEST_H */
