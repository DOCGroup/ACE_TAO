// -*- C++ -*-
// File: SO_Group.h

// Author: Phil Mesnier

#ifndef _SO_GROUP_H_
#define _SO_GROUP_H_

#include "Library.h"
#include <memory>
#include <vector>

/**
 * @brief Holds information used to reduce libraries shared by applications.
 */
class SO_Group
{
public:
  SO_Group ();
  ~SO_Group () = default;
  SO_Group (SO_Group const &) = delete;
  SO_Group (SO_Group &&) = delete;
  SO_Group &operator= (SO_Group const &) = delete;
  SO_Group &operator= (SO_Group &&) = delete;

  /**
   * Runs ldd for the executable, adds libraries with a .shobj directory, and
   * adds the executable's undefined symbols to the unresolved list.
   */
  void add_executable (char const *);

  /// Resolves references across the executable's libraries.
  void analize ();

  /// Writes the reduced library project files.
  void write_results ();

  /// Loads object modules for the group's libraries.
  void load_modules ();

  void list_libs();

private:
  Obj_Module undef_wrapper_;
  Sig_List &undefs_;
  std::vector<std::unique_ptr<Library>> libs_;
};

#endif //_SO_GROUP_H_
