// $Id$

#ifndef ORBIX_MARSHAL_IMPL_H
#define ORBIX_MARSHAL_IMPL_H

// to include DSI related files

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "marshal.h"


class Marshal_SSI_Impl
  : public virtual MarshalBOAImpl
{
  // =TITLE
  //  Marshal_SSI_Impl
  // =DESCRIPTION
  //  Implementation of the interface using the static skeleton interface
public:
  Marshal_SSI_Impl (const char *object_name = (const char *)NULL);
  // constructor

  ~Marshal_SSI_Impl (void);
  // destructor

  virtual void test_short (CORBA::Short s1,
                           CORBA::Short& s2,
                           CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ());
  // test short

  virtual void test_long (CORBA::Long l1,
                          CORBA::Long& l2,
                          CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ());
  // test long

  virtual void test_octet (CORBA::Octet o1,
                           CORBA::Octet& o2,
                           CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ());
  // test octet

  virtual void test_char (CORBA::Char c1,
                          CORBA::Char& c2,
                          CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ());
  // test char

  virtual void test_double (CORBA::Double d1,
                            CORBA::Double& d2,
                            CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ());
  // test double

  virtual void test_struct (const Marshal::Marshal_Struct& ms1,
                            Marshal::Marshal_Struct& ms2,
                            CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ());
  // test struct

  virtual void test_union (const Marshal::Marshal_Union& u1,
                           Marshal::Marshal_Union& u2,
                           CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ());
  // test union

  virtual void test_any (const CORBA::Any& a1,
                         CORBA::Any *& a2,
                         CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ());
  // test any

  /*
  virtual void test_sequence (const Marshal::ShortSeq& s1,
                             const Marshal::LongSeq& l1,
                             const Marshal::OctetSeq& o1,
                             const Marshal::CharSeq& c1,
                             const Marshal::DoubleSeq& d1,
                             const Marshal::StructSeq& S1,
                             const Marshal::UnionSeq& U1,
                             const Marshal::AnySeq& a1,
                             Marshal::ShortSeq *& s2,
                             Marshal::LongSeq *& l2,
                             Marshal::OctetSeq *& o2,
                             Marshal::CharSeq *& c2,
                             Marshal::DoubleSeq *& d2,
                             Marshal::StructSeq *& S2,
                             Marshal::UnionSeq *& U2,
                             Marshal::AnySeq *& a2);
                             */
  virtual void test_sequence (const Marshal::AnySeq& a1,
                              Marshal::AnySeq *& a2,
                              CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ());
  // test sequence

  virtual void test_recursive (const Marshal::Marshal_Recursive& mr1,
                               Marshal::Marshal_Recursive *& mr2,
                               CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ());
  // test struct

};

class Marshal_DSI_Impl : public CORBA::DynamicImplementation
{
  // =TITLE
  //  Marshal_DSI_Impl
  // =DESCRIPTION
  //  Implementation of the interface using the dynamic skeleton interface
public:
  Marshal_DSI_Impl (CORBA::ORB_ptr orb, const char *object_name = (const char *)NULL);
  // constructor

  ~Marshal_DSI_Impl (void);
  // destructor

private:
  virtual void invoke (CORBA::ServerRequest& req, CORBA::Environment_ptr,
                       CORBA::Environment&IT_env =
                       CORBA::IT_chooseDefaultEnv());
  // This method is a must

  void test_short (CORBA::Short s1,
                   CORBA::Short& s2);
  // test short

  void test_long (CORBA::Long l1,
                  CORBA::Long& l2);
  // test long

  void test_octet (CORBA::Octet o1,
                   CORBA::Octet& o2);
  // test octet

  void test_char (CORBA::Char c1,
                  CORBA::Char& c2);
  // test char

  void test_double (CORBA::Double d1,
                    CORBA::Double& d2);
  // test double

  void test_struct (const Marshal::Marshal_Struct& ms1,
                    Marshal::Marshal_Struct& ms2);
  // test struct

  void test_union (const Marshal::Marshal_Union& u1,
                   Marshal::Marshal_Union& u2);
  // test union

  void test_any (const CORBA::Any& a1,
                 CORBA::Any *& a2);
  // test any

  void test_sequence (const Marshal::AnySeq& a1,
                      Marshal::AnySeq *& a2);
  // test sequence

  void test_recursive (const Marshal::Marshal_Recursive& mr1,
                       Marshal::Marshal_Recursive *& mr2);
  // test struct

private:
  //= helper functions - they behave as if they were skeletons
  void test_short_skel (CORBA::ServerRequest& req);
  void test_long_skel (CORBA::ServerRequest& req);
  void test_octet_skel (CORBA::ServerRequest& req);
  void test_char_skel (CORBA::ServerRequest& req);
  void test_double_skel (CORBA::ServerRequest& req);
  void test_struct_skel (CORBA::ServerRequest& req);
  void test_union_skel (CORBA::ServerRequest& req);
  void test_any_skel (CORBA::ServerRequest& req);
  void test_sequence_skel (CORBA::ServerRequest& req);
  void test_recursive_skel (CORBA::ServerRequest& req);

  // we need the orb to create argument lists (NVList) for us
  CORBA::ORB_ptr orb_; // not owned by us

#if 0
  CORBA::NVList_ptr short_nv_;
  CORBA::NVList_ptr long_nv_;
  CORBA::NVList_ptr octet_nv_;
  CORBA::NVList_ptr char_nv_;
  CORBA::NVList_ptr double_nv_;
  CORBA::NVList_ptr struct_nv_;
  CORBA::NVList_ptr union_nv_;
  CORBA::NVList_ptr any_nv_;
  CORBA::NVList_ptr seq_nv_;
  CORBA::NVList_ptr recursive_nv_;
  CORBA::Any_ptr result_; // void result
#endif
};

#if defined (__ACE_INLINE__)
#include "orbix_marshal_impl.i"
#endif /* __ACE_INLINE__ */

#endif // defined (ORBIX_MARSHAL_IMPL_H)
