// $Id$
#ifndef CCF_IDL2_TRAVERSAL_HPP
#define CCF_IDL2_TRAVERSAL_HPP

#include "MSVC_Pragmas.hpp"

#include "CCF/CompilerElements/Traversal.hpp"
#include "CCF/IDL2/IDL2_SyntaxTreeFwd.hpp"

namespace IDL2
{
  namespace Traversal
  {
    //
    //
    //
    struct Comma : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::CommaPtr
      NodePtr;

      virtual void visit_comma (NodePtr const& s)
      {
      }
    };

    //
    //
    //
    struct Declaration : virtual CCF::Traversal::Visitor
    {
    };

    //
    //
    //
    struct TypeDecl : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::TypeDeclPtr
      NodePtr;

      virtual void visit_type_decl (NodePtr const& n)
      {
      }
    };

    //
    //
    //
    struct TypeDef : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::TypeDefPtr
      NodePtr;

      virtual void visit_type_def (NodePtr const& n)
      {
      }
    };

    //
    //
    //
    struct BuiltInTypeDef : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::BuiltInTypeDefPtr
      NodePtr;

      virtual void visit_built_in_type_def (NodePtr const& n)
      {
      }
    };


    //
    //
    //
    struct Void : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::VoidPtr
      NodePtr;

      virtual void visit_void (NodePtr const& n)
      {
      }
    };

    //
    //
    //
    struct Long : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::LongPtr
      NodePtr;

      virtual void visit_long (NodePtr const& n)
      {
      }
    };

    //
    //
    //
    struct Boolean : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::BooleanPtr
      NodePtr;

      virtual void visit_boolean (NodePtr const& n)
      {
      }
    };

    //
    //
    //
    struct String : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::StringPtr
      NodePtr;

      virtual void visit_string (NodePtr const& n)
      {
      }
    };

    //
    //
    //
    struct Scope : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::ScopePtr
      NodePtr;

      // visit_scope () function will never be called because Scope is
      // an abstract syntax tree node. It is here for symmetry.
      virtual void visit_scope (NodePtr const& s);
      virtual void visit_scope_scope (NodePtr const& s);
      virtual void visit_scope_pre (NodePtr const& ) {}
      virtual void visit_scope_post (NodePtr const& ) {}
    };

    //
    //
    //
    struct Module : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::ModulePtr
      NodePtr;

      virtual void visit_module (NodePtr const& m);
      virtual void visit_module_pre (NodePtr const& m);
      virtual void visit_module_scope (NodePtr const& m);
      virtual void visit_module_post (NodePtr const& m);
    };


    //
    //
    //
    struct InterfaceDecl : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::InterfaceDeclPtr
      NodePtr;

      virtual void visit_interface_decl (NodePtr const& i);
      virtual void visit_interface_decl_pre (NodePtr const& i);
      virtual void visit_interface_decl_post (NodePtr const& i);
    };


    //
    //
    //
    struct InterfaceDef : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::InterfaceDefPtr
      NodePtr;

      virtual void visit_interface_def (NodePtr const& i);
      virtual void visit_interface_def_pre (NodePtr const& i);
      virtual void visit_interface_def_scope (NodePtr const& i);
      virtual void visit_interface_def_post (NodePtr const& i);
    };


    //
    //
    //
    struct AbstractInterfaceDecl : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::AbstractInterfaceDeclPtr
      NodePtr;

      virtual void visit_abstract_interface_decl (NodePtr const& i);
      virtual void visit_abstract_interface_decl_pre (NodePtr const& i);
      virtual void visit_abstract_interface_decl_post (NodePtr const& i);
    };


    //
    //
    //
    struct AbstractInterfaceDef : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::AbstractInterfaceDefPtr
      NodePtr;

      virtual void visit_abstract_interface_def (NodePtr const& i);
      virtual void visit_abstract_interface_def_pre (NodePtr const& i);
      virtual void visit_abstract_interface_def_scope (NodePtr const& i);
      virtual void visit_abstract_interface_def_post (NodePtr const& i);
    };

    //
    //
    //
    struct LocalInterfaceDecl : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::LocalInterfaceDeclPtr
      NodePtr;

      virtual void visit_local_interface_decl (NodePtr const& i);
      virtual void visit_local_interface_decl_pre (NodePtr const& i);
      virtual void visit_local_interface_decl_post (NodePtr const& i);
    };


    //
    //
    //
    struct LocalInterfaceDef : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::LocalInterfaceDefPtr
      NodePtr;

      virtual void visit_local_interface_def (NodePtr const& i);
      virtual void visit_local_interface_def_pre (NodePtr const& i);
      virtual void visit_local_interface_def_scope (NodePtr const& i);
      virtual void visit_local_interface_def_post (NodePtr const& i);
    };


    //
    //
    //
    struct UnconstrainedInterfaceDecl : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::UnconstrainedInterfaceDeclPtr
      NodePtr;

      virtual void visit_unconstrained_interface_decl (NodePtr const& i);
      virtual void visit_unconstrained_interface_decl_pre (NodePtr const& i);
      virtual void visit_unconstrained_interface_decl_post (NodePtr const& i);
    };


    //
    //
    //
    struct UnconstrainedInterfaceDef : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::UnconstrainedInterfaceDefPtr
      NodePtr;

      virtual void visit_unconstrained_interface_def (NodePtr const& i);
      virtual void visit_unconstrained_interface_def_pre (NodePtr const& i);
      virtual void visit_unconstrained_interface_def_scope (NodePtr const& i);
      virtual void visit_unconstrained_interface_def_post (NodePtr const& i);
    };


    //
    //
    //
    struct AttributeDecl : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::AttributeDeclPtr
      NodePtr;

      AttributeDecl (CCF::Traversal::Visitor* type = 0)
          : type_ (type)
      {
      }

      virtual void visit_attribute_decl (NodePtr const& n);
      virtual void visit_attribute_pre (NodePtr const& n);
      virtual void visit_attribute_post (NodePtr const& n);

    private:
      CCF::Traversal::Visitor* type_;
    };

    //
    //
    //
    struct OperationParameter : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::OperationParameterPtr
      NodePtr;

      OperationParameter (CCF::Traversal::Visitor* in,
                          CCF::Traversal::Visitor* out,
                          CCF::Traversal::Visitor* inout)
          : in_ (in), out_ (out), inout_ (inout)
      {
      }

      OperationParameter ()
          : in_ (0), out_ (0), inout_ (0)
      {
      }


      virtual void visit_operation_parameter (NodePtr const& op);

      virtual void visit_operation_parameter_pre (NodePtr const&)
      {
      }

      virtual void visit_operation_parameter_post (NodePtr const&)
      {
      }

    private:
      CCF::Traversal::Visitor* in_;
      CCF::Traversal::Visitor* out_;
      CCF::Traversal::Visitor* inout_;
    };

    //
    //
    //
    struct OperationDecl : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::OperationDeclPtr
      NodePtr;

      OperationDecl (CCF::Traversal::Visitor* type = 0)
          : type_ (type)
      {
      }


      virtual void visit_operation_decl (NodePtr const& n);

      virtual void visit_operation_decl_pre (NodePtr const& n);
      virtual void visit_operation_decl_type (NodePtr const& n);
      virtual void visit_operation_decl_name (NodePtr const& n);
      virtual void visit_operation_decl_parameters (NodePtr const& n);
      virtual void visit_operation_decl_post (NodePtr const& n);

    private:
      CCF::Traversal::Visitor* type_;
    };



    //
    //
    //
    struct FileScope : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::FileScopePtr
      NodePtr;

      virtual void visit_file_scope (NodePtr const& r);
      virtual void visit_file_scope_pre (NodePtr const& r);
      virtual void visit_file_scope_scope (NodePtr const& r);
      virtual void visit_file_scope_post (NodePtr const& r);
    };

    //
    //
    //
    struct TranslationRegion : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::TranslationRegionPtr
      NodePtr;

      virtual void visit_translation_region (NodePtr const& tr);
      virtual void visit_translation_region_pre (NodePtr const& tr);
      virtual void visit_translation_region_content (NodePtr const& tr);
      virtual void visit_translation_region_post (NodePtr const& tr);
    };


    //
    //
    //
    struct IncludeTranslationRegion : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::IncludeTranslationRegionPtr
      NodePtr;

      virtual void
      visit_include_translation_region (NodePtr const& n);

      virtual void
      visit_include_translation_region_pre (NodePtr const& n);

      virtual void
      visit_include_translation_region_content (NodePtr const& n);

      virtual void
      visit_include_translation_region_post (NodePtr const& n);
    };


    //
    //
    //
    struct SysIncludeTranslationRegion : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::SysIncludeTranslationRegionPtr
      NodePtr;

      virtual void
      visit_sys_include_translation_region (NodePtr const& n);

      virtual void
      visit_sys_include_translation_region_pre (NodePtr const& n);

      virtual void
      visit_sys_include_translation_region_content (NodePtr const& n);

      virtual void
      visit_sys_include_translation_region_post (NodePtr const& n);
    };


    //
    //
    //
    struct ImpliedIncludeTranslationRegion : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::ImpliedIncludeTranslationRegionPtr
      NodePtr;

      virtual void
      visit_implied_include_translation_region (NodePtr const& n);

      virtual void
      visit_implied_include_translation_region_pre (NodePtr const& n);

      virtual void
      visit_implied_include_translation_region_content (NodePtr const& n);

      virtual void
      visit_implied_include_translation_region_post (NodePtr const& n);
    };


    //
    //
    //
    struct PrincipalTranslationRegion : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::PrincipalTranslationRegionPtr
      NodePtr;

      virtual void
      visit_principal_translation_region (NodePtr const& n);

      virtual void
      visit_principal_translation_region_pre (NodePtr const& n);

      virtual void
      visit_principal_translation_region_content (NodePtr const& n);

      virtual void
      visit_principal_translation_region_post (NodePtr const& n);
    };


    //
    //
    //
    struct TranslationUnit : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::TranslationUnitPtr
      NodePtr;

      virtual void visit_translation_unit (NodePtr const& tu);
      virtual void visit_translation_unit_pre (NodePtr const& tu);
      virtual void visit_translation_unit_content (NodePtr const& tu);
      virtual void visit_translation_unit_post (NodePtr const& tu);
    };
  }
}

#endif // CCF_IDL2_TRAVERSAL_HPP
