/* -*- c++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    IDL_TO_PICML_BE_DLL
//
// = FILENAME
//    be_global.h
//
// = DESCRIPTION
//    Header file for class containing compiler back end global data.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef IDL_TO_PICML_BE_GLOBAL_H
#define IDL_TO_PICML_BE_GLOBAL_H

#include "IDL_TO_WSDL_BE_Export.h"
#include "ast_type.h"
#include "ace/SString.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/Unbounded_Set.h"
#include "ace/Null_Mutex.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "idl_defines.h"

#include "xercesc/dom/DOM.hpp"
#include "xercesc/dom/DOMElement.hpp"
#include "xercesc/framework/LocalFileFormatTarget.hpp"

using namespace xercesc;

#define X(str) XStr (ACE_TEXT (str))
class AST_Generator;
class AST_Module;
class AST_Component;

// Defines a class containing all back end global data.

class IDL_TO_WSDL_BE_Export BE_GlobalData
{
public:
  // = TITLE
  //    BE_GlobalData
  //
  // = DESCRIPTION
  //    Storage of global data specific to the compiler back end
  //
  BE_GlobalData (void);
  // Constructor.

  virtual ~BE_GlobalData (void);
  // Destructor.

  // Empty DOMElements are created for forward declarations, then
  // looked up and completed when the full definition is seen.
  
  typedef ACE_Hash_Map_Entry<const char *,
                             DOMElement *>
    DECL_ELEM_TABLE_ENTRY;
    
  typedef ACE_Hash_Map_Manager_Ex<const char *,
                                  DOMElement *,
                                  ACE_Hash<const char *>,
                                  ACE_Equal_To<const char *>,
                                  ACE_Null_Mutex>
    DECL_ELEM_TABLE;

  typedef ACE_Hash_Map_Iterator_Ex<const char *,
                                   DOMElement *,
                                   ACE_Hash<const char *>,
                                   ACE_Equal_To<const char *>,
                                   ACE_Null_Mutex>
    DECL_ELEM_TABLE_ITERATOR;
    
  enum diagnostic_type
    {
      ADDING,
      REMOVING
    };
    
  // Data accessors.

  const char *filename (void) const;
  void filename (char *fname);

  const char* output_dir (void) const;
  void output_dir (const char* s);
  
  const char* input_wsdl (void) const;
  void input_wsdl (const char* s);
  
  unsigned long npredefined (void) const;
  // Accessor for the readonly member.
  
  long nfiles (void) const;
  void nfiles (long val);
  // Accessors for the member.
  
  bool first_file (void) const;
  void first_file (bool val);
  // Accessors for the member.
  
  ACE_CString output_file (void) const;
  void output_file (const char *val);
  // Accessors for the member.
  
  bool do_removal (void) const;
  void do_removal (bool val);
  // Accessors for the member.
  
  DOMDocument *doc (void) const;
  // Readonly accessor for the member.
  
  DOMElement *root_element (void) const;
  void root_element (DOMElement *elem);
  // Accessors for the member.
  
  DOMElement *types_schema (void) const;
  void types_schema (DOMElement *elem);
  // Accessors for the member.

  DOMElement *msg_insert_point (void) const;
  void msg_insert_point (DOMElement *elem);
  // Accessors for the member.

  DOMElement *port_type_insert_point (void) const;
  void port_type_insert_point (DOMElement *elem);
  // Accessors for the member.

  DOMElement *binding_insert_point (void) const;
  void binding_insert_point (DOMElement *elem);
  // Accessors for the member.

  DECL_ELEM_TABLE &decl_elem_table (void);
  // Accessor for the member.
  
  // Utility methods.
  
  ACE_CString spawn_options (void);
  // Command line passed to ACE_Process::spawn. Different
  // implementations in different backends.

  void parse_args (long &i, char **av);
  // Parse args that affect the backend.
  
  void prep_be_arg (char *s);
  // Special BE arg call factored out of DRV_args.
  
  void arg_post_proc (void);
  // Checks made after parsing args.
  
  void usage (void) const;
  // Display usage of BE-specific options.
  
  AST_Generator *generator_init (void);
  // Create an AST node generator.
  
  void xerces_init (void);
  // Initialize all the Xerces stuff.
  
  void cache_files (char *files[], long nfiles);
  // Cache the list of files from the command line.
  
  void destroy (void);
  // Cleanup.
  
  char *hex_string (unsigned long number);
  // Conversion utility.
  
  ACE_CString basic_seq_suffix (void) const;
  // Accessor for the member.

  DOMElement *imported_dom_element (DOMElement *sub_tree,
                                    const char *local_name,
                                    bool in_file_iteration = false);
  // Have we already imported <local_name> in scope <sub_tree>?
 
  DOMElement *imported_module_dom_elem (DOMElement *sub_tree,
                                        AST_Module *node);
  // Specialization for IDL modules of above method.
  
  DOMElement *lookup_by_tag_and_kind (DOMElement *scope,
                                      const char *tag_name,
                                      const char *kind_name);
  // Utility operation.
  
  void release_node (DOMElement *node);
  // Remove a subtree from the DOM tree and release its resources.
  
  void check_for_basic_seq (AST_Decl *d, ACE_CString &str);
  void check_for_basic_type (AST_Decl *d, ACE_CString &str);
  // Utility methods.
  
  void to_wsdl_name (ACE_CString &name);
  void to_idl_name (ACE_CString &name);
  // Conversion methods, replaces :: with . and vice versa.
/*
  void emit_diagnostic (DOMElement *node, diagnostic_type dt = ADDING);
  // Report each model element added or removed.
  
  void emit_attribute_diagnostic (DOMElement *node,
                                  const char *name,
                                  const char *new_value,
                                  DOMText *old_value);
  // Report a change in a GME attribute value.
  
  void type_change_diagnostic (DOMElement *node,
                               const XMLCh *new_ref);
  // Will emit a diagnostic if the 'referred' attribute
  // value of node is about to be changed.
  
  void base_component_diagnostic (DOMElement *elem,
                                  AST_Component *node,
                                  AST_Component *base,
                                  bool was_derived,
                                  const XMLCh *base_id_from_idl = 0);
  // Specialized method for reporting additions, removals,
  // or changes to a component's parent.
  
  void included_file_diagnostic (DOMElement *fileref,
                                 DOMElement *file,
                                 const char *fileref_name);
  // Specialized method for reporting added file includes.
*/
private:  
  char *get_name (DOMElement *node);
  // Utility function that gets the value of the child with
  // tag 'name'. Caller must release return value with
  // XMLString::release().
  
  bool match_module_opening (DOMElement *elem, AST_Decl *node);
  // Checks local name of first member, recursively if necessary.
  
  void create_document (DOMImplementation *impl);
  void parse_document (DOMImplementation *impl);
  // One or the other gets called depending on whether or not we are
  // importing an existing WSDL file.
  
  void create_dom_writer (DOMImplementation *impl);
  // Create and configure the DOM writer.
  
  void create_target (void);
  // Create the output XML file.
  
  void set_root_attrs (void);
  // Set the namespace attributes of the root element.
  
  void set_common_attrs (DOMElement *elem);
  // Sets namespace attributes common to 'definitions' and
  // 'schema'.
  
  void create_types_schema (void);
  // Create the nested nodes that hold the type definitions.

private:
  char *filename_;
  // Name of the IDL file we are processing.

  char *output_dir_;
  // Directory where the generated file is to be
  // kept. Default value is 0 for this string which means the current
  // directory from which the <idl_to_picml> is called.
  
  char *input_wsdl_;
  // URL of the XME file to input, if any.

  unsigned long npredefined_;
  // Number of predefined types in IDML.
  
  const char** pdt_names (void) const;
  // Get a list of our predefined types.

  const char *allfiles_[NFILES];
  // All files in the command line.
  
  long nfiles_;
  // Actual length of the above list.
  
  bool first_file_;
  // Are we processing the first file on the command line?
  
  ACE_CString output_file_;
  // Set from command line or had default value.
  
  bool do_removal_;
  // Do we spawn the removing visitor?
  
  DOMDocument *doc_;
  DOMWriter *writer_;
  XMLFormatTarget *target_;
  DOMElement *root_element_;
  DOMElement *types_schema_;
  DOMElement *msg_insert_point_;
  DOMElement *port_type_insert_point_;
  DOMElement *binding_insert_point_;
  // DOM items we need to cache.
  
  DECL_ELEM_TABLE decl_elem_table_;
  // Hash map of DOMElements, keyed by repository id.
  
  ACE_CString basic_seq_suffix_;
  // Appended to name of basic type to get corresponding sequence.
  
  ACE_CString schema_path_;
  // Location of the WSDL schema.
};

#endif /* IDL_TO_PICML_BE_GLOBAL_H */
