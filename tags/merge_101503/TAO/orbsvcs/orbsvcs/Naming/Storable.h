//=============================================================================
/**
 *  @file   Storable.h
 *
 *  $Id$
 *
 *  @author Bruce Trask <trask_b@ociweb.com> Chanaka Liyanaarachchi <chanaka@ociweb.com>
 */
//=============================================================================


#ifndef STORABLE_H
#define STORABLE_H

#include "tao/corba.h"
#include "ace/SString.h"

const int szInt =  sizeof(int);
const int szChar = sizeof(char);
const int szBool = sizeof(int);

class TAO_NS_Persistence_Header
{
 public:
  void size (unsigned int size);
  unsigned int size () const ;

  void destroyed (int flag);
  int destroyed () const ;

  private:
  unsigned int size_;
  int destroyed_;
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

  ACE_CString ref () const;
  void ref (const ACE_CString& ior);

 private:
  Record_Type type_;
  ACE_CString id_;
  ACE_CString kind_;
  ACE_CString ref_;
};

class TAO_NS_Persistence_Global
{
 public:
  void counter (unsigned int counter);
  unsigned int counter () const ;

  private:
  unsigned int counter_;
};
  
class TAO_Storable_Base
{
public:
  TAO_Storable_Base();

  virtual ~TAO_Storable_Base();

  virtual void remove() = 0;

  virtual int exists() = 0;

  virtual int open () = 0;

  virtual int close () = 0;

  virtual int flock (int whence, int start, int len) = 0;

  virtual int funlock (int whence, int start, int len) = 0;

  virtual time_t last_changed(void) = 0;

  virtual TAO_Storable_Base& operator << (
              const TAO_NS_Persistence_Header& header)  = 0;

  virtual TAO_Storable_Base& operator << (
              const TAO_NS_Persistence_Record& record) = 0;

  virtual TAO_Storable_Base& operator >> (
              TAO_NS_Persistence_Header& header)  = 0;

  virtual TAO_Storable_Base& operator >> (
              TAO_NS_Persistence_Record& record) = 0;

  virtual TAO_Storable_Base& operator << (
              const TAO_NS_Persistence_Global& global) = 0;

  virtual TAO_Storable_Base& operator >> (
              TAO_NS_Persistence_Global& global)  = 0;

};
  
class TAO_Naming_Service_Persistence_Factory
{
public:
  TAO_Naming_Service_Persistence_Factory();

  virtual ~TAO_Naming_Service_Persistence_Factory();

  // Factory Methods

  virtual TAO_Storable_Base *create_stream(const ACE_CString & file,
                                           const ACE_TCHAR * mode) = 0;
};

#if defined (__ACE_INLINE__)
#include "Storable.inl"
#endif /* __ACE_INLINE_ */

#endif
