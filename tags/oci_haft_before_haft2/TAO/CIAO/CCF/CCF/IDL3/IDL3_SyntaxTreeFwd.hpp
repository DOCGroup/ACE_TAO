// $Id$
#ifndef CCF_IDL3_SYNTAX_TREE_FWD_HPP
#define CCF_IDL3_SYNTAX_TREE_FWD_HPP

#include "MSVC_Pragmas.hpp"

#include "CCF/IDL2/IDL2_SyntaxTreeFwd.hpp"

namespace IDL3
{
  namespace SyntaxTree
  {
    // Import all nodes of IDL2
    using namespace IDL2::SyntaxTree;

    class ComponentDecl;
    typedef
    StrictPtr<ComponentDecl>
    ComponentDeclPtr;

    class ComponentDef;
    typedef
    StrictPtr<ComponentDef>
    ComponentDefPtr;

    class ProvidesDecl;
    typedef
    StrictPtr<ProvidesDecl>
    ProvidesDeclPtr;

    class UsesDecl;
    typedef
    StrictPtr<UsesDecl>
    UsesDeclPtr;

    class PublishesDecl;
    typedef
    StrictPtr<PublishesDecl>
    PublishesDeclPtr;

    class EmitsDecl;
    typedef
    StrictPtr<EmitsDecl>
    EmitsDeclPtr;

    class ConsumesDecl;
    typedef
    StrictPtr<ConsumesDecl>
    ConsumesDeclPtr;

    class HomeDef;
    typedef
    StrictPtr<HomeDef>
    HomeDefPtr;

    class HomeFactoryDecl;
    typedef
    StrictPtr<HomeFactoryDecl>
    HomeFactoryDeclPtr;

    //@@ should it be here or in separate file?
    class ConcreteEventTypeDef;
    typedef
    StrictPtr<ConcreteEventTypeDef>
    ConcreteEventTypeDefPtr;
  }
}

#endif // CCF_IDL3_SYNTAX_TREE_FWD_HPP
