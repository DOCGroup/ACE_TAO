// -*- C++ -*-
// File: Obj_Module.h

// Author: Phil Mesnier

#ifndef _OBJ_MODULE_H_
#define _OBJ_MODULE_H_

#include "Sig_List.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Message_Block;
ACE_END_VERSIONED_NAMESPACE_DECL

/**
 * @brief Stores the symbols imported and exported by one object module.
 *
 * Modules with external references are included in the reduced library.
 */
class Obj_Module {
public:
  explicit Obj_Module (ACE_CString const &, size_t = 500);
  Obj_Module (Obj_Module const &) = delete;
  Obj_Module (Obj_Module &&) = delete;
  Obj_Module &operator= (Obj_Module const &) = delete;
  Obj_Module &operator= (Obj_Module &&) = delete;

  /// Returns the signatures defined in this module.
  Sig_List & exports();

  /// Returns the signatures used by this module but not defined within it.
  Sig_List & imports();

  /// Returns the object module name.
  ACE_CString const &name() const;

  /**
   * Invokes GNU nm on the supplied file and parses its output into imported
   * and exported signatures.
   */
  void add_source (char const *, bool = false);

  /// Returns the number of external references to this module.
  size_t extref () const;

  /// Adds an external reference to this module.
  void add_extref ();

private:
  /// The message block is borrowed; its chain owns any heap continuations.
  void populate_sig_list (Sig_List &, size_t, ACE_Message_Block &);
  /// buf is borrowed; null tells read_line to discard input.
  int read_line (ACE_HANDLE src, ACE_Message_Block *&buf);

  ACE_CString name_;
  Sig_List imports_;
  Sig_List exports_;
  size_t extrefs_ {};
};

#endif /* _OBJ_MODULE_H_ */
