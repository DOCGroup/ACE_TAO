#if !defined (TAO_MARSHAL_IMPL_H)
#define TAO_MARSHAL_IMPL_H

// to include DSI related files

#include "ace/OS.h"

#include "marshalS.h"
//#include "tao/dynimpl.h"

class Marshal_SSI_Impl 
  : public virtual _skel_Marshal
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

  virtual void test_short (CORBA_Short s1,
			  CORBA_Short& s2,
			   CORBA_Environment &env);
  // test short
	
  virtual void test_long (CORBA_Long l1,
			 CORBA_Long& l2,
			   CORBA_Environment &env);
  // test long
	
  virtual void test_octet (CORBA_Octet o1,
			  CORBA_Octet& o2,
			   CORBA_Environment &env);
  // test octet
	
  virtual void test_char (CORBA_Char c1,
			 CORBA_Char& c2,
			   CORBA_Environment &env);
  // test char
	
  virtual void test_double (CORBA_Double d1,
			   CORBA_Double& d2,
			   CORBA_Environment &env);
  // test double
	
  virtual void test_struct (const Marshal::Marshal_Struct& ms1,
			   Marshal::Marshal_Struct& ms2,
			   CORBA_Environment &env);
  // test struct
	
  virtual void test_union (const Marshal::Marshal_Union& u1,
			  Marshal::Marshal_Union& u2,
			   CORBA_Environment &env);
  // test union
	
  virtual void test_any (const CORBA_Any& a1,
			CORBA_Any *& a2,
			   CORBA_Environment &env);
  // test any
	
  virtual void test_sequence (const Marshal::AnySeq& a1,
			     Marshal::AnySeq *& a2,
			   CORBA_Environment &env);
  // test sequence

#if 0
  virtual void test_recursive (const Marshal::Marshal_Recursive& mr1,
			       Marshal::Marshal_Recursive *& mr2,
			   CORBA_Environment &env);
  // test struct
#endif
	
};

class Marshal_DSI_Impl : public CORBA_Object //: public CORBA_DynamicImplementation
{
  // =TITLE
  //  Marshal_DSI_Impl
  // =DESCRIPTION
  //  Implementation of the interface using the dynamic skeleton interface
  // =NOTES
  // We do not have a DynamicImplementation as yet. So we do a trick here. We
  // simulate the skeletons here and these call the private methods defined below:
public:
  Marshal_DSI_Impl (CORBA_ORB_ptr orb, const char *object_name = (const char *)NULL);
  // constructor

  ~Marshal_DSI_Impl (void);
  // destructor

  virtual void invoke (CORBA_ServerRequest & req, CORBA_Environment &env);
  // This method is a must, but of no use

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
private:
  void test_short (CORBA_Short s1,
		   CORBA_Short& s2,
		   CORBA_Environment &env);
  // test short
	
  void test_long (CORBA_Long l1,
		  CORBA_Long& l2,
		  CORBA_Environment &env);
  // test long
	
  void test_octet (CORBA_Octet o1,
		   CORBA_Octet& o2,
		   CORBA_Environment &env);
  // test octet
	
  void test_char (CORBA_Char c1,
		  CORBA_Char& c2,
		  CORBA_Environment &env);
  // test char
	
  void test_double (CORBA_Double d1,
		    CORBA_Double& d2,
		    CORBA_Environment &env);
  // test double
	
  void test_struct (const Marshal::Marshal_Struct& ms1,
		    Marshal::Marshal_Struct& ms2,
		    CORBA_Environment &env);
  // test struct
	
  void test_union (const Marshal::Marshal_Union& u1,
		   Marshal::Marshal_Union& u2,
		   CORBA_Environment &env);
  // test union
	
  void test_any (const CORBA_Any& a1,
		 CORBA_Any *& a2,
		 CORBA_Environment &env);
  // test any
	
  void test_sequence (const Marshal::AnySeq& a1,
		      Marshal::AnySeq *& a2,
		      CORBA_Environment &env);
  // test sequence

#if 0
  void test_recursive (const Marshal::Marshal_Recursive& mr1,
		       Marshal::Marshal_Recursive *& mr2,
		       CORBA_Environment &env);
  // test struct
#endif
	
  // we need the orb to create argument lists (NVList) for us
  CORBA_ORB_ptr orb_; // not owned by us
};

#if defined (__ACE_INLINE__)
#include "tao_marshal_impl.i"
#endif /* __ACE_INLINE__ */

#endif // defined (TAO_MARSHAL_IMPL_H)
