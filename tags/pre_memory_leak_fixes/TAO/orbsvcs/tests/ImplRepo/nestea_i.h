// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/ImplRepo
// 
// = FILENAME
//    nestea_i.h
//
// = DESCRIPTION
//    This class is an implementation of the Nestea Bookshelf interface.
//
// = AUTHOR
//    Darrell Brunsch <brunsch@cs.wustl.edu>
// 
// ============================================================================

#if !defined (NESTEA_I_H)
#define	NESTEA_I_H

#include "NesteaS.h"

// Forward declarations.
class Nestea_i;

// Typedefs.
typedef Nestea_i *Nestea_i_ptr;
typedef Nestea_i_ptr Nestea_i_ref;

class Nestea_i: public POA_Nestea_Bookshelf
{
  // = TITLE
  //    Nestea Bookshelf Implementation
  //
  // = DESCRIPTION
  //    Implements the Nestea Bookshelf server, which keeps track of the
  //    number of nestea cans in a bookshelf.  You can drink Nestea to add
  //    it to the bookshelf or crush the cans to remove them.
public:
  Nestea_i (const char *filename = "nestea.dat", int shutdown = 0);
  // Constructor that takes in an optional shutdown parameter which, if nonzero,
  // will shutdown the server after each call.

  virtual ~Nestea_i (void);
  // Destructor

  virtual void drink (CORBA::Long cans,
                      CORBA::Environment &ACE_TRY_ENV =
                        TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Add <cans> number of cans to the bookshelf.

  virtual void crush (CORBA::Long cans,
                      CORBA::Environment &ACE_TRY_ENV =
                        TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Removes <cans> number of cans from the bookshelf.

  virtual CORBA::Long bookshelf_size (CORBA::Environment &ACE_TRY_ENV =
                                        TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Returns the number of cans in the bookshelf.

  virtual char *get_praise (CORBA::Environment &ACE_TRY_ENV = 
                              TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Returns comments about your collection.

private:
  int save_data (void);
  // Saves bookshelf data to a file.

  int load_data (void);
  // Loads bookshelf data from a file.

  char *data_filename_;
  // The name of the file to store the data in.

  ACE_UINT32 cans_;
  // Number of cans in the bookshelf.
};

#endif /* NESTEA_I_H */
