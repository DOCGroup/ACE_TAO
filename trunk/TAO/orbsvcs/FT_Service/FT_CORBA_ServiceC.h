/* -*- C++ -*- $Id$ */
// ============================================================================
//
// = LIBRARY
//     FT_Service(aka. DOORS, CASO etc.)
//
// = FILENAME
//     FT_CORBA_ServiceC.h
//
// = DESCRIPTION
//     This is the generated code by the IDL compiler for the FT
//     specification. The generated code here would be used by the
//     service layer. The service layer essentially consists of the
//     Replication Manager, Property Manager, Generic Factory, Fault
//     Monitors, and Fault Notfifiers.
//
// = AUTHOR
//     Balachandran Natarajan <bala@cs.wustl.edu>
//
// ============================================================================
#ifndef TAO_FT_CORBA_SERVICEC_H
#define TAO_FT_CORBA_SERVICEC_H

#include "ace/pre.h"
#include "tao/FT_CORBAC.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/CosNotificationC.h"
#include "orbsvcs/CosNotifyFilterC.h"
#include "orbsvcs/CosNotifyCommC.h"

#if (TAO_HAS_FT_CORBA == 1)

#if defined (ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION)
#include "ace/streams.h"
#endif /* ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION */

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_EXPORT_MACRO)
#undef TAO_EXPORT_MACRO
#endif
#define TAO_EXPORT_MACRO TAO_Export

#if defined (TAO_EXPORT_NESTED_CLASSES)
#  if defined (TAO_EXPORT_NESTED_MACRO)
#    undef TAO_EXPORT_NESTED_MACRO
#  endif /* defined (TAO_EXPORT_NESTED_MACRO) */
#  define TAO_EXPORT_NESTED_MACRO TAO_Export
#endif /* TAO_EXPORT_NESTED_CLASSES */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */


class TAO_Export FT_GenericFactory_var : public TAO_Base_var
{
public:
  FT_GenericFactory_var (void); // default constructor
  FT_GenericFactory_var (FT_GenericFactory_ptr p) : ptr_ (p) {}
  FT_GenericFactory_var (const FT_GenericFactory_var &); // copy constructor
  ~FT_GenericFactory_var (void); // destructor

  FT_GenericFactory_var &operator= (FT_GenericFactory_ptr);
  FT_GenericFactory_var &operator= (const FT_GenericFactory_var &);
  FT_GenericFactory_ptr operator-> (void) const;

  operator const FT_GenericFactory_ptr &() const;
  operator FT_GenericFactory_ptr &();
  // in, inout, out, _retn
  FT_GenericFactory_ptr in (void) const;
  FT_GenericFactory_ptr &inout (void);
  FT_GenericFactory_ptr &out (void);
  FT_GenericFactory_ptr _retn (void);
  FT_GenericFactory_ptr ptr (void) const;

private:
  FT_GenericFactory_ptr ptr_;
  // Unimplemented - prevents widening assignment.
  FT_GenericFactory_var (const TAO_Base_var &rhs);
  FT_GenericFactory_var &operator= (const TAO_Base_var &rhs);
};



class TAO_Export FT_GenericFactory_out
{
public:
  FT_GenericFactory_out (FT_GenericFactory_ptr &);
  FT_GenericFactory_out (FT_GenericFactory_var &);
  FT_GenericFactory_out (const FT_GenericFactory_out &);
  FT_GenericFactory_out &operator= (const FT_GenericFactory_out &);
  FT_GenericFactory_out &operator= (const FT_GenericFactory_var &);
  FT_GenericFactory_out &operator= (FT_GenericFactory_ptr);
  operator FT_GenericFactory_ptr &();
  FT_GenericFactory_ptr &ptr (void);
  FT_GenericFactory_ptr operator-> (void);

private:
  FT_GenericFactory_ptr &ptr_;
};


class TAO_Export FT_FaultNotifier_var : public TAO_Base_var
{
public:
  FT_FaultNotifier_var (void); // default constructor
  FT_FaultNotifier_var (FT_FaultNotifier_ptr p) : ptr_ (p) {}
  FT_FaultNotifier_var (const FT_FaultNotifier_var &); // copy constructor
  ~FT_FaultNotifier_var (void); // destructor

  FT_FaultNotifier_var &operator= (FT_FaultNotifier_ptr);
  FT_FaultNotifier_var &operator= (const FT_FaultNotifier_var &);
  FT_FaultNotifier_ptr operator-> (void) const;

  operator const FT_FaultNotifier_ptr &() const;
  operator FT_FaultNotifier_ptr &();
  // in, inout, out, _retn
  FT_FaultNotifier_ptr in (void) const;
  FT_FaultNotifier_ptr &inout (void);
  FT_FaultNotifier_ptr &out (void);
  FT_FaultNotifier_ptr _retn (void);
  FT_FaultNotifier_ptr ptr (void) const;

private:
  FT_FaultNotifier_ptr ptr_;
  // Unimplemented - prevents widening assignment.
  FT_FaultNotifier_var (const TAO_Base_var &rhs);
  FT_FaultNotifier_var &operator= (const TAO_Base_var &rhs);
};



class TAO_Export FT_FaultNotifier_out
{
public:
  FT_FaultNotifier_out (FT_FaultNotifier_ptr &);
  FT_FaultNotifier_out (FT_FaultNotifier_var &);
  FT_FaultNotifier_out (const FT_FaultNotifier_out &);
  FT_FaultNotifier_out &operator= (const FT_FaultNotifier_out &);
  FT_FaultNotifier_out &operator= (const FT_FaultNotifier_var &);
  FT_FaultNotifier_out &operator= (FT_FaultNotifier_ptr);
  operator FT_FaultNotifier_ptr &();
  FT_FaultNotifier_ptr &ptr (void);
  FT_FaultNotifier_ptr operator-> (void);

private:
  FT_FaultNotifier_ptr &ptr_;
};


// This is a hack that has been used here to get around the
// problem. For the user to use FT::Name we need to have this sort of
// hack. May not be the right thing to do. But ok for now.

class TAO_Export FT_Name : public CosNaming::Name
{
 public:
  FT_Name (void); // default ctor
  FT_Name (CORBA::ULong max); // uses max size
  FT_Name (
           CORBA::ULong max,
           CORBA::ULong length,
           CosNaming::NameComponent *buffer,
           CORBA::Boolean release=0
           );
  FT_Name (const FT_Name &); // copy ctor
  ~FT_Name (void);
  static void _tao_any_destructor (void*);

#if !defined(__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)
  typedef FT_Name_var _var_type;
#endif /* ! __GNUC__ || g++ >= 2.8 */

};

// *************************************************************
// class FT_Name_var
// *************************************************************

class TAO_Export FT_Name_var
{
public:
  FT_Name_var (void); // default constructor
  FT_Name_var (FT_Name *);
  FT_Name_var (const FT_Name_var &); // copy constructor
  ~FT_Name_var (void); // destructor

  FT_Name_var &operator= (FT_Name *);
  FT_Name_var &operator= (const FT_Name_var &);
  FT_Name *operator-> (void);
  const FT_Name *operator-> (void) const;

  operator const FT_Name &() const;
  operator FT_Name &();
  operator FT_Name &() const;
  operator FT_Name *&(); // variable-size base types only

  CosNaming::NameComponent &operator[] (CORBA::ULong index);
  // in, inout, out, _retn
  const FT_Name &in (void) const;
  FT_Name &inout (void);
  FT_Name *&out (void);
  FT_Name *_retn (void);
  FT_Name *ptr (void) const;

private:
  FT_Name *ptr_;
};


class TAO_Export FT_Name_out
{
public:
  FT_Name_out (FT_Name *&);
  FT_Name_out (FT_Name_var &);
  FT_Name_out (const FT_Name_out &);
  FT_Name_out &operator= (const FT_Name_out &);
  FT_Name_out &operator= (FT_Name *);
  operator FT_Name *&();
  FT_Name *&ptr (void);
  FT_Name *operator-> (void);
  CosNaming::NameComponent &operator[] (CORBA::ULong index);

private:
  FT_Name *&ptr_;
  // assignment from T_var not allowed
  void operator= (const FT_Name_var &);
};


struct TAO_Export FT_Property
{

#if !defined(__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)
  typedef FT_Property_var _var_type;
#endif /* ! __GNUC__ || g++ >= 2.8 */

  static void _tao_any_destructor (void*);

  FT_Name nam;
  FT::Value val;
};

class TAO_Export FT_Property_var
{
public:
  FT_Property_var (void); // default constructor
  FT_Property_var (FT_Property *);
  FT_Property_var (const FT_Property_var &); // copy constructor
  ~FT_Property_var (void); // destructor

  FT_Property_var &operator= (FT_Property *);
  FT_Property_var &operator= (const FT_Property_var &);
  FT_Property *operator-> (void);
  const FT_Property *operator-> (void) const;

  operator const FT_Property &() const;
  operator FT_Property &();
  operator FT_Property &() const;
  operator FT_Property *&(); // variable-size types only

  // in, inout, out, _retn
  const FT_Property &in (void) const;
  FT_Property &inout (void);
  FT_Property *&out (void);
  FT_Property *_retn (void);
  FT_Property *ptr (void) const;

private:
  FT_Property *ptr_;
};

class TAO_Export FT_Property_out
{
public:
  FT_Property_out (FT_Property *&);
  FT_Property_out (FT_Property_var &);
  FT_Property_out (const FT_Property_out &);
  FT_Property_out &operator= (const FT_Property_out &);
  FT_Property_out &operator= (FT_Property *);
  operator FT_Property *&();
  FT_Property *&ptr (void);
  FT_Property *operator-> (void);

private:
  FT_Property *&ptr_;
  // assignment from T_var not allowed
  void operator= (const FT_Property_var &);
};


#if !defined (TAO_USE_SEQUENCE_TEMPLATES)

class TAO_EXPORT_NESTED_MACRO _TAO_Unbounded_Sequence_FT_Properties : public TAO_Unbounded_Base_Sequence
{
public:
  // = Initialization and termination methods.

  _TAO_Unbounded_Sequence_FT_Properties (void); // Default constructor.
  _TAO_Unbounded_Sequence_FT_Properties (CORBA::ULong maximum);
  _TAO_Unbounded_Sequence_FT_Properties (CORBA::ULong maximum,
                                         CORBA::ULong length,
                                         FT_Property *data,
                                         CORBA::Boolean release = 0);
  _TAO_Unbounded_Sequence_FT_Properties (const _TAO_Unbounded_Sequence_FT_Properties &rhs);
  _TAO_Unbounded_Sequence_FT_Properties &operator= (const _TAO_Unbounded_Sequence_FT_Properties &rhs);
  virtual ~_TAO_Unbounded_Sequence_FT_Properties (void); // Dtor.
  // = Accessors.
  FT_Property &operator[] (CORBA::ULong i);
  const FT_Property &operator[] (CORBA::ULong i) const;
  // = Static operations.
  static FT_Property *allocbuf (CORBA::ULong size);
  static void freebuf (FT_Property *buffer);
  virtual void _allocate_buffer (CORBA::ULong length);
  virtual void _deallocate_buffer (void);
  // Implement the TAO_Base_Sequence methods (see Sequence.h)

  FT_Property *get_buffer (CORBA::Boolean orphan = 0);
  const FT_Property *get_buffer (void) const;
  void replace (CORBA::ULong max,
                CORBA::ULong length,
                FT_Property *data,
                CORBA::Boolean release);
};


#endif /* !TAO_USE_SEQUENCE_TEMPLATES */


// *************************************************************
// Properties
// *************************************************************

class TAO_Export FT_Properties : public
#if !defined (TAO_USE_SEQUENCE_TEMPLATES)
_TAO_Unbounded_Sequence_FT_Properties
#else /* TAO_USE_SEQUENCE_TEMPLATES */
TAO_Unbounded_Sequence<FT_Property>
#endif /* !TAO_USE_SEQUENCE_TEMPLATES */
{
 public:
  FT_Properties (void); // default ctor
  FT_Properties (CORBA::ULong max); // uses max size
  FT_Properties (
                 CORBA::ULong max,
                 CORBA::ULong length,
                 FT_Property *buffer,
                 CORBA::Boolean release=0
                 );
  FT_Properties (const FT_Properties &); // copy ctor
  ~FT_Properties (void);
  static void _tao_any_destructor (void*);

#if !defined(__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)
  typedef FT_Properties_var _var_type;
#endif /* ! __GNUC__ || g++ >= 2.8 */

};



// *************************************************************
// class FT::Properties_var
// *************************************************************

class TAO_Export FT_Properties_var
{
public:
  FT_Properties_var (void); // default constructor
  FT_Properties_var (FT_Properties *);
  FT_Properties_var (const FT_Properties_var &); // copy constructor
  ~FT_Properties_var (void); // destructor

  FT_Properties_var &operator= (FT_Properties *);
  FT_Properties_var &operator= (const FT_Properties_var &);
  FT_Properties *operator-> (void);
  const FT_Properties *operator-> (void) const;

  operator const FT_Properties &() const;
  operator FT_Properties &();
  operator FT_Properties &() const;
  operator FT_Properties *&(); // variable-size base types only

  FT_Property &operator[] (CORBA::ULong index);
  // in, inout, out, _retn
  const FT_Properties &in (void) const;
  FT_Properties &inout (void);
  FT_Properties *&out (void);
  FT_Properties *_retn (void);
  FT_Properties *ptr (void) const;

private:
  FT_Properties *ptr_;
};

class TAO_Export FT_Properties_out
{
public:
  FT_Properties_out (FT_Properties *&);
  FT_Properties_out (FT_Properties_var &);
  FT_Properties_out (const FT_Properties_out &);
  FT_Properties_out &operator= (const FT_Properties_out &);
  FT_Properties_out &operator= (FT_Properties *);
  operator FT_Properties *&();
  FT_Properties *&ptr (void);
  FT_Properties *operator-> (void);
  FT_Property &operator[] (CORBA::ULong index);

private:
  FT_Properties *&ptr_;
  // assignment from T_var not allowed
  void operator= (const FT_Properties_var &);
};



#if !defined (TAO_USE_SEQUENCE_TEMPLATES)

class TAO_EXPORT_NESTED_MACRO _TAO_Unbounded_Sequence_FT_Locations :
  public TAO_Unbounded_Base_Sequence
{
public:
  // = Initialization and termination methods.

  _TAO_Unbounded_Sequence_FT_Locations (void); // Default constructor.
  _TAO_Unbounded_Sequence_FT_Locations (CORBA::ULong maximum);
  _TAO_Unbounded_Sequence_FT_Locations (CORBA::ULong maximum,
                                        CORBA::ULong length,
                                        FT::Location *data,
                                        CORBA::Boolean release = 0);
  _TAO_Unbounded_Sequence_FT_Locations (const _TAO_Unbounded_Sequence_FT_Locations &rhs);
  _TAO_Unbounded_Sequence_FT_Locations &operator= (const _TAO_Unbounded_Sequence_FT_Locations &rhs);
  virtual ~_TAO_Unbounded_Sequence_FT_Locations (void); // Dtor.
  // = Accessors.
  FT::Location &operator[] (CORBA::ULong i);
  const FT::Location &operator[] (CORBA::ULong i) const;
  // = Static operations.
  static FT::Location *allocbuf (CORBA::ULong size);
  static void freebuf (FT::Location *buffer);
  virtual void _allocate_buffer (CORBA::ULong length);
  virtual void _deallocate_buffer (void);
  // Implement the TAO_Base_Sequence methods (see Sequence.h)

  FT::Location *get_buffer (CORBA::Boolean orphan = 0);
  const FT::Location *get_buffer (void) const;
  void replace (CORBA::ULong max,
                CORBA::ULong length,
                FT::Location *data,
                CORBA::Boolean release);
};

#endif /* !TAO_USE_SEQUENCE_TEMPLATES */

// *************************************************************
// Locations
// *************************************************************

class TAO_Export FT_Locations : public
#if !defined (TAO_USE_SEQUENCE_TEMPLATES)
_TAO_Unbounded_Sequence_FT_Locations
#else /* TAO_USE_SEQUENCE_TEMPLATES */
TAO_Unbounded_Sequence<FT::Location>
#endif /* !TAO_USE_SEQUENCE_TEMPLATES */
{
 public:
  FT_Locations (void); // default ctor
  FT_Locations (CORBA::ULong max); // uses max size
  FT_Locations (
                CORBA::ULong max,
                CORBA::ULong length,
                FT::Location *buffer,
                CORBA::Boolean release=0
                );
  FT_Locations (const FT_Locations &); // copy ctor
  ~FT_Locations (void);
  static void _tao_any_destructor (void*);

#if !defined(__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)
  typedef FT_Locations_var _var_type;
#endif /* ! __GNUC__ || g++ >= 2.8 */

};



// *************************************************************
// class FT::Locations_var
// *************************************************************

class TAO_Export FT_Locations_var
{
public:
  FT_Locations_var (void); // default constructor
  FT_Locations_var (FT_Locations *);
  FT_Locations_var (const FT_Locations_var &); // copy constructor
  ~FT_Locations_var (void); // destructor

  FT_Locations_var &operator= (FT_Locations *);
  FT_Locations_var &operator= (const FT_Locations_var &);
  FT_Locations *operator-> (void);
  const FT_Locations *operator-> (void) const;

  operator const FT_Locations &() const;
  operator FT_Locations &();
  operator FT_Locations &() const;
  operator FT_Locations *&(); // variable-size base types only

  FT::Location &operator[] (CORBA::ULong index);
  // in, inout, out, _retn
  const FT_Locations &in (void) const;
  FT_Locations &inout (void);
  FT_Locations *&out (void);
  FT_Locations *_retn (void);
  FT_Locations *ptr (void) const;

  private:
  FT_Locations *ptr_;
};


class TAO_Export FT_Locations_out
{
public:
  FT_Locations_out (FT_Locations *&);
  FT_Locations_out (FT_Locations_var &);
  FT_Locations_out (const FT_Locations_out &);
  FT_Locations_out &operator= (const FT_Locations_out &);
  FT_Locations_out &operator= (FT_Locations *);
  operator FT_Locations *&();
  FT_Locations *&ptr (void);
  FT_Locations *operator-> (void);
  //FT::Location &operator[] (CORBA::ULong index);

private:
  FT_Locations *&ptr_;
  // assignment from T_var not allowed
  void operator= (const FT_Locations_var &);
};


struct TAO_Export FT_FactoryInfo
{

#if !defined(__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)
    typedef FT_FactoryInfo_var _var_type;
#endif /* ! __GNUC__ || g++ >= 2.8 */

    static void _tao_any_destructor (void*);

    FT_GenericFactory_var factory;
    FT::Location the_location;
    FT_Criteria the_criteria;
};

class TAO_Export FT_FactoryInfo_var
{
public:
  FT_FactoryInfo_var (void); // default constructor
  FT_FactoryInfo_var (FT_FactoryInfo *);
  FT_FactoryInfo_var (const FT_FactoryInfo_var &); // copy constructor
  ~FT_FactoryInfo_var (void); // destructor

  FT_FactoryInfo_var &operator= (FT_FactoryInfo *);
  FT_FactoryInfo_var &operator= (const FT_FactoryInfo_var &);
  FT_FactoryInfo *operator-> (void);
  const FT_FactoryInfo *operator-> (void) const;

  operator const FT_FactoryInfo &() const;
  operator FT_FactoryInfo &();
  operator FT_FactoryInfo &() const;
  operator FT_FactoryInfo *&(); // variable-size types only

  // in, inout, out, _retn
  const FT_FactoryInfo &in (void) const;
  FT_FactoryInfo &inout (void);
  FT_FactoryInfo *&out (void);
  FT_FactoryInfo *_retn (void);
  FT_FactoryInfo *ptr (void) const;

private:
  FT_FactoryInfo *ptr_;
};

class TAO_Export FT_FactoryInfo_out
{
public:
  FT_FactoryInfo_out (FT_FactoryInfo *&);
  FT_FactoryInfo_out (FT_FactoryInfo_var &);
  FT_FactoryInfo_out (const FT_FactoryInfo_out &);
  FT_FactoryInfo_out &operator= (const FT_FactoryInfo_out &);
  FT_FactoryInfo_out &operator= (FT_FactoryInfo *);
  operator FT_FactoryInfo *&();
  FT_FactoryInfo *&ptr (void);
  FT_FactoryInfo *operator-> (void);

private:
  FT_FactoryInfo *&ptr_;
  // assignment from T_var not allowed
  void operator= (const FT_FactoryInfo_var &);
};


#if !defined (TAO_USE_SEQUENCE_TEMPLATES)

class TAO_EXPORT_NESTED_MACRO _TAO_Unbounded_Sequence_FT_FactoryInfos : public TAO_Unbounded_Base_Sequence
{
public:
  // = Initialization and termination methods.

  _TAO_Unbounded_Sequence_FT_FactoryInfos (void); // Default constructor.
  _TAO_Unbounded_Sequence_FT_FactoryInfos (CORBA::ULong maximum);
  _TAO_Unbounded_Sequence_FT_FactoryInfos (CORBA::ULong maximum,
                                           CORBA::ULong length,
                                           FT_FactoryInfo *data,
        CORBA::Boolean release = 0);
  _TAO_Unbounded_Sequence_FT_FactoryInfos (const _TAO_Unbounded_Sequence_FT_FactoryInfos &rhs);
  _TAO_Unbounded_Sequence_FT_FactoryInfos &operator= (const _TAO_Unbounded_Sequence_FT_FactoryInfos &rhs);
  virtual ~_TAO_Unbounded_Sequence_FT_FactoryInfos (void); // Dtor.
  // = Accessors.
  FT_FactoryInfo &operator[] (CORBA::ULong i);
  const FT_FactoryInfo &operator[] (CORBA::ULong i) const;
  // = Static operations.
  static FT_FactoryInfo *allocbuf (CORBA::ULong size);
  static void freebuf (FT_FactoryInfo *buffer);
  virtual void _allocate_buffer (CORBA::ULong length);
  virtual void _deallocate_buffer (void);
  // Implement the TAO_Base_Sequence methods (see Sequence.h)

  FT_FactoryInfo *get_buffer (CORBA::Boolean orphan = 0);
  const FT_FactoryInfo *get_buffer (void) const;
  void replace (CORBA::ULong max,
                CORBA::ULong length,
                FT_FactoryInfo *data,
                CORBA::Boolean release);
};



#endif /* !TAO_USE_SEQUENCE_TEMPLATES */

// *************************************************************
// FactoryInfos
// *************************************************************

class TAO_Export FT_FactoryInfos : public
#if !defined (TAO_USE_SEQUENCE_TEMPLATES)
_TAO_Unbounded_Sequence_FT_FactoryInfos
#else /* TAO_USE_SEQUENCE_TEMPLATES */
TAO_Unbounded_Sequence<FT_FactoryInfo>
#endif /* !TAO_USE_SEQUENCE_TEMPLATES */
{
 public:
  FT_FactoryInfos (void); // default ctor
  FT_FactoryInfos (CORBA::ULong max); // uses max size
  FT_FactoryInfos (
                   CORBA::ULong max,
                   CORBA::ULong length,
                   FT_FactoryInfo *buffer,
                   CORBA::Boolean release=0
                   );
  FT_FactoryInfos (const FT_FactoryInfos &); // copy ctor
  ~FT_FactoryInfos (void);
  static void _tao_any_destructor (void*);

#if !defined(__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)
  typedef FT_FactoryInfos_var _var_type;
#endif /* ! __GNUC__ || g++ >= 2.8 */

};


// *************************************************************
// class FT::FactoryInfos_var
// *************************************************************

class TAO_Export FT_FactoryInfos_var
{
public:
  FT_FactoryInfos_var (void); // default constructor
  FT_FactoryInfos_var (FT_FactoryInfos *);
  FT_FactoryInfos_var (const FT_FactoryInfos_var &); // copy constructor
  ~FT_FactoryInfos_var (void); // destructor

  FT_FactoryInfos_var &operator= (FT_FactoryInfos *);
  FT_FactoryInfos_var &operator= (const FT_FactoryInfos_var &);
  FT_FactoryInfos *operator-> (void);
  const FT_FactoryInfos *operator-> (void) const;

  operator const FT_FactoryInfos &() const;
  operator FT_FactoryInfos &();
  operator FT_FactoryInfos &() const;
  operator FT_FactoryInfos *&(); // variable-size base types only

  FT_FactoryInfo &operator[] (CORBA::ULong index);
  // in, inout, out, _retn
  const FT_FactoryInfos &in (void) const;
  FT_FactoryInfos &inout (void);
  FT_FactoryInfos *&out (void);
  FT_FactoryInfos *_retn (void);
  FT_FactoryInfos *ptr (void) const;

private:
  FT_FactoryInfos *ptr_;
};

class TAO_Export FT_FactoryInfos_out
{
public:
  FT_FactoryInfos_out (FT_FactoryInfos *&);
  FT_FactoryInfos_out (FT_FactoryInfos_var &);
  FT_FactoryInfos_out (const FT_FactoryInfos_out &);
  FT_FactoryInfos_out &operator= (const FT_FactoryInfos_out &);
  FT_FactoryInfos_out &operator= (FT_FactoryInfos *);
  operator FT_FactoryInfos *&();
  FT_FactoryInfos *&ptr (void);
  FT_FactoryInfos *operator-> (void);
  FT_FactoryInfo &operator[] (CORBA::ULong index);

private:
    FT_FactoryInfos *&ptr_;
  // assignment from T_var not allowed
    void operator= (const FT_FactoryInfos_var &);
};


struct TAO_Export FT_FaultMonitoringIntervalAndTimeoutValue
{

#if !defined(__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)
  typedef FT_FaultMonitoringIntervalAndTimeoutValue_var _var_type;
#endif /* ! __GNUC__ || g++ >= 2.8 */

  static void _tao_any_destructor (void*);

  TimeBase::TimeT monitoring_interval;
  TimeBase::TimeT timeout;
};

class TAO_Export FT_FaultMonitoringIntervalAndTimeoutValue_var
{
public:
  FT_FaultMonitoringIntervalAndTimeoutValue_var (void); // default constructor
  FT_FaultMonitoringIntervalAndTimeoutValue_var (FT_FaultMonitoringIntervalAndTimeoutValue *);
  FT_FaultMonitoringIntervalAndTimeoutValue_var (const FT_FaultMonitoringIntervalAndTimeoutValue_var &); // copy constructor
  FT_FaultMonitoringIntervalAndTimeoutValue_var (const FT_FaultMonitoringIntervalAndTimeoutValue &); // fixed-size types only
  ~FT_FaultMonitoringIntervalAndTimeoutValue_var (void); // destructor

  FT_FaultMonitoringIntervalAndTimeoutValue_var &operator= (FT_FaultMonitoringIntervalAndTimeoutValue *);
  FT_FaultMonitoringIntervalAndTimeoutValue_var &operator= (const FT_FaultMonitoringIntervalAndTimeoutValue_var &);
  FT_FaultMonitoringIntervalAndTimeoutValue_var &operator= (const FT_FaultMonitoringIntervalAndTimeoutValue &); // fixed-size types only
  FT_FaultMonitoringIntervalAndTimeoutValue *operator-> (void);
  const FT_FaultMonitoringIntervalAndTimeoutValue *operator-> (void) const;

  operator const FT_FaultMonitoringIntervalAndTimeoutValue &() const;
  operator FT_FaultMonitoringIntervalAndTimeoutValue &();
  operator FT_FaultMonitoringIntervalAndTimeoutValue &() const;

  // in, inout, out, _retn
  const FT_FaultMonitoringIntervalAndTimeoutValue &in (void) const;
  FT_FaultMonitoringIntervalAndTimeoutValue &inout (void);
  FT_FaultMonitoringIntervalAndTimeoutValue &out (void);
  FT_FaultMonitoringIntervalAndTimeoutValue _retn (void);
  FT_FaultMonitoringIntervalAndTimeoutValue *ptr (void) const;

private:
  FT_FaultMonitoringIntervalAndTimeoutValue *ptr_;
};

//typedef FT_FaultMonitoringIntervalAndTimeoutValue &FT_FaultMonitoringIntervalAndTimeoutValue_out;



class TAO_Export FT_InterfaceNotFound : public CORBA::UserException
{
public:
  FT_InterfaceNotFound (void); // default ctor
  FT_InterfaceNotFound (const FT_InterfaceNotFound &); // copy ctor
  ~FT_InterfaceNotFound (void);
  static void _tao_any_destructor (void*);
  FT_InterfaceNotFound &operator= (const FT_InterfaceNotFound &);


  virtual void _raise (void);

  virtual void _tao_encode (
      TAO_OutputCDR &,
      CORBA::Environment &) const;
  virtual void _tao_decode (
      TAO_InputCDR &,
      CORBA::Environment &);

  static FT_InterfaceNotFound *_downcast (CORBA::Exception *);


  // = TAO extension
  static CORBA::Exception *_alloc (void);

}; // exception FT::InterfaceNotFound

class TAO_Export FT_ObjectGroupNotFound : public CORBA::UserException
{
public:
FT_ObjectGroupNotFound (void); // default ctor
FT_ObjectGroupNotFound (const FT_ObjectGroupNotFound &); // copy ctor
~FT_ObjectGroupNotFound (void);
static void _tao_any_destructor (void*);
FT_ObjectGroupNotFound &operator= (const FT_ObjectGroupNotFound &);


virtual void _raise (void);

virtual void _tao_encode (
    TAO_OutputCDR &,
    CORBA::Environment &) const;
virtual void _tao_decode (
    TAO_InputCDR &,
    CORBA::Environment &);

static FT_ObjectGroupNotFound *_downcast (CORBA::Exception *);


// = TAO extension
static CORBA::Exception *_alloc (void);

}; // exception FT::ObjectGroupNotFound

class TAO_Export FT_MemberNotFound : public CORBA::UserException
{
public:


FT_MemberNotFound (void); // default ctor
FT_MemberNotFound (const FT_MemberNotFound &); // copy ctor
~FT_MemberNotFound (void);
static void _tao_any_destructor (void*);
FT_MemberNotFound &operator= (const FT_MemberNotFound &);


virtual void _raise (void);

virtual void _tao_encode (
    TAO_OutputCDR &,
    CORBA::Environment &) const;
virtual void _tao_decode (
    TAO_InputCDR &,
    CORBA::Environment &);

static FT_MemberNotFound *_downcast (CORBA::Exception *);


// = TAO extension
static CORBA::Exception *_alloc (void);

}; // exception FT::MemberNotFound



class TAO_Export FT_ObjectNotFound : public CORBA::UserException
{
public:


FT_ObjectNotFound (void); // default ctor
FT_ObjectNotFound (const FT_ObjectNotFound &); // copy ctor
~FT_ObjectNotFound (void);
static void _tao_any_destructor (void*);
FT_ObjectNotFound &operator= (const FT_ObjectNotFound &);


virtual void _raise (void);

virtual void _tao_encode (
    TAO_OutputCDR &,
    CORBA::Environment &) const;
virtual void _tao_decode (
    TAO_InputCDR &,
    CORBA::Environment &);

static FT_ObjectNotFound *_downcast (CORBA::Exception *);


// = TAO extension
static CORBA::Exception *_alloc (void);

}; // exception FT::ObjectNotFound


class TAO_Export FT_MemberAlreadyPresent : public CORBA::UserException
{
public:


FT_MemberAlreadyPresent (void); // default ctor
FT_MemberAlreadyPresent (const FT_MemberAlreadyPresent &); // copy ctor
~FT_MemberAlreadyPresent (void);
static void _tao_any_destructor (void*);
FT_MemberAlreadyPresent &operator= (const FT_MemberAlreadyPresent &);


virtual void _raise (void);

virtual void _tao_encode (
    TAO_OutputCDR &,
    CORBA::Environment &) const;
virtual void _tao_decode (
    TAO_InputCDR &,
    CORBA::Environment &);

static FT_MemberAlreadyPresent *_downcast (CORBA::Exception *);


// = TAO extension
static CORBA::Exception *_alloc (void);

}; // exception FT::MemberAlreadyPresent


class TAO_Export FT_BadReplicationStyle : public CORBA::UserException
{
public:


FT_BadReplicationStyle (void); // default ctor
FT_BadReplicationStyle (const FT_BadReplicationStyle &); // copy ctor
~FT_BadReplicationStyle (void);
static void _tao_any_destructor (void*);
FT_BadReplicationStyle &operator= (const FT_BadReplicationStyle &);


virtual void _raise (void);

virtual void _tao_encode (
    TAO_OutputCDR &,
    CORBA::Environment &) const;
virtual void _tao_decode (
    TAO_InputCDR &,
    CORBA::Environment &);

static FT_BadReplicationStyle *_downcast (CORBA::Exception *);


// = TAO extension
static CORBA::Exception *_alloc (void);

}; // exception FT::BadReplicationStyle



class TAO_Export FT_ObjectNotCreated : public CORBA::UserException
{
public:


FT_ObjectNotCreated (void); // default ctor
FT_ObjectNotCreated (const FT_ObjectNotCreated &); // copy ctor
~FT_ObjectNotCreated (void);
static void _tao_any_destructor (void*);
FT_ObjectNotCreated &operator= (const FT_ObjectNotCreated &);


virtual void _raise (void);

virtual void _tao_encode (
    TAO_OutputCDR &,
    CORBA::Environment &) const;
virtual void _tao_decode (
    TAO_InputCDR &,
    CORBA::Environment &);

static FT_ObjectNotCreated *_downcast (CORBA::Exception *);


// = TAO extension
static CORBA::Exception *_alloc (void);

}; // exception FT::ObjectNotCreated



class TAO_Export FT_ObjectNotAdded : public CORBA::UserException
{
public:
FT_ObjectNotAdded (void); // default ctor
FT_ObjectNotAdded (const FT_ObjectNotAdded &); // copy ctor
~FT_ObjectNotAdded (void);
static void _tao_any_destructor (void*);
FT_ObjectNotAdded &operator= (const FT_ObjectNotAdded &);


virtual void _raise (void);

virtual void _tao_encode (
    TAO_OutputCDR &,
    CORBA::Environment &) const;
virtual void _tao_decode (
    TAO_InputCDR &,
    CORBA::Environment &);

static FT_ObjectNotAdded *_downcast (CORBA::Exception *);


// = TAO extension
static CORBA::Exception *_alloc (void);

}; // exception FT::ObjectNotAdded



class TAO_Export FT_PrimaryNotSet : public CORBA::UserException
{
public:


FT_PrimaryNotSet (void); // default ctor
FT_PrimaryNotSet (const FT_PrimaryNotSet &); // copy ctor
~FT_PrimaryNotSet (void);
static void _tao_any_destructor (void*);
FT_PrimaryNotSet &operator= (const FT_PrimaryNotSet &);


virtual void _raise (void);

virtual void _tao_encode (
    TAO_OutputCDR &,
    CORBA::Environment &) const;
virtual void _tao_decode (
    TAO_InputCDR &,
    CORBA::Environment &);

static FT_PrimaryNotSet *_downcast (CORBA::Exception *);


// = TAO extension
static CORBA::Exception *_alloc (void);

}; // exception FT::PrimaryNotSet


class TAO_Export FT_UnsupportedProperty : public CORBA::UserException
{
public:

FT_Name nam;
FT::Value val;

FT_UnsupportedProperty (void); // default ctor
FT_UnsupportedProperty (const FT_UnsupportedProperty &); // copy ctor
~FT_UnsupportedProperty (void);
static void _tao_any_destructor (void*);
FT_UnsupportedProperty &operator= (const FT_UnsupportedProperty &);


virtual void _raise (void);

virtual void _tao_encode (
    TAO_OutputCDR &,
    CORBA::Environment &) const;
virtual void _tao_decode (
    TAO_InputCDR &,
    CORBA::Environment &);

static FT_UnsupportedProperty *_downcast (CORBA::Exception *);

FT_UnsupportedProperty (
  const FT_Name & _tao_nam,
  const FT::Value & _tao_val
);

// = TAO extension
static CORBA::Exception *_alloc (void);

}; // exception FT::UnsupportedProperty


class TAO_Export FT_InvalidProperty : public CORBA::UserException
{
public:

FT_Name nam;
FT::Value val;

FT_InvalidProperty (void); // default ctor
FT_InvalidProperty (const FT_InvalidProperty &); // copy ctor
~FT_InvalidProperty (void);
static void _tao_any_destructor (void*);
FT_InvalidProperty &operator= (const FT_InvalidProperty &);


virtual void _raise (void);

virtual void _tao_encode (
    TAO_OutputCDR &,
    CORBA::Environment &) const;
virtual void _tao_decode (
    TAO_InputCDR &,
    CORBA::Environment &);

static FT_InvalidProperty *_downcast (CORBA::Exception *);

FT_InvalidProperty (
  const FT_Name & _tao_nam,
  const FT::Value & _tao_val
);

// = TAO extension
static CORBA::Exception *_alloc (void);

}; // exception FT::InvalidProperty


class TAO_Export FT_NoFactory : public CORBA::UserException
{
public:

FT::Location the_location;
TAO_String_Manager type_id;

FT_NoFactory (void); // default ctor
FT_NoFactory (const FT_NoFactory &); // copy ctor
~FT_NoFactory (void);
static void _tao_any_destructor (void*);
FT_NoFactory &operator= (const FT_NoFactory &);


virtual void _raise (void);

virtual void _tao_encode (
    TAO_OutputCDR &,
    CORBA::Environment &) const;
virtual void _tao_decode (
    TAO_InputCDR &,
    CORBA::Environment &);

static FT_NoFactory *_downcast (CORBA::Exception *);

FT_NoFactory (
  const FT::Location & _tao_the_location,
  const char * _tao_type_id
);

// = TAO extension
static CORBA::Exception *_alloc (void);

}; // exception FT::NoFactory


class TAO_Export FT_InvalidCriteria : public CORBA::UserException
{
public:

FT_Criteria invalid_criteria;

FT_InvalidCriteria (void); // default ctor
FT_InvalidCriteria (const FT_InvalidCriteria &); // copy ctor
~FT_InvalidCriteria (void);
static void _tao_any_destructor (void*);
FT_InvalidCriteria &operator= (const FT_InvalidCriteria &);


virtual void _raise (void);

virtual void _tao_encode (
    TAO_OutputCDR &,
    CORBA::Environment &) const;
virtual void _tao_decode (
    TAO_InputCDR &,
    CORBA::Environment &);

static FT_InvalidCriteria *_downcast (CORBA::Exception *);

FT_InvalidCriteria (
  const FT_Criteria & _tao_invalid_criteria
);

// = TAO extension
static CORBA::Exception *_alloc (void);

}; // exception FT::InvalidCriteria


class TAO_Export FT_CannotMeetCriteria : public CORBA::UserException
{
public:

FT_Criteria unmet_criteria;

FT_CannotMeetCriteria (void); // default ctor
FT_CannotMeetCriteria (const FT_CannotMeetCriteria &); // copy ctor
~FT_CannotMeetCriteria (void);
static void _tao_any_destructor (void*);
FT_CannotMeetCriteria &operator= (const FT_CannotMeetCriteria &);


virtual void _raise (void);

virtual void _tao_encode (
    TAO_OutputCDR &,
    CORBA::Environment &) const;
virtual void _tao_decode (
    TAO_InputCDR &,
    CORBA::Environment &);

static FT_CannotMeetCriteria *_downcast (CORBA::Exception *);

FT_CannotMeetCriteria (
  const  FT_Criteria & _tao_unmet_criteria
);

// = TAO extension
static CORBA::Exception *_alloc (void);

}; // exception FT::CannotMeetCriteria



class TAO_Export FT_PropertyManager_var : public TAO_Base_var
{
public:
  FT_PropertyManager_var (void); // default constructor
  FT_PropertyManager_var (FT_PropertyManager_ptr p) : ptr_ (p) {}
  FT_PropertyManager_var (const FT_PropertyManager_var &); // copy constructor
  ~FT_PropertyManager_var (void); // destructor

  FT_PropertyManager_var &operator= (FT_PropertyManager_ptr);
  FT_PropertyManager_var &operator= (const FT_PropertyManager_var &);
  FT_PropertyManager_ptr operator-> (void) const;

  operator const FT_PropertyManager_ptr &() const;
  operator FT_PropertyManager_ptr &();
  // in, inout, out, _retn
  FT_PropertyManager_ptr in (void) const;
  FT_PropertyManager_ptr &inout (void);
  FT_PropertyManager_ptr &out (void);
  FT_PropertyManager_ptr _retn (void);
  FT_PropertyManager_ptr ptr (void) const;

private:
  FT_PropertyManager_ptr ptr_;
  // Unimplemented - prevents widening assignment.
  FT_PropertyManager_var (const TAO_Base_var &rhs);
  FT_PropertyManager_var &operator= (const TAO_Base_var &rhs);
};



class TAO_Export FT_PropertyManager_out
{
public:
  FT_PropertyManager_out (FT_PropertyManager_ptr &);
  FT_PropertyManager_out (FT_PropertyManager_var &);
  FT_PropertyManager_out (const FT_PropertyManager_out &);
  FT_PropertyManager_out &operator= (const FT_PropertyManager_out &);
  FT_PropertyManager_out &operator= (const FT_PropertyManager_var &);
  FT_PropertyManager_out &operator= (FT_PropertyManager_ptr);
  operator FT_PropertyManager_ptr &();
  FT_PropertyManager_ptr &ptr (void);
  FT_PropertyManager_ptr operator-> (void);

private:
  FT_PropertyManager_ptr &ptr_;
};


class TAO_Export FT_PropertyManager : public virtual CORBA_Object
{
public:
#if !defined(__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)
  typedef FT_PropertyManager_ptr _ptr_type;
  typedef FT_PropertyManager_var _var_type;
#endif /* ! __GNUC__ || g++ >= 2.8 */

  // the static operations
  static FT_PropertyManager_ptr _duplicate (FT_PropertyManager_ptr obj);
  static FT_PropertyManager_ptr _narrow (
      CORBA::Object_ptr obj,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  static FT_PropertyManager_ptr _unchecked_narrow (
      CORBA::Object_ptr obj,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  static FT_PropertyManager_ptr _nil (void)
    {
      return (FT_PropertyManager_ptr)0;
    }

  static void _tao_any_destructor (void*);

  virtual void set_default_properties (
      const FT::Properties & props,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::InvalidProperty,
      FT::UnsupportedProperty
    ));

  virtual FT::Properties * get_default_properties (
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual void remove_default_properties (
      const FT::Properties & props,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::InvalidProperty,
      FT::UnsupportedProperty
    ));

  virtual void set_type_properties (
      const char * type_id,
      const FT::Properties & overrides,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::InvalidProperty,
      FT::UnsupportedProperty
    ));

  virtual FT::Properties * get_type_properties (
      const char * type_id,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual void remove_type_properties (
      const char * type_id,
      const FT::Properties & props,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::InvalidProperty,
      FT::UnsupportedProperty
    ));

  virtual void set_properties_dynamically (
      FT::ObjectGroup_ptr object_group,
      const FT::Properties & overrides,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::ObjectGroupNotFound,
      FT::InvalidProperty,
      FT::UnsupportedProperty
    ));

  virtual FT::Properties * get_properties (
      FT::ObjectGroup_ptr object_group,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::ObjectGroupNotFound
    ));

  virtual CORBA::Boolean _is_a (
      const CORBA::Char *type_id,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  virtual void *_tao_QueryInterface (ptr_arith_t type);

  virtual const char* _interface_repository_id (void) const;

protected:
  FT_PropertyManager (void);
  FT_PropertyManager (TAO_Stub *objref,
      TAO_ServantBase *_tao_servant = 0,
      CORBA::Boolean _tao_collocated = 0
    );
  virtual ~FT_PropertyManager (void);
private:
  FT_PropertyManager (const FT_PropertyManager &);
  void operator= (const FT_PropertyManager &);
};


class TAO_Export FT_ObjectGroupManager_var : public TAO_Base_var
{
public:
  FT_ObjectGroupManager_var (void); // default constructor
  FT_ObjectGroupManager_var (FT_ObjectGroupManager_ptr p) : ptr_ (p) {}
  FT_ObjectGroupManager_var (const FT_ObjectGroupManager_var &); // copy constructor
  ~FT_ObjectGroupManager_var (void); // destructor

  FT_ObjectGroupManager_var &operator= (FT_ObjectGroupManager_ptr);
  FT_ObjectGroupManager_var &operator= (const FT_ObjectGroupManager_var &);
  FT_ObjectGroupManager_ptr operator-> (void) const;

  operator const FT_ObjectGroupManager_ptr &() const;
  operator FT_ObjectGroupManager_ptr &();
  // in, inout, out, _retn
  FT_ObjectGroupManager_ptr in (void) const;
  FT_ObjectGroupManager_ptr &inout (void);
  FT_ObjectGroupManager_ptr &out (void);
  FT_ObjectGroupManager_ptr _retn (void);
  FT_ObjectGroupManager_ptr ptr (void) const;

private:
  FT_ObjectGroupManager_ptr ptr_;
  // Unimplemented - prevents widening assignment.
  FT_ObjectGroupManager_var (const TAO_Base_var &rhs);
  FT_ObjectGroupManager_var &operator= (const TAO_Base_var &rhs);
};


class TAO_Export FT_ObjectGroupManager_out
{
public:
  FT_ObjectGroupManager_out (FT_ObjectGroupManager_ptr &);
  FT_ObjectGroupManager_out (FT_ObjectGroupManager_var &);
  FT_ObjectGroupManager_out (const FT_ObjectGroupManager_out &);
  FT_ObjectGroupManager_out &operator= (const FT_ObjectGroupManager_out &);
  FT_ObjectGroupManager_out &operator= (const FT_ObjectGroupManager_var &);
  FT_ObjectGroupManager_out &operator= (FT_ObjectGroupManager_ptr);
  operator FT_ObjectGroupManager_ptr &();
  FT_ObjectGroupManager_ptr &ptr (void);
  FT_ObjectGroupManager_ptr operator-> (void);

private:
  FT_ObjectGroupManager_ptr &ptr_;
};

class TAO_Export FT_ObjectGroupManager : public virtual CORBA_Object
{
public:
#if !defined(__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)
  typedef FT_ObjectGroupManager_ptr _ptr_type;
  typedef FT_ObjectGroupManager_var _var_type;
#endif /* ! __GNUC__ || g++ >= 2.8 */

  // the static operations
  static FT_ObjectGroupManager_ptr _duplicate (FT_ObjectGroupManager_ptr obj);
  static FT_ObjectGroupManager_ptr _narrow (
      CORBA::Object_ptr obj,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  static FT_ObjectGroupManager_ptr _unchecked_narrow (
      CORBA::Object_ptr obj,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  static FT_ObjectGroupManager_ptr _nil (void)
    {
      return (FT_ObjectGroupManager_ptr)0;
    }

  static void _tao_any_destructor (void*);

  virtual FT::ObjectGroup_ptr create_member (
      FT::ObjectGroup_ptr object_group,
      const FT::Location & the_location,
      const char * type_id,
      const FT::Criteria & the_criteria,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::ObjectGroupNotFound,
      FT::MemberAlreadyPresent,
      FT::NoFactory,
      FT::ObjectNotCreated,
      FT::InvalidCriteria,
      FT::CannotMeetCriteria
    ));

  virtual FT::ObjectGroup_ptr add_member (
      FT::ObjectGroup_ptr object_group,
      const FT::Location & the_location,
      CORBA::Object_ptr member,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::ObjectGroupNotFound,
      FT::MemberAlreadyPresent,
      FT::ObjectNotAdded
    ));

  virtual FT::ObjectGroup_ptr remove_member (
      FT::ObjectGroup_ptr object_group,
      const FT::Location & the_location,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::ObjectGroupNotFound,
      FT::MemberNotFound
    ));

  virtual FT::ObjectGroup_ptr set_primary_member (
      FT::ObjectGroup_ptr object_group,
      const FT::Location & the_location,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::ObjectGroupNotFound,
      FT::MemberNotFound,
      FT::PrimaryNotSet,
      FT::BadReplicationStyle
    ));

  virtual FT::Locations * locations_of_members (
      FT::ObjectGroup_ptr object_group,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::ObjectGroupNotFound
    ));

  virtual FT::ObjectGroupId get_object_group_id (
      FT::ObjectGroup_ptr object_group,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::ObjectGroupNotFound
    ));

  virtual FT::ObjectGroup_ptr get_object_group_ref (
      FT::ObjectGroup_ptr object_group,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::ObjectGroupNotFound
    ));

  virtual CORBA::Object_ptr get_member_ref (
      FT::ObjectGroup_ptr object_group,
      const FT::Location & loc,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::ObjectGroupNotFound,
      FT::MemberNotFound
    ));

  virtual CORBA::Boolean _is_a (
      const CORBA::Char *type_id,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  virtual void *_tao_QueryInterface (ptr_arith_t type);

  virtual const char* _interface_repository_id (void) const;

protected:
  FT_ObjectGroupManager (void);
  FT_ObjectGroupManager (TAO_Stub *objref,
      TAO_ServantBase *_tao_servant = 0,
      CORBA::Boolean _tao_collocated = 0
    );
  virtual ~FT_ObjectGroupManager (void);
private:
  FT_ObjectGroupManager (const FT_ObjectGroupManager &);
  void operator= (const FT_ObjectGroupManager &);
};


/*class TAO_Export FT_GenericFactory_var : public TAO_Base_var
{
public:
  FT_GenericFactory_var (void); // default constructor
  FT_GenericFactory_var (FT_GenericFactory_ptr p) : ptr_ (p) {}
  FT_GenericFactory_var (const FT_GenericFactory_var &); // copy constructor
  ~FT_GenericFactory_var (void); // destructor

  FT_GenericFactory_var &operator= (FT_GenericFactory_ptr);
  FT_GenericFactory_var &operator= (const FT_GenericFactory_var &);
  FT_GenericFactory_ptr operator-> (void) const;

  operator const FT_GenericFactory_ptr &() const;
  operator FT_GenericFactory_ptr &();
  // in, inout, out, _retn
  FT_GenericFactory_ptr in (void) const;
  FT_GenericFactory_ptr &inout (void);
  FT_GenericFactory_ptr &out (void);
  FT_GenericFactory_ptr _retn (void);
  FT_GenericFactory_ptr ptr (void) const;

private:
  FT_GenericFactory_ptr ptr_;
  // Unimplemented - prevents widening assignment.
  FT_GenericFactory_var (const TAO_Base_var &rhs);
  FT_GenericFactory_var &operator= (const TAO_Base_var &rhs);
};


class TAO_Export FT_GenericFactory_out
{
public:
  FT_GenericFactory_out (FT_GenericFactory_ptr &);
  FT_GenericFactory_out (FT_GenericFactory_var &);
  FT_GenericFactory_out (const FT_GenericFactory_out &);
  FT_GenericFactory_out &operator= (const FT_GenericFactory_out &);
  FT_GenericFactory_out &operator= (const FT_GenericFactory_var &);
  FT_GenericFactory_out &operator= (FT_GenericFactory_ptr);
  operator FT_GenericFactory_ptr &();
  FT_GenericFactory_ptr &ptr (void);
  FT_GenericFactory_ptr operator-> (void);

private:
  FT_GenericFactory_ptr &ptr_;
};
*/

class TAO_Export FT_GenericFactory : public virtual CORBA_Object
{
public:
#if !defined(__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)
  typedef FT_GenericFactory_ptr _ptr_type;
  typedef FT_GenericFactory_var _var_type;
#endif /* ! __GNUC__ || g++ >= 2.8 */

  // the static operations
  static FT_GenericFactory_ptr _duplicate (FT_GenericFactory_ptr obj);
  static FT_GenericFactory_ptr _narrow (
      CORBA::Object_ptr obj,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  static FT_GenericFactory_ptr _unchecked_narrow (
      CORBA::Object_ptr obj,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  static FT_GenericFactory_ptr _nil (void)
    {
      return (FT_GenericFactory_ptr)0;
    }

  static void _tao_any_destructor (void*);

  typedef CORBA::Any FactoryCreationId;
  typedef CORBA::Any_ptr FactoryCreationId_ptr;
  typedef CORBA::Any_var FactoryCreationId_var;
  typedef CORBA::Any_out FactoryCreationId_out;
    static CORBA::TypeCode_ptr _tc_FactoryCreationId;

  virtual CORBA::Object_ptr create_object (
      const char * type_id,
      const FT::Criteria & the_criteria,
      FT::GenericFactory::FactoryCreationId_out factory_creation_id,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::NoFactory,
      FT::ObjectNotCreated,
      FT::InvalidCriteria,
      FT::InvalidProperty,
      FT::CannotMeetCriteria
    ));

  virtual void delete_object (
      const FT::GenericFactory::FactoryCreationId & factory_creation_id,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::ObjectNotFound
    ));

  virtual CORBA::Boolean _is_a (
      const CORBA::Char *type_id,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  virtual void *_tao_QueryInterface (ptr_arith_t type);

  virtual const char* _interface_repository_id (void) const;

protected:
  FT_GenericFactory (void);
  FT_GenericFactory (TAO_Stub *objref,
      TAO_ServantBase *_tao_servant = 0,
      CORBA::Boolean _tao_collocated = 0
    );
  virtual ~FT_GenericFactory (void);
private:
  FT_GenericFactory (const FT_GenericFactory &);
  void operator= (const FT_GenericFactory &);
};


class TAO_Export FT_ReplicationManager_var : public TAO_Base_var
{
public:
  FT_ReplicationManager_var (void); // default constructor
  FT_ReplicationManager_var (FT_ReplicationManager_ptr p) : ptr_ (p) {}
  FT_ReplicationManager_var (const FT_ReplicationManager_var &); // copy constructor
  ~FT_ReplicationManager_var (void); // destructor

  FT_ReplicationManager_var &operator= (FT_ReplicationManager_ptr);
  FT_ReplicationManager_var &operator= (const FT_ReplicationManager_var &);
  FT_ReplicationManager_ptr operator-> (void) const;

  operator const FT_ReplicationManager_ptr &() const;
  operator FT_ReplicationManager_ptr &();
  // in, inout, out, _retn
  FT_ReplicationManager_ptr in (void) const;
  FT_ReplicationManager_ptr &inout (void);
  FT_ReplicationManager_ptr &out (void);
  FT_ReplicationManager_ptr _retn (void);
  FT_ReplicationManager_ptr ptr (void) const;

private:
  FT_ReplicationManager_ptr ptr_;
  // Unimplemented - prevents widening assignment.
  FT_ReplicationManager_var (const TAO_Base_var &rhs);
  FT_ReplicationManager_var &operator= (const TAO_Base_var &rhs);
};


class TAO_Export FT_ReplicationManager_out
{
public:
  FT_ReplicationManager_out (FT_ReplicationManager_ptr &);
  FT_ReplicationManager_out (FT_ReplicationManager_var &);
  FT_ReplicationManager_out (const FT_ReplicationManager_out &);
  FT_ReplicationManager_out &operator= (const FT_ReplicationManager_out &);
  FT_ReplicationManager_out &operator= (const FT_ReplicationManager_var &);
  FT_ReplicationManager_out &operator= (FT_ReplicationManager_ptr);
  operator FT_ReplicationManager_ptr &();
  FT_ReplicationManager_ptr &ptr (void);
  FT_ReplicationManager_ptr operator-> (void);

private:
  FT_ReplicationManager_ptr &ptr_;
};


class TAO_Export FT_ReplicationManager: public virtual FT_PropertyManager, public virtual FT_ObjectGroupManager, public virtual FT_GenericFactory
{
public:
#if !defined(__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)
  typedef FT_ReplicationManager_ptr _ptr_type;
  typedef FT_ReplicationManager_var _var_type;
#endif /* ! __GNUC__ || g++ >= 2.8 */

  // the static operations
  static FT_ReplicationManager_ptr _duplicate (FT_ReplicationManager_ptr obj);
  static FT_ReplicationManager_ptr _narrow (
      CORBA::Object_ptr obj,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  static FT_ReplicationManager_ptr _unchecked_narrow (
      CORBA::Object_ptr obj,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  static FT_ReplicationManager_ptr _nil (void)
    {
      return (FT_ReplicationManager_ptr)0;
    }

  static void _tao_any_destructor (void*);

  virtual void register_fault_notifier (
      FT_FaultNotifier_ptr fault_notifier,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual FT::FaultNotifier_ptr get_fault_notifier (
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::InterfaceNotFound
    ));

  virtual CORBA::Boolean _is_a (
      const CORBA::Char *type_id,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  virtual void *_tao_QueryInterface (ptr_arith_t type);

  virtual const char* _interface_repository_id (void) const;

protected:
  FT_ReplicationManager (void);
  FT_ReplicationManager (TAO_Stub *objref,
      TAO_ServantBase *_tao_servant = 0,
      CORBA::Boolean _tao_collocated = 0
    );
  virtual ~FT_ReplicationManager (void);
private:
  FT_ReplicationManager (const FT_ReplicationManager &);
  void operator= (const FT_ReplicationManager &);
};



class TAO_Export FT_PullMonitorable_var : public TAO_Base_var
{
public:
  FT_PullMonitorable_var (void); // default constructor
  FT_PullMonitorable_var (FT_PullMonitorable_ptr p) : ptr_ (p) {}
  FT_PullMonitorable_var (const FT_PullMonitorable_var &); // copy constructor
  ~FT_PullMonitorable_var (void); // destructor

  FT_PullMonitorable_var &operator= (FT_PullMonitorable_ptr);
  FT_PullMonitorable_var &operator= (const FT_PullMonitorable_var &);
  FT_PullMonitorable_ptr operator-> (void) const;

  operator const FT_PullMonitorable_ptr &() const;
  operator FT_PullMonitorable_ptr &();
  // in, inout, out, _retn
  FT_PullMonitorable_ptr in (void) const;
  FT_PullMonitorable_ptr &inout (void);
  FT_PullMonitorable_ptr &out (void);
  FT_PullMonitorable_ptr _retn (void);
  FT_PullMonitorable_ptr ptr (void) const;

private:
  FT_PullMonitorable_ptr ptr_;
  // Unimplemented - prevents widening assignment.
  FT_PullMonitorable_var (const TAO_Base_var &rhs);
  FT_PullMonitorable_var &operator= (const TAO_Base_var &rhs);
};

class TAO_Export FT_PullMonitorable_out
{
public:
  FT_PullMonitorable_out (FT_PullMonitorable_ptr &);
  FT_PullMonitorable_out (FT_PullMonitorable_var &);
  FT_PullMonitorable_out (const FT_PullMonitorable_out &);
  FT_PullMonitorable_out &operator= (const FT_PullMonitorable_out &);
  FT_PullMonitorable_out &operator= (const FT_PullMonitorable_var &);
  FT_PullMonitorable_out &operator= (FT_PullMonitorable_ptr);
  operator FT_PullMonitorable_ptr &();
  FT_PullMonitorable_ptr &ptr (void);
  FT_PullMonitorable_ptr operator-> (void);

private:
  FT_PullMonitorable_ptr &ptr_;
};


class TAO_Export FT_PullMonitorable : public virtual CORBA_Object
{
public:
#if !defined(__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)
  typedef FT_PullMonitorable_ptr _ptr_type;
  typedef FT_PullMonitorable_var _var_type;
#endif /* ! __GNUC__ || g++ >= 2.8 */

  // the static operations
  static FT_PullMonitorable_ptr _duplicate (FT_PullMonitorable_ptr obj);
  static FT_PullMonitorable_ptr _narrow (
      CORBA::Object_ptr obj,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  static FT_PullMonitorable_ptr _unchecked_narrow (
      CORBA::Object_ptr obj,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  static FT_PullMonitorable_ptr _nil (void)
    {
      return (FT_PullMonitorable_ptr)0;
    }

  static void _tao_any_destructor (void*);

  virtual CORBA::Boolean is_alive (
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual CORBA::Boolean _is_a (
      const CORBA::Char *type_id,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  virtual void *_tao_QueryInterface (ptr_arith_t type);

  virtual const char* _interface_repository_id (void) const;

protected:
  FT_PullMonitorable (void);
  FT_PullMonitorable (TAO_Stub *objref,
      TAO_ServantBase *_tao_servant = 0,
      CORBA::Boolean _tao_collocated = 0
    );
  virtual ~FT_PullMonitorable (void);
private:
  FT_PullMonitorable (const FT_PullMonitorable &);
  void operator= (const FT_PullMonitorable &);
};


/*class TAO_Export FT_FaultNotifier_var : public TAO_Base_var
{
public:
  FT_FaultNotifier_var (void); // default constructor
  FT_FaultNotifier_var (FT_FaultNotifier_ptr p) : ptr_ (p) {}
  FT_FaultNotifier_var (const FT_FaultNotifier_var &); // copy constructor
  ~FT_FaultNotifier_var (void); // destructor

  FT_FaultNotifier_var &operator= (FT_FaultNotifier_ptr);
  FT_FaultNotifier_var &operator= (const FT_FaultNotifier_var &);
  FT_FaultNotifier_ptr operator-> (void) const;

  operator const FT_FaultNotifier_ptr &() const;
  operator FT_FaultNotifier_ptr &();
  // in, inout, out, _retn
  FT_FaultNotifier_ptr in (void) const;
  FT_FaultNotifier_ptr &inout (void);
  FT_FaultNotifier_ptr &out (void);
  FT_FaultNotifier_ptr _retn (void);
  FT_FaultNotifier_ptr ptr (void) const;

private:
  FT_FaultNotifier_ptr ptr_;
  // Unimplemented - prevents widening assignment.
  FT_FaultNotifier_var (const TAO_Base_var &rhs);
  FT_FaultNotifier_var &operator= (const TAO_Base_var &rhs);
};


class TAO_Export FT_FaultNotifier_out
{
public:
  FT_FaultNotifier_out (FT_FaultNotifier_ptr &);
  FT_FaultNotifier_out (FT_FaultNotifier_var &);
  FT_FaultNotifier_out (const FT_FaultNotifier_out &);
  FT_FaultNotifier_out &operator= (const FT_FaultNotifier_out &);
  FT_FaultNotifier_out &operator= (const FT_FaultNotifier_var &);
  FT_FaultNotifier_out &operator= (FT_FaultNotifier_ptr);
  operator FT_FaultNotifier_ptr &();
  FT_FaultNotifier_ptr &ptr (void);
  FT_FaultNotifier_ptr operator-> (void);

private:
  FT_FaultNotifier_ptr &ptr_;
};
*/

class TAO_Export FT_FaultNotifier : public virtual CORBA_Object
{
public:
#if !defined(__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)
  typedef FT_FaultNotifier_ptr _ptr_type;
  typedef FT_FaultNotifier_var _var_type;
#endif /* ! __GNUC__ || g++ >= 2.8 */

  // the static operations
  static FT_FaultNotifier_ptr _duplicate (FT_FaultNotifier_ptr obj);
  static FT_FaultNotifier_ptr _narrow (
      CORBA::Object_ptr obj,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  static FT_FaultNotifier_ptr _unchecked_narrow (
      CORBA::Object_ptr obj,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  static FT_FaultNotifier_ptr _nil (void)
    {
      return (FT_FaultNotifier_ptr)0;
    }

  static void _tao_any_destructor (void*);

  typedef CORBA::ULongLong ConsumerId;
  typedef CORBA::ULongLong_out ConsumerId_out;
    static CORBA::TypeCode_ptr _tc_ConsumerId;

  virtual void push_structured_fault (
      const CosNotification::StructuredEvent & event,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual void push_sequence_fault (
      const CosNotification::EventBatch & events,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual CosNotifyFilter::Filter_ptr create_subscription_filter (
      const char * constraint_grammar,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      CosNotifyFilter::InvalidGrammar
    ));

  virtual FT::FaultNotifier::ConsumerId connect_structured_fault_consumer (
      CosNotifyComm::StructuredPushConsumer_ptr push_consumer,
      CosNotifyFilter::Filter_ptr filter,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual FT::FaultNotifier::ConsumerId connect_sequence_fault_consumer (
      CosNotifyComm::SequencePushConsumer_ptr push_consumer,
      CosNotifyFilter::Filter_ptr filter,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual void disconnect_consumer (
      FT::FaultNotifier::ConsumerId connection,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      CosEventComm::Disconnected
    ));

  virtual CORBA::Boolean _is_a (
      const CORBA::Char *type_id,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  virtual void *_tao_QueryInterface (ptr_arith_t type);

  virtual const char* _interface_repository_id (void) const;

protected:
  FT_FaultNotifier (void);
  FT_FaultNotifier (TAO_Stub *objref,
      TAO_ServantBase *_tao_servant = 0,
      CORBA::Boolean _tao_collocated = 0
    );
  virtual ~FT_FaultNotifier (void);
private:
  FT_FaultNotifier (const FT_FaultNotifier &);
  void operator= (const FT_FaultNotifier &);
};


// *************************************************************
// State
// *************************************************************

class TAO_Export FT_State : public
#if !defined (TAO_USE_SEQUENCE_TEMPLATES)
  TAO_Unbounded_Sequence<CORBA::Octet>
#else /* TAO_USE_SEQUENCE_TEMPLATES */
  TAO_Unbounded_Sequence<CORBA::Octet>
#endif /* !TAO_USE_SEQUENCE_TEMPLATES */
{
public:
  FT_State (void); // default ctor
  FT_State (CORBA::ULong max); // uses max size
  FT_State (
    CORBA::ULong max,
    CORBA::ULong length,
    CORBA::Octet *buffer,
    CORBA::Boolean release=0
  );
  FT_State (const FT_State &); // copy ctor
  ~FT_State (void);
  static void _tao_any_destructor (void*);

#if !defined(__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)
  typedef FT_State_var _var_type;
#endif /* ! __GNUC__ || g++ >= 2.8 */


#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
  FT_State (
      CORBA::ULong length,
      const ACE_Message_Block* mb
    )
    : TAO_Unbounded_Sequence<CORBA::Octet> (length, mb) {}
#endif /* TAO_NO_COPY_OCTET_SEQUENCE == 1 */

};


// *************************************************************
// class FT::State_var
// *************************************************************

class TAO_Export FT_State_var
{
public:
  FT_State_var (void); // default constructor
  FT_State_var (FT_State *);
  FT_State_var (const FT_State_var &); // copy constructor
  FT_State_var (const FT_State &); // fixed-size base types only
  ~FT_State_var (void); // destructor

  FT_State_var &operator= (FT_State *);
  FT_State_var &operator= (const FT_State_var &);
  FT_State_var &operator= (const FT_State &); // fixed-size base types only
  FT_State *operator-> (void);
  const FT_State *operator-> (void) const;

  operator const FT_State &() const;
  operator FT_State &();
  operator FT_State &() const;

  CORBA::Octet &operator[] (CORBA::ULong index);
  // in, inout, out, _retn
  const FT_State &in (void) const;
  FT_State &inout (void);
  FT_State *&out (void);
  FT_State *_retn (void);
  FT_State *ptr (void) const;

private:
  FT_State *ptr_;
};


class TAO_Export FT_State_out
{
public:
  FT_State_out (FT_State *&);
  FT_State_out (FT_State_var &);
  FT_State_out (const FT_State_out &);
  FT_State_out &operator= (const FT_State_out &);
  FT_State_out &operator= (FT_State *);
  operator FT_State *&();
  FT_State *&ptr (void);
  FT_State *operator-> (void);
  CORBA::Octet &operator[] (CORBA::ULong index);

private:
  FT_State *&ptr_;
  // assignment from T_var not allowed
  void operator= (const FT_State_var &);
};



class TAO_Export FT_NoStateAvailable : public CORBA::UserException
{
public:


FT_NoStateAvailable (void); // default ctor
FT_NoStateAvailable (const FT_NoStateAvailable &); // copy ctor
~FT_NoStateAvailable (void);
static void _tao_any_destructor (void*);
FT_NoStateAvailable &operator= (const FT_NoStateAvailable &);


virtual void _raise (void);

virtual void _tao_encode (
    TAO_OutputCDR &,
    CORBA::Environment &) const;
virtual void _tao_decode (
    TAO_InputCDR &,
    CORBA::Environment &);

static FT_NoStateAvailable *_downcast (CORBA::Exception *);


// = TAO extension
static CORBA::Exception *_alloc (void);

}; // exception FT::NoStateAvailable


class TAO_Export FT_InvalidState : public CORBA::UserException
{
public:


FT_InvalidState (void); // default ctor
FT_InvalidState (const FT_InvalidState &); // copy ctor
~FT_InvalidState (void);
static void _tao_any_destructor (void*);
FT_InvalidState &operator= (const FT_InvalidState &);


virtual void _raise (void);

virtual void _tao_encode (
    TAO_OutputCDR &,
    CORBA::Environment &) const;
virtual void _tao_decode (
    TAO_InputCDR &,
    CORBA::Environment &);

static FT_InvalidState *_downcast (CORBA::Exception *);


// = TAO extension
static CORBA::Exception *_alloc (void);

}; // exception FT::InvalidState



class TAO_Export FT_NoUpdateAvailable : public CORBA::UserException
{
public:


FT_NoUpdateAvailable (void); // default ctor
FT_NoUpdateAvailable (const FT_NoUpdateAvailable &); // copy ctor
~FT_NoUpdateAvailable (void);
static void _tao_any_destructor (void*);
FT_NoUpdateAvailable &operator= (const FT_NoUpdateAvailable &);


virtual void _raise (void);

virtual void _tao_encode (
    TAO_OutputCDR &,
    CORBA::Environment &) const;
virtual void _tao_decode (
    TAO_InputCDR &,
    CORBA::Environment &);

static FT_NoUpdateAvailable *_downcast (CORBA::Exception *);


// = TAO extension
static CORBA::Exception *_alloc (void);

}; // exception FT::NoUpdateAvailable




class TAO_Export FT_InvalidUpdate : public CORBA::UserException
{
public:


FT_InvalidUpdate (void); // default ctor
FT_InvalidUpdate (const FT_InvalidUpdate &); // copy ctor
~FT_InvalidUpdate (void);
static void _tao_any_destructor (void*);
FT_InvalidUpdate &operator= (const FT_InvalidUpdate &);


virtual void _raise (void);

virtual void _tao_encode (
    TAO_OutputCDR &,
    CORBA::Environment &) const;
virtual void _tao_decode (
    TAO_InputCDR &,
    CORBA::Environment &);

static FT_InvalidUpdate *_downcast (CORBA::Exception *);


// = TAO extension
static CORBA::Exception *_alloc (void);

}; // exception FT::InvalidUpdate


class TAO_Export FT_Checkpointable_var : public TAO_Base_var
{
public:
  FT_Checkpointable_var (void); // default constructor
  FT_Checkpointable_var (FT_Checkpointable_ptr p) : ptr_ (p) {}
  FT_Checkpointable_var (const FT_Checkpointable_var &); // copy constructor
  ~FT_Checkpointable_var (void); // destructor

  FT_Checkpointable_var &operator= (FT_Checkpointable_ptr);
  FT_Checkpointable_var &operator= (const FT_Checkpointable_var &);
  FT_Checkpointable_ptr operator-> (void) const;

  operator const FT_Checkpointable_ptr &() const;
  operator FT_Checkpointable_ptr &();
  // in, inout, out, _retn
  FT_Checkpointable_ptr in (void) const;
  FT_Checkpointable_ptr &inout (void);
  FT_Checkpointable_ptr &out (void);
  FT_Checkpointable_ptr _retn (void);
  FT_Checkpointable_ptr ptr (void) const;

private:
  FT_Checkpointable_ptr ptr_;
  // Unimplemented - prevents widening assignment.
  FT_Checkpointable_var (const TAO_Base_var &rhs);
  FT_Checkpointable_var &operator= (const TAO_Base_var &rhs);
};



class TAO_Export FT_Checkpointable_out
{
public:
  FT_Checkpointable_out (FT_Checkpointable_ptr &);
  FT_Checkpointable_out (FT_Checkpointable_var &);
  FT_Checkpointable_out (const FT_Checkpointable_out &);
  FT_Checkpointable_out &operator= (const FT_Checkpointable_out &);
  FT_Checkpointable_out &operator= (const FT_Checkpointable_var &);
  FT_Checkpointable_out &operator= (FT_Checkpointable_ptr);
  operator FT_Checkpointable_ptr &();
  FT_Checkpointable_ptr &ptr (void);
  FT_Checkpointable_ptr operator-> (void);

private:
  FT_Checkpointable_ptr &ptr_;
};


class TAO_Export FT_Checkpointable : public virtual CORBA_Object
{
public:
#if !defined(__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)
  typedef FT_Checkpointable_ptr _ptr_type;
  typedef FT_Checkpointable_var _var_type;
#endif /* ! __GNUC__ || g++ >= 2.8 */

  // the static operations
  static FT_Checkpointable_ptr _duplicate (FT_Checkpointable_ptr obj);
  static FT_Checkpointable_ptr _narrow (
      CORBA::Object_ptr obj,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  static FT_Checkpointable_ptr _unchecked_narrow (
      CORBA::Object_ptr obj,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  static FT_Checkpointable_ptr _nil (void)
    {
      return (FT_Checkpointable_ptr)0;
    }

  static void _tao_any_destructor (void*);

  virtual FT::State * get_state (
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::NoStateAvailable
    ));

  virtual void set_state (
      const FT::State & s,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::InvalidState
    ));

  virtual CORBA::Boolean _is_a (
      const CORBA::Char *type_id,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  virtual void *_tao_QueryInterface (ptr_arith_t type);

  virtual const char* _interface_repository_id (void) const;

protected:
  FT_Checkpointable (void);
  FT_Checkpointable (TAO_Stub *objref,
      TAO_ServantBase *_tao_servant = 0,
      CORBA::Boolean _tao_collocated = 0
    );
  virtual ~FT_Checkpointable (void);
private:
  FT_Checkpointable (const FT_Checkpointable &);
  void operator= (const FT_Checkpointable &);
};



class TAO_Export FT_Updateable_var : public TAO_Base_var
{
public:
  FT_Updateable_var (void); // default constructor
  FT_Updateable_var (FT_Updateable_ptr p) : ptr_ (p) {}
  FT_Updateable_var (const FT_Updateable_var &); // copy constructor
  ~FT_Updateable_var (void); // destructor

  FT_Updateable_var &operator= (FT_Updateable_ptr);
  FT_Updateable_var &operator= (const FT_Updateable_var &);
  FT_Updateable_ptr operator-> (void) const;

  operator const FT_Updateable_ptr &() const;
  operator FT_Updateable_ptr &();
  // in, inout, out, _retn
  FT_Updateable_ptr in (void) const;
  FT_Updateable_ptr &inout (void);
  FT_Updateable_ptr &out (void);
  FT_Updateable_ptr _retn (void);
  FT_Updateable_ptr ptr (void) const;

private:
  FT_Updateable_ptr ptr_;
  // Unimplemented - prevents widening assignment.
  FT_Updateable_var (const TAO_Base_var &rhs);
  FT_Updateable_var &operator= (const TAO_Base_var &rhs);
};



class TAO_Export FT_Updateable_out
{
public:
  FT_Updateable_out (FT_Updateable_ptr &);
  FT_Updateable_out (FT_Updateable_var &);
  FT_Updateable_out (const FT_Updateable_out &);
  FT_Updateable_out &operator= (const FT_Updateable_out &);
  FT_Updateable_out &operator= (const FT_Updateable_var &);
  FT_Updateable_out &operator= (FT_Updateable_ptr);
  operator FT_Updateable_ptr &();
  FT_Updateable_ptr &ptr (void);
  FT_Updateable_ptr operator-> (void);

private:
  FT_Updateable_ptr &ptr_;
};



class TAO_Export FT_Updateable: public virtual FT_Checkpointable
{
public:
#if !defined(__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)
  typedef FT_Updateable_ptr _ptr_type;
  typedef FT_Updateable_var _var_type;
#endif /* ! __GNUC__ || g++ >= 2.8 */

  // the static operations
  static FT_Updateable_ptr _duplicate (FT_Updateable_ptr obj);
  static FT_Updateable_ptr _narrow (
      CORBA::Object_ptr obj,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  static FT_Updateable_ptr _unchecked_narrow (
      CORBA::Object_ptr obj,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  static FT_Updateable_ptr _nil (void)
    {
      return (FT_Updateable_ptr)0;
    }

  static void _tao_any_destructor (void*);

  virtual FT::State * get_update (
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::NoUpdateAvailable
    ));

  virtual void set_update (
      const FT::State & s,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::InvalidUpdate
    ));

  virtual CORBA::Boolean _is_a (
      const CORBA::Char *type_id,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  virtual void *_tao_QueryInterface (ptr_arith_t type);

  virtual const char* _interface_repository_id (void) const;

protected:
  FT_Updateable (void);
  FT_Updateable (TAO_Stub *objref,
      TAO_ServantBase *_tao_servant = 0,
      CORBA::Boolean _tao_collocated = 0
    );
  virtual ~FT_Updateable (void);
private:
  FT_Updateable (const FT_Updateable &);
  void operator= (const FT_Updateable &);
};



TAO_Export void operator<<= (CORBA::Any &, const FT::Property &); // copying version
TAO_Export void operator<<= (CORBA::Any &, FT::Property*); // noncopying version
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::Property *&); // deprecated
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, const FT::Property *&);
TAO_Export void operator<<= (CORBA::Any &, const FT::Properties &); // copying version
TAO_Export void operator<<= (CORBA::Any &, FT::Properties*); // noncopying version
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::Properties *&); // deprecated
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, const FT::Properties *&);

// Goofiness for FT_Name. This is just a hack and not a good one at
// that.
TAO_Export void operator<<= (CORBA::Any &, const FT::Name &); // copying version
TAO_Export void operator<<= (CORBA::Any &, FT::Name*); // noncopying version
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::Name *&); // deprecated
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, const FT::Name *&);
// End Goofiness

TAO_Export void operator<<= (CORBA::Any &, const FT::Locations &); // copying version
TAO_Export void operator<<= (CORBA::Any &, FT::Locations*); // noncopying version
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::Locations *&); // deprecated
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, const FT::Locations *&);
TAO_Export void operator<<= (CORBA::Any &, const FT::FactoryInfo &); // copying version
TAO_Export void operator<<= (CORBA::Any &, FT::FactoryInfo*); // noncopying version
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::FactoryInfo *&); // deprecated
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, const FT::FactoryInfo *&);
TAO_Export void operator<<= (CORBA::Any &, const FT::FactoryInfos &); // copying version
TAO_Export void operator<<= (CORBA::Any &, FT::FactoryInfos*); // noncopying version
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::FactoryInfos *&); // deprecated
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, const FT::FactoryInfos *&);
TAO_Export void operator<<= (CORBA::Any &, const FT::FaultMonitoringIntervalAndTimeoutValue &); // copying version
TAO_Export void operator<<= (CORBA::Any &, FT::FaultMonitoringIntervalAndTimeoutValue*); // noncopying version
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::FaultMonitoringIntervalAndTimeoutValue *&); // deprecated
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, const FT::FaultMonitoringIntervalAndTimeoutValue *&);
TAO_Export void operator<<= (CORBA::Any &, const FT::InterfaceNotFound &); // copying version
TAO_Export void operator<<= (CORBA::Any &, FT::InterfaceNotFound*); // noncopying version
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::InterfaceNotFound *&); // deprecated
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, const FT::InterfaceNotFound *&);
TAO_Export void operator<<= (CORBA::Any &, const FT::ObjectGroupNotFound &); // copying version
TAO_Export void operator<<= (CORBA::Any &, FT::ObjectGroupNotFound*); // noncopying version
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::ObjectGroupNotFound *&); // deprecated
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, const FT::ObjectGroupNotFound *&);
TAO_Export void operator<<= (CORBA::Any &, const FT::MemberNotFound &); // copying version
TAO_Export void operator<<= (CORBA::Any &, FT::MemberNotFound*); // noncopying version
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::MemberNotFound *&); // deprecated
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, const FT::MemberNotFound *&);
TAO_Export void operator<<= (CORBA::Any &, const FT::ObjectNotFound &); // copying version
TAO_Export void operator<<= (CORBA::Any &, FT::ObjectNotFound*); // noncopying version
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::ObjectNotFound *&); // deprecated
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, const FT::ObjectNotFound *&);
TAO_Export void operator<<= (CORBA::Any &, const FT::MemberAlreadyPresent &); // copying version
TAO_Export void operator<<= (CORBA::Any &, FT::MemberAlreadyPresent*); // noncopying version
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::MemberAlreadyPresent *&); // deprecated
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, const FT::MemberAlreadyPresent *&);
TAO_Export void operator<<= (CORBA::Any &, const FT::BadReplicationStyle &); // copying version
TAO_Export void operator<<= (CORBA::Any &, FT::BadReplicationStyle*); // noncopying version
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::BadReplicationStyle *&); // deprecated
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, const FT::BadReplicationStyle *&);
TAO_Export void operator<<= (CORBA::Any &, const FT::ObjectNotCreated &); // copying version
TAO_Export void operator<<= (CORBA::Any &, FT::ObjectNotCreated*); // noncopying version
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::ObjectNotCreated *&); // deprecated
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, const FT::ObjectNotCreated *&);
TAO_Export void operator<<= (CORBA::Any &, const FT::ObjectNotAdded &); // copying version
TAO_Export void operator<<= (CORBA::Any &, FT::ObjectNotAdded*); // noncopying version
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::ObjectNotAdded *&); // deprecated
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, const FT::ObjectNotAdded *&);
TAO_Export void operator<<= (CORBA::Any &, const FT::PrimaryNotSet &); // copying version
TAO_Export void operator<<= (CORBA::Any &, FT::PrimaryNotSet*); // noncopying version
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::PrimaryNotSet *&); // deprecated
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, const FT::PrimaryNotSet *&);
TAO_Export void operator<<= (CORBA::Any &, const FT::UnsupportedProperty &); // copying version
TAO_Export void operator<<= (CORBA::Any &, FT::UnsupportedProperty*); // noncopying version
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::UnsupportedProperty *&); // deprecated
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, const FT::UnsupportedProperty *&);
TAO_Export void operator<<= (CORBA::Any &, const FT::InvalidProperty &); // copying version
TAO_Export void operator<<= (CORBA::Any &, FT::InvalidProperty*); // noncopying version
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::InvalidProperty *&); // deprecated
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, const FT::InvalidProperty *&);
TAO_Export void operator<<= (CORBA::Any &, const FT::NoFactory &); // copying version
TAO_Export void operator<<= (CORBA::Any &, FT::NoFactory*); // noncopying version
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::NoFactory *&); // deprecated
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, const FT::NoFactory *&);
TAO_Export void operator<<= (CORBA::Any &, const FT::InvalidCriteria &); // copying version
TAO_Export void operator<<= (CORBA::Any &, FT::InvalidCriteria*); // noncopying version
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::InvalidCriteria *&); // deprecated
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, const FT::InvalidCriteria *&);
TAO_Export void operator<<= (CORBA::Any &, const FT::CannotMeetCriteria &); // copying version
TAO_Export void operator<<= (CORBA::Any &, FT::CannotMeetCriteria*); // noncopying version
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::CannotMeetCriteria *&); // deprecated
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, const FT::CannotMeetCriteria *&);
extern TAO_Export FT::PropertyManager_ptr (*_TAO_collocation_FT_PropertyManager_Stub_Factory_function_pointer) (
    CORBA::Object_ptr obj
  );
// Any operators for interface FT::PropertyManager
TAO_Export void operator<<= (CORBA::Any &, FT::PropertyManager_ptr);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::PropertyManager *&);
extern TAO_Export FT::ObjectGroupManager_ptr (*_TAO_collocation_FT_ObjectGroupManager_Stub_Factory_function_pointer) (
    CORBA::Object_ptr obj
  );
// Any operators for interface FT::ObjectGroupManager
TAO_Export void operator<<= (CORBA::Any &, FT::ObjectGroupManager_ptr);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::ObjectGroupManager *&);
extern TAO_Export FT::GenericFactory_ptr (*_TAO_collocation_FT_GenericFactory_Stub_Factory_function_pointer) (
    CORBA::Object_ptr obj
  );
// Any operators for interface FT::GenericFactory
TAO_Export void operator<<= (CORBA::Any &, FT::GenericFactory_ptr);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::GenericFactory *&);
extern TAO_Export FT::ReplicationManager_ptr (*_TAO_collocation_FT_ReplicationManager_Stub_Factory_function_pointer) (
    CORBA::Object_ptr obj
  );
// Any operators for interface FT::ReplicationManager
TAO_Export void operator<<= (CORBA::Any &, FT::ReplicationManager_ptr);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::ReplicationManager *&);
extern TAO_Export FT::PullMonitorable_ptr (*_TAO_collocation_FT_PullMonitorable_Stub_Factory_function_pointer) (
    CORBA::Object_ptr obj
  );
// Any operators for interface FT::PullMonitorable
TAO_Export void operator<<= (CORBA::Any &, FT::PullMonitorable_ptr);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::PullMonitorable *&);
extern TAO_Export FT::FaultNotifier_ptr (*_TAO_collocation_FT_FaultNotifier_Stub_Factory_function_pointer) (
    CORBA::Object_ptr obj
  );
// Any operators for interface FT::FaultNotifier
TAO_Export void operator<<= (CORBA::Any &, FT::FaultNotifier_ptr);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::FaultNotifier *&);
TAO_Export void operator<<= (CORBA::Any &, const FT::State &); // copying version
TAO_Export void operator<<= (CORBA::Any &, FT::State*); // noncopying version
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::State *&); // deprecated
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, const FT::State *&);
TAO_Export void operator<<= (CORBA::Any &, const FT::NoStateAvailable &); // copying version
TAO_Export void operator<<= (CORBA::Any &, FT::NoStateAvailable*); // noncopying version
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::NoStateAvailable *&); // deprecated
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, const FT::NoStateAvailable *&);
TAO_Export void operator<<= (CORBA::Any &, const FT::InvalidState &); // copying version
TAO_Export void operator<<= (CORBA::Any &, FT::InvalidState*); // noncopying version
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::InvalidState *&); // deprecated
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, const FT::InvalidState *&);
TAO_Export void operator<<= (CORBA::Any &, const FT::NoUpdateAvailable &); // copying version
TAO_Export void operator<<= (CORBA::Any &, FT::NoUpdateAvailable*); // noncopying version
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::NoUpdateAvailable *&); // deprecated
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, const FT::NoUpdateAvailable *&);
TAO_Export void operator<<= (CORBA::Any &, const FT::InvalidUpdate &); // copying version
TAO_Export void operator<<= (CORBA::Any &, FT::InvalidUpdate*); // noncopying version
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::InvalidUpdate *&); // deprecated
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, const FT::InvalidUpdate *&);
extern TAO_Export FT::Checkpointable_ptr (*_TAO_collocation_FT_Checkpointable_Stub_Factory_function_pointer) (
    CORBA::Object_ptr obj
  );
// Any operators for interface FT::Checkpointable
TAO_Export void operator<<= (CORBA::Any &, FT::Checkpointable_ptr);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::Checkpointable *&);
extern TAO_Export FT::Updateable_ptr (*_TAO_collocation_FT_Updateable_Stub_Factory_function_pointer) (
    CORBA::Object_ptr obj
  );
// Any operators for interface FT::Updateable
TAO_Export void operator<<= (CORBA::Any &, FT::Updateable_ptr);
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, FT::Updateable *&);


TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const FT_Name &);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      FT_Name &);

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const FT::Properties &);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      FT::Properties &);

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const FT::Locations & );
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      FT::Locations & );

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const FT::FactoryInfos &);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      FT::FactoryInfos &);

TAO_Export CORBA::Boolean  operator<< (TAO_OutputCDR &,
                                       const FT_State & );
TAO_Export CORBA::Boolean  operator>> (TAO_InputCDR &,
                                       FT_State &);

#ifndef __ACE_INLINE__
TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const FT::Property &);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &, FT::Property &);

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const FT::FactoryInfo &);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &, FT::FactoryInfo &);

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const FT::FaultMonitoringIntervalAndTimeoutValue &);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      FT::FaultMonitoringIntervalAndTimeoutValue &);
TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const FT::InterfaceNotFound &);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      FT::InterfaceNotFound &);

# if !defined (ACE_LACKS_IOSTREAM_TOTALLY)

TAO_Export ostream& operator<< (ostream &, const FT::InterfaceNotFound &);
TAO_Export ostream& operator<< (ostream &, const FT::ObjectGroupNotFound &);
TAO_Export ostream& operator<< (ostream &, const FT::MemberNotFound &);
TAO_Export ostream& operator<< (ostream &, const FT::ObjectNotFound &);
TAO_Export ostream& operator<< (ostream &, const FT::MemberAlreadyPresent &);
TAO_Export ostream& operator<< (ostream &, const FT::BadReplicationStyle &);
TAO_Export ostream& operator<< (ostream &, const FT::ObjectNotCreated &);
TAO_Export ostream& operator<< (ostream &, const FT::ObjectNotAdded &);
TAO_Export ostream& operator<< (ostream &, const FT::PrimaryNotSet &);
TAO_Export ostream& operator<< (ostream &, const FT::UnsupportedProperty &);
TAO_Export ostream& operator<< (ostream &, const FT::NoStateAvailable &);
TAO_Export ostream& operator<< (ostream &, const FT::InvalidState &);
TAO_Export ostream& operator<< (ostream &, const FT::NoUpdateAvailable &);
TAO_Export ostream& operator<< (ostream &, const FT::InvalidUpdate &);

# endif /* ACE_LACKS_IOSTREAM_TOTALLY */

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const FT::ObjectGroupNotFound &);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      FT::ObjectGroupNotFound &);
TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const FT::MemberNotFound &);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      FT::MemberNotFound &);


TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const FT::Name &);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      FT::Name &);

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const FT::ObjectNotFound &);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &, FT::ObjectNotFound &);

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const FT::MemberAlreadyPresent &);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &, FT::MemberAlreadyPresent &);

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const FT::BadReplicationStyle &);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &, FT::BadReplicationStyle &);

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const FT::ObjectNotCreated &);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &, FT::ObjectNotCreated &);

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const FT::ObjectNotAdded &);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &, FT::ObjectNotAdded &);

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const FT::PrimaryNotSet &);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &, FT::PrimaryNotSet &);

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const FT::UnsupportedProperty &);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &, FT::UnsupportedProperty &);

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const FT::InvalidProperty &);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &, FT::InvalidProperty &);

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const FT::NoFactory &);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &, FT::NoFactory &);

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const FT::InvalidCriteria &);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &, FT::InvalidCriteria &);

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const FT::CannotMeetCriteria &);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &, FT::CannotMeetCriteria &);

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const FT::PropertyManager_ptr );
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &, FT::PropertyManager_ptr &);
TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const FT::ObjectGroupManager_ptr );
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &, FT::ObjectGroupManager_ptr &);
TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const FT::GenericFactory_ptr );
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &, FT::GenericFactory_ptr &);
TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const FT::ReplicationManager_ptr );
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &, FT::ReplicationManager_ptr &);
TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const FT::PullMonitorable_ptr );
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &, FT::PullMonitorable_ptr &);
TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const FT::FaultNotifier_ptr );
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &, FT::FaultNotifier_ptr &);

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const FT::State & );
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      FT::State &);

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const FT::NoStateAvailable &);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      FT::NoStateAvailable &);

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const FT::InvalidState &);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      FT::InvalidState &);

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const FT::NoUpdateAvailable &);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      FT::NoUpdateAvailable &);

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const FT::InvalidUpdate &);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      FT::InvalidUpdate &);

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const FT::Checkpointable_ptr );
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &, FT::Checkpointable_ptr &);
TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const FT::Updateable_ptr );
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &, FT::Updateable_ptr &);


TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const FT_Updateable_ptr);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      FT_Updateable_ptr &);


TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const FT_Checkpointable_ptr);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      FT_Checkpointable_ptr &);

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const FT_FaultNotifier_ptr);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      FT_FaultNotifier_ptr &);


TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const FT_PullMonitorable_ptr );
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      FT_PullMonitorable_ptr &);

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const FT_ReplicationManager_ptr);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      FT_ReplicationManager_ptr &);


TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const FT_GenericFactory_ptr );
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      FT_GenericFactory_ptr &);


TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const FT_ObjectGroupManager_ptr);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      FT_ObjectGroupManager_ptr &);


TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const FT_PropertyManager_ptr);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      FT_PropertyManager_ptr &);

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const FT_GenericFactory_ptr);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      FT_GenericFactory_ptr &);

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const FT_FaultNotifier_ptr);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      FT_FaultNotifier_ptr &);


TAO_Export CORBA::Boolean  operator<< (TAO_OutputCDR &,
                                       const FT_Properties &);
TAO_Export CORBA::Boolean  operator>> (TAO_InputCDR &,
                                       FT_Properties &);


TAO_Export CORBA::Boolean  operator<< (TAO_OutputCDR &,
                                       const FT_Locations &);
TAO_Export CORBA::Boolean  operator>> (TAO_InputCDR &,
                                       FT_Locations &);

TAO_Export CORBA::Boolean  operator<< (TAO_OutputCDR &,
                                       const FT_FactoryInfos &);
TAO_Export CORBA::Boolean  operator>> (TAO_InputCDR &,
                                       FT_FactoryInfos &);


TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const FT_PropertyManager_ptr);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      FT_PropertyManager_ptr &);


TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const FT_ObjectGroupManager_ptr);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      FT_ObjectGroupManager_ptr &);


TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const FT_GenericFactory_ptr);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      FT_GenericFactory_ptr & );


TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const FT_ReplicationManager_ptr);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      FT_ReplicationManager_ptr &);


TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const FT_PullMonitorable_ptr);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      FT_PullMonitorable_ptr &);


TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const FT_HeartbeatEnabledPolicy_ptr);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      FT_HeartbeatEnabledPolicy_ptr &);

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const FT_GenericFactory_ptr);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      FT_GenericFactory_ptr &);
TAO_Export CORBA::Boolean  operator>> (TAO_InputCDR &,
                                       FT_Properties &);


TAO_Export CORBA::Boolean  operator<< (TAO_OutputCDR &,
                                       const FT_Locations & );
CORBA::Boolean TAO_Export operator>> (TAO_InputCDR &,
                                      FT_Locations &);


TAO_Export CORBA::Boolean  operator<< (TAO_OutputCDR &,
                                       const FT_FactoryInfos &);
TAO_Export CORBA::Boolean  operator>> (TAO_InputCDR &,
                                       FT_FactoryInfos &);


TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &,
                                      const FT_RequestDurationPolicy_ptr);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &,
                                      FT_RequestDurationPolicy_ptr &);


#endif /* __ACE_INLINE__ */

#if defined (__ACE_INLINE__)
#include "FT_CORBA_ServiceC.i"
#endif /* defined INLINE */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /*TAO_HAS_FT_CORBA == 1 */

#endif /*TAO_FT_CORBA_SERVICEC_H */
