/**************************************************************************
This file has been hand-crafted to work with the ORB API of TAO
***************************************************************************/

#ifndef TTCP_SEQUENCE_C_H
#define TTCP_SEQUENCE_C_H

#include <stdlib.h>
#include <string.h>

#include "corba/orb.h"
#include "corba/stub.h"
#include "debug.h"

// mapping for PerfStruct
struct PerfStruct{
  CORBA_Short s;
  CORBA_Char  c;
  CORBA_Long  l;
  CORBA_Octet o;
  CORBA_Double d;
  CORBA_Octet pad[8];


  typedef CORBA_Octet pad_slice;
  typedef pad_slice *pad_slice_ptr;

  static inline pad_slice *pad_alloc() {
    return new CORBA_Octet[8];
  }

  static inline void pad_free(pad_slice *data) {
    if (data) delete[] data;
  }

};
extern CORBA_TypeCode_ptr _tc_PerfStruct;

class ttcp_sequence;
typedef ttcp_sequence *ttcp_sequence_ptr;
typedef ttcp_sequence_ptr ttcp_sequenceRef;

class ttcp_sequence: public virtual CORBA_Object
{
public:
  static ttcp_sequence_ptr _duplicate(ttcp_sequence_ptr obj);
  static ttcp_sequence_ptr _narrow(CORBA_Object_ptr obj);
  static ttcp_sequence_ptr _nil();


  class ShortSeq
  {
  public:
    ShortSeq();
    ShortSeq(CORBA_ULong max);
    ShortSeq(CORBA_ULong max,
	     CORBA_ULong length,
	     CORBA_Short *value,
	     CORBA_Boolean release = CORBA_B_FALSE
	     );
    //    ShortSeq(const ShortSeq&);
    ~ShortSeq();
    CORBA_ULong maximum() const;
    void length(CORBA_ULong);
    CORBA_ULong length() const;
    CORBA_Short &operator[](CORBA_ULong index);
    const CORBA_Short &operator[](CORBA_ULong index) const;
  private:
    CORBA_ULong _maximum;
    CORBA_ULong _length;
    CORBA_Short *_buffer;
    CORBA_Boolean _release;
  };
  static CORBA_TypeCode_ptr _tc_ShortSeq;

  class LongSeq
  {
  public:
    LongSeq();
    LongSeq(CORBA_ULong max);
    LongSeq(CORBA_ULong max,
	    CORBA_ULong length,
	    CORBA_Long *value,
	    CORBA_Boolean release = CORBA_B_FALSE
	    );
    //    LongSeq(const LongSeq&);
    ~LongSeq();
    CORBA_ULong maximum() const;
    void length(CORBA_ULong);
    CORBA_ULong length() const;
    CORBA_Long &operator[](CORBA_ULong index);
    const CORBA_Long &operator[](CORBA_ULong index) const;
  private:
    CORBA_ULong _maximum;
    CORBA_ULong _length;
    CORBA_Long *_buffer;
    CORBA_Boolean _release;
  };
  static CORBA_TypeCode_ptr _tc_LongSeq;

  class DoubleSeq
  {
  public:
    DoubleSeq();
    DoubleSeq(CORBA_ULong max);
    DoubleSeq(CORBA_ULong max,
	      CORBA_ULong length,
	      CORBA_Double *value,
	      CORBA_Boolean release = CORBA_B_FALSE
	      );
    //    DoubleSeq(const DoubleSeq&);
    ~DoubleSeq();
    CORBA_ULong maximum() const;
    void length(CORBA_ULong);
    CORBA_ULong length() const;
    CORBA_Double &operator[](CORBA_ULong index);
    const CORBA_Double &operator[](CORBA_ULong index) const;
  private:
    CORBA_ULong _maximum;
    CORBA_ULong _length;
    CORBA_Double *_buffer;
    CORBA_Boolean _release;
  };
  static CORBA_TypeCode_ptr _tc_DoubleSeq;

  class CharSeq
  {
  public:
    CharSeq();
    CharSeq(CORBA_ULong max);
    CharSeq(CORBA_ULong max,
	    CORBA_ULong length,
	    CORBA_Char *value,
	    CORBA_Boolean release = CORBA_B_FALSE
	    );
    //    CharSeq(const CharSeq&);
    ~CharSeq();
    CORBA_ULong maximum() const;
    void length(CORBA_ULong);
    CORBA_ULong length() const;
    CORBA_Char &operator[](CORBA_ULong index);
    const CORBA_Char &operator[](CORBA_ULong index) const;
  private:
    CORBA_ULong _maximum;
    CORBA_ULong _length;
    CORBA_Char *_buffer;
    CORBA_Boolean _release;
  };
  static CORBA_TypeCode_ptr _tc_CharSeq;

  class OctetSeq
  {
  public:
    OctetSeq();
    OctetSeq(CORBA_ULong max);
    OctetSeq(CORBA_ULong max,
	     CORBA_ULong length,
	     CORBA_Octet *value,
	     CORBA_Boolean release = CORBA_B_FALSE
	     );
    //    OctetSeq(const OctetSeq&);
    ~OctetSeq();
    CORBA_ULong maximum() const;
    void length(CORBA_ULong);
    CORBA_ULong length() const;
    CORBA_Octet &operator[](CORBA_ULong index);
    const CORBA_Octet &operator[](CORBA_ULong index) const;
  private:
    CORBA_ULong _maximum;
    CORBA_ULong _length;
    CORBA_Octet *_buffer;
    CORBA_Boolean _release;
  };
  static CORBA_TypeCode_ptr _tc_OctetSeq;

  class StructSeq
  {
  public:
    StructSeq();
    StructSeq(CORBA_ULong max);
    StructSeq(CORBA_ULong max,
	      CORBA_ULong length,
	      PerfStruct *value,
	      CORBA_Boolean release = CORBA_B_FALSE
	      );
    //    StructSeq(const StructSeq&);
    ~StructSeq();
    CORBA_ULong maximum() const;
    void length(CORBA_ULong);
    CORBA_ULong length() const;
    PerfStruct &operator[](CORBA_ULong index);
    const PerfStruct &operator[](CORBA_ULong index) const;
  private:
    CORBA_ULong _maximum;
    CORBA_ULong _length;
    PerfStruct *_buffer;
    CORBA_Boolean _release;
  };
  static CORBA_TypeCode_ptr _tc_StructSeq;


  // now the methods
  virtual void sendShortSeq(const ShortSeq& ttcp_seq, CORBA_Environment &);
  virtual void sendLongSeq(const LongSeq& ttcp_seq, CORBA_Environment &);
  virtual void sendDoubleSeq(const DoubleSeq& ttcp_seq, CORBA_Environment &);
  virtual void sendOctetSeq(const OctetSeq& ttcp_seq, CORBA_Environment &);
  virtual void sendCharSeq(const CharSeq& ttcp_seq, CORBA_Environment &);
  virtual void sendStructSeq(const StructSeq& ttcp_seq, CORBA_Environment &);

  virtual void start_timer(CORBA_Environment &);
  virtual void stop_timer(CORBA_Environment &);

protected:
  ttcp_sequence() {}
  ttcp_sequence(STUB_Object *refdata);
  virtual ~ttcp_sequence() {}

private:
  ttcp_sequence(const ttcp_sequence &) {}
  void operator=(const ttcp_sequence &) {}
};

#endif




