//
// $Id$
//
#if !defined (SERVANT_BASE_H)
#define SERVANT_BASE_H

class TAO_Export TAO_ServantBase
{
  // = TITLE
  //   Base class for skeletons and servants.
  //
  // = DESCRIPTION
  //   The POA spec requires that all servants inherit from this
  //   class.
  //

  friend class TAO_POA;

public:
  virtual ~TAO_ServantBase (void);
  // destructor

  TAO_ServantBase &operator= (const TAO_ServantBase &);
  // assignment operator.

  virtual PortableServer::POA_ptr _default_POA (CORBA::Environment &env);

  virtual void *_downcast (const char *repository_id) = 0;
  // Get the correct vtable

  virtual STUB_Object *_create_stub (CORBA_Environment &env);
  // This is an auxiliar method for _this() and _narrow().

protected:
  TAO_ServantBase (void);
  // Default constructor, only derived classes can be created.

  TAO_ServantBase (const TAO_ServantBase &);
  // Copy constructor, protected so no instances can be created.

  void _set_parent (TAO_IUnknown *p);
  // Set the "parent" in the QueryInterface hierarchy.
  // @@ TODO use a conformant name; since it is an
  // internal (implementation) method its name should start with '_'

  virtual CORBA::Boolean _is_a (const char* logical_type_id,
				CORBA::Environment &env);
  // Local implementation of the CORBA::Object::_is_a method.

  virtual void _dispatch (CORBA::ServerRequest &request,
                          void *context,
                          CORBA::Environment &env);
  // Dispatches a request to the object: find the operation, cast
  // the type to the most derived type, demarshall all the
  // parameters from the request and finally invokes the operation,
  // storing the results and out parameters (if any) or the
  // exceptions thrown into <request>.
  // @@ TODO use a conformant name; since it is an
  // internal (implementation) method its name should start with '_'

  virtual int _find (const CORBA::String &opname,
                     TAO_Skeleton &skelfunc);
  // Find an operation in the operation table.
  // @@ TODO use a conformant name; since it is an
  // internal (implementation) method its name should start with '_'

  virtual int _bind (const CORBA::String &opname,
                     const TAO_Skeleton skel_ptr);
  // Register a CORBA IDL operation name.
  // @@ TODO use a conformant name; since it is an
  // internal (implementation) method its name should start with '_'

  TAO_IUnknown *_get_parent (void) const;
  // Get the "parent" in the QueryInterface hierarchy.

  virtual const char *_interface_repository_id (void) const = 0;
  // Get this interface's repository id

protected:
  TAO_Operation_Table *optable_;
  // The operation table for this servant, it is initialized by the
  // most derived class.

  TAO_IUnknown *parent_;
  // @@ TODO find out why is this here....
};

class TAO_Export TAO_Local_ServantBase : public TAO_ServantBase
{
  // = TITLE
  //   Base class for local skeletons and servants.
  //
protected:
  STUB_Object *_create_stub (CORBA_Environment &env);
  // This is an auxiliar method for _this().  Make sure *not* to
  // register with the default POA
};

// Base class for DSI
class TAO_DynamicImplementation : public virtual TAO_ServantBase
{
  //
  // = DESCRIPTION
  //
  //   It is expected that the invoke() and _primary_interface()
  //   methods will be only invoked by the POA in the context of
  //   serving a CORBA request. Invoking this method in other
  //   circumstances may lead to unpredictable results.

public:

  virtual void invoke (CORBA::ServerRequest_ptr request,
                       CORBA::Environment &env) = 0;
  // The invoke() method receives requests issued to any CORBA object
  // incarnated by the DSI servant and performs the processing
  // necessary to execute the request.

  // virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &oid,
  //                                                 PortableServer::POA_ptr poa,
  //                                                 CORBA::Environment &env) = 0;

  virtual PortableServer::RepositoryId _primary_interface (const PortableServer::ObjectId &oid,
                                                           PortableServer::POA_ptr poa,
                                                           CORBA::Environment &env) = 0;
  // The _primary_interface() method receives an ObjectId value and a
  // POA_ptr as input parameters and returns a valid RepositoryId
  // representing the most-derived interface for that oid.

  CORBA::Object_ptr _this (CORBA::Environment &env);
  // Returns a CORBA::Object_ptr for the target object.

protected:

  virtual const char *_interface_repository_id (void) const;
  // Return 0.  Should never be used.

  virtual void *_downcast (const char *repository_id);
  // Simply returns "this"

  virtual STUB_Object *_create_stub (CORBA_Environment &env);
  // This is an auxiliar method for _this() and _narrow().

  virtual void _dispatch (CORBA::ServerRequest &request,
                          void *context,
                          CORBA::Environment &env);
  // Turns around and calls invoke
};

#endif /* SERVANT_BASE_H */
