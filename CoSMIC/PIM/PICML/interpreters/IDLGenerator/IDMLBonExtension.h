#ifndef IDMLBONEXTENSION_H
#define IDMLBONEXTENSION_H

#include "BONImpl.h"
#include "Extensions.h"

namespace IDML
{
DECLARE_ABSTRACT_BONEXTENSION( BON::FCO, OrderableImpl, Orderable );
DECLARE_BONEXTENSION( BON::Folder, InterfaceDefinitionsImpl, InterfaceDefinitions );
DECLARE_BONEXTENSION( BON::Folder, PredefinedTypesImpl, PredefinedTypes );
DECLARE_ABSTRACT_BONEXTENSION( BON::Model, PrefixableImpl, Prefixable );
DECLARE_ABSTRACT_BONEXTENSION( BON::FCO, ConstantTypeImpl, ConstantType );
DECLARE_ABSTRACT_BONEXTENSION( BON::FCO, ManageableImpl, Manageable );
DECLARE_ABSTRACT_BONEXTENSION( BON::FCO, MemberTypeImpl, MemberType );
DECLARE_ABSTRACT_BONEXTENSION( BON::FCO, ProvideableImpl, Provideable );
DECLARE_ABSTRACT_BONEXTENSION( BON::FCO, TaggableImpl, Taggable );
DECLARE_BONEXTENSION3( BON::Model, Orderable, Taggable, ExceptionImpl, Exception );
DECLARE_BONEXTENSION2( Orderable, Prefixable, FileImpl, File );
DECLARE_ABSTRACT_BONEXTENSION3( BON::Model, Orderable, Taggable, OperationBaseImpl, OperationBase );
DECLARE_BONEXTENSION3( Orderable, Taggable, Prefixable, PackageImpl, Package );
DECLARE_BONEXTENSION3( BON::Model, Orderable, Taggable, ReadonlyAttributeImpl, ReadonlyAttribute );
DECLARE_ABSTRACT_BONEXTENSION2( Taggable, MemberType, NamedTypeImpl, NamedType );
DECLARE_ABSTRACT_BONEXTENSION( Taggable, PortImpl, Port );
DECLARE_ABSTRACT_BONEXTENSION2( MemberType, ConstantType, PredefinedTypeImpl, PredefinedType );
DECLARE_BONEXTENSION2( BON::Reference, Manageable, ComponentRefImpl, ComponentRef );
DECLARE_BONEXTENSION3( BON::Reference, Orderable, Taggable, ConstantImpl, Constant );
DECLARE_BONEXTENSION( ReadonlyAttribute, AttributeImpl, Attribute );
DECLARE_ABSTRACT_BONEXTENSION( OperationBase, HasExceptionsImpl, HasExceptions );
DECLARE_ABSTRACT_BONEXTENSION3( BON::Model, Orderable, NamedType, InheritableImpl, Inheritable );
DECLARE_BONEXTENSION( OperationBase, OnewayOperationImpl, OnewayOperation );
DECLARE_ABSTRACT_BONEXTENSION2( Orderable, NamedType, NoInheritableImpl, NoInheritable );
DECLARE_BONEXTENSION2( BON::Atom, PredefinedType, BooleanImpl, Boolean );
DECLARE_BONEXTENSION2( BON::Atom, PredefinedType, ByteImpl, Byte );
DECLARE_BONEXTENSION3( BON::Atom, PredefinedType, Provideable, GenericObjectImpl, GenericObject );
DECLARE_BONEXTENSION2( BON::Atom, PredefinedType, GenericValueImpl, GenericValue );
DECLARE_BONEXTENSION2( BON::Atom, PredefinedType, GenericValueObjectImpl, GenericValueObject );
DECLARE_BONEXTENSION2( BON::Atom, PredefinedType, LongIntegerImpl, LongInteger );
DECLARE_BONEXTENSION2( BON::Atom, PredefinedType, RealNumberImpl, RealNumber );
DECLARE_BONEXTENSION2( BON::Atom, PredefinedType, ShortIntegerImpl, ShortInteger );
DECLARE_BONEXTENSION2( BON::Atom, PredefinedType, StringImpl, String );
DECLARE_BONEXTENSION2( BON::Atom, PredefinedType, TypeEncodingImpl, TypeEncoding );
DECLARE_BONEXTENSION2( BON::Atom, PredefinedType, TypeKindImpl, TypeKind );
DECLARE_BONEXTENSION3( BON::Reference, Orderable, NamedType, BoxedImpl, Boxed );
DECLARE_BONEXTENSION2( BON::Reference, Port, InEventPortImpl, InEventPort );
DECLARE_BONEXTENSION2( BON::Reference, Port, OutEventPortImpl, OutEventPort );
DECLARE_BONEXTENSION2( BON::Reference, Port, ProvidedRequestPortImpl, ProvidedRequestPort );
DECLARE_BONEXTENSION2( BON::Reference, Port, RequiredRequestPortImpl, RequiredRequestPort );
DECLARE_BONEXTENSION2( BON::Model, NoInheritable, AggregateImpl, Aggregate );
DECLARE_BONEXTENSION3( BON::Model, NoInheritable, ConstantType, EnumImpl, Enum );
DECLARE_BONEXTENSION( HasExceptions, FactoryOperationImpl, FactoryOperation );
DECLARE_ABSTRACT_BONEXTENSION( Inheritable, HasOperationsImpl, HasOperations );
DECLARE_BONEXTENSION( HasExceptions, LookupOperationImpl, LookupOperation );
DECLARE_ABSTRACT_BONEXTENSION2( BON::Model, Orderable, SupportsInterfacesImpl, SupportsInterfaces );
DECLARE_BONEXTENSION2( BON::Model, NoInheritable, SwitchedAggregateImpl, SwitchedAggregate );
DECLARE_BONEXTENSION( HasExceptions, TwowayOperationImpl, TwowayOperation );
DECLARE_BONEXTENSION2( BON::Reference, NoInheritable, AliasImpl, Alias );
DECLARE_BONEXTENSION2( BON::Reference, NoInheritable, CollectionImpl, Collection );
DECLARE_BONEXTENSION3( NamedType, SupportsInterfaces, Manageable, ComponentImpl, Component );
DECLARE_BONEXTENSION2( SupportsInterfaces, HasOperations, ComponentFactoryImpl, ComponentFactory );
DECLARE_BONEXTENSION3( HasOperations, Provideable, Prefixable, ObjectImpl, Object );
DECLARE_ABSTRACT_BONEXTENSION3( Prefixable, SupportsInterfaces, HasOperations, ObjectByValueImpl, ObjectByValue );
DECLARE_BONEXTENSION( ObjectByValue, EventImpl, Event );
DECLARE_BONEXTENSION( ObjectByValue, ValueObjectImpl, ValueObject );
DECLARE_BONEXTENSION( BON::Atom, EnumValueImpl, EnumValue );
DECLARE_BONEXTENSION( BON::Atom, LabelImpl, Label );
DECLARE_BONEXTENSION( BON::Atom, PrivateFlagImpl, PrivateFlag );
DECLARE_BONEXTENSION( BON::Connection, LabelConnectionImpl, LabelConnection );
DECLARE_BONEXTENSION( BON::Connection, MakeMemberPrivateImpl, MakeMemberPrivate );
DECLARE_BONEXTENSION( BON::Connection, ManagesComponentImpl, ManagesComponent );
DECLARE_BONEXTENSION( BON::Reference, AttributeMemberImpl, AttributeMember );
DECLARE_BONEXTENSION( BON::Reference, DiscriminatorImpl, Discriminator );
DECLARE_BONEXTENSION( BON::Reference, ExceptionRefImpl, ExceptionRef );
DECLARE_BONEXTENSION( BON::Reference, FileRefImpl, FileRef );
DECLARE_BONEXTENSION( BON::Reference, GetExceptionImpl, GetException );
DECLARE_BONEXTENSION( BON::Reference, InParameterImpl, InParameter );
DECLARE_BONEXTENSION( BON::Reference, InheritsImpl, Inherits );
DECLARE_BONEXTENSION( BON::Reference, InoutParameterImpl, InoutParameter );
DECLARE_BONEXTENSION( BON::Reference, LookupKeyImpl, LookupKey );
DECLARE_BONEXTENSION( BON::Reference, MemberImpl, Member );
DECLARE_BONEXTENSION( BON::Reference, OutParameterImpl, OutParameter );
DECLARE_BONEXTENSION( BON::Reference, ReturnTypeImpl, ReturnType );
DECLARE_BONEXTENSION( BON::Reference, SetExceptionImpl, SetException );
DECLARE_BONEXTENSION( BON::Reference, SupportsImpl, Supports );

//*******************************************************************
//   C  L  A  S  S   OrderableImpl
//*******************************************************************
class OrderableImpl : virtual public BON::FCOImpl
{
public:
  virtual void initialize (void);

  std::set<Orderable> depends_on_me;
  std::set<std::pair<Orderable, int> > fwd_decl_children;
  std::vector<Orderable> ordered_children;
  
  bool fwd_declared_;
};

//*******************************************************************
//   C  L  A  S  S   InterfaceDefinitionsImpl
//*******************************************************************
class InterfaceDefinitionsImpl :
  virtual public BON::FolderImpl
{
public:
	//
	// kind and subfolder getters
	virtual std::set<File>                  getFile();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PredefinedTypesImpl
//*******************************************************************
class PredefinedTypesImpl :
  virtual public BON::FolderImpl
{
public:
	//
	// kind and subfolder getters
	virtual std::set<Boolean>               getBoolean();
	virtual std::set<Byte>                  getByte();
	virtual std::set<GenericObject>         getGenericObject();
	virtual std::set<GenericValue>          getGenericValue();
	virtual std::set<GenericValueObject>    getGenericValueObject();
	virtual std::set<LongInteger>           getLongInteger();
	virtual std::set<PredefinedType>        getPredefinedType();
	virtual std::set<RealNumber>            getRealNumber();
	virtual std::set<ShortInteger>          getShortInteger();
	virtual std::set<String>                getString();
	virtual std::set<TypeEncoding>          getTypeEncoding();
	virtual std::set<TypeKind>              getTypeKind();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PrefixableImpl
//*******************************************************************
class PrefixableImpl :
  virtual public BON::ModelImpl
{
public:
	//
	// attribute getters
	virtual std::string getPrefixTag();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ConstantTypeImpl
//*******************************************************************
class ConstantTypeImpl :
  virtual public BON::FCOImpl 
{
public:

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ManageableImpl
//*******************************************************************
class ManageableImpl :
  virtual public BON::FCOImpl 
{
public:
	//
	// connectionEnd getters
	virtual std::set<ManagesComponent>      getInManagesComponentLinks();
	virtual std::multiset<ComponentFactory> getManagesComponentSrcs();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   MemberTypeImpl
//*******************************************************************
class MemberTypeImpl :
  virtual public BON::FCOImpl 
{
public:

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ProvideableImpl
//*******************************************************************
class ProvideableImpl :
  virtual public BON::FCOImpl 
{
public:

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   TaggableImpl
//*******************************************************************
class TaggableImpl :
  virtual public BON::FCOImpl 
{
public:
	//
	// attribute getters
	virtual std::string getSpecifyIdTag();
	virtual std::string getVersionTag();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ExceptionImpl
//*******************************************************************
class ExceptionImpl :
  virtual public BON::ModelImpl,
  virtual public OrderableImpl,
  virtual public TaggableImpl 
{
public:
	//
	// kind and role getters
	virtual std::set<Member>                getMember();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   FileImpl
//*******************************************************************
class FileImpl :
  virtual public OrderableImpl,
  virtual public PrefixableImpl 
{
public:
	//
	// kind and role getters
	virtual std::set<Aggregate>             getAggregate();
	virtual std::set<Alias>                 getAlias();
	virtual std::set<Boxed>                 getBoxed();
	virtual std::set<Collection>            getCollection();
	virtual std::set<Component>             getComponent();
	virtual std::set<ComponentFactory>      getComponentFactory();
	virtual std::set<ComponentRef>          getComponentRef();
	virtual std::set<Constant>              getConstant();
	virtual std::set<Enum>                  getEnum();
	virtual std::set<Event>                 getEvent();
	virtual std::set<Exception>             getException();
	virtual std::set<FileRef>               getFileRef();
	virtual std::set<ManagesComponent>      getManagesComponent();
	virtual std::set<NamedType>             getNamedType();
	virtual std::set<Object>                getObject();
	virtual std::set<Package>               getPackage();
	virtual std::set<SwitchedAggregate>     getSwitchedAggregate();
	virtual std::set<ValueObject>           getValueObject();
  
  bool include_components_idl () const;
  void include_components_idl (bool val);
  
  bool include_orb_idl () const;
  void include_orb_idl (bool val);
  
  virtual void initialize ();
  
public:
  std::set<File> discovered_includes_;

private:
  bool include_components_idl_;
  bool include_orb_idl_;
};


//*******************************************************************
//   C  L  A  S  S   OperationBaseImpl
//*******************************************************************
class OperationBaseImpl :
  virtual public BON::ModelImpl,
  virtual public OrderableImpl,
  virtual public TaggableImpl 
{
public:
	//
	// kind and role getters
	virtual std::set<InParameter>           getInParameter();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PackageImpl
//*******************************************************************
class PackageImpl :
  virtual public OrderableImpl,
  virtual public TaggableImpl,
  virtual public PrefixableImpl 
{
public:
	//
	// kind and role getters
	virtual std::set<Aggregate>             getAggregate();
	virtual std::set<Alias>                 getAlias();
	virtual std::set<Boxed>                 getBoxed();
	virtual std::set<Collection>            getCollection();
	virtual std::set<Component>             getComponent();
	virtual std::set<ComponentFactory>      getComponentFactory();
	virtual std::set<ComponentRef>          getComponentRef();
	virtual std::set<Constant>              getConstant();
	virtual std::set<Enum>                  getEnum();
	virtual std::set<Event>                 getEvent();
	virtual std::set<Exception>             getException();
	virtual std::set<ManagesComponent>      getManagesComponent();
	virtual std::set<NamedType>             getNamedType();
	virtual std::set<Object>                getObject();
	virtual std::set<Package>               getPackage();
	virtual std::set<SwitchedAggregate>     getSwitchedAggregate();
	virtual std::set<ValueObject>           getValueObject();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ReadonlyAttributeImpl
//*******************************************************************
class ReadonlyAttributeImpl :
  virtual public BON::ModelImpl,
  virtual public OrderableImpl,
  virtual public TaggableImpl 
{
public:
	//
	// kind and role getters
	virtual std::set<AttributeMember>       getAttributeMember();
	virtual std::set<GetException>          getGetException();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   NamedTypeImpl
//*******************************************************************
class NamedTypeImpl :
  virtual public TaggableImpl,
  virtual public MemberTypeImpl 
{
public:

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PortImpl
//*******************************************************************
class PortImpl :
  virtual public TaggableImpl 
{
public:

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PredefinedTypeImpl
//*******************************************************************
class PredefinedTypeImpl :
  virtual public MemberTypeImpl,
  virtual public ConstantTypeImpl 
{
public:

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ComponentRefImpl
//*******************************************************************
class ComponentRefImpl :
  virtual public BON::ReferenceImpl,
  virtual public ManageableImpl 
{
public:
	//
	// ref getters
	virtual Component   getComponent();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ConstantImpl
//*******************************************************************
class ConstantImpl :
  virtual public BON::ReferenceImpl,
  virtual public OrderableImpl,
  virtual public TaggableImpl 
{
public:
	//
	// attribute getters
	virtual std::string getvalue();
	//
	// ref getters
	virtual ConstantType                    getConstantType();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   AttributeImpl
//*******************************************************************
class AttributeImpl :
  virtual public ReadonlyAttributeImpl 
{
public:
	//
	// kind and role getters
	virtual std::set<SetException>          getSetException();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   HasExceptionsImpl
//*******************************************************************
class HasExceptionsImpl :
  virtual public OperationBaseImpl 
{
public:
	//
	// kind and role getters
	virtual std::set<ExceptionRef>          getExceptionRef();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   InheritableImpl
//*******************************************************************
class InheritableImpl :
  virtual public BON::ModelImpl,
  virtual public OrderableImpl,
  virtual public NamedTypeImpl 
{
public:
	//
	// kind and role getters
	virtual std::set<Attribute>             getAttribute();
	virtual std::set<Inherits>              getInherits();
	virtual std::set<ReadonlyAttribute>     getReadonlyAttribute();
	virtual std::set<ReadonlyAttribute>     getReadonlyAttribute(int dummy);

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   OnewayOperationImpl
//*******************************************************************
class OnewayOperationImpl :
  virtual public OperationBaseImpl 
{
public:

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   NoInheritableImpl
//*******************************************************************
class NoInheritableImpl :
  virtual public OrderableImpl,
  virtual public NamedTypeImpl
{
public:

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   BooleanImpl
//*******************************************************************
class BooleanImpl :
  virtual public BON::AtomImpl,
  virtual public PredefinedTypeImpl 
{
public:

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ByteImpl
//*******************************************************************
class ByteImpl :
  virtual public BON::AtomImpl,
  virtual public PredefinedTypeImpl 
{
public:

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   GenericObjectImpl
//*******************************************************************
class GenericObjectImpl :
  virtual public BON::AtomImpl,
  virtual public PredefinedTypeImpl,
  virtual public ProvideableImpl 
{
public:

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   GenericValueImpl
//*******************************************************************
class GenericValueImpl :
  virtual public BON::AtomImpl,
  virtual public PredefinedTypeImpl 
{
public:

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   GenericValueObjectImpl
//*******************************************************************
class GenericValueObjectImpl :
  virtual public BON::AtomImpl,
  virtual public PredefinedTypeImpl 
{
public:

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   LongIntegerImpl
//*******************************************************************
class LongIntegerImpl :
  virtual public BON::AtomImpl,
  virtual public PredefinedTypeImpl 
{
public:

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   RealNumberImpl
//*******************************************************************
class RealNumberImpl :
  virtual public BON::AtomImpl,
  virtual public PredefinedTypeImpl 
{
public:

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ShortIntegerImpl
//*******************************************************************
class ShortIntegerImpl :
  virtual public BON::AtomImpl,
  virtual public PredefinedTypeImpl 
{
public:

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   StringImpl
//*******************************************************************
class StringImpl :
  virtual public BON::AtomImpl,
  virtual public PredefinedTypeImpl 
{
public:

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   TypeEncodingImpl
//*******************************************************************
class TypeEncodingImpl :
  virtual public BON::AtomImpl,
  virtual public PredefinedTypeImpl 
{
public:

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   TypeKindImpl
//*******************************************************************
class TypeKindImpl :
  virtual public BON::AtomImpl,
  virtual public PredefinedTypeImpl 
{
public:

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   BoxedImpl
//*******************************************************************
class BoxedImpl :
  virtual public BON::ReferenceImpl,
  virtual public OrderableImpl,
  virtual public NamedTypeImpl 
{
public:
	//
	// ref getters
	virtual MemberType  getMemberType();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   InEventPortImpl
//*******************************************************************
class InEventPortImpl :
  virtual public BON::ReferenceImpl,
  virtual public PortImpl 
{
public:
	//
	// ref getters
	virtual Event       getEvent();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   OutEventPortImpl
//*******************************************************************
class OutEventPortImpl :
  virtual public BON::ReferenceImpl,
  virtual public PortImpl 
{
public:
	//
	// attribute getters
	virtual bool        issingle_destination();
	//
	// ref getters
	virtual Event       getEvent();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ProvidedRequestPortImpl
//*******************************************************************
class ProvidedRequestPortImpl :
  virtual public BON::ReferenceImpl,
  virtual public PortImpl 
{
public:
	//
	// ref getters
	virtual Provideable getProvideable();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   RequiredRequestPortImpl
//*******************************************************************
class RequiredRequestPortImpl :
  virtual public BON::ReferenceImpl,
  virtual public PortImpl 
{
public:
	//
	// attribute getters
	virtual bool        ismultiple_connections();
	//
	// ref getters
	virtual Provideable getProvideable();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   AggregateImpl
//*******************************************************************
class AggregateImpl :
  virtual public BON::ModelImpl,
  virtual public NoInheritableImpl 
{
public:
	//
	// kind and role getters
	virtual std::set<Member>                getMember();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   EnumImpl
//*******************************************************************
class EnumImpl :
  virtual public BON::ModelImpl,
  virtual public NoInheritableImpl,
  virtual public ConstantTypeImpl 
{
public:
	//
	// kind and role getters
	virtual std::set<EnumValue>             getEnumValue();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   FactoryOperationImpl
//*******************************************************************
class FactoryOperationImpl :
  virtual public HasExceptionsImpl 
{
public:

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   HasOperationsImpl
//*******************************************************************
class HasOperationsImpl :
  virtual public InheritableImpl 
{
public:
	//
	// kind and role getters
	virtual std::set<Aggregate>             getAggregate();
	virtual std::set<Alias>                 getAlias();
	virtual std::set<Collection>            getCollection();
	virtual std::set<Constant>              getConstant();
	virtual std::set<Enum>                  getEnum();
	virtual std::set<Exception>             getException();
	virtual std::set<NoInheritable>         getNoInheritable();
	virtual std::set<OnewayOperation>       getOnewayOperation();
	virtual std::set<SwitchedAggregate>     getSwitchedAggregate();
	virtual std::set<TwowayOperation>       getTwowayOperation();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   LookupOperationImpl
//*******************************************************************
class LookupOperationImpl :
  virtual public HasExceptionsImpl 
{
public:

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   SupportsInterfacesImpl
//*******************************************************************
class SupportsInterfacesImpl :
  virtual public BON::ModelImpl,
  virtual public OrderableImpl
{
public:
	//
	// kind and role getters
	virtual std::set<Supports>              getSupports();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   SwitchedAggregateImpl
//*******************************************************************
class SwitchedAggregateImpl :
  virtual public BON::ModelImpl,
  virtual public NoInheritableImpl 
{
public:
	//
	// kind and role getters
	virtual std::set<Discriminator>         getDiscriminator();
	virtual std::set<Label>                 getLabel();
	virtual std::set<LabelConnection>       getLabelConnection();
	virtual std::set<Member>                getMember();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   TwowayOperationImpl
//*******************************************************************
class TwowayOperationImpl :
  virtual public HasExceptionsImpl 
{
public:
	//
	// kind and role getters
	virtual std::set<InoutParameter>        getInoutParameter();
	virtual std::set<OutParameter>          getOutParameter();
	virtual std::set<ReturnType>            getReturnType();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   AliasImpl
//*******************************************************************
class AliasImpl :
  virtual public BON::ReferenceImpl,
  virtual public NoInheritableImpl 
{
public:
	//
	// ref getters
	virtual MemberType  getMemberType();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   CollectionImpl
//*******************************************************************
class CollectionImpl :
  virtual public BON::ReferenceImpl,
  virtual public NoInheritableImpl 
{
public:
	//
	// ref getters
	virtual MemberType  getMemberType();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ComponentImpl
//*******************************************************************
class ComponentImpl :
  virtual public NamedTypeImpl,
  virtual public SupportsInterfacesImpl,
  virtual public ManageableImpl
{
public:
	//
	// kind and role getters
	virtual std::set<InEventPort>           getInEventPort();
	virtual std::set<OutEventPort>          getOutEventPort();
	virtual std::set<Port>                  getPort();
	virtual std::set<ProvidedRequestPort>   getProvidedRequestPort();
	virtual std::set<RequiredRequestPort>   getRequiredRequestPort();

	bool ref_managed () const;
	void ref_managed (bool val);
	
	Orderable base_component () const;
	void base_component (const Orderable &base);
	
	virtual void initialize ();
	
private:
  bool ref_managed_;
  Orderable base_component_;
};


//*******************************************************************
//   C  L  A  S  S   ComponentFactoryImpl
//*******************************************************************
class ComponentFactoryImpl :
  virtual public SupportsInterfacesImpl,
  virtual public HasOperationsImpl 
{
public:
	//
	// connectionEnd getters
	virtual std::multiset<Manageable>       getManagesComponentDsts();
	virtual std::set<ManagesComponent>      getOutManagesComponentLinks();
	//
	// kind and role getters
	virtual std::set<FactoryOperation>      getFactoryOperation();
	virtual std::set<LookupKey>             getLookupKey();
	virtual std::set<LookupOperation>       getLookupOperation();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ObjectImpl
//*******************************************************************
class ObjectImpl :
  virtual public HasOperationsImpl,
  virtual public ProvideableImpl,
  virtual public PrefixableImpl 
{
public:
	//
	// attribute getters
	virtual bool        isabstract();
	virtual bool        islocal();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ObjectByValueImpl
//*******************************************************************
class ObjectByValueImpl :
  virtual public PrefixableImpl,
  virtual public SupportsInterfacesImpl,
  virtual public HasOperationsImpl 
{
public:
	//
	// attribute getters
	virtual bool        isabstract();
	//
	// kind and role getters
	virtual std::set<FactoryOperation>      getFactoryOperation();
	virtual std::set<MakeMemberPrivate>     getMakeMemberPrivate();
	virtual std::set<Member>                getMember();
	virtual std::set<PrivateFlag>           getPrivateFlag();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   EventImpl
//*******************************************************************
class EventImpl :
  virtual public ObjectByValueImpl 
{
public:

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ValueObjectImpl
//*******************************************************************
class ValueObjectImpl :
  virtual public ObjectByValueImpl 
{
public:

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   EnumValueImpl
//*******************************************************************
class EnumValueImpl :
  virtual public BON::AtomImpl 
{
public:

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   LabelImpl
//*******************************************************************
class LabelImpl :
  virtual public BON::AtomImpl 
{
public:
	//
	// connectionEnd getters
	virtual std::set<LabelConnection>       getInLabelConnectionLinks();
	virtual std::multiset<Member>           getLabelConnectionSrcs();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PrivateFlagImpl
//*******************************************************************
class PrivateFlagImpl :
  virtual public BON::AtomImpl 
{
public:
	//
	// connectionEnd getters
	virtual std::set<MakeMemberPrivate>     getInMakeMemberPrivateLinks();
	virtual std::multiset<Member>           getMakeMemberPrivateSrcs();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   LabelConnectionImpl
//*******************************************************************
class LabelConnectionImpl :
  virtual public BON::ConnectionImpl 
{
public:
	//
	// connectionEnd getters
	virtual Label       getDst();
	virtual Member      getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   MakeMemberPrivateImpl
//*******************************************************************
class MakeMemberPrivateImpl :
  virtual public BON::ConnectionImpl 
{
public:
	//
	// connectionEnd getters
	virtual PrivateFlag getDst();
	virtual Member      getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ManagesComponentImpl
//*******************************************************************
class ManagesComponentImpl :
  virtual public BON::ConnectionImpl 
{
public:
	//
	// connectionEnd getters
	virtual Manageable  getDst();
	virtual ComponentFactory                getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   AttributeMemberImpl
//*******************************************************************
class AttributeMemberImpl :
  virtual public BON::ReferenceImpl 
{
public:
	//
	// ref getters
	virtual MemberType  getMemberType();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   DiscriminatorImpl
//*******************************************************************
class DiscriminatorImpl :
  virtual public BON::ReferenceImpl 
{
public:
	//
	// ref getters
	virtual ConstantType                    getConstantType();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ExceptionRefImpl
//*******************************************************************
class ExceptionRefImpl :
  virtual public BON::ReferenceImpl 
{
public:
	//
	// ref getters
	virtual Exception   getException();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   FileRefImpl
//*******************************************************************
class FileRefImpl :
  virtual public BON::ReferenceImpl 
{
public:
	//
	// ref getters
	virtual File        getFile();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   GetExceptionImpl
//*******************************************************************
class GetExceptionImpl :
  virtual public BON::ReferenceImpl 
{
public:
	//
	// ref getters
	virtual Exception   getException();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   InParameterImpl
//*******************************************************************
class InParameterImpl :
  virtual public BON::ReferenceImpl 
{
public:
	//
	// ref getters
	virtual MemberType  getMemberType();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   InheritsImpl
//*******************************************************************
class InheritsImpl :
  virtual public BON::ReferenceImpl 
{
public:
	//
	// ref getters
	virtual Inheritable getInheritable();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   InoutParameterImpl
//*******************************************************************
class InoutParameterImpl :
  virtual public BON::ReferenceImpl 
{
public:
	//
	// ref getters
	virtual MemberType  getMemberType();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   LookupKeyImpl
//*******************************************************************
class LookupKeyImpl :
  virtual public BON::ReferenceImpl 
{
public:
	//
	// ref getters
	virtual ValueObject getValueObject();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   MemberImpl
//*******************************************************************
class MemberImpl :
  virtual public BON::ReferenceImpl 
{
public:
	//
	// connectionEnd getters
	virtual std::multiset<Label>            getLabelConnectionDsts();
	virtual std::multiset<PrivateFlag>      getMakeMemberPrivateDsts();
	virtual std::set<LabelConnection>       getOutLabelConnectionLinks();
	virtual std::set<MakeMemberPrivate>     getOutMakeMemberPrivateLinks();
	//
	// ref getters
	virtual MemberType  getMemberType();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   OutParameterImpl
//*******************************************************************
class OutParameterImpl :
  virtual public BON::ReferenceImpl 
{
public:
	//
	// ref getters
	virtual MemberType  getMemberType();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ReturnTypeImpl
//*******************************************************************
class ReturnTypeImpl :
  virtual public BON::ReferenceImpl 
{
public:
	//
	// ref getters
	virtual MemberType  getMemberType();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   SetExceptionImpl
//*******************************************************************
class SetExceptionImpl :
  virtual public BON::ReferenceImpl 
{
public:
	//
	// ref getters
	virtual Exception   getException();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   SupportsImpl
//*******************************************************************
class SupportsImpl :
  virtual public BON::ReferenceImpl 
{
public:
	//
	// ref getters
	virtual Object      getObject();

	///BUP
	// add your own members here
	///EUP
};



}; // namespace IDML

///BUP
// add your additional class definitions here
///EUP

#endif // IDMLBONEXTENSION_H
