/* -*- C++ -*- */
// $Id$
//
// ==================================================================
//
// = LIBRARY
//    PSS
//
// = FILENAME
//    PSDL_Scope
//
// = DESCRIPTION
//    This Class is a base class for building the ASTs. The derived
//    classes override the virtual methods that they need. The class
//    also contains the methods which are common to all the derived
//    classes.
//
// = AUTHOR
//    Priyanka Gontla <gontla_p@ociweb.com>
//
// ==================================================================

#ifndef TAO_PSDL_SCOPE_H
#define TAO_PSDL_SCOPE_H

#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "psdl_export.h"

#include "tao/TAO_Singleton.h"

#include "ace/Hash_Map_Manager_T.h"
#include "ace/Array_Base.h"

// Fwd Declarations.
class TAO_PSDL_Node;
class TAO_PSDL_Scope;
class TAO_PSDL_Root_Scope;
class TAO_PSDL_Stream;

typedef ACE_Hash_Map_Manager_Ex<ACE_CString, TAO_PSDL_Scope *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, TAO_SYNCH_MUTEX> Scope_Map;

typedef Scope_Map::iterator Scope_Map_Iterator;

class TAO_PSDL_Export TAO_PSDL_Scope
{
  // The TAO_PSDL_Scope class serves two purposes. One it acts as the
  // base class from which the child classes derive to be able to add
  // the members in their scopes. TAO_PSDL_Root_Scope is the root
  // scope where any thing goes into. If there is a module in the idl
  // file, all the members in the module are to be added to an
  // instance of TAO_PSDL_Module_Scope. And, it also acts as a class
  // that has the common members to be accessed by all the derived
  // classes. For example, add_module_to_scope is the common method
  // for all the sub-classes to invoke from the add_module method.
  friend class TAO_Singleton<TAO_PSDL_Scope, TAO_SYNCH_MUTEX>;

public:

  TAO_PSDL_Scope (void);

  virtual ~TAO_PSDL_Scope (void);

  /// The add functions are overridden by the derived classes to add
  /// the respective types in their scopes. The return value is a '0'
  /// if successful and '-1' if it ends up as a failure.
  virtual int add_module (ACE_CString identifier);

  virtual int add_interface (ACE_CString identifier);

  virtual int add_struct (ACE_CString identifier);

  virtual int add_typedef (ACE_CString identifier,
                           ACE_CString identifier_type);

  virtual int add_const_decl (ACE_CString identifier,
                              ACE_CString identifier_type);

  virtual int add_except_decl (ACE_CString identifier,
                               ACE_CString identifier_type);

  virtual int add_exception (ACE_CString identifier);

  virtual int add_op_dcl (ACE_CString identifier);

  virtual int add_member_decl (ACE_CString identifier,
                               ACE_CString identifier_type);

  /// Return the top scope.
  virtual TAO_PSDL_Scope *pop_top_scope (void);

  /// Push the scope that is passed into the stack of scopes.
  virtual void push_scope (TAO_PSDL_Scope *scope);

  /// Return the pointer to the parent scope.
  virtual TAO_PSDL_Scope *parent_scope (void);

  /// Return the pointer to Scope_Map of the instance.
  virtual Scope_Map *scope_map (void);

  /// Function to help indent the output of the parse tree.
  virtual void dump (CORBA::ULong depth);

  /// Function to find the TAO_PSDL_Scope for the given identifier_name
  virtual int find (const ACE_CString &identifier_name,
                    ACE_CString &identifier_type);

  /// Function to check if a particular identifier_name exists in the
  /// scope.
  virtual int find (const ACE_CString &identifier_name);

  /// To get the name of the module to which an identifier_name
  /// belongs. returns '0' on succes and '-1' on failure.
  virtual int get_module_name (const ACE_CString &identifier_name,
                               ACE_CString &module_name);

  /// To get the name of the interface to which an identifier_name
  /// belongs. returns '0' on succes and '-1' on failure.
  virtual int get_interface_name (const ACE_CString &identifier_name,
                                  ACE_CString &interface_name);

  /// Each identifier will have an instance of a derived type of
  /// TAO_PSDL_Scope *.. the following method is an accessor to the
  /// type of the identifier name
  virtual ACE_CString identifier_type (void);
  virtual ACE_CString module_name (void);
  virtual ACE_CString interface_name (void);

  /// As the function name implies, the following methods help add the
  /// respective types to the scope passed in. The return value is a
  /// '0' on success and '-1' on a failure.
  int add_module_to_scope (ACE_CString identifier,
                           TAO_PSDL_Scope *scope);

  int add_interface_to_scope (ACE_CString identifier,
                              TAO_PSDL_Scope *scope);

  int add_interface_dcl_to_scope (ACE_CString identifier,
                                  TAO_PSDL_Scope *scope);

  int add_struct_to_scope (ACE_CString identifier,
                           TAO_PSDL_Scope *scope);

  int add_typedef_to_scope (ACE_CString identifier,
                            ACE_CString identifier_type,
                            TAO_PSDL_Scope *scope);

  int add_const_decl_to_scope (ACE_CString identifier,
                               ACE_CString identifier_type,
                               TAO_PSDL_Scope *scope);

  int add_except_decl_to_scope (ACE_CString identifier,
                                ACE_CString identifier_type,
                                TAO_PSDL_Scope *scope);

  int add_exception_to_scope (ACE_CString identifier,
                              TAO_PSDL_Scope *scope);

  int add_scoped_decl_to_scope (ACE_CString identifier,
                                ACE_CString identifier_type,
                                TAO_PSDL_Scope *scope);

  int add_enum_decl_to_scope (ACE_CString identifier,
                              ACE_CString identifier_type,
                              TAO_PSDL_Scope *scope);

  int add_op_dcl_to_scope (ACE_CString identifier,
                           TAO_PSDL_Scope *scope);

  int add_member_decl_to_scope (ACE_CString identifier,
                                ACE_CString identifier_type,
                                TAO_PSDL_Scope *scope);

  /// Set the root scope the very first time.
  void set_root_scope (void);

  /// Set and get the name of the stub that has to be generated. The name
  /// is based on the input file that is passed to the compiler.
  void set_stub_prefix (const char *filename);
  const ACE_CString &get_stub_prefix (void);

  /// These methods help write the header files that needs to be
  /// included and other initialization part to the stubs.
  void header_initialization (TAO_PSDL_Stream *ps_sh);
  void stub_initialization (TAO_PSDL_Stream *ps_si);

  /// Get the pointer to the TAO_PSDL_Stream for the stub header.
  TAO_PSDL_Stream *get_sh (void);

  /// Get the pointer to the TAO_PSDL_Stream for the stub
  /// implementation file.
  TAO_PSDL_Stream *get_si (void);

  /// Get the pointer to the TAO_PSDL_Stream for the stub
  /// inline file.
  TAO_PSDL_Stream *get_sinline (void);

  /// As the name suggests, check if the identifier name is already
  /// used for something else in the sc
  int check_identifier (ACE_CString identifier,
                        TAO_PSDL_Scope *scope);

  /// This is to check if a forward declaration has been done
  /// already.
  void check_name_in_scope (ACE_CString identifier,
                            TAO_PSDL_Scope *scope);

  /// Return a unique instance
  static TAO_PSDL_Scope *instance (void);

  /// This wont be necessary in the final version .. its helpful to
  /// print the symbol table to the std output for now.
  void print_depth (CORBA::ULong depth);

  /// Method used to set the scope back to the previous one.
  void set_scope (void);

  /// Set interface scope to the previous one.
  void set_interface_scope (void);

  /// Set module scope to the previous one.
  void set_module_scope (void);

  /// Methods to save and retrieve the identifier values.
  void save_identifier (ACE_CString identifier);
  ACE_CString get_identifier (void);

  /// Accessor methods to the name of the name_space: Module name
  void set_name_space (ACE_CString name_space);
  ACE_CString get_name_space (void);

  /// Accessor methods to the name of the current interface.
  void set_interface_name (ACE_CString interface_name);
  ACE_CString get_interface_name (void);

  // Helper method for the semantic checking. By converting all the
  // identifiers to lower case before adding to the scope.
  void to_lower_case (ACE_CString &identifier);

  /// Helper method to convert the int value of the identifier type to
  /// its string equivalent based on the conversion values generated
  /// by the Yacc Parser (please see PSDL_y.h for the conversion values).
  ACE_CString convert_str (int identifier_type);

private:

  /// Array of TAO_PSDL_Scope pointers to be used with the singleton
  /// instance.
  TAO_PSDL_Scope **psdl_scope_;

  /// Save the psdl_scopes for use while writing
  /// to stubs.
  ACE_Array_Base <TAO_PSDL_Scope *> ast_scope_;

  /// Arrays to save the module and interface names
  ACE_Array_Base <ACE_CString> module_names_;
  ACE_Array_Base <ACE_CString> interface_names_;

  /// Count of the members in the psdl_scope_
  unsigned long psdl_scope_top_;

  /// Pointer to the instance of the TAO_PSDL_Root_Scope.
  TAO_PSDL_Root_Scope *root_scope_;

  /// Identifier used for the save_identifier and get_identifier
  /// methods.
  ACE_CString identifier_;

  /// Save for use while writing to stubs.
  ACE_CString name_space_;
  ACE_CString interface_name_;

  /// Name of the filename that is passed to the compiler psdl_tao.
  ACE_CString stub_prefix_;

  /// Pointers to the TAO_PSDL_Streams for the stub header,
  /// implementation and inline files.
  TAO_PSDL_Stream *ps_sh_;
  TAO_PSDL_Stream *ps_si_;
  TAO_PSDL_Stream *ps_sin_;

};

#include "ace/post.h"

#endif /* TAO_PSDL_SCOPE_H */
