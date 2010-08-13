// -*- C++ -*-

//=============================================================================
/**
 *  @file    nestea_i.h
 *
 *  $Id$
 *
 *  This class is an implementation of the Nestea Bookshelf interface.
 *
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================


#if !defined (NESTEA_I_H)
#define NESTEA_I_H

#include "NesteaS.h"

// Forward declarations.
class Nestea_i;

// Typedefs.
typedef Nestea_i *Nestea_i_ptr;
typedef Nestea_i_ptr Nestea_i_ref;

/**
 * @class Nestea_i:
 *
 * @brief Nestea Bookshelf Implementation
 *
 * Implements the Nestea Bookshelf server, which keeps track of the
 * number of nestea cans in a bookshelf.  You can drink Nestea to add
 * it to the bookshelf or crush the cans to remove them.
 */
class Nestea_i: public POA_Nestea_Bookshelf
{
public:
  /// Constructor
  Nestea_i (CORBA::ORB_ptr orb, const ACE_TCHAR *filename = ACE_TEXT("nestea.dat"));

  /// Destructor
  virtual ~Nestea_i (void);

  /// Add <cans> number of cans to the bookshelf.
  virtual void drink (CORBA::Long cans);

  /// Removes <cans> number of cans from the bookshelf.
  virtual void crush (CORBA::Long cans);

  /// Returns the number of cans in the bookshelf.
  virtual CORBA::Long bookshelf_size (void);

  /// Returns comments about your collection.
  virtual char *get_praise (void);

  virtual void shutdown(void);
private:
  /// Saves bookshelf data to a file.
  int save_data (void);

  /// Loads bookshelf data from a file.
  int load_data (void);

  /// The name of the file to store the data in.
  ACE_TCHAR *data_filename_;

  /// Number of cans in the bookshelf.
  ACE_UINT32 cans_;

  CORBA::ORB_var orb_;
};

#endif /* NESTEA_I_H */
