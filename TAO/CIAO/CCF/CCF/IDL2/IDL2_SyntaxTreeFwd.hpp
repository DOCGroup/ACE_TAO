// $Id$
#ifndef CCF_IDL2_SYNTAX_TREE_FWD_HPP
#define CCF_IDL2_SYNTAX_TREE_FWD_HPP

#include "MSVC_Pragmas.hpp"

#include "CCF/CompilerElements/ReferenceCounting.hpp"

namespace IDL2
{
  namespace SyntaxTree
  {
    using ReferenceCounting::StrictPtr;

    class Comma;
    typedef
    StrictPtr<Comma>
    CommaPtr;

    class Declaration;
    typedef
    StrictPtr<Declaration>
    DeclarationPtr;

    class TypeDecl;
    typedef
    StrictPtr<TypeDecl>
    TypeDeclPtr;

    class TypeDef;
    typedef
    StrictPtr<TypeDef>
    TypeDefPtr;

    class BuiltInTypeDef;
    typedef
    StrictPtr<BuiltInTypeDef>
    BuiltInTypeDefPtr;

    class Void;
    typedef
    StrictPtr<Void>
    VoidPtr;

    class Long;
    typedef
    StrictPtr<Long>
    LongPtr;

    class Boolean;
    typedef
    StrictPtr<Boolean>
    BooleanPtr;

    class String;
    typedef
    StrictPtr<String>
    StringPtr;

    class Scope;
    typedef
    StrictPtr<Scope>
    ScopePtr;

    class Module;
    typedef
    StrictPtr<Module>
    ModulePtr;

    class InterfaceDecl;
    typedef
    StrictPtr<InterfaceDecl>
    InterfaceDeclPtr;

    class InterfaceDef;
    typedef
    StrictPtr<InterfaceDef>
    InterfaceDefPtr;

    class AbstractInterfaceDecl;
    typedef
    StrictPtr<AbstractInterfaceDecl>
    AbstractInterfaceDeclPtr;

    class AbstractInterfaceDef;
    typedef
    StrictPtr<AbstractInterfaceDef>
    AbstractInterfaceDefPtr;

    class LocalInterfaceDecl;
    typedef
    StrictPtr<LocalInterfaceDecl>
    LocalInterfaceDeclPtr;

    class LocalInterfaceDef;
    typedef
    StrictPtr<LocalInterfaceDef>
    LocalInterfaceDefPtr;

    class UnconstrainedInterfaceDecl;
    typedef
    StrictPtr<UnconstrainedInterfaceDecl>
    UnconstrainedInterfaceDeclPtr;

    class UnconstrainedInterfaceDef;
    typedef
    StrictPtr<UnconstrainedInterfaceDef>
    UnconstrainedInterfaceDefPtr;

    class AttributeDecl;
    typedef
    StrictPtr<AttributeDecl>
    AttributeDeclPtr;

    class OperationParameter;
    typedef
    StrictPtr<OperationParameter>
    OperationParameterPtr;

    class OperationDecl;
    typedef
    StrictPtr<OperationDecl>
    OperationDeclPtr;

    class FileScope;
    typedef
    StrictPtr<FileScope>
    FileScopePtr;

    class TranslationRegion;
    typedef
    StrictPtr<TranslationRegion>
    TranslationRegionPtr;

    class IncludeTranslationRegion;
    typedef
    StrictPtr<IncludeTranslationRegion>
    IncludeTranslationRegionPtr;

    class SysIncludeTranslationRegion;
    typedef
    StrictPtr<SysIncludeTranslationRegion>
    SysIncludeTranslationRegionPtr;

    class ImpliedIncludeTranslationRegion;
    typedef
    StrictPtr<ImpliedIncludeTranslationRegion>
    ImpliedIncludeTranslationRegionPtr;

    class PrincipalTranslationRegion;
    typedef
    StrictPtr<PrincipalTranslationRegion>
    PrincipalTranslationRegionPtr;

    class TranslationUnit;
    typedef
    StrictPtr<TranslationUnit>
    TranslationUnitPtr;
  }
}

#endif // CCF_IDL2_SYNTAX_TREE_FWD_HPP
