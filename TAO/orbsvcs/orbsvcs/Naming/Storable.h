// $Id$
// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//   Storable.h
//
// = AUTHOR
//    Bruce Trask <trask_b@ociweb.com>
//    Chanaka Liyanaarachchi <chanaka@ociweb.com>
//
// ============================================================================

#ifndef STORABLE_H
#define STORABLE_H

#include "tao/corba.h"

const int szInt =  sizeof(int);
const int szChar = sizeof(char);
const int szBool = sizeof(int);

class TAO_Writer_Base;
class TAO_Reader_Base;

class TAO_Storable_Base
{
public:
  TAO_Storable_Base();

  TAO_Storable_Base (TAO_Reader_Base&);

  virtual ~TAO_Storable_Base();

  virtual void Write(TAO_Writer_Base&) = 0;

};

class TAO_NS_Persistence_Header
{
 public:
  void size (unsigned int size);
  unsigned int size () const ;

  void context_name (const ACE_CString& context_name);
  ACE_CString context_name () const;

  private:
  unsigned int size_;
  ACE_CString context_name_;
};

class TAO_NS_Persistence_Record
{
 public:
  enum Record_Type {NCONTEXT, OBJREF};

  Record_Type type () const;
  void type (Record_Type type);

  ACE_CString id () const;
  void id (const ACE_CString& id);

  ACE_CString kind () const;
  void kind (const ACE_CString& kind);

  ACE_CString ior () const;
  void ior (const ACE_CString& ior);

  ACE_CString context_binding () const;
  void context_binding (const ACE_CString& context_binding);

 private:
  Record_Type type_;
  ACE_CString id_;
  ACE_CString kind_;
  ACE_CString ior_;
  ACE_CString context_binding_;
};

class TAO_Writer_Base
{
 public:
  virtual ~TAO_Writer_Base();
  TAO_Writer_Base();

  virtual int open (const char* name) = 0;
  virtual int close () = 0;

  virtual TAO_Writer_Base& operator << (
              const TAO_NS_Persistence_Header& header)  = 0;

  virtual TAO_Writer_Base& operator << (
              const TAO_NS_Persistence_Record& record) = 0;



 public:
  int delete_bindings_;
  // this is used to indicate to the Writer that the bindings map is now
  // empty and therefore now persistence resources are needed for it.

};

class TAO_Reader_Base
{
 public:
  TAO_Reader_Base ();
  virtual ~TAO_Reader_Base ();

  virtual int open (const char* name) = 0;
  virtual int close () = 0;

  virtual TAO_Reader_Base& operator >> (
              TAO_NS_Persistence_Header& header)  = 0;

  virtual TAO_Reader_Base& operator >> (
              TAO_NS_Persistence_Record& record) = 0;



};

class TAO_Naming_Service_Persistence_Factory
{
public:
  TAO_Naming_Service_Persistence_Factory();

  virtual ~TAO_Naming_Service_Persistence_Factory();

  // Factory Methods

  virtual TAO_Reader_Base *create_reader() = 0;
  virtual TAO_Writer_Base *create_writer() = 0;
};

#if defined (__ACE_INLINE__)
#include "Storable.inl"
#endif /* __ACE_INLINE_ */

#endif


