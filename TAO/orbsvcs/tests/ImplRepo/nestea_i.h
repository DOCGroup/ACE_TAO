// -*- C++ -*-

//=============================================================================
/**
 *  @file    nestea_i.h
 *
 *  This class is an implementation of the Nestea Bookshelf interface.
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================

#if !defined (NESTEA_I_H)
#define NESTEA_I_H

#include "NesteaS.h"
#include "ace/SString.h"

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
  explicit Nestea_i (CORBA::ORB_ptr orb, ACE_TCHAR const *filename = ACE_TEXT ("nestea.dat"));

  /// Destructor
  ~Nestea_i () override = default;

  Nestea_i (Nestea_i const &) = delete;
  Nestea_i &operator= (Nestea_i const &) = delete;
  Nestea_i (Nestea_i &&) = delete;
  Nestea_i &operator= (Nestea_i &&) = delete;

  /// Add <cans> number of cans to the bookshelf.
  void drink (CORBA::Long cans) override;

  /// Removes <cans> number of cans from the bookshelf.
  void crush (CORBA::Long cans) override;

  /// Returns the number of cans in the bookshelf.
  CORBA::Long bookshelf_size () override;

  /// Returns comments about your collection.
  char *get_praise () override;

  void shutdown () override;
private:
  /// Saves bookshelf data to a file.
  int save_data ();

  /// Loads bookshelf data from a file.
  int load_data ();

  /// The name of the file to store the data in.
  ACE_TString data_filename_ {};

  /// Number of cans in the bookshelf.
  ACE_UINT32 cans_ {};

  CORBA::ORB_var orb_ {};
};

#endif /* NESTEA_I_H */
