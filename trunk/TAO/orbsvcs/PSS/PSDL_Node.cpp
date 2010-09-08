// -*- C++ -*-
// $Id$

#include "PSDL_Node.h"
#include "PSDL_Scope.h"
#include "PSDL_Scope_Visitor.h"
#include "PSDL_Node_Visitor.h"
#include "PSDL_y.h"

#include "PSDL_Stream.h"
#include "tao/CORBA_String.h"



TAO_PSDL_Node::TAO_PSDL_Node (void)
  : identifiers_ (),
    count_ (0),
    predefined_type_ (0),
    psdl_scope_ (0),
    psdl_scope_visitor_ (0)
{
}

TAO_PSDL_Node::~TAO_PSDL_Node (void)
{
}

int
TAO_PSDL_Node::type_of_node (void) const
{
  return this->type_;
}


TAO_PSDL_Scope_Visitor *
TAO_PSDL_Node::psdl_scope_visitor (void)
{
  if (this->psdl_scope_visitor_ == 0)
    this->psdl_scope_visitor_ = new TAO_PSDL_Scope_Visitor (this);

  return this->psdl_scope_visitor_;
}

TAO_PSDL_Scope *
TAO_PSDL_Node::psdl_scope (void)
{
  this->psdl_scope_ = TAO_PSDL_Scope::instance ()->pop_top_scope ();

  return this->psdl_scope_;
}

ACE_CString
TAO_PSDL_Node::get_identifier_type (void)
{
  // We are going to save such that the first element is the type of the
  // identifier and the second element is the actual identifier
  return this->identifiers_[0];
}

ACE_CString
TAO_PSDL_Node::get_identifier_value (void)
{
  // We are going to save such as the first element is the type of the
  // identifier and
  // the second element is the actual identifier
  return this->identifiers_[1];
}

void
TAO_PSDL_Node::set_identifier (ACE_CString identifier)
{
  if (this->identifiers_.size () == 0)
    this->identifiers_.size (2);

  this->identifiers_[this->count_] = identifier;
  ++this->count_;
}

void
TAO_PSDL_Node::set_predefined_type (void)
{
  this->predefined_type_ = 1;
}

/// ****************************************************************

TAO_PSDL_Specification::TAO_PSDL_Specification (TAO_PSDL_Node *definition)
  : definition_ (definition),
    specification_ (0)
{
}

TAO_PSDL_Specification::TAO_PSDL_Specification (TAO_PSDL_Node *definition,
                                                TAO_PSDL_Node *specification)
  : definition_ (definition),
    specification_ (specification)
{
}

TAO_PSDL_Specification::~TAO_PSDL_Specification (void)
{
  delete this->definition_;
  delete this->specification_;
}

TAO_PSDL_Node *
TAO_PSDL_Specification::definition (void) const
{
  return this->definition_;
}

TAO_PSDL_Node *
TAO_PSDL_Specification::specification (void) const
{
  return this->specification_;
}

int
TAO_PSDL_Specification::accept (TAO_PSDL_Node_Visitor *visitor)
{
  int result = visitor->visit_specification (this);

  // The control goes back to psdl_tao now. So, put in the end stuff
  // to the header file.

  // Get a pointer to PSDL_Stream of the stub header.
  TAO_PSDL_Stream *ps_sh = TAO_PSDL_Scope::instance ()->get_sh ();

  *ps_sh << "#if defined (__ACE_INLINE__)"; ps_sh->nl ();
  *ps_sh << "#include \""
         << TAO_PSDL_Scope::instance ()->get_stub_prefix ()
         << "C.i\""; ps_sh->nl ();
  *ps_sh << "#endif /* defined INLINE */"; ps_sh->nl ();

  ps_sh->nl ();

  *ps_sh << "#if defined(_MSC_VER)"; ps_sh->nl ();
  *ps_sh << "#pragma warning(pop)"; ps_sh->nl ();
  *ps_sh << "#endif /* _MSC_VER */"; ps_sh->nl ();

  ps_sh->nl ();

  *ps_sh << "#endif /* ifndef */"; ps_sh->nl ();

  return result;
}

/// ****************************************************************

TAO_PSDL_Definition::TAO_PSDL_Definition (TAO_PSDL_Node *definition_type)
  : definition_type_ (definition_type)
{
}

TAO_PSDL_Definition::~TAO_PSDL_Definition (void)
{
  delete this->definition_type_;
}

TAO_PSDL_Node *
TAO_PSDL_Definition::definition_type (void) const
{
  return this->definition_type_;
}

int
TAO_PSDL_Definition::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_definition (this);
}

/// ****************************************************************

TAO_PSDL_Module::TAO_PSDL_Module (int type,
                                  TAO_PSDL_Node *identifier,
                                  TAO_PSDL_Node *specification)
  : identifier_ (identifier),
    specification_ (specification)
{
  this->type_ = type;
}

TAO_PSDL_Module::~TAO_PSDL_Module (void)
{
  delete this->identifier_;
  delete this->specification_;
}

TAO_PSDL_Node *
TAO_PSDL_Module::identifier (void) const
{
  return this->identifier_;
}

TAO_PSDL_Node *
TAO_PSDL_Module::specification (void) const
{
  return this->specification_;
}

void
TAO_PSDL_Module::set_identifier_name (ACE_CString identifier_name)
{
  this->module_name_ = identifier_name;
}

ACE_CString
TAO_PSDL_Module::identifier_name (void) const
{
  return this->module_name_;
}

int
TAO_PSDL_Module::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_module (this);
}

/// ***************************************************************

TAO_PSDL_Abstract_Storagetype::TAO_PSDL_Abstract_Storagetype (TAO_PSDL_Node *storagetype_dcl)
  : storagetype_dcl_ (storagetype_dcl)
{
}

TAO_PSDL_Abstract_Storagetype::~TAO_PSDL_Abstract_Storagetype (void)
{
  delete this->storagetype_dcl_;
}

TAO_PSDL_Node *
TAO_PSDL_Abstract_Storagetype::storagetype_dcl (void) const
{
  return this->storagetype_dcl_;
}

int
TAO_PSDL_Abstract_Storagetype::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_abstract_storagetype (this);
}

/// ****************************************************************

TAO_PSDL_Abstract_Storagetype_Dcl::TAO_PSDL_Abstract_Storagetype_Dcl (
     TAO_PSDL_Node *abs_storagetype_header)
  : abs_storagetype_header_ (abs_storagetype_header),
    abs_storagetype_body_ (0)
{
}

TAO_PSDL_Abstract_Storagetype_Dcl::TAO_PSDL_Abstract_Storagetype_Dcl (
     TAO_PSDL_Node *abs_storagetype_header,
     TAO_PSDL_Node *abs_storagetype_body)
  : abs_storagetype_header_ (abs_storagetype_header),
    abs_storagetype_body_ (abs_storagetype_body)
{
}

TAO_PSDL_Abstract_Storagetype_Dcl::~TAO_PSDL_Abstract_Storagetype_Dcl (void)
{
  delete this->abs_storagetype_header_;
  if (this->abs_storagetype_body_ != 0)
    delete this->abs_storagetype_body_;
}

TAO_PSDL_Node *
TAO_PSDL_Abstract_Storagetype_Dcl::abs_storagetype_header (void) const
{
  return this->abs_storagetype_header_;
}

TAO_PSDL_Node *
TAO_PSDL_Abstract_Storagetype_Dcl::abs_storagetype_body (void) const
{
  return this->abs_storagetype_body_;
}

int
TAO_PSDL_Abstract_Storagetype_Dcl::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_abstract_storagetype_dcl (this);
}

/// ****************************************************************

TAO_PSDL_Abstract_Storagetype_Fwd_Dcl::TAO_PSDL_Abstract_Storagetype_Fwd_Dcl (
     TAO_PSDL_Node *identifier)
  : identifier_ (identifier)
{
}

TAO_PSDL_Abstract_Storagetype_Fwd_Dcl::~TAO_PSDL_Abstract_Storagetype_Fwd_Dcl (void)
{
  delete this->identifier_;
}

TAO_PSDL_Node *
TAO_PSDL_Abstract_Storagetype_Fwd_Dcl::identifier (void) const
{
  return this->identifier_;
}

int
TAO_PSDL_Abstract_Storagetype_Fwd_Dcl::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_abstract_storagetype_fwd_dcl (this);
}

/// ****************************************************************

TAO_PSDL_Abstract_Storagetype_Header::TAO_PSDL_Abstract_Storagetype_Header (TAO_PSDL_Node *identifier)
  : identifier_ (identifier)
{
}

TAO_PSDL_Abstract_Storagetype_Header::TAO_PSDL_Abstract_Storagetype_Header (
     TAO_PSDL_Node *identifier,
     TAO_PSDL_Node *abs_storagetype_inh_spec)
  : identifier_ (identifier),
    abs_storagetype_inh_spec_ (abs_storagetype_inh_spec)
{
}

TAO_PSDL_Abstract_Storagetype_Header::~TAO_PSDL_Abstract_Storagetype_Header (void)
{
  delete this->identifier_;
  delete this->abs_storagetype_inh_spec_;
}

TAO_PSDL_Node *
TAO_PSDL_Abstract_Storagetype_Header::identifier (void) const
{
  return this->identifier_;
}

TAO_PSDL_Node *
TAO_PSDL_Abstract_Storagetype_Header::abs_storagetype_inh_spec (void) const
{
  return this->abs_storagetype_inh_spec_;
}

int
TAO_PSDL_Abstract_Storagetype_Header::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_abstract_storagetype_header (this);
}

/// ****************************************************************

TAO_PSDL_Abstract_Storagetype_Body::TAO_PSDL_Abstract_Storagetype_Body (TAO_PSDL_Node *abs_storagetype_member)
  : abs_storagetype_member_ (abs_storagetype_member)
{
}

TAO_PSDL_Abstract_Storagetype_Body::TAO_PSDL_Abstract_Storagetype_Body (
     TAO_PSDL_Node *abs_storagetype_member,
     TAO_PSDL_Node *abs_storagetype_body)
  : abs_storagetype_member_ (abs_storagetype_member),
    abs_storagetype_body_ (abs_storagetype_body)
{
}

TAO_PSDL_Abstract_Storagetype_Body::~TAO_PSDL_Abstract_Storagetype_Body (void)
{
  delete this->abs_storagetype_member_;
  delete this->abs_storagetype_body_;
}

TAO_PSDL_Node *
TAO_PSDL_Abstract_Storagetype_Body::abs_storagetype_member (void) const
{
  return this->abs_storagetype_member_;
}

TAO_PSDL_Node *
TAO_PSDL_Abstract_Storagetype_Body::abs_storagetype_body (void) const
{
  return this->abs_storagetype_body_;
}

int
TAO_PSDL_Abstract_Storagetype_Body::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_abstract_storagetype_body (this);
}

/// ****************************************************************

TAO_PSDL_Abstract_Storagetype_Member::TAO_PSDL_Abstract_Storagetype_Member (TAO_PSDL_Node *declaration)
  : declaration_ (declaration)
{
}

TAO_PSDL_Abstract_Storagetype_Member::~TAO_PSDL_Abstract_Storagetype_Member (void)
{
  delete this->declaration_;
}

TAO_PSDL_Node *
TAO_PSDL_Abstract_Storagetype_Member::declaration (void) const
{
  return this->declaration_;
}

int
TAO_PSDL_Abstract_Storagetype_Member::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_abstract_storagetype_member (this);
}

/// ****************************************************************

TAO_PSDL_Abstract_Storagetype_Inh_Spec::TAO_PSDL_Abstract_Storagetype_Inh_Spec (TAO_PSDL_Node *abs_storagetype_name)
  : abs_storagetype_name_ (abs_storagetype_name)
{
}

TAO_PSDL_Abstract_Storagetype_Inh_Spec::~TAO_PSDL_Abstract_Storagetype_Inh_Spec (void)
{
  delete this->abs_storagetype_name_;
}

TAO_PSDL_Node *
TAO_PSDL_Abstract_Storagetype_Inh_Spec::abs_storagetype_name (void) const
{
  return this->abs_storagetype_name_;
}

int
TAO_PSDL_Abstract_Storagetype_Inh_Spec::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_abstract_storagetype_inh_spec (this);
}

/// ****************************************************************

TAO_PSDL_Abstract_Storagetype_Name::TAO_PSDL_Abstract_Storagetype_Name (TAO_PSDL_Node *scoped_name)
  : scoped_name_ (scoped_name),
    abs_storagetype_name_ (0)
{
}

TAO_PSDL_Abstract_Storagetype_Name::TAO_PSDL_Abstract_Storagetype_Name (
     TAO_PSDL_Node *scoped_name,
     TAO_PSDL_Node *abs_storagetype_name)
  : scoped_name_ (scoped_name),
    abs_storagetype_name_ (abs_storagetype_name)
{
}

TAO_PSDL_Abstract_Storagetype_Name::~TAO_PSDL_Abstract_Storagetype_Name (void)
{
  delete this->scoped_name_;
  if (this->abs_storagetype_name_ != 0)
    delete this->abs_storagetype_name_;
}

TAO_PSDL_Node *
TAO_PSDL_Abstract_Storagetype_Name::scoped_name (void) const
{
  return this->scoped_name_;
}

TAO_PSDL_Node *
TAO_PSDL_Abstract_Storagetype_Name::abs_storagetype_name (void) const
{
  return this->abs_storagetype_name_;
}

int
TAO_PSDL_Abstract_Storagetype_Name::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_abstract_storagetype_name (this);
}

/// ****************************************************************

TAO_PSDL_Psdl_State_Dcl::TAO_PSDL_Psdl_State_Dcl (TAO_PSDL_Node *key_word,
                                                  TAO_PSDL_Node *psdl_state_type_spec,
                                                  TAO_PSDL_Node *simple_declarator)
  : key_word_ (key_word),
    psdl_state_type_spec_ (psdl_state_type_spec),
    simple_declarator_ (simple_declarator)
{
}

TAO_PSDL_Psdl_State_Dcl::TAO_PSDL_Psdl_State_Dcl (TAO_PSDL_Node *psdl_state_type_spec,
                                                  TAO_PSDL_Node *simple_declarator)
  : key_word_ (0),
    psdl_state_type_spec_ (psdl_state_type_spec),
    simple_declarator_ (simple_declarator)
{
}

TAO_PSDL_Psdl_State_Dcl::~TAO_PSDL_Psdl_State_Dcl (void)
{
  if (this->key_word_ != 0)
    delete this->key_word_;
  delete this->psdl_state_type_spec_;
  delete this->simple_declarator_;
}

TAO_PSDL_Node *
TAO_PSDL_Psdl_State_Dcl::key_word (void) const
{
  return this->key_word_;
}

TAO_PSDL_Node *
TAO_PSDL_Psdl_State_Dcl::psdl_state_type_spec (void) const
{
  return this->psdl_state_type_spec_;
}

TAO_PSDL_Node *
TAO_PSDL_Psdl_State_Dcl::simple_declarator (void) const
{
  return this->simple_declarator_;
}

int
TAO_PSDL_Psdl_State_Dcl::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_psdl_state_dcl (this);
}

/// ****************************************************************

TAO_PSDL_Psdl_State_Type_Spec::TAO_PSDL_Psdl_State_Type_Spec (TAO_PSDL_Node *type_spec)
  : type_spec_ (type_spec)
{
}

TAO_PSDL_Psdl_State_Type_Spec::~TAO_PSDL_Psdl_State_Type_Spec (void)
{
  delete this->type_spec_;
}

TAO_PSDL_Node *
TAO_PSDL_Psdl_State_Type_Spec::type_spec (void) const
{
  return this->type_spec_;
}

int
TAO_PSDL_Psdl_State_Type_Spec::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_psdl_state_type_spec (this);
}

/// ****************************************************************

TAO_PSDL_Abstract_Storagetype_Ref_Type::TAO_PSDL_Abstract_Storagetype_Ref_Type  (TAO_PSDL_Node *key_word,
  TAO_PSDL_Node *abs_storagetype_name)
  : key_word_ (key_word),
    abs_storagetype_name_ (abs_storagetype_name)
{
}

TAO_PSDL_Abstract_Storagetype_Ref_Type::TAO_PSDL_Abstract_Storagetype_Ref_Type (TAO_PSDL_Node *abs_storagetype_name)
  : key_word_ (0),
    abs_storagetype_name_ (abs_storagetype_name)
{
}

TAO_PSDL_Abstract_Storagetype_Ref_Type::~TAO_PSDL_Abstract_Storagetype_Ref_Type (void)
{
  if (this->key_word_ != 0)
    delete this->key_word_;
  delete this->abs_storagetype_name_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Abstract_Storagetype_Ref_Type::key_word (void) const
{
  return this->key_word_;
}

TAO_PSDL_Node *
TAO_PSDL_Abstract_Storagetype_Ref_Type::abs_storagetype_name (void) const
{
  return this->abs_storagetype_name_;
}

/// = The Node methods.
int
TAO_PSDL_Abstract_Storagetype_Ref_Type::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_abstract_storagetype_ref_type (this);
}

/// ****************************************************************

TAO_PSDL_Abstract_Storagehome::TAO_PSDL_Abstract_Storagehome (TAO_PSDL_Node *abs_storagehome_dcl)
  : abs_storagehome_dcl_ (abs_storagehome_dcl)
{
}

TAO_PSDL_Abstract_Storagehome::~TAO_PSDL_Abstract_Storagehome (void)
{
  delete this->abs_storagehome_dcl_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Abstract_Storagehome::abs_storagehome_dcl (void) const
{
  return this->abs_storagehome_dcl_;
}

/// = The Node methods.
int
TAO_PSDL_Abstract_Storagehome::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_abstract_storagehome (this);
}

/// ****************************************************************

TAO_PSDL_Abstract_Storagehome_Fwd_Dcl::TAO_PSDL_Abstract_Storagehome_Fwd_Dcl (
     TAO_PSDL_Node *identifier)
  : identifier_ (identifier)
{
}

TAO_PSDL_Abstract_Storagehome_Fwd_Dcl::~TAO_PSDL_Abstract_Storagehome_Fwd_Dcl (void)
{
  delete this->identifier_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Abstract_Storagehome_Fwd_Dcl::identifier (void) const
{
  return this->identifier_;
}

/// = The Node methods.
int
TAO_PSDL_Abstract_Storagehome_Fwd_Dcl::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_abstract_storagehome_fwd_dcl (this);
}

/// ****************************************************************

TAO_PSDL_Abstract_Storagehome_Dcl::TAO_PSDL_Abstract_Storagehome_Dcl (TAO_PSDL_Node *abs_storagehome_header)
  : abs_storagehome_header_ (abs_storagehome_header),
    abs_storagehome_body_ (0)
{
}

TAO_PSDL_Abstract_Storagehome_Dcl::TAO_PSDL_Abstract_Storagehome_Dcl (TAO_PSDL_Node *abs_storagehome_header,
                                   TAO_PSDL_Node *abs_storagehome_body)
  : abs_storagehome_header_ (abs_storagehome_header),
    abs_storagehome_body_ (abs_storagehome_body)
{
}

TAO_PSDL_Abstract_Storagehome_Dcl::~TAO_PSDL_Abstract_Storagehome_Dcl (void)
{
  delete this->abs_storagehome_header_;
  delete this->abs_storagehome_body_;
}


/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Abstract_Storagehome_Dcl::abs_storagehome_header (void) const
{
  return this->abs_storagehome_header_;
}

TAO_PSDL_Node *
TAO_PSDL_Abstract_Storagehome_Dcl::abs_storagehome_body (void) const
{
 return this->abs_storagehome_body_;
}

/// = The Node methods.
int
TAO_PSDL_Abstract_Storagehome_Dcl::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_abstract_storagehome_dcl (this);
}

/// ****************************************************************

TAO_PSDL_Abstract_Storagehome_Header::TAO_PSDL_Abstract_Storagehome_Header (TAO_PSDL_Node *identifier,
                                      TAO_PSDL_Node *abs_storagetype_name)
  : identifier_ (identifier),
    abs_storagetype_name_ (abs_storagetype_name),
    abs_storagehome_inh_spec_ (0)
{
}

TAO_PSDL_Abstract_Storagehome_Header::TAO_PSDL_Abstract_Storagehome_Header (TAO_PSDL_Node *identifier,
                                      TAO_PSDL_Node *abs_storagetype_name,
                                      TAO_PSDL_Node *abs_storagehome_inh_spec)
  : identifier_ (identifier),
    abs_storagetype_name_ (abs_storagetype_name),
    abs_storagehome_inh_spec_ (abs_storagehome_inh_spec)
{
}

TAO_PSDL_Abstract_Storagehome_Header::~TAO_PSDL_Abstract_Storagehome_Header (void)
{
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Abstract_Storagehome_Header::identifier (void) const
{
  return this->identifier_;
}

TAO_PSDL_Node *
TAO_PSDL_Abstract_Storagehome_Header::abs_storagetype_name (void) const
{
  return this->abs_storagetype_name_;
}

TAO_PSDL_Node *
TAO_PSDL_Abstract_Storagehome_Header::abs_storagehome_inh_spec (void) const
{
  return this->abs_storagehome_inh_spec_;
}


/// = The Node methods.
int
TAO_PSDL_Abstract_Storagehome_Header::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_abstract_storagehome_header (this);
}

/// ****************************************************************


TAO_PSDL_Abstract_Storagehome_Body::TAO_PSDL_Abstract_Storagehome_Body ()
{
}

TAO_PSDL_Abstract_Storagehome_Body::TAO_PSDL_Abstract_Storagehome_Body (TAO_PSDL_Node *abs_storagehome_member)
  : abs_storagehome_member_ (abs_storagehome_member)
{
}

TAO_PSDL_Abstract_Storagehome_Body::TAO_PSDL_Abstract_Storagehome_Body (TAO_PSDL_Node *abs_storagehome_member,
                                    TAO_PSDL_Node *abs_storagehome_body)
  : abs_storagehome_member_ (abs_storagehome_member),
    abs_storagehome_body_ (abs_storagehome_body)
{
}

TAO_PSDL_Abstract_Storagehome_Body::~TAO_PSDL_Abstract_Storagehome_Body (void)
{
  delete this->abs_storagehome_member_;

  if (this->abs_storagehome_body_ != 0)
    delete this->abs_storagehome_body_;
}


/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Abstract_Storagehome_Body::abs_storagehome_member (void) const
{
  return this->abs_storagehome_member_;
}

TAO_PSDL_Node *
TAO_PSDL_Abstract_Storagehome_Body::abs_storagehome_body (void) const
{
  return this->abs_storagehome_body_;
}

/// = The Node methods.
int
TAO_PSDL_Abstract_Storagehome_Body::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_abstract_storagehome_body (this);
}


/// ****************************************************************

TAO_PSDL_Abstract_Storagehome_Member::TAO_PSDL_Abstract_Storagehome_Member (TAO_PSDL_Node *declaration_type)
  : declaration_type_ (declaration_type)
{
}

TAO_PSDL_Abstract_Storagehome_Member::~TAO_PSDL_Abstract_Storagehome_Member (void)
{
  delete this->declaration_type_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Abstract_Storagehome_Member::declaration_type (void) const
{
  return this->declaration_type_;
}

/// = The Node methods.
int
TAO_PSDL_Abstract_Storagehome_Member::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_abstract_storagehome_member (this);
}

/// ****************************************************************

TAO_PSDL_Abstract_Storagehome_Inh_Spec::TAO_PSDL_Abstract_Storagehome_Inh_Spec (TAO_PSDL_Node *abs_storagehome_name)
  : abs_storagehome_name_ (abs_storagehome_name)
{
}

TAO_PSDL_Abstract_Storagehome_Inh_Spec::~TAO_PSDL_Abstract_Storagehome_Inh_Spec (void)
{
  delete this->abs_storagehome_name_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Abstract_Storagehome_Inh_Spec::abs_storagehome_name (void) const
{
  return this->abs_storagehome_name_;
}


/// = The Node methods.
int
TAO_PSDL_Abstract_Storagehome_Inh_Spec::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_abstract_storagehome_inh_spec (this);
}

/// ****************************************************************

TAO_PSDL_Abstract_Storagehome_Name::TAO_PSDL_Abstract_Storagehome_Name (TAO_PSDL_Node *scoped_name)
  : scoped_name_ (scoped_name),
    abs_storagehome_name_ (0)
{
}

TAO_PSDL_Abstract_Storagehome_Name::TAO_PSDL_Abstract_Storagehome_Name (TAO_PSDL_Node *scoped_name,
                                    TAO_PSDL_Node *abs_storagehome_name)
  : scoped_name_ (scoped_name),
    abs_storagehome_name_ (abs_storagehome_name)
{
}

TAO_PSDL_Abstract_Storagehome_Name::~TAO_PSDL_Abstract_Storagehome_Name (void)
{
  delete this->scoped_name_;
  if (this->abs_storagehome_name_ != 0)
    delete this->abs_storagehome_name_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Abstract_Storagehome_Name::scoped_name (void) const
{
  return this->scoped_name_;
}

TAO_PSDL_Node *
TAO_PSDL_Abstract_Storagehome_Name::abs_storagehome_name (void) const
{
  return this->abs_storagehome_name_;
}

/// = The Node methods.
int
TAO_PSDL_Abstract_Storagehome_Name::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_abstract_storagehome_name (this);
}

/// ****************************************************************

TAO_PSDL_Local_Op_Dcl::TAO_PSDL_Local_Op_Dcl (TAO_PSDL_Node *op_type_spec,
                                              TAO_PSDL_Node *identifier,
                                              TAO_PSDL_Node *parameter_dcls,
                                              TAO_PSDL_Node *raises_expr)
  : op_type_spec_ (op_type_spec),
    identifier_ (identifier),
    parameter_dcls_ (parameter_dcls),
    raises_expr_ (raises_expr)
{
}

TAO_PSDL_Local_Op_Dcl::TAO_PSDL_Local_Op_Dcl (TAO_PSDL_Node *op_type_spec,
                                              TAO_PSDL_Node *identifier,
                                              TAO_PSDL_Node *parameter_dcls)
  : op_type_spec_ (op_type_spec),
    identifier_ (identifier),
    parameter_dcls_ (parameter_dcls),
    raises_expr_ (0)
{
}

TAO_PSDL_Local_Op_Dcl::~TAO_PSDL_Local_Op_Dcl (void)
{
  delete this->op_type_spec_;
  delete this->identifier_;
  delete this->parameter_dcls_;
  if (this->raises_expr_ != 0)
    delete this->raises_expr_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Local_Op_Dcl::op_type_spec (void) const
{
  return this->op_type_spec_;
}

TAO_PSDL_Node *
TAO_PSDL_Local_Op_Dcl::identifier (void) const
{
  return this->identifier_;
}

TAO_PSDL_Node *
TAO_PSDL_Local_Op_Dcl::parameter_dcls (void) const
{
  return this->parameter_dcls_;
}

TAO_PSDL_Node *
TAO_PSDL_Local_Op_Dcl::raises_expr (void) const
{
  return this->raises_expr_;
}

/// = The Node methods.
int
TAO_PSDL_Local_Op_Dcl::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_local_op_dcl (this);
}

/// ****************************************************************

TAO_PSDL_Key_Dcl::TAO_PSDL_Key_Dcl (TAO_PSDL_Node *identifier,
                                    TAO_PSDL_Node *simple_declarator)
  : identifier_ (identifier),
    simple_declarator_ (simple_declarator)
{
}

TAO_PSDL_Key_Dcl::TAO_PSDL_Key_Dcl (TAO_PSDL_Node *identifier)
  : identifier_ (identifier),
    simple_declarator_ (0)
{
}

TAO_PSDL_Key_Dcl::~TAO_PSDL_Key_Dcl (void)
{
  delete this->identifier_;
  if (this->simple_declarator_ != 0)
    delete this->simple_declarator_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Key_Dcl::identifier (void) const
{
  return this->identifier_;
}

TAO_PSDL_Node *
TAO_PSDL_Key_Dcl::simple_declarator (void) const
{
  return this->simple_declarator_;
}

/// = The Node methods.
int
TAO_PSDL_Key_Dcl::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_key_dcl (this);
}

/// ****************************************************************

TAO_PSDL_Catalog::TAO_PSDL_Catalog (TAO_PSDL_Node *identifier)
  : identifier_ (identifier),
    catalog_inh_spec_ (0),
    catalog_body_ (0)
{
}

TAO_PSDL_Catalog::TAO_PSDL_Catalog (TAO_PSDL_Node *identifier,
                                    TAO_PSDL_Node *catalog_inh_spec,
                                    TAO_PSDL_Node *catalog_body)
  :identifier_ (identifier),
   catalog_inh_spec_ (catalog_inh_spec),
   catalog_body_ (catalog_body)
{
}

TAO_PSDL_Catalog::TAO_PSDL_Catalog (TAO_PSDL_Node *identifier,
                                    TAO_PSDL_Node *catalog_body)
  : identifier_ (identifier),
    catalog_inh_spec_ (0),
    catalog_body_ (catalog_body)
{
}

TAO_PSDL_Catalog::~TAO_PSDL_Catalog (void)
{
  delete this->identifier_;

  if (this->catalog_inh_spec_ != 0)
  delete this->catalog_inh_spec_;

  delete this->catalog_body_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Catalog::identifier (void) const
{
  return this->identifier_;
}

TAO_PSDL_Node *
TAO_PSDL_Catalog::catalog_inh_spec (void) const
{
  return this->catalog_inh_spec_;
}

TAO_PSDL_Node *
TAO_PSDL_Catalog::catalog_body (void) const
{
  return this->catalog_body_;
}

  /// = The Node methods.
int
TAO_PSDL_Catalog::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_catalog (this);
}

/// ****************************************************************


TAO_PSDL_Catalog_Inh_Spec::TAO_PSDL_Catalog_Inh_Spec (TAO_PSDL_Node *catalog_name)
  : catalog_name_ (catalog_name)
{
}

TAO_PSDL_Catalog_Inh_Spec::~TAO_PSDL_Catalog_Inh_Spec (void)
{
  delete this->catalog_name_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Catalog_Inh_Spec::catalog_name (void) const
{
  return this->catalog_name_;
}

/// = The Node methods.
int
TAO_PSDL_Catalog_Inh_Spec::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_catalog_inh_spec (this);
}

/// ****************************************************************

TAO_PSDL_Catalog_Name::TAO_PSDL_Catalog_Name (TAO_PSDL_Node *scoped_name)
  : scoped_name_ (scoped_name),
    catalog_name_ (0)
{
}

TAO_PSDL_Catalog_Name::TAO_PSDL_Catalog_Name (TAO_PSDL_Node *scoped_name,
                                              TAO_PSDL_Node *catalog_name)
  : scoped_name_ (scoped_name),
    catalog_name_ (catalog_name)
{
}

TAO_PSDL_Catalog_Name::~TAO_PSDL_Catalog_Name (void)
{
  delete this->scoped_name_;
  if (this->catalog_name_ != 0)
    delete this->catalog_name_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Catalog_Name::scoped_name (void) const
{
  return this->scoped_name_;
}

TAO_PSDL_Node *
TAO_PSDL_Catalog_Name::catalog_name (void) const
{
  return this->catalog_name_;
}

/// = The Node methods.
int
TAO_PSDL_Catalog_Name::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_catalog_name (this);
}

/// ****************************************************************

TAO_PSDL_Catalog_Body::TAO_PSDL_Catalog_Body ()
  : catalog_member_ (0),
    catalog_body_ (0)
{
}

TAO_PSDL_Catalog_Body::TAO_PSDL_Catalog_Body (TAO_PSDL_Node *catalog_member)
  : catalog_member_ (catalog_member)
{
}

TAO_PSDL_Catalog_Body::TAO_PSDL_Catalog_Body (TAO_PSDL_Node *catalog_member,
                                              TAO_PSDL_Node *catalog_body)
  : catalog_member_ (catalog_member),
    catalog_body_ (catalog_body)
{
}

TAO_PSDL_Catalog_Body::~TAO_PSDL_Catalog_Body (void)
{
  if (this->catalog_member_ != 0)
    delete this->catalog_member_;

  if (this->catalog_body_ != 0)
    delete this->catalog_body_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Catalog_Body::catalog_member (void) const
{
  return this->catalog_member_;
}

TAO_PSDL_Node *
TAO_PSDL_Catalog_Body::catalog_body (void) const
{
  return this->catalog_body_;
}

/// = The Node methods.
int
TAO_PSDL_Catalog_Body::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_catalog_body (this);
}

/// ****************************************************************

TAO_PSDL_Catalog_Member::TAO_PSDL_Catalog_Member (TAO_PSDL_Node *declaration)
  : declaration_ (declaration)
{
}

TAO_PSDL_Catalog_Member::~TAO_PSDL_Catalog_Member (void)
{
  delete this->declaration_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Catalog_Member::declaration (void) const
{
  return this->declaration_;
}

/// = The Node methods.
int
TAO_PSDL_Catalog_Member::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_catalog_member (this);
}

/// ****************************************************************

TAO_PSDL_Provides_Dcl::TAO_PSDL_Provides_Dcl (
     TAO_PSDL_Node *abs_storagehome_name,
     TAO_PSDL_Node *simple_declarator)
  : abs_storagehome_name_ (abs_storagehome_name),
    simple_declarator_ (simple_declarator)
{
}

TAO_PSDL_Provides_Dcl::~TAO_PSDL_Provides_Dcl (void)
{
  delete this->abs_storagehome_name_;
  delete this->simple_declarator_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Provides_Dcl::abs_storagehome_name (void) const
{
  return this->abs_storagehome_name_;
}

TAO_PSDL_Node *
TAO_PSDL_Provides_Dcl::simple_declarator (void) const
{
  return this->simple_declarator_;
}

/// = The Node methods.
int
TAO_PSDL_Provides_Dcl::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_provides_dcl (this);
}

/// ****************************************************************

TAO_PSDL_Storagetype::TAO_PSDL_Storagetype (TAO_PSDL_Node *declaration)
  : declaration_ (declaration)
{
}

TAO_PSDL_Storagetype::~TAO_PSDL_Storagetype (void)
{
  delete this->declaration_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Storagetype::declaration (void) const
{
  return this->declaration_;
}

/// = The Node methods.
int
TAO_PSDL_Storagetype::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_storagetype (this);
}

/// ****************************************************************

TAO_PSDL_Storagetype_Dcl::TAO_PSDL_Storagetype_Dcl (TAO_PSDL_Node *storagetype_header)
  : storagetype_header_ (storagetype_header),
    storagetype_body_ (0)
{
}

TAO_PSDL_Storagetype_Dcl::TAO_PSDL_Storagetype_Dcl (
     TAO_PSDL_Node *storagetype_header,
     TAO_PSDL_Node *storagetype_body)
  : storagetype_header_ (storagetype_header),
    storagetype_body_ (storagetype_body)
{
}

TAO_PSDL_Storagetype_Dcl::~TAO_PSDL_Storagetype_Dcl (void)
{
  delete this->storagetype_header_;
  if (this->storagetype_body_ != 0)
    delete this->storagetype_body_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Storagetype_Dcl::storagetype_header (void) const
{
  return this->storagetype_header_;
}

TAO_PSDL_Node *
TAO_PSDL_Storagetype_Dcl::storagetype_body (void) const
{
  return this->storagetype_body_;
}

/// = The Node methods.
int
TAO_PSDL_Storagetype_Dcl::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_storagetype_dcl (this);
}

/// ****************************************************************

TAO_PSDL_Storagetype_Fwd_Dcl::TAO_PSDL_Storagetype_Fwd_Dcl (TAO_PSDL_Node *identifier)
  : identifier_ (identifier)
{
}

TAO_PSDL_Storagetype_Fwd_Dcl::~TAO_PSDL_Storagetype_Fwd_Dcl (void)
{
  delete this->identifier_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Storagetype_Fwd_Dcl::identifier (void) const
{
  return this->identifier_;
}

/// = The Node methods.
int
TAO_PSDL_Storagetype_Fwd_Dcl::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_storagetype_fwd_dcl (this);
}

/// ****************************************************************

TAO_PSDL_Storagetype_Header::TAO_PSDL_Storagetype_Header (
     TAO_PSDL_Node *identifier,
     TAO_PSDL_Node *storagetype_inh_spec,
     TAO_PSDL_Node *storagetype_impl_spec)
  : identifier_ (identifier),
    storagetype_inh_spec_ (storagetype_inh_spec),
    storagetype_impl_spec_ (storagetype_impl_spec)
{
}

TAO_PSDL_Storagetype_Header::TAO_PSDL_Storagetype_Header (
     TAO_PSDL_Node *identifier,
     TAO_PSDL_Node *storagetype_inh_spec)
  : identifier_ (identifier),
    storagetype_inh_spec_ (storagetype_inh_spec),
    storagetype_impl_spec_ (0)
{
}

TAO_PSDL_Storagetype_Header::TAO_PSDL_Storagetype_Header (
     TAO_PSDL_Node *identifier)
  : identifier_ (identifier),
    storagetype_inh_spec_ (0),
    storagetype_impl_spec_ (0)
{
}

TAO_PSDL_Storagetype_Header::~TAO_PSDL_Storagetype_Header (void)
{
  delete this->identifier_;
  if (this->storagetype_inh_spec_ != 0)
    delete this->storagetype_inh_spec_;

  if (this->storagetype_impl_spec_ != 0)
    delete this->storagetype_impl_spec_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Storagetype_Header::identifier (void) const
{
  return this->identifier_;
}

TAO_PSDL_Node *
TAO_PSDL_Storagetype_Header::storagetype_inh_spec (void) const
{
  return this->storagetype_inh_spec_;
}

TAO_PSDL_Node *
TAO_PSDL_Storagetype_Header::storagetype_impl_spec (void) const
{
  return this->storagetype_impl_spec_;
}

/// = The Node methods.
int
TAO_PSDL_Storagetype_Header::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_storagetype_header (this);
}

/// ****************************************************************

TAO_PSDL_Storagetype_Body::TAO_PSDL_Storagetype_Body ()
  : storagetype_member_ (0),
    storagetype_body_ (0)
{
}

TAO_PSDL_Storagetype_Body::TAO_PSDL_Storagetype_Body (
     TAO_PSDL_Node *storagetype_member)
  : storagetype_member_ (storagetype_member),
    storagetype_body_ (0)
{
}

TAO_PSDL_Storagetype_Body::TAO_PSDL_Storagetype_Body (
     TAO_PSDL_Node *storagetype_member,
     TAO_PSDL_Node *storagetype_body)
  : storagetype_member_ (storagetype_member),
    storagetype_body_ (storagetype_body)
{
}

TAO_PSDL_Storagetype_Body::~TAO_PSDL_Storagetype_Body (void)
{
  if (this->storagetype_member_ != 0)
    delete this->storagetype_member_;
  if (this->storagetype_body_ != 0)
    delete this->storagetype_body_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Storagetype_Body::storagetype_member (void) const
{
  return this->storagetype_member_;
}

TAO_PSDL_Node *
TAO_PSDL_Storagetype_Body::storagetype_body (void) const
{
  return this->storagetype_body_;
}

/// = The Node methods.
int
TAO_PSDL_Storagetype_Body::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_storagetype_body (this);
}

/// ****************************************************************


TAO_PSDL_Storagetype_Member::TAO_PSDL_Storagetype_Member (TAO_PSDL_Node *declaration)
  : declaration_ (declaration)
{
}

TAO_PSDL_Storagetype_Member::~TAO_PSDL_Storagetype_Member (void)
{
  delete this->declaration_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Storagetype_Member::declaration (void) const
{
  return this->declaration_;
}

/// = The Node methods.
int
TAO_PSDL_Storagetype_Member::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_storagetype_member (this);
}

/// ****************************************************************


TAO_PSDL_Storagetype_Inh_Spec::TAO_PSDL_Storagetype_Inh_Spec (TAO_PSDL_Node *storagetype_name)
  : storagetype_name_ (storagetype_name)
{
}

TAO_PSDL_Storagetype_Inh_Spec::~TAO_PSDL_Storagetype_Inh_Spec (void)
{
  delete this->storagetype_name_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Storagetype_Inh_Spec::storagetype_name (void) const
{
  return this->storagetype_name_;
}

/// = The Node methods.
int
TAO_PSDL_Storagetype_Inh_Spec::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_storagetype_inh_spec (this);
}

/// ****************************************************************


TAO_PSDL_Storagetype_Name::TAO_PSDL_Storagetype_Name (TAO_PSDL_Node *scoped_name)
  : scoped_name_ (scoped_name)
{
}

TAO_PSDL_Storagetype_Name::~TAO_PSDL_Storagetype_Name (void)
{
  delete this->scoped_name_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Storagetype_Name::scoped_name (void) const
{
  return this->scoped_name_;
}

/// = The Node methods.
int
TAO_PSDL_Storagetype_Name::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_storagetype_name (this);
}

/// ****************************************************************


TAO_PSDL_Storagetype_Impl_Spec::TAO_PSDL_Storagetype_Impl_Spec (TAO_PSDL_Node *abstract_storagetype_name)
  : abstract_storagetype_name_ (abstract_storagetype_name)
{
}

TAO_PSDL_Storagetype_Impl_Spec::~TAO_PSDL_Storagetype_Impl_Spec (void)
{
  delete this->abstract_storagetype_name_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Storagetype_Impl_Spec::abstract_storagetype_name (void) const
{
  return this->abstract_storagetype_name_;
}

/// = The Node methods.
int
TAO_PSDL_Storagetype_Impl_Spec::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_storagetype_impl_spec (this);
}

/// ****************************************************************

TAO_PSDL_Storagetype_Ref_Type::TAO_PSDL_Storagetype_Ref_Type (TAO_PSDL_Node *storagetype_name)
  : storagetype_name_ (storagetype_name)
{
}

TAO_PSDL_Storagetype_Ref_Type::~TAO_PSDL_Storagetype_Ref_Type (void)
{
  delete this->storagetype_name_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Storagetype_Ref_Type::storagetype_name (void) const
{
  return this->storagetype_name_;
}

/// = The Node methods.
int
TAO_PSDL_Storagetype_Ref_Type::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_storagetype_ref_type (this);
}

/// ****************************************************************
TAO_PSDL_Storagehome_Scope::TAO_PSDL_Storagehome_Scope (TAO_PSDL_Node *storagehome_name)
  : storagehome_name_ (storagehome_name)
{
}

TAO_PSDL_Storagehome_Scope::~TAO_PSDL_Storagehome_Scope (void)
{
  delete this->storagehome_name_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Storagehome_Scope::storagehome_name (void) const
{
  return this->storagehome_name_;
}


/// = The Node methods.
int
TAO_PSDL_Storagehome_Scope::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_storagehome_scope (this);
}

/// ****************************************************************


TAO_PSDL_Store_Directive::TAO_PSDL_Store_Directive (
     TAO_PSDL_Node *simple_declarator,
     TAO_PSDL_Node *psdl_concrete_state_type,
     TAO_PSDL_Node *storagehome_scope)
  : simple_declarator_ (simple_declarator),
    psdl_concrete_state_type_ (psdl_concrete_state_type),
    storagehome_scope_ (storagehome_scope)
{
}

TAO_PSDL_Store_Directive::TAO_PSDL_Store_Directive (
     TAO_PSDL_Node *simple_declarator,
     TAO_PSDL_Node *psdl_concrete_state_type)
  : simple_declarator_ (simple_declarator),
    psdl_concrete_state_type_ (psdl_concrete_state_type),
    storagehome_scope_ (0)
{
}

TAO_PSDL_Store_Directive::~TAO_PSDL_Store_Directive (void)
{
  delete this->simple_declarator_;
  delete this->psdl_concrete_state_type_;

  if (this->storagehome_scope_ != 0)
    delete this->storagehome_scope_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Store_Directive::simple_declarator (void) const
{
  return this->simple_declarator_;
}

TAO_PSDL_Node *
TAO_PSDL_Store_Directive::psdl_concrete_state_type (void) const
{
  return this->psdl_concrete_state_type_;
}

TAO_PSDL_Node *
TAO_PSDL_Store_Directive::storagehome_scope (void) const
{
  return this->storagehome_scope_;
}

/// = The Node methods.
int
TAO_PSDL_Store_Directive::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_store_directive (this);
}

/// ****************************************************************
TAO_PSDL_Psdl_Concrete_State_Type::TAO_PSDL_Psdl_Concrete_State_Type (TAO_PSDL_Node *storagetype)
  : storagetype_ (storagetype)
{
}

TAO_PSDL_Psdl_Concrete_State_Type::~TAO_PSDL_Psdl_Concrete_State_Type (void)
{
  delete this->storagetype_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Psdl_Concrete_State_Type::storagetype (void) const
{
  return this->storagetype_;
}

/// = The Node methods.
int
TAO_PSDL_Psdl_Concrete_State_Type::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_psdl_concrete_state_type (this);
}

/// ****************************************************************
TAO_PSDL_Ref_Rep_Directive::TAO_PSDL_Ref_Rep_Directive (TAO_PSDL_Node *simple_declarator)
  : simple_declarator_ (simple_declarator)
{
}

TAO_PSDL_Ref_Rep_Directive::~TAO_PSDL_Ref_Rep_Directive (void)
{
  delete this->simple_declarator_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Ref_Rep_Directive::simple_declarator (void) const
{
  return this->simple_declarator_;
}

/// = The Node methods.
int
TAO_PSDL_Ref_Rep_Directive::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_ref_rep_directive (this);
}

/// ****************************************************************

TAO_PSDL_Storagehome::TAO_PSDL_Storagehome (TAO_PSDL_Node *storagehome_header)
  : storagehome_header_ (storagehome_header),
    storagehome_body_ (0)
{
}

TAO_PSDL_Storagehome::TAO_PSDL_Storagehome (TAO_PSDL_Node *storagehome_header,
                                             TAO_PSDL_Node *storagehome_body)
  : storagehome_header_ (storagehome_header),
    storagehome_body_ (storagehome_body)
{
}

TAO_PSDL_Storagehome::~TAO_PSDL_Storagehome (void)
{
  delete this->storagehome_header_;
  delete this->storagehome_body_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Storagehome::storagehome_header (void) const
{
  return this->storagehome_header_;
}

TAO_PSDL_Node *
TAO_PSDL_Storagehome::storagehome_body (void) const
{
  return this->storagehome_body_;
}

/// = The Node methods.
int
TAO_PSDL_Storagehome::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_storagehome (this);
}

/// ****************************************************************
TAO_PSDL_Storagehome_Header::TAO_PSDL_Storagehome_Header (
      TAO_PSDL_Node *identifier,
      TAO_PSDL_Node *storagetype_name,
      TAO_PSDL_Node *storagehome_inh_spec,
      TAO_PSDL_Node *storagehome_impl_spec)
  : identifier_ (identifier),
    storagetype_name_ (storagetype_name),
    storagehome_inh_spec_ (storagehome_inh_spec),
    storagehome_impl_spec_ (storagehome_impl_spec)
{
}

TAO_PSDL_Storagehome_Header::TAO_PSDL_Storagehome_Header (
      TAO_PSDL_Node *identifier,
      TAO_PSDL_Node *storagetype_name,
      TAO_PSDL_Node *storagehome_inh_spec)
  : identifier_ (identifier),
    storagetype_name_ (storagetype_name),
    storagehome_inh_spec_ (storagehome_inh_spec),
    storagehome_impl_spec_ (0)
{
}

TAO_PSDL_Storagehome_Header::TAO_PSDL_Storagehome_Header (
      TAO_PSDL_Node *identifier,
      TAO_PSDL_Node *storagetype_name)
  : identifier_ (identifier),
    storagetype_name_ (storagetype_name),
    storagehome_inh_spec_ (0),
    storagehome_impl_spec_ (0)
{
}

TAO_PSDL_Storagehome_Header::~TAO_PSDL_Storagehome_Header (void)
{
  delete this->identifier_;
  delete this->storagetype_name_;
  delete this->storagehome_inh_spec_;
  delete this->storagehome_impl_spec_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Storagehome_Header::identifier (void) const
{
  return this->identifier_;
}

TAO_PSDL_Node *
TAO_PSDL_Storagehome_Header::storagetype_name (void) const
{
  return this->storagetype_name_;
}

TAO_PSDL_Node *
TAO_PSDL_Storagehome_Header::storagehome_inh_spec (void) const
{
  return this->storagehome_inh_spec_;
}

TAO_PSDL_Node *
TAO_PSDL_Storagehome_Header::storagehome_impl_spec (void) const
{
  return this->storagehome_impl_spec_;
}

/// = The Node methods.
int
TAO_PSDL_Storagehome_Header::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_storagehome_header (this);
}

/// ****************************************************************

TAO_PSDL_Storagehome_Body::TAO_PSDL_Storagehome_Body ()
  : storagehome_member_ (0),
    storagehome_body_ (0)
{
}

TAO_PSDL_Storagehome_Body::TAO_PSDL_Storagehome_Body (
      TAO_PSDL_Node *storagehome_member)
  : storagehome_member_ (storagehome_member),
    storagehome_body_ (0)
{
}

TAO_PSDL_Storagehome_Body::TAO_PSDL_Storagehome_Body (
     TAO_PSDL_Node *storagehome_member,
     TAO_PSDL_Node *storagehome_body)
  : storagehome_member_ (storagehome_member),
    storagehome_body_ (storagehome_body)
{
}

TAO_PSDL_Storagehome_Body::~TAO_PSDL_Storagehome_Body (void)
{
  delete this->storagehome_member_;
  delete this->storagehome_body_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Storagehome_Body::storagehome_member (void) const
{
  return this->storagehome_member_;
}

TAO_PSDL_Node *
TAO_PSDL_Storagehome_Body::storagehome_body (void) const
{
  return this->storagehome_body_;
}


/// = The Node methods.
int
TAO_PSDL_Storagehome_Body::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_storagehome_body  (this);
}

/// ****************************************************************


TAO_PSDL_Storagehome_Member::TAO_PSDL_Storagehome_Member (
      TAO_PSDL_Node *key_dcl)
  : key_dcl_ (key_dcl)
{
}

TAO_PSDL_Storagehome_Member::~TAO_PSDL_Storagehome_Member (void)
{
  delete this->key_dcl_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Storagehome_Member::key_dcl (void) const
{
  return this->key_dcl_;
}

/// = The Node methods.
int
TAO_PSDL_Storagehome_Member::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_storagehome_member  (this);
}

/// ****************************************************************

TAO_PSDL_Storagehome_Inh_Spec::TAO_PSDL_Storagehome_Inh_Spec (TAO_PSDL_Node *storagehome_name)
  : storagehome_name_ (storagehome_name)
{
}

TAO_PSDL_Storagehome_Inh_Spec::~TAO_PSDL_Storagehome_Inh_Spec (void)
{
  delete this->storagehome_name_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Storagehome_Inh_Spec::storagehome_name (void) const
{
  return this->storagehome_name_;
}

/// = The Node methods.
int
TAO_PSDL_Storagehome_Inh_Spec::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_storagehome_inh_spec  (this);
}

/// ****************************************************************

TAO_PSDL_Storagehome_Name::TAO_PSDL_Storagehome_Name (TAO_PSDL_Node *scoped_name)
  : scoped_name_ (scoped_name)
{
}

TAO_PSDL_Storagehome_Name::~TAO_PSDL_Storagehome_Name (void)
{
  delete this->scoped_name_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Storagehome_Name::scoped_name (void) const
{
  return this->scoped_name_;
}

/// = The Node methods.
int
TAO_PSDL_Storagehome_Name::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_storagehome_name (this);
}

/// ****************************************************************

TAO_PSDL_Storagehome_Impl_Spec::TAO_PSDL_Storagehome_Impl_Spec (
     TAO_PSDL_Node *abstract_storagehome_name)
  : abstract_storagehome_name_ (abstract_storagehome_name)
{
}

TAO_PSDL_Storagehome_Impl_Spec::~TAO_PSDL_Storagehome_Impl_Spec (void)
{
  delete this->abstract_storagehome_name_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Storagehome_Impl_Spec::abstract_storagehome_name (void) const
{
  return this->abstract_storagehome_name_;
}

/// = The Node methods.
int
TAO_PSDL_Storagehome_Impl_Spec::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_storagehome_impl_spec (this);
}

/// ****************************************************************

TAO_PSDL_Primary_Key_Dcl::TAO_PSDL_Primary_Key_Dcl ()
  : identifier_ (0)
{
}

TAO_PSDL_Primary_Key_Dcl::TAO_PSDL_Primary_Key_Dcl (TAO_PSDL_Node *identifier)
  : identifier_ (identifier)
{
}

TAO_PSDL_Primary_Key_Dcl::~TAO_PSDL_Primary_Key_Dcl (void)
{
  delete this->identifier_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Primary_Key_Dcl::identifier (void) const
{
  return this->identifier_;
}

/// = The Node methods.
int
TAO_PSDL_Primary_Key_Dcl::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_primary_key_dcl  (this);
}

/// ****************************************************************

TAO_PSDL_Identifier::TAO_PSDL_Identifier (char *value)
  : value_ (CORBA::string_dup (value)),
    value_type_ (0)
{
  TAO_PSDL_Scope::instance ()->save_identifier (value);
}

TAO_PSDL_Identifier::TAO_PSDL_Identifier (int value_type)
  : value_type_ (value_type)
{
}

TAO_PSDL_Identifier::~TAO_PSDL_Identifier (void)
{
  if (this->value_ != 0)
    delete this->value_;
}

/// Get the value
char *
TAO_PSDL_Identifier::value (void) const
{
  return this->value_;
}

int
TAO_PSDL_Identifier::value_type (void) const
{
  return this->value_type_;
}

/// = The Node methods.
int
TAO_PSDL_Identifier::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_identifier  (this);
}

/// ****************************************************************

TAO_PSDL_Type_Dcl::TAO_PSDL_Type_Dcl (TAO_PSDL_Node *type_of_type_dcl)
  : key_word_ (0),
    type_of_type_dcl_ (type_of_type_dcl)
{
}

TAO_PSDL_Type_Dcl::TAO_PSDL_Type_Dcl (int key_word,
                                      TAO_PSDL_Node *type_of_type_dcl)
  : key_word_ (key_word),
    type_of_type_dcl_ (type_of_type_dcl)
{
  this->type_ = key_word;

  this->psdl_scope_visitor ()->visit_type_dcl (this);

  // @@ why this if loop
  if (ACE_OS::strcmp (this->identifiers_[0].c_str (), "") == 0)
    {
      this->identifiers_[0] = this->psdl_scope_visitor ()->get_scoped_type ();
    }

  if (key_word != TAO_PSDL_TYPEDEF)
    {
      this->psdl_scope ()->check_name_in_scope (this->identifiers_[0],
                                                this->psdl_scope ());
    }

  this->psdl_scope ()->add_typedef (this->identifiers_[1],
                                    this->identifiers_[0]);

  // We dont these values of the identifiers anymore as they have been
  // added to the corresponding AST. So, set count to 'zero' again.
  this->count_ = 0;

}

TAO_PSDL_Type_Dcl::~TAO_PSDL_Type_Dcl (void)
{
  delete this->type_of_type_dcl_;
}

/// Get the value
int
TAO_PSDL_Type_Dcl::key_word (void) const
{
  return this->key_word_;
}

TAO_PSDL_Node *
TAO_PSDL_Type_Dcl::type_of_type_dcl (void) const
{
  return this->type_of_type_dcl_;
}

/// = The Node methods.
int
TAO_PSDL_Type_Dcl::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_type_dcl  (this);
}

/// ****************************************************************

TAO_PSDL_Type_Declarator::TAO_PSDL_Type_Declarator (TAO_PSDL_Node *type_spec,
                                                     TAO_PSDL_Node *declarators)
  : type_spec_ (type_spec),
    declarators_ (declarators)
{
}

TAO_PSDL_Type_Declarator::~TAO_PSDL_Type_Declarator (void)
{
  delete this->type_spec_;
  delete this->declarators_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Type_Declarator::type_spec (void) const
{
  return this->type_spec_;
}

TAO_PSDL_Node *
TAO_PSDL_Type_Declarator::declarators (void) const
{
  return this->declarators_;
}

/// = The Node methods.
int
TAO_PSDL_Type_Declarator::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_type_declarator  (this);
}

/// ****************************************************************

TAO_PSDL_Type_Spec::TAO_PSDL_Type_Spec (TAO_PSDL_Node *type_of_type_spec)
  : type_of_type_spec_ (type_of_type_spec)
{
}

TAO_PSDL_Type_Spec::~TAO_PSDL_Type_Spec (void)
{
  delete this->type_of_type_spec_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Type_Spec::type_of_type_spec (void) const
{
  return this->type_of_type_spec_;
}

/// = The Node methods.
int
TAO_PSDL_Type_Spec::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_type_spec  (this);
}

/// ****************************************************************
TAO_PSDL_Simple_Type_Spec::TAO_PSDL_Simple_Type_Spec (TAO_PSDL_Node *type_of_simple_type_spec)
  : type_of_simple_type_spec_ (type_of_simple_type_spec)
{
}

TAO_PSDL_Simple_Type_Spec::~TAO_PSDL_Simple_Type_Spec (void)
{
  delete this->type_of_simple_type_spec_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Simple_Type_Spec::type_of_simple_type_spec (void) const
{
  return this->type_of_simple_type_spec_;
}

/// = The Node methods.
int
TAO_PSDL_Simple_Type_Spec::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_simple_type_spec  (this);
}

/// ****************************************************************
TAO_PSDL_Base_Type_Spec::TAO_PSDL_Base_Type_Spec (
     TAO_PSDL_Node *type_of_base_type_spec)
  : type_of_base_type_spec_ (type_of_base_type_spec)
{
}

TAO_PSDL_Base_Type_Spec::~TAO_PSDL_Base_Type_Spec (void)
{
  delete this->type_of_base_type_spec_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Base_Type_Spec::type_of_base_type_spec (void) const
{
  return this->type_of_base_type_spec_;
}

/// = The Node methods.
int
TAO_PSDL_Base_Type_Spec::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_base_type_spec  (this);
}

/// ****************************************************************
TAO_PSDL_Template_Type_Spec::TAO_PSDL_Template_Type_Spec (
     TAO_PSDL_Node *type_of_template_type_spec)
  : type_of_template_type_spec_ (type_of_template_type_spec)
{
}

TAO_PSDL_Template_Type_Spec::~TAO_PSDL_Template_Type_Spec (void)
{
  delete this->type_of_template_type_spec_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Template_Type_Spec::type_of_template_type_spec (void) const
{
  return this->type_of_template_type_spec_;
}

/// = The Node methods.
int
TAO_PSDL_Template_Type_Spec::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_template_type_spec  (this);
}

/// ***********************************************************

TAO_PSDL_Constr_Type_Spec::TAO_PSDL_Constr_Type_Spec (
     TAO_PSDL_Node *type_of_constr_type_spec)
  : type_of_constr_type_spec_ (type_of_constr_type_spec)
{
}

TAO_PSDL_Constr_Type_Spec::~TAO_PSDL_Constr_Type_Spec (void)
{
  delete this->type_of_constr_type_spec_;
}

TAO_PSDL_Node *
TAO_PSDL_Constr_Type_Spec::type_of_constr_type_spec (void) const
{
  return this->type_of_constr_type_spec_;
}

/// = The Node methods.
int
TAO_PSDL_Constr_Type_Spec::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_constr_type_spec  (this);
}

/// ****************************************************************

TAO_PSDL_Declarators::TAO_PSDL_Declarators (TAO_PSDL_Node *declarator)
  : declarator_ (declarator),
    set_of_declarators_ (0)
{
}

TAO_PSDL_Declarators::TAO_PSDL_Declarators (TAO_PSDL_Node *declarator,
                                            TAO_PSDL_Node *set_of_declarators)
  : declarator_ (declarator),
    set_of_declarators_ (set_of_declarators)
{
}

TAO_PSDL_Declarators::~TAO_PSDL_Declarators (void)
{
  delete this->declarator_;

  if (this->set_of_declarators_ != 0)
    delete this->set_of_declarators_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Declarators::declarator (void) const
{
  return this->declarator_;
}

TAO_PSDL_Node *
TAO_PSDL_Declarators::set_of_declarators (void) const
{
  return this->set_of_declarators_;
}

/// = The Node methods.
int
TAO_PSDL_Declarators::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_declarators  (this);
}

/// ****************************************************************
TAO_PSDL_Declarator::TAO_PSDL_Declarator (TAO_PSDL_Node *type_of_declarator)
  : type_of_declarator_ (type_of_declarator)
{
}

TAO_PSDL_Declarator::~TAO_PSDL_Declarator (void)
{
  delete this->type_of_declarator_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Declarator::type_of_declarator (void) const
{
  return this->type_of_declarator_;
}

/// = The Node methods.
int
TAO_PSDL_Declarator::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_declarator  (this);
}

/// ****************************************************************

TAO_PSDL_Simple_Declarator::TAO_PSDL_Simple_Declarator (
     TAO_PSDL_Node *identifier)
  : identifier_ (identifier),
    simple_declarator_ (0)
{
}

TAO_PSDL_Simple_Declarator::TAO_PSDL_Simple_Declarator (
     TAO_PSDL_Node *identifier,
     TAO_PSDL_Node *simple_declarator)
  : identifier_ (identifier),
    simple_declarator_ (simple_declarator)
{
}

TAO_PSDL_Simple_Declarator::~TAO_PSDL_Simple_Declarator (void)
{
  delete this->identifier_;

  if (this->simple_declarator_ != 0)
    delete this->simple_declarator_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Simple_Declarator::identifier (void) const
{
  return this->identifier_;
}

TAO_PSDL_Node *
TAO_PSDL_Simple_Declarator::simple_declarator (void) const
{
  return this->simple_declarator_;
}

/// = The Node methods.
int
TAO_PSDL_Simple_Declarator::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_simple_declarator  (this);
}

/// ****************************************************************

TAO_PSDL_Simple_Declarator_List::TAO_PSDL_Simple_Declarator_List (
     TAO_PSDL_Node *simple_declarator)
  : simple_declarator_list_ (0),
    simple_declarator_ (simple_declarator)
{
}

TAO_PSDL_Simple_Declarator_List::TAO_PSDL_Simple_Declarator_List (
     TAO_PSDL_Node *simple_declarator_list,
     TAO_PSDL_Node *simple_declarator)
  : simple_declarator_list_ (simple_declarator_list),
    simple_declarator_ (simple_declarator)
{
}

TAO_PSDL_Simple_Declarator_List::~TAO_PSDL_Simple_Declarator_List (void)
{
  if (this->simple_declarator_list_ != 0)
  delete this->simple_declarator_list_;

  if (this->simple_declarator_ != 0)
    delete this->simple_declarator_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Simple_Declarator_List::simple_declarator_list (void) const
{
  return this->simple_declarator_list_;
}

TAO_PSDL_Node *
TAO_PSDL_Simple_Declarator_List::simple_declarator (void) const
{
  return this->simple_declarator_;
}

/// = The Node methods.
int
TAO_PSDL_Simple_Declarator_List::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_simple_declarator_list  (this);
}

/// ****************************************************************

TAO_PSDL_Complex_Declarator::TAO_PSDL_Complex_Declarator (TAO_PSDL_Node *array_declarator)
  : array_declarator_ (array_declarator)
{
}

TAO_PSDL_Complex_Declarator::~TAO_PSDL_Complex_Declarator (void)
{
  delete this->array_declarator_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Complex_Declarator::array_declarator (void) const
{
  return this->array_declarator_;
}

/// = The Node methods.
int
TAO_PSDL_Complex_Declarator::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_complex_declarator  (this);
}

/// ****************************************************************

TAO_PSDL_Predefined_Type::TAO_PSDL_Predefined_Type (int type_one)
  : type_one_ (type_one),
    type_two_ (0),
    type_three_ (0),
    type_of_variable_one_ (0),
    type_of_variable_two_ (0),
    type_of_variable_three_ (0)
{
}

TAO_PSDL_Predefined_Type::TAO_PSDL_Predefined_Type (int type_one,
                                                    int type_two)
  : type_one_ (type_one),
    type_two_ (type_two),
    type_three_ (0),
    type_of_variable_one_ (0),
    type_of_variable_two_ (0),
    type_of_variable_three_ (0)
{
}

TAO_PSDL_Predefined_Type::TAO_PSDL_Predefined_Type (int type_one,
                                                    int type_two,
                                                    int type_three)
  : type_one_ (type_one),
    type_two_ (type_two),
    type_three_ (type_three),
    type_of_variable_one_ (0),
    type_of_variable_two_ (0),
    type_of_variable_three_ (0)
{
}

TAO_PSDL_Predefined_Type::TAO_PSDL_Predefined_Type (int type_one,
                                                    TAO_PSDL_Node *type_of_variable_one)
  : type_one_ (type_one),
    type_two_ (0),
    type_three_ (0),
    type_of_variable_one_ (type_of_variable_one),
    type_of_variable_two_ (0),
    type_of_variable_three_ (0)
{
}

TAO_PSDL_Predefined_Type::TAO_PSDL_Predefined_Type (TAO_PSDL_Node *type_of_variable_one)
  : type_one_ (0),
    type_two_ (0),
    type_three_ (0),
    type_of_variable_one_ (type_of_variable_one),
    type_of_variable_two_ (0),
    type_of_variable_three_ (0)
{
}

TAO_PSDL_Predefined_Type::TAO_PSDL_Predefined_Type (TAO_PSDL_Node *type_of_variable_one,
                                                    TAO_PSDL_Node *type_of_variable_two)
  : type_one_ (0),
    type_two_ (0),
    type_three_ (0),
    type_of_variable_one_ (type_of_variable_one),
    type_of_variable_two_ (type_of_variable_two),
    type_of_variable_three_ (0)
{
  this->psdl_scope_visitor ()->visit_predefined_type (this);
  /*
  this->psdl_scope ()->add_enum_decl (this->identifier_value_,
                                      this->identifier_type_);
  */
}

TAO_PSDL_Predefined_Type::TAO_PSDL_Predefined_Type (int type_one,
                                                    TAO_PSDL_Node *type_of_variable_one,
                                                    TAO_PSDL_Node *type_of_variable_two)
  : type_one_ (type_one),
    type_two_ (0),
    type_three_ (0),
    type_of_variable_one_ (type_of_variable_one),
    type_of_variable_two_ (type_of_variable_two),
    type_of_variable_three_ (0)
{
  // @@ I am not sure if it is needed. This is nto needed in case fo
  // structs atleast. So, I will check it that it doesnt happen in
  // case of structs. Or rather will comment it out and will check
  // later if need arised by.
  // this->psdl_scope_visitor ()->visit_predefined_type (this);

  /*  if (this->type_one_ == TAO_PSDL_ENUM)
    {
      this->psdl_scope ()->add_enum_decl (this->identifier_type_,
                                          "enum");
    }
  */
}

TAO_PSDL_Predefined_Type::TAO_PSDL_Predefined_Type (
      TAO_PSDL_Node *type_of_variable_one,
      TAO_PSDL_Node *type_of_variable_two,
      TAO_PSDL_Node *type_of_variable_three)
  : type_one_ (0),
    type_two_ (0),
    type_three_ (0),
    type_of_variable_one_ (type_of_variable_one),
    type_of_variable_two_ (type_of_variable_two),
    type_of_variable_three_ (type_of_variable_three)
{
}

TAO_PSDL_Predefined_Type::~TAO_PSDL_Predefined_Type (void)
{
  if (this->type_of_variable_one_ != 0)
    delete this->type_of_variable_one_;
  if (this->type_of_variable_two_ != 0)
    delete this->type_of_variable_two_;
  if (this->type_of_variable_three_ != 0)
    delete this->type_of_variable_three_;
}

/// Get the value
int
TAO_PSDL_Predefined_Type::type_one (void) const
{
  return this->type_one_;
}

int
TAO_PSDL_Predefined_Type::type_two (void) const
{
  return this->type_two_;
}

int
TAO_PSDL_Predefined_Type::type_three (void) const
{
  return this->type_three_;
}

TAO_PSDL_Node *
TAO_PSDL_Predefined_Type::type_of_variable_one (void) const
{
  return this->type_of_variable_one_;
}

TAO_PSDL_Node *
TAO_PSDL_Predefined_Type::type_of_variable_two (void) const
{
  return this->type_of_variable_two_;
}

TAO_PSDL_Node *
TAO_PSDL_Predefined_Type::type_of_variable_three (void) const
{
  return this->type_of_variable_three_;
}

/// = The Node methods.
int
TAO_PSDL_Predefined_Type::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_predefined_type  (this);
}

/// ****************************************************************

TAO_PSDL_Member_List::TAO_PSDL_Member_List (TAO_PSDL_Node *member)
  : member_ (member),
    member_list_ (0)
{
}

TAO_PSDL_Member_List::TAO_PSDL_Member_List (TAO_PSDL_Node *member,
                                            TAO_PSDL_Node *member_list)
  : member_ (member),
    member_list_ (member_list)
{
}

TAO_PSDL_Member_List::~TAO_PSDL_Member_List (void)
{
  delete this->member_;

  if (this->member_list_ != 0)
    delete this->member_list_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Member_List::member (void) const
{
  return this->member_;
}

TAO_PSDL_Node *
TAO_PSDL_Member_List::member_list (void) const
{
  return this->member_list_;
}

/// = The Node methods.
int
TAO_PSDL_Member_List::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_member_list  (this);
}

/// ****************************************************************

TAO_PSDL_Member::TAO_PSDL_Member (TAO_PSDL_Node *type_spec,
                                  TAO_PSDL_Node *declarators)
  : type_spec_ (type_spec),
    declarators_ (declarators)
{
  this->psdl_scope_visitor ()->visit_member (this);

  if (this->predefined_type_)
    {
      // This is a predefined type. So, no need to check. But, reset the
      // value to the default value.
      this->predefined_type_ = 0;
    }
  else
    {
      // This is a user-defined type. So, check if it has already been
      // defined.
      this->psdl_scope ()->check_name_in_scope (this->identifiers_[0],
                                                this->psdl_scope ());
    }

  this->psdl_scope ()->add_member_decl (this->identifiers_[1],
                                        this->identifiers_[0]);

  // We dont these values of the identifiers anymore as they have been
  // added to the corresponding AST. So, set count to 'zero' again.
  this->count_ = 0;
}

TAO_PSDL_Member::~TAO_PSDL_Member (void)
{
  delete this->type_spec_;
  delete this->declarators_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Member::type_spec (void) const
{
  return this->type_spec_;
}

TAO_PSDL_Node *
TAO_PSDL_Member::declarators (void) const
{
  return this->declarators_;
}

/// = The Node methods.
int
TAO_PSDL_Member::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_member  (this);
}

/// ****************************************************************

TAO_PSDL_Union_Type::TAO_PSDL_Union_Type (TAO_PSDL_Node *identifier,
                                          TAO_PSDL_Node *switch_type_spec,
                                          TAO_PSDL_Node *switch_body)
  : identifier_ (identifier),
    switch_type_spec_ (switch_type_spec),
    switch_body_ (switch_body)
{
}

TAO_PSDL_Union_Type::~TAO_PSDL_Union_Type (void)
{
  delete this->identifier_;
  delete this->switch_type_spec_;
  delete this->switch_body_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Union_Type::identifier (void) const
{
  return this->identifier_;
}

TAO_PSDL_Node *
TAO_PSDL_Union_Type::switch_type_spec (void) const
{
  return this->switch_type_spec_;
}

TAO_PSDL_Node *
TAO_PSDL_Union_Type::switch_body (void) const
{
  return this->switch_body_;
}

/// = The Node methods.
int
TAO_PSDL_Union_Type::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_union_type (this);
}

/// ****************************************************************

TAO_PSDL_Switch_Type_Spec::TAO_PSDL_Switch_Type_Spec (TAO_PSDL_Node *type)
  : type_ (type)
{
}

TAO_PSDL_Switch_Type_Spec::~TAO_PSDL_Switch_Type_Spec (void)
{
  delete this->type_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Switch_Type_Spec::type (void) const
{
  return this->type_;
}

/// = The Node methods.
int
TAO_PSDL_Switch_Type_Spec::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_switch_type_spec  (this);
}

/// ****************************************************************

TAO_PSDL_Switch_Body::TAO_PSDL_Switch_Body (TAO_PSDL_Node *switch_case)
  : switch_case_ (switch_case),
    switch_body_ (0)
{
}

TAO_PSDL_Switch_Body::TAO_PSDL_Switch_Body (TAO_PSDL_Node *switch_case,
                                            TAO_PSDL_Node *switch_body)
  : switch_case_ (switch_case),
    switch_body_ (switch_body)
{
}

TAO_PSDL_Switch_Body::~TAO_PSDL_Switch_Body (void)
{
  delete this->switch_case_;
  delete this->switch_body_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Switch_Body::switch_case (void) const
{
  return this->switch_case_;
}

TAO_PSDL_Node *
TAO_PSDL_Switch_Body::switch_body (void) const
{
  return this->switch_body_;
}

/// = The Node methods.
int
TAO_PSDL_Switch_Body::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_switch_body (this);
}

/// ****************************************************************

TAO_PSDL_Case::TAO_PSDL_Case (TAO_PSDL_Node *case_label,
                              TAO_PSDL_Node *element_spec)
  : case_label_ (case_label),
    element_spec_ (element_spec)
{
}

TAO_PSDL_Case::~TAO_PSDL_Case (void)
{
  delete this->case_label_;
  delete this->element_spec_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Case::case_label (void) const
{
  return this->case_label_;
}

TAO_PSDL_Node *
TAO_PSDL_Case::element_spec (void) const
{
  return this->element_spec_;
}

/// = The Node methods.
int
TAO_PSDL_Case::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_case_case (this);
}

/// ****************************************************************

TAO_PSDL_Case_Label::TAO_PSDL_Case_Label (TAO_PSDL_Node * type)
  : type_ (type),
    const_exp_ (0),
    case_label_ (0)
{
}

TAO_PSDL_Case_Label::TAO_PSDL_Case_Label (TAO_PSDL_Node * type,
                                          TAO_PSDL_Node *const_exp)
  : type_ (type),
    const_exp_ (const_exp),
    case_label_ (0)
{
}

TAO_PSDL_Case_Label::TAO_PSDL_Case_Label (TAO_PSDL_Node * type,
                                          TAO_PSDL_Node *const_exp,
                                          TAO_PSDL_Node *case_label)
  : type_ (type),
    const_exp_ (const_exp),
    case_label_ (case_label)
{
}

TAO_PSDL_Case_Label::~TAO_PSDL_Case_Label (void)
{
  if (this->const_exp_ != 0)
    delete this->const_exp_;

  if (this->case_label_ != 0)
    delete this->case_label_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Case_Label::type (void) const
{
  return this->type_;
}

TAO_PSDL_Node *
TAO_PSDL_Case_Label::const_exp (void) const
{
  return this->const_exp_;
}

TAO_PSDL_Node *
TAO_PSDL_Case_Label::case_label (void) const
{
  return this->case_label_;
}

int
TAO_PSDL_Case_Label::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_case_label (this);
}

/// ****************************************************************

TAO_PSDL_Element_Spec::TAO_PSDL_Element_Spec (TAO_PSDL_Node *type_spec,
                                              TAO_PSDL_Node *declarator)
  : type_spec_ (type_spec),
    declarator_ (declarator)
{
}

TAO_PSDL_Element_Spec::~TAO_PSDL_Element_Spec (void)
{
  delete this->type_spec_;
  delete this->declarator_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Element_Spec::type_spec (void) const
{
  return this->type_spec_;
}

TAO_PSDL_Node *
TAO_PSDL_Element_Spec::declarator (void) const
{
  return this->declarator_;
}

/// = The Node methods.
int
TAO_PSDL_Element_Spec::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_element_spec (this);
}

/// ****************************************************************

TAO_PSDL_Const_Dcl::TAO_PSDL_Const_Dcl (TAO_PSDL_Node *const_type,
                                        TAO_PSDL_Node *identifier,
                                        TAO_PSDL_Node *const_exp)
  : const_type_ (const_type),
    identifier_ (identifier),
    const_exp_ (const_exp)
{
  this->psdl_scope_visitor ()->visit_const_dcl (this);

  this->psdl_scope ()->add_const_decl (this->identifiers_[1],
                                       this->identifiers_[0]);

  // We dont these values of the identifiers anymore as they have been
  // added to the corresponding AST. So, set count to 'zero' again.
  this->count_ = 0;

}

TAO_PSDL_Const_Dcl::~TAO_PSDL_Const_Dcl (void)
{
  delete this->const_type_;
  delete this->identifier_;
  delete this->const_exp_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Const_Dcl::const_type (void) const
{
  return this->const_type_;
}

TAO_PSDL_Node *
TAO_PSDL_Const_Dcl::identifier (void) const
{
  return this->identifier_;
}

TAO_PSDL_Node *
TAO_PSDL_Const_Dcl::const_exp (void) const
{
  return this->const_exp_;
}

/// = The Node methods.
int
TAO_PSDL_Const_Dcl::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_const_dcl (this);
}

/// ****************************************************************

TAO_PSDL_Const_Type::TAO_PSDL_Const_Type (TAO_PSDL_Node *type)
  : type_ (type)
{
}

TAO_PSDL_Const_Type::~TAO_PSDL_Const_Type (void)
{
  delete this->type_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Const_Type::type (void) const
{
  return this->type_;
}

/// = The Node methods.
int
TAO_PSDL_Const_Type::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_const_type (this);
}

/// ****************************************************************

TAO_PSDL_Const_Exp::TAO_PSDL_Const_Exp (TAO_PSDL_Node *expr)
  : expr_ (expr)
{
}

TAO_PSDL_Const_Exp::~TAO_PSDL_Const_Exp (void)
{
  delete this->expr_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Const_Exp::expr (void) const
{
  return this->expr_;
}

/// = The Node methods.
int
TAO_PSDL_Const_Exp::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_const_exp (this);
}

/// ****************************************************************

TAO_PSDL_Or_Expr::TAO_PSDL_Or_Expr (TAO_PSDL_Node *xor_expr)
  : or_expr_ (0),
    xor_expr_ (xor_expr)
{
}

TAO_PSDL_Or_Expr::TAO_PSDL_Or_Expr (TAO_PSDL_Node *or_expr,
                                    TAO_PSDL_Node *xor_expr)
  : or_expr_ (or_expr),
    xor_expr_ (xor_expr)
{
}

TAO_PSDL_Or_Expr::~TAO_PSDL_Or_Expr (void)
{
  if (this->or_expr_ != 0)
    delete this->or_expr_;

  delete this->xor_expr_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Or_Expr::or_expr (void) const
{
  return this->or_expr_;
}

TAO_PSDL_Node *
TAO_PSDL_Or_Expr::xor_expr (void) const
{
  return this->xor_expr_;
}

/// = The Node methods.
int
TAO_PSDL_Or_Expr::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_or_expr (this);
}

/// ****************************************************************

TAO_PSDL_Xor_Expr::TAO_PSDL_Xor_Expr (TAO_PSDL_Node *and_expr)
  : xor_expr_ (0),
    and_expr_ (and_expr)
{
}

TAO_PSDL_Xor_Expr::TAO_PSDL_Xor_Expr (TAO_PSDL_Node *xor_expr,
                                      TAO_PSDL_Node *and_expr)
  : xor_expr_ (xor_expr),
    and_expr_ (and_expr)
{
}

TAO_PSDL_Xor_Expr::~TAO_PSDL_Xor_Expr (void)
{
  if (this->xor_expr_ != 0)
    delete this->xor_expr_;

  delete this->and_expr_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Xor_Expr::xor_expr (void) const
{
  return this->xor_expr_;
}

TAO_PSDL_Node *
TAO_PSDL_Xor_Expr::and_expr (void) const
{
  return this->and_expr_;
}

/// = The Node methods.
int
TAO_PSDL_Xor_Expr::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_xor_expr (this);
}

/// ****************************************************************

TAO_PSDL_And_Expr::TAO_PSDL_And_Expr (TAO_PSDL_Node *shift_expr)
  : and_expr_ (0),
    shift_expr_ (shift_expr)
{
}

TAO_PSDL_And_Expr::TAO_PSDL_And_Expr (TAO_PSDL_Node *and_expr,
                                      TAO_PSDL_Node *shift_expr)
  : and_expr_ (and_expr),
    shift_expr_ (shift_expr)
{
}

TAO_PSDL_And_Expr::~TAO_PSDL_And_Expr (void)
{
  if (this->shift_expr_ != 0)
    delete this->shift_expr_;

  delete this->and_expr_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_And_Expr::and_expr (void) const
{
  return this->and_expr_;
}

TAO_PSDL_Node *
TAO_PSDL_And_Expr::shift_expr (void) const
{
  return this->shift_expr_;
}

/// = The Node methods.
int
TAO_PSDL_And_Expr::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_and_expr (this);
}

/// ****************************************************************
TAO_PSDL_Shift_Expr::TAO_PSDL_Shift_Expr (TAO_PSDL_Node *add_expr)
  : shift_expr_ (0),
    type_ (0),
    add_expr_ (add_expr)
{
}

TAO_PSDL_Shift_Expr::TAO_PSDL_Shift_Expr (TAO_PSDL_Node *shift_expr,
                                          TAO_PSDL_Node * type,
                                          TAO_PSDL_Node *add_expr)
  : shift_expr_ (shift_expr),
    type_ (type),
    add_expr_ (add_expr)
{
}

TAO_PSDL_Shift_Expr::~TAO_PSDL_Shift_Expr (void)
{
  if (this->shift_expr_ != 0)
    delete this->shift_expr_;

  delete this->add_expr_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Shift_Expr::type (void) const
{
  return this->type_;
}

TAO_PSDL_Node *
TAO_PSDL_Shift_Expr::add_expr (void) const
{
  return this->add_expr_;
}

TAO_PSDL_Node *
TAO_PSDL_Shift_Expr::shift_expr (void) const
{
  return this->shift_expr_;
}

/// = The Node methods.
int
TAO_PSDL_Shift_Expr::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_shift_expr (this);
}

/// ****************************************************************

TAO_PSDL_Add_Expr::TAO_PSDL_Add_Expr (TAO_PSDL_Node *mult_expr)
  : add_expr_ (0),
    type_ (0),
    mult_expr_ (mult_expr)
{
}

TAO_PSDL_Add_Expr::TAO_PSDL_Add_Expr (TAO_PSDL_Node *add_expr,
                                      TAO_PSDL_Node * type,
                                      TAO_PSDL_Node *mult_expr)
  : add_expr_ (add_expr),
    type_ (type),
    mult_expr_ (mult_expr)
{
}

TAO_PSDL_Add_Expr::~TAO_PSDL_Add_Expr (void)
{
  if (this->add_expr_ != 0)
    delete this->add_expr_;

  delete this->mult_expr_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Add_Expr::type (void) const
{
  return this->type_;
}

TAO_PSDL_Node *
TAO_PSDL_Add_Expr::add_expr (void) const
{
  return this->add_expr_;
}

TAO_PSDL_Node *
TAO_PSDL_Add_Expr::mult_expr (void) const
{
  return this->mult_expr_;
}

/// = The Node methods.
int
TAO_PSDL_Add_Expr::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_add_expr (this);
}

/// ****************************************************************

TAO_PSDL_Mult_Expr::TAO_PSDL_Mult_Expr (TAO_PSDL_Node *unary_expr)
  : mult_expr_ (0),
    type_ (0),
    unary_expr_ (unary_expr)
{
}

TAO_PSDL_Mult_Expr::TAO_PSDL_Mult_Expr (TAO_PSDL_Node *mult_expr,
                                        TAO_PSDL_Node *unary_expr)
  : mult_expr_ (mult_expr),
    type_ (0),
    unary_expr_ (unary_expr)
{
}

TAO_PSDL_Mult_Expr::TAO_PSDL_Mult_Expr (TAO_PSDL_Node *mult_expr,
                                        TAO_PSDL_Node * type,
                                        TAO_PSDL_Node *unary_expr)
  : mult_expr_ (mult_expr),
    type_ (type),
    unary_expr_ (unary_expr)
{
}

TAO_PSDL_Mult_Expr::~TAO_PSDL_Mult_Expr (void)
{
  if (this->mult_expr_ != 0)
    delete this->mult_expr_;

  if (this->type_ != 0)
    delete this->type_;

  delete this->unary_expr_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Mult_Expr::type (void) const
{
  return this->type_;
}

TAO_PSDL_Node *
TAO_PSDL_Mult_Expr::unary_expr (void) const
{
  return this->unary_expr_;
}

TAO_PSDL_Node *
TAO_PSDL_Mult_Expr::mult_expr (void) const
{
  return this->mult_expr_;
}

/// = The Node methods.
int
TAO_PSDL_Mult_Expr::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_mult_expr (this);
}

/// ****************************************************************

TAO_PSDL_Unary_Expr::TAO_PSDL_Unary_Expr (TAO_PSDL_Node *unary_operator,
                                          TAO_PSDL_Node *primary_expr)
  : unary_operator_ (unary_operator),
    primary_expr_ (primary_expr)
{
}

TAO_PSDL_Unary_Expr::TAO_PSDL_Unary_Expr (TAO_PSDL_Node *primary_expr)
  : unary_operator_ (0),
    primary_expr_ (primary_expr)
{
}

TAO_PSDL_Unary_Expr::~TAO_PSDL_Unary_Expr (void)
{
  if (this->unary_operator_ != 0)
    delete this->unary_operator_;

  delete this->primary_expr_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Unary_Expr::unary_operator (void) const
{
  return this->unary_operator_;
}

TAO_PSDL_Node *
TAO_PSDL_Unary_Expr::primary_expr (void) const
{
  return this->primary_expr_;
}

/// = The Node methods.
int
TAO_PSDL_Unary_Expr::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_unary_expr  (this);
}

/// ****************************************************************
TAO_PSDL_Primary_Expr::TAO_PSDL_Primary_Expr (TAO_PSDL_Node *type_of_primary_expr)
  : type_of_primary_expr_ (type_of_primary_expr)
{
}

TAO_PSDL_Primary_Expr::~TAO_PSDL_Primary_Expr (void)
{
  delete this->type_of_primary_expr_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Primary_Expr::type_of_primary_expr (void) const
{
  return this->type_of_primary_expr_;
}

/// = The Node methods.
int
TAO_PSDL_Primary_Expr::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_primary_expr (this);
}

/// ****************************************************************

TAO_PSDL_Except_Dcl::TAO_PSDL_Except_Dcl (TAO_PSDL_Node *identifier)
  : identifier_ (identifier),
    member_list_ (0)
{
}

TAO_PSDL_Except_Dcl::TAO_PSDL_Except_Dcl (TAO_PSDL_Node *identifier,
                                          TAO_PSDL_Node *member_list)
  : identifier_ (identifier),
    member_list_ (member_list)
{
}

TAO_PSDL_Except_Dcl::~TAO_PSDL_Except_Dcl (void)
{
  delete this->identifier_;
  if (this->member_list_ != 0)
    delete this->member_list_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Except_Dcl::identifier (void) const
{
  return this->identifier_;
}

TAO_PSDL_Node *
TAO_PSDL_Except_Dcl::member_list (void) const
{
  return this->member_list_;
}

/// = The Node methods.
int
TAO_PSDL_Except_Dcl::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_except_dcl (this);
}

/// ****************************************************************

TAO_PSDL_Op_Dcl::TAO_PSDL_Op_Dcl (TAO_PSDL_Node *op_attribute,
                                  TAO_PSDL_Node *op_type_spec,
                                  TAO_PSDL_Node *identifier,
                                  TAO_PSDL_Node * parameter_dcls,
                                  TAO_PSDL_Node * raises_expr,
                                  TAO_PSDL_Node * context_expr)
  : op_attribute_ (op_attribute),
    op_type_spec_ (op_type_spec),
    identifier_ (identifier),
    parameter_dcls_ (parameter_dcls),
    raises_expr_ (raises_expr),
    context_expr_ (context_expr)
{
}

TAO_PSDL_Op_Dcl::TAO_PSDL_Op_Dcl (TAO_PSDL_Node *op_type_spec,
                                  TAO_PSDL_Node *identifier,
                                  TAO_PSDL_Node * parameter_dcls,
                                  TAO_PSDL_Node * raises_expr,
                                  TAO_PSDL_Node * context_expr)
  : op_attribute_ (0),
    op_type_spec_ (op_type_spec),
    identifier_ (identifier),
    parameter_dcls_ (parameter_dcls),
    raises_expr_ (raises_expr),
    context_expr_ (context_expr)
{
}

TAO_PSDL_Op_Dcl::TAO_PSDL_Op_Dcl (TAO_PSDL_Node *op_type_spec,
                                  TAO_PSDL_Node *identifier,
                                  TAO_PSDL_Node * parameter_dcls,
                                  TAO_PSDL_Node * raises_expr)
  : op_attribute_ (0),
    op_type_spec_ (op_type_spec),
    identifier_ (identifier),
    parameter_dcls_ (parameter_dcls),
    raises_expr_ (raises_expr),
    context_expr_ (0)
{
}

TAO_PSDL_Op_Dcl::TAO_PSDL_Op_Dcl (TAO_PSDL_Node *op_type_spec,
                                   TAO_PSDL_Node *identifier,
                                   TAO_PSDL_Node * parameter_dcls)
  : op_attribute_ (0),
    op_type_spec_ (op_type_spec),
    identifier_ (identifier),
    parameter_dcls_ (parameter_dcls),
    raises_expr_ (0),
    context_expr_ (0)
{
}

TAO_PSDL_Op_Dcl::~TAO_PSDL_Op_Dcl (void)
{
  if (this->op_attribute_ != 0)
    delete this->op_attribute_;

  delete this->op_type_spec_;
  delete this->identifier_;
  delete this->parameter_dcls_;

  if (this->raises_expr_ != 0)
    delete this->raises_expr_;

  if (this->context_expr_ != 0)
    delete this->context_expr_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Op_Dcl::op_attribute (void) const
{
  return this->op_attribute_;
}

TAO_PSDL_Node *
TAO_PSDL_Op_Dcl::op_type_spec (void) const
{
  return this->op_type_spec_;
}

TAO_PSDL_Node *
TAO_PSDL_Op_Dcl::identifier (void) const
{
  return this->identifier_;
}

TAO_PSDL_Node *
TAO_PSDL_Op_Dcl::parameter_dcls (void) const
{
  return this->parameter_dcls_;
}

TAO_PSDL_Node *
TAO_PSDL_Op_Dcl::raises_expr (void) const
{
  return this->raises_expr_;
}

TAO_PSDL_Node *
TAO_PSDL_Op_Dcl::context_expr (void) const
{
  return this->context_expr_;
}

/// = The Node methods.
int
TAO_PSDL_Op_Dcl::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_op_dcl (this);
}

/// ****************************************************************

TAO_PSDL_Op_Type_Spec::TAO_PSDL_Op_Type_Spec (TAO_PSDL_Node *type_spec)
  : type_spec_ (type_spec),
    type_ (0)
{
}

TAO_PSDL_Op_Type_Spec::TAO_PSDL_Op_Type_Spec (int type)
  : type_spec_ (0),
    type_ (type)
{
}

TAO_PSDL_Op_Type_Spec::~TAO_PSDL_Op_Type_Spec (void)
{
  if (this->type_spec_ != 0)
    delete this->type_spec_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Op_Type_Spec::type_spec (void) const
{
  return this->type_spec_;
}

int
TAO_PSDL_Op_Type_Spec::type (void) const
{
  return this->type_;
}

/// = The Node methods.
int
TAO_PSDL_Op_Type_Spec::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_op_type_spec (this);
}

/// ****************************************************************

TAO_PSDL_Parameter_Dcls::TAO_PSDL_Parameter_Dcls ()
  : param_dcl_ (0)
{
}

TAO_PSDL_Parameter_Dcls::TAO_PSDL_Parameter_Dcls (TAO_PSDL_Node *param_dcl)
  : param_dcl_ (param_dcl)
{
}

TAO_PSDL_Parameter_Dcls::~TAO_PSDL_Parameter_Dcls (void)
{
  delete this->param_dcl_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Parameter_Dcls::param_dcl (void) const
{
  return this->param_dcl_;
}

/// = The Node methods.
int
TAO_PSDL_Parameter_Dcls::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_parameter_dcls (this);
}

/// ****************************************************************

TAO_PSDL_Param_Dcl::TAO_PSDL_Param_Dcl (TAO_PSDL_Node *param_attribute,
                                        TAO_PSDL_Node *param_type_spec,
                                        TAO_PSDL_Node *simple_declarator)
  : param_attribute_ (param_attribute),
    param_type_spec_ (param_type_spec),
    simple_declarator_ (simple_declarator),
    param_dcl_ (0)
{
}

TAO_PSDL_Param_Dcl::TAO_PSDL_Param_Dcl (TAO_PSDL_Node *param_attribute,
                                        TAO_PSDL_Node *param_type_spec,
                                        TAO_PSDL_Node *simple_declarator,
                                        TAO_PSDL_Node *param_dcl)
  : param_attribute_ (param_attribute),
    param_type_spec_ (param_type_spec),
    simple_declarator_ (simple_declarator),
    param_dcl_ (param_dcl)
{
}

TAO_PSDL_Param_Dcl::~TAO_PSDL_Param_Dcl (void)
{
  delete this->param_attribute_;
  delete this->param_type_spec_;
  delete this->simple_declarator_;

  if (this->param_dcl_ != 0)
    delete this->param_dcl_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Param_Dcl::param_attribute (void) const
{
  return this->param_attribute_;
}

TAO_PSDL_Node *
TAO_PSDL_Param_Dcl::param_type_spec (void) const
{
  return this->param_type_spec_;
}

TAO_PSDL_Node *
TAO_PSDL_Param_Dcl::simple_declarator (void) const
{
  return this->simple_declarator_;
}

TAO_PSDL_Node *
TAO_PSDL_Param_Dcl::param_dcl (void) const
{
  return this->param_dcl_;
}

/// = The Node methods.
int
TAO_PSDL_Param_Dcl::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_param_dcl (this);
}

/// ****************************************************************

TAO_PSDL_Raises_Expr::TAO_PSDL_Raises_Expr (TAO_PSDL_Node *first_scoped_name)
  : first_scoped_name_ (first_scoped_name),
    second_scoped_name_ (0)
{
  this->psdl_scope_visitor ()->visit_raises_expr (this);

  this->set_identifier ("raises_exception");
}

TAO_PSDL_Raises_Expr::TAO_PSDL_Raises_Expr (TAO_PSDL_Node *first_scoped_name,
                                            TAO_PSDL_Node *second_scoped_name)
  : first_scoped_name_ (first_scoped_name),
    second_scoped_name_ (second_scoped_name)
{
  this->set_identifier ("raises_exception");

  this->psdl_scope_visitor ()->visit_raises_expr (this);
}

TAO_PSDL_Raises_Expr::~TAO_PSDL_Raises_Expr (void)
{
  delete this->first_scoped_name_;

  if (this->second_scoped_name_ != 0)
    delete this->second_scoped_name_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Raises_Expr::first_scoped_name (void) const
{
  return this->first_scoped_name_;
}

TAO_PSDL_Node *
TAO_PSDL_Raises_Expr::second_scoped_name (void) const
{
  return this->second_scoped_name_;
}

/// = The Node methods.
int
TAO_PSDL_Raises_Expr::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_raises_expr (this);
}

/// ****************************************************************
TAO_PSDL_Context_Expr::TAO_PSDL_Context_Expr (TAO_PSDL_Node *string_literal)
  : string_literal_ (string_literal)
{
}

TAO_PSDL_Context_Expr::~TAO_PSDL_Context_Expr (void)
{
  delete this->string_literal_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Context_Expr::string_literal (void) const
{
  return this->string_literal_;
}

/// = The Node methods.
int
TAO_PSDL_Context_Expr::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_context_expr (this);
}

/// ****************************************************************
TAO_PSDL_Param_Type_Spec::TAO_PSDL_Param_Type_Spec (
     TAO_PSDL_Node *type_of_param)
  : type_of_param_ (type_of_param)
{
}

TAO_PSDL_Param_Type_Spec::~TAO_PSDL_Param_Type_Spec (void)
{
  delete this->type_of_param_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Param_Type_Spec::type_of_param (void) const
{
  return this->type_of_param_;
}

/// = The Node methods.
int
TAO_PSDL_Param_Type_Spec::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_param_type_spec (this);
}

/// ****************************************************************

TAO_PSDL_Fixed_Pt_Type::TAO_PSDL_Fixed_Pt_Type (
     TAO_PSDL_Node *positive_int_const_one,
     TAO_PSDL_Node *positive_int_const_two)
  : positive_int_const_one_ (positive_int_const_one),
    positive_int_const_two_ (positive_int_const_two)
{
}

TAO_PSDL_Fixed_Pt_Type::~TAO_PSDL_Fixed_Pt_Type (void)
{
  delete this->positive_int_const_one_;
  delete this->positive_int_const_two_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Fixed_Pt_Type::positive_int_const_one (void) const
{
  return this->positive_int_const_one_;
}

TAO_PSDL_Node *
TAO_PSDL_Fixed_Pt_Type::positive_int_const_two (void) const
{
  return this->positive_int_const_two_;
}

int
TAO_PSDL_Fixed_Pt_Type::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_fixed_pt_type (this);
}

/// ****************************************************************

TAO_PSDL_Constr_Forward_Decl::TAO_PSDL_Constr_Forward_Decl (TAO_PSDL_Node * type,
                                                            TAO_PSDL_Node *identifier)
  : type_ (type),
    identifier_ (identifier)
{
}

TAO_PSDL_Constr_Forward_Decl::~TAO_PSDL_Constr_Forward_Decl (void)
{
  delete this->identifier_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Constr_Forward_Decl::type (void) const
{
  return this->type_;
}

TAO_PSDL_Node *
TAO_PSDL_Constr_Forward_Decl::identifier (void) const
{
  return this->identifier_;
}

int
TAO_PSDL_Constr_Forward_Decl::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_constr_forward_decl (this);
}

/// ***************************************************************

TAO_PSDL_Interface::TAO_PSDL_Interface (TAO_PSDL_Node *type_of_dcl)
  : type_of_dcl_ (type_of_dcl)
{
}

TAO_PSDL_Interface::~TAO_PSDL_Interface (void)
{
  delete this->type_of_dcl_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Interface::type_of_dcl (void) const
{
  return this->type_of_dcl_;
}

/// = The Node methods.
int
TAO_PSDL_Interface::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_interface (this);
}

/// ****************************************************************

TAO_PSDL_Interface_Body::TAO_PSDL_Interface_Body (TAO_PSDL_Node *export_variable)
  : export_variable_ (export_variable),
    interface_body_ (0)
{
}

TAO_PSDL_Interface_Body::TAO_PSDL_Interface_Body (
     TAO_PSDL_Node *export_variable,
     TAO_PSDL_Node *interface_body)
  : export_variable_ (export_variable),
    interface_body_ (interface_body)
{
}

TAO_PSDL_Interface_Body::~TAO_PSDL_Interface_Body (void)
{
  delete this->export_variable_;

  if (this->interface_body_ != 0)
    delete this->interface_body_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Interface_Body::export_variable (void) const
{
  return this->export_variable_;
}

TAO_PSDL_Node *
TAO_PSDL_Interface_Body::interface_body (void) const
{
  return this->interface_body_;
}


/// = The Node methods.
int
TAO_PSDL_Interface_Body::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_interface_body (this);
}

/// ****************************************************************

TAO_PSDL_Interface_Dcl::TAO_PSDL_Interface_Dcl (
     TAO_PSDL_Node *interface_header)
  : interface_header_ (interface_header),
    interface_body_ (0)
{
}

TAO_PSDL_Interface_Dcl::TAO_PSDL_Interface_Dcl (
     TAO_PSDL_Node *interface_header,
     TAO_PSDL_Node *interface_body)
  : interface_header_ (interface_header),
    interface_body_ (interface_body)
{
}


TAO_PSDL_Interface_Dcl::~TAO_PSDL_Interface_Dcl (void)
{
  delete this->interface_header_;

  if (this->interface_body_ != 0)
    delete this->interface_body_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Interface_Dcl::interface_header (void) const
{
  return this->interface_header_;
}

TAO_PSDL_Node *
TAO_PSDL_Interface_Dcl::interface_body (void) const
{
  return this->interface_body_;
}

/// = The Node methods.
int
TAO_PSDL_Interface_Dcl::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_interface_dcl (this);
}

/// ****************************************************************

TAO_PSDL_Forward_Dcl::TAO_PSDL_Forward_Dcl (TAO_PSDL_Node *identifier)
  : type_ (0),
    identifier_ (identifier)
{
  this->psdl_scope_visitor ()->visit_forward_dcl (this);

  TAO_PSDL_Scope::instance ()->add_interface_dcl_to_scope (this->identifiers_[0],
                         //@@ deal                         "forward_dcl",
                                                           this->psdl_scope ());
}

TAO_PSDL_Forward_Dcl::TAO_PSDL_Forward_Dcl (int type,
                                            TAO_PSDL_Node *identifier)
  : type_ (type),
    identifier_ (identifier)
{
  this->psdl_scope_visitor ()->visit_forward_dcl (this);

  TAO_PSDL_Scope::instance ()->add_interface_dcl_to_scope (this->identifiers_[0],
                                     //@@ deal             "forward_dcl",
                                                           this->psdl_scope ());

}

TAO_PSDL_Forward_Dcl::~TAO_PSDL_Forward_Dcl (void)
{
  delete this->identifier_;
}

/// Get the value
int
TAO_PSDL_Forward_Dcl::type (void) const
{
  return this->type_;
}

TAO_PSDL_Node *
TAO_PSDL_Forward_Dcl::identifier (void) const
{
  return this->identifier_;
}

/// = The Node methods.
int
TAO_PSDL_Forward_Dcl::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_forward_dcl (this);
}

/// ****************************************************************

TAO_PSDL_Interface_Header::TAO_PSDL_Interface_Header (TAO_PSDL_Node *identifier)
  : type_ (0),
    identifier_ (identifier),
    interface_inheritance_spec_ (0)
{
}

TAO_PSDL_Interface_Header::TAO_PSDL_Interface_Header (TAO_PSDL_Node *type,
                                                      TAO_PSDL_Node *identifier)
  : type_ (type),
    identifier_ (identifier),
    interface_inheritance_spec_ (0)
{
}

TAO_PSDL_Interface_Header::TAO_PSDL_Interface_Header (TAO_PSDL_Node * type,
                                                      TAO_PSDL_Node *identifier,
                                                      TAO_PSDL_Node *interface_inheritance_spec)
  : type_ (type),
    identifier_ (identifier),
    interface_inheritance_spec_ (interface_inheritance_spec)
{
}

TAO_PSDL_Interface_Header::~TAO_PSDL_Interface_Header (void)
{
  if (this->identifier_ != 0)
    delete this->identifier_;

  if (this->interface_inheritance_spec_ != 0)
    delete this->interface_inheritance_spec_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Interface_Header::type (void) const
{
  return this->type_;
}

TAO_PSDL_Node *
TAO_PSDL_Interface_Header::identifier (void) const
{
  return this->identifier_;
}

TAO_PSDL_Node *
TAO_PSDL_Interface_Header::interface_inheritance_spec (void) const
{
  return this->interface_inheritance_spec_;
}

/// = The Node methods.
int
TAO_PSDL_Interface_Header::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_interface_header (this);
}


/// ****************************************************************
TAO_PSDL_Export_Dcl::TAO_PSDL_Export_Dcl (TAO_PSDL_Node *type_of_export_one)
  : type_of_export_one_ (type_of_export_one),
    type_of_export_two_ (0)
{
}

TAO_PSDL_Export_Dcl::TAO_PSDL_Export_Dcl (TAO_PSDL_Node *type_of_export_one,
                                          TAO_PSDL_Node *type_of_export_two)
  : type_of_export_one_ (type_of_export_one),
    type_of_export_two_ (type_of_export_two)
{
}

TAO_PSDL_Export_Dcl::~TAO_PSDL_Export_Dcl (void)
{
  delete this->type_of_export_one_;

  if (this->type_of_export_two_ != 0)
    delete this->type_of_export_two_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Export_Dcl::type_of_export_one (void) const
{
  return this->type_of_export_one_;
}

TAO_PSDL_Node *
TAO_PSDL_Export_Dcl::type_of_export_two (void) const
{
  return this->type_of_export_two_;
}

/// = The Node methods.
int
TAO_PSDL_Export_Dcl::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_export_dcl (this);
}

/// ****************************************************************

TAO_PSDL_Interface_Inheritance_Spec::TAO_PSDL_Interface_Inheritance_Spec (
     TAO_PSDL_Node *interface_name)
  : interface_name_ (interface_name)
{
}

TAO_PSDL_Interface_Inheritance_Spec::~TAO_PSDL_Interface_Inheritance_Spec (void)
{
  if (this->interface_name_ != 0)
    delete this->interface_name_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Interface_Inheritance_Spec::interface_name (void) const
{
  return this->interface_name_;
}


/// = The Node methods.
int
TAO_PSDL_Interface_Inheritance_Spec::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_interface_inheritance_spec (this);
}

/// ****************************************************************

TAO_PSDL_Interface_Name::TAO_PSDL_Interface_Name (
     TAO_PSDL_Node *scoped_name)
  : scoped_name_ (scoped_name),
    interface_name_ (0)
{
}

TAO_PSDL_Interface_Name::TAO_PSDL_Interface_Name (
     TAO_PSDL_Node *scoped_name,
     TAO_PSDL_Node *interface_name)
  : scoped_name_ (scoped_name),
    interface_name_ (interface_name)
{
}

TAO_PSDL_Interface_Name::~TAO_PSDL_Interface_Name (void)
{
  delete this->scoped_name_;

  if (this->interface_name_ != 0)
    delete this->interface_name_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Interface_Name::scoped_name (void) const
{
  return this->scoped_name_;
}

TAO_PSDL_Node *
TAO_PSDL_Interface_Name::interface_name (void) const
{
  return this->interface_name_;
}

/// = The Node methods.
int
TAO_PSDL_Interface_Name::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_interface_name (this);
}

/// ****************************************************************

TAO_PSDL_Scoped_Name::TAO_PSDL_Scoped_Name (
     TAO_PSDL_Node *identifier)
  : identifier_ (identifier),
    scoped_name_ (0)
{
  this->psdl_scope_visitor ()->visit_scoped_name (this);
}

TAO_PSDL_Scoped_Name::TAO_PSDL_Scoped_Name (
     TAO_PSDL_Node *scoped_name,
     TAO_PSDL_Node *identifier)
  : identifier_ (identifier),
    scoped_name_ (scoped_name)
{
  this->psdl_scope_visitor ()->visit_scoped_name (this);
}

TAO_PSDL_Scoped_Name::~TAO_PSDL_Scoped_Name (void)
{
  delete this->identifier_;

  if (this->scoped_name_ != 0)
    delete this->scoped_name_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Scoped_Name::identifier (void) const
{
  return this->identifier_;
}

TAO_PSDL_Node *
TAO_PSDL_Scoped_Name::scoped_name (void) const
{
  return this->scoped_name_;
}

/// = The Node methods.
int
TAO_PSDL_Scoped_Name::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_scoped_name (this);
}

/// ****************************************************************

TAO_PSDL_Value::TAO_PSDL_Value (TAO_PSDL_Node *type_of_value_dcl)
  : type_of_value_dcl_ (type_of_value_dcl)
{
}

TAO_PSDL_Value::~TAO_PSDL_Value (void)
{
  delete this->type_of_value_dcl_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Value::type_of_value_dcl (void) const
{
  return this->type_of_value_dcl_;
}

/// = The Node methods.
int
TAO_PSDL_Value::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_value (this);
}

/// ****************************************************************

TAO_PSDL_Value_Common_Base::TAO_PSDL_Value_Common_Base (TAO_PSDL_Node *identifier)
  : identifier_ (identifier)
{
}

TAO_PSDL_Value_Common_Base::~TAO_PSDL_Value_Common_Base (void)
{
  delete this->identifier_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Value_Common_Base::identifier (void) const
{
  return this->identifier_;
}

/// = The Node methods.
int
TAO_PSDL_Value_Common_Base::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_value_common_base (this);
}

/// ****************************************************************

TAO_PSDL_Value_Forward_Dcl::TAO_PSDL_Value_Forward_Dcl (
     TAO_PSDL_Node *value_common_base)
  : value_common_base_ (value_common_base),
    abstract_ (0)
{
}

TAO_PSDL_Value_Forward_Dcl::TAO_PSDL_Value_Forward_Dcl (
     TAO_PSDL_Node *abstract,
     TAO_PSDL_Node *value_common_base)
  : value_common_base_ (value_common_base),
    abstract_ (abstract)
{
}

TAO_PSDL_Value_Forward_Dcl::~TAO_PSDL_Value_Forward_Dcl (void)
{
  delete this->value_common_base_;

  if (this->abstract_ != 0)
    delete this->abstract_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Value_Forward_Dcl::value_common_base (void) const
{
  return this->value_common_base_;
}

TAO_PSDL_Node *
TAO_PSDL_Value_Forward_Dcl::abstract (void) const
{
  return this->abstract_;
}

/// = The Node methods.
int
TAO_PSDL_Value_Forward_Dcl::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_value_forward_dcl (this);
}

/// ****************************************************************

TAO_PSDL_Value_Box_Dcl::TAO_PSDL_Value_Box_Dcl (
     TAO_PSDL_Node *value_common_base,
     TAO_PSDL_Node *type_spec)
  : value_common_base_ (value_common_base),
    type_spec_ (type_spec)
{
}

TAO_PSDL_Value_Box_Dcl::~TAO_PSDL_Value_Box_Dcl (void)
{
  delete this->value_common_base_;
  delete this->type_spec_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Value_Box_Dcl::value_common_base (void) const
{
  return this->value_common_base_;
}

TAO_PSDL_Node *
TAO_PSDL_Value_Box_Dcl::type_spec (void) const
{
  return this->type_spec_;
}

/// = The Node methods.
int
TAO_PSDL_Value_Box_Dcl::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_value_box_dcl (this);
}

/// ****************************************************************

TAO_PSDL_Value_Abs_Dcl::TAO_PSDL_Value_Abs_Dcl (
     TAO_PSDL_Node *value_common_base,
     TAO_PSDL_Node *export_variable)
  : value_common_base_ (value_common_base),
    value_inheritance_spec_ (0),
    export_variable_ (export_variable)
{
}

TAO_PSDL_Value_Abs_Dcl::TAO_PSDL_Value_Abs_Dcl (
     TAO_PSDL_Node *value_common_base,
     TAO_PSDL_Node *value_inheritance_spec,
     TAO_PSDL_Node *export_variable)
  : value_common_base_ (value_common_base),
    value_inheritance_spec_ (value_inheritance_spec),
    export_variable_ (export_variable)
{
}

TAO_PSDL_Value_Abs_Dcl::~TAO_PSDL_Value_Abs_Dcl (void)
{
  delete this->value_common_base_;
  delete this->export_variable_;

  if (this->value_inheritance_spec_ != 0)
    delete this->value_inheritance_spec_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Value_Abs_Dcl::value_common_base (void) const
{
  return this->value_common_base_;
}

TAO_PSDL_Node *
TAO_PSDL_Value_Abs_Dcl::export_variable (void) const
{
  return this->export_variable_;
}

TAO_PSDL_Node *
TAO_PSDL_Value_Abs_Dcl::value_inheritance_spec (void) const
{
  return this->value_inheritance_spec_;
}

/// = The Node methods.
int
TAO_PSDL_Value_Abs_Dcl::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_value_abs_dcl (this);
}

/// ****************************************************************

TAO_PSDL_Value_Dcl::TAO_PSDL_Value_Dcl (
     TAO_PSDL_Node *value_header,
     TAO_PSDL_Node *value_element)
  : value_header_ (value_header),
    value_element_ (value_element)
{
}

TAO_PSDL_Value_Dcl::~TAO_PSDL_Value_Dcl (void)
{
  delete this->value_header_;
  delete this->value_element_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Value_Dcl::value_header (void) const
{
  return this->value_header_;
}

TAO_PSDL_Node *
TAO_PSDL_Value_Dcl::value_element (void) const
{
  return this->value_element_;
}

/// = The Node methods.
int
TAO_PSDL_Value_Dcl::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_value_dcl (this);
}

/// ****************************************************************

TAO_PSDL_Value_Header::TAO_PSDL_Value_Header (
     TAO_PSDL_Node *value_common_base,
     TAO_PSDL_Node *value_inheritance_spec)
  : custom_ (0),
    value_common_base_ (value_common_base),
    value_inheritance_spec_ (value_inheritance_spec)
{
}

TAO_PSDL_Value_Header::TAO_PSDL_Value_Header (TAO_PSDL_Node *custom,
                                              TAO_PSDL_Node *value_common_base,
                                              TAO_PSDL_Node *value_inheritance_spec)
  : custom_ (custom),
    value_common_base_ (value_common_base),
    value_inheritance_spec_ (value_inheritance_spec)
{
}

TAO_PSDL_Value_Header::~TAO_PSDL_Value_Header (void)
{
  delete this->value_common_base_;

  if (this->custom_ != 0)
    delete this->custom_;

  delete this->value_inheritance_spec_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Value_Header::custom (void) const
{
  return this->custom_;
}

TAO_PSDL_Node *
TAO_PSDL_Value_Header::value_common_base (void) const
{
  return this->value_common_base_;
}

TAO_PSDL_Node *
TAO_PSDL_Value_Header::value_inheritance_spec (void) const
{
  return this->value_inheritance_spec_;
}

/// = The Node methods.
int
TAO_PSDL_Value_Header::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_value_header (this);
}

/// ****************************************************************

TAO_PSDL_Value_Inheritance_Spec::TAO_PSDL_Value_Inheritance_Spec (
     TAO_PSDL_Node *interface_name)
  : truncatable_ (0),
    value_name_ (0),
    interface_name_ (interface_name)
{
}

TAO_PSDL_Value_Inheritance_Spec::TAO_PSDL_Value_Inheritance_Spec (
     TAO_PSDL_Node *value_name,
     TAO_PSDL_Node *interface_name)
  : truncatable_ (0),
    value_name_ (value_name),
    interface_name_ (interface_name)
{
}

TAO_PSDL_Value_Inheritance_Spec::TAO_PSDL_Value_Inheritance_Spec (
     TAO_PSDL_Node *truncatable,
     TAO_PSDL_Node *value_name,
     TAO_PSDL_Node *interface_name)
  : truncatable_ (truncatable),
    value_name_ (value_name),
    interface_name_ (interface_name)
{
}

TAO_PSDL_Value_Inheritance_Spec::~TAO_PSDL_Value_Inheritance_Spec (void)
{
  if (this->truncatable_ != 0)
    delete this->truncatable_;

  delete this->interface_name_;

  if (this->value_name_ != 0)
    delete this->value_name_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Value_Inheritance_Spec::truncatable (void) const
{
  return this->truncatable_;
}

TAO_PSDL_Node *
TAO_PSDL_Value_Inheritance_Spec::value_name (void) const
{
  return this->value_name_;
}

TAO_PSDL_Node *
TAO_PSDL_Value_Inheritance_Spec::interface_name (void) const
{
  return this->interface_name_;
}

/// = The Node methods.
int
TAO_PSDL_Value_Inheritance_Spec::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_value_inheritance_spec (this);
}

/// ****************************************************************

TAO_PSDL_Value_Name::TAO_PSDL_Value_Name (TAO_PSDL_Node *scoped_name)
  : scoped_name_ (scoped_name),
    value_name_ (0)
{
}

TAO_PSDL_Value_Name::TAO_PSDL_Value_Name (TAO_PSDL_Node *scoped_name,
                                          TAO_PSDL_Node *value_name)
  : scoped_name_ (scoped_name),
    value_name_ (value_name)
{
}

TAO_PSDL_Value_Name::~TAO_PSDL_Value_Name (void)
{
  if (this->scoped_name_ != 0)
    delete this->scoped_name_;

  if (this->value_name_ != 0)
    delete this->value_name_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Value_Name::scoped_name (void) const
{
  return this->scoped_name_;
}

TAO_PSDL_Node *
TAO_PSDL_Value_Name::value_name (void) const
{
  return this->value_name_;
}

/// = The Node methods.
int
TAO_PSDL_Value_Name::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_value_name (this);
}

/// ****************************************************************

TAO_PSDL_Value_Element::TAO_PSDL_Value_Element (
     TAO_PSDL_Node *type_of_element)
  : type_of_element_ (type_of_element)
{
}

TAO_PSDL_Value_Element::~TAO_PSDL_Value_Element (void)
{
  delete this->type_of_element_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Value_Element::type_of_element (void) const
{
  return this->type_of_element_;
}

/// = The Node methods.
int
TAO_PSDL_Value_Element::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_value_element (this);
}

/// ****************************************************************

TAO_PSDL_Array_Declarator::TAO_PSDL_Array_Declarator (
     TAO_PSDL_Node *identifier,
     TAO_PSDL_Node *fixed_array_size)
  : identifier_ (identifier),
    fixed_array_size_ (fixed_array_size)
{
}

TAO_PSDL_Array_Declarator::~TAO_PSDL_Array_Declarator (void)
{
  delete this->identifier_;
  delete this->fixed_array_size_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Array_Declarator::identifier (void) const
{
  return this->identifier_;
}

TAO_PSDL_Node *
TAO_PSDL_Array_Declarator::fixed_array_size (void) const
{
  return this->fixed_array_size_;
}

/// = The Node methods.
int
TAO_PSDL_Array_Declarator::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_array_declarator (this);
}

/// ****************************************************************

TAO_PSDL_Fixed_Array_Size::TAO_PSDL_Fixed_Array_Size (
     TAO_PSDL_Node *positive_int_const)
  : positive_int_const_ (positive_int_const)
{
}

TAO_PSDL_Fixed_Array_Size::~TAO_PSDL_Fixed_Array_Size (void)
{
  delete this->positive_int_const_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Fixed_Array_Size::positive_int_const (void) const
{
  return this->positive_int_const_;
}

/// = The Node methods.
int
TAO_PSDL_Fixed_Array_Size::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_fixed_array_size (this);
}

/// ****************************************************************

TAO_PSDL_Enumerator::TAO_PSDL_Enumerator (TAO_PSDL_Node *identifier)
  : identifier_ (identifier),
    enumerator_ (0)
{
}

TAO_PSDL_Enumerator::TAO_PSDL_Enumerator (TAO_PSDL_Node *identifier,
                                          TAO_PSDL_Node *enumerator)
  : identifier_ (identifier),
    enumerator_ (enumerator)
{
}

TAO_PSDL_Enumerator::~TAO_PSDL_Enumerator (void)
{
  delete this->identifier_;

  if (this->enumerator_ != 0)
    delete this->enumerator_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Enumerator::identifier (void) const
{
  return this->identifier_;
}

TAO_PSDL_Node *
TAO_PSDL_Enumerator::enumerator (void) const
{
  return this->enumerator_;
}

/// = The Node methods.
int
TAO_PSDL_Enumerator::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_enumerator (this);
}

/// ****************************************************************

TAO_PSDL_Literal::TAO_PSDL_Literal (CORBA::Boolean boolean_value)
  : boolean_value_ (boolean_value),
    int_value_ (0),
    double_value_ (0),
    char_value_ (0)
{
}

TAO_PSDL_Literal::TAO_PSDL_Literal (int int_value)
  : boolean_value_ (0),
    int_value_ (int_value),
    double_value_ (0),
    char_value_ (0)
{
}

TAO_PSDL_Literal::TAO_PSDL_Literal (double double_value)
  : boolean_value_ (0),
    int_value_ (0),
    double_value_ (double_value),
    char_value_ (0)
{
}

TAO_PSDL_Literal::TAO_PSDL_Literal (const char *char_value)
  : boolean_value_ (0),
    int_value_ (0),
    double_value_ (0),
    char_value_ (char_value)
{
}

TAO_PSDL_Literal::~TAO_PSDL_Literal (void)
{
  // We cannot delete this since it is a constant
  /*if (this->char_value_ != 0)
    delete this->char_value_; */
}

/// Get the value
CORBA::Boolean
TAO_PSDL_Literal::boolean_value (void) const
{
  return this->boolean_value_;
}

int
TAO_PSDL_Literal::int_value (void) const
{
  return this->int_value_;
}

double
TAO_PSDL_Literal::double_value (void) const
{
  return this->double_value_;
}

const char *
TAO_PSDL_Literal::char_value (void) const
{
  return this->char_value_;
}

/// = The Node methods.
int
TAO_PSDL_Literal::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_literal (this);
}

/// ****************************************************************

TAO_PSDL_State_Member::TAO_PSDL_State_Member (TAO_PSDL_Node *public_or_private,
                                              TAO_PSDL_Node *type_spec,
                                              TAO_PSDL_Node *declarators)
  : public_or_private_ (public_or_private),
    type_spec_ (type_spec),
    declarators_ (declarators)
{
}

TAO_PSDL_State_Member::~TAO_PSDL_State_Member (void)
{
  delete this->public_or_private_;

  delete this->type_spec_;

  delete this->declarators_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_State_Member::public_or_private (void) const
{
  return this->public_or_private_;
}

TAO_PSDL_Node *
TAO_PSDL_State_Member::type_spec (void) const
{
  return this->type_spec_;
}

TAO_PSDL_Node *
TAO_PSDL_State_Member::declarators (void) const
{
  return this->declarators_;
}

/// = The Node methods.
int
TAO_PSDL_State_Member::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_state_member (this);
}

/// ****************************************************************

TAO_PSDL_Init_Dcl::TAO_PSDL_Init_Dcl (TAO_PSDL_Node *identifier)
  : identifier_ (identifier),
    init_param_decls_ (0)
{
}

TAO_PSDL_Init_Dcl::TAO_PSDL_Init_Dcl (TAO_PSDL_Node *identifier,
                                      TAO_PSDL_Node *init_param_decls)
  : identifier_ (identifier),
    init_param_decls_ (init_param_decls)
{
}

TAO_PSDL_Init_Dcl::~TAO_PSDL_Init_Dcl (void)
{
  delete this->identifier_;

  if (this->init_param_decls_ != 0)
    delete this->init_param_decls_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Init_Dcl::identifier (void) const
{
  return this->identifier_;
}

TAO_PSDL_Node *
TAO_PSDL_Init_Dcl::init_param_decls (void) const
{
  return this->init_param_decls_;
}

/// = The Node methods.
int
TAO_PSDL_Init_Dcl::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_init_dcl (this);
}

/// ****************************************************************

TAO_PSDL_Init_Param_Decls::TAO_PSDL_Init_Param_Decls (TAO_PSDL_Node *init_param_decl)
  : init_param_decl_ (init_param_decl),
    init_param_decls_ (0)
{
}

TAO_PSDL_Init_Param_Decls::TAO_PSDL_Init_Param_Decls (TAO_PSDL_Node *init_param_decl,
                                                      TAO_PSDL_Node *init_param_decls)
  : init_param_decl_ (init_param_decl),
    init_param_decls_ (init_param_decls)
{
}

TAO_PSDL_Init_Param_Decls::~TAO_PSDL_Init_Param_Decls (void)
{
  delete this->init_param_decl_;

  if (this->init_param_decls_ != 0)
    delete this->init_param_decls_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Init_Param_Decls::init_param_decl (void) const
{
  return this->init_param_decl_;
}

TAO_PSDL_Node *
TAO_PSDL_Init_Param_Decls::init_param_decls (void) const
{
  return this->init_param_decls_;
}

/// = The Node methods.
int
TAO_PSDL_Init_Param_Decls::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_init_param_decls (this);
}

/// ****************************************************************

TAO_PSDL_Init_Param_Decl::TAO_PSDL_Init_Param_Decl (TAO_PSDL_Node *init_param_attribute,
                                              TAO_PSDL_Node *param_type_spec,
                                              TAO_PSDL_Node *simple_declarator)
  : init_param_attribute_ (init_param_attribute),
    param_type_spec_ (param_type_spec),
    simple_declarator_ (simple_declarator)
{
}

TAO_PSDL_Init_Param_Decl::~TAO_PSDL_Init_Param_Decl (void)
{
  delete this->init_param_attribute_;

  delete this->param_type_spec_;

  delete this->simple_declarator_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Init_Param_Decl::init_param_attribute (void) const
{
  return this->init_param_attribute_;
}

TAO_PSDL_Node *
TAO_PSDL_Init_Param_Decl::param_type_spec (void) const
{
  return this->param_type_spec_;
}

TAO_PSDL_Node *
TAO_PSDL_Init_Param_Decl::simple_declarator (void) const
{
  return this->simple_declarator_;
}

/// = The Node methods.
int
TAO_PSDL_Init_Param_Decl::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_init_param_decl (this);
}

/// ****************************************************************

TAO_PSDL_Factory_Parameters::TAO_PSDL_Factory_Parameters ()
  : simple_declarator_ (0)
{
}

TAO_PSDL_Factory_Parameters::TAO_PSDL_Factory_Parameters (TAO_PSDL_Node *simple_declarator)
  : simple_declarator_ (simple_declarator)
{
}

TAO_PSDL_Factory_Parameters::~TAO_PSDL_Factory_Parameters (void)
{
  if (this->simple_declarator_ != 0)
    delete this->simple_declarator_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Factory_Parameters::simple_declarator (void) const
{
  return this->simple_declarator_;
}

/// = The Node methods.
int
TAO_PSDL_Factory_Parameters::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_factory_parameters (this);
}

/// ****************************************************************

TAO_PSDL_Attr_Dcl::TAO_PSDL_Attr_Dcl (int readonly,
                                      TAO_PSDL_Node *param_type_spec,
                                      TAO_PSDL_Node *simple_declarator)
  : readonly_ (readonly),
    param_type_spec_ (param_type_spec),
    simple_declarator_ (simple_declarator)
{
}

TAO_PSDL_Attr_Dcl::TAO_PSDL_Attr_Dcl (TAO_PSDL_Node *param_type_spec,
                                      TAO_PSDL_Node *simple_declarator)
  : readonly_ (0),
    param_type_spec_ (param_type_spec),
    simple_declarator_ (simple_declarator)
{
}

TAO_PSDL_Attr_Dcl::~TAO_PSDL_Attr_Dcl (void)
{
  delete this->param_type_spec_;

  delete this->simple_declarator_;
}

/// Get the value
int
TAO_PSDL_Attr_Dcl::readonly (void) const
{
  return this->readonly_;
}

TAO_PSDL_Node *
TAO_PSDL_Attr_Dcl::param_type_spec (void) const
{
  return this->param_type_spec_;
}

TAO_PSDL_Node *
TAO_PSDL_Attr_Dcl::simple_declarator (void) const
{
  return this->simple_declarator_;
}

/// = The Node methods.
int
TAO_PSDL_Attr_Dcl::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_attr_dcl (this);
}

/// ****************************************************************

TAO_PSDL_Positive_Int_Const::TAO_PSDL_Positive_Int_Const (TAO_PSDL_Node *const_exp)
  : const_exp_ (const_exp)
{
}

TAO_PSDL_Positive_Int_Const::~TAO_PSDL_Positive_Int_Const (void)
{
  delete this->const_exp_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Positive_Int_Const::const_exp (void) const
{
  return this->const_exp_;
}

/// = The Node methods.
int
TAO_PSDL_Positive_Int_Const::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_positive_int_const (this);
}

/// ****************************************************************

TAO_PSDL_Factory_Dcl::TAO_PSDL_Factory_Dcl (TAO_PSDL_Node *identifier,
                                            TAO_PSDL_Node *factory_parameters)
  : identifier_ (identifier),
    factory_parameters_ (factory_parameters)
{
}

TAO_PSDL_Factory_Dcl::~TAO_PSDL_Factory_Dcl (void)
{
  delete this->identifier_;

  if (this->factory_parameters_ != 0)
    delete this->factory_parameters_;
}

/// Get the value
TAO_PSDL_Node *
TAO_PSDL_Factory_Dcl::identifier (void) const
{
  return this->identifier_;
}

TAO_PSDL_Node *
TAO_PSDL_Factory_Dcl::factory_parameters (void) const
{
  return this->factory_parameters_;
}

/// = The Node methods.
int
TAO_PSDL_Factory_Dcl::accept (TAO_PSDL_Node_Visitor *visitor)
{
  return visitor->visit_factory_dcl (this);
}
