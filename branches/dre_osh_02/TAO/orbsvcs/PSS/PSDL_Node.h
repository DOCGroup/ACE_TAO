/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    PSS
//
// = FILENAME
//    PSDL_Node
//
// = DESCRIPTION
//    This class is the base type for all the type of possible nodes
//    in the parse tree.
//
// = AUTHOR
//    Priyanka Gontla <gontla_p@ociweb.com>
//
// ============================================================================

#ifndef TAO_PSDL_NODE_H
#define TAO_PSDL_NODE_H

#include "psdl_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "PSDL_Scope.h"

#include "tao/corbafwd.h"
#include "ace/SString.h"


class TAO_PSDL_Node_Visitor;
class TAO_PSDL_Scope_Visitor;

class TAO_String_Manager;

class TAO_PSDL_Export TAO_PSDL_Node
{
public:

  // Constructor and destructor
  TAO_PSDL_Node (void);

  virtual ~TAO_PSDL_Node (void);

  // This method is for applying the visitor pattern. When the control
  // reaches a particular node, since we donot know the type of the
  // node, we invoke the accept method. Each derived classes accept
  // method, will invoke the corresponding visitor method which does
  // the needful for that particular type of the node.
  virtual int accept (TAO_PSDL_Node_Visitor *visitor) = 0;

  // Needed when writing to stubs.
  virtual int type_of_node (void) const;

  // Accessors to the identifiers for use while building the ASTs
  ACE_CString get_identifier_type (void);
  ACE_CString get_identifier_value (void);
  void set_identifier (ACE_CString identifier);

  // Set the predefined_type_ to 1 to indicate that the type is a
  // predefined type.
  void set_predefined_type (void);

  /// Accessor methods.
  TAO_PSDL_Scope *psdl_scope (void);
  TAO_PSDL_Scope_Visitor *psdl_scope_visitor (void);

protected :

  int type_;

  // Array to carry the temporary identifier_type and identifier_value
  // before they are added to the related scope. ie,. to the ASTs
  ACE_Array_Base <ACE_CString> identifiers_;

  // Index to keep track of the present member of the identifiers_
  // array.
  CORBA::ULong count_;

  // Check if the type of the identifier is a predefined one.
  // The default value is that any identifier_type is a user-defined
  // one i.e. if the value is zero, it is a user-defined type. When
  // the identifier_type is actually a predefined one, its value is to
  // be changed to 1.
  CORBA::Boolean predefined_type_;
  /// Pointer to psdl_scope_
  TAO_PSDL_Scope *psdl_scope_;

  // Pointer to the base visitor class
  TAO_PSDL_Scope_Visitor *psdl_scope_visitor_;
};

// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Specification : public TAO_PSDL_Node
{
  // This class is for psdl_specification s.
public:
  TAO_PSDL_Specification (TAO_PSDL_Node *definition);
  TAO_PSDL_Specification (TAO_PSDL_Node *definition,
                          TAO_PSDL_Node *specification);

  virtual ~TAO_PSDL_Specification (void);

  /// Get the values.
  TAO_PSDL_Node *definition (void) const;
  TAO_PSDL_Node *specification (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *definition_;
  TAO_PSDL_Node *specification_;

};

// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Definition : public TAO_PSDL_Node
{
  // For psdl_definition
public:
  TAO_PSDL_Definition (TAO_PSDL_Node *definition_type);

  virtual ~TAO_PSDL_Definition (void);

  /// Get the value
  TAO_PSDL_Node *definition_type (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *definition_type_;
};

// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Module : public TAO_PSDL_Node
{
  // for psdl_module
public:
  TAO_PSDL_Module (int type,
                   TAO_PSDL_Node *identifier,
                   TAO_PSDL_Node *specification);

  virtual ~TAO_PSDL_Module (void);

  /// Get the value
  TAO_PSDL_Node *identifier (void) const;
  TAO_PSDL_Node *specification (void) const;

  void set_identifier_name (ACE_CString);
  ACE_CString identifier_name (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *identifier_;
  TAO_PSDL_Node *specification_;
  ACE_CString module_name_;
};

// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Abstract_Storagetype : public TAO_PSDL_Node
{
  // For abstract_storagetype
public:
  TAO_PSDL_Abstract_Storagetype (TAO_PSDL_Node *storagetype_dcl);

  virtual ~TAO_PSDL_Abstract_Storagetype (void);

  /// Get the value
  TAO_PSDL_Node *storagetype_dcl (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *storagetype_dcl_;
};

// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Abstract_Storagetype_Dcl : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Abstract_Storagetype_Dcl (TAO_PSDL_Node *abs_storagetype_header);

  TAO_PSDL_Abstract_Storagetype_Dcl (TAO_PSDL_Node *abs_storagetype_header,
                                     TAO_PSDL_Node *abs_storagetype_body);

  virtual ~TAO_PSDL_Abstract_Storagetype_Dcl (void);

  /// Get the value
  TAO_PSDL_Node *abs_storagetype_header (void) const;
  TAO_PSDL_Node *abs_storagetype_body (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *abs_storagetype_header_;
  TAO_PSDL_Node *abs_storagetype_body_;

};

// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Abstract_Storagetype_Fwd_Dcl : public TAO_PSDL_Node
{
public:
  TAO_PSDL_Abstract_Storagetype_Fwd_Dcl (TAO_PSDL_Node *identifer);

  virtual ~TAO_PSDL_Abstract_Storagetype_Fwd_Dcl (void);

  /// Get the value
  TAO_PSDL_Node *identifier (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *identifier_;
};

// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Abstract_Storagetype_Header : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Abstract_Storagetype_Header (TAO_PSDL_Node *identifer);
  TAO_PSDL_Abstract_Storagetype_Header (TAO_PSDL_Node *identifer,
                                        TAO_PSDL_Node *abs_storagetype_inh_spec);

  virtual ~TAO_PSDL_Abstract_Storagetype_Header (void);

  /// Get the value
  TAO_PSDL_Node *identifier (void) const;
  TAO_PSDL_Node *abs_storagetype_inh_spec (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *identifier_;
  TAO_PSDL_Node *abs_storagetype_inh_spec_;
};

// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Abstract_Storagetype_Body : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Abstract_Storagetype_Body (TAO_PSDL_Node *abs_storagetype_member);
  TAO_PSDL_Abstract_Storagetype_Body (TAO_PSDL_Node *abs_storagetype_member,
                                      TAO_PSDL_Node *abs_storagetype_body);

  virtual ~TAO_PSDL_Abstract_Storagetype_Body (void);

  /// Get the value
  TAO_PSDL_Node *abs_storagetype_member (void) const;
  TAO_PSDL_Node *abs_storagetype_body (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *abs_storagetype_member_;
  TAO_PSDL_Node *abs_storagetype_body_;
};

// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Abstract_Storagetype_Member : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Abstract_Storagetype_Member (TAO_PSDL_Node *declaration);
  virtual ~TAO_PSDL_Abstract_Storagetype_Member (void);

  /// Get the value
  TAO_PSDL_Node *declaration (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *declaration_;
};

// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Abstract_Storagetype_Inh_Spec : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Abstract_Storagetype_Inh_Spec (TAO_PSDL_Node *abs_storagetype_name);
  virtual ~TAO_PSDL_Abstract_Storagetype_Inh_Spec (void);

  /// Get the value
  TAO_PSDL_Node *abs_storagetype_name (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *abs_storagetype_name_;
};

// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Abstract_Storagetype_Name : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Abstract_Storagetype_Name (TAO_PSDL_Node *scoped_name);
  TAO_PSDL_Abstract_Storagetype_Name (TAO_PSDL_Node *scoped_name,
                                      TAO_PSDL_Node *abs_storagetype_name);
  virtual ~TAO_PSDL_Abstract_Storagetype_Name (void);

  /// Get the value
  TAO_PSDL_Node *scoped_name (void) const;
  TAO_PSDL_Node *abs_storagetype_name (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *scoped_name_;
  TAO_PSDL_Node *abs_storagetype_name_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Psdl_State_Dcl : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Psdl_State_Dcl (TAO_PSDL_Node *key_word,
                           TAO_PSDL_Node *psdl_state_type_spec,
                           TAO_PSDL_Node *simple_declarator);

  TAO_PSDL_Psdl_State_Dcl (TAO_PSDL_Node *psdl_state_type_spec,
                           TAO_PSDL_Node *simple_declarator);

  virtual ~TAO_PSDL_Psdl_State_Dcl (void);

  /// Get the value
  TAO_PSDL_Node *psdl_state_type_spec (void) const;
  TAO_PSDL_Node *simple_declarator (void) const;
  TAO_PSDL_Node *key_word (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *key_word_;
  TAO_PSDL_Node *psdl_state_type_spec_;
  TAO_PSDL_Node *simple_declarator_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Psdl_State_Type_Spec : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Psdl_State_Type_Spec (TAO_PSDL_Node *type_spec);

  virtual ~TAO_PSDL_Psdl_State_Type_Spec (void);

  /// Get the value
  TAO_PSDL_Node *type_spec (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *type_spec_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Abstract_Storagetype_Ref_Type : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Abstract_Storagetype_Ref_Type (TAO_PSDL_Node *key_word,
                                          TAO_PSDL_Node *abs_storagetype_name);

  TAO_PSDL_Abstract_Storagetype_Ref_Type (TAO_PSDL_Node *abs_storagetype_name);

  virtual ~TAO_PSDL_Abstract_Storagetype_Ref_Type (void);

  /// Get the value
  TAO_PSDL_Node *key_word (void) const;
  TAO_PSDL_Node *abs_storagetype_name (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *key_word_;
  TAO_PSDL_Node *abs_storagetype_name_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Abstract_Storagehome : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Abstract_Storagehome (TAO_PSDL_Node *abs_storagehome_dcl);

  virtual ~TAO_PSDL_Abstract_Storagehome (void);

  /// Get the value
  TAO_PSDL_Node *abs_storagehome_dcl (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *abs_storagehome_dcl_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Abstract_Storagehome_Fwd_Dcl : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Abstract_Storagehome_Fwd_Dcl (TAO_PSDL_Node *identifier);

  virtual ~TAO_PSDL_Abstract_Storagehome_Fwd_Dcl (void);

  /// Get the value
  TAO_PSDL_Node *identifier (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *identifier_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Abstract_Storagehome_Dcl : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Abstract_Storagehome_Dcl (TAO_PSDL_Node *abs_storagehome_header);

  TAO_PSDL_Abstract_Storagehome_Dcl (TAO_PSDL_Node *abs_storagehome_header,
                                     TAO_PSDL_Node *abs_storagehome_body);

  virtual ~TAO_PSDL_Abstract_Storagehome_Dcl (void);

  /// Get the value
  TAO_PSDL_Node *abs_storagehome_header (void) const;
  TAO_PSDL_Node *abs_storagehome_body (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *abs_storagehome_header_;
  TAO_PSDL_Node *abs_storagehome_body_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Abstract_Storagehome_Header : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Abstract_Storagehome_Header (TAO_PSDL_Node *identifier,
                                        TAO_PSDL_Node *abs_storagetype_name);

  TAO_PSDL_Abstract_Storagehome_Header (TAO_PSDL_Node *identifier,
                                        TAO_PSDL_Node *abs_storagetype_name,
                                        TAO_PSDL_Node *abs_storagehome_inh_spec);

  virtual ~TAO_PSDL_Abstract_Storagehome_Header (void);

  /// Get the value
  TAO_PSDL_Node *identifier (void) const;
  TAO_PSDL_Node *abs_storagetype_name (void) const;
  TAO_PSDL_Node *abs_storagehome_inh_spec (void) const;


  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *identifier_;
  TAO_PSDL_Node *abs_storagetype_name_;
  TAO_PSDL_Node *abs_storagehome_inh_spec_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Abstract_Storagehome_Body : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Abstract_Storagehome_Body ();

  TAO_PSDL_Abstract_Storagehome_Body (TAO_PSDL_Node *abs_storagehome_member);

  TAO_PSDL_Abstract_Storagehome_Body (TAO_PSDL_Node *abs_storagehome_member,
                                      TAO_PSDL_Node *abs_storagehome_body);

  virtual ~TAO_PSDL_Abstract_Storagehome_Body (void);

  /// Get the value
  TAO_PSDL_Node *abs_storagehome_member (void) const;
  TAO_PSDL_Node *abs_storagehome_body (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *abs_storagehome_member_;
  TAO_PSDL_Node *abs_storagehome_body_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Abstract_Storagehome_Member : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Abstract_Storagehome_Member (TAO_PSDL_Node *declaration_type);

  virtual ~TAO_PSDL_Abstract_Storagehome_Member (void);

  /// Get the value
  TAO_PSDL_Node *declaration_type (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *declaration_type_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Abstract_Storagehome_Inh_Spec : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Abstract_Storagehome_Inh_Spec (TAO_PSDL_Node *abs_storagehome_name);

  virtual ~TAO_PSDL_Abstract_Storagehome_Inh_Spec (void);

  /// Get the value
  TAO_PSDL_Node *abs_storagehome_name (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *abs_storagehome_name_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Abstract_Storagehome_Name : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Abstract_Storagehome_Name (TAO_PSDL_Node *scoped_name);

  TAO_PSDL_Abstract_Storagehome_Name (TAO_PSDL_Node *scoped_name,
                                      TAO_PSDL_Node *abs_storagehome_name);

  virtual ~TAO_PSDL_Abstract_Storagehome_Name (void);

  /// Get the value
  TAO_PSDL_Node *scoped_name (void) const;
  TAO_PSDL_Node *abs_storagehome_name (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *scoped_name_;
  TAO_PSDL_Node *abs_storagehome_name_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Local_Op_Dcl : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Local_Op_Dcl (TAO_PSDL_Node *op_type_spec,
                         TAO_PSDL_Node *identifier,
                         TAO_PSDL_Node *parameter_dcls,
                         TAO_PSDL_Node *raises_expr);

  TAO_PSDL_Local_Op_Dcl (TAO_PSDL_Node *op_type_spec,
                         TAO_PSDL_Node *identifier,
                         TAO_PSDL_Node *parameter_dcls);

  virtual ~TAO_PSDL_Local_Op_Dcl (void);

  /// Get the value
  TAO_PSDL_Node *op_type_spec (void) const;
  TAO_PSDL_Node *identifier (void) const;
  TAO_PSDL_Node *parameter_dcls (void) const;
  TAO_PSDL_Node *raises_expr (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *op_type_spec_;
  TAO_PSDL_Node *identifier_;
  TAO_PSDL_Node *parameter_dcls_;
  TAO_PSDL_Node *raises_expr_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Key_Dcl : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Key_Dcl (TAO_PSDL_Node *identifier,
                    TAO_PSDL_Node *simple_declarator);

  TAO_PSDL_Key_Dcl (TAO_PSDL_Node *identifier);

  virtual ~TAO_PSDL_Key_Dcl (void);

  /// Get the value
  TAO_PSDL_Node *identifier (void) const;
  TAO_PSDL_Node *simple_declarator (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *identifier_;
  TAO_PSDL_Node *simple_declarator_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Catalog : public TAO_PSDL_Node
{
public:
  TAO_PSDL_Catalog (TAO_PSDL_Node *identifier);

  TAO_PSDL_Catalog (TAO_PSDL_Node *identifier,
                    TAO_PSDL_Node *catalog_inh_spec,
                    TAO_PSDL_Node *catalog_body);

  TAO_PSDL_Catalog (TAO_PSDL_Node *identifier,
                    TAO_PSDL_Node *catalog_body);

  virtual ~TAO_PSDL_Catalog (void);

  /// Get the value
  TAO_PSDL_Node *identifier (void) const;
  TAO_PSDL_Node *catalog_inh_spec (void) const;
  TAO_PSDL_Node *catalog_body (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *identifier_;
  TAO_PSDL_Node *catalog_inh_spec_;
  TAO_PSDL_Node *catalog_body_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Catalog_Inh_Spec : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Catalog_Inh_Spec (TAO_PSDL_Node *catalog_name);

  virtual ~TAO_PSDL_Catalog_Inh_Spec (void);

  /// Get the value
  TAO_PSDL_Node *catalog_name (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *catalog_name_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Catalog_Name : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Catalog_Name (TAO_PSDL_Node *scoped_name);
  TAO_PSDL_Catalog_Name (TAO_PSDL_Node *scoped_name,
                         TAO_PSDL_Node *catalog_name);

  virtual ~TAO_PSDL_Catalog_Name (void);

  /// Get the value
  TAO_PSDL_Node *scoped_name (void) const;
  TAO_PSDL_Node *catalog_name (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *scoped_name_;
  TAO_PSDL_Node *catalog_name_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Catalog_Body : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Catalog_Body ();
  TAO_PSDL_Catalog_Body (TAO_PSDL_Node *catalog_member);
  TAO_PSDL_Catalog_Body (TAO_PSDL_Node *catalog_member,
                         TAO_PSDL_Node *catalog_body);

  virtual ~TAO_PSDL_Catalog_Body (void);

  /// Get the value
  TAO_PSDL_Node *catalog_member (void) const;
  TAO_PSDL_Node *catalog_body (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *catalog_member_;
  TAO_PSDL_Node *catalog_body_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Catalog_Member : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Catalog_Member (TAO_PSDL_Node *declaration);

  virtual ~TAO_PSDL_Catalog_Member (void);

  /// Get the value
  TAO_PSDL_Node *declaration (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *declaration_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Provides_Dcl : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Provides_Dcl (TAO_PSDL_Node *abs_storagehome_name,
                         TAO_PSDL_Node *simple_declarator);

  virtual ~TAO_PSDL_Provides_Dcl (void);

  /// Get the value
  TAO_PSDL_Node *abs_storagehome_name (void) const;
  TAO_PSDL_Node *simple_declarator (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *abs_storagehome_name_;
  TAO_PSDL_Node *simple_declarator_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Storagetype : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Storagetype (TAO_PSDL_Node *declaration);

  virtual ~TAO_PSDL_Storagetype (void);

  /// Get the value
  TAO_PSDL_Node *declaration (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *declaration_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Storagetype_Dcl : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Storagetype_Dcl (TAO_PSDL_Node *storagetype_header);

  TAO_PSDL_Storagetype_Dcl (TAO_PSDL_Node *storagetype_header,
                            TAO_PSDL_Node *storagetype_body);

  virtual ~TAO_PSDL_Storagetype_Dcl (void);

  /// Get the value
  TAO_PSDL_Node *storagetype_header (void) const;
  TAO_PSDL_Node *storagetype_body (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *storagetype_header_;
  TAO_PSDL_Node *storagetype_body_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Storagetype_Fwd_Dcl : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Storagetype_Fwd_Dcl (TAO_PSDL_Node *identifier);

  virtual ~TAO_PSDL_Storagetype_Fwd_Dcl (void);

  /// Get the value
  TAO_PSDL_Node *identifier (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *identifier_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Storagetype_Header : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Storagetype_Header (TAO_PSDL_Node *identifier,
                               TAO_PSDL_Node *storagetype_inh_spec,
                               TAO_PSDL_Node *storagetype_impl_spec);

  TAO_PSDL_Storagetype_Header (TAO_PSDL_Node *identifier,
                               TAO_PSDL_Node *storagetype_inh_spec);

  TAO_PSDL_Storagetype_Header (TAO_PSDL_Node *identifier);

  virtual ~TAO_PSDL_Storagetype_Header (void);

  /// Get the value
  TAO_PSDL_Node *identifier (void) const;
  TAO_PSDL_Node *storagetype_inh_spec (void) const;
  TAO_PSDL_Node *storagetype_impl_spec (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *identifier_;
  TAO_PSDL_Node *storagetype_inh_spec_;
  TAO_PSDL_Node *storagetype_impl_spec_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Storagetype_Body : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Storagetype_Body ();

  TAO_PSDL_Storagetype_Body (TAO_PSDL_Node *storagetype_member);

  TAO_PSDL_Storagetype_Body (TAO_PSDL_Node *storagetype_member,
                             TAO_PSDL_Node *storagetype_body);

  virtual ~TAO_PSDL_Storagetype_Body (void);

  /// Get the value
  TAO_PSDL_Node *storagetype_member (void) const;
  TAO_PSDL_Node *storagetype_body (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *storagetype_member_;
  TAO_PSDL_Node *storagetype_body_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Storagetype_Member : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Storagetype_Member (TAO_PSDL_Node *declaration);

  virtual ~TAO_PSDL_Storagetype_Member (void);

  /// Get the value
  TAO_PSDL_Node *declaration (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *declaration_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Storagetype_Inh_Spec : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Storagetype_Inh_Spec (TAO_PSDL_Node *storagetype_name);

  virtual ~TAO_PSDL_Storagetype_Inh_Spec (void);

  /// Get the value
  TAO_PSDL_Node *storagetype_name (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *storagetype_name_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Storagetype_Name : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Storagetype_Name (TAO_PSDL_Node *scoped_name);

  virtual ~TAO_PSDL_Storagetype_Name (void);

  /// Get the value
  TAO_PSDL_Node *scoped_name (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *scoped_name_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Storagetype_Impl_Spec : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Storagetype_Impl_Spec (TAO_PSDL_Node *abstract_storagetype_name);

  virtual ~TAO_PSDL_Storagetype_Impl_Spec (void);

  /// Get the value
  TAO_PSDL_Node *abstract_storagetype_name (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *abstract_storagetype_name_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Storagetype_Ref_Type : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Storagetype_Ref_Type (TAO_PSDL_Node *storagetype_name);

  virtual ~TAO_PSDL_Storagetype_Ref_Type (void);

  /// Get the value
  TAO_PSDL_Node *storagetype_name (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *storagetype_name_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Storagehome_Scope : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Storagehome_Scope (TAO_PSDL_Node *storagehome_name);

  virtual ~TAO_PSDL_Storagehome_Scope (void);

  /// Get the value
  TAO_PSDL_Node *storagehome_name (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *storagehome_name_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Store_Directive : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Store_Directive (TAO_PSDL_Node *simple_declarator,
                            TAO_PSDL_Node *psdl_concrete_state_type,
                            TAO_PSDL_Node *storagehome_scope);

  TAO_PSDL_Store_Directive (TAO_PSDL_Node *simple_declarator,
                            TAO_PSDL_Node *psdl_concrete_state_type);

  virtual ~TAO_PSDL_Store_Directive (void);

  /// Get the value
  TAO_PSDL_Node *simple_declarator (void) const;
  TAO_PSDL_Node *psdl_concrete_state_type (void) const;
  TAO_PSDL_Node *storagehome_scope (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *simple_declarator_;
  TAO_PSDL_Node *psdl_concrete_state_type_;
  TAO_PSDL_Node *storagehome_scope_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Psdl_Concrete_State_Type : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Psdl_Concrete_State_Type (TAO_PSDL_Node *storagetype);

  virtual ~TAO_PSDL_Psdl_Concrete_State_Type (void);

  /// Get the value
  TAO_PSDL_Node *storagetype (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *storagetype_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Ref_Rep_Directive : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Ref_Rep_Directive (TAO_PSDL_Node *simple_declarator);

  virtual ~TAO_PSDL_Ref_Rep_Directive (void);

  /// Get the value
  TAO_PSDL_Node *simple_declarator (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *simple_declarator_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Storagehome : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Storagehome (TAO_PSDL_Node *storagehome_header);

  TAO_PSDL_Storagehome (TAO_PSDL_Node *storagehome_header,
                        TAO_PSDL_Node *storagehome_body);

  virtual ~TAO_PSDL_Storagehome (void);

  /// Get the value
  TAO_PSDL_Node *storagehome_header (void) const;
  TAO_PSDL_Node *storagehome_body (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *storagehome_header_;
  TAO_PSDL_Node *storagehome_body_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Storagehome_Header : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Storagehome_Header (TAO_PSDL_Node *identifier,
                               TAO_PSDL_Node *storagetype_name,
                               TAO_PSDL_Node *storagehome_inh_spec,
                               TAO_PSDL_Node *storagehome_impl_spec);

  TAO_PSDL_Storagehome_Header (TAO_PSDL_Node *identifier,
                               TAO_PSDL_Node *storagetype_name,
                               TAO_PSDL_Node *storagehome_inh_spec);

  TAO_PSDL_Storagehome_Header (TAO_PSDL_Node *identifier,
                               TAO_PSDL_Node *storagetype_name);

  virtual ~TAO_PSDL_Storagehome_Header (void);

  /// Get the value
  TAO_PSDL_Node *identifier (void) const;
  TAO_PSDL_Node *storagetype_name (void) const;
  TAO_PSDL_Node *storagehome_inh_spec (void) const;
  TAO_PSDL_Node *storagehome_impl_spec (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *identifier_;
  TAO_PSDL_Node *storagetype_name_;
  TAO_PSDL_Node *storagehome_inh_spec_;
  TAO_PSDL_Node *storagehome_impl_spec_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Storagehome_Body : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Storagehome_Body ();

  TAO_PSDL_Storagehome_Body (TAO_PSDL_Node *storagehome_member);

  TAO_PSDL_Storagehome_Body (TAO_PSDL_Node *storagehome_member,
                             TAO_PSDL_Node *storagehome_body);

  virtual ~TAO_PSDL_Storagehome_Body (void);

  /// Get the value
  TAO_PSDL_Node *storagehome_member (void) const;
  TAO_PSDL_Node *storagehome_body (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *storagehome_member_;
  TAO_PSDL_Node *storagehome_body_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Storagehome_Member : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Storagehome_Member (TAO_PSDL_Node *key_dcl);

  virtual ~TAO_PSDL_Storagehome_Member (void);

  /// Get the value
  TAO_PSDL_Node *key_dcl (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *key_dcl_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Storagehome_Inh_Spec : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Storagehome_Inh_Spec (TAO_PSDL_Node *storagehome_name);

  virtual ~TAO_PSDL_Storagehome_Inh_Spec (void);

  /// Get the value
  TAO_PSDL_Node *storagehome_name (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *storagehome_name_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Storagehome_Name : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Storagehome_Name (TAO_PSDL_Node *scoped_name);

  virtual ~TAO_PSDL_Storagehome_Name (void);

  /// Get the value
  TAO_PSDL_Node *scoped_name (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *scoped_name_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Storagehome_Impl_Spec : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Storagehome_Impl_Spec (TAO_PSDL_Node *abstract_storagehome_name);

  virtual ~TAO_PSDL_Storagehome_Impl_Spec (void);

  /// Get the value
  TAO_PSDL_Node *abstract_storagehome_name (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *abstract_storagehome_name_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Primary_Key_Dcl : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Primary_Key_Dcl ();

  TAO_PSDL_Primary_Key_Dcl (TAO_PSDL_Node *identifier);

  virtual ~TAO_PSDL_Primary_Key_Dcl (void);

  /// Get the value
  TAO_PSDL_Node *identifier (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *identifier_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Identifier : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Identifier (char *value);

  TAO_PSDL_Identifier (int value_type);

  virtual ~TAO_PSDL_Identifier (void);

  /// Get the value
  char *value (void) const;

  int value_type (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  char *value_;
  int value_type_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Type_Dcl : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Type_Dcl (TAO_PSDL_Node *type_of_type_dcl);

  TAO_PSDL_Type_Dcl (int key_word,
                     TAO_PSDL_Node *type_of_type_dcl);

  virtual ~TAO_PSDL_Type_Dcl (void);

  /// Get the value
  int key_word (void) const;
  TAO_PSDL_Node *type_of_type_dcl (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  int key_word_;
  TAO_PSDL_Node *type_of_type_dcl_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Type_Declarator : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Type_Declarator (TAO_PSDL_Node *type_spec,
                            TAO_PSDL_Node *declarators);

  virtual ~TAO_PSDL_Type_Declarator (void);

  /// Get the value
  TAO_PSDL_Node *type_spec (void) const;
  TAO_PSDL_Node *declarators (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *type_spec_;
  TAO_PSDL_Node *declarators_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Type_Spec : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Type_Spec (TAO_PSDL_Node *type_of_type_spec);

  virtual ~TAO_PSDL_Type_Spec (void);

  /// Get the value
  TAO_PSDL_Node *type_of_type_spec (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *type_of_type_spec_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Simple_Type_Spec : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Simple_Type_Spec (TAO_PSDL_Node *type_of_simple_type_spec);

  virtual ~TAO_PSDL_Simple_Type_Spec (void);

  /// Get the value
  TAO_PSDL_Node *type_of_simple_type_spec (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *type_of_simple_type_spec_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Base_Type_Spec : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Base_Type_Spec (TAO_PSDL_Node *type_of_base_type_spec);

  virtual ~TAO_PSDL_Base_Type_Spec (void);

  /// Get the value
  TAO_PSDL_Node *type_of_base_type_spec (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *type_of_base_type_spec_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Template_Type_Spec : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Template_Type_Spec (TAO_PSDL_Node *type_of_template_type_spec);

  virtual ~TAO_PSDL_Template_Type_Spec (void);

  /// Get the value
  TAO_PSDL_Node *type_of_template_type_spec (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *type_of_template_type_spec_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Constr_Type_Spec : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Constr_Type_Spec (TAO_PSDL_Node *type_of_constr_type_spec);

  virtual ~TAO_PSDL_Constr_Type_Spec (void);

  /// Get the value
  TAO_PSDL_Node *type_of_constr_type_spec (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *type_of_constr_type_spec_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Declarators : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Declarators (TAO_PSDL_Node *declarator);

  TAO_PSDL_Declarators (TAO_PSDL_Node *declarator,
                        TAO_PSDL_Node *set_of_declarators);

  virtual ~TAO_PSDL_Declarators (void);

  /// Get the value
  TAO_PSDL_Node *declarator (void) const;
  TAO_PSDL_Node *set_of_declarators (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *declarator_;
  TAO_PSDL_Node *set_of_declarators_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Declarator : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Declarator (TAO_PSDL_Node *type_of_declarator);

  virtual ~TAO_PSDL_Declarator (void);

  /// Get the value
  TAO_PSDL_Node *type_of_declarator (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *type_of_declarator_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Simple_Declarator : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Simple_Declarator (TAO_PSDL_Node *identifier);
  TAO_PSDL_Simple_Declarator (TAO_PSDL_Node *identifier,
                              TAO_PSDL_Node *simple_declarator);

  virtual ~TAO_PSDL_Simple_Declarator (void);

  /// Get the value
  TAO_PSDL_Node *identifier (void) const;
  TAO_PSDL_Node *simple_declarator (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *identifier_;
  TAO_PSDL_Node *simple_declarator_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Simple_Declarator_List : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Simple_Declarator_List (TAO_PSDL_Node *simple_declarator);

  TAO_PSDL_Simple_Declarator_List (TAO_PSDL_Node *simple_declarator_list,
                                   TAO_PSDL_Node *simple_declarator);

  virtual ~TAO_PSDL_Simple_Declarator_List (void);

  /// Get the value
  TAO_PSDL_Node *simple_declarator_list (void) const;
  TAO_PSDL_Node *simple_declarator (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *simple_declarator_list_;
  TAO_PSDL_Node *simple_declarator_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Complex_Declarator : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Complex_Declarator (TAO_PSDL_Node *array_declarator);

  virtual ~TAO_PSDL_Complex_Declarator (void);

  /// Get the value
  TAO_PSDL_Node *array_declarator (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *array_declarator_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Predefined_Type : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Predefined_Type (int type_one);

  TAO_PSDL_Predefined_Type (int type_one,
                            int type_two);

  TAO_PSDL_Predefined_Type (int type_one,
                            int type_two,
                            int type_three);

  TAO_PSDL_Predefined_Type (int type_one,
                            TAO_PSDL_Node *type_of_variable_one);

  TAO_PSDL_Predefined_Type (TAO_PSDL_Node *type_of_variable_one);

  TAO_PSDL_Predefined_Type (int type_one,
                            TAO_PSDL_Node *type_of_variable_one,
                            TAO_PSDL_Node *type_of_variable_two);

  TAO_PSDL_Predefined_Type (TAO_PSDL_Node *type_of_variable_one,
                            TAO_PSDL_Node *type_of_variable_two);

  TAO_PSDL_Predefined_Type (TAO_PSDL_Node *type_of_variable_one,
                            TAO_PSDL_Node *type_of_variable_two,
                            TAO_PSDL_Node *type_of_variable_three);

  virtual ~TAO_PSDL_Predefined_Type (void);

  /// Get the value
  int type_one (void) const;
  int type_two (void) const;
  int type_three (void) const;
  TAO_PSDL_Node *type_of_variable_one (void) const;
  TAO_PSDL_Node *type_of_variable_two (void) const;
  TAO_PSDL_Node *type_of_variable_three (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  int type_one_;
  int type_two_;
  int type_three_;

  TAO_PSDL_Node *type_of_variable_one_;
  TAO_PSDL_Node *type_of_variable_two_;
  TAO_PSDL_Node *type_of_variable_three_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Member_List : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Member_List (TAO_PSDL_Node *member);
  TAO_PSDL_Member_List (TAO_PSDL_Node *member,
                        TAO_PSDL_Node *member_list);

  virtual ~TAO_PSDL_Member_List (void);

  /// Get the value
  TAO_PSDL_Node *member (void) const;
  TAO_PSDL_Node *member_list (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *member_;
  TAO_PSDL_Node *member_list_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Member : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Member (TAO_PSDL_Node *type_spec,
                   TAO_PSDL_Node *declarators);

  virtual ~TAO_PSDL_Member (void);

  /// Get the value
  TAO_PSDL_Node *type_spec (void) const;
  TAO_PSDL_Node *declarators (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *type_spec_;
  TAO_PSDL_Node *declarators_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Union_Type : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Union_Type (TAO_PSDL_Node *identifier,
                       TAO_PSDL_Node *switch_type_spec,
                       TAO_PSDL_Node *switch_body);

  virtual ~TAO_PSDL_Union_Type (void);

  /// Get the value
  TAO_PSDL_Node *identifier (void) const;
  TAO_PSDL_Node *switch_type_spec (void) const;
  TAO_PSDL_Node *switch_body (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *identifier_;
  TAO_PSDL_Node *switch_type_spec_;
  TAO_PSDL_Node *switch_body_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Switch_Type_Spec : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Switch_Type_Spec (TAO_PSDL_Node *type);

  virtual ~TAO_PSDL_Switch_Type_Spec (void);

  /// Get the value
  TAO_PSDL_Node *type (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *type_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Switch_Body : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Switch_Body (TAO_PSDL_Node *switch_case);

  TAO_PSDL_Switch_Body (TAO_PSDL_Node *switch_case,
                        TAO_PSDL_Node *switch_body);

  virtual ~TAO_PSDL_Switch_Body (void);

  /// Get the value
  TAO_PSDL_Node *switch_case (void) const;
  TAO_PSDL_Node *switch_body (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *switch_case_;
  TAO_PSDL_Node *switch_body_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Case : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Case (TAO_PSDL_Node *case_label,
                 TAO_PSDL_Node *element_spec);

  virtual ~TAO_PSDL_Case (void);

  /// Get the value
  TAO_PSDL_Node *case_label (void) const;
  TAO_PSDL_Node *element_spec (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *case_label_;
  TAO_PSDL_Node *element_spec_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Case_Label : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Case_Label (TAO_PSDL_Node * type);

  TAO_PSDL_Case_Label (TAO_PSDL_Node * type,
                       TAO_PSDL_Node *const_exp);

  TAO_PSDL_Case_Label (TAO_PSDL_Node * type,
                       TAO_PSDL_Node *const_exp,
                       TAO_PSDL_Node *case_label);

  virtual ~TAO_PSDL_Case_Label (void);

  /// Get the value
  TAO_PSDL_Node * type (void) const;
  TAO_PSDL_Node *const_exp (void) const;
  TAO_PSDL_Node *case_label (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node * type_;
  TAO_PSDL_Node *const_exp_;
  TAO_PSDL_Node *case_label_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Element_Spec : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Element_Spec (TAO_PSDL_Node *type_spec,
                         TAO_PSDL_Node *declarator);

  virtual ~TAO_PSDL_Element_Spec (void);

  /// Get the value
  TAO_PSDL_Node *type_spec (void) const;
  TAO_PSDL_Node *declarator (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *type_spec_;
  TAO_PSDL_Node *declarator_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Const_Dcl : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Const_Dcl (TAO_PSDL_Node *const_type,
                      TAO_PSDL_Node *identifier,
                      TAO_PSDL_Node *const_exp);

  virtual ~TAO_PSDL_Const_Dcl (void);

  /// Get the value
  TAO_PSDL_Node *const_type (void) const;
  TAO_PSDL_Node *identifier (void) const;
  TAO_PSDL_Node *const_exp (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *const_type_;
  TAO_PSDL_Node *identifier_;
  TAO_PSDL_Node *const_exp_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Const_Type : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Const_Type (TAO_PSDL_Node *type);

  virtual ~TAO_PSDL_Const_Type (void);

  /// Get the value
  TAO_PSDL_Node *type (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *type_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Const_Exp : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Const_Exp (TAO_PSDL_Node *expr);

  virtual ~TAO_PSDL_Const_Exp (void);

  /// Get the value
  TAO_PSDL_Node *expr (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *expr_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Or_Expr : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Or_Expr (TAO_PSDL_Node *xor_expr);
  TAO_PSDL_Or_Expr (TAO_PSDL_Node *or_expr,
                   TAO_PSDL_Node *xor_expr);

  virtual ~TAO_PSDL_Or_Expr (void);

  /// Get the value
  TAO_PSDL_Node *or_expr (void) const;
  TAO_PSDL_Node *xor_expr (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *or_expr_;
  TAO_PSDL_Node *xor_expr_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Xor_Expr : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Xor_Expr (TAO_PSDL_Node *and_expr);
  TAO_PSDL_Xor_Expr (TAO_PSDL_Node *xor_expr,
                   TAO_PSDL_Node *and_expr);

  virtual ~TAO_PSDL_Xor_Expr (void);

  /// Get the value
  TAO_PSDL_Node *xor_expr (void) const;
  TAO_PSDL_Node *and_expr (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *xor_expr_;
  TAO_PSDL_Node *and_expr_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_And_Expr : public TAO_PSDL_Node
{
public:

  TAO_PSDL_And_Expr (TAO_PSDL_Node *shift_expr);
  TAO_PSDL_And_Expr (TAO_PSDL_Node *and_expr,
                     TAO_PSDL_Node *shift_expr);

  virtual ~TAO_PSDL_And_Expr (void);

  /// Get the value
  TAO_PSDL_Node *and_expr (void) const;
  TAO_PSDL_Node *shift_expr (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *and_expr_;
  TAO_PSDL_Node *shift_expr_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Shift_Expr : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Shift_Expr (TAO_PSDL_Node *add_expr);

  TAO_PSDL_Shift_Expr (TAO_PSDL_Node *shift_expr,
                       TAO_PSDL_Node * type,
                       TAO_PSDL_Node *add_expr);

  virtual ~TAO_PSDL_Shift_Expr (void);

  /// Get the value
  TAO_PSDL_Node * type (void) const;
  TAO_PSDL_Node *add_expr (void) const;
  TAO_PSDL_Node *shift_expr (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *shift_expr_;
  TAO_PSDL_Node * type_;
  TAO_PSDL_Node *add_expr_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Add_Expr : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Add_Expr (TAO_PSDL_Node *mult_expr);
  TAO_PSDL_Add_Expr (TAO_PSDL_Node *add_expr,
                     TAO_PSDL_Node * type,
                     TAO_PSDL_Node *mult_expr);

  virtual ~TAO_PSDL_Add_Expr (void);

  /// Get the value
  TAO_PSDL_Node * type (void) const;
  TAO_PSDL_Node *add_expr (void) const;
  TAO_PSDL_Node *mult_expr (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *add_expr_;
  TAO_PSDL_Node * type_;
  TAO_PSDL_Node *mult_expr_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Mult_Expr : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Mult_Expr (TAO_PSDL_Node *unary_expr);

  TAO_PSDL_Mult_Expr (TAO_PSDL_Node *mult_expr,
                      TAO_PSDL_Node *unary_expr);

  TAO_PSDL_Mult_Expr (TAO_PSDL_Node *mult_expr,
                      TAO_PSDL_Node * type,
                      TAO_PSDL_Node *unary_expr);

  virtual ~TAO_PSDL_Mult_Expr (void);

  /// Get the value
  TAO_PSDL_Node *type (void) const;
  TAO_PSDL_Node *unary_expr (void) const;
  TAO_PSDL_Node *mult_expr (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *mult_expr_;
  TAO_PSDL_Node * type_;
  TAO_PSDL_Node *unary_expr_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Unary_Expr : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Unary_Expr (TAO_PSDL_Node *unary_operator,
                       TAO_PSDL_Node *primary_expr);

  TAO_PSDL_Unary_Expr (TAO_PSDL_Node *primary_expr);

  virtual ~TAO_PSDL_Unary_Expr (void);

  /// Get the value
  TAO_PSDL_Node *unary_operator (void) const;
  TAO_PSDL_Node *primary_expr (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *unary_operator_;
  TAO_PSDL_Node *primary_expr_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Primary_Expr : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Primary_Expr (TAO_PSDL_Node *type_of_primary_expr);

  virtual ~TAO_PSDL_Primary_Expr (void);

  /// Get the value
  TAO_PSDL_Node *type_of_primary_expr (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *type_of_primary_expr_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Except_Dcl : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Except_Dcl (TAO_PSDL_Node *identifier);

  TAO_PSDL_Except_Dcl (TAO_PSDL_Node *identifier,
                       TAO_PSDL_Node *member_list);

  virtual ~TAO_PSDL_Except_Dcl (void);

  /// Get the value
  TAO_PSDL_Node *identifier (void) const;
  TAO_PSDL_Node *member_list (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *identifier_;
  TAO_PSDL_Node *member_list_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Op_Dcl : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Op_Dcl (TAO_PSDL_Node *op_attribute,
                   TAO_PSDL_Node *op_type_spec,
                   TAO_PSDL_Node *identifier,
                   TAO_PSDL_Node * parameter_dcls,
                   TAO_PSDL_Node * raises_expr,
                   TAO_PSDL_Node * context_expr);

  TAO_PSDL_Op_Dcl (TAO_PSDL_Node *op_type_spec,
                   TAO_PSDL_Node *identifier,
                   TAO_PSDL_Node * parameter_dcls,
                   TAO_PSDL_Node * raises_expr,
                   TAO_PSDL_Node * context_expr);

  TAO_PSDL_Op_Dcl (TAO_PSDL_Node *op_type_spec,
                   TAO_PSDL_Node *identifier,
                   TAO_PSDL_Node * parameter_dcls,
                   TAO_PSDL_Node * raises_expr);

  /*
   Other kinds of similar constructors.

   TAO_PSDL_Op_Dcl (TAO_PSDL_Node *op_attribute,
                    TAO_PSDL_Node *op_type_spec,
                    TAO_PSDL_Node *identifier,
                    TAO_PSDL_Node * parameter_dcls,
                    TAO_PSDL_Node * raises_expr);

  TAO_PSDL_Op_Dcl (TAO_PSDL_Node *op_attribute,
                   TAO_PSDL_Node *op_type_spec,
                   TAO_PSDL_Node *identifier,
                   TAO_PSDL_Node * parameter_dcls,
                   TAO_PSDL_Node * context_expr);
  */

  TAO_PSDL_Op_Dcl (TAO_PSDL_Node *op_type_spec,
                   TAO_PSDL_Node *identifier,
                   TAO_PSDL_Node * parameter_dcls);

  virtual ~TAO_PSDL_Op_Dcl (void);

  /// Get the value
  TAO_PSDL_Node *op_attribute (void) const;
  TAO_PSDL_Node *op_type_spec (void) const;
  TAO_PSDL_Node *identifier (void) const;
  TAO_PSDL_Node * parameter_dcls (void) const;
  TAO_PSDL_Node * raises_expr (void) const;
  TAO_PSDL_Node * context_expr (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *op_attribute_;
  TAO_PSDL_Node *op_type_spec_;
  TAO_PSDL_Node *identifier_;
  TAO_PSDL_Node * parameter_dcls_;
  TAO_PSDL_Node * raises_expr_;
  TAO_PSDL_Node * context_expr_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Op_Type_Spec : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Op_Type_Spec (TAO_PSDL_Node *type_spec);

  TAO_PSDL_Op_Type_Spec (int type);

  virtual ~TAO_PSDL_Op_Type_Spec (void);

  /// Get the value
  TAO_PSDL_Node *type_spec (void) const;
  int type (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node * type_spec_;
  int type_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Parameter_Dcls : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Parameter_Dcls ();
  TAO_PSDL_Parameter_Dcls (TAO_PSDL_Node *param_dcl);

  virtual ~TAO_PSDL_Parameter_Dcls (void);

  /// Get the value
  TAO_PSDL_Node *param_dcl (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *param_dcl_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Param_Dcl : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Param_Dcl (TAO_PSDL_Node *param_attribute,
                      TAO_PSDL_Node *param_type_spec,
                      TAO_PSDL_Node *simple_declarator);

  TAO_PSDL_Param_Dcl (TAO_PSDL_Node *param_attribute,
                      TAO_PSDL_Node *param_type_spec,
                      TAO_PSDL_Node *simple_declarator,
                      TAO_PSDL_Node *param_dcl);

  virtual ~TAO_PSDL_Param_Dcl (void);

  /// Get the value
  TAO_PSDL_Node *param_attribute (void) const;
  TAO_PSDL_Node *param_type_spec (void) const;
  TAO_PSDL_Node *simple_declarator (void) const;
  TAO_PSDL_Node *param_dcl (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *param_attribute_;
  TAO_PSDL_Node *param_type_spec_;
  TAO_PSDL_Node *simple_declarator_;
  TAO_PSDL_Node *param_dcl_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Raises_Expr : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Raises_Expr (TAO_PSDL_Node *first_scoped_name);

  TAO_PSDL_Raises_Expr (TAO_PSDL_Node *first_scoped_name,
                        TAO_PSDL_Node *second_scoped_name);

  virtual ~TAO_PSDL_Raises_Expr (void);

  /// Get the value
  TAO_PSDL_Node *first_scoped_name (void) const;
  TAO_PSDL_Node *second_scoped_name (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *first_scoped_name_;
  TAO_PSDL_Node *second_scoped_name_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Context_Expr : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Context_Expr (TAO_PSDL_Node *string_literal);

  virtual ~TAO_PSDL_Context_Expr (void);

  /// Get the value
  TAO_PSDL_Node *string_literal (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *string_literal_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Param_Type_Spec : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Param_Type_Spec (TAO_PSDL_Node *type_of_param);

  virtual ~TAO_PSDL_Param_Type_Spec (void);

  /// Get the value
  TAO_PSDL_Node *type_of_param (void) const;


  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *type_of_param_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Fixed_Pt_Type : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Fixed_Pt_Type (TAO_PSDL_Node *positive_int_const_one,
                          TAO_PSDL_Node *positive_int_const_two);

  virtual ~TAO_PSDL_Fixed_Pt_Type (void);

  /// Get the value
  TAO_PSDL_Node *positive_int_const_one (void) const;
  TAO_PSDL_Node *positive_int_const_two (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *positive_int_const_one_;
  TAO_PSDL_Node *positive_int_const_two_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Constr_Forward_Decl : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Constr_Forward_Decl (TAO_PSDL_Node * type,
                                TAO_PSDL_Node *identifier);

  virtual ~TAO_PSDL_Constr_Forward_Decl (void);

  /// Get the value
  TAO_PSDL_Node * type (void) const;
  TAO_PSDL_Node *identifier (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node * type_;
  TAO_PSDL_Node *identifier_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Interface : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Interface (TAO_PSDL_Node *type_of_dcl);

  virtual ~TAO_PSDL_Interface (void);

  /// Get the value
  TAO_PSDL_Node *type_of_dcl (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *type_of_dcl_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Interface_Body : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Interface_Body (TAO_PSDL_Node *export_variable);

  TAO_PSDL_Interface_Body (TAO_PSDL_Node *export_variable,
                           TAO_PSDL_Node *interface_body);

  virtual ~TAO_PSDL_Interface_Body (void);

  /// Get the value
  TAO_PSDL_Node *export_variable (void) const;
  TAO_PSDL_Node *interface_body (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:
  /// The values
  TAO_PSDL_Node *export_variable_;
  TAO_PSDL_Node *interface_body_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Interface_Dcl : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Interface_Dcl (TAO_PSDL_Node *interface_header);

  TAO_PSDL_Interface_Dcl (TAO_PSDL_Node *interface_header,
                          TAO_PSDL_Node *interface_body);

  virtual ~TAO_PSDL_Interface_Dcl (void);

  /// Get the value
  TAO_PSDL_Node *interface_header (void) const;
  TAO_PSDL_Node *interface_body (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:

  /// The values
  TAO_PSDL_Node *interface_header_;
  TAO_PSDL_Node *interface_body_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Forward_Dcl : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Forward_Dcl (TAO_PSDL_Node *identifier);

  TAO_PSDL_Forward_Dcl (int type,
                        TAO_PSDL_Node *identifier);

  virtual ~TAO_PSDL_Forward_Dcl (void);

  /// Get the value
  int type (void) const;
  TAO_PSDL_Node *identifier (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:

  /// The values
  int type_;
  TAO_PSDL_Node *identifier_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Interface_Header : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Interface_Header (TAO_PSDL_Node *identifier);

  TAO_PSDL_Interface_Header (TAO_PSDL_Node * type,
                             TAO_PSDL_Node *identifier);

  TAO_PSDL_Interface_Header (TAO_PSDL_Node * type,
                             TAO_PSDL_Node *identifier,
                             TAO_PSDL_Node *interface_inheritance_spec);

  /*
     Other Possible values.
     TAO_PSDL_Interface_Header (TAO_PSDL_Node *identifier,
                                TAO_PSDL_Node *interface_inheritance_spec);
  */

  virtual ~TAO_PSDL_Interface_Header (void);

  /// Get the value
  TAO_PSDL_Node * type (void) const;
  TAO_PSDL_Node *identifier (void) const;
  TAO_PSDL_Node *interface_inheritance_spec (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:

  /// The values

  /// the value of type can be either type_ or
  /// interface_inheritance_spec_ if interface_inheritance_spec_ is
  /// zero. If it is not zero, everything is straight forward.
  TAO_PSDL_Node * type_;
  TAO_PSDL_Node *identifier_;
  TAO_PSDL_Node *interface_inheritance_spec_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Export_Dcl : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Export_Dcl (TAO_PSDL_Node *type_of_export_one);

  TAO_PSDL_Export_Dcl (TAO_PSDL_Node *type_of_export_one,
                       TAO_PSDL_Node *type_of_export_two);

  virtual ~TAO_PSDL_Export_Dcl (void);

  /// Get the value
  TAO_PSDL_Node *type_of_export_one (void) const;
  TAO_PSDL_Node *type_of_export_two (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:

  /// The values
  TAO_PSDL_Node *type_of_export_one_;
  TAO_PSDL_Node *type_of_export_two_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Interface_Inheritance_Spec : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Interface_Inheritance_Spec (TAO_PSDL_Node *interface_name);

  virtual ~TAO_PSDL_Interface_Inheritance_Spec (void);

  /// Get the value
  TAO_PSDL_Node *interface_name (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:

  /// The values
  TAO_PSDL_Node *interface_name_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Interface_Name : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Interface_Name (TAO_PSDL_Node *scoped_name);
  TAO_PSDL_Interface_Name (TAO_PSDL_Node *scoped_name,
                           TAO_PSDL_Node *interface_name);

  virtual ~TAO_PSDL_Interface_Name (void);

  /// Get the value
  TAO_PSDL_Node *scoped_name (void) const;
  TAO_PSDL_Node *interface_name (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:

  /// The values
  TAO_PSDL_Node *scoped_name_;
  TAO_PSDL_Node *interface_name_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Scoped_Name : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Scoped_Name (TAO_PSDL_Node *identifier);
  TAO_PSDL_Scoped_Name (TAO_PSDL_Node *scoped_name,
                        TAO_PSDL_Node *identifier);

  virtual ~TAO_PSDL_Scoped_Name (void);

  /// Get the value
  TAO_PSDL_Node *identifier (void) const;
  TAO_PSDL_Node *scoped_name (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:

  /// The values
  TAO_PSDL_Node *identifier_;
  TAO_PSDL_Node *scoped_name_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Value : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Value (TAO_PSDL_Node *type_of_value_dcl);
  virtual ~TAO_PSDL_Value (void);

  /// Get the value
  TAO_PSDL_Node *type_of_value_dcl (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:

  /// The values
  TAO_PSDL_Node *type_of_value_dcl_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Value_Common_Base : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Value_Common_Base (TAO_PSDL_Node *identifier);
  virtual ~TAO_PSDL_Value_Common_Base (void);

  /// Get the value
  TAO_PSDL_Node *identifier (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:

  /// The values
  TAO_PSDL_Node *identifier_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Value_Forward_Dcl : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Value_Forward_Dcl (TAO_PSDL_Node *value_common_base);
  TAO_PSDL_Value_Forward_Dcl (TAO_PSDL_Node *abstract,
                              TAO_PSDL_Node *value_common_base);
  virtual ~TAO_PSDL_Value_Forward_Dcl (void);

  /// Get the value
  TAO_PSDL_Node *abstract (void) const;
  TAO_PSDL_Node *value_common_base (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:

  /// The values
  TAO_PSDL_Node *value_common_base_;
  TAO_PSDL_Node *abstract_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Value_Box_Dcl : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Value_Box_Dcl (TAO_PSDL_Node *value_common_base,
                          TAO_PSDL_Node *type_spec);
  virtual ~TAO_PSDL_Value_Box_Dcl (void);

  /// Get the value
  TAO_PSDL_Node *value_common_base (void) const;
  TAO_PSDL_Node *type_spec (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:

  /// The values
  TAO_PSDL_Node *value_common_base_;
  TAO_PSDL_Node *type_spec_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Value_Abs_Dcl : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Value_Abs_Dcl (TAO_PSDL_Node *value_common_base,
                          TAO_PSDL_Node *export_variable);

  TAO_PSDL_Value_Abs_Dcl (TAO_PSDL_Node *value_common_base,
                          TAO_PSDL_Node *value_inheritance_spec,
                          TAO_PSDL_Node *export_variable);

  virtual ~TAO_PSDL_Value_Abs_Dcl (void);

  /// Get the value
  TAO_PSDL_Node *value_common_base (void) const;
  TAO_PSDL_Node *value_inheritance_spec (void) const;
  TAO_PSDL_Node *export_variable (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:

  /// The values
  TAO_PSDL_Node *value_common_base_;
  TAO_PSDL_Node *value_inheritance_spec_;
  TAO_PSDL_Node *export_variable_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Value_Dcl : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Value_Dcl (TAO_PSDL_Node *value_header,
                      TAO_PSDL_Node *value_element);

  virtual ~TAO_PSDL_Value_Dcl (void);

  /// Get the value
  TAO_PSDL_Node *value_header (void) const;
  TAO_PSDL_Node *value_element (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:

  /// The values
  TAO_PSDL_Node *value_header_;
  TAO_PSDL_Node *value_element_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Value_Header : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Value_Header (TAO_PSDL_Node *value_common_base,
                         TAO_PSDL_Node *value_inheritance_spec);

  TAO_PSDL_Value_Header (TAO_PSDL_Node *custom,
                         TAO_PSDL_Node *value_common_base,
                         TAO_PSDL_Node *value_inheritance_spec);

  virtual ~TAO_PSDL_Value_Header (void);

  /// Get the value
  TAO_PSDL_Node *custom (void) const;
  TAO_PSDL_Node *value_common_base (void) const;
  TAO_PSDL_Node *value_inheritance_spec (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:

  /// The values
  TAO_PSDL_Node *custom_;
  TAO_PSDL_Node *value_common_base_;
  TAO_PSDL_Node *value_inheritance_spec_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Value_Inheritance_Spec : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Value_Inheritance_Spec (TAO_PSDL_Node *interface_name);

  /*
    Other forms
    TAO_PSDL_Value_Inheritance_Spec (TAO_PSDL_Node *value_name);
  */

  TAO_PSDL_Value_Inheritance_Spec (TAO_PSDL_Node *value_name,
                                   TAO_PSDL_Node *interface_name);

  /*
   Other forms of similar constuctors
   TAO_PSDL_Value_Inheritance_Spec (TAO_PSDL_Node *truncatable,
                                    TAO_PSDL_Node *value_name,
  */

  TAO_PSDL_Value_Inheritance_Spec (TAO_PSDL_Node *truncatable,
                                   TAO_PSDL_Node *value_name,
                                   TAO_PSDL_Node *interface_name);

  virtual ~TAO_PSDL_Value_Inheritance_Spec (void);

  /// Get the value
  TAO_PSDL_Node *truncatable (void) const;
  TAO_PSDL_Node *value_name (void) const;
  TAO_PSDL_Node *interface_name (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:

  /// The values
  TAO_PSDL_Node *truncatable_;
  TAO_PSDL_Node *value_name_;
  TAO_PSDL_Node *interface_name_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Value_Name : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Value_Name (TAO_PSDL_Node *scoped_name);

  TAO_PSDL_Value_Name (TAO_PSDL_Node *scoped_name,
                       TAO_PSDL_Node *value_name);

  virtual ~TAO_PSDL_Value_Name (void);

  /// Get the value
  TAO_PSDL_Node *scoped_name (void) const;
  TAO_PSDL_Node *value_name (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:

  /// The values
  TAO_PSDL_Node *scoped_name_;
  TAO_PSDL_Node *value_name_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Value_Element : public TAO_PSDL_Node
{
public:

  /// type_of_element can be export | state_member | init_dcl
  TAO_PSDL_Value_Element (TAO_PSDL_Node *type_of_element);

  virtual ~TAO_PSDL_Value_Element (void);

  /// Get the value
  TAO_PSDL_Node *type_of_element (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:

  /// The values
  TAO_PSDL_Node *type_of_element_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Array_Declarator : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Array_Declarator (TAO_PSDL_Node *identifier,
                             TAO_PSDL_Node *fixed_array_size);

  virtual ~TAO_PSDL_Array_Declarator (void);

  /// Get the value
  TAO_PSDL_Node *identifier (void) const;
  TAO_PSDL_Node *fixed_array_size (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:

  /// The values
  TAO_PSDL_Node *identifier_;
  TAO_PSDL_Node *fixed_array_size_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Fixed_Array_Size : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Fixed_Array_Size (TAO_PSDL_Node *positive_int_const);

  virtual ~TAO_PSDL_Fixed_Array_Size (void);

  /// Get the value
  TAO_PSDL_Node *positive_int_const (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:

  /// The values
  TAO_PSDL_Node *positive_int_const_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Attr_Dcl : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Attr_Dcl (int readonly,
                     TAO_PSDL_Node *param_type_spec,
                     TAO_PSDL_Node *simple_declarator);

  TAO_PSDL_Attr_Dcl (TAO_PSDL_Node *param_type_spec,
                     TAO_PSDL_Node *simple_declarator);

  virtual ~TAO_PSDL_Attr_Dcl (void);

  /// Get the value
  int readonly (void) const;
  TAO_PSDL_Node *param_type_spec (void) const;
  TAO_PSDL_Node *simple_declarator (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:

  /// The values
  int readonly_;
  TAO_PSDL_Node *param_type_spec_;
  TAO_PSDL_Node *simple_declarator_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Enumerator : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Enumerator (TAO_PSDL_Node *identifier);

  TAO_PSDL_Enumerator (TAO_PSDL_Node *identifier,
                       TAO_PSDL_Node *enumerator);

  virtual ~TAO_PSDL_Enumerator (void);

  /// Get the value
  TAO_PSDL_Node *identifier (void) const;
  TAO_PSDL_Node *enumerator (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:

  /// The values
  TAO_PSDL_Node *identifier_;
  TAO_PSDL_Node *enumerator_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Literal : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Literal (CORBA::Boolean boolean_value);

  TAO_PSDL_Literal (int int_value);

  TAO_PSDL_Literal (double double_value);

  TAO_PSDL_Literal (const char *char_value);

  virtual ~TAO_PSDL_Literal (void);

  /// Get the value
  CORBA::Boolean boolean_value (void) const;
  int int_value (void) const;
  double double_value (void) const;
  const char *char_value (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:

  /// The values
  CORBA::Boolean boolean_value_;
  int int_value_;
  double double_value_;
  const char *char_value_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_State_Member : public TAO_PSDL_Node
{
public:

  TAO_PSDL_State_Member (TAO_PSDL_Node *public_or_private,
                         TAO_PSDL_Node *type_spec,
                         TAO_PSDL_Node *declarators);

  /* Other forms
    TAO_PSDL_State_Member (TAO_PSDL_Node *public_or_private,
                           TAO_PSDL_Node *type_spec,
                           TAO_PSDL_Node *declarators);
  */

  virtual ~TAO_PSDL_State_Member (void);

  /// Get the value
  TAO_PSDL_Node *public_or_private (void) const;
  TAO_PSDL_Node *type_spec (void) const;
  TAO_PSDL_Node *declarators (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:

  /// The values
  TAO_PSDL_Node *public_or_private_;
  TAO_PSDL_Node *type_spec_;
  TAO_PSDL_Node *declarators_;

};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Init_Dcl : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Init_Dcl (TAO_PSDL_Node *identifier);

  TAO_PSDL_Init_Dcl (TAO_PSDL_Node *identifier,
                     TAO_PSDL_Node *init_param_decls);

  virtual ~TAO_PSDL_Init_Dcl (void);

  /// Get the value
  TAO_PSDL_Node *identifier (void) const;
  TAO_PSDL_Node *init_param_decls (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:

  /// The values
  TAO_PSDL_Node *identifier_;
  TAO_PSDL_Node *init_param_decls_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Init_Param_Decls : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Init_Param_Decls (TAO_PSDL_Node *init_param_decl);

  TAO_PSDL_Init_Param_Decls (TAO_PSDL_Node *init_param_decl,
                             TAO_PSDL_Node *init_param_decls);

  virtual ~TAO_PSDL_Init_Param_Decls (void);

  /// Get the value
  TAO_PSDL_Node *init_param_decl (void) const;
  TAO_PSDL_Node *init_param_decls (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:

  /// The values
  TAO_PSDL_Node *init_param_decl_;
  TAO_PSDL_Node *init_param_decls_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Init_Param_Decl : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Init_Param_Decl (TAO_PSDL_Node *init_param_attribute,
                            TAO_PSDL_Node *param_type_spec,
                            TAO_PSDL_Node *simple_declarator);

  virtual ~TAO_PSDL_Init_Param_Decl (void);

  /// Get the value
  TAO_PSDL_Node *init_param_attribute (void) const;
  TAO_PSDL_Node *param_type_spec (void) const;
  TAO_PSDL_Node *simple_declarator (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:

  /// The values
  TAO_PSDL_Node *init_param_attribute_;
  TAO_PSDL_Node *param_type_spec_;
  TAO_PSDL_Node *simple_declarator_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Positive_Int_Const : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Positive_Int_Const (TAO_PSDL_Node *const_exp);

  virtual ~TAO_PSDL_Positive_Int_Const (void);

  /// Get the value
  TAO_PSDL_Node *const_exp (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:

  /// The values
  TAO_PSDL_Node *const_exp_;
};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Factory_Dcl : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Factory_Dcl (TAO_PSDL_Node *identifier,
                        TAO_PSDL_Node *factory_parameters);

  virtual ~TAO_PSDL_Factory_Dcl (void);

  /// Get the value
  TAO_PSDL_Node *identifier (void) const;
  TAO_PSDL_Node *factory_parameters (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:

  /// The values
  TAO_PSDL_Node *identifier_;
  TAO_PSDL_Node *factory_parameters_;

};

/// ****************************************************************

class TAO_PSDL_Export TAO_PSDL_Factory_Parameters : public TAO_PSDL_Node
{
public:

  TAO_PSDL_Factory_Parameters ();
  TAO_PSDL_Factory_Parameters (TAO_PSDL_Node *simple_declarator);

  virtual ~TAO_PSDL_Factory_Parameters (void);

  /// Get the value
  TAO_PSDL_Node *simple_declarator (void) const;

  /// = The Node methods.
  int accept (TAO_PSDL_Node_Visitor *visitor);

private:

  /// The values
  TAO_PSDL_Node *simple_declarator_;
};

#endif /* TAO_PSDL_NODE_H */
