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
public:
  virtual ~TAO_ServantBase (void);
  // destructor

  TAO_ServantBase &operator= (const TAO_ServantBase &);
  // assignment operator.

  virtual PortableServer::POA_ptr _default_POA (CORBA::Environment &env);

  virtual void dispatch (CORBA::ServerRequest &_tao_request,
			 void *_tao_context,
			 CORBA::Environment &_tao_environment);
  // Dispatches a request to the object: find the operation, cast
  // the type to the most derived type, demarshall all the
  // parameters from the request and finally invokes the operation,
  // storing the results and out parameters (if any) or the
  // exceptions thrown into <_tao_request>.
  // @@ TODO use a conformant name; since it is an
  // internal (implementation) method its name should start with '_'

  virtual int find (const CORBA::String &opname,
		    TAO_Skeleton &skelfunc);
  // Find an operation in the operation table.
  // @@ TODO use a conformant name; since it is an
  // internal (implementation) method its name should start with '_'

  virtual int bind (const CORBA::String &opname,
		    const TAO_Skeleton skel_ptr);
  // Register a CORBA IDL operation name.
  // @@ TODO use a conformant name; since it is an
  // internal (implementation) method its name should start with '_'

  TAO_IUnknown *get_parent (void) const;
  // Get the "parent" in the QueryInterface hierarchy.

  virtual const char *_interface_repository_id (void) const = 0;
  // Get this interface's repository id

  virtual void *_downcast (const char *repository_id) = 0;
  // Get the correct vtable

protected:
  TAO_ServantBase (void);
  // Default constructor, only derived classes can be created.

  TAO_ServantBase (const TAO_ServantBase &);
  // Copy constructor, protected so no instances can be created.

  void set_parent (TAO_IUnknown *p);
  // Set the "parent" in the QueryInterface hierarchy.
  // @@ TODO use a conformant name; since it is an
  // internal (implementation) method its name should start with '_'

protected:
  STUB_Object *_create_stub (CORBA_Environment &_env);
  // This is an auxiliar method for _this().

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
  STUB_Object *_create_stub (CORBA_Environment &_env);
  // This is an auxiliar method for _this().  Make sure *not* to
  // register with the default POA
};

#endif /* SERVANT_BASE_H */
