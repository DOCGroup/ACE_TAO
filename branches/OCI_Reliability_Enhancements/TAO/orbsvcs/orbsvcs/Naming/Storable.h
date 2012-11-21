// -*- C++ -*-

//=============================================================================
/**
 *  @file   Storable.h
 *
 *  $Id$
 *
 *  @author Bruce Trask <trask_b@ociweb.com>
 *  @author Chanaka Liyanaarachchi <chanaka@ociweb.com>
 */
//=============================================================================


#ifndef TAO_NAMING_STORABLE_H
#define TAO_NAMING_STORABLE_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"
#include "ace/SString.h"
#include "orbsvcs/Naming/naming_serv_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

static const int szInt =  sizeof(int);
static const int szChar = sizeof(char);
static const int szBool = sizeof(int);

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
  enum Record_Type {LOCAL_NCONTEXT, OBJREF, REMOTE_NCONTEXT};

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

class TAO_Naming_Serv_Export TAO_NS_Persistence_Global
{
 public:
  void counter (unsigned int counter);
  unsigned int counter () const ;

  private:
  unsigned int counter_;
};

class TAO_Naming_Serv_Export TAO_Storable_Base
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

  // Mimic a portion of the std::ios interface.  We need to be able
  // to indicate error states from the extraction operators below.
  enum Storable_State { goodbit = 0,
                        badbit  = 1,
                        eofbit  = 2,
                        failbit = 4
                      };

  void clear (Storable_State state = goodbit);

  void setstate (Storable_State state);

  Storable_State rdstate (void) const;

  bool good (void) const;

  bool bad (void) const;

  bool eof (void) const;

  bool fail (void) const;

  virtual TAO_Storable_Base& operator << (
              const TAO_NS_Persistence_Header& header) = 0;

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

  virtual TAO_Storable_Base& operator << (
		  	      const ACE_CString& str) = 0;

  virtual TAO_Storable_Base& operator >> (
              ACE_CString& str) = 0;

private:
  Storable_State state_;
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

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Naming/Storable.inl"
#endif /* __ACE_INLINE_ */

#endif  /* TAO_NAMING_STORABLE_H */
