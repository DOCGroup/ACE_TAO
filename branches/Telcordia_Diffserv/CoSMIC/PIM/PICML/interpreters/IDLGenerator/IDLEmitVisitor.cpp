#include "stdafx.h"
#include "IDLEmitVisitor.h"
#include <algorithm>

#define EMITS_DEBUG 0

#if (EMITS_DEBUG == 1)
#include <fstream>
#endif

const char *IDL_INCLUDE_SUFFIX = "_from_IDL_include";
size_t IDL_INCLUDE_SUFFIX_LEN = strlen (IDL_INCLUDE_SUFFIX);

namespace IDML
{
  IDLEmitVisitor::IDLEmitVisitor( std::ostream& strm )
    : ofs (strm)
  {
  }

  bool IDLEmitVisitor::visitBoolean( const Boolean& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitByte( const Byte& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitEnumValue( const EnumValue& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitGenericObject( const GenericObject& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitGenericValue( const GenericValue& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitGenericValueObject (
      const GenericValueObject& object
    )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitLabel( const Label& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitLongInteger( const LongInteger& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitPrivateFlag( const PrivateFlag& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitRealNumber( const RealNumber& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitShortInteger( const ShortInteger& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitString( const String& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitTypeEncoding( const TypeEncoding& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitTypeKind( const TypeKind& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitAggregate( const Aggregate& object )
  {
    if (!object) return false;

    ofs << nl
        << "struct " << object->getName () << nl
        << "{" << idt;

    this->emitPreprocDirectives (object);
    this->emitStructMembers (object);

    ofs << uidt_nl
        << "};";

    return true;
  }


  bool IDLEmitVisitor::visitAttribute( const Attribute& object )
  {
    if (!object) return false;

    ofs << nl
        << "attribute ";

    this->emitAttributeMember (object);
    this->emitGetExceptions (object, false);
    this->emitSetExceptions (object);
    ofs << ";";
    this->emitPreprocDirectives (object);

    return true;
  }


  bool IDLEmitVisitor::visitComponent( const Component& object )
  {
    if (!object) return false;

    ofs << nl
        << "component " << object->getName ();

    this->emitComponentInherits (object);
    this->emitSupports (object);

    ofs << nl
        << "{" << idt;

    this->emitPreprocDirectives (object);
    this->emitPorts (object);

    // Just attributes, no need to order them first.
    this->visitChildren (object);

    ofs << uidt_nl
        << "};";

    // If this component does not have a home declared (not used
    // in EJB for example), we generate a ault declaration in IDL,
    // since it is required.
    this->emitDefaultHome (object);

    return true;
  }


  bool IDLEmitVisitor::visitComponentFactory (
      const ComponentFactory& object
    )
  {
    if (!object) return false;

    ofs << nl
        << "home " << object->getName ();

    this->emitInherits (object);
    this->emitSupports (object);
    this->emitManages (object);
    this->emitLookupKey (object);

    ofs << nl
        << "{" << idt;

    this->emitPreprocDirectives (object);
    this->visitChildren (object);

    ofs << uidt_nl
        << "};";

    return true;
  }


  bool IDLEmitVisitor::visitEnum( const Enum& object )
  {
    if (!object) return false;

    ofs << nl
        << "enum " << object->getName () << nl
        << "{" << idt;

    this->emitEnumValues (object);

    ofs << uidt_nl
        << "};";

    this->emitPreprocDirectives (object);

    return true;
  }


  bool IDLEmitVisitor::visitEvent( const Event& object )
  {
    if (!object) return false;

    // Constraints ensure that 'abstract' and 'custom' are not both set.
    ofs << nl
        << (object->isabstract () ? "abstract " : "") << "eventtype "
        << object-> getName ();

    return this->visitObjectByValue (object);
  }


  bool IDLEmitVisitor::visitException( const Exception& object )
  {
    if (!object) return false;

    ofs << nl
        << "exception " << object->getName () << nl
        << "{" << idt;

    this->emitPreprocDirectives (object);
    this->emitStructMembers (object);

    ofs << uidt_nl
        << "};";

    return true;
  }


  bool IDLEmitVisitor::visitFactoryOperation (
      const FactoryOperation& object
    )
  {
    if (!object) return false;

    ofs << nl
        << "factory ";

    return this->visitHasExceptions (object);
  }


  bool IDLEmitVisitor::visitFile( const File& object )
  {
    if (!object) return false;

    ofs.gen_ifdef_macro (object->getName ());

    this->emitIncludedFiles (object);
    this->emitPrefix (object);
    this->emitFwdDecls (object);
    this->visitChildren (object);

    ofs.gen_endif (object->getName ());

    return true;
  }


  bool IDLEmitVisitor::visitHasExceptions( const HasExceptions& object )
  {
    ofs << object->getName ();
    this->emitParameters (object);
    this->emitExceptions (object);
    ofs << ";";
    this->emitPreprocDirectives (object);

    return true;
  }


  bool IDLEmitVisitor::visitHasOperations( const HasOperations& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitInheritable( const Inheritable& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitLookupOperation (
      const LookupOperation& object
    )
  {
    if (!object) return false;

    ofs << nl
        << "finder ";

    return this->visitHasExceptions (object);
  }


  bool IDLEmitVisitor::visitObject( const Object& object )
  {
    if (!object) return false;

    // Cannot be both abstract and local, just minimizing IFs.
    ofs << nl
        << (object->isabstract () ? "abstract " : "")
        << (object->islocal () ? "local " : "")
        << "interface " << object->getName ();

    this->emitInherits (object);

    ofs << nl
        << "{" << idt;

    this->emitPreprocDirectives (object);
    this->visitChildren (object);

    ofs << uidt_nl
        << "};";

    return true;
  }


  bool IDLEmitVisitor::visitObjectByValue( const ObjectByValue& object )
  {
    // Common to ValueObject and Event.

    this->emitInherits (object);
    this->emitSupports (object);

    ofs << nl
        << "{" << idt;

    this->emitPreprocDirectives (object);

    // Generate the declarations before the members, in case a member
    // references a declaration.
    this->visitChildren (object);
    this->emitOBVMembers (object);

    ofs << uidt_nl
        << "};";

    return true;
  }


  bool IDLEmitVisitor::visitOnewayOperation (
      const OnewayOperation& object
    )
  {
    if (!object) return false;

    ofs << nl
        << "oneway void " << object->getName ();

    this->emitInParameters (object);

    ofs << ";";

    this->emitPreprocDirectives (object);

    return true;
  }


  bool IDLEmitVisitor::visitOperationBase( const OperationBase& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitPackage( const Package& object )
  {
    if (!object) return false;

    ofs << nl
        << "module " << object->getName () << nl
        << "{" << idt;

    this->emitPreprocDirectives (object);
    this->emitFwdDecls (object);
    this->visitChildren (object);

    ofs << uidt_nl
        << "};";

    return true;
  }


  bool IDLEmitVisitor::visitPrefixable( const Prefixable& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitReadonlyAttribute (
      const ReadonlyAttribute& object
    )
  {
    if (!object) return false;

    Attribute narrowed (object);
    if (narrowed) return this->visitAttribute (narrowed);

    ofs << nl
        << "readonly attribute ";

    this->emitAttributeMember (object);
    this->emitGetExceptions (object, true);
    ofs << ";";
    this->emitPreprocDirectives (object);

    return true;
  }


  bool IDLEmitVisitor::visitSupportsInterfaces (
      const SupportsInterfaces& object
    )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitSwitchedAggregate (
      const SwitchedAggregate& object
    )
  {
    if (!object) return false;

    ofs << nl
        << "union " << object->getName () << " switch (";

    this->emitDiscriminator (object);

    ofs << ")" << nl
        << "{" << idt;

    this->emitPreprocDirectives (object);
    this->emitUnionMembers (object);

    ofs << uidt_nl
        << "};";

    return true;
  }


  bool IDLEmitVisitor::visitTwowayOperation (
      const TwowayOperation& object
    )
  {
    if (!object) return false;

    ofs << nl;
    this->emitReturnType (object);
    return this->visitHasExceptions (object);
  }


  bool IDLEmitVisitor::visitValueObject( const ValueObject& object )
  {
    if (!object) return false;

    // Constraints ensure that 'abstract' and 'custom' are not both set.
    ofs << nl
        << (object->isabstract () ? "abstract " : "") << "valuetype "
        << object-> getName ();

    return this->visitObjectByValue (object);
  }


  bool IDLEmitVisitor::visitLabelConnection (
      const LabelConnection& object
    )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitMakeMemberPrivate (
      const MakeMemberPrivate& object
    )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitManagesComponent (
      const ManagesComponent& object
    )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitAlias( const Alias& object )
  {
    if (!object) return false;

    ofs << nl
        << "typedef ";

    this->emitAliasMemberType (object);

    ofs << " " << object->getName () << ";";

    this->emitPreprocDirectives (object);

    return true;
  }


  bool IDLEmitVisitor::visitAttributeMember (
      const AttributeMember& object
    )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitBoxed( const Boxed& object )
  {
    if (!object) return false;

    ofs << nl
        << "valuetype " << object->getName () << " ";

    this->emitMemberType (object);

    ofs << ";";

    this->emitPreprocDirectives (object);

    return true;
  }


  bool IDLEmitVisitor::visitCollection( const Collection& object )
  {
    if (!object) return false;

    // This will generate the proper #include if true.
    if (this->is_included_predefined_sequence (object)) return true;

    ofs << nl
        << "typedef sequence<";

    this->emitMemberType (object);

    ofs << "> " << object->getName () << ";";

    this->emitPreprocDirectives (object);

    return true;
  }


  bool IDLEmitVisitor::visitComponentRef( const ComponentRef& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitConstant( const Constant& object )
  {
    if (!object) return false;

    ofs << nl
        << "const ";

    this->emitConstantType (object);

    ofs << " " << object->getName () << " = "
        << object->getvalue () << ";";

    return true;
  }


  bool IDLEmitVisitor::visitDiscriminator( const Discriminator& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitExceptionRef( const ExceptionRef& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitFileRef( const FileRef& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitGetException( const GetException& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitInEventPort( const InEventPort& object )
  {
    if (!object) return false;

    ofs << "consumes ";

    BON::Model gparent = object->getParentModel ()->getParentModel ();
    Event e = object->getEvent ();
    BON::Model e_parent = e->getParentModel ();

    // Can we use the local name?
    if (e_parent == gparent) ofs << e->getName ();
    else ofs << this->scoped_name (e);

    ofs << " " << object->getName () << ";";

    return true;
  }


  bool IDLEmitVisitor::visitInParameter( const InParameter& object )
  {
    if (!object) return false;

    ofs << "in ";

    return true;
  }


  bool IDLEmitVisitor::visitInherits( const Inherits& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitInoutParameter( const InoutParameter& object )
  {
    if (!object) return false;

    ofs << "inout ";

    return true;
  }


  bool IDLEmitVisitor::visitLookupKey( const LookupKey& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitMember( const Member& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitOutEventPort( const OutEventPort& object )
  {
    if (!object) return false;

    ofs << (object->issingle_destination () ? "emits " : "publishes ");

    BON::Model gparent = object->getParentModel ()->getParentModel ();
    Event e = object->getEvent ();
    BON::Model e_parent = e->getParentModel ();

    // Can we use the local name?
    if (e_parent == gparent) ofs << e->getName ();
    else ofs << this->scoped_name (e);

    ofs << " " << object->getName () << ";";

    return true;
  }


  bool IDLEmitVisitor::visitOutParameter( const OutParameter& object )
  {
    if (!object) return false;

    ofs << "out ";

    return true;
  }


  bool IDLEmitVisitor::visitProvidedRequestPort (
      const ProvidedRequestPort& object
    )
  {
    if (!object) return false;

    Provideable p = object->getProvideable ();
    GenericObject go (p);
    BON::Model gparent = object->getParentModel ()->getParentModel ();
    BON::Model p_parent = p->getParentModel ();
    ofs << "provides "
        << (go ? "Object"
               : (gparent == p_parent ? p->getName ()
                                      : this->scoped_name (p)))
        << " " << object->getName () << ";";

    return true;
  }


  bool IDLEmitVisitor::visitRequiredRequestPort (
      const RequiredRequestPort& object
    )
  {
    if (!object) return false;

    Provideable p = object->getProvideable ();
    GenericObject go (p);
    BON::Model gparent = object->getParentModel ()->getParentModel ();
    BON::Model p_parent = p->getParentModel ();
    ofs << "uses "
        << (object->ismultiple_connections () ? "multiple " : "")
        << (go ? "Object"
               : (gparent == p_parent ? p->getName ()
                                      : this->scoped_name (p)))
        << " " << object->getName () << ";";

    return true;
  }


  bool IDLEmitVisitor::visitReturnType( const ReturnType& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitSetException( const SetException& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitSupports( const Supports& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitConstantType( const ConstantType& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitManageable( const Manageable& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitMemberType( const MemberType& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitNamedType( const NamedType& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitNoInheritable( const NoInheritable& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitPort( const Port& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitPredefinedType( const PredefinedType& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitTaggable( const Taggable& object )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitInterfaceDefinitions (
      const InterfaceDefinitions& object
    )
  {
    if ( !object)
      return false;

    return true;
  }


  bool IDLEmitVisitor::visitPredefinedTypes (
      const PredefinedTypes& object
    )
  {
    if ( !object)
      return false;

    return true;
  }


  void IDLEmitVisitor::visitObjectImpl( const BON::Object& obj )
  {}


  void IDLEmitVisitor::visitFCOImpl( const BON::FCO& fco )
  {
    if ( !visitConstantType( fco))
    if ( !visitManageable( fco))
    if ( !visitMemberType( fco))
    if ( !visitNamedType( fco))
    if ( !visitNoInheritable( fco))
    if ( !visitPort( fco))
    if ( !visitPredefinedType( fco))
    if ( !visitTaggable( fco))
    {
      // visiting other fco
    }
  }


  void IDLEmitVisitor::visitAtomImpl( const BON::Atom& atom )
  {
    if ( !visitBoolean( atom))
    if ( !visitByte( atom))
    if ( !visitEnumValue( atom))
    if ( !visitGenericObject( atom))
    if ( !visitGenericValue( atom))
    if ( !visitGenericValueObject( atom))
    if ( !visitLabel( atom))
    if ( !visitLongInteger( atom))
    if ( !visitPrivateFlag( atom))
    if ( !visitRealNumber( atom))
    if ( !visitShortInteger( atom))
    if ( !visitString( atom))
    if ( !visitTypeEncoding( atom))
    if ( !visitTypeKind( atom))
    {
      // visiting other Atom
    }
  }


  void IDLEmitVisitor::visitReferenceImpl( const BON::Reference& ref )
  {
    if ( !visitAlias( ref))
    if ( !visitAttributeMember( ref))
    if ( !visitBoxed( ref))
    if ( !visitCollection( ref))
    if ( !visitComponentRef( ref))
    if ( !visitConstant( ref))
    if ( !visitDiscriminator( ref))
    if ( !visitExceptionRef( ref))
    if ( !visitFileRef( ref))
    if ( !visitGetException( ref))
    if ( !visitInEventPort( ref))
    if ( !visitInParameter( ref))
    if ( !visitInherits( ref))
    if ( !visitInoutParameter( ref))
    if ( !visitLookupKey( ref))
    if ( !visitMember( ref))
    if ( !visitOutEventPort( ref))
    if ( !visitOutParameter( ref))
    if ( !visitProvidedRequestPort( ref))
    if ( !visitRequiredRequestPort( ref))
    if ( !visitReturnType( ref))
    if ( !visitSetException( ref))
    if ( !visitSupports( ref))
    {
      // visiting other Reference
    }
  }


  void IDLEmitVisitor::visitConnectionImpl( const BON::Connection& conn )
  {
    if ( !visitLabelConnection( conn))
    if ( !visitMakeMemberPrivate( conn))
    if ( !visitManagesComponent( conn))
    {
      // visiting other Connection
    }
  }


  void IDLEmitVisitor::visitModelImpl( const BON::Model& model )
  {
    if ( !visitAggregate( model))
    if ( !visitAttribute( model))
    if ( !visitComponent( model))
    if ( !visitComponentFactory( model))
    if ( !visitEnum( model))
    if ( !visitEvent( model))
    if ( !visitException( model))
    if ( !visitFactoryOperation( model))
    if ( !visitFile( model))
    if ( !visitHasExceptions( model))
    if ( !visitHasOperations( model))
    if ( !visitInheritable( model))
    if ( !visitLookupOperation( model))
    if ( !visitObject( model))
    if ( !visitObjectByValue( model))
    if ( !visitOnewayOperation( model))
    if ( !visitOperationBase( model))
    if ( !visitPackage( model))
    if ( !visitPrefixable( model))
    if ( !visitReadonlyAttribute( model))
    if ( !visitSupportsInterfaces( model))
    if ( !visitSwitchedAggregate( model))
    if ( !visitTwowayOperation( model))
    if ( !visitValueObject( model))
    {
      // visiting other Model
    }
  }


  void IDLEmitVisitor::visitFolderImpl( const BON::Folder& fold )
  {
    if ( !visitInterfaceDefinitions( fold))
    if ( !visitPredefinedTypes( fold))
    {
      // visiting other Folder
    }
  }

  void IDLEmitVisitor::visitOrderableImpl( const Orderable& object )
  {
    if ( !visitAggregate( object))
    if ( !visitAttribute( object))
    if ( !visitComponent( object))
    if ( !visitComponentFactory( object))
    if ( !visitEnum( object))
    if ( !visitEvent( object))
    if ( !visitException( object))
    if ( !visitFactoryOperation( object))
    if ( !visitFile( object))
//    if ( !visitHasExceptions( object))
//    if ( !visitHasOperations( object))
//    if ( !visitInheritable( object))
    if ( !visitLookupOperation( object))
    if ( !visitObject( object))
//    if ( !visitObjectByValue( object))
    if ( !visitOnewayOperation( object))
//    if ( !visitOperationBase( object))
    if ( !visitPackage( object))
//    if ( !visitPrefixable( object))
    if ( !visitReadonlyAttribute( object))
//    if ( !visitSupportsInterfaces( object))
    if ( !visitSwitchedAggregate( object))
    if ( !visitTwowayOperation( object))
    if ( !visitValueObject( object))
    if ( !visitAlias( object))
    if ( !visitBoxed( object))
    if ( !visitCollection( object))
    if ( !visitConstant( object))
    {
      // visiting other Model
    }
  }

  void IDLEmitVisitor::order_children( const Orderable& object )
  {
    bool changed = true;

#if (EMITS_DEBUG == 1)
    BON::Model scope (object->getParentModel ());
    std::string filename;
    if (scope)
      {
        filename = scope->getName ();
        std::string::size_type pos = filename.rfind ('/');
        if (std::string::npos != pos) filename = filename.substr (pos + 1);
        filename += "_";
      }
    filename += object->getName ();
    filename += "_Log";
    std::ofstream tmp (filename.c_str ());
    IDLStream dbg (tmp);
#endif

//    typedef std::pair<Orderable, Orderable> SwappedPair;
//    typedef std::set<SwappedPair> SwappedSet;
//    SwappedSet swapped;

    while (changed)
      {
        // If none of the stuff below changes this, we are done.
        changed = false;

#if (EMITS_DEBUG == 1)
        dbg << nl;

        for (std::vector<Orderable>::iterator i =
               object->ordered_children.begin ();
             i != object->ordered_children.end ();
             i++)
          {
            dbg << (*i)->getName () << nl;
          }

        dbg << nl;
#endif

        for (std::vector<Orderable>::iterator i =
               object->ordered_children.begin ();
             i != object->ordered_children.end ();
             i++)
          {
            std::set<Orderable>& refs = (*i)->depends_on_me;

#if (EMITS_DEBUG == 1)
            dbg << (*i)->getName () << ": depends on me" << nl;
            for (std::set<Orderable>::iterator r = refs.begin ();
                 r != refs.end ();
                 r++)
              {
                dbg << (*r)->getName () << nl;
              }

            dbg << nl;
#endif

            // For each entry in our ordered_children list, we iterate
            // over its depends_on_me list...
            for (std::set<Orderable>::iterator ri = refs.begin ();
                 ri != refs.end ();
                 ri++)
              {
               // and see if the item in depends_on_me is also
                // in ordered_children. If no match is found,
                // tmp will be ordered_children.end().
                std::vector<Orderable>::iterator tmp =
                  std::find (object->ordered_children.begin (),
                             object->ordered_children.end (),
                             (*ri));

                // If the expression is true, it means we have a
                // match, and also that the depending element comes
                // earlier in the list than its dependent, so we
                // swap them. This swap will probably mess up the
                // iterators, so we must start over. We are done
                // when the whole nested loop executes with no
                // changes. Inefficient, but safe.
                if (tmp - i < 0 )
                  {
                    Collection ctmp (*tmp);
                    Collection ci (*i);

                    // The two IF blocks below catch simple recursive types,
                    // bypassing swapping, which would regress infinitely.
                    // They also catch non-recursive case, but the resulting
                    // forward declaration will do no harm and subsitute for a
                    // swap.
                    if (ctmp)
                      {
                        if (ctmp->getMemberType ()->getName () == (*i)->getName ())
                          {
                            this->emitFwdDeclNested (*i, 0);
                            continue;
                          }
                      }

                    if (ci)
                      {
                        if (ci->getMemberType ()->getName () == (*tmp)->getName ())
                          {
                            this->emitFwdDeclNested (*tmp, 0);
                            continue;
                          }
                      }
#if (EMITS_DEBUG == 1)
                    dbg << "swapping " << (*tmp)->getName ()
                        << " and " << (*i)->getName () << nl;
#endif
                    std::swap ((*tmp), (*i));
                    changed = true;
                    break;
                  }
              }

            // We must break out of both loops.
            if (changed) break;
          }
      }
  }

  std::string IDLEmitVisitor::scoped_name( const BON::FCO& object )
  {
    PredefinedType pdt (object);
    if (pdt) return this->basic_name (pdt);

    BON::Model parent (object->getParentModel ());

    // If we are a file, return an empty string.
    if (!parent) return "";

    std::string local_name (object->getName ());

    // If our parent is a file, don't tack on its name.
    if (!File (parent))
      {
        return this->scoped_name (parent) + "::" + local_name;
      }

    return local_name;
  }

  std::string IDLEmitVisitor::basic_name( const PredefinedType& pdt )
  {
    if (String (pdt))             return "string";
    if (LongInteger (pdt))        return "long";
    if (RealNumber (pdt))         return "double";
    if (ShortInteger (pdt))       return "short";
    if (Boolean (pdt))            return "boolean";
    if (Byte (pdt))               return "octet";
    if (GenericValue (pdt))       return "any";
    if (GenericObject (pdt))      return "Object";
    if (GenericValueObject (pdt)) return "ValueBase";
    if (TypeEncoding (pdt))       return "CORBA::TypeCode";
    if (TypeKind (pdt))           return "CORBA::TCKind";
    else                          return "";
  }

  std::string IDLEmitVisitor::label_scope_prefix( const Discriminator& d )
  {
    Enum e (d->getReferred ());
    if (!e) return "";
    return this->scoped_name (e->getParentModel ());
  }

  bool IDLEmitVisitor::is_included_predefined_sequence( const Collection& c )
  {
    if (!c) return false;
    std::string name (c->getName ());
    long pos = name.length () - IDL_INCLUDE_SUFFIX_LEN;
    if (pos <= 0) return false;
    if (name.substr ((size_t) pos) != IDL_INCLUDE_SUFFIX) return false;


    ofs << nl << "#include \"tao/";

    MemberType mt = c->getMemberType ();
    if (String (mt))              ofs << "String";
    else if (LongInteger (mt))    ofs << "Long";
    else if (RealNumber (mt))     ofs << "Double";
    else if (ShortInteger (mt))   ofs << "Short";
    else if (Boolean (mt))        ofs << "Boolean";
    else if (Byte (mt))           ofs << "Octet";
    else if (GenericValue (mt))   ofs << "AnyTypeCode/Any";
    else return false;

    ofs << "Seq.pidl\"";

    return true;
  }

  void IDLEmitVisitor::emitIncludedFiles( const File& f )
  {
    bool first = true;

    if (f->include_components_idl ())
      {
        ofs << nl
            << "#include <Components.idl>" << nl;

        first = false;
      }
    else if (f->include_orb_idl ())
      {
        ofs << nl
            << "#include <orb.idl>" << nl;

        first = false;
      }

    File fi;
    std::set<File>::iterator found;

    // If a file in this list is also in the list below, skip it.
    std::set<FileRef> file_includes = f->getFileRef ();
    for (std::set<FileRef>::const_iterator it = file_includes.begin ();
         it != file_includes.end ();
         it++)
      {
        if (first) ofs << nl;

        fi = (*it)->getFile ();
        if (!fi) continue;
        found = std::find (f->discovered_includes_.begin (),
                           f->discovered_includes_.end (),
                           fi);
        if (found != f->discovered_includes_.end ()) continue;

        // All generated IDL files will be in the same directory,
        // so get rid of any path elements in includes.
        std::string name (fi->getName ());

        ofs << "#include \"" << name << ".idl\"" << nl;

        first = false;
      }

    // Now emit the discovered includes.
    for (std::set<File>::const_iterator i = f->discovered_includes_.begin ();
         i != f->discovered_includes_.end ();
         ++i)
      {
        if (first) ofs << nl;

        // All generated IDL files will be in the same directory,
        // so get rid of any path elements in includes.
        std::string name ((*i)->getName ());
        ofs << "#include \"" << name << ".idl\"" << nl;

        first = false;
      }
  }

  void IDLEmitVisitor::emitPrefix( const File& f )
  {
    std::string global_prefix = f->getPrefixTag ();
    if (global_prefix != "")
      {
        ofs << nl
            << "typeprefix :: \"" << global_prefix << "\";" << nl;
      }
  }

  void IDLEmitVisitor::emitFwdDecls( const Orderable& object )
  {
    std::set<std::pair<Orderable, int> > fwd_decls = object->fwd_decl_children;
    if (fwd_decls.size () == 0) return;

    for (std::set<std::pair<Orderable, int> >::iterator i =
           fwd_decls.begin ();
         i != fwd_decls.end ();
         i++)
      {
        this->emitFwdDeclNested ((*i).first, (*i).second);
        ofs << nl;
      }
  }

  void IDLEmitVisitor::emitFwdDeclNested ( const Orderable& object,
                                           int level )
  {
    // Forward decls for recursive structs and unions are generated
    // a different way than for interface/valuetypes. This check
    // keeps them from being generated more than once each.
    if (object->fwd_declared_) return;

    if (level == 0)
      {
        Object odf (object);
        Component cdf (object);
        ValueObject vdf (object);
        Event edf (object);
        Aggregate adf (object);
        SwitchedAggregate sdf (object);

        ofs << nl;

        if (odf)
          {
            ofs << (odf->isabstract () ? "abstract " : "")
                << (odf->islocal () ? "local " : "") << "interface ";
          }
        else if (cdf)
          {
            ofs << "component ";
          }
        else if (vdf)
          {
            ofs << (vdf->isabstract () ? "abstract " : "") << "valuetype ";
          }
        else if (edf)
          {
            ofs << (edf->isabstract () ? "abstract " : "") << "eventtype ";
          }
        else if (adf)
          {
            ofs << "struct ";
          }
        else if (sdf)
          {
            ofs << "union ";
          }
        else
          {
            AfxMessageBox ("Error: Bad forward declare type\n");
          }

        ofs << object->getName () << ";";

        // Set the flag so this won't get generated again.
        object->fwd_declared_ = true;
      }
    else
      {
        Orderable container (object);
        for (int i = 0; i < level; ++i)
          {
            container = Orderable (container->getParentModel ());
          }

        ofs << nl
            << "module " << container->getName () << nl
            << "{" << idt;

        this->emitFwdDeclNested (object, level - 1);

        ofs << uidt_nl
            << "};";
      }
  }

  void
  IDLEmitVisitor::visitChildren (const Orderable& object)
  {
    this->order_children (object);
    Component comp (object);
    bool not_first = false;

    for (std::vector<Orderable>::iterator it =
           object->ordered_children.begin ();
         it != object->ordered_children.end ();
         it++)
      {
        // Relative IDs of derived GME children have one of the
        // digits in the number below added to their base value
        // - easy way to check.
        long rel_id;
        (*it)->getFCOI ()->get_RelID (&rel_id);
        if (rel_id & 0x18000000) continue;

        if (not_first) ofs << nl;
        if ((*it)->getParent () != object) continue;
        this->visitOrderableImpl (*it);
        not_first = true;
      }
  }

  void IDLEmitVisitor::emitPreprocDirectives( const Taggable& t )
  {
    if (!t) return;
    std::string holder = t->getSpecifyIdTag ();
    bool hit = false;

    if (holder != "")
      {
        ofs << nl
            << "typeid " << t->getName ()
            << " \"" << holder << "\";" << nl;
        return;
      }

    Prefixable p (t);
    if (p)
      {
        holder = p->getPrefixTag ();

        if (holder != "")
          {
            ofs << nl
                << "typeprefix " << t->getName ()
                << " \"" << holder << "\";";
            hit = true;
          }
      }

    holder = t->getVersionTag ();
    if (holder != "")
      {
        ofs << nl
            << "#pragma version " << t->getName ()
            << " " << holder;
        hit = true;
      }

    if (hit) ofs << nl;
  }

  void
  IDLEmitVisitor::emitStructMembers (const Orderable& object)
  {
    std::set<Member> members;
    Exception excep (object);
    Aggregate aggr (object);

    if (excep) members = excep->getMember ();
    else members = aggr->getMember ();

    MemberType mt;
    BON::Model o_ma = object->getParentModel ();
    BON::Model m_ma;

    for (std::set<Member>::const_iterator i = members.begin ();
         i != members.end ();
         i++)
      {
        mt = (*i)->getMemberType ();

        ofs << nl;

        if (!this->emitPredefinedSequence (mt))
          {
            m_ma = mt->getParentModel ();
            ofs << (m_ma == o_ma ? mt->getName () : this->scoped_name (mt));
          }

        ofs << " " << (*i)->getName () << ";";
      }
  }

  void IDLEmitVisitor::emitUnionMembers (const SwitchedAggregate& s)
  {
    std::set<Discriminator> disc = s->getDiscriminator ();
    std::set<Discriminator>::iterator di = disc.begin ();
    Byte is_char_disc = (*di)->getReferred ();

    std::set<Member> members = s->getMember ();
    MemberType mt;

    for (std::set<Member>::const_iterator i = members.begin ();
         i != members.end ();
         i++)
      {
        std::multiset<Label> labels = (*i)->getLabelConnectionDsts ();

        for (std::multiset<Label>::const_iterator li = labels.begin ();
             li != labels.end ();
             ++li)
          {
            ofs << nl;
            std::string tmp = (*li)->getName ();

            if (tmp != "default")
              {
                ofs << "case ::";
              }

            // If is_char_disc is true, label_prefix will be empty.
            ofs << (is_char_disc ? "'" : "")
                << tmp
                << (is_char_disc ? "'" : "")
                << ":";
          }

        mt = (*i)->getMemberType ();

        ofs << " " << this->scoped_name (mt)
            << " " << (*i)->getName () << ";";
      }
  }

  void IDLEmitVisitor::emitAttributeMember( const ReadonlyAttribute& ra )
  {
    std::set<AttributeMember> member = ra->getAttributeMember ();
    std::set<AttributeMember>::const_iterator i = member.begin ();
    MemberType mt ((*i)->getMemberType ());
    if (this->emitPredefinedSequence (mt)) return;

    // Can we use the local name? The attribute type could be
    // defined in the same home or interface or just outside it.
    BON::Model r_parent = ra->getParentModel ();
    BON::Model r_gparent = r_parent->getParentModel ();
    BON::Model m_parent = mt->getParentModel ();
    if (m_parent == r_parent || m_parent == r_gparent) ofs << mt->getName ();
    else ofs << this->scoped_name (mt);

    ofs << " " << ra->getName ();
  }

  void IDLEmitVisitor::emitMemberType( const NamedType& nt )
  {
    Boxed b (nt);
    Collection c (nt);
    if (!b && !c) return;
    MemberType mt;
    if (b) mt = b->getMemberType ();
    else mt = c->getMemberType ();
    BON::Model m_parent = mt->getParentModel ();
    BON::Model n_parent = nt->getParentModel ();
    if (m_parent == n_parent) ofs << mt->getName ();
    else ofs << this->scoped_name (mt);
  }

  void IDLEmitVisitor::emitAliasMemberType( const NamedType& nt )
  {
    Alias a (nt);
    MemberType mt = a->getMemberType ();
    if (this->emitPredefinedSequence (mt)) return;
    BON::Model m_parent = mt->getParentModel ();
    BON::Model n_parent = nt->getParentModel ();
    if (m_parent == n_parent) ofs << mt->getName ();
    else ofs << this->scoped_name (mt);
  }

  void IDLEmitVisitor::emitConstantType( const Constant& c )
  {
    ConstantType ct = c->getConstantType ();
    BON::Model c_parent = c->getParentModel ();
    BON::Model ct_parent = ct->getParentModel ();
    if (c_parent == ct_parent) ofs << ct->getName ();
    else ofs << this->scoped_name (ct);
  }

  void IDLEmitVisitor::emitInherits( const Inheritable& object )
  {
    std::set<Inherits> parents = object->getInherits ();
    if (parents.size () == 0) return;

    ofs << nl
        << "  : ";

    bool obv_inherits_concrete = false;
    ObjectByValue obv (object);

    // If a concrete valuetype of eventtype has a concrete
    // parent and one or more abstract ones, the concrete
    // parent must appear first in the inheritance list. The GME
    // Constraint Manager will ensure that no more than one concrete
    // parent is inherited in such a case.
    if (obv && !obv->isabstract ())
      {
        obv_inherits_concrete = this->emitOBVInheritsConcrete (parents);
      }

    Inheritable inh;
    BON::Model i_ma;
    BON::Model o_ma = object->getParentModel ();

    for (std::set<Inherits>::iterator i = parents.begin ();
         i != parents.end ();
         i++)
      {
        if (i != parents.begin () || obv_inherits_concrete) ofs << ", ";
        inh = (*i)->getInheritable ();
        i_ma = inh->getParentModel ();
        if (i_ma == o_ma) ofs << inh->getName ();
        else ofs << this->scoped_name (inh);
      }
  }

  void IDLEmitVisitor::emitComponentInherits (const Component &comp)
  {
    Orderable base = comp->base_component ();
    if (!base) return;

    ofs << nl
        << "  : ";

    BON::Model i_ma = comp->getParentModel ();
    BON::Model o_ma = base->getParentModel ();
    if (i_ma == o_ma) ofs << base->getName ();
    else ofs << this->scoped_name (base);
  }

  bool IDLEmitVisitor::emitOBVInheritsConcrete (
      std::set<Inherits>& parents
    )
  {
    for (std::set<Inherits>::iterator i = parents.begin ();
         i != parents.end ();
         i++)
      {
        // Inside this function, we know this will work.
        ObjectByValue obv ((*i)->getInheritable ());

        if (!obv->isabstract ())
          {
            // Generate it and remove it from the list.
            ofs << this->scoped_name (obv);
            parents.erase (i);
            return true;
          }
      }

    return false;
  }

  void IDLEmitVisitor::emitReturnType( const TwowayOperation& op )
  {
    std::set<ReturnType> rettype = op->getReturnType ();
    if (rettype.size () == 0)
      {
        ofs << "void ";
        return;
      }

    std::set<ReturnType>::const_iterator i = rettype.begin ();
    ofs << this->scoped_name ((*i)->getReferred ()) << " ";
  }

  void IDLEmitVisitor::emitParameters( const HasExceptions& he )
  {
    ofs << " (";

    std::set<BON::Reference> refs = he->getChildReferences ();
    std::set<ExceptionRef> exceps = he->getExceptionRef ();
    unsigned long rt_factor = 0;
    TwowayOperation op (he);
    if (op) rt_factor = op->getReturnType ().size ();

    if (refs.size () == exceps.size () + rt_factor)
      {
        ofs << ")";
        return;
      }
    else
      {
        ofs << idt << idt_nl;
      }

    bool first = true;
    BON::FCO holder;
    BON::Model op_parent = he->getParentModel ();
    BON::Model arg_parent;

    for (std::set<BON::Reference>::const_iterator i = refs.begin ();
         i != refs.end ();
         i++)
      {
        if (ReturnType (*i)) continue;
        if (ExceptionRef (*i)) continue;
        if (!first) ofs << "," << nl;
        this->visitReferenceImpl (*i);
        holder = (*i)->getReferred ();

        // Can we use the local type name?
        arg_parent = holder->getParentModel ();
        if (op_parent == arg_parent) ofs << holder->getName ();
        else ofs << this->scoped_name (holder);

        ofs << " " << (*i)->getName ();
        first = false;
      }

    ofs << uidt_nl
        << ")" << uidt;
  }

  void IDLEmitVisitor::emitInParameters( const OperationBase& op )
  {
    ofs << " (";
    std::set<ExceptionRef> exceps;
    HasExceptions he (op);
    if (he) exceps = he->getExceptionRef ();
    std::set<BON::Reference> refs = op->getChildReferences ();

    if (refs.size () == exceps.size ())
      {
        ofs << ")";
        return;
      }

    ofs << idt << idt_nl;

    BON::FCO holder;
    BON::Model op_parent = op->getParentModel ();
    BON::Model arg_parent;
    bool first = true;

    for (std::set<BON::Reference>::const_iterator i = refs.begin ();
         i != refs.end ();
         ++i)
      {
        if (ExceptionRef (*i)) continue;
        if (!first) ofs << "," << nl;
        ofs << "in ";
        holder = (*i)->getReferred ();

        // Can we use the local type name?
        arg_parent = holder->getParentModel ();
        if (op_parent == arg_parent) ofs << holder->getName ();
        else ofs << this->scoped_name (holder);

        ofs << " " << (*i)->getName ();
        first = false;
      }

    ofs << uidt_nl
        << ")" << uidt;
  }

  void IDLEmitVisitor::emitExceptions( const HasExceptions& he )
  {
    std::set<ExceptionRef> exceptions = he->getExceptionRef ();
    if (exceptions.size () == 0) return;

    ofs << idt_nl
        << "raises (";

    Exception ex;
    BON::Model ex_ma;
    BON::Model op_ma = he->getParentModel ();
    BON::Model op_gma = op_ma->getParentModel ();

    for (std::set<ExceptionRef>::iterator i = exceptions.begin ();
         i != exceptions.end ();
         i++)
      {
        if (i != exceptions.begin ()) ofs << ", ";

        // Can we use the local name?
        ex = (*i)->getException ();
        ex_ma = ex->getParentModel ();
        if (ex_ma == op_ma || ex_ma == op_gma) ofs << ex->getName ();
        else ofs << this->scoped_name (ex);
      }

    ofs << ")" << uidt;
  }

  void IDLEmitVisitor::emitGetExceptions( const ReadonlyAttribute& r,
                                          bool readonly )
  {
    std::set<GetException> get_exceptions = r->getGetException ();
    if (get_exceptions.size () == 0) return;

    ofs << idt_nl
        << (!readonly ? "get" : "" ) << "raises (";

    Exception e;
    BON::Model r_ma = r->getParentModel ();
    BON::Model r_gma = r_ma->getParentModel ();
    BON::Model e_ma;

    for (std::set<GetException>::iterator i = get_exceptions.begin ();
         i != get_exceptions.end ();
         i++)
      {
        if (i != get_exceptions.begin ()) ofs << ", ";
        e = (*i)->getException ();
        e_ma = e->getParentModel ();
        if (e_ma == r_ma || e_ma == r_gma) ofs << e->getName ();
        else ofs << this->scoped_name (e);
      }

    ofs << ")" << uidt;
  }

  void IDLEmitVisitor::emitSetExceptions( const Attribute& a )
  {
    std::set<SetException> set_exceptions = a->getSetException ();
    if (set_exceptions.size () == 0) return;

    ofs << idt_nl
        << "setraises (";

    Exception e;
    BON::Model a_ma = a->getParentModel ();
    BON::Model a_gma = a_ma->getParentModel ();
    BON::Model e_ma;

    for (std::set<SetException>::iterator i = set_exceptions.begin ();
         i != set_exceptions.end ();
         i++)
      {
        if (i != set_exceptions.begin ()) ofs << ", ";
        e = (*i)->getException ();
        e_ma = e->getParentModel ();
        if (e_ma == a_ma || e_ma == a_gma) ofs << e->getName ();
        else ofs << this->scoped_name (e);
      }

    ofs << ")" << uidt;
  }

  void IDLEmitVisitor::emitSupports( const SupportsInterfaces& s )
  {
    std::set<Supports> supported = s->getSupports ();
    if (supported.size () == 0) return;

    ofs << nl
        << "  supports ";

   BON::Model s_parent = s->getParentModel ();
   BON::Model i_parent;
   Object holder;

   for (std::set<Supports>::iterator i = supported.begin ();
         i != supported.end ();
         i++)
      {
        if (i != supported.begin ()) ofs << ", ";
        holder = (*i)->getObject ();
        i_parent = holder->getParentModel ();

        // Can we use the local name?
        if (s_parent == i_parent) ofs << holder->getName ();
        else ofs << this->scoped_name (holder);
      }
  }

  void IDLEmitVisitor::emitPorts( const Component& c )
  {
    std::set<Port> ports = c->getPort ();
    bool not_first = false;

    for (std::set<Port>::const_iterator i = ports.begin ();
         i != ports.end ();
         i++)
      {
        // Relative IDs of derived GME ports have one of the
        // digits in the number below added to their base value
        // - easy way to check.
        long rel_id;
        (*i)->getFCOI ()->get_RelID (&rel_id);
        if (rel_id & 0x18000000) continue;

        if (not_first) ofs << nl;

        ofs << nl;
        this->visitReferenceImpl (*i);
        this->emitPreprocDirectives (*i);
        not_first = true;
      }

    // Leave a space between the ports and the attributes, if there
    // are any that are not inherited.
    if (not_first)
      {
        for (std::vector<Orderable>::const_iterator oci =
               c->ordered_children.begin ();
             oci != c->ordered_children.end ();
             oci++)
          {
            long oc_rel_id;
            (*oci)->getFCOI ()->get_RelID (&oc_rel_id);
            if (oc_rel_id & 0x18000000) continue;
            ofs << nl;
            break;
          }
      }
  }

  void
  IDLEmitVisitor::emitDefaultHome( const Component& c )
  {
    // No default home if there's one in the model.
    if (c->getManagesComponentSrcs ().size () != 0) return;
    if (c->ref_managed ()) return;

    ofs << nl << nl
        << "home " << c->getName () << "PICMLDefaultHome";

    // Local name for default home is ok - the managed component
    // is being declared just above it.
    ofs << nl
        << "  manages " << c->getName () << nl
        << "{" << nl
        << "};";
  }

  void IDLEmitVisitor::emitManages( const ComponentFactory& cf )
  {
    // Metamodel constraint forces the size to be exactly 1.
    std::multiset<Manageable> manages = cf->getManagesComponentDsts ();
    std::multiset<Manageable>::iterator i = manages.begin ();
    ofs << nl
        << "  manages ";

    // Local name is ok unless we are connected to a component reference.
    Component comp (*i);
    if (comp) ofs << (*i)->getName ();
    else ofs << this->scoped_name (ComponentRef (*i)->getReferred ());
  }

  void IDLEmitVisitor::emitLookupKey( const ComponentFactory& cf )
  {
    std::set<LookupKey> key = cf->getLookupKey ();
    if (key.size () == 0) return;
    std::set<LookupKey>::const_iterator i = key.begin ();
    ValueObject vd = (*i)->getValueObject ();
    BON::Model k_parent = vd->getParentModel ();
    BON::Model cf_parent = cf->getParentModel ();
    ofs << nl
        << "  primarykey "
        << (k_parent == cf_parent ? vd->getName () : this->scoped_name (vd));
  }

  void IDLEmitVisitor::emitOBVMembers( const ObjectByValue& obv )
  {
    std::set<Member> members = obv->getMember ();

    for (std::set<Member>::const_iterator i = members.begin ();
         i != members.end ();
         i++)
      {
        ofs << nl;
        std::set<BON::Connection> conns = (*i)->getOutConnLinks ();
        ofs << (conns.size () > 0 ? "private " : "public ")
            << this->scoped_name ((*i)->getReferred ())
            << " " << (*i)->getName () << ";";
      }
  }

  void IDLEmitVisitor::emitEnumValues( const Enum& e )
  {
    std::set<EnumValue> values = e->getEnumValue ();
    for (std::set<EnumValue>::const_iterator it = values.begin ();
         it != values.end ();
         it++)
      {
        if (it != values.begin ()) ofs << ",";
        ofs << nl
            << (*it)->getName ();
      }
  }

  void IDLEmitVisitor::emitDiscriminator( const SwitchedAggregate& sa)
  {
    std::set<Discriminator> disc = sa->getDiscriminator ();
    std::set<Discriminator>::iterator i = disc.begin ();
    Byte bt = (*i)->getReferred ();

    // The default output name for a Byte is "octet" which is an
    // illegal discriminator type in IDL.
    if (bt)
      {
        ofs << "char";
      }
    else
      {
        ConstantType ct = (*i)->getReferred ();
        BON::Model sa_parent = sa->getParentModel ();
        BON::Model d_parent = ct->getParentModel ();
        ofs << (sa_parent == d_parent ? ct->getName () : this->scoped_name (ct));
      }
  }

  bool IDLEmitVisitor::emitPredefinedSequence( const MemberType& m )
  {
    Collection c (m);
    if (!c) return false;
    MemberType mt = c->getMemberType ();

    if (String (mt))            ofs << "CORBA::StringSeq";
    else if (LongInteger (mt))  ofs << "CORBA::LongSeq";
    else if (RealNumber (mt))   ofs << "CORBA::DoubleSeq";
    else if (ShortInteger (mt)) ofs << "CORBA::ShortSeq";
    else if (Boolean (mt))      ofs << "CORBA::BooleanSeq";
    else if (Byte (mt))         ofs << "CORBA::OctetSeq";
    else if (GenericValue (mt)) ofs << "CORBA::AnySeq";
    else return false;

    return true;
  }

} // namespace BON
