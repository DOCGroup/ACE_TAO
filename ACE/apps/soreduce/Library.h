// -*- C++ -*-
#ifndef _LIBRARY_H_
#define _LIBRARY_H_

// -*- C++ -*-
// File: Library.h

// Author: Phil Mesnier

// A Library is a collection of Obj_Modules that define a single shared
// library. It is used to manipulate the list of unresolved references by
// removing those that are resolved and adding those brought in by new modules
// that are required to resolve references.  The Library is responsible
// for outputting a specialized mpc file to build the reduce footprint library.

#include "Obj_Module.h"
#include <memory>

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

// The MPC generator class serves as the base class used to output the
// custom mpc files used to build the subsetted libraries.
// The base class will make libACE_subset.so

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

// Generate mpc files for libraries dependant on ACE, that are not TAO.
class MPC_ACE_Dep_Lib : public MPC_Generator
{
public:
  explicit MPC_ACE_Dep_Lib (ACE_CString const &libname);

protected:
  void write_baseprojects () override;
  void write_projectinfo () override;
};

// Generates mpc files for libTAO_subset.so
class MPC_TAO_Lib : public MPC_ACE_Dep_Lib
{
public:
  explicit MPC_TAO_Lib (ACE_CString const &libname);

protected:
  void write_baseprojects () override;
  void write_projectinfo () override;
};

// Generates makefiles for libs dependant on TAO.  This has a problem when
// building libraries in the orbsvcs tree.
class MPC_TAO_Dep_Lib : public MPC_TAO_Lib
{
public:
  explicit MPC_TAO_Dep_Lib (ACE_CString const &);

protected:
  void write_baseprojects () override;
  void write_projectinfo () override;
};

//----------------------------------------------------------------------------

class Library
{
public:
  explicit Library (ACE_TCHAR const *name = nullptr);
  /// Constructor is responsible for loading all of the modules related to the
  /// library
  ~Library ();
  Library (Library const &) = delete;
  Library (Library &&) = delete;
  Library &operator= (Library const &) = delete;
  Library &operator= (Library &&) = delete;

  // Resolve interates over the supplied list of undefined signatures to locate
  // modules that contain definitions. Any symbol defined in a module marked as
  // exported is simply removed from the undef list. Any symbol defined in a
  // module not yet exported removed from the undef list, the module is marked
  // as exported, and its unresolved symbols are added to the undef list.
  void resolve (Sig_List &undefs);

  // Outputs a list of files suitable for inclusion in an mpc file to produce
  // a subsetted library. If requested, reference counts for each module are
  // also listed.
  void write_export_list (bool);

  // set the path to find the .so files
  void set_path (ACE_TCHAR const *p);

  // Load the actual .so files from the path.
  void load_modules();

  // returns the library name
  ACE_CString const &name () const;

  // Returns true if the module count is greater than zero.
  bool has_modules () const;

private:
  ACE_CString name_;
  ACE_CString path_;

  int num_modules_;
  int num_extrefs_;

  std::unique_ptr<Obj_Module *[]> modules_;
  Sig_List exported_;
  std::unique_ptr<MPC_Generator> mpcfile_;
};

#endif /* _LIBRARY_H_ */
