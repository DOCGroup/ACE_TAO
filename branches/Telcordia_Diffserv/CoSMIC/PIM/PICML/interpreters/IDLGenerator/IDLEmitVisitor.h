#ifndef IDLEMITVISITOR_H
#define IDLEMITVISITOR_H

#include "IDMLBonExtension.h"
#include "IDLStream.h"

namespace IDML
{
  class IDLEmitVisitor	: public BON::Visitor
  {
	public :
	  IDLEmitVisitor( std::ostream& strm );
  	
		void visitObjectImpl( const BON::Object& object );
		void visitFCOImpl( const BON::FCO& fco );
		void visitAtomImpl( const BON::Atom& atom );
		void visitReferenceImpl( const BON::Reference& ref );
		void visitConnectionImpl( const BON::Connection& conn);
		void visitModelImpl( const BON::Model& model );
		void visitFolderImpl( const BON::Folder& fold );
		void visitOrderableImpl( const Orderable& object );
		
	private :
		bool visitBoolean( const Boolean& object );
		bool visitByte( const Byte& object );
		bool visitEnumValue( const EnumValue& object );
		bool visitGenericObject( const GenericObject& object );
		bool visitGenericValue( const GenericValue& object );
		bool visitGenericValueObject( const GenericValueObject& object );
		bool visitLabel( const Label& object );
		bool visitLongInteger( const LongInteger& object );
		bool visitPrivateFlag( const PrivateFlag& object );
		bool visitRealNumber( const RealNumber& object );
		bool visitShortInteger( const ShortInteger& object );
		bool visitString( const String& object );
		bool visitTypeEncoding( const TypeEncoding& object );
		bool visitTypeKind( const TypeKind& object );
		bool visitAggregate( const Aggregate& object );
		bool visitAttribute( const Attribute& object );
		bool visitComponent( const Component& object );
		bool visitComponentFactory( const ComponentFactory& object );
		bool visitEnum( const Enum& object );
		bool visitEvent( const Event& object );
		bool visitException( const Exception& object );
		bool visitFactoryOperation( const FactoryOperation& object );
		bool visitFile( const File& object );
		bool visitHasExceptions( const HasExceptions& object );
		bool visitHasOperations( const HasOperations& object );
		bool visitInheritable( const Inheritable& object );
		bool visitLookupOperation( const LookupOperation& object );
		bool visitObject( const Object& object );
		bool visitObjectByValue( const ObjectByValue& object );
		bool visitOnewayOperation( const OnewayOperation& object );
		bool visitOperationBase( const OperationBase& object );
		bool visitPackage( const Package& object );
		bool visitPrefixable( const Prefixable& object );
		bool visitReadonlyAttribute( const ReadonlyAttribute& object );
		bool visitSupportsInterfaces( const SupportsInterfaces& object );
		bool visitSwitchedAggregate( const SwitchedAggregate& object );
		bool visitTwowayOperation( const TwowayOperation& object );
		bool visitValueObject( const ValueObject& object );
		bool visitLabelConnection( const LabelConnection& object );
		bool visitMakeMemberPrivate( const MakeMemberPrivate& object );
		bool visitManagesComponent( const ManagesComponent& object );
		bool visitAlias( const Alias& object );
		bool visitAttributeMember( const AttributeMember& object );
		bool visitBoxed( const Boxed& object );
		bool visitCollection( const Collection& object );
		bool visitComponentRef( const ComponentRef& object );
		bool visitConstant( const Constant& object );
		bool visitDiscriminator( const Discriminator& object );
		bool visitExceptionRef( const ExceptionRef& object );
		bool visitFileRef( const FileRef& object );
		bool visitGetException( const GetException& object );
		bool visitInEventPort( const InEventPort& object );
		bool visitInParameter( const InParameter& object );
		bool visitInherits( const Inherits& object );
		bool visitInoutParameter( const InoutParameter& object );
		bool visitLookupKey( const LookupKey& object );
		bool visitMember( const Member& object );
		bool visitOutEventPort( const OutEventPort& object );
		bool visitOutParameter( const OutParameter& object );
		bool visitProvidedRequestPort( const ProvidedRequestPort& object );
		bool visitRequiredRequestPort( const RequiredRequestPort& object );
		bool visitReturnType( const ReturnType& object );
		bool visitSetException( const SetException& object );
		bool visitSupports( const Supports& object );
		bool visitConstantType( const ConstantType& object );
		bool visitManageable( const Manageable& object );
		bool visitMemberType( const MemberType& object );
		bool visitNamedType( const NamedType& object );
		bool visitNoInheritable( const NoInheritable& object );
		bool visitPort( const Port& object );
		bool visitPredefinedType( const PredefinedType& object );
		bool visitTaggable( const Taggable& object );
		bool visitInterfaceDefinitions( const InterfaceDefinitions& object );
		bool visitPredefinedTypes( const PredefinedTypes& object );
  
    void order_children( const Orderable& object );
    std::string scoped_name( const BON::FCO& object );
    std::string basic_name( const PredefinedType& pdt );
    std::string label_scope_prefix( const Discriminator& d );
    bool is_included_predefined_sequence( const Collection& c);
    
    void emitIncludedFiles( const File& f );
    void emitPrefix( const File& f);
	  void emitFwdDecls( const Orderable& object );
	  void emitFwdDeclNested( const Orderable& object, int level );
	  void visitChildren( const Orderable& object );
    void emitPreprocDirectives(const Taggable& t );
	  void emitStructMembers( const Orderable& object );
	  void emitUnionMembers (const SwitchedAggregate& s);
	  void emitAttributeMember( const ReadonlyAttribute& ra );
	  void emitMemberType( const NamedType& nt );
	  void emitAliasMemberType( const NamedType& nt );
	  void emitConstantType( const Constant& c );
	  void emitInherits( const Inheritable& object );
	  void emitComponentInherits( const Component& comp );
	  bool emitOBVInheritsConcrete( std::set<Inherits>& parents );
	  void emitReturnType( const TwowayOperation& op );
	  void emitParameters( const HasExceptions& he );
	  void emitInParameters( const OperationBase& op );
	  void emitExceptions( const HasExceptions& he );
	  void emitGetExceptions (const ReadonlyAttribute& r,
	                          bool readonly);
	  void emitSetExceptions (const Attribute& a);
	  void emitSupports( const SupportsInterfaces& s );
	  void emitPorts( const Component& c );
	  void emitDefaultHome( const Component& c );
	  void emitManages( const ComponentFactory& cf );
	  void emitLookupKey( const ComponentFactory& cf );
	  void emitOBVMembers( const ObjectByValue& obv );
	  void emitEnumValues( const Enum& e );
	  void emitDiscriminator( const SwitchedAggregate& sa );
	  bool emitPredefinedSequence( const MemberType& m );

  private:
    IDLStream ofs;
  };
} // namespace

#endif // IDLEMITVISITOR_H
