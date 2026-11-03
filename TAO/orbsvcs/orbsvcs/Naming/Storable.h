// -*- C++ -*-

//=============================================================================
/**
 *  @file   Storable.h
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

#include "tao/Storable_Base.h"
#include <string>
#include "orbsvcs/Naming/naming_serv_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

constexpr int szInt = sizeof(int);
constexpr int szChar = sizeof(char);
constexpr int szBool = sizeof(int);

class TAO_NS_Persistence_Header
{
 public:
  void size (unsigned int size);
  unsigned int size () const ;

  void destroyed (int flag);
  int destroyed () const ;

  private:
  unsigned int size_ {};
  int destroyed_ {};
};

class TAO_NS_Persistence_Record
{
 public:
  enum Record_Type {UNDEFINED, LOCAL_NCONTEXT, OBJREF, REMOTE_NCONTEXT};

  TAO_NS_Persistence_Record (Record_Type type = TAO_NS_Persistence_Record::UNDEFINED);

  Record_Type type () const;
  void type (Record_Type type);

  std::string const & id () const;
  void id (std::string const &id);

  std::string const & kind () const;
  void kind (std::string const &kind);

  std::string const & ref () const;
  void ref (std::string const &ior);

 private:
  Record_Type type_;
  std::string id_;
  std::string kind_;
  std::string ref_;
};

class TAO_Naming_Serv_Export TAO_NS_Persistence_Global
{
 public:
  void counter (unsigned int counter);
  unsigned int counter () const ;

  private:
  unsigned int counter_ {};
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Naming/Storable.inl"
#endif /* __ACE_INLINE_ */

#endif  /* TAO_NAMING_STORABLE_H */
