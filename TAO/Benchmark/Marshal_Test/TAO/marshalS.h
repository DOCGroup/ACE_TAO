/**************************************************************************
This file has been hand-crafted to work with the ORB API of TAO
***************************************************************************/

#ifndef MARSHAL_S_H
#define MARSHAL_S_H

#include <stdlib.h>
#include <string.h>

#include "marshalC.h"
#include "tao/connect.h"
#include "tao/orb.h"
#include "tao/params.h"
#include "tao/iiopobj.h"
#include "tao/optable.h"  // for registering our methods
#include "tao/corbacom.h"
#include "tao/nvlist.h"
#include "tao/orbobj.h"
#include "tao/debug.h"


class _skel_Marshal;
typedef _skel_Marshal *_skel_Marshal_ptr;
typedef _skel_Marshal *_skel_Marshal_ref;

class _skel_Marshal: public Marshal
{
public:
  // pure virtual functions implemented by the implementation class
  virtual void test_short(CORBA_Short s1,
			  CORBA_Short& s2,
			  CORBA_Environment &env) = 0;
	
  virtual void test_long(CORBA_Long l1,
			 CORBA_Long& l2,
			 CORBA_Environment &env) = 0;
	
  virtual void test_octet(CORBA_Octet o1,
			  CORBA_Octet& o2,
			  CORBA_Environment &env) = 0;
	
  virtual void test_char(CORBA_Char c1,
			 CORBA_Char& c2,
			 CORBA_Environment &env) = 0;
	
  virtual void test_double(CORBA_Double d1,
			   CORBA_Double& d2,
			   CORBA_Environment &env) = 0;
	
  virtual void test_struct(const Marshal::Marshal_Struct& ms1,
			   Marshal::Marshal_Struct& ms2,
			   CORBA_Environment &env) = 0;
	
  virtual void test_union(const Marshal::Marshal_Union& u1,
			  Marshal::Marshal_Union& u2,
			  CORBA_Environment &env) = 0;
	
  virtual void test_any(const CORBA_Any& a1,
			CORBA_Any *& a2,
			CORBA_Environment &env) = 0;
	
  virtual void test_sequence(const Marshal::AnySeq& as1,
			     Marshal::AnySeq *& as2,
			     CORBA_Environment &env) = 0;
	

  // skeletons that do the upcall
  static void _Marshal_is_a_skel (CORBA_ServerRequest &req, 
				  CORBA_Object_ptr obj,
				  CORBA_Environment &env);
  static void _Marshal_test_short_skel (CORBA_ServerRequest &req,
					CORBA_Object_ptr obj,
					CORBA_Environment &env);
  static void _Marshal_test_long_skel  (CORBA_ServerRequest &req,
					CORBA_Object_ptr obj,
					CORBA_Environment &env);
  static void _Marshal_test_octet_skel (CORBA_ServerRequest &req,
					CORBA_Object_ptr obj,
					CORBA_Environment &env);
  static void _Marshal_test_char_skel (CORBA_ServerRequest &req,
				       CORBA_Object_ptr obj,
				       CORBA_Environment &env);
  static void _Marshal_test_double_skel (CORBA_ServerRequest &req,
					 CORBA_Object_ptr obj,
					 CORBA_Environment &env);
  static void _Marshal_test_struct_skel (CORBA_ServerRequest &req,
					 CORBA_Object_ptr obj,
					 CORBA_Environment &env);
  static void _Marshal_test_union_skel (CORBA_ServerRequest &req,
					CORBA_Object_ptr obj,
					CORBA_Environment &env);
  static void _Marshal_test_any_skel (CORBA_ServerRequest &req,
				      CORBA_Object_ptr obj,
				      CORBA_Environment &env);
  static void _Marshal_test_sequence_skel (CORBA_ServerRequest &req,
					   CORBA_Object_ptr obj,
					   CORBA_Environment &env);
protected:
  _skel_Marshal (const char *obj_name = 0);
  virtual ~_skel_Marshal () {}
};

#endif




