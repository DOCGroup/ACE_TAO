// $Id$

#include "IR_Simulator_visitor.h"

#include "be_sunsoft.h"
#include "be_extern.h"

#include "ast_argument.h"
#include "ast_array.h"
#include "ast_attribute.h"
#include "ast_component_fwd.h"
#include "ast_provides.h"
#include "ast_uses.h"
#include "ast_publishes.h"
#include "ast_emits.h"
#include "ast_consumes.h"
#include "ast_enum.h"
#include "ast_enum_val.h"
#include "ast_eventtype.h"
#include "ast_eventtype_fwd.h"
#include "ast_exception.h"
#include "ast_finder.h"
#include "ast_field.h"
#include "ast_home.h"
#include "ast_operation.h"
#include "ast_root.h"
#include "ast_sequence.h"
#include "ast_string.h"
#include "ast_structure_fwd.h"
#include "ast_template_module.h"
#include "ast_template_module_inst.h"
#include "ast_template_module_ref.h"
#include "ast_param_holder.h"
#include "ast_typedef.h"
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


#include <iostream>
#include <limits>

#if 0
#define XMI_TRACE(X) std::cout << X << std::endl
#else
#define XMI_TRACE(X)
#endif

namespace DAnCE
{
  namespace XMI
  {
    struct Incr_Guard
    {
    public:
      Incr_Guard (size_t &subject, bool arm = false)
        : subject_ (subject),
          armed_ (arm)
      {
      }

      ~Incr_Guard (void)
      {
        if (this->armed_)
          --subject_;
      }
      void arm (void)
      {
        this->armed_ = true;
        ++subject_;
      }

    private:
      size_t &subject_;
      bool armed_;
    };

    ir_simulator_visitor::ir_simulator_visitor (void)
      : associating_ (false)
    {
    }

    ir_simulator_visitor::~ir_simulator_visitor (void)
    {
    }

    int
    ir_simulator_visitor::visit_decl (AST_Decl *)
    {
      XMI_TRACE ("got a decl");
      return 0;
    }

    int
    ir_simulator_visitor::visit_scope (UTL_Scope *node)
    {
      XMI_TRACE ("got a scope");

      for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
           !si.is_done ();
           si.next ())
        {
          AST_Decl *d = si.item ();

          if (d->ast_accept (this) != 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "ir_simulator_visitor::visit_scope - "
                                 "codegen for scope failed\n"),
                                -1);
            }
        }
      return 0;
    }

    int
    ir_simulator_visitor::visit_type (AST_Type *)
    {
      XMI_TRACE ("got a type");
      return 0;
    }

    int
    ir_simulator_visitor::visit_predefined_type (AST_PredefinedType *)
    {
      XMI_TRACE ("predef type");
      return 0;
    }

    int
    ir_simulator_visitor::visit_module (AST_Module *node)
    {
      XMI_TRACE ("module");

      if (!this->do_i_care (node))
        {
          return 0;
        }

      Incr_Guard guard (this->associating_);

      if (!this->associating_)
        {
          // not associating, imported, not my concern
          if (node->imported ())
            {
              return 0;
            }
          // not imported, but not associating, need to set the guard
          else
            {
              guard.arm ();
            }
        }
      // associating, import
      else
        {
          node->set_imported (false);
        }

      try
        {
          if (this->visit_scope (node) != 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "idl3_to_idl2_visitor::visit_module - "
                                 "codegen for scope failed\n"),
                                -1);
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
    ir_simulator_visitor::visit_template_module (AST_Template_Module *node)
    {
      XMI_TRACE ("template module");

      throw Error ("Template modules not supported", node);

      return 0;
    }

    int
    ir_simulator_visitor::visit_template_module_inst (
      AST_Template_Module_Inst *node)
    {
      XMI_TRACE ("template module instance");

      throw Error ("Template modules not supported", node);

      return 0;
    }

    int
    ir_simulator_visitor::visit_template_module_ref (
      AST_Template_Module_Ref *node)
    {
      XMI_TRACE ("template module reference");

      throw Error ("Template modules not supported", node);

      return 0;
    }

    int
    ir_simulator_visitor::visit_param_holder (AST_Param_Holder *node)
    {
      XMI_TRACE ("param holder");

      throw Error ("Param holders not supported", node);

      return 0;
    }

    int
    ir_simulator_visitor::visit_finder (AST_Finder *node)
    {
      XMI_TRACE ("finder");

      throw Error ("Finders not supported", node);

      return 0;
    }

    int
    ir_simulator_visitor::visit_interface (AST_Interface *node)
    {
      XMI_TRACE ("interface");

      if (!this->do_i_care (node))
        {
          return 0;
        }

      Incr_Guard guard (this->associating_);

      if (!this->associating_)
        {
          // not associating, imported, not my concern
          if (node->imported ())
            {
              return 0;
            }
          // not imported, but not associating, need to set the guard
          else
            {
              guard.arm ();
            }
        }
      // associating, import
      else
        {
          node->set_imported (false);
        }

      try
        {
          // Inheritance
          for (long i = 0; i < node->n_inherits (); ++i)
            {
              node->inherits ()[i]->ast_accept (this);
            }

          if (this->visit_scope (node) != 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "idl3_to_idl2_visitor::visit_interface - "
                                 "codegen for scope failed\n"),
                                -1);
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
    ir_simulator_visitor::visit_interface_fwd (AST_InterfaceFwd *node)
    {
      XMI_TRACE ("interface_fwd");

      // won't hurt to import the fwd interface
      node->set_imported (false);

      return 0;
    }

    int
    ir_simulator_visitor::visit_valuebox (AST_ValueBox *node)
    {
      XMI_TRACE ("valuebox");

      if (!this->do_i_care (node))
        {
          return 0;
        }

      Incr_Guard guard (this->associating_);

      if (!this->associating_)
        {
          // not associating, imported, not my concern
          if (node->imported ()) return 0;
          // not imported, but not associating, need to set the guard
          else guard.arm ();
        }
      // associating, import
      else
        {
          node->set_imported (false);
        }

      try
        {
          // **** need to visit type we are boxing?
          node->boxed_type ()->ast_accept (this);
        }
      catch (Error &err)
        {
          err.node (node);
          throw;
        }

      return 0;
    }

    int
    ir_simulator_visitor::visit_valuetype (AST_ValueType *node)
    {
      XMI_TRACE ("valuetype");

      try
        {
          this->visit_valuetype_impl (node);
        }
      catch (Error &err)
        {
          err.node (node);
        }

      return 0;
    }

    void
    ir_simulator_visitor::visit_valuetype_impl (AST_ValueType *node)
    {
      Incr_Guard guard (this->associating_);

      if (!this->do_i_care (node))
        {
          return;
        }

      if (!this->associating_)
        {
          // not associating, imported, not my concern
          if (node->imported ())
            {
              return;
            }
          // not imported, but not associating, need to set the guard
          else
            {
              guard.arm ();
            }
        }
      // associating, import
      else
        {
          node->set_imported (false);
        }

      try
        {
          AST_Type *t = node->inherits_concrete ();
          AST_ValueType *v =
            AST_ValueType::narrow_from_decl (t);

          if (v != 0)
            {
              this->visit_valuetype_impl (v);
            }

          long lim = node->n_supports ();
          AST_Type **sppts = node->supports ();

          for (long i = 0; i < lim; ++i)
            {
              sppts[i]->ast_accept (this);
            }

          this->visit_scope (node);
        }
      catch (Error &err)
        {
          err.node (node);
          throw;
        }
    }

    int
    ir_simulator_visitor::visit_valuetype_fwd (AST_ValueTypeFwd *node)
    {
      XMI_TRACE ("valuetype_fwd");

      try
        {
          // no harm from importing a fwd interface
          //          node->is_imported (false);
        }
      catch (Error &err)
        {
          err.node (node);
        }

      return 0;
    }

    int
    ir_simulator_visitor::visit_component (AST_Component *node)
    {
      XMI_TRACE ("component");

      if (!this->do_i_care (node))
        {
          return 0;
        }

      Incr_Guard guard (this->associating_);

      if (!this->associating_)
        {
          // not associating, imported, not my concern
          if (node->imported ())
            {
              return 0;
            }
          // not imported, but not associating, need to set the guard
          else
            {
              guard.arm ();
            }
        }
      // associating, import
      else
        {
          node->set_imported (false);
        }

      try
        {
          if (node->base_component () != 0)
            {
              node->base_component ()->ast_accept (this);
            }

          long len = node->n_supports ();
          AST_Type **sppts = node->supports ();

          for (long i = 0; i < len; ++i)
            {
              sppts[i]->ast_accept (this);
            }

          this->visit_scope (node);
        }
      catch (Error &err)
        {
          err.node (node);
        }

      return 0;
    }

    int
    ir_simulator_visitor::visit_component_fwd (AST_ComponentFwd *node)
    {
      XMI_TRACE ("component_fwd");

      try
        {
          // no harm from importing a fwd interface
          //          node->is_imported (false);
        }
      catch (Error &err)
        {
          err.node (node);
        }

      return 0;
    }

    int
    ir_simulator_visitor::visit_provides (AST_Provides *node)
    {
      return node->provides_type ()->ast_accept (this);
    }

    int
    ir_simulator_visitor::visit_uses (AST_Uses *node)
    {
      return node->uses_type ()->ast_accept (this);
    }

    int
    ir_simulator_visitor::visit_publishes (AST_Publishes *node)
    {
      return node->publishes_type ()->ast_accept (this);
    }

    int
    ir_simulator_visitor::visit_emits (AST_Emits *node)
    {
      return node->emits_type ()->ast_accept (this);
    }

    int
    ir_simulator_visitor::visit_consumes (AST_Consumes *node)
    {
      return node->consumes_type ()->ast_accept (this);
    }

    int
    ir_simulator_visitor::visit_porttype (AST_PortType *)
    {
      return 0;
    }

    int
    ir_simulator_visitor::visit_extended_port (AST_Extended_Port *)
    {
      return 0;
    }

    int
    ir_simulator_visitor::visit_mirror_port (AST_Mirror_Port *)
    {
      return 0;
    }

    int
    ir_simulator_visitor::visit_connector (AST_Connector *)
    {
      return 0;
    }

    int
    ir_simulator_visitor::visit_eventtype (AST_EventType *node)
    {
      XMI_TRACE ("eventtype");

      try
        {
          this->visit_valuetype_impl (node);
        }
      catch (Error &err)
        {
          err.node (node);
        }

      return 0;
    }

    int
    ir_simulator_visitor::visit_eventtype_fwd (AST_EventTypeFwd *node)
    {
      XMI_TRACE ("eventtype_fwd");

      this->visit_valuetype_fwd (node);

      return 0;
    }

    int
    ir_simulator_visitor::visit_home (AST_Home *node)
    {
      XMI_TRACE ("home");

      if (!this->do_i_care (node))
        {
          return 0;
        }

      Incr_Guard guard (this->associating_);

      if (!this->associating_)
        {
          // not associating, imported, not my concern
          if (node->imported ())
            {
              return 0;
            }
          // not imported, but not associating, need to set the guard
          else
            {
              guard.arm ();
            }
        }
      // associating, import
      else
        {
          node->set_imported (false);
        }

      try
        {
          // **** supported interfaces
          long len = node->n_supports ();
          AST_Type **sppts = node->supports ();

          for (long i = 0; i < len; ++i)
            {
              sppts[i]->ast_accept (this);
            }

          if (node->base_home ())
            {
              node->base_home ()->ast_accept (this);
            }

          if (node->managed_component ())
            {
              node->managed_component ()->ast_accept (this);
            }

          if (node->primary_key ())
            {
              node->primary_key ()->ast_accept (this);
            }

          this->visit_scope (node);
        }
      catch (Error &err)
        {
          err.node (node);
        }

      return 0;
    }

    int
    ir_simulator_visitor::visit_factory (AST_Factory *node)
    {
      XMI_TRACE ("factory");

      throw Error ("Factories not supported", node);

      return 0;
    }

    int
    ir_simulator_visitor::visit_structure (AST_Structure *node)
    {
      XMI_TRACE ("structure");

      try
        {
          this->visit_struct_impl (node);
        }
      catch (Error &err)
        {
          err.node (node);
          throw;
        }

      return 0;
    }

    void
    ir_simulator_visitor::visit_struct_impl (AST_Structure *node)
    {
      if (!this->do_i_care (node))
        {
          return;
        }

      Incr_Guard guard (this->associating_);

      if (!this->associating_)
        {
          // not associating, imported, not my concern
          if (node->imported ())
            {
              return;
            }
          // not imported, but not associating, need to set the guard
          else
            {
              guard.arm ();
            }
        }
      // associating, import
      else
        {
          node->set_imported (false);
        }

      try
        {
          this->visit_scope (node);
        }
      catch (Error &err)
        {
          err.node (node);
          throw;
        }
    }

    int
    ir_simulator_visitor::visit_structure_fwd (AST_StructureFwd *node)
    {
      XMI_TRACE ("structure_fwd");

      try
        {
          node->set_imported (false);
        }
      catch (Error &err)
        {
          err.node (node);
        }

      return 0;
    }

    int
    ir_simulator_visitor::visit_exception (AST_Exception *node)
    {
      XMI_TRACE ("exception");


      try
        {
          this->visit_struct_impl (node);
        }
      catch (Error &err)
        {
          err.node (node);
          throw;
        }

      return 0;
    }

    int
    ir_simulator_visitor::visit_expression (AST_Expression *)
    {
      XMI_TRACE ("expression");

      /*      Incr_Guard guard (this->associating_);

      if (!this->associating_)
        {
          // not associating, imported, not my concern
          if (node->imported ()) return;
          // not imported, but not associating, need to set the guard
          else guard.arm ();
        }
      // associating, import
      else node->set_imported (false);
      */
      return 0;
    }

    int
    ir_simulator_visitor::visit_enum (AST_Enum *node)
    {
      XMI_TRACE ("enum");

      if (!this->do_i_care (node))
        {
          return 0;
        }

      Incr_Guard guard (this->associating_);

      if (!this->associating_)
        {
          // not associating, imported, not my concern
          if (node->imported ())
            {
              return 0;
            }
          // not imported, but not associating, need to set the guard
          else
            {
              guard.arm ();
            }
        }
      // associating, import
      else
        {
          node->set_imported (false);
        }

      try
        {
          this->visit_scope (node);
        }
      catch (Error &err)
        {
          err.node (node);
          throw;
        }

      return 0;
    }

    int
    ir_simulator_visitor::visit_operation (AST_Operation *node)
    {
      XMI_TRACE ("operation");

      Incr_Guard guard (this->associating_);

      if (!this->associating_)
        {
          // not associating, imported, not my concern
          if (node->imported ())
            {
              return 0;
            }
          // not imported, but not associating, need to set the guard
          else
            {
              guard.arm ();
            }
        }
      // associating, import
      else
        {
          node->set_imported (false);
        }

      try
        {
          //          ACE_DEBUG ((LM_DEBUG, "foo"));

          if (node->return_type ())
            {
              //              ACE_DEBUG ((LM_DEBUG, "bar"));
              node->return_type ()->ast_accept (this);
            }

          // **** arguments
          // **** exceptions
          UTL_ExceptList *exceptions = node->exceptions ();

          if (exceptions != 0 && exceptions->length () > 0)
            {
              for (UTL_ExceptlistActiveIterator ei (exceptions);
                   !ei.is_done ();
                   ei.next ())
                {
                  ei.item ()->ast_accept (this);
                }
            }

          this->visit_scope (node);
        }
      catch (Error &err)
        {
          err.node (node);
        }

      return 0;
    }

    int
    ir_simulator_visitor::visit_field (AST_Field *node)
    {
      XMI_TRACE ("field");

      if (node == 0)
        {
          ACE_DEBUG ((LM_DEBUG, "null field?\n"));
          return 0;
        }

      Incr_Guard guard (this->associating_);

      if (!this->associating_)
        {
          // not associating, imported, not my concern
          if (node->imported ())
            {
              return 0;
            }
          // not imported, but not associating, need to set the guard
          else
            {
              guard.arm ();
            }
        }
      // associating, import
      else
        {
          node->set_imported (false);
        }

      try
        {
          if (node->field_type ())
            {
              node->field_type ()->ast_accept (this);
            }
        }
      catch (Error &err)
        {
          err.node (node);
        }

      return 0;
    }

    int
    ir_simulator_visitor::visit_argument (AST_Argument *node)
    {
      XMI_TRACE ("argument");

      this->visit_field (node);

      return 0;
    }

    int
    ir_simulator_visitor::visit_attribute (AST_Attribute *node)
    {
      XMI_TRACE ("attribute");

      return this->visit_field (node);
    }

    int
    ir_simulator_visitor::visit_union (AST_Union *node)
    {
      XMI_TRACE ("union");

      if (!this->do_i_care (node))
        {
          return 0;
        }

      Incr_Guard guard (this->associating_);

      if (!this->associating_)
        {
          // not associating, imported, not my concern
          if (node->imported ())
            {
              return 0;
            }
          // not imported, but not associating, need to set the guard
          else
            {
              guard.arm ();
            }
        }
      // associating, import
      else
        {
          node->set_imported (false);
        }

      if (node->disc_type ())
        {
          this->visit_type (node->disc_type ());
        }

      return this->visit_scope (node);
    }

    int
    ir_simulator_visitor::visit_union_fwd (AST_UnionFwd *node)
    {
      XMI_TRACE ("union_fwd");
      node->set_imported (false);
      return 0;
    }

    int
    ir_simulator_visitor::visit_union_branch (AST_UnionBranch *node)
    {
      XMI_TRACE ("union_branch");
      this->visit_field (node);
      return 0;
    }

    int
    ir_simulator_visitor::visit_union_label (AST_UnionLabel *node)
    {
      XMI_TRACE ("union_label");

      if (node->label_val ())
        {
          node->label_val ()->ast_accept (this);
        }

      return 0;
    }

    int
    ir_simulator_visitor::visit_constant (AST_Constant *)
    {
      XMI_TRACE ("constant");

      return 0;
    }

    int
    ir_simulator_visitor::visit_enum_val (AST_EnumVal *)
    {
      XMI_TRACE ("enum val");
      return 0;
    }

    int
    ir_simulator_visitor::visit_array (AST_Array *node)
    {
      XMI_TRACE ("array val");

      if (!this->do_i_care (node))
        {
          return 0;
        }

      Incr_Guard guard (this->associating_);

      if (!this->associating_)
        {
          // not associating, imported, not my concern
          if (node->imported ())
            {
              return 0;
            }
          // not imported, but not associating, need to set the guard
          else
            {
              guard.arm ();
            }
        }
      // associating, import
      else
        {
          node->set_imported (false);
        }

      return node->base_type ()->ast_accept (this);
    }

    int
    ir_simulator_visitor::visit_sequence (AST_Sequence *node)
    {
      XMI_TRACE ("sequence val");

      try
        {
          Incr_Guard guard (this->associating_);

          if (!this->associating_)
            {
              // not associating, imported, not my concern
              if (node->imported ())
                {
                  return 0;
                }
              // not imported, but not associating, need to set the guard
              else
                {
                  guard.arm ();
                }
            }
          // associating, import
          else
            {
              node->set_imported (false);
            }

          node->base_type ()->ast_accept (this);
        }
      catch (Error &err)
        {
          err.node (node);
          throw;
        }

      return 0;
    }

    int
    ir_simulator_visitor::visit_string (AST_String *)
    {
      XMI_TRACE ("string");
      return 0;
    }

    int
    ir_simulator_visitor::visit_typedef (AST_Typedef *node)
    {
      XMI_TRACE ("typedef");

      Incr_Guard guard (this->associating_);

      if (!this->associating_)
        {
          // not associating, imported, not my concern
          if (node->imported ())
            {
              return 0;
            }
          // not imported, but not associating, need to set the guard
          else
            {
              guard.arm ();
            }
        }
      // associating, import
      else
        {
          node->set_imported (false);
        }

      return node->base_type ()->ast_accept (this);
    }

    int
    ir_simulator_visitor::visit_root (AST_Root *node)
    {
      XMI_TRACE ("root");

      try
        {
          this->visit_scope (node);
        }
      catch (const Error &ex)
        {
          if (ex.node_ != 0)
            {
              ACE_ERROR ((LM_ERROR, "%s:%d:error: %s\n",
                          ex.node_->file_name ().c_str (),
                          ex.node_->line (),
                          ex.diag_.c_str ()));
            }
          else
            {
              ACE_ERROR ((LM_ERROR, "::error:%s\n",
                          ex.diag_.c_str ()));
            }

          return -1;
        }

      return 0;
    }

    int
    ir_simulator_visitor::visit_native (AST_Native *)
    {
      XMI_TRACE ("native");
      return 0;
    }

    bool
    ir_simulator_visitor::do_i_care (AST_Decl *node)
    {
      std::string name (node->repoID ());

      return this->seen_types_.insert (name).second;
    }
  }
}
