/**
 * @file IDL3_to_XMI_visitor.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * $Id$
 * Implements a visitor that generates XMI for the CARDAMOM
 * code generation facility.
 *
 * This IDL->XMI mapping appears to be based on a IDL->UML->XMI
 * mapping, and should be valid XMI version 1.1
 */

#ifndef CIAO_XMI_VISITOR_H
#define CIAO_XMI_VISITOR_H

#include "ace/Containers.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Null_Mutex.h"
#include "ace/SString.h"

#include "xercesc/util/XercesDefs.hpp"

#include "ast_visitor.h"
#include "ast_component.h"
#include "ast_concrete_type.h"

#include "utl_scoped_name.h"
#include "Literals.h"
#include "XML/XercesString.h"
#include "CIAO_IDL3_TO_XMI_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_OutStream;
class UTL_ExceptList;

namespace XERCES_CPP_NAMESPACE
{
  class DOMDocument;
  class DOMElement;
}

namespace DAnCE
{
  namespace XMI
  {

    /**
     * @class idl3_to_xmi_visitor
     * @brief Visitor that generates XMI for the CARDAMOM code
     * generation facilities.
     */
    class CIAO_IDL3_TO_XMI_Export idl3_to_xmi_visitor : public ast_visitor
    {
    public:
      idl3_to_xmi_visitor (bool skip_imported);
      virtual ~idl3_to_xmi_visitor (void);

      virtual int visit_decl (AST_Decl *d);
      virtual int visit_scope (UTL_Scope *node);
      virtual int visit_type (AST_Type *node);
      virtual int visit_predefined_type (AST_PredefinedType *node);
      virtual int visit_module (AST_Module *node);
      virtual int visit_template_module (AST_Template_Module *node);
      virtual int visit_template_module_inst (AST_Template_Module_Inst *node);
      virtual int visit_template_module_ref (AST_Template_Module_Ref *node);
      virtual int visit_param_holder(AST_Param_Holder *node);
      virtual int visit_interface (AST_Interface *node);
      virtual int visit_interface_fwd (AST_InterfaceFwd *node);
      virtual int visit_valuebox (AST_ValueBox *node);
      virtual int visit_valuetype (AST_ValueType *node);
      virtual int visit_valuetype_fwd (AST_ValueTypeFwd *node);
      virtual int visit_eventtype (AST_EventType *node);
      virtual int visit_eventtype_fwd (AST_EventTypeFwd *node);
      virtual int visit_component (AST_Component *node);
      virtual int visit_component_fwd (AST_ComponentFwd *node);
      virtual int visit_provides (AST_Provides *node);
      virtual int visit_uses (AST_Uses *node);
      virtual int visit_publishes (AST_Publishes *node);
      virtual int visit_emits (AST_Emits *node);
      virtual int visit_consumes (AST_Consumes *node);
      virtual int visit_porttype (AST_PortType *node);
      virtual int visit_extended_port (AST_Extended_Port *node);
      virtual int visit_mirror_port (AST_Mirror_Port *node);
      virtual int visit_connector (AST_Connector *node);
      virtual int visit_home (AST_Home *node);
      virtual int visit_factory (AST_Factory *node);
      virtual int visit_structure (AST_Structure *node);
      virtual int visit_structure_fwd (AST_StructureFwd *node);
      virtual int visit_exception (AST_Exception *node);
      virtual int visit_expression (AST_Expression *node);
      virtual int visit_enum (AST_Enum *node);
      virtual int visit_operation (AST_Operation *node);
      virtual int visit_field (AST_Field *node);
      virtual int visit_argument (AST_Argument *node);
      virtual int visit_attribute (AST_Attribute *node);
      virtual int visit_union (AST_Union *node);
      virtual int visit_union_fwd (AST_UnionFwd *node);
      virtual int visit_union_branch (AST_UnionBranch *node);
      virtual int visit_union_label (AST_UnionLabel *node);
      virtual int visit_constant (AST_Constant *node);
      virtual int visit_enum_val (AST_EnumVal *node);
      virtual int visit_array (AST_Array *node);
      virtual int visit_sequence (AST_Sequence *node);
      virtual int visit_string (AST_String *node);
      virtual int visit_typedef (AST_Typedef *node);
      virtual int visit_root (AST_Root *node);
      virtual int visit_native (AST_Native *node);
      virtual int visit_finder (AST_Finder *node);

      struct Error
      {
        Error (const ACE_CString &diag, AST_Decl *node = 0)
          : diag_ (diag),
            node_ (node)
        {}

        void node (AST_Decl *node) { if (node_ == 0) node_ = node; }

        const ACE_CString diag_;
        AST_Decl *node_;
      };

    private:
      /// Generates several elements common to most XMI elements.
      /// Generates the following attributes: xmi.id, name, visibility, namespace, stereotype
      /// and the tagged value grouping.
      /// @param stereotype The stereotype this element should be associated with.
      void gen_common_elements (AST_Decl *node, const ACE_TCHAR *stereotype);

      /// Converts a number into a string representation.
      /// Caller responsible for memory.
      template <typename T>
      ACE_TCHAR * number_to_string (T val);

      /// Common elements of operation XMI generation
      void visit_operation_impl (AST_Operation *node, const ACE_TCHAR *stereotype);

      /// implementation for elements common to both eventtypes
      /// and valuetypes
      void visit_valuetype_impl (AST_ValueType *node);

      /// implementation of elements common to exceptions and structures.
      void visit_struct_impl (AST_Structure *node);

      /// Generation of common associations of anonymous and typedefed arrays.
      void gen_array_associations (AST_Decl *node, AST_Array *array);

      /// Generation of common associations of anonymous and typedefed sequences.
      void gen_sequence_associations (AST_Decl *node, AST_Sequence *sequence);

      /// sets an attribute on the element at the top of the stack.
      void set_attribute (const ACE_TCHAR *name,
                          const ACE_TCHAR *value);


      /// sets an attribute on the element at the top of the stack
      void set_attribute (const ACE_TCHAR *name,
                          const XMLCh *value);

      /// Adds a text child to the element at the top of the stack
      void add_text (const ACE_TCHAR *text);

      /// Greates a new element as a child of the current stack element
      XERCES_CPP_NAMESPACE::DOMElement * create_element (const ACE_TCHAR *name);

      /// Generate stereotypes for XMI document
      XERCES_CPP_NAMESPACE::DOMElement * generate_stereotypes (void);

      /// Helper method which creates new stereotype elements
      void add_stereotype (const LITERAL_T name,
                           const LITERAL_T bc);

      /// Generates an XMI ID.
      /// @param node If passed, the file name and line number will be
      ///             generated as part of the ID.
      static XMLCh * gen_xmi_id (AST_Decl * node = 0);

      /// Generates an XMI ID with file_name and line as part of the id.
      static XMLCh * gen_xmi_id (const ACE_TCHAR *file_name, long line);

      /// Used to store the next ID for generation
      static size_t current_id_;

      /// Generates the tagged value grouping common to most XMI elements
      void gen_tagged_value (AST_Decl *);

      /// Sets an attribute with name containment_name whose value is the
      /// ID of the top of the namespace stack.
      void set_containing_element (const ACE_TCHAR *containment_name);

      /// Looks up the xmi:id assoviated with xid_name and sets it as an
      /// attribute
      void find_and_set_xid_as_attr (const ACE_TCHAR *attr_name, const ACE_TCHAR *xid_name);

      /// Creates a XMI ID based on node, and sets its value to
      /// the xmi.id attribute of the current top of element stack.
      void create_and_store_xmi_id (AST_Decl *node);

      /// Creates a XMI ID based on node, and sets its value to
      /// the name attribute of the current top of element stack.
      void create_and_store_xmi_id (const ACE_TCHAR *name, AST_Decl *node = 0);

      /// Finds and returns the xmi.id associated with name.
      XMLCh * lookup_xid (const ACE_TCHAR *name);

      /// Finds and returns the xmi.id associated with the provided type.
      XMLCh * lookup_type_xid (AST_Type *);

      /// Used to add generalizations - most commonly used for supports
      /// relationships.  in component foo supports bar, foo is the sub,
      /// bar is the super.
      XMLCh * add_generalization (const ACE_TCHAR *sub, const ACE_TCHAR *super);
      XMLCh * add_generalization (AST_Type *sub, AST_Type *super);

      XMLCh * add_generalization (const XMLCh *sub, const XMLCh *super);

      /// @param component Repository ID of component
      /// @param port_kind text of port stereotype from LITERALS
      /// @param port_type Repository ID of port type (ie, interface type for facet)
      /// @param name The name of the port
      /// @param is_multiple The multiplicity of the port (ie, uses multiple.)
      void add_port (const ACE_TCHAR *port_kind,
                     AST_Field *port_node);

      void add_managed_component (const ACE_TCHAR *home, const ACE_TCHAR *component);


      ACE_TCHAR * union_label_value (AST_Expression *exp);

      typedef ACE_Hash_Map_Manager_Ex< ACE_TString,
                                       DAnCE::XML::XStr,
                                       ACE_Hash <ACE_TString>,
                                       ACE_Equal_To <ACE_TString>,
                                       ACE_Null_Mutex > XMI_ID_MAP;

      /// Stores xmi.ids for elements, indexed by repoId.
      XMI_ID_MAP id_map_;

      typedef ACE_Unbounded_Stack<XERCES_CPP_NAMESPACE::DOMElement *> ELEMENT_STACK;

      /// Stores elements we are currently generating XMI.  Top of stack is the current
      /// element.
      ELEMENT_STACK stack_;

      struct ElementContext {
        ACE_TString ns_;
        XERCES_CPP_NAMESPACE::DOMElement *elem_;
      };

      typedef ACE_Hash_Map_Manager_Ex< ACE_TString,
                                       ElementContext,
                                       ACE_Hash <ACE_TString>,
                                       ACE_Equal_To <ACE_TString>,
                                       ACE_Null_Mutex > REPO_ID_MAP;

      /// Stores elements that were forward declared. Also stores modules that can
      /// be reopened.
      REPO_ID_MAP repo_id_map_;

      /// Provides a "protected" push that is popped when the current scope
      /// is exited.
      struct ES_Guard
      {
        ES_Guard (const ACE_TCHAR *name, idl3_to_xmi_visitor *);
        ES_Guard (XERCES_CPP_NAMESPACE::DOMElement *ele,
                  idl3_to_xmi_visitor *);
        ~ES_Guard (void);

        idl3_to_xmi_visitor &vis_;
      };

      friend struct ES_Guard;

      typedef ACE_Unbounded_Stack< ACE_TString > STRING_STACK;

      /// Stores namespaces associated with our current scope.
      STRING_STACK namespace_;

      /// Provides a protected push for the namespace stack that
      /// pops when the current scope is exited.
      struct NS_Guard
      {
        NS_Guard (const ACE_TCHAR *name, idl3_to_xmi_visitor *);
        ~NS_Guard (void);

        idl3_to_xmi_visitor &vis_;
      };

      friend struct NS_Guard;

      XERCES_CPP_NAMESPACE::DOMDocument *dom_;

      XERCES_CPP_NAMESPACE::DOMElement *root_;

      XERCES_CPP_NAMESPACE::DOMElement *generalizations_;

      XERCES_CPP_NAMESPACE::DOMElement *associations_;

      DAnCE::XML::XStr base_id_;

      /// true if for some reason the generated output is bad
      /// and shouldn't be written.
      bool output_dirty_;

      bool skip_imported_;

      /// true if we are in enum "scope".
      bool visiting_enum_;

      /// Used to determine IDL ordering of union labels.
      size_t order_;

      /// USed to cache the type of union descriminators
      AST_ConcreteType *union_disc_;

      /// Used to cache the type whose scope we are currently in.
      AST_Decl *cached_type_;
    };
  }
}

#endif
