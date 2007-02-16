// -*- C++ -*-

#ifndef OPERATION_TABLE_H
#define OPERATION_TABLE_H

#include <ace/Synch.h>
#include <tao/NVList.h>
#include <tao/Typecode.h>
//
#include "tao/IFR_Client/IFR_BasicC.h"

//***************************************************************************
//
// Arg encapuslates a single argument's details.

class Arg
{
public:
  Arg ();
  ~Arg (void);

  void init (CORBA::TypeCode_ptr tc, CORBA::Flags m);

  // tc_ is the type code for the arugment. It may be arbirarily complex.
  // In the future, it might be resonable to further pre-evaluate complex
  // type codes to tag individual elements as having an object reference
  // or not, but for now we just maintain that for the whole argument.
  CORBA::TypeCode_ptr tc_;

  // mode_ is the data-direction flag. It is set for IN, INOUT, OUT args.
  // It is not set for return values. Mode_ is used along with tc_ to
  // populate arg NVLists to handle requests.
  CORBA::Flags mode_;

  // This flag is set by the ObjectMap class. If negative, then the
  // status is unknown and must be set. If positive, then the argument
  // must be processed to proxify a contained object reference. Complex
  // types (structs, unions, etc.) must be traversed at parse time to
  // locate the object references within.
  int must_proxy_;
};

//***************************************************************************
//
// ArgList is a reference counted object, it may be associated with more
// than one interface/operation. Such a multiplicity will happen whenever
// an interface is a known base of other interfaces, in which case two or
// more interfaces will have the same operation with the same arguments.

class ArgList
{
public:
  ArgList ();
  ArgList (CORBA::ULong argc,
              CORBA::ULong excepc,
              int oneway);
  ~ArgList (void);

  void init (CORBA::ULong argc,
             CORBA::ULong excepc,
             int oneway);

  void prepare_request(CORBA::NVList_ptr args,
                       CORBA::TypeCode_ptr &result_tc,
                       int &is_oneway);

  Arg * find_exception (const char * id,
                           CORBA::Any *& value);

  // Reference count manipulators. Object will destroy self when count
  // goes to zero
  int add_ref (void);
  int remove_ref (void);

  // The must_proxy flag is a short-hand way to determine if *any* arguments
  // need to be proxified. This isn't known until the first time the operation
  // is actually traversed by the ObjectMap. Since all operations on an
  // interface are added to this collection whenever the interface repository
  // is queried, it is possible that any particular operation is never
  // invoked.
  int must_proxy(CORBA::Flags) const;
  void must_proxy (CORBA::Flags,int );

  // Returns true if the associated operation is a one-way
  int is_oneway (void) const;

  // The details for the return type, which may be null.
  void result (CORBA::TypeCode_ptr tc);
  Arg& result ();

  // Access to the list of arguments for this operation
  CORBA::ULong num_args(void) const;
  void set_arg (CORBA::ULong ndx, CORBA::TypeCode_ptr, CORBA::Flags);
  Arg *args (void);

  // Access to the list of user exceptions for this operation
  CORBA::ULong num_exceps(void) const;
  void set_excep (CORBA::ULong ndx, CORBA::TypeCode_ptr);
  Arg *exceps (void);

private:
  int must_proxy_[2];
  Arg result_;
  CORBA::ULong num_args_;
  Arg *args_;
  CORBA::ULong num_exceptions_;
  Arg *exceptions_;

  int is_oneway_;

  int refcount_;
};

//***************************************************************************
//
// OpArgs is keyed by interface repository name, and includes the
// particulars of an operation in an interface, including the oneway flag,
// result value typecode, arg mode & type. Also, the OpArgs contains the
// proxification requirements for the arguments.
//

class OpArgs
{
public:
  OpArgs (const char *ref_id, ArgList *args = 0);
  ~OpArgs (void);

  ArgList *args (void);

  const char * id (void) const;

  OpArgs *next (void);
  void next (OpArgs *);

private:
  OpArgs *next_;
  ArgList *args_;
  char * id_;

};

//***************************************************************************
//
// Operation has a name, and participates in a balanced tree. The
// operation also has a collection interfaces that have an operation by
// this name. This includes base interfaces and happenstance names
//

class Operation
{
public:
  Operation (const char *name);
  ~Operation (void);

  ArgList *arg_list (const char * rep_id);

  const char * name(void) const;
  Operation *left(void);
  Operation *right(void);

  void left (Operation *o);
  void right (Operation *o);

  OpArgs *args (const char *rep_id);
  void add_set (OpArgs *arglist);

private:
  char *name_;
  Operation *left_;
  Operation *right_;
  OpArgs *arg_set_;
};

//***************************************************************************
//
// OperationTable is a collection Operation objects, sorted by opname
// with a few helper methods.

class OperationTable
{
public:
  OperationTable ();
  ~OperationTable ();

  ArgList * find (const char * opname,
                     const char * rep_id);

  ArgList * add_interface (CORBA::InterfaceDef_ptr intDef,
                              const char * opname = 0);

private:
  ArgList * add_is_a (const char *rep_id);
  ArgList * add_non_existent (const char *rep_id);

  Operation * find_or_add (const char *opname);

  Operation *head_;
  ACE_RW_Thread_Mutex lock_;
};


#endif
