// $Id$
#include "IDL2_Traversal.hpp"
#include "IDL2_SyntaxTree.hpp"

namespace IDL2
{
  namespace Traversal
  {
    namespace
    {
      void
      iterate_scope (SyntaxTree::ScopePtr const& s,
                     CCF::Traversal::Visitor* v)
      {
        for (SyntaxTree::Scope::Iterator i = s->begin (); i != s->end (); i++)
        {
          (*i)->accept (v);
        }
      }
    }


    //
    // Scope
    //

    void Scope::
    visit_scope (NodePtr const& s)
    {
      visit_scope_pre (s);
      visit_scope_scope (s);
      visit_scope_post (s);
    }

    void Scope::
    visit_scope_scope (NodePtr const& s)
    {
      iterate_scope (s, this);
    }


    //
    // Module
    //

    void Module::
    visit_module (NodePtr const& m)
    {
      visit_module_pre   (m);
      visit_module_scope (m);
      visit_module_post  (m);
    }

    void Module::
    visit_module_pre (NodePtr const& m)
    {
    }

    void Module::
    visit_module_scope (NodePtr const& m)
    {
      iterate_scope (m, this);
    }

    void Module::
    visit_module_post (NodePtr const& m)
    {
    }

    //
    // InterfaceDecl
    //

    void InterfaceDecl::
    visit_interface_decl (NodePtr const& i)
    {
      visit_interface_decl_pre (i);
      visit_interface_decl_post (i);
    }

    void InterfaceDecl::
    visit_interface_decl_pre (NodePtr const&)
    {
    }

    void InterfaceDecl::
    visit_interface_decl_post (NodePtr const&)
    {
    }


    //
    // InterfaceDef
    //

    void InterfaceDef::
    visit_interface_def (NodePtr const& i)
    {
      visit_interface_def_pre (i);
      visit_interface_def_scope (i);
      visit_interface_def_post (i);
    }

    void InterfaceDef::
    visit_interface_def_pre (NodePtr const&)
    {
    }

    void InterfaceDef::
    visit_interface_def_scope (NodePtr const& i)
    {
      iterate_scope (i, this);
    }

    void InterfaceDef::
    visit_interface_def_post (NodePtr const&)
    {
    }

    //
    // AbstractInterfaceDecl
    //

    void AbstractInterfaceDecl::
    visit_abstract_interface_decl (NodePtr const& i)
    {
      visit_abstract_interface_decl_pre (i);
      visit_abstract_interface_decl_post (i);
    }

    void AbstractInterfaceDecl::
    visit_abstract_interface_decl_pre (NodePtr const&)
    {
    }

    void AbstractInterfaceDecl::
    visit_abstract_interface_decl_post (NodePtr const&)
    {
    }


    //
    // AbstractInterfaceDef
    //

    void AbstractInterfaceDef::
    visit_abstract_interface_def (NodePtr const& i)
    {
      visit_abstract_interface_def_pre (i);
      visit_abstract_interface_def_scope (i);
      visit_abstract_interface_def_post (i);
    }

    void AbstractInterfaceDef::
    visit_abstract_interface_def_pre (NodePtr const&)
    {
    }

    void AbstractInterfaceDef::
    visit_abstract_interface_def_scope (NodePtr const& i)
    {
      iterate_scope (i, this);
    }

    void AbstractInterfaceDef::
    visit_abstract_interface_def_post (NodePtr const&)
    {
    }

    //
    // LocalInterfaceDecl
    //

    void LocalInterfaceDecl::
    visit_local_interface_decl (NodePtr const& i)
    {
      visit_local_interface_decl_pre (i);
      visit_local_interface_decl_post (i);
    }

    void LocalInterfaceDecl::
    visit_local_interface_decl_pre (NodePtr const&)
    {
    }

    void LocalInterfaceDecl::
    visit_local_interface_decl_post (NodePtr const&)
    {
    }


    //
    // LocalInterfaceDef
    //

    void LocalInterfaceDef::
    visit_local_interface_def (NodePtr const& i)
    {
      visit_local_interface_def_pre (i);
      visit_local_interface_def_scope (i);
      visit_local_interface_def_post (i);
    }

    void LocalInterfaceDef::
    visit_local_interface_def_pre (NodePtr const&)
    {
    }

    void LocalInterfaceDef::
    visit_local_interface_def_scope (NodePtr const& i)
    {
      iterate_scope (i, this);
    }

    void LocalInterfaceDef::
    visit_local_interface_def_post (NodePtr const&)
    {
    }

    //
    // UnconstrainedInterfaceDecl
    //

    void UnconstrainedInterfaceDecl::
    visit_unconstrained_interface_decl (NodePtr const& i)
    {
      visit_unconstrained_interface_decl_pre (i);
      visit_unconstrained_interface_decl_post (i);
    }

    void UnconstrainedInterfaceDecl::
    visit_unconstrained_interface_decl_pre (NodePtr const&)
    {
    }

    void UnconstrainedInterfaceDecl::
    visit_unconstrained_interface_decl_post (NodePtr const&)
    {
    }


    //
    // UnconstrainedInterfaceDef
    //

    void UnconstrainedInterfaceDef::
    visit_unconstrained_interface_def (NodePtr const& i)
    {
      visit_unconstrained_interface_def_pre (i);
      visit_unconstrained_interface_def_scope (i);
      visit_unconstrained_interface_def_post (i);
    }

    void UnconstrainedInterfaceDef::
    visit_unconstrained_interface_def_pre (NodePtr const&)
    {
    }

    void UnconstrainedInterfaceDef::
    visit_unconstrained_interface_def_scope (NodePtr const& i)
    {
      iterate_scope (i, this);
    }

    void UnconstrainedInterfaceDef::
    visit_unconstrained_interface_def_post (NodePtr const&)
    {
    }

    //
    //
    //

    void AttributeDecl::
    visit_attribute_decl (NodePtr const& n)
    {
      visit_attribute_pre (n);

      if (type_) n->type ()->accept (type_);
      else n->type ()->accept (this);

      visit_attribute_post (n);
    }


    void AttributeDecl::
    visit_attribute_pre (NodePtr const& n)
    {
    }

    void AttributeDecl::
    visit_attribute_post (NodePtr const& n)
    {
    }


    //
    // OperationParameter
    //
    void OperationParameter::
    visit_operation_parameter (NodePtr const& op)
    {
      visit_operation_parameter_pre (op);

      CCF::Traversal::Visitor* v = this;

      switch (op->direction ())
      {
      case SyntaxTree::OperationParameter::Direction::IN:
        {
          if (in_) v = in_;
          break;
        }
      case SyntaxTree::OperationParameter::Direction::OUT:
        {
          if (out_) v = out_;
          break;
        }
      case SyntaxTree::OperationParameter::Direction::INOUT:
        {
          if (inout_) v = inout_;
          break;
        }
      }

      op->type ()->accept (v);

      visit_operation_parameter_post (op);
    }

    //
    // OperationDecl
    //

    void OperationDecl::
    visit_operation_decl (NodePtr const& n)
    {
      visit_operation_decl_pre (n);
      visit_operation_decl_type (n);
      visit_operation_decl_name (n);
      visit_operation_decl_parameters (n);
      visit_operation_decl_post (n);
    }

    void OperationDecl::
    visit_operation_decl_pre (NodePtr const& n)
    {
    }

    void OperationDecl::
    visit_operation_decl_type (NodePtr const& n)
    {
      if (type_) n->type ()->accept (type_);
      else n->type ()->accept (this);
    }

    void OperationDecl::
    visit_operation_decl_name (NodePtr const& n)
    {
    }

    void OperationDecl::
    visit_operation_decl_parameters (NodePtr const& n)
    {
      for (SyntaxTree::OperationDecl::Iterator i = n->begin ();
           i != n->end ();
           i++)
      {
        (*i)->accept (this);

        if (i + 1 != n->end ())
        {
          //@@ could be OperationParameterComma
          SyntaxTree::CommaPtr c (new SyntaxTree::Comma);
          c->accept (this);
        }
      }
    }

    void OperationDecl::
    visit_operation_decl_post (NodePtr const& n)
    {
    }

    //
    // FileScope
    //

    void FileScope::
    visit_file_scope (NodePtr const& fs)
    {
      visit_file_scope_pre   (fs);
      visit_file_scope_scope (fs);
      visit_file_scope_post  (fs);
    }

    void FileScope::
    visit_file_scope_pre (NodePtr const& fs)
    {
    }

    void FileScope::
    visit_file_scope_scope (NodePtr const& fs)
    {
      iterate_scope (fs, this);
    }

    void FileScope::
    visit_file_scope_post (NodePtr const& fs)
    {
    }

    //
    // TranslationRegion
    //

    namespace
    {
      void
      iterate_translation_region (SyntaxTree::TranslationRegionPtr const& tr,
                                  CCF::Traversal::Visitor* v)
      {
        // visit nested translation regions
        for (SyntaxTree::TranslationRegion::Iterator i = tr->begin ();
             i != tr->end ();
             i++)
        {
          (*i)->accept (v);
        }

        // visit associated scope
        tr->scope ()->accept (v);
      }
    }


    void TranslationRegion::
    visit_translation_region (NodePtr const& n)
    {
      visit_translation_region_pre (n);
      visit_translation_region_content (n);
      visit_translation_region_post (n);
    }

    void TranslationRegion::
    visit_translation_region_pre (NodePtr const& n)
    {
      iterate_translation_region (n, this);
    }

    void TranslationRegion::
    visit_translation_region_content (NodePtr const& n)
    {
    }

    void TranslationRegion::
    visit_translation_region_post (NodePtr const& n)
    {
    }

    //
    // IncludeTranslationRegion
    //

    void IncludeTranslationRegion::
    visit_include_translation_region (NodePtr const& n)
    {
      visit_include_translation_region_pre (n);
      visit_include_translation_region_content (n);
      visit_include_translation_region_post (n);
    }

    void IncludeTranslationRegion::
    visit_include_translation_region_pre (NodePtr const& n)
    {
      iterate_translation_region (n, this);
    }

    void IncludeTranslationRegion::
    visit_include_translation_region_content (NodePtr const& n)
    {
    }

    void IncludeTranslationRegion::
    visit_include_translation_region_post (NodePtr const& n)
    {
    }

    //
    // SysIncludeTranslationRegion
    //

    void SysIncludeTranslationRegion::
    visit_sys_include_translation_region (NodePtr const& n)
    {
      visit_sys_include_translation_region_pre (n);
      visit_sys_include_translation_region_content (n);
      visit_sys_include_translation_region_post (n);
    }

    void SysIncludeTranslationRegion::
    visit_sys_include_translation_region_pre (NodePtr const& n)
    {
      iterate_translation_region (n, this);
    }

    void SysIncludeTranslationRegion::
    visit_sys_include_translation_region_content (NodePtr const& n)
    {
    }

    void SysIncludeTranslationRegion::
    visit_sys_include_translation_region_post (NodePtr const& n)
    {
    }

    //
    // ImpliedIncludeTranslationRegion
    //

    void ImpliedIncludeTranslationRegion::
    visit_implied_include_translation_region (NodePtr const& n)
    {
      visit_implied_include_translation_region_pre (n);
      visit_implied_include_translation_region_content (n);
      visit_implied_include_translation_region_post (n);
    }

    void ImpliedIncludeTranslationRegion::
    visit_implied_include_translation_region_pre (NodePtr const& n)
    {
      iterate_translation_region (n, this);
    }

    void ImpliedIncludeTranslationRegion::
    visit_implied_include_translation_region_content (NodePtr const& n)
    {
    }

    void ImpliedIncludeTranslationRegion::
    visit_implied_include_translation_region_post (NodePtr const& n)
    {
    }


    //
    // PrincipalTranslationRegion
    //

    void PrincipalTranslationRegion::
    visit_principal_translation_region (NodePtr const& n)
    {
      visit_principal_translation_region_pre (n);
      visit_principal_translation_region_content (n);
      visit_principal_translation_region_post (n);
    }

    void PrincipalTranslationRegion::
    visit_principal_translation_region_pre (NodePtr const& n)
    {
      iterate_translation_region (n, this);
    }

    void PrincipalTranslationRegion::
    visit_principal_translation_region_content (NodePtr const& n)
    {
    }

    void PrincipalTranslationRegion::
    visit_principal_translation_region_post (NodePtr const& n)
    {
    }

    //
    // TranslationUnit
    //

    void TranslationUnit::
    visit_translation_unit (NodePtr const& tu)
    {
      visit_translation_unit_pre (tu);
      visit_translation_unit_content (tu);
      visit_translation_unit_post (tu);
    }

    void TranslationUnit::
    visit_translation_unit_pre (NodePtr const& tu)
    {
    }

    void TranslationUnit::
    visit_translation_unit_content (NodePtr const& tu)
    {
      // Visit nested translation regions
      for (SyntaxTree::TranslationUnit::Iterator i = tu->begin ();
           i != tu->end ();
           i++)
      {
        (*i)->accept (this);
      }
    }

    void TranslationUnit::
    visit_translation_unit_post (NodePtr const& tu)
    {
    }
  }
}
