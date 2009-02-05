// $Id$

#include "IDL3_to_XMI_visitor.h"
#include "identifier_helper.h"

#include "be_sunsoft.h"
#include "be_extern.h"

#include "ast_argument.h"
#include "ast_array.h"
#include "ast_attribute.h"
#include "ast_component_fwd.h"
#include "ast_enum.h"
#include "ast_enum_val.h"
#include "ast_eventtype.h"
#include "ast_eventtype_fwd.h"
#include "ast_exception.h"
#include "ast_factory.h"
#include "ast_field.h"
#include "ast_home.h"
#include "ast_operation.h"
#include "ast_root.h"
#include "ast_sequence.h"
#include "ast_string.h"
#include "ast_structure_fwd.h"
#include "ast_union.h"
#include "ast_union_branch.h"
#include "ast_union_fwd.h"
#include "ast_union_label.h"
#include "ast_valuebox.h"
#include "ast_valuetype_fwd.h"
#include "ast_native.h"
#include "utl_exceptlist.h"
#include "utl_identifier.h"
#include "utl_idlist.h"
#include "utl_string.h"
#include "global_extern.h"
#include "nr_extern.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/ACE.h"

#include "Literals.h"
#include "XML/XercesString.h"

#include <iostream>
#include <limits>
#include <sstream>

#include "xercesc/dom/DOM.hpp"

#include "XML/XML_Helper.h"

using XERCES_CPP_NAMESPACE::DOMDocument;
using XERCES_CPP_NAMESPACE::DOMAttr;
using XERCES_CPP_NAMESPACE::DOMElement;
using XERCES_CPP_NAMESPACE::DOMText;
using XERCES_CPP_NAMESPACE::DOMDocumentType;
using CIAO::XML::XStr;



#if 0
struct Foo {
  std::string foo_;
  Foo (const char *foo) : foo_ (foo)
  {
    std::cout << "Entering " << foo_ << std::endl;
  }
  ~Foo ()
  {
    std::cout << "Leaving " << foo_ << std::endl;
  }
};

#define XMI_TRACE(X) Foo _foobarbaz (X);
#else
#define XMI_TRACE(X)
#endif

namespace CIAO
{
  namespace XMI
  {
    size_t idl3_to_xmi_visitor::current_id_ = 0;

    template <typename T>
    ACE_TCHAR *
    idl3_to_xmi_visitor::number_to_string (T val)
    {
      std::stringstream str;

      str << val;

      return ACE::strnew (ACE_TEXT_CHAR_TO_TCHAR (str.str ().c_str ()));
    }

    template <>
    ACE_TCHAR *
    idl3_to_xmi_visitor::number_to_string (bool val)
    {
      if (val)
        return ACE::strnew (LITERALS[CAPS_TRUE]);
      else
        return ACE::strnew (LITERALS[CAPS_FALSE]);
    }

    idl3_to_xmi_visitor::idl3_to_xmi_visitor (bool skip_imported)
      : dom_ (0),
        root_ (0),
        generalizations_ (0),
        associations_ (0),
        base_id_ (idl3_to_xmi_visitor::gen_xmi_id ()),
        skip_imported_ (skip_imported),
        union_disc_ (0)
    {
    }

    idl3_to_xmi_visitor::~idl3_to_xmi_visitor (void)
    {
    }

    int
    idl3_to_xmi_visitor::visit_decl (AST_Decl *)
    {
      XMI_TRACE ("got a decl");
      return 0;
    }

    char *
    copy_scoped_name (UTL_ScopedName *name)
    {
      ACE_CString fullname;
      for (UTL_ScopedNameActiveIterator si (name);
           !si.is_done ();
           si.next ())
        {
          Identifier *item = si.item ();
          fullname += item->get_string ();
        }

      return fullname.rep ();
    }

    int
    idl3_to_xmi_visitor::visit_scope (UTL_Scope *node)
    {
      XMI_TRACE ("got a scope");

      this->order_ = 0;

      for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
           !si.is_done ();
           si.next ())
        {
          AST_Decl *d = si.item ();
          d->ast_accept (this);
          ++this->order_;
        }
      return 0;
    }

    int
    idl3_to_xmi_visitor::visit_type (AST_Type *)
    {
      XMI_TRACE ("got a type");
      return 0;
    }

    int
    idl3_to_xmi_visitor::visit_predefined_type (AST_PredefinedType *t)
    {
      XMI_TRACE ("predef type");
      ACE_DEBUG ((LM_DEBUG, "pdt: %s\n", t->repoID ()));

      switch (t->pt ())
        {
        case AST_PredefinedType::PT_pseudo:
          this->create_and_store_xmi_id (t);
        default:
          break;
        }
      return 0;
    }

    int
    idl3_to_xmi_visitor::visit_module (AST_Module *node)
    {
      XMI_TRACE ("module");

      try
        {
          ES_Guard package_guard (LITERALS[PACKAGE_TAG], this);

          this->gen_common_elements (node, LITERALS[ST_MODULE]);

          {
            NS_Guard ns_guard (ACE_TEXT_CHAR_TO_TCHAR (node->repoID ()), this);
            ES_Guard owned_guard (LITERALS[OWNEDELEMENT_TAG], this);
            this->visit_scope (node);
          }
        }
      catch (Error &err)
        {
          err.node (node);
          throw;
        }

      return 0;
    }

    int
    idl3_to_xmi_visitor::visit_interface (AST_Interface *node)
    {
      XMI_TRACE ("interface");

      if (this->skip_imported_ && node->imported ())
        {
          this->visit_scope (node);
          return 0;
        }

      try
        {
          ES_Guard class_guard (LITERALS[CLASS_TAG], this);

          this->gen_common_elements (node, LITERALS[ST_INTF]);

          NS_Guard ns_guard (ACE_TEXT_CHAR_TO_TCHAR (node->repoID ()), this);

          { // Operations.
            ES_Guard owned_guard (LITERALS[OWNEDELEMENT_TAG], this);
            this->cached_type_ = node;
            this->visit_scope (node);
            this->cached_type_ = 0;
          }

          // Inheritance
          for (long i = 0; i < node->n_inherits (); ++i)
            {
              XStr xid (this->add_generalization (ACE_TEXT_CHAR_TO_TCHAR (node->repoID ()),
                                                  ACE_TEXT_CHAR_TO_TCHAR (node->inherits ()[i]->repoID ())));
            }

        }
      catch (Error &err)
        {
          err.node (node);
          throw;
        }

      return 0;
    }

    int
    idl3_to_xmi_visitor::visit_interface_fwd (AST_InterfaceFwd *node)
    {
      XMI_TRACE ("interface_fwd");

      try
        {
          this->create_and_store_xmi_id (node);
        }
      catch (Error &err)
        {
          err.node (node);
          throw;
        }

      return 0;
    }

    int
    idl3_to_xmi_visitor::visit_valuebox (AST_ValueBox *node)
    {
      XMI_TRACE ("valuebox");

      if (this->skip_imported_ && node->imported ())
        return 0;

      try
        {
          ES_Guard class_guard (LITERALS[CLASS_TAG], this);
          this->gen_common_elements (node, LITERALS[ST_BOXVALUE]);

          // add a generalization for the value we box
          XStr xid (this->add_generalization (ACE_TEXT_CHAR_TO_TCHAR (node->repoID ()),
                                              ACE_TEXT_CHAR_TO_TCHAR (node->boxed_type ()->repoID ())));
        }
      catch (Error &err)
        {
          err.node (node);
          throw;
        }

      return 0;
    }

    int
    idl3_to_xmi_visitor::visit_valuetype (AST_ValueType *node)
    {
      XMI_TRACE ("valuetype");

      if (this->skip_imported_ && node->imported ())
        {
          this->visit_scope (node);
          return 0;
        }

      try
        {
          ES_Guard class_guard (LITERALS[CLASS_TAG], this);

          this->visit_valuetype_impl (node, LITERALS[ST_VALUE]);
        }
      catch (Error &err)
        {
          err.node (node);
          throw;
        }

      return 0;
    }

    void
    idl3_to_xmi_visitor::visit_valuetype_impl (AST_ValueType *node, const ACE_TCHAR *stereo)
    {
      try
        {
          this->gen_common_elements (node, stereo);

          // isAbstract
          if (node->is_abstract ())
            this->set_attribute (LITERALS[ABSTRACT], LITERALS[_TRUE]);
          else this->set_attribute (LITERALS[ABSTRACT], LITERALS[_FALSE]);

          NS_Guard ns_guard (ACE_TEXT_CHAR_TO_TCHAR (node->repoID ()), this);

          if (node->inherits_concrete () != 0)
            XStr xid (this->add_generalization (ACE_TEXT_CHAR_TO_TCHAR (node->repoID ()),
                                                ACE_TEXT_CHAR_TO_TCHAR (node->inherits_concrete ()->repoID ())));

          for (long i = 0; i < node->n_supports (); ++i)
            {
              XStr xid (this->add_generalization (ACE_TEXT_CHAR_TO_TCHAR (node->repoID ()),
                                                  ACE_TEXT_CHAR_TO_TCHAR (node->supports ()[i]->repoID ())));
            }

          /* NOTE: CDMW XMI generator ignores supports specifications.
          // Supports
          if (node->n_supports () != 0)
          throw Error ("ValueTypes which support are not currently supported.", node);
          */

          ES_Guard oe_guard (LITERALS[OWNEDELEMENT_TAG], this);

          this->visit_scope (node);
        }
      catch (Error &err)
        {
          err.node (node);
          throw;
        }
    }

    int
    idl3_to_xmi_visitor::visit_valuetype_fwd (AST_ValueTypeFwd *node)
    {
      XMI_TRACE ("valuetype_fwd");

      try
        {
          this->create_and_store_xmi_id (node);
        }
      catch (Error &err)
        {
          err.node (node);
          throw;
        }

      return 0;
    }

    int
    idl3_to_xmi_visitor::visit_component (AST_Component *node)
    {
      XMI_TRACE ("component");

      if (this->skip_imported_ && node->imported ())
        {
          this->visit_scope (node);
          return 0;
        }

      try
        {
          ES_Guard class_guard (LITERALS[CLASS_TAG], this);

          this->gen_common_elements (node, LITERALS[ST_COMP]);

          NS_Guard ns_guard (ACE_TEXT_CHAR_TO_TCHAR (node->repoID ()), this);

          for (long i = 0; i < node->n_supports (); ++i)
            {
              XStr xid (this->add_generalization (ACE_TEXT_CHAR_TO_TCHAR (node->repoID ()),
                                                  ACE_TEXT_CHAR_TO_TCHAR (node->supports ()[i]->repoID ())));

            }

          if (node->base_component () != 0)
            {
              XStr xid (this->add_generalization (ACE_TEXT_CHAR_TO_TCHAR (node->repoID ()),
                                                  ACE_TEXT_CHAR_TO_TCHAR (node->base_component ()->repoID ())));
            }

          // Component ports
          this->gen_component_ports (node->provides (),
                                     ACE_TEXT_CHAR_TO_TCHAR (node->repoID ()),
                                     LITERALS[ST_PROVIDES],
                                     ACE_TEXT_CHAR_TO_TCHAR (node->file_name ().c_str ()));

          this->gen_component_ports (node->uses (),
                                     ACE_TEXT_CHAR_TO_TCHAR (node->repoID ()),
                                     LITERALS[ST_USES],
                                     ACE_TEXT_CHAR_TO_TCHAR (node->file_name ().c_str ()));

          this->gen_component_ports (node->emits (),
                                     ACE_TEXT_CHAR_TO_TCHAR (node->repoID ()),
                                     LITERALS[ST_EMITS],
                                     ACE_TEXT_CHAR_TO_TCHAR (node->file_name ().c_str ()));

          this->gen_component_ports (node->publishes (),
                                     ACE_TEXT_CHAR_TO_TCHAR (node->repoID ()),
                                     LITERALS[ST_PUBLISH],
                                     ACE_TEXT_CHAR_TO_TCHAR (node->file_name ().c_str ()));

          this->gen_component_ports (node->consumes (),
                                     ACE_TEXT_CHAR_TO_TCHAR (node->repoID ()),
                                     LITERALS[ST_CONSUMES],
                                     ACE_TEXT_CHAR_TO_TCHAR (node->file_name ().c_str ()));


          ES_Guard oe_guard (LITERALS[OWNEDELEMENT_TAG], this);

          // attributes in scope.
          this->cached_type_ = node;
          this->visit_scope (node);
          this->cached_type_ = 0;
        }
      catch (Error &err)
        {
          err.node (node);
          throw;
        }

      return 0;
    }

    int
    idl3_to_xmi_visitor::visit_component_fwd (AST_ComponentFwd *node)
    {
      XMI_TRACE ("component_fwd");

      try
        {
          this->create_and_store_xmi_id (node);
        }
      catch (Error &err)
        {
          err.node (node);
          throw;
        }

      return 0;
    }

    int
    idl3_to_xmi_visitor::visit_eventtype (AST_EventType *node)
    {
      XMI_TRACE ("eventtype");

      if (this->skip_imported_ && node->imported ())
        {
          this->visit_scope (node);
          return 0;
        }

      try
        {
          ES_Guard class_guard (LITERALS[CLASS_TAG], this);

          this->visit_valuetype_impl (node, LITERALS[ST_EVENT]);
        }
      catch (Error &err)
        {
          err.node (node);
          throw;
        }

      return 0;
    }

    int
    idl3_to_xmi_visitor::visit_eventtype_fwd (AST_EventTypeFwd *node)
    {
      XMI_TRACE ("eventtype_fwd");

      this->visit_valuetype_fwd (node);

      return 0;
    }

    int
    idl3_to_xmi_visitor::visit_home (AST_Home *node)
    {
      XMI_TRACE ("home");


      if (this->skip_imported_ && node->imported ())
        {
          this->visit_scope (node);
          return 0;
        }

      try
        {
          ES_Guard es_guard (LITERALS[CLASS_TAG], this);

          this->gen_common_elements (node, LITERALS[ST_HOME]);

          NS_Guard ns_guard (ACE_TEXT_CHAR_TO_TCHAR (node->repoID ()), this);

          for (long i = 0; i < node->n_supports (); ++i)
            {
              XStr xid (this->add_generalization (ACE_TEXT_CHAR_TO_TCHAR (node->repoID ()),
                                                  ACE_TEXT_CHAR_TO_TCHAR (node->supports ()[i]->repoID ())));

            }

          if (node->base_home () != 0)
            {
              XStr xid (this->add_generalization (ACE_TEXT_CHAR_TO_TCHAR (node->repoID ()),
                                                  ACE_TEXT_CHAR_TO_TCHAR (node->base_home ()->repoID ())));
            }

          if (node->factories ().size () != 0)
            {
              ES_Guard noe_guard (LITERALS[OWNEDELEMENT_TAG], this);
              for (size_t i = 0; i < node->factories ().size (); ++i)
                {
                  AST_Operation **op = 0;
                  node->factories ().get (op, i);
                  this->visit_operation_impl (*op, LITERALS[ST_HFACT]);
                }
            }

          if (node->finders () .size () != 0)
            throw Error ("home finders not supported", node);

          this->add_managed_component (ACE_TEXT_CHAR_TO_TCHAR (node->repoID ()),
                                       ACE_TEXT_CHAR_TO_TCHAR (node->managed_component ()->repoID ()));

          ES_Guard noe_guard (LITERALS[OWNEDELEMENT_TAG], this);

          this->cached_type_ = node;
          // attributes in scope.
          this->visit_scope (node);
          this->cached_type_ = 0;
        }
      catch (Error &err)
        {
          err.node (node);
          throw;
        }

      return 0;
    }

    int
    idl3_to_xmi_visitor::visit_factory (AST_Factory *node)
    {
      XMI_TRACE ("factory");
      if (this->skip_imported_ && node->imported ())
        return 0;
      throw Error ("Factories not supported", node);

      return 0;
    }

    int
    idl3_to_xmi_visitor::visit_structure (AST_Structure *node)
    {
      XMI_TRACE ("structure");


      if (this->skip_imported_ && node->imported ())
        {
          this->visit_scope (node);
          return 0;
        }

      try
        {
          ES_Guard class_guard (LITERALS[CLASS_TAG], this);

          this->visit_struct_impl (node, LITERALS[ST_STRUCT]);
        }
      catch (Error &err)
        {
          err.node (node);
          throw;
        }

      return 0;
    }

    void
    idl3_to_xmi_visitor::visit_struct_impl (AST_Structure *node, const ACE_TCHAR *stereotype)
    {
      try
        {
          this->gen_common_elements (node, stereotype);
          ES_Guard ns_oe_guard (LITERALS[OWNEDELEMENT_TAG], this);
          NS_Guard ns_guard (ACE_TEXT_CHAR_TO_TCHAR (node->repoID ()), this);
          this->visit_scope (node);
        }
      catch (Error &err)
        {
          err.node (node);
          throw;
        }
    }

    int
    idl3_to_xmi_visitor::visit_structure_fwd (AST_StructureFwd *node)
    {
      XMI_TRACE ("structure_fwd");

      try
        {
          this->create_and_store_xmi_id (node);
        }
      catch (Error &err)
        {
          err.node (node);
          throw;
        }

      return 0;
    }

    int
    idl3_to_xmi_visitor::visit_exception (AST_Exception *node)
    {
      XMI_TRACE ("exception");

      if (this->skip_imported_ && node->imported ())
        {
          this->visit_scope (node);
          return 0;
        }

      try
        {
          ES_Guard class_guard (LITERALS[EXCEPTION_TAG], this);

          this->visit_struct_impl (node, LITERALS[ST_EX]);
        }
      catch (Error &err)
        {
          err.node (node);
          throw;
        }

      return 0;
    }

    int
    idl3_to_xmi_visitor::visit_expression (AST_Expression *)
    {
      XMI_TRACE ("expression");

      // we don't particularly care about expressions.

      return 0;
    }

    int
    idl3_to_xmi_visitor::visit_enum (AST_Enum *node)
    {
      XMI_TRACE ("enum");

      if (this->skip_imported_ && node->imported ())
        {
          this->visit_scope (node);
          return 0;
        }

      try
        {
          ES_Guard es_guard (LITERALS[CLASS_TAG], this);
          this->gen_common_elements (node, LITERALS[ST_ENUM]);

          NS_Guard ns_guard (ACE_TEXT_CHAR_TO_TCHAR (node->repoID ()), this);
          ES_Guard oe_guard (LITERALS[OWNEDELEMENT_TAG], this);

          this->visit_scope (node);
        }
      catch (Error &err)
        {
          err.node (node);
          throw;
        }

      return 0;
    }

    void
    idl3_to_xmi_visitor::visit_operation_impl (AST_Operation *op, const ACE_TCHAR *stereotype)
    {
      if (this->skip_imported_ && op->imported ())
        {
          this->visit_scope (op);
          return;
        }

      // This is a mess because the operation has so many special cases....
      try
        {
          ES_Guard op_guard (LITERALS[OP_TAG], this);

          // name and visiblity
          this->set_attribute (LITERALS[NAME],
                               ACE_TEXT_CHAR_TO_TCHAR (op->original_local_name ()->get_string ()));

          this->set_attribute (LITERALS[VISIBIL], LITERALS[PUBLIC]);

          this->set_containing_element (LITERALS[OWNER]);

          // If we have a stereotype, set it
          if (stereotype != 0)
              this->find_and_set_xid_as_attr (LITERALS[STEREO_ATTR],
                                              stereotype);

          // XMI ID
          this->create_and_store_xmi_id (op);

          // while not strictly a namespace, the repo id goes on the
          // namespace stack as the owner of contained elements.
          NS_Guard ns_guard (ACE_TEXT_CHAR_TO_TCHAR (op->repoID ()), this);

          { // we need to generate the tagged value in a custom way here
            ES_Guard me_guard (LITERALS[ME_TV_TAG], this);

            {
              ES_Guard tv_guard (LITERALS[TV_TAG], this);
              this->set_attribute (LITERALS[TAG], LITERALS[TYPEID]);
              this->set_attribute (LITERALS[VALUE],
                                   ACE_TEXT_CHAR_TO_TCHAR (op->repoID ()));
            }

            UTL_ExceptList *exceptions = op->exceptions ();
            if (exceptions != 0 && exceptions->length () > 0)
              {
                for (UTL_ExceptlistActiveIterator ei (exceptions);
                     !ei.is_done ();
                     ei.next ())
                  {
                    ES_Guard tv_guard (LITERALS[TV_TAG], this);
                    this->set_attribute (LITERALS[TAG], LITERALS[RAISES]);
                    this->set_attribute (LITERALS[VALUE],
                                         ACE_TEXT_CHAR_TO_TCHAR (ei.item ()->full_name ()));
                  }
              }
          }

          ES_Guard bfp_guard (LITERALS[BFP_TAG], this);

          // return value
          {
            ES_Guard param_guard (LITERALS[PARAM_TAG], this);

            // I don't think anything else can refer to the parameter by
            // xid, so I won't store it.
            XStr xid = this->gen_xmi_id (op);

            this->set_attribute (LITERALS[XMI_ID], xid);

            this->set_attribute (LITERALS[KIND], LITERALS[RETURN]);

            this->set_containing_element (LITERALS[BFEATURE]);

            xid = this->lookup_type_xid (op->return_type ());
            this->set_attribute (LITERALS[TYPE],
                                 xid);
          }

          if (op->argument_count () != 0)
            {
              // Visit arguments
              for (UTL_ScopeActiveIterator si (op, UTL_Scope::IK_decls);
                   !si.is_done ();)
                {
                  si.item ()->ast_accept (this);
                  si.next ();
                }
            }
        }
      catch (Error &err)
        {
          err.node (op);
          throw;
        }
    }

    int
    idl3_to_xmi_visitor::visit_operation (AST_Operation *op)
    {
      XMI_TRACE ("operation");

      this->visit_operation_impl (op, 0);

      return 0;
    }

    int
    idl3_to_xmi_visitor::visit_field (AST_Field *node)
    {
      XMI_TRACE ("field");

      if (this->skip_imported_ && node->imported ())
        {
          return 0;
        }

      try
        {
          ES_Guard attr_guard (LITERALS[ATTR_TAG], this);

          // I don't think anything else can refer to the parameter by
          // xid, so I won't store it.
          XStr xid = this->gen_xmi_id (node);
          this->set_attribute (LITERALS[XMI_ID], xid);

          this->set_attribute (LITERALS[NAME],
                               ACE_TEXT_CHAR_TO_TCHAR (node->original_local_name ()->get_string ()));

          switch (node->visibility ())
            {
            case AST_Field::vis_NA:
            case AST_Field::vis_PUBLIC:
              this->set_attribute (LITERALS[VISIBIL], LITERALS[PUBLIC]);
              break;
            case AST_Field::vis_PRIVATE:
              this->set_attribute (LITERALS[VISIBIL], LITERALS[_PRIVATE]);
              break;
            default:
              throw Error ("unknown visibility type detected.", node);
            }

          // I think this is fixed.
          this->set_attribute (LITERALS[MULT], LITERALS[MULT_OTO]);

          this->set_containing_element (LITERALS[OWNER]);

          xid = this->lookup_type_xid (node->field_type ());
          this->set_attribute (LITERALS[TYPE],
                               xid);


        }
      catch (Error &err)
        {
          err.node (node);
          throw;
        }

      return 0;
    }

    int
    idl3_to_xmi_visitor::visit_argument (AST_Argument *node)
    {
      XMI_TRACE ("argument");

      if (this->skip_imported_ && node->imported ())
        {
          return 0;
        }

      try
        {
          ES_Guard param_guard (LITERALS[PARAM_TAG], this);

          this->set_attribute (LITERALS[NAME],
                               ACE_TEXT_CHAR_TO_TCHAR (node->original_local_name ()->get_string ()));

          // I don't think anything else can refer to the parameter by
          // xid, so I won't store it.
          XStr xid = this->gen_xmi_id (node);
          this->set_attribute (LITERALS[XMI_ID], xid);

          // kind
          switch (node->direction ())
            {
            case AST_Argument::dir_IN:
              this->set_attribute (LITERALS[KIND], LITERALS[_IN]);
              break;

            case AST_Argument::dir_INOUT:
              this->set_attribute (LITERALS[KIND], LITERALS[INOUT]);
              break;

            case AST_Argument::dir_OUT:
              this->set_attribute (LITERALS[KIND], LITERALS[_OUT]);
              break;

            default:
              throw Error ("Unknown argument direction", node);
            }

          this->set_containing_element (LITERALS[BFEATURE]);

          xid = this->lookup_type_xid (node->field_type ());
          this->set_attribute (LITERALS[TYPE],
                               xid);

        }
      catch (Error &err)
        {
          err.node (node);
          throw;
        }

      return 0;
    }

    int
    idl3_to_xmi_visitor::visit_attribute (AST_Attribute *node)
    {
      XMI_TRACE ("attribute");

      if (this->skip_imported_ && node->imported ())
        {
          return 0;
        }

      if (this->cached_type_ == 0)
        {
          ACE_DEBUG ((LM_DEBUG, "Zounds! %d\n",
                      this->cached_type_));
          throw Error ("Internal error - attribute expected "
                       "to have a cached type, but didn't");
        }

      try
        {
          // ***
          NS_Guard global_ns (ACE_TEXT ("::"), this);
          ES_Guard assoc_group (this->associations_, this);
          ES_Guard attr_guard (LITERALS[ASSOC_TAG], this);

          this->set_attribute (LITERALS[VISIBIL], LITERALS[PUBLIC]);
          if (node->readonly ())
            this->find_and_set_xid_as_attr (LITERALS[STEREO_ATTR],
                                            LITERALS[ST_RO]);
          this->set_containing_element (LITERALS[OWNER]);
          // I don't think anything else can refer to the attribute by
          // xid, so I won't store it.
          XStr xid = this->gen_xmi_id (node);
          this->set_attribute (LITERALS[XMI_ID], xid);

          ES_Guard assoc_conn (LITERALS[ASSOC_CONN_TAG], this);

          { // Containing type
            ES_Guard assoc_end (LITERALS[ASSOC_END_TAG], this);
            this->set_attribute (LITERALS[MULT], ACE_TEXT ("1"));
            this->find_and_set_xid_as_attr (LITERALS[TYPE],
                                            ACE_TEXT_CHAR_TO_TCHAR (this->cached_type_->repoID ()));
          }

          {
            ES_Guard assoc_end (LITERALS[ASSOC_END_TAG], this);
            this->set_attribute (LITERALS[NAME],
                                 ACE_TEXT_CHAR_TO_TCHAR (node->original_local_name ()->get_string ()));
            this->set_attribute (LITERALS[MULT], ACE_TEXT ("1"));
            xid = this->lookup_type_xid (node->field_type ());
            this->set_attribute (LITERALS[TYPE], xid);
          }
        }
      catch (Error &err)
        {
          err.node (node);
          throw;
        }

      return 0;
    }

    int
    idl3_to_xmi_visitor::visit_union (AST_Union *node)
    {
      XMI_TRACE ("union");

      if (this->skip_imported_ && node->imported ())
        {
          return 0;
        }

      try
        {
          ES_Guard class_guard (LITERALS[CLASS_TAG], this);

          this->gen_common_elements (node, LITERALS[ST_UNION]);

          NS_Guard ns_guard (ACE_TEXT_CHAR_TO_TCHAR (node->repoID ()), this);
          ES_Guard oe_guard (LITERALS[OWNEDELEMENT_TAG], this);

          // Set discriminator type
          this->union_disc_ = node->disc_type ();
          this->visit_scope (node);
        }
      catch (Error &err)
        {
          this->union_disc_ = 0;
          err.node (node);
          throw;
        }

      return 0;
    }

    int
    idl3_to_xmi_visitor::visit_union_fwd (AST_UnionFwd *node)
    {
      XMI_TRACE ("union_fwd");


      try
        {
          this->create_and_store_xmi_id (node);
        }
      catch (Error &err)
        {
          err.node (node);
          throw;
        }

      return 0;
    }

    int
    idl3_to_xmi_visitor::visit_union_branch (AST_UnionBranch *node)
    {
      XMI_TRACE ("union_branch");

      try
        {
          ES_Guard attr_guard (LITERALS[ATTR_TAG], this);

          // I don't think anything else can refer to the parameter by
          // xid, so I won't store it.
          XStr xid = this->gen_xmi_id (node);
          this->set_attribute (LITERALS[XMI_ID], xid);

          // name
          this->set_attribute (LITERALS[NAME],
                               ACE_TEXT_CHAR_TO_TCHAR (node->original_local_name ()->get_string ()));

          // visiblity
          this->set_attribute (LITERALS[VISIBIL], LITERALS[PUBLIC]);

          this->set_containing_element (LITERALS[OWNER]);

          // I think this is fixed.
          this->set_attribute (LITERALS[MULT], LITERALS[MULT_ZTO]);

          xid = this->lookup_type_xid (node->field_type ());
          this->set_attribute (LITERALS[TYPE],
                               xid);

          {
            ES_Guard me_tv_guard (LITERALS[ME_TV_TAG], this);

            {
              ES_Guard tv_guard (LITERALS[TV_TAG], this);
              this->set_attribute (LITERALS[TAG], LITERALS[IDLORDER]);

              // convert the order value to a string
              // need a buffer, with a little safety padding
              ACE_Auto_Basic_Array_Ptr<ACE_TCHAR> buffer
                (this->number_to_string (this->order_));

              this->set_attribute (LITERALS[VALUE], buffer.get ());
            }

            for (unsigned long i = 0; i < node->label_list_length (); ++i)
              this->visit_union_label (node->label (i));

          }
        }
      catch (Error  &err)
        {
          err.node (node);
          throw;
        }

      return 0;
    }

    ACE_TCHAR *
    idl3_to_xmi_visitor::union_label_value (AST_Expression *exp)
    {
      ACE_Auto_Basic_Array_Ptr<ACE_TCHAR> buffer;

      // @@TODO: Yuck, there has got to be a better way....
      AST_Expression::AST_ExprValue *ev = exp->ev ();
      const size_t bool_length = 6;

      if (exp->ec () != AST_Expression::EC_symbol ||
          this->union_disc_->node_type () == AST_Decl::NT_typedef ||
          this->union_disc_->node_type () == AST_Decl::NT_pre_defined)
        {
          /*          AST_Expression::ExprType type;

          if (this->union_disc_->node_type () == AST_Decl::NT_typedef)
            {
              AST_Typedef *td (AST_Typedef::narrow_from_decl (this->union_disc_));
              type = td->base_type ()->node_type ();
            }
            else type = ev->et;*/

          // We have a non-enum discriminator
          // Allocate memory
          switch (ev->et)
            {
            case AST_Expression::EV_bool:
              ACE_DEBUG ((LM_DEBUG, "bool disc\n"));
              buffer.reset (new ACE_TCHAR [bool_length]); // length of 'false' + 1
              break;
            case AST_Expression::EV_char:
              ACE_DEBUG ((LM_DEBUG, "char disc\n"));
              buffer.reset (new ACE_TCHAR[1]);
              break;
            default:
              ACE_DEBUG ((LM_DEBUG, "numberic?disc\n"));
              // don't need to allocate room for numerics, handled by
              // number_to_string
              break;
            }

          // Decode the type
          switch (ev->et)
            {
            case AST_Expression::EV_long:
              buffer.reset (this->number_to_string(ev->u.lval));
              break;
            case AST_Expression::EV_ulong:
              buffer.reset (this->number_to_string(ev->u.ulval));
              break;
            case AST_Expression::EV_short:
              buffer.reset (this->number_to_string(ev->u.sval));
              break;
            case AST_Expression::EV_ushort:
              buffer.reset (this->number_to_string(ev->u.usval));
              break;

            case AST_Expression::EV_bool:
              buffer.reset (this->number_to_string (ev->u.bval));
              break;
            case AST_Expression::EV_char:
              *buffer.get () = ev->u.cval;
              break;
            default:
              throw Error ("Unknown union union label type");
            }
        }
      else
        {
          AST_Enum *desc (0);
          if ((desc = AST_Enum::narrow_from_decl (this->union_disc_)) == 0)
            throw Error ("Descriminator type is not an enum");

          AST_Decl *ev_decl =
            desc->lookup_by_name (exp->n (), 1);

          if (ev_decl == 0)
            throw Error ("Couldn't look up enum name");

          AST_EnumVal *ev = AST_EnumVal::narrow_from_decl (ev_decl);

          if (ev == 0)
            throw Error ("Couldn't look up enum name");

          buffer.reset (ACE::strnew (ACE_TEXT_CHAR_TO_TCHAR (ev->full_name ())));
          //const char *name = desc->lookup_by_value (exp)->full_name ();
          //          const char *name = exp->n ();
          //buffer.reset (copy_scoped_name (exp->n ()));
        }

      if (buffer.get () == 0)
        throw Error ("Unable to parse union label");

      return buffer.release ();
    }

    int
    idl3_to_xmi_visitor::visit_union_label (AST_UnionLabel *node)
    {
      XMI_TRACE ("union_label");

      ES_Guard tv_guard (LITERALS[TV_TAG], this);
      this->set_attribute (LITERALS[TAG], LITERALS[CASE]);

      if (node->label_kind () == AST_UnionLabel::UL_default)
        this->set_attribute (LITERALS[VALUE], LITERALS[DEFAULT_UNION]);
      else
        {
          ACE_Auto_Basic_Array_Ptr<ACE_TCHAR>
            buffer (this->union_label_value (node->label_val ()));

          this->set_attribute (LITERALS[VALUE], buffer.get ());
        }

      return 0;
    }

    int
      idl3_to_xmi_visitor::visit_constant (AST_Constant *node)
    {
      XMI_TRACE ("constant");

      if (this->skip_imported_ && node->imported ())
        {
          return 0;
        }

      try
        {
          ES_Guard attr_guard (LITERALS[ATTR_TAG], this);

          this->set_attribute (LITERALS[OWNER_SCOPE], LITERALS[CLASSIFIER]);
          this->set_attribute (LITERALS[CHANGEABLE], LITERALS[FROZEN]);
          this->gen_common_elements (node, LITERALS[ST_CONST]);

          // Constant type
          const ACE_TCHAR *exprtype (0);
          AST_Expression::AST_ExprValue *val
            (node->constant_value ()->ev ());

          ACE_Auto_Basic_Array_Ptr<ACE_TCHAR> str_value;

          switch (node->et ())
            {
            case AST_Expression::EV_short:
              exprtype = LITERALS[ST_SHORT];
              str_value.reset (this->number_to_string (val->u.sval));
              break;
            case AST_Expression::EV_ushort:
              exprtype = LITERALS[ST_US];
              str_value.reset (this->number_to_string (val->u.usval));
              break;
            case AST_Expression::EV_long:
              exprtype = LITERALS[ST_LONG];
              str_value.reset (this->number_to_string (val->u.lval));
              break;
            case AST_Expression::EV_ulong:
              exprtype = LITERALS[ST_UL];
              str_value.reset (this->number_to_string (val->u.ulval));
              break;
            case AST_Expression::EV_longlong:
              exprtype = LITERALS[ST_LL];
              str_value.reset (this->number_to_string (val->u.llval));
            case AST_Expression::EV_ulonglong:
              exprtype = LITERALS[ST_ULL];
              str_value.reset (this->number_to_string (val->u.ullval));
              break;
            case AST_Expression::EV_float:
              exprtype = LITERALS[ST_FLOAT];
              str_value.reset (this->number_to_string (val->u.fval));
              break;
            case AST_Expression::EV_double:
              exprtype = LITERALS[ST_DOUBLE];
              str_value.reset (this->number_to_string (val->u.dval));
              break;
            case AST_Expression::EV_longdouble:
              exprtype = LITERALS[ST_LD];
              str_value.reset (this->number_to_string (val->u.dval));
              break;
            case AST_Expression::EV_char:
              exprtype = LITERALS[ST_CHAR];
              str_value.reset (this->number_to_string (val->u.cval));
              break;
            case AST_Expression::EV_wchar:
              exprtype = LITERALS[ST_WCHAR];
              str_value.reset (this->number_to_string (val->u.wcval));
              break;
            case AST_Expression::EV_octet:
              exprtype = LITERALS[ST_OCTET];
              str_value.reset (this->number_to_string (val->u.dval));
              break;
            case AST_Expression::EV_bool:
              exprtype = LITERALS[ST_BOOL];
              str_value.reset (this->number_to_string (val->u.bval));
              break;
            case AST_Expression::EV_string:
              exprtype = LITERALS[ST_STR];
              str_value.reset (ACE::strnew (
                ACE_TEXT_CHAR_TO_TCHAR (val->u.strval->get_string ())));
              break;
            case AST_Expression::EV_wstring:
              exprtype = LITERALS[ST_WSTR];
              str_value.reset (ACE::strnew (
                ACE_TEXT_CHAR_TO_TCHAR (val->u.wstrval)));
              break;
            case AST_Expression::EV_enum:
              {
                UTL_Scope *s = node->defined_in ();
                AST_Decl *d =
                  s->lookup_by_name (node->constant_value ()->n (), 1);
                exprtype = ACE_TEXT_CHAR_TO_TCHAR (
                  ScopeAsDecl (d->defined_in ())->repoID ());
                str_value.reset (ACE::strnew (
                  ACE_TEXT_CHAR_TO_TCHAR (d->full_name ())));
              }
              break;

            default:
              throw Error ("Unsupported constant type", node);
            }

          // type attribute
          XStr xid (this->lookup_xid (exprtype));
          this->set_attribute (LITERALS[TYPE],
                               xid);

          {
            ES_Guard me_tv (LITERALS[ME_TV_TAG], this);
            {
              ES_Guard tv (LITERALS[TV_TAG], this);
              this->set_attribute (LITERALS[TAG], LITERALS[INIT_VAL]);
              this->set_attribute (LITERALS[VALUE], str_value. get ());
            }
          }
        }
      catch (Error &err)
        {
          err.node (node);
          throw;
        }

      return 0;
    }

    int
    idl3_to_xmi_visitor::visit_enum_val (AST_EnumVal *node)
    {

      if (this->skip_imported_ && node->imported ())
        {
          return 0;
        }

      try
        {
          XMI_TRACE ("enum val");
          ES_Guard att_guard (LITERALS[ATTR_TAG], this);

          this->create_and_store_xmi_id (ACE_TEXT_CHAR_TO_TCHAR (node->repoID ()));
          this->set_attribute (LITERALS[NAME],
                               ACE_TEXT_CHAR_TO_TCHAR (node->original_local_name ()->get_string ()));

          // I think this is fixed.
          this->set_attribute (LITERALS[MULT], LITERALS[MULT_OTO]);
          this->set_containing_element (LITERALS[OWNER]);

          ES_Guard me_guard (LITERALS[ME_TV_TAG], this);
          ES_Guard tv_guard (LITERALS[TV_TAG], this);

          this->set_attribute (LITERALS[TAG], LITERALS[IDLORDER]);

          // convert the enum value to a string
          // need a buffer, with a little safety padding

          ACE_Auto_Basic_Array_Ptr<ACE_TCHAR> buffer
            (this->number_to_string (node->constant_value ()->ev ()->u.eval));

          this->set_attribute (LITERALS[VALUE], buffer.get ());
        }
      catch (Error  &err)
        {
          err.node (node);
          throw;
        }

      return 0;
    }

    int
    idl3_to_xmi_visitor::visit_array (AST_Array *node)
    {
      XMI_TRACE ("array val");
      throw Error ("Arrays not supported", node);
      return 0;
    }

    int
    idl3_to_xmi_visitor::visit_sequence (AST_Sequence *node)
    {
      XMI_TRACE ("sequence val");
      if (this->skip_imported_ && node->imported ())
        {
          return 0;
        }

      try
        {
          ES_Guard class_guard (LITERALS[CLASS_TAG], this);

          this->gen_common_elements (node,
                                     LITERALS[ST_SEQ]);

          // Generate the bounds and type  on the sequence, which is an
          // association in the association tree.
          NS_Guard global_ns (ACE_TEXT ("::"), this);
          ES_Guard assoc_group (this->associations_, this);
          ES_Guard assoc_g (LITERALS[ASSOC_TAG], this);

          // I don't think anything else can refer to this association
          // xid, so I won't store it.
          XStr xid = this->gen_xmi_id (node);
          this->set_attribute (LITERALS[XMI_ID], xid);
          this->set_attribute (LITERALS[VISIBIL], LITERALS[PUBLIC]);
          this->set_containing_element (LITERALS[NS]);

          ES_Guard assoc_conn (LITERALS[ASSOC_CONN_TAG], this);


          { // Bounds
            ES_Guard assocend (LITERALS[ASSOC_END_TAG], this);

            this->set_attribute (LITERALS[MULT], LITERALS[MULT_ZTO]);

            xid = this->lookup_xid (ACE_TEXT_CHAR_TO_TCHAR (node->repoID ()));
            this->set_attribute (LITERALS[TYPE], xid);

            ES_Guard assoc_qual (LITERALS[ASSOC_END_QUAL_TAG], this);
            ES_Guard attr (LITERALS[ATTR_TAG], this);

            xid = this->gen_xmi_id (node);
            this->set_attribute (LITERALS[XMI_ID], xid);
            this->set_attribute (LITERALS[NAME], LITERALS[INDEX]);
            this->set_attribute (LITERALS[TYPE], LITERALS[ST_LONG]);

            ES_Guard me_c (LITERALS[ME_CONS_TAG], this);
            ES_Guard cons (LITERALS[CONSTRAINT], this);
            ES_Guard me_tv (LITERALS[ME_TV_TAG], this);
            ES_Guard tv (LITERALS[TV_TAG], this);

            this->set_attribute (LITERALS[TAG], LITERALS[CONST_UPPER]);

            // @@TODO: need to extract bound from node.
            if (node->unbounded ())
              this->set_attribute (LITERALS[CONST_UPPER], LITERALS[UNBOUNDED_SEQ]);
            else
              { // bounded sequence
                // convert the array bound to a string
                // need a buffer, with a little safety padding
                ACE_Auto_Basic_Array_Ptr<ACE_TCHAR> buffer
                  (this->number_to_string (node->max_size ()->ev ()->u.ulval));

                this->set_attribute (LITERALS[CONST_UPPER], buffer.get ());
              }
          }

          // Sequence type
          ES_Guard assoc_end (LITERALS[ASSOC_END_TAG], this);
          this->set_attribute (LITERALS[MULT], LITERALS[MULT_OTO]);

          XStr seq_type = this->lookup_type_xid (node->base_type ());
          this->set_attribute (LITERALS[TYPE],
                               seq_type);
        }
      catch (Error &err)
        {
          err.node (node);
          throw;
        }

      return 0;
    }

    int
    idl3_to_xmi_visitor::visit_string (AST_String *)
    {
      XMI_TRACE ("string");
      return 0;
    }

    int
    idl3_to_xmi_visitor::visit_typedef (AST_Typedef *node)
    {
      XMI_TRACE ("typedef");

      if (this->skip_imported_ && node->imported ())
          return 0;

      try
        {
          ES_Guard class_guard (LITERALS[CLASS_TAG], this);

          AST_Decl *base (node->base_type ());
          if (base->node_type () == AST_Decl::NT_sequence ||
              base->node_type () == AST_Decl::NT_array)
            {
              // Anonymous Sequences and arrays appear to be a special case
              const ACE_TCHAR *stereo (0);
              switch (base->node_type ())
                {
                case AST_Decl::NT_sequence:
                  stereo = LITERALS[ST_SEQ];
                  break;
                case AST_Decl::NT_array:
                  stereo = LITERALS[ST_ARR];
                  break;
                default:
                  throw Error ("Unsupported anonymous type inside typedef.",
                               node);
                }
              this->gen_common_elements (node, stereo);

              // Hopefully we have hit something like:
              // typedef char AirportName[4];
              AST_Array* the_array = 0;
              AST_Sequence* the_sequence = 0;
              if (base->node_type () == AST_Decl::NT_array)
                {
                  the_array = AST_Array::narrow_from_decl (base);
                }
              else
                {
                  the_sequence = AST_Sequence::narrow_from_decl (base);
                }

              // <UML:Namespace.ownedElement>
              NS_Guard global_ns (ACE_TEXT ("::"), this);

              // <UML:Association xmi.id="xmi.1210085542354" visibility="public" namespace="xmi.1210085542350">
              ES_Guard assoc_group (this->associations_, this);
              ES_Guard assoc_g (LITERALS[ASSOC_TAG], this);

              XStr xid = this->gen_xmi_id (node);
              this->set_attribute (LITERALS[XMI_ID], xid);
              this->set_attribute (LITERALS[VISIBIL], LITERALS[PUBLIC]);
              this->set_containing_element (LITERALS[NS]);

              // <UML:Association.connection>
              ES_Guard assoc_conn (LITERALS[ASSOC_CONN_TAG], this);

              {
                // <UML:AssociationEnd multiplicity="0..1" type="xmi.1210085542353">
                ES_Guard assocend (LITERALS[ASSOC_END_TAG], this);

                this->set_attribute (LITERALS[MULT], LITERALS[MULT_ZTO]);

                xid =
                  this->lookup_xid (ACE_TEXT_CHAR_TO_TCHAR (node->repoID ()));
                this->set_attribute (LITERALS[TYPE], xid);

                // <UML:AssociationEnd.qualifier>
                ES_Guard assoc_qual (LITERALS[ASSOC_END_QUAL_TAG], this);

                for (unsigned long i = 0;
                     the_array != 0  && i < the_array->n_dims ();
                     ++i)
                  {
                    // <UML:Attribute xmi.id="xmi.1210085542355" name="index0" type='xmi.1210085542329'> - N.B. that type is long.
                    ES_Guard attr (LITERALS[ATTR_TAG], this);

                    xid = this->gen_xmi_id (node);
                    this->set_attribute (LITERALS[XMI_ID], xid);

                    std::stringstream str;
                    str << LITERALS[INDEX];
                    str << i;

                    this->set_attribute (
                      LITERALS[NAME],
                      ACE_TEXT_CHAR_TO_TCHAR (str.str ().c_str ()));
                    xid = this->lookup_xid (LITERALS[ST_LONG]);
                    this->set_attribute (LITERALS[TYPE], xid);

                    // <UML:ModelElement.constraint>
                    ES_Guard me_c (LITERALS[ME_CONS_TAG], this);

                    // <UML:Constraint xmi.id="xmi.1210085542356">
                    ES_Guard cons (LITERALS[CONSTRAINT], this);

                    xid = this->gen_xmi_id (node);
                    this->set_attribute (LITERALS[XMI_ID], xid);

                    // <UML:ModelElement.taggedValue>
                    ES_Guard me_tv (LITERALS[ME_TV_TAG], this);

                    // <UML:TaggedValue tag="constraintUpperValue" value="4"/>
                    ES_Guard tv (LITERALS[TV_TAG], this);

                    this->set_attribute (LITERALS[TAG], LITERALS[CONST_UPPER]);

                    ACE_Auto_Basic_Array_Ptr<ACE_TCHAR> buffer (
                      this->number_to_string (
                        the_array->dims ()[i]->ev ()->u.ulval));

                    this->set_attribute (LITERALS[VALUE], buffer.get ());
                  }

                if (the_sequence)
                  {
                    ES_Guard attr (LITERALS[ATTR_TAG], this);

                    xid = this->gen_xmi_id (node);
                    this->set_attribute (LITERALS[XMI_ID], xid);
                    this->set_attribute (LITERALS[NAME], LITERALS[INDEX]);
                    xid = this->lookup_xid (LITERALS[ST_LONG]);
                    this->set_attribute (LITERALS[TYPE], xid);


                    ES_Guard me_c (LITERALS[ME_CONS_TAG], this);
                    ES_Guard cons (LITERALS[CONSTRAINT], this);
                    ES_Guard me_tv (LITERALS[ME_TV_TAG], this);
                    ES_Guard tv (LITERALS[TV_TAG], this);

                    this->set_attribute (LITERALS[TAG], LITERALS[CONST_UPPER]);

                    // @@TODO: need to extract bound from node.
                    if (the_sequence->unbounded ())
                      {
                        this->set_attribute (LITERALS[CONST_UPPER],
                                             LITERALS[UNBOUNDED_SEQ]);
                      }
                    else
                      { // bounded sequence
                        // convert the array bound to a string
                        // need a buffer, with a little safety padding
                        ACE_Auto_Basic_Array_Ptr<ACE_TCHAR> buffer (
                          this->number_to_string (
                            the_sequence->max_size ()->ev ()->u.ulval));

                        this->set_attribute (LITERALS[CONST_UPPER],
                                             buffer.get ());
                      }
                  }

                  // <UML:AssociationEnd multiplicity="1..1" type="xmi.1210085542346"/> - type is Char for this IDL
                  ES_Guard assoc_end (LITERALS[ASSOC_END_TAG], this);
                  this->set_attribute (LITERALS[MULT], LITERALS[MULT_OTO]);

                  XStr seq_type =
                    this->lookup_type_xid (
                      (the_array ?
                       the_array->base_type () :
                       the_sequence->base_type ()));
                  this->set_attribute (LITERALS[TYPE],
                                       seq_type);
              }
            }
          else
            {
              this->gen_common_elements (node,
                                         LITERALS[ST_TYPEDEF]);

              // Generalization
              XStr xid (this->add_generalization (node,
                                                  node->base_type ()));
            }
        }
      catch (Error &err)
        {
          err.node (node);
          throw;
        }

      return 0;
    }

    int
    idl3_to_xmi_visitor::visit_root (AST_Root *node)
    {
      XMI_TRACE ("root");
      try
        {
          ACE_CString fn;

          if ((fn = be_global->filename ()) == "")
            {
              fn = idl_global->stripped_filename ()->get_string ();
              fn = fn.substr (0, fn.rfind ('.'));
              fn += ".xmi";
            }

          const char *path = be_global->output_dir ();
          ACE_CString target_name;

          if (path != 0)
            {
              target_name = path;
              target_name += "/";
            }

          target_name += fn;

          ACE_DEBUG ((LM_DEBUG, "Opening %C\n",
                      target_name.c_str ()));
          
          CIAO::XML::NoOp_Resolver res_func;
          CIAO::XML::XML_Schema_Resolver<> resolver (res_func);
          CIAO::XML::XML_Error_Handler handler;
          CIAO::XML::XML_Helper<> helper (resolver, handler);

          // Create XML document
          std::auto_ptr<DOMDocumentType> doctype (
            helper.create_doctype (ACE_TEXT("XMI"),
                                   0,
                                   ACE_TEXT_CHAR_TO_TCHAR (be_global->dtd ())));

          if ((this->dom_ = helper.create_dom (ACE_TEXT("XMI"),
                                               ACE_TEXT(""),
                                               doctype.get ())) == 0)
            ACE_ERROR_RETURN ((LM_ERROR, "Unable to create XML document."), -1);

          this->root_ = this->dom_->getDocumentElement ();

          this->stack_.push (this->root_);

          this->set_attribute (LITERALS[VERSION_TAG],
                               LITERALS[VERSION_VALUE]);

          this->set_attribute (LITERALS[NS_TAG],
                               LITERALS[NS_VALUE]);

          DOMElement *tmp = 0;

          // Pregenerate stereotypes to cache xmi.ids.
          DOMElement *stereotypes = this->generate_stereotypes ();

          // Create XMI header
          {
            ES_Guard ht_g (LITERALS[HEADER_TAG], this);
            ES_Guard d_t (LITERALS[DOC_TAG], this);
            ES_Guard e_t (LITERALS[EXPORTER_TAG], this);
            this->add_text (LITERALS[EXPORTER_VALUE]);
          }

          this->stack_.push (this->create_element (LITERALS[CONTENT_TAG]));
          this->stack_.push (this->create_element (LITERALS[MODEL_TAG]));

          // Store the base model xid
          this->set_attribute (LITERALS[XMI_ID], this->base_id_);
          this->id_map_.bind (ACE_TEXT ("::"), this->base_id_);


          // Global namespace
          this->stack_.push (this->create_element (LITERALS[OWNEDELEMENT_TAG]));
          this->namespace_.push (ACE_TEXT ("::"));

          // Create generalizations and associations elements, but don't join
          // them to the tree yet
          this->associations_ = this->dom_->createElement (XStr (LITERALS[OWNEDELEMENT_TAG]));
          this->generalizations_ = this->dom_->createElement (XStr (LITERALS[OWNEDELEMENT_TAG]));

          if (this->visit_scope (node) != 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "idl3_to_idl2_visitor::visit_root - "
                                 "codegen for scope failed\n"),
                                -1);
            }

          this->stack_.pop (tmp);

          this->stack_.top (tmp);

          tmp->appendChild (this->generalizations_);
          tmp->appendChild (this->associations_);
          tmp->appendChild (stereotypes);

          helper.write_DOM (this->dom_,
                            ACE_TEXT_CHAR_TO_TCHAR (target_name.c_str ()));
        }
      catch (const Error &ex)
        {
          if (ex.node_ != 0)
            ACE_ERROR ((LM_ERROR, "%s:%d:error: %s\n",
                        ex.node_->file_name ().c_str (),
                        ex.node_->line (),
                        ex.diag_.c_str ()));
          else
            ACE_ERROR ((LM_ERROR, "::error:%s\n",
                        ex.diag_.c_str ()));

          return -1;
        }
      return 0;
    }

    int
    idl3_to_xmi_visitor::visit_native (AST_Native *)
    {
      XMI_TRACE ("native");
      return 0;
    }

    void
    idl3_to_xmi_visitor::set_attribute (const ACE_TCHAR *name,
                                        const ACE_TCHAR *value)
    {
      DOMElement *ele = 0;
      this->stack_.top (ele);

      ele->setAttribute (XStr (name), XStr (value));
    }


    void
    idl3_to_xmi_visitor::set_attribute (const ACE_TCHAR *name,
                                        const XMLCh *value)
    {
      DOMElement *ele = 0;
      this->stack_.top (ele);

      ele->setAttribute (XStr (name), value);
    }

    void
    idl3_to_xmi_visitor::add_text (const ACE_TCHAR *text)
    {
      DOMElement *ele = 0;
      this->stack_.top (ele);

      DOMText *node = this->dom_->createTextNode (XStr (text));
      ele->appendChild (node);
    }

    DOMElement *
    idl3_to_xmi_visitor::create_element (const ACE_TCHAR *name)
    {
      DOMElement *ele = 0;
      this->stack_.top (ele);

      DOMElement *node = this->dom_->createElement (XStr (name));

      ele->appendChild (node);

      return node;
    }

    DOMElement *
    idl3_to_xmi_visitor::generate_stereotypes (void)
    {
      DOMElement *retval = this->dom_->createElement (XStr (LITERALS[OWNEDELEMENT_TAG]));

      // put it on the stack to subsequent create_element calls refer to it.
      this->stack_.push (retval);

      this->add_stereotype (ST_CONSUMES, ASSOC);
      this->add_stereotype (ST_ANON_SEQ, CLASS);
      this->add_stereotype (ST_PRINC, DATATYPE);
      this->add_stereotype (ST_UNION, CLASS);
      this->add_stereotype (ST_USES, ASSOC);
      this->add_stereotype (ST_SEQ, CLASS);
      this->add_stereotype (ST_ROE, _NULL);
      this->add_stereotype (ST_RO, _NULL);
      this->add_stereotype (ST_OBJ, DATATYPE);
      this->add_stereotype (ST_STR, DATATYPE);
      this->add_stereotype (ST_LD, DATATYPE);
      this->add_stereotype (ST_ANON_ARR, CLASS);
      this->add_stereotype (ST_ALIAS, CLASS);
      this->add_stereotype (ST_NULL, DATATYPE);
      this->add_stereotype (ST_VALUE, CLASS);
      this->add_stereotype (ST_VOID, DATATYPE);
      this->add_stereotype (ST_EX, CLASS);
      this->add_stereotype (ST_OCTET, DATATYPE);
      this->add_stereotype (ST_MANAGES, ASSOC);
      this->add_stereotype (ST_WSTR, DATATYPE);
      this->add_stereotype (ST_FLOAT, DATATYPE);
      this->add_stereotype (ST_ENUM, CLASS);
      this->add_stereotype (ST_VAL_SUP, CLASS);
      this->add_stereotype (ST_MODULE, PACKAGE);
      this->add_stereotype (ST_HFINDER, CLASS);
      this->add_stereotype (ST_DOUBLE, DATATYPE);
      this->add_stereotype (ST_LL, DATATYPE);
      this->add_stereotype (ST_HFACT, CLASS);
      this->add_stereotype (ST_ARR, CLASS);
      this->add_stereotype (ST_COMP, CLASS);
      this->add_stereotype (ST_SHORT, DATATYPE);
      this->add_stereotype (ST_TYPEDEF, CLASS);
      this->add_stereotype (ST_ULL, DATATYPE);
      this->add_stereotype (ST_LONG, DATATYPE);
      this->add_stereotype (ST_TC, DATATYPE);
      this->add_stereotype (ST_HOME, CLASS);
      this->add_stereotype (ST_STRUCT, CLASS);
      this->add_stereotype (ST_FIXED, DATATYPE);
      this->add_stereotype (ST_US, DATATYPE);
      this->add_stereotype (ST_EMITS, ASSOC);
      this->add_stereotype (ST_BOXVALUE, CLASS);
      this->add_stereotype (ST_BOOL, DATATYPE);
      this->add_stereotype (ST_CONST, ATTR);
      this->add_stereotype (ST_PUBLISH, ASSOC);
      this->add_stereotype (ST_CONSTANTS, CLASS);
      this->add_stereotype (ST_PROVIDES, ASSOC);
      this->add_stereotype (ST_WCHAR, DATATYPE);
      this->add_stereotype (ST_UL, DATATYPE);
      this->add_stereotype (ST_VAL_FACT, CLASS);
      this->add_stereotype (ST_EVENT, CLASS);
      this->add_stereotype (ST_CHAR, DATATYPE);
      this->add_stereotype (ST_INTF, CLASS);
      this->add_stereotype (ST_PRI_KEY, CLASS);
      this->add_stereotype (ST_ANY, DATATYPE);

      this->stack_.pop (retval);

      return retval;
    }

    void
    idl3_to_xmi_visitor::add_stereotype (const LITERAL_T name,
                                         const LITERAL_T bc)
    {
      // Generate our xmi.id
      XStr xid (this->gen_xmi_id ());

      // Bind the id in the map
      this->id_map_.bind (LITERALS[name], xid);

      DOMElement *ele = this->create_element (LITERALS[STEREOTYPE_TAG]);
      ele->setAttribute (XStr (LITERALS[XMI_ID]), xid);
      ele->setAttribute (XStr (LITERALS[NAME]), XStr (LITERALS[name]));
      ele->setAttribute (XStr (LITERALS[BASEC]), XStr (LITERALS[bc]));

      return;
    }

    XMLCh *
    idl3_to_xmi_visitor::gen_xmi_id (const ACE_TCHAR *name, long line)
    {
      if (current_id_ == 0)
        {
          ACE_OS::srand (ACE_OS::gettimeofday ().msec ());
          current_id_ = rand ();
        }
      std::stringstream str;
      str << name << ':' << line << '.' << current_id_++;

      XStr retval (ACE_TEXT_CHAR_TO_TCHAR (str.str ().c_str ()));
      return retval.release ();

    }

    XMLCh *
    idl3_to_xmi_visitor::gen_xmi_id (AST_Decl *node)
    {
      if (current_id_ == 0)
        {
          ACE_OS::srand (ACE_OS::gettimeofday ().msec ());
          current_id_ = rand ();
        }

      // we want these IDs to be unique, but the CDMW code generator sometimes depends
      // on the order these things were declared in IDL, so for cases like that,
      // we generate an ID that will sort to the order it was declared in IDL,
      // no matter which order we visit the nodes.
      if (node != 0)
        return gen_xmi_id (ACE_TEXT_CHAR_TO_TCHAR (node->file_name ().c_str ()),
                           node->line ());

      std::stringstream str;

      str << "xmi." << current_id_++;
      XStr retval (ACE_TEXT_CHAR_TO_TCHAR (str.str ().c_str ()));
      return retval.release ();
    }

    void
    idl3_to_xmi_visitor::set_containing_element (const ACE_TCHAR *cont_name)
    {
      ACE_TString tmp;
      XStr xid;

      this->namespace_.top (tmp);
      this->id_map_.find (tmp, xid);
      this->set_attribute (cont_name, xid);
    }

    void
    idl3_to_xmi_visitor::create_and_store_xmi_id (AST_Decl *node)
    {
      try
        {
          this->create_and_store_xmi_id (ACE_TEXT_CHAR_TO_TCHAR (node->repoID ()), node);
        }
      catch (Error &ex)
        {
          ex.node (node);
          throw;
        }
    }

    void
    idl3_to_xmi_visitor::create_and_store_xmi_id (const ACE_TCHAR *name, AST_Decl *node)
    {
      XStr xid = this->gen_xmi_id (node);

      // store xmi_id
      if (this->id_map_.trybind (name, xid) == -1)
        throw Error ("error binding in the XMI ID map");

      this->set_attribute (LITERALS[XMI_ID], xid);
    }

    XMLCh *
    idl3_to_xmi_visitor::lookup_xid (const ACE_TCHAR *name)
    {
      XStr tmp;
      if (this->id_map_.find (name, tmp) == -1)
        {
          ACE_CString err ("unknown XMI ID requested for ");
          err += ACE_TEXT_ALWAYS_CHAR (name);
          throw Error (err.c_str ());
        }

      return tmp.release ();
    }

    XMLCh *
    idl3_to_xmi_visitor::lookup_type_xid (AST_Type *node)
    {
      AST_PredefinedType *pdt = 0;

      switch (node->node_type ())
        {
        case AST_Decl::NT_string:
          return this->lookup_xid (LITERALS[ST_STR]);
        case AST_Decl::NT_wstring:
          return this->lookup_xid (LITERALS[ST_WSTR]);

        case AST_Decl::NT_pre_defined:
          pdt = AST_PredefinedType::narrow_from_decl (node);

          switch (pdt->pt ())
            {
            case AST_PredefinedType::PT_pseudo:
              return this->lookup_xid (ACE_TEXT_CHAR_TO_TCHAR (pdt->repoID ()));
            case AST_PredefinedType::PT_object:
              return this->lookup_xid (LITERALS[ST_OBJ]);
            case AST_PredefinedType::PT_any:
              return this->lookup_xid (LITERALS[ST_ANY]);
            case AST_PredefinedType::PT_long:
              return this->lookup_xid (LITERALS[ST_LONG]);
            case AST_PredefinedType::PT_ulong:
              return this->lookup_xid (LITERALS[ST_UL]);
            case AST_PredefinedType::PT_longlong:
              return this->lookup_xid (LITERALS[ST_LL]);
            case AST_PredefinedType::PT_ulonglong:
              return this->lookup_xid (LITERALS[ST_ULL]);
            case AST_PredefinedType::PT_short:
              return this->lookup_xid (LITERALS[ST_SHORT]);
            case AST_PredefinedType::PT_ushort:
              return this->lookup_xid (LITERALS[ST_US]);
            case AST_PredefinedType::PT_float:
              return this->lookup_xid (LITERALS[ST_FLOAT]);
            case AST_PredefinedType::PT_double:
              return this->lookup_xid (LITERALS[ST_DOUBLE]);
            case AST_PredefinedType::PT_longdouble:
              return this->lookup_xid (LITERALS[ST_LD]);
            case AST_PredefinedType::PT_char:
              return this->lookup_xid (LITERALS[ST_CHAR]);
            case AST_PredefinedType::PT_wchar:
              return this->lookup_xid (LITERALS[ST_WCHAR]);
            case AST_PredefinedType::PT_boolean:
              return this->lookup_xid (LITERALS[ST_BOOL]);
            case AST_PredefinedType::PT_octet:
              return this->lookup_xid (LITERALS[ST_OCTET]);
            case AST_PredefinedType::PT_void:
              return this->lookup_xid (LITERALS[ST_VOID]);
            default:
              throw Error ("Unknown or unsupported predefined type",
                           pdt);
            }
        default:
          return this->lookup_xid (ACE_TEXT_CHAR_TO_TCHAR (node->repoID ()));
        }
    }

    void
    idl3_to_xmi_visitor::find_and_set_xid_as_attr (const ACE_TCHAR *attr_name,
                                                   const ACE_TCHAR *xid_name)
    {
      XStr tmp;

      if (this->id_map_.find (xid_name, tmp) == -1)
        {
          ACE_CString err ("unknown XMI ID when looking up ");
          err += ACE_TEXT_ALWAYS_CHAR (xid_name);
          throw Error (err.c_str ());
        }

      this->set_attribute (attr_name, tmp);
    }

    void
    idl3_to_xmi_visitor::gen_tagged_value (AST_Decl *node)
    {
      ES_Guard me_guard (LITERALS[ME_TV_TAG], this);
      ES_Guard tv_guard (LITERALS[TV_TAG], this);

      this->set_attribute (LITERALS[TAG], LITERALS[TYPEID]);
      this->set_attribute (LITERALS[VALUE],
                           ACE_TEXT_CHAR_TO_TCHAR (node->repoID ()));
    }

    XMLCh *
    idl3_to_xmi_visitor::add_generalization (const ACE_TCHAR *sub, const ACE_TCHAR *super)
    {
      return this->add_generalization (XStr (this->lookup_xid (sub)),
                                       XStr (this->lookup_xid (super)));
    }

    XMLCh *
    idl3_to_xmi_visitor::add_generalization (AST_Type *sub, AST_Type *super)
    {
      return this->add_generalization (XStr (this->lookup_type_xid (sub)),
                                       XStr (this->lookup_type_xid (super)));
    }

    XMLCh *
    idl3_to_xmi_visitor::add_generalization (const XMLCh *sub, const XMLCh *super)
    {
      XStr xid;

      { // add generalization to the 'generalizations' set
        ES_Guard es_guard (this->generalizations_, this);
        NS_Guard ns_guard (ACE_TEXT ("::"), this);

        ES_Guard gen_guard (LITERALS[GEN_TAG], this);

        // Generate XMI ID
        xid = this->gen_xmi_id ();
        this->set_attribute (LITERALS[XMI_ID], xid);

        this->set_containing_element (LITERALS[NS]);

        { // subtype
          ES_Guard st_guard (LITERALS[GEN_SUB_TAG], this);
          ES_Guard element_guard (LITERALS[GEN_ELEMENT_TAG], this);

          this->set_attribute (LITERALS[IDREF], sub);
        }

        { // subtype
          ES_Guard st_guard (LITERALS[GEN_SUPER_TAG], this);
          ES_Guard element_guard (LITERALS[GEN_ELEMENT_TAG], this);

          this->set_attribute (LITERALS[IDREF], super);
        }
      }

      { // Reference generalization in the calling element.
        ES_Guard et_g (LITERALS[GEN_ELEMENT_GEN_TAG], this);
        ES_Guard g_g (LITERALS[GEN_TAG], this);
        this->set_attribute (LITERALS[IDREF], xid);
      }

      return xid.release ();
    }

    void
    idl3_to_xmi_visitor::add_port (const ACE_TCHAR *component,
                                   const ACE_TCHAR *port_kind,
                                   const ACE_TCHAR *port_type,
                                   const ACE_TCHAR *name,
                                   bool is_multiple,
                                   const ACE_TCHAR *file_name,
                                   long line)
    {
      if (is_multiple)
        throw Error ("uses multiple not yet supported.");

      ES_Guard es_guard (this->associations_, this);
      NS_Guard ns_guard (ACE_TEXT ("::"), this);

      ES_Guard assoc_guard (LITERALS[ASSOC_TAG], this);

      // Generate XMI ID
      XStr xid (this->gen_xmi_id (file_name, line));
      this->set_attribute (LITERALS[XMI_ID], xid);
      this->set_attribute (LITERALS[VISIBIL], LITERALS[PUBLIC]);
      this->find_and_set_xid_as_attr (LITERALS[STEREO_ATTR], port_kind);
      this->set_containing_element (LITERALS[NS]);

      ES_Guard conn_guard (LITERALS[ASSOC_CONN_TAG], this);

      { // component end
        ES_Guard end_guard (LITERALS[ASSOC_END_TAG], this);
        this->set_attribute (LITERALS[MULT], ACE_TEXT ("1"));
        this->find_and_set_xid_as_attr (LITERALS[TYPE], component);
      }

      { // component end
        ES_Guard end_guard (LITERALS[ASSOC_END_TAG], this);
        this->set_attribute (LITERALS[NAME], name);
        this->set_attribute (LITERALS[MULT], ACE_TEXT ("1"));
        this->find_and_set_xid_as_attr (LITERALS[TYPE], port_type);
      }
    }

    void
    idl3_to_xmi_visitor::add_managed_component (const ACE_TCHAR *home,
                                                const ACE_TCHAR *component)
    {
      ES_Guard es_guard (this->associations_, this);
      NS_Guard ns_guard (ACE_TEXT ("::"), this);

      ES_Guard assoc_guard (LITERALS[ASSOC_TAG], this);

      // Generate XMI ID
      XStr xid (this->gen_xmi_id ());
      this->set_attribute (LITERALS[XMI_ID], xid);
      this->set_attribute (LITERALS[VISIBIL], LITERALS[PUBLIC]);
      this->find_and_set_xid_as_attr (LITERALS[STEREO_ATTR], LITERALS[ST_MANAGES]);
      this->set_containing_element (LITERALS[NS]);

      ES_Guard conn_guard (LITERALS[ASSOC_CONN_TAG], this);

      { // home end
        ES_Guard end_guard (LITERALS[ASSOC_END_TAG], this);
        this->find_and_set_xid_as_attr (LITERALS[STEREO_ATTR], LITERALS[ST_HOME]);
        this->find_and_set_xid_as_attr (LITERALS[TYPE], home);
      }

      { // component end
        ES_Guard end_guard (LITERALS[ASSOC_END_TAG], this);
        this->find_and_set_xid_as_attr (LITERALS[STEREO_ATTR], LITERALS[ST_COMP]);
        this->find_and_set_xid_as_attr (LITERALS[TYPE], component);
      }
    }

    void
    idl3_to_xmi_visitor::gen_common_elements (AST_Decl *node, const ACE_TCHAR *stereotype)
    {
      this->create_and_store_xmi_id (node);
      this->set_attribute (LITERALS[NAME],
                           ACE_TEXT_CHAR_TO_TCHAR (node->original_local_name ()->get_string ()));
      this->set_attribute (LITERALS[VISIBIL], LITERALS[PUBLIC]);
      this->set_containing_element (LITERALS[NS]);
      this->find_and_set_xid_as_attr (LITERALS[STEREO_ATTR],
                                      stereotype);

      NS_Guard ns_guard (ACE_TEXT_CHAR_TO_TCHAR (node->repoID ()), this);
      this->gen_tagged_value (node);
    }
    void
    idl3_to_xmi_visitor::gen_component_ports (PORTS &ports,
                                              const ACE_TCHAR *component,
                                              const ACE_TCHAR *port_kind,
                                              const ACE_TCHAR *file_name)
    {
      for (size_t i = 0; i < ports.size (); ++i)
        {
          AST_Component::port_description *pd = 0;
          ports.get (pd, i);
          this->add_port (component,
                          port_kind,
                          ACE_TEXT_CHAR_TO_TCHAR (pd->impl->repoID ()),
                          ACE_TEXT_CHAR_TO_TCHAR (IdentifierHelper::original_local_name (pd->id)->get_string ()),
                          pd->is_multiple,
                          file_name,
                          pd->line_number);
        }
    }

    idl3_to_xmi_visitor::ES_Guard::ES_Guard (const ACE_TCHAR *name, idl3_to_xmi_visitor *vis)
      : vis_ (*vis)
    {
      if (vis_.stack_.push (vis_.create_element (name)))
        throw Error ("element stack error");
    }

    idl3_to_xmi_visitor::ES_Guard::ES_Guard (DOMElement *ele, idl3_to_xmi_visitor *vis)
      : vis_ (*vis)
    {
      if (vis_.stack_.push (ele) != 0)
        throw Error ("element stack error");
    }

    idl3_to_xmi_visitor::ES_Guard::~ES_Guard (void)
    {
      DOMElement *tmp;
      if (vis_.stack_.pop (tmp) != 0)
        {
          vis_.output_dirty_ = true;
          ACE_ERROR ((LM_ERROR, "error: element stack underflow."));
        }
    }

    idl3_to_xmi_visitor::NS_Guard::NS_Guard (const ACE_TCHAR *name, idl3_to_xmi_visitor *vis)
      : vis_ (*vis)
    {
      int res = vis_.namespace_.push (name);

      if (res == 0) return;
      throw Error ("stack error");
    }

    idl3_to_xmi_visitor::NS_Guard::~NS_Guard (void)
    {
      ACE_TString tmp;
      if (vis_.namespace_.pop (tmp) != 0)
        {
          vis_.output_dirty_ = true;
          ACE_ERROR ((LM_ERROR, "error:namespace stack underflow"));
        }
    }
  }
}
