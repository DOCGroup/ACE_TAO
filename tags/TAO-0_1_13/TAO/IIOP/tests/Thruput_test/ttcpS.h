/**************************************************************************
This file has been hand-crafted to work with the ORB API of TAO
***************************************************************************/

#ifndef TTCP_SEQUENCE_S_H
#define TTCP_SEQUENCE_S_H

#include <stdlib.h>
#include <string.h>

#include "ttcpC.h"


class _skel_ttcp_sequence;
typedef _skel_ttcp_sequence *_skel_ttcp_sequence_ptr;
typedef _skel_ttcp_sequence *_skel_ttcp_sequence_ref;

class _skel_ttcp_sequence: public ttcp_sequence
{
public:
  // pure virtual functions implemented by the implementation class
  virtual void sendShortSeq(const ShortSeq& ttcp_seq, CORBA_Environment &) = 0;
  virtual void sendLongSeq(const LongSeq& ttcp_seq, CORBA_Environment &) = 0;
  virtual void sendDoubleSeq(const DoubleSeq& ttcp_seq, CORBA_Environment &) = 0;
  virtual void sendOctetSeq(const OctetSeq& ttcp_seq, CORBA_Environment &) = 0;
  virtual void sendCharSeq(const CharSeq& ttcp_seq, CORBA_Environment &) = 0;
  virtual void sendStructSeq(const StructSeq& ttcp_seq, CORBA_Environment &) = 0;
  virtual void start_timer(CORBA_Environment &) = 0;
  virtual void stop_timer(CORBA_Environment &) = 0;

  // skeletons that do the upcall
  static void _ttcp_sequence_is_a_skel (CORBA_ServerRequest &req, 
					CORBA_Object_ptr obj,
					CORBA_Environment &env);
  static void _ttcp_sequence_sendShortSeq_skel (CORBA_ServerRequest &req,
						CORBA_Object_ptr obj,
						CORBA_Environment &env);
  static void _ttcp_sequence_sendLongSeq_skel  (CORBA_ServerRequest &req,
						CORBA_Object_ptr obj,
						CORBA_Environment &env);
  static void _ttcp_sequence_sendDoubleSeq_skel (CORBA_ServerRequest &req,
						 CORBA_Object_ptr obj,
						 CORBA_Environment &env);
  static void _ttcp_sequence_sendOctetSeq_skel (CORBA_ServerRequest &req,
						CORBA_Object_ptr obj,
						CORBA_Environment &env);
  static void _ttcp_sequence_sendCharSeq_skel (CORBA_ServerRequest &req,
					       CORBA_Object_ptr obj,
					       CORBA_Environment &env);
  static void _ttcp_sequence_sendStructSeq_skel (CORBA_ServerRequest &req,
						 CORBA_Object_ptr obj,
						 CORBA_Environment &env);
  static void _ttcp_sequence_start_timer_skel (CORBA_ServerRequest &req,
					       CORBA_Object_ptr obj,
					       CORBA_Environment &env);
  static void _ttcp_sequence_stop_timer_skel (CORBA_ServerRequest &req,
					      CORBA_Object_ptr obj,
					      CORBA_Environment &env);
protected:
  _skel_ttcp_sequence(const char *obj_name = 0);
  virtual ~_skel_ttcp_sequence() {}
};

#endif




