/**************************************************************************
This file has been hand-crafted to work with the ORB API of TAO
***************************************************************************/

#ifndef MARSHAL_C_H
#define MARSHAL_C_H

#include <stdlib.h>
#include <string.h>

#include "tao/orb.h"
#include "tao/orbobj.h"
#include "tao/object.h"
#include "tao/stub.h"
#include "tao/typecode.h"
#include "tao/cdr.h"

class Marshal;
typedef Marshal *Marshal_ptr;
typedef Marshal_ptr MarshalRef;

class Marshal: public virtual CORBA_Object
{
public:
  static Marshal_ptr _duplicate (Marshal_ptr obj);
  static Marshal_ptr _narrow (CORBA_Object_ptr obj);
  static Marshal_ptr _nil ();

  struct Marshal_Struct {
    CORBA_Short	s;
    CORBA_Long	l;
    CORBA_Char	c;
    CORBA_Octet	o;
    CORBA_Double	d;
  };
  static CORBA_TypeCode_ptr _tc_Marshal_Struct;

  enum discrim {
    e_0th,
    e_1st,
    e_2nd,
    e_3rd,
    e_4th,
    e_5th,
    e_6th
  };

  static CORBA_TypeCode_ptr _tc_discrim;

  class Marshal_Union
  {
  private:
    discrim	_disc;
    CORBA_Short	__s;
    CORBA_Long	__l;
    CORBA_Char	__c;
    CORBA_Octet	__o;
    CORBA_Double	__d;
    Marshal::Marshal_Struct	__ms;
  public:
    Marshal_Union() : _disc(discrim(0)) {}
    ~Marshal_Union() {}
    Marshal_Union(const Marshal_Union& obj);
    Marshal_Union& operator=(const Marshal_Union& obj);
	
    void _d(discrim val) { _disc = val; }
    discrim _d() const { return _disc; }
	
    void s(CORBA_Short val) {
      __s = val;
      _disc = Marshal::e_0th;
    }
    CORBA_Short s() const { return __s; }
	
    void l(CORBA_Long val) {
      __l = val;
      _disc = Marshal::e_1st;
    }
    CORBA_Long l() const { return __l; }
	
    void c(CORBA_Char val) {
      __c = val;
      _disc = Marshal::e_2nd;
    }
    CORBA_Char c() const { return __c; }
	
    void o(CORBA_Octet val) {
      __o = val;
      _disc = Marshal::e_3rd;
    }
    CORBA_Octet o() const { return __o; }
	
    void d(CORBA_Double val) {
      __d = val;
      _disc = Marshal::e_4th;
    }
    CORBA_Double d() const { return __d; }
	
    void ms(const Marshal::Marshal_Struct& val) {
      __ms = val;
      _disc = Marshal::e_5th;
    }
    const Marshal::Marshal_Struct& ms() const { return __ms; }
    Marshal::Marshal_Struct& ms() { return __ms; }
	
  };
	
  static CORBA_TypeCode_ptr _tc_Marshal_Union;

  class AnySeq
  {
  public:
    static CORBA_Any *allocbuf (CORBA_ULong nelems);
    static void freebuf (CORBA_Any *data);
    AnySeq (CORBA_ULong max=0);
    AnySeq (CORBA_ULong max,
	    CORBA_ULong length,
	    CORBA_Any *value,
	    CORBA_Boolean release = CORBA_B_FALSE
	    );
    //    AnySeq (const AnySeq&);
    ~AnySeq ();
    CORBA_ULong maximum () const;
    void length (CORBA_ULong);
    CORBA_ULong length () const;
    CORBA_Any &operator[] (CORBA_ULong index);
    const CORBA_Any &operator[] (CORBA_ULong index) const;
  private:
    CORBA_ULong _maximum;
    CORBA_ULong _length;
    CORBA_Any *_buffer;
    CORBA_Boolean _release;
  };
  static CORBA_TypeCode_ptr _tc_AnySeq;


  // now the methods
  virtual void test_short(CORBA_Short s1,
			  CORBA_Short& s2,
			  CORBA_Environment &env);
	
  virtual void test_long(CORBA_Long l1,
			 CORBA_Long& l2,
			 CORBA_Environment &env);
	
  virtual void test_octet(CORBA_Octet o1,
			  CORBA_Octet& o2,
			  CORBA_Environment &env);
	
  virtual void test_char(CORBA_Char c1,
			 CORBA_Char& c2,
			 CORBA_Environment &env);
	
  virtual void test_double(CORBA_Double d1,
			   CORBA_Double& d2,
			   CORBA_Environment &env);
	
  virtual void test_struct(const Marshal::Marshal_Struct& ms1,
			   Marshal::Marshal_Struct& ms2,
			   CORBA_Environment &env);
	
  virtual void test_union(const Marshal::Marshal_Union& u1,
			  Marshal::Marshal_Union& u2,
			  CORBA_Environment &env);
	
  virtual void test_any(const CORBA_Any& a1,
			CORBA_Any *& a2,
			CORBA_Environment &env);
	
  virtual void test_sequence(const Marshal::AnySeq& as1,
			     Marshal::AnySeq *& as2,
			     CORBA_Environment &env);
	
#if 0
  virtual void test_recursive(const Marshal::Marshal_Recursive& mr1,
			      Marshal::Marshal_Recursive *& mr2,
			      CORBA_Environment &env);
#endif

protected:
  Marshal() {}
  Marshal(STUB_Object *refdata);
  virtual ~Marshal() {}

private:
  Marshal(const Marshal &) {}
  void operator=(const Marshal &) {}
};

#endif




