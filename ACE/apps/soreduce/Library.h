// -*- C++ -*-
#ifndef _LIBRARY_H_
#define _LIBRARY_H_

// -*- C++ -*-
// File: Library.h

// Author: Phil Mesnier

#include "Obj_Module.h"
#include <memory>
#include <vector>

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

/**
 * @brief Generates MPC files for subset libraries.
 *
 * The base class generates a project for libACE_subset.so.
 */
class MPC_Generator
{
public:
  explicit MPC_Generator (ACE_CString const &libname);
  virtual ~MPC_Generator () = default;
  MPC_Generator (MPC_Generator const &) = delete;
  MPC_Generator (MPC_Generator &&) = delete;
  MPC_Generator &operator= (MPC_Generator const &) = delete;
  MPC_Generator &operator= (MPC_Generator &&) = delete;

  void write_prolog (ACE_CString const &);
  void write_file (ACE_CString const &);
  void write_epilog ();

protected:
  virtual void write_baseprojects ();
  virtual void write_projectinfo ();

  ofstream mpcfile_;
  ACE_CString libname_;
  ACE_CString mpcfilename_;
};

/// Generates MPC files for libraries that depend on ACE but not TAO.
class MPC_ACE_Dep_Lib : public MPC_Generator
{
public:
  explicit MPC_ACE_Dep_Lib (ACE_CString const &libname);
  MPC_ACE_Dep_Lib (MPC_ACE_Dep_Lib const &) = delete;
  MPC_ACE_Dep_Lib (MPC_ACE_Dep_Lib &&) = delete;
  MPC_ACE_Dep_Lib &operator= (MPC_ACE_Dep_Lib const &) = delete;
  MPC_ACE_Dep_Lib &operator= (MPC_ACE_Dep_Lib &&) = delete;

protected:
  void write_baseprojects () override;
  void write_projectinfo () override;
};

/// Generates MPC files for libTAO_subset.so.
class MPC_TAO_Lib : public MPC_ACE_Dep_Lib
{
public:
  explicit MPC_TAO_Lib (ACE_CString const &libname);
  MPC_TAO_Lib (MPC_TAO_Lib const &) = delete;
  MPC_TAO_Lib (MPC_TAO_Lib &&) = delete;
  MPC_TAO_Lib &operator= (MPC_TAO_Lib const &) = delete;
  MPC_TAO_Lib &operator= (MPC_TAO_Lib &&) = delete;

protected:
  void write_baseprojects () override;
  void write_projectinfo () override;
};

/**
 * @brief Generates MPC files for libraries that depend on TAO.
 *
 * The generated projects need special handling for libraries in the orbsvcs
 * tree.
 */
class MPC_TAO_Dep_Lib : public MPC_TAO_Lib
{
public:
  explicit MPC_TAO_Dep_Lib (ACE_CString const &);
  MPC_TAO_Dep_Lib (MPC_TAO_Dep_Lib const &) = delete;
  MPC_TAO_Dep_Lib (MPC_TAO_Dep_Lib &&) = delete;
  MPC_TAO_Dep_Lib &operator= (MPC_TAO_Dep_Lib const &) = delete;
  MPC_TAO_Dep_Lib &operator= (MPC_TAO_Dep_Lib &&) = delete;

protected:
  void write_baseprojects () override;
  void write_projectinfo () override;
};

/**
 * @brief Models a shared library and the object modules it contains.
 *
 * It resolves undefined references by loading required modules and generates
 * an MPC project to build a reduced library.
 */
class Library
{
public:
  explicit Library (ACE_TCHAR const *name = {});
  ~Library () = default;
  Library (Library const &) = delete;
  Library (Library &&) = delete;
  Library &operator= (Library const &) = delete;
  Library &operator= (Library &&) = delete;

  /**
   * Resolves undefined signatures against modules in this library. Symbols
   * found in a module mark it as exported and add its unresolved imports to
   * the list being resolved.
   */
  void resolve (Sig_List &undefs);

  /**
   * Writes the object modules needed for a subset library to an MPC file.
   * Optionally writes reference counts for each module.
   */
  void write_export_list (bool);

  /// Sets the path used to find the shared library object files.
  void set_path (ACE_TCHAR const *p);

  /// Loads the object modules from the library path.
  void load_modules();

  /// Returns the library name.
  ACE_CString const &name () const;

  /// Returns true if the library contains object modules.
  bool has_modules () const;

private:
  ACE_CString name_;
  ACE_CString path_;

  size_t num_extrefs_ {};

  std::vector<std::unique_ptr<Obj_Module>> modules_;
  Sig_List exported_;
  std::unique_ptr<MPC_Generator> mpcfile_;
};

#endif /* _LIBRARY_H_ */
