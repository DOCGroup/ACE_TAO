// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Servant_Base.h
//
// = AUTHOR
//     Irfan Pyarali
//
// ============================================================================

#if !defined (SERVANT_BASE_H)
#define SERVANT_BASE_H

class TAO_Export TAO_ServantBase
  // = TITLE
  //   Base class for skeletons and servants.
  //
  // = DESCRIPTION
  //   The POA spec requires that all servants inherit from this
  //   class.
{
  friend class TAO_POA;
  friend class CORBA_Object;
public:
  virtual ~TAO_ServantBase (void);
  // destructor

  TAO_ServantBase &operator= (const TAO_ServantBase &);
  // assignment operator.

  virtual PortableServer::POA_ptr _default_POA (CORBA::Environment &env);
  // Returns the default POA for this servant.

  virtual void *_downcast (const char *repository_id) = 0;
  // Get the correct vtable.

  virtual STUB_Object *_create_stub (CORBA_Environment &env);
  // This is an auxiliar method for _this() and _narrow().

protected:
  TAO_ServantBase (void);
  // Default constructor, only derived classes can be created.

  TAO_ServantBase (const TAO_ServantBase &);
  // Copy constructor, protected so no instances can be created.

  virtual CORBA::Boolean _is_a (const char* logical_type_id,
				CORBA::Environment &env);
  // Local implementation of the CORBA::Object::_is_a method.

  virtual void _dispatch (CORBA::ServerRequest &request,
                          void *context,
                          CORBA::Environment &env);
  // Dispatches a request to the object: find the operation, cast the
  // type to the most derived type, demarshall all the parameters from
  // the request and finally invokes the operation, storing the
  // results and out parameters (if any) or the exceptions thrown into
  // <request>.

  virtual int _find (const char *opname,
                     TAO_Skeleton &skelfunc);
  // Find an operation in the operation table.

  virtual int _bind (const char *opname,
                     const TAO_Skeleton skel_ptr);
  // Register a CORBA IDL operation name.

  virtual const char *_interface_repository_id (void) const = 0;
  // Get this interface's repository id.

protected:
  TAO_Operation_Table *optable_;
  // The operation table for this servant, it is initialized by the
  // most derived class.
};

class TAO_Export TAO_Local_ServantBase : public TAO_ServantBase
  // = TITLE
  //   Base class for local skeletons and servants.
  //
{
protected:
  STUB_Object *_create_stub (CORBA_Environment &env);
  // This is an auxiliar method for _this().  Make sure *not* to
  // register with the default POA
};

class TAO_Export TAO_DynamicImplementation : public virtual TAO_ServantBase
  // = TITLE
  //   Base class for DSI.
  //
  // = DESCRIPTION

  //   It is expected that the invoke() and _primary_interface()
  //   methods will be only invoked by the POA in the context of
  //   serving a CORBA request. Invoking this method in other
  //   circumstances may lead to unpredictable results.
{
public:
  virtual void invoke (CORBA::ServerRequest_ptr request) = 0;
  // The invoke() method receives requests issued to any CORBA object
  // incarnated by the DSI servant and performs the processing
  // necessary to execute the request.

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
  // Turns around and calls invoke.
};

#endif /* SERVANT_BASE_H */
