

#include "CosNaming.hh"

const CORBA::TypeCode_ptr CosNaming::_tc_Istring = "\
0\
"
;


#ifndef CosNaming__IDL_SEQUENCE_CosNaming_NameComponent_TC
#define CosNaming__IDL_SEQUENCE_CosNaming_NameComponent_TC


const CORBA::TypeCode_ptr CosNaming::_tc__IDL_SEQUENCE_CosNaming_NameComponent = "\
S{R~CosNaming::NameComponent~id{0},kind{0}},0\
"
;

#endif


#ifndef CosNaming__IDL_SEQUENCE_CosNaming_NameComponent_Ops
#define CosNaming__IDL_SEQUENCE_CosNaming_NameComponent_Ops

CosNaming::_IDL_SEQUENCE_CosNaming_NameComponent:: ~_IDL_SEQUENCE_CosNaming_NameComponent() {
    if (_buffer && _release) {
        {
        }
        freebuf (_buffer);
    }
}

CosNaming::_IDL_SEQUENCE_CosNaming_NameComponent:: _IDL_SEQUENCE_CosNaming_NameComponent () {
    _release = 1;
    _length = 0;
    _maximum = 0;
    _buffer = NULL;
}


CosNaming::_IDL_SEQUENCE_CosNaming_NameComponent:: _IDL_SEQUENCE_CosNaming_NameComponent (CORBA::ULong max, CORBA::ULong length, CosNaming::NameComponent* data, CORBA::Boolean release) {
    _release = release;
    _length = length;
    _maximum = max;
    _buffer = data;
}

CosNaming::_IDL_SEQUENCE_CosNaming_NameComponent:: _IDL_SEQUENCE_CosNaming_NameComponent (const CosNaming::_IDL_SEQUENCE_CosNaming_NameComponent &IT_s) {
    _length = IT_s._length;
    _release = 1;
    CORBA::ULong IT_maximum = _maximum = IT_s._maximum;
    if (IT_maximum) {
        _buffer = allocbuf (IT_maximum);
        for (CORBA::ULong IT_i = 0; IT_i < _length; IT_i++) {
            _buffer[IT_i] = IT_s._buffer[IT_i];
        }
    } else
        _buffer = NULL;
}

CosNaming::_IDL_SEQUENCE_CosNaming_NameComponent:: _IDL_SEQUENCE_CosNaming_NameComponent (CORBA::ULong IT_size) {
    if (IT_size) {
        _buffer = allocbuf (IT_size);
    } else
        _buffer = NULL;
    _length = 0;
    _release = 1;
    _maximum = IT_size;
}

CosNaming::_IDL_SEQUENCE_CosNaming_NameComponent& CosNaming::_IDL_SEQUENCE_CosNaming_NameComponent:: operator= (const CosNaming::_IDL_SEQUENCE_CosNaming_NameComponent &IT_s){
    if (this == &IT_s) return *this;
    if (_buffer && _release) {
    }
    if (!_release || (IT_s._maximum > _maximum)) {
        if (_buffer && _release) {
            freebuf (_buffer);
        }
        _buffer = allocbuf (IT_s._maximum);
        _maximum = IT_s._maximum;
        _release = 1;
    }
    _length = IT_s._length;
    for (CORBA::ULong IT_j = 0; IT_j < _length; IT_j++) {
        _buffer[IT_j] = IT_s._buffer[IT_j];
    }
    return *this;
}

CORBA::ULong CosNaming::_IDL_SEQUENCE_CosNaming_NameComponent:: maximum () const
{
    return _maximum;
}

CORBA::ULong CosNaming::_IDL_SEQUENCE_CosNaming_NameComponent:: length () const
{
    return _length;
}

void CosNaming::_IDL_SEQUENCE_CosNaming_NameComponent:: length (CORBA::ULong len) {
    if (len > _maximum) {
        CosNaming::_IDL_SEQUENCE_CosNaming_NameComponent tempSeq (*this);
        if (_buffer && _release) {
            {
            }
            freebuf (_buffer);
        }
        _release = 1;
        _length = len;
        _maximum = len;
        _buffer = allocbuf(len);
        for (CORBA::ULong IT_i = 0; IT_i < tempSeq._length; IT_i++) {
            _buffer[IT_i] = tempSeq._buffer[IT_i];
        }
    } else if (len < _length) {
        if (_release) {
        }
        _length = len;
    } else {
        _length = len;
    }
}

CosNaming::NameComponent& CosNaming::_IDL_SEQUENCE_CosNaming_NameComponent:: operator [] (CORBA::ULong IT_i)  {
    if (IT_i >= _length)
        ;
    return _buffer [IT_i];
}

const CosNaming::NameComponent& CosNaming::_IDL_SEQUENCE_CosNaming_NameComponent:: operator [] (CORBA::ULong IT_i) const  {
    if (IT_i >= _length)
        ;
    return _buffer [IT_i];
}

CosNaming::NameComponent* CosNaming::_IDL_SEQUENCE_CosNaming_NameComponent:: allocbuf (CORBA::ULong nelems)
{
    return (new CosNaming::NameComponent[nelems]);
}

void CosNaming::_IDL_SEQUENCE_CosNaming_NameComponent:: freebuf (CosNaming::NameComponent* data)
{
    if (data) delete [] data;
}

void CosNaming::_IDL_SEQUENCE_CosNaming_NameComponent:: decodeInOutOp (CORBA::Request &IT_r) {
    CORBA::ULong IT_max;
    CORBA::ULong IT_length;
    IT_r >> IT_max;
    IT_r >> IT_length;
    unsigned char IT_codeSeqElems = IT_max ? 1 : 0;
    if (!_release || (IT_max > _maximum)) {
        if (_buffer && _release) {
            {
            }
            freebuf (_buffer);
        }
        _buffer = allocbuf (IT_max);
        _maximum = IT_max;
        _release = 1;
    }
    if (IT_length > IT_max)
        IT_r.makeRuntimeException4 ();
    else {
        for (CORBA::ULong IT_i = 0; IT_i < IT_length; IT_i++) {
            _buffer[IT_i].decodeInOutOp (IT_r);
        }
    }
    _length = IT_length;
}

void CosNaming::_IDL_SEQUENCE_CosNaming_NameComponent:: encodeOp (CORBA::Request &IT_r) const {
    IT_r << _maximum;
    IT_r << _length;
    unsigned char IT_codeSeqElems = _maximum ? 1 : 0;
    for (CORBA::ULong IT_i = 0; IT_i < _length; IT_i++) {
        _buffer[IT_i].encodeOp (IT_r);
    }
}

void CosNaming::_IDL_SEQUENCE_CosNaming_NameComponent:: decodeOp (CORBA::Request &IT_r) {
    IT_r >> _maximum;
    IT_r >> _length;
    unsigned char IT_codeSeqElems = _maximum ? 1 : 0;
    if (IT_codeSeqElems) {
        _buffer = allocbuf (_maximum);
        for (CORBA::ULong IT_i = 0; IT_i < _length; IT_i++) {
            _buffer[IT_i].decodeOp (IT_r);
        }
    }
    else
      _buffer = NULL;
}


CosNaming::NameComponent& CosNaming::_IDL_SEQUENCE_CosNaming_NameComponent_var:: operator[] (CORBA::ULong index) {
    return (*_ptr) [index];
}

const CosNaming::NameComponent& CosNaming::_IDL_SEQUENCE_CosNaming_NameComponent_var:: operator[] (CORBA::ULong index) const {
    return (*_ptr) [index];
}


#endif


#ifndef CosNaming_NameComponent_Ops
#define CosNaming_NameComponent_Ops

void CosNaming::NameComponent:: encodeOp (CORBA::Request &IT_r) const {

    IT_r.encodeStringOp (id);

    IT_r.encodeStringOp (kind);
}

void CosNaming::NameComponent:: decodeOp (CORBA::Request &IT_r) {

    {
        char *IT_0 = 0;
        IT_r.decodeStringOp(IT_0);
        id = IT_0;
    }

    {
        char *IT_1 = 0;
        IT_r.decodeStringOp(IT_1);
        kind = IT_1;
    }
}

void CosNaming::NameComponent:: decodeInOutOp (CORBA::Request &IT_r) {

    {
        char* IT_2;
        IT_2 = id;
        (id).release (0);
        IT_r.decodeInOutStrOp(IT_2, 0);
        id = IT_2;
    }

    {
        char* IT_3;
        IT_3 = kind;
        (kind).release (0);
        IT_r.decodeInOutStrOp(IT_3, 0);
        kind = IT_3;
    }
}

void* CosNaming::NameComponent:: IT_anySupport (CORBA::Request &IT_r,
		void *& IT_v, void *IT_to, const CORBA::Flags& IT_f) {
    CosNaming::NameComponent* IT_l = (CosNaming::NameComponent*)IT_v;

    if (IT_f.isSetAll (CORBA::ARG_INOUT)) {
        if (!IT_l)
            IT_l = new CosNaming::NameComponent();
        IT_l -> decodeInOutOp (IT_r);
        IT_v = IT_l;
    }
    else if (IT_f.isSet (CORBA::ARG_IN)) {
        IT_l -> encodeOp (IT_r);
    }
    else if (IT_f.isSet (CORBA::ARG_OUT)) {
        if (!IT_l)
            IT_l = new CosNaming::NameComponent();
        IT_l -> decodeOp (IT_r);
        IT_v = IT_l;
    }
    else if (IT_f.isSet (CORBA::_ANY_ASSIGN)) {
       CosNaming::NameComponent*IT_s = IT_to ? (CosNaming::NameComponent*)IT_to : new CosNaming::NameComponent;
       *IT_s = *IT_l;  return IT_s;
    }
    else if (IT_f.isSet (CORBA::_ANY_DELETE)) {
       if (IT_to) IT_l->NameComponent::~NameComponent();
       else delete IT_l;
       return NULL;
    }
    else if (IT_f.isSet (CORBA::_ANY_SIZEOF)) {
       return (void*) (sizeof (CosNaming::NameComponent));
    }
    else if (IT_f.isNil ()) {
        if (!IT_l)
            IT_l = new CosNaming::NameComponent();
        IT_l -> decodeOp (IT_r);
        IT_v = IT_l;
    }
    return NULL;
}

const void *CosNaming::NameComponent:: IT_fn =
CORBA::anyTable.record ("CosNaming::NameComponent", &CosNaming::NameComponent:: IT_anySupport);

CosNaming::NameComponent ::~NameComponent () {
}

CosNaming::NameComponent:: NameComponent (const CosNaming::NameComponent &IT_s) 
	 {
    id = IT_s.id;
    kind = IT_s.kind;
}

CosNaming::NameComponent:: NameComponent () {
}

CosNaming::NameComponent &CosNaming::NameComponent:: operator= (const CosNaming::NameComponent& IT_s) {
    if (this == &IT_s) return *this;
    id = IT_s.id;
    kind = IT_s.kind;
    return *this;
}

const CORBA::TypeCode_ptr CosNaming::_tc_NameComponent = "\
R~CosNaming::NameComponent~id{0},kind{0}\
"
;


#endif

const CORBA::TypeCode_ptr CosNaming::_tc_Name = "\
S{R~CosNaming::NameComponent~id{0},kind{0}},0\
"
;

const CORBA::TypeCode_ptr CosNaming::_tc_BindingType = "\
E~CosNaming::BindingType~nobject,ncontext\
"
;


#ifndef CosNaming__IDL_SEQUENCE_CosNaming_Binding_TC
#define CosNaming__IDL_SEQUENCE_CosNaming_Binding_TC


const CORBA::TypeCode_ptr CosNaming::_tc__IDL_SEQUENCE_CosNaming_Binding = "\
S{R~CosNaming::Binding~binding_name{S{R~CosNaming::NameComponent~id{0},kind{0}},0},binding_type{E~CosNaming::BindingType~nobject,ncontext}},0\
"
;

#endif


#ifndef CosNaming__IDL_SEQUENCE_CosNaming_Binding_Ops
#define CosNaming__IDL_SEQUENCE_CosNaming_Binding_Ops

CosNaming::_IDL_SEQUENCE_CosNaming_Binding:: ~_IDL_SEQUENCE_CosNaming_Binding() {
    if (_buffer && _release) {
        {
        }
        freebuf (_buffer);
    }
}

CosNaming::_IDL_SEQUENCE_CosNaming_Binding:: _IDL_SEQUENCE_CosNaming_Binding () {
    _release = 1;
    _length = 0;
    _maximum = 0;
    _buffer = NULL;
}


CosNaming::_IDL_SEQUENCE_CosNaming_Binding:: _IDL_SEQUENCE_CosNaming_Binding (CORBA::ULong max, CORBA::ULong length, CosNaming::Binding* data, CORBA::Boolean release) {
    _release = release;
    _length = length;
    _maximum = max;
    _buffer = data;
}

CosNaming::_IDL_SEQUENCE_CosNaming_Binding:: _IDL_SEQUENCE_CosNaming_Binding (const CosNaming::_IDL_SEQUENCE_CosNaming_Binding &IT_s) {
    _length = IT_s._length;
    _release = 1;
    CORBA::ULong IT_maximum = _maximum = IT_s._maximum;
    if (IT_maximum) {
        _buffer = allocbuf (IT_maximum);
        for (CORBA::ULong IT_i = 0; IT_i < _length; IT_i++) {
            _buffer[IT_i] = IT_s._buffer[IT_i];
        }
    } else
        _buffer = NULL;
}

CosNaming::_IDL_SEQUENCE_CosNaming_Binding:: _IDL_SEQUENCE_CosNaming_Binding (CORBA::ULong IT_size) {
    if (IT_size) {
        _buffer = allocbuf (IT_size);
    } else
        _buffer = NULL;
    _length = 0;
    _release = 1;
    _maximum = IT_size;
}

CosNaming::_IDL_SEQUENCE_CosNaming_Binding& CosNaming::_IDL_SEQUENCE_CosNaming_Binding:: operator= (const CosNaming::_IDL_SEQUENCE_CosNaming_Binding &IT_s){
    if (this == &IT_s) return *this;
    if (_buffer && _release) {
    }
    if (!_release || (IT_s._maximum > _maximum)) {
        if (_buffer && _release) {
            freebuf (_buffer);
        }
        _buffer = allocbuf (IT_s._maximum);
        _maximum = IT_s._maximum;
        _release = 1;
    }
    _length = IT_s._length;
    for (CORBA::ULong IT_j = 0; IT_j < _length; IT_j++) {
        _buffer[IT_j] = IT_s._buffer[IT_j];
    }
    return *this;
}

CORBA::ULong CosNaming::_IDL_SEQUENCE_CosNaming_Binding:: maximum () const
{
    return _maximum;
}

CORBA::ULong CosNaming::_IDL_SEQUENCE_CosNaming_Binding:: length () const
{
    return _length;
}

void CosNaming::_IDL_SEQUENCE_CosNaming_Binding:: length (CORBA::ULong len) {
    if (len > _maximum) {
        CosNaming::_IDL_SEQUENCE_CosNaming_Binding tempSeq (*this);
        if (_buffer && _release) {
            {
            }
            freebuf (_buffer);
        }
        _release = 1;
        _length = len;
        _maximum = len;
        _buffer = allocbuf(len);
        for (CORBA::ULong IT_i = 0; IT_i < tempSeq._length; IT_i++) {
            _buffer[IT_i] = tempSeq._buffer[IT_i];
        }
    } else if (len < _length) {
        if (_release) {
        }
        _length = len;
    } else {
        _length = len;
    }
}

CosNaming::Binding& CosNaming::_IDL_SEQUENCE_CosNaming_Binding:: operator [] (CORBA::ULong IT_i)  {
    if (IT_i >= _length)
        ;
    return _buffer [IT_i];
}

const CosNaming::Binding& CosNaming::_IDL_SEQUENCE_CosNaming_Binding:: operator [] (CORBA::ULong IT_i) const  {
    if (IT_i >= _length)
        ;
    return _buffer [IT_i];
}

CosNaming::Binding* CosNaming::_IDL_SEQUENCE_CosNaming_Binding:: allocbuf (CORBA::ULong nelems)
{
    return (new CosNaming::Binding[nelems]);
}

void CosNaming::_IDL_SEQUENCE_CosNaming_Binding:: freebuf (CosNaming::Binding* data)
{
    if (data) delete [] data;
}

void CosNaming::_IDL_SEQUENCE_CosNaming_Binding:: decodeInOutOp (CORBA::Request &IT_r) {
    CORBA::ULong IT_max;
    CORBA::ULong IT_length;
    IT_r >> IT_max;
    IT_r >> IT_length;
    unsigned char IT_codeSeqElems = IT_max ? 1 : 0;
    if (!_release || (IT_max > _maximum)) {
        if (_buffer && _release) {
            {
            }
            freebuf (_buffer);
        }
        _buffer = allocbuf (IT_max);
        _maximum = IT_max;
        _release = 1;
    }
    if (IT_length > IT_max)
        IT_r.makeRuntimeException4 ();
    else {
        for (CORBA::ULong IT_i = 0; IT_i < IT_length; IT_i++) {
            _buffer[IT_i].decodeInOutOp (IT_r);
        }
    }
    _length = IT_length;
}

void CosNaming::_IDL_SEQUENCE_CosNaming_Binding:: encodeOp (CORBA::Request &IT_r) const {
    IT_r << _maximum;
    IT_r << _length;
    unsigned char IT_codeSeqElems = _maximum ? 1 : 0;
    for (CORBA::ULong IT_i = 0; IT_i < _length; IT_i++) {
        _buffer[IT_i].encodeOp (IT_r);
    }
}

void CosNaming::_IDL_SEQUENCE_CosNaming_Binding:: decodeOp (CORBA::Request &IT_r) {
    IT_r >> _maximum;
    IT_r >> _length;
    unsigned char IT_codeSeqElems = _maximum ? 1 : 0;
    if (IT_codeSeqElems) {
        _buffer = allocbuf (_maximum);
        for (CORBA::ULong IT_i = 0; IT_i < _length; IT_i++) {
            _buffer[IT_i].decodeOp (IT_r);
        }
    }
    else
      _buffer = NULL;
}


CosNaming::Binding& CosNaming::_IDL_SEQUENCE_CosNaming_Binding_var:: operator[] (CORBA::ULong index) {
    return (*_ptr) [index];
}

const CosNaming::Binding& CosNaming::_IDL_SEQUENCE_CosNaming_Binding_var:: operator[] (CORBA::ULong index) const {
    return (*_ptr) [index];
}


#endif


#ifndef CosNaming_Binding_Ops
#define CosNaming_Binding_Ops

void CosNaming::Binding:: encodeOp (CORBA::Request &IT_r) const {

    binding_name.encodeOp (IT_r);

    {
    CORBA::ULong IT_0 = (CORBA::ULong) binding_type;
    IT_r << IT_0;
    }
}

void CosNaming::Binding:: decodeOp (CORBA::Request &IT_r) {

    binding_name.decodeOp (IT_r);

    {
    CORBA::ULong IT_1;
    IT_r >> IT_1;
    binding_type = CosNaming::BindingType(IT_1);
    }
}

void CosNaming::Binding:: decodeInOutOp (CORBA::Request &IT_r) {

    binding_name.decodeInOutOp (IT_r);

    {
    CORBA::ULong IT_2;
    IT_r >> IT_2;
    binding_type = CosNaming::BindingType(IT_2);
    }
}

void* CosNaming::Binding:: IT_anySupport (CORBA::Request &IT_r,
		void *& IT_v, void *IT_to, const CORBA::Flags& IT_f) {
    CosNaming::Binding* IT_l = (CosNaming::Binding*)IT_v;

    if (IT_f.isSetAll (CORBA::ARG_INOUT)) {
        if (!IT_l)
            IT_l = new CosNaming::Binding();
        IT_l -> decodeInOutOp (IT_r);
        IT_v = IT_l;
    }
    else if (IT_f.isSet (CORBA::ARG_IN)) {
        IT_l -> encodeOp (IT_r);
    }
    else if (IT_f.isSet (CORBA::ARG_OUT)) {
        if (!IT_l)
            IT_l = new CosNaming::Binding();
        IT_l -> decodeOp (IT_r);
        IT_v = IT_l;
    }
    else if (IT_f.isSet (CORBA::_ANY_ASSIGN)) {
       CosNaming::Binding*IT_s = IT_to ? (CosNaming::Binding*)IT_to : new CosNaming::Binding;
       *IT_s = *IT_l;  return IT_s;
    }
    else if (IT_f.isSet (CORBA::_ANY_DELETE)) {
       if (IT_to) IT_l->Binding::~Binding();
       else delete IT_l;
       return NULL;
    }
    else if (IT_f.isSet (CORBA::_ANY_SIZEOF)) {
       return (void*) (sizeof (CosNaming::Binding));
    }
    else if (IT_f.isNil ()) {
        if (!IT_l)
            IT_l = new CosNaming::Binding();
        IT_l -> decodeOp (IT_r);
        IT_v = IT_l;
    }
    return NULL;
}

const void *CosNaming::Binding:: IT_fn =
CORBA::anyTable.record ("CosNaming::Binding", &CosNaming::Binding:: IT_anySupport);

CosNaming::Binding ::~Binding () {
}

CosNaming::Binding:: Binding (const CosNaming::Binding &IT_s) 
	 : binding_name(IT_s.binding_name) {
    binding_type = IT_s.binding_type;
}

CosNaming::Binding:: Binding () {
}

CosNaming::Binding &CosNaming::Binding:: operator= (const CosNaming::Binding& IT_s) {
    if (this == &IT_s) return *this;
    binding_name = IT_s.binding_name;
    binding_type = IT_s.binding_type;
    return *this;
}

const CORBA::TypeCode_ptr CosNaming::_tc_Binding = "\
R~CosNaming::Binding~binding_name{S{R~CosNaming::NameComponent~id{0},kind{0}},0},binding_type{E~CosNaming::BindingType~nobject,ncontext}\
"
;


#endif

const CORBA::TypeCode_ptr CosNaming::_tc_BindingList = "\
S{R~CosNaming::Binding~binding_name{S{R~CosNaming::NameComponent~id{0},kind{0}},0},binding_type{E~CosNaming::BindingType~nobject,ncontext}},0\
"
;

CosNaming::NamingContext::NamingContext (char *IT_OR) {
      m_pptr = new NamingContext_dispatch (IT_OR, this,(CORBA::Object*)this);
} 
CosNaming::NamingContext::NamingContext (ObjectReference *IT_OR) {
      m_pptr = new NamingContext_dispatch (IT_OR, this,(CORBA::Object*)this);
} 

#ifndef CosNaming_NamingContextForwC
#define CosNaming_NamingContextForwC
CORBA::ObjectRef CosNaming::NamingContext_getBase(void *IT_p){
    return (CosNaming::NamingContext*) IT_p;}

void CosNaming::NamingContext_release (CosNaming::NamingContext_ptr IT_p, CORBA::Environment &IT_env) {
    CORBA::release(IT_p, IT_env);}

void CosNaming::NamingContext_release (CosNaming::NamingContext_ptr IT_p) {
    CosNaming::NamingContext_release (IT_p, CORBA::IT_chooseDefaultEnv ()); }

CosNaming::NamingContext_ptr CosNaming::NamingContext_nil (CORBA::Environment &) {
    return CosNaming::NamingContext:: _nil ();}

CosNaming::NamingContext_ptr CosNaming::NamingContext_nil () {
    return CosNaming::NamingContext_nil (CORBA::IT_chooseDefaultEnv ());}

CosNaming::NamingContext_ptr CosNaming::NamingContext_duplicate (CosNaming::NamingContext_ptr IT_p, CORBA::Environment &IT_env) {
    return (CosNaming::NamingContext::_duplicate(IT_p, IT_env)); }
CosNaming::NamingContext_ptr CosNaming::NamingContext_duplicate (CosNaming::NamingContext_ptr IT_p) {
    return CosNaming::NamingContext_duplicate (IT_p, CORBA::IT_chooseDefaultEnv ()); }
#endif

CosNaming::NamingContext_ptr CosNaming::NamingContext::_duplicate(NamingContext_ptr obj, CORBA::Environment& IT_pEnv) {
        CORBA::EnvExcRaiser IT_raise;
        CORBA::Environment &IT_env = IT_raise.registerEnv (&IT_pEnv);
       if (!obj) {
          return (obj);
       }
       NamingContext_ptr IT_obj = obj->__duplicate (IT_env);
       return IT_obj;
}



CosNaming::NamingContext* CosNaming::NamingContext:: _bind (const char* IT_markerServer, const char* host,
		const CORBA::Context &IT_c, 
		CORBA::Environment &IT_env) {
       return (NamingContext*)CORBA::Factory.New (IT_markerServer, IT_env, IT_c, host, 
		CosNaming_NamingContext_IMPL, CosNaming_NamingContext_IR);
}



CosNaming::NamingContext* CosNaming::NamingContext:: _bind (CORBA::Environment &IT_env) {
       return _bind (NULL,NULL,CORBA::Context(), IT_env); }


CosNaming::NamingContext* CosNaming::NamingContext:: _bind (const char* IT_markerServer, const char* host,
                CORBA::Environment &IT_env) {
       return _bind (IT_markerServer, host, CORBA::Context (), IT_env); }
CosNaming::NamingContext* CosNaming::NamingContext::_narrow (CORBA::Object* IT_obj, CORBA::Environment &IT_pEnv) {
        CORBA::EnvExcRaiser IT_raise;
        CORBA::Environment &IT_env = IT_raise.registerEnv (&IT_pEnv);
        if (CORBA::is_nil (IT_obj)) {
                return NamingContext:: _nil();
        }

       CosNaming::NamingContext* IT_p = (CosNaming::NamingContext*)CORBA::Object::_castDown (IT_obj, CosNaming_NamingContext_IR, IT_env);
        IT_p = IT_p ? IT_p->__duplicate(IT_env) : NULL;
        if ((IT_env) &&(!strcmp(IT_env.exception()->id(), "CORBA::StExcep::BAD_PARAM")) && (((CORBA::BAD_PARAM*)IT_env.exception())-> minor() == NARROW_FAILED)) {
                IT_env.exception_free();
        }
        return IT_p;
   }

const CORBA::TypeCode_ptr CosNaming::NamingContext::_tc_NotFoundReason = "\
E~CosNaming::NamingContext::NotFoundReason~missing_node,not_context,not_object\
"
;

const char* CosNaming::NamingContext::ex_NotFound = "CosNaming::NamingContext::NotFound";
const char* CosNaming::NamingContext::NotFound:: _ex  = "CosNaming::NamingContext::NotFound";

CORBA::Exception* CosNaming::NamingContext::NotFound:: copy() const {
    return new NotFound(*this);
}

CosNaming::NamingContext::NotFound:: NotFound (const CosNaming::NamingContext::NotFoundReason& _why, const CosNaming::_IDL_SEQUENCE_CosNaming_NameComponent& _rest_of_name)

		: CORBA::UserException (CosNaming::NamingContext::ex_NotFound) 
{
    why = _why;
    rest_of_name = _rest_of_name;
}

void CosNaming::NamingContext::NotFound:: throwit ()
{
    throw (*this);
}

CosNaming::NamingContext::NotFound* CosNaming::NamingContext::NotFound:: _narrow (CORBA::Exception* e)
{
    if (!strcmp(e->id(),CosNaming::NamingContext::NotFound:: _ex))
        return ((CosNaming::NamingContext::NotFound *) e);
    else return 0;
}


#ifndef CosNaming_NamingContext_NotFound_Ops
#define CosNaming_NamingContext_NotFound_Ops

void CosNaming::NamingContext::NotFound:: encodeOp (CORBA::Request &IT_r) const {

    {
    CORBA::ULong IT_0 = (CORBA::ULong) why;
    IT_r << IT_0;
    }

    rest_of_name.encodeOp (IT_r);
}

void CosNaming::NamingContext::NotFound:: decodeOp (CORBA::Request &IT_r) {

    {
    CORBA::ULong IT_1;
    IT_r >> IT_1;
    why = CosNaming::NamingContext::NotFoundReason(IT_1);
    }

    rest_of_name.decodeOp (IT_r);
}

void CosNaming::NamingContext::NotFound:: decodeInOutOp (CORBA::Request &IT_r) {

    {
    CORBA::ULong IT_2;
    IT_r >> IT_2;
    why = CosNaming::NamingContext::NotFoundReason(IT_2);
    }

    rest_of_name.decodeInOutOp (IT_r);
}

CosNaming::NamingContext::NotFound ::~NotFound () {
}

CosNaming::NamingContext::NotFound:: NotFound (const CosNaming::NamingContext::NotFound &IT_s) 
	 : CORBA::UserException (IT_s) ,rest_of_name(IT_s.rest_of_name) {
    why = IT_s.why;
}

CosNaming::NamingContext::NotFound:: NotFound () 
		: CORBA::UserException (CosNaming::NamingContext::ex_NotFound)
{
}

CosNaming::NamingContext::NotFound &CosNaming::NamingContext::NotFound:: operator= (const CosNaming::NamingContext::NotFound& IT_s) {
    if (this == &IT_s) return *this;
    why = IT_s.why;
    rest_of_name = IT_s.rest_of_name;
    return *this;
}


#endif

const char* CosNaming::NamingContext::ex_CannotProceed = "CosNaming::NamingContext::CannotProceed";
const char* CosNaming::NamingContext::CannotProceed:: _ex  = "CosNaming::NamingContext::CannotProceed";

CORBA::Exception* CosNaming::NamingContext::CannotProceed:: copy() const {
    return new CannotProceed(*this);
}

CosNaming::NamingContext::CannotProceed:: CannotProceed (const CosNaming::_IDL_SEQUENCE_CosNaming_NameComponent& _rest_of_name)

		: CORBA::UserException (CosNaming::NamingContext::ex_CannotProceed) 
{
    rest_of_name = _rest_of_name;
}

void CosNaming::NamingContext::CannotProceed:: throwit ()
{
    throw (*this);
}

CosNaming::NamingContext::CannotProceed* CosNaming::NamingContext::CannotProceed:: _narrow (CORBA::Exception* e)
{
    if (!strcmp(e->id(),CosNaming::NamingContext::CannotProceed:: _ex))
        return ((CosNaming::NamingContext::CannotProceed *) e);
    else return 0;
}


#ifndef CosNaming_NamingContext_CannotProceed_Ops
#define CosNaming_NamingContext_CannotProceed_Ops

void CosNaming::NamingContext::CannotProceed:: encodeOp (CORBA::Request &IT_r) const {

    rest_of_name.encodeOp (IT_r);
}

void CosNaming::NamingContext::CannotProceed:: decodeOp (CORBA::Request &IT_r) {

    rest_of_name.decodeOp (IT_r);
}

void CosNaming::NamingContext::CannotProceed:: decodeInOutOp (CORBA::Request &IT_r) {

    rest_of_name.decodeInOutOp (IT_r);
}

CosNaming::NamingContext::CannotProceed ::~CannotProceed () {
}

CosNaming::NamingContext::CannotProceed:: CannotProceed (const CosNaming::NamingContext::CannotProceed &IT_s) 
	 : CORBA::UserException (IT_s) ,rest_of_name(IT_s.rest_of_name) {
}

CosNaming::NamingContext::CannotProceed:: CannotProceed () 
		: CORBA::UserException (CosNaming::NamingContext::ex_CannotProceed)
{
}

CosNaming::NamingContext::CannotProceed &CosNaming::NamingContext::CannotProceed:: operator= (const CosNaming::NamingContext::CannotProceed& IT_s) {
    if (this == &IT_s) return *this;
    rest_of_name = IT_s.rest_of_name;
    return *this;
}


#endif

const char* CosNaming::NamingContext::ex_InvalidName = "CosNaming::NamingContext::InvalidName";
const char* CosNaming::NamingContext::InvalidName:: _ex  = "CosNaming::NamingContext::InvalidName";

CORBA::Exception* CosNaming::NamingContext::InvalidName:: copy() const {
    return new InvalidName(*this);
}

void CosNaming::NamingContext::InvalidName:: throwit ()
{
    throw (*this);
}

CosNaming::NamingContext::InvalidName* CosNaming::NamingContext::InvalidName:: _narrow (CORBA::Exception* e)
{
    if (!strcmp(e->id(),CosNaming::NamingContext::InvalidName:: _ex))
        return ((CosNaming::NamingContext::InvalidName *) e);
    else return 0;
}


#ifndef CosNaming_NamingContext_InvalidName_Ops
#define CosNaming_NamingContext_InvalidName_Ops

void CosNaming::NamingContext::InvalidName:: encodeOp (CORBA::Request &IT_r) const {
}

void CosNaming::NamingContext::InvalidName:: decodeOp (CORBA::Request &IT_r) {
}

void CosNaming::NamingContext::InvalidName:: decodeInOutOp (CORBA::Request &IT_r) {
}

CosNaming::NamingContext::InvalidName ::~InvalidName () {
}

CosNaming::NamingContext::InvalidName:: InvalidName (const CosNaming::NamingContext::InvalidName &IT_s) 
	 : CORBA::UserException (IT_s) {
}

CosNaming::NamingContext::InvalidName:: InvalidName () 
		: CORBA::UserException (CosNaming::NamingContext::ex_InvalidName)
{
}

CosNaming::NamingContext::InvalidName &CosNaming::NamingContext::InvalidName:: operator= (const CosNaming::NamingContext::InvalidName& IT_s) {
    if (this == &IT_s) return *this;
    return *this;
}


#endif

const char* CosNaming::NamingContext::ex_AlreadyBound = "CosNaming::NamingContext::AlreadyBound";
const char* CosNaming::NamingContext::AlreadyBound:: _ex  = "CosNaming::NamingContext::AlreadyBound";

CORBA::Exception* CosNaming::NamingContext::AlreadyBound:: copy() const {
    return new AlreadyBound(*this);
}

void CosNaming::NamingContext::AlreadyBound:: throwit ()
{
    throw (*this);
}

CosNaming::NamingContext::AlreadyBound* CosNaming::NamingContext::AlreadyBound:: _narrow (CORBA::Exception* e)
{
    if (!strcmp(e->id(),CosNaming::NamingContext::AlreadyBound:: _ex))
        return ((CosNaming::NamingContext::AlreadyBound *) e);
    else return 0;
}


#ifndef CosNaming_NamingContext_AlreadyBound_Ops
#define CosNaming_NamingContext_AlreadyBound_Ops

void CosNaming::NamingContext::AlreadyBound:: encodeOp (CORBA::Request &IT_r) const {
}

void CosNaming::NamingContext::AlreadyBound:: decodeOp (CORBA::Request &IT_r) {
}

void CosNaming::NamingContext::AlreadyBound:: decodeInOutOp (CORBA::Request &IT_r) {
}

CosNaming::NamingContext::AlreadyBound ::~AlreadyBound () {
}

CosNaming::NamingContext::AlreadyBound:: AlreadyBound (const CosNaming::NamingContext::AlreadyBound &IT_s) 
	 : CORBA::UserException (IT_s) {
}

CosNaming::NamingContext::AlreadyBound:: AlreadyBound () 
		: CORBA::UserException (CosNaming::NamingContext::ex_AlreadyBound)
{
}

CosNaming::NamingContext::AlreadyBound &CosNaming::NamingContext::AlreadyBound:: operator= (const CosNaming::NamingContext::AlreadyBound& IT_s) {
    if (this == &IT_s) return *this;
    return *this;
}


#endif

const char* CosNaming::NamingContext::ex_NotEmpty = "CosNaming::NamingContext::NotEmpty";
const char* CosNaming::NamingContext::NotEmpty:: _ex  = "CosNaming::NamingContext::NotEmpty";

CORBA::Exception* CosNaming::NamingContext::NotEmpty:: copy() const {
    return new NotEmpty(*this);
}

void CosNaming::NamingContext::NotEmpty:: throwit ()
{
    throw (*this);
}

CosNaming::NamingContext::NotEmpty* CosNaming::NamingContext::NotEmpty:: _narrow (CORBA::Exception* e)
{
    if (!strcmp(e->id(),CosNaming::NamingContext::NotEmpty:: _ex))
        return ((CosNaming::NamingContext::NotEmpty *) e);
    else return 0;
}


#ifndef CosNaming_NamingContext_NotEmpty_Ops
#define CosNaming_NamingContext_NotEmpty_Ops

void CosNaming::NamingContext::NotEmpty:: encodeOp (CORBA::Request &IT_r) const {
}

void CosNaming::NamingContext::NotEmpty:: decodeOp (CORBA::Request &IT_r) {
}

void CosNaming::NamingContext::NotEmpty:: decodeInOutOp (CORBA::Request &IT_r) {
}

CosNaming::NamingContext::NotEmpty ::~NotEmpty () {
}

CosNaming::NamingContext::NotEmpty:: NotEmpty (const CosNaming::NamingContext::NotEmpty &IT_s) 
	 : CORBA::UserException (IT_s) {
}

CosNaming::NamingContext::NotEmpty:: NotEmpty () 
		: CORBA::UserException (CosNaming::NamingContext::ex_NotEmpty)
{
}

CosNaming::NamingContext::NotEmpty &CosNaming::NamingContext::NotEmpty:: operator= (const CosNaming::NamingContext::NotEmpty& IT_s) {
    if (this == &IT_s) return *this;
    return *this;
}


#endif

const CORBA::TypeCode_ptr CosNaming::_tc_NamingContextRef = "\
O~CosNaming::NamingContext\
"
;

void* CosNaming::NamingContextProxyFactoryClass::New (char *IT_OR, CORBA::Environment&) {
        return  new class NamingContext(IT_OR);}

void* CosNaming::NamingContextProxyFactoryClass::New (ObjectReference *IT_OR, CORBA::Environment&) {
        return  new class NamingContext(IT_OR);}

void* CosNaming::NamingContextProxyFactoryClass::New2 () {
        return  new class NamingContext((char*)0);}

CORBA::Object* CosNaming::NamingContextProxyFactoryClass::NewObject (char *IT_OR, CORBA::Environment&) {
        return  new class NamingContext(IT_OR);}

CORBA::Object* CosNaming::NamingContextProxyFactoryClass::NewObject (ObjectReference *IT_OR, CORBA::Environment&) {
        return  new class NamingContext(IT_OR);}

CORBA::Object* CosNaming::NamingContextProxyFactoryClass::New2Object () {
        return  new class NamingContext((char*)0);}

void* CosNaming::NamingContextProxyFactoryClass::IT_castUp (void *IT_p, char* IT_s, CORBA::Environment &IT_env) {
      void *IT_l;
      if (!CORBA::_interfaceCmp (IT_s,CosNaming_NamingContext_IR, IT_env))
         return IT_p;
      else if (IT_l=CORBA::ObjectFactoryClass::IT_castUp((CORBA::Object*)((class CosNaming::NamingContext*)IT_p),IT_s, IT_env))
         return IT_l;
      else return NULL;
    }


CORBA::PPTR* CosNaming::NamingContextProxyFactoryClass::pptr (void *IT_p) {
       return ((class CosNaming::NamingContext*)IT_p)->_pptr ();}

void CosNaming::NamingContextProxyFactoryClass::baseInterfaces (_IDL_SEQUENCE_string& seq) {
      add (seq, CosNaming_NamingContext_IR);
      CORBA::ObjectFactoryClass::baseInterfaces (seq);
}

CosNaming::NamingContextProxyFactoryClass CosNaming::NamingContextProxyFactory(1);

    void CosNaming::NamingContext:: bind (const CosNaming::Name& n, CORBA::Object_ptr obj, CORBA::Environment &IT_pEnv) throw (CORBA::SystemException, CosNaming::NamingContext::NotFound, CosNaming::NamingContext::CannotProceed, CosNaming::NamingContext::InvalidName, CosNaming::NamingContext::AlreadyBound) {

        CORBA::EnvExcRaiser IT_raise;
        CORBA::Environment &IT_env = IT_raise.registerEnv (&IT_pEnv);
        if (IT_env || m_isNull) {
            return ;
        }
        CORBA::Request IT_r (this, "bind",IT_env,1,0);
        if (!IT_r.isException (IT_env)) {

            n.encodeOp (IT_r);


            {
                CORBA::Object* IT_l0 = (CORBA::Object*) obj;
                IT_r << IT_l0;
            }
        }

        IT_r.invoke (CORBA::Flags(0),IT_env);
        if (!IT_r.isException (IT_env)) {

        }
        else if (CORBA::UserException:: _narrow (IT_env.exception ())) { 
            const char* IT_str = IT_env.exception_id (); 
            if (!strcmp (IT_str, CosNaming::NamingContext::ex_NotFound)) {
                CORBA::Exception *IT_ex =  new CosNaming::NamingContext::NotFound;
                ((CosNaming::NamingContext::NotFound*)IT_ex) -> decodeOp (IT_r);
                IT_env = IT_ex;
            }

           else
            if (!strcmp (IT_str, CosNaming::NamingContext::ex_CannotProceed)) {
                CORBA::Exception *IT_ex =  new CosNaming::NamingContext::CannotProceed;
                ((CosNaming::NamingContext::CannotProceed*)IT_ex) -> decodeOp (IT_r);
                IT_env = IT_ex;
            }

           else
            if (!strcmp (IT_str, CosNaming::NamingContext::ex_InvalidName)) {
                CORBA::Exception *IT_ex =  new CosNaming::NamingContext::InvalidName;
                ((CosNaming::NamingContext::InvalidName*)IT_ex) -> decodeOp (IT_r);
                IT_env = IT_ex;
            }

           else
            if (!strcmp (IT_str, CosNaming::NamingContext::ex_AlreadyBound)) {
                CORBA::Exception *IT_ex =  new CosNaming::NamingContext::AlreadyBound;
                ((CosNaming::NamingContext::AlreadyBound*)IT_ex) -> decodeOp (IT_r);
                IT_env = IT_ex;
            }
        } else if (CORBA::SystemException:: _narrow (IT_env.exception ())) {
        }
        if (IT_r.exceptionKind () == CORBA::SYSTEM_EXCEPTION) {
            IT_r.checkEnv (IT_env);
        }
    }

    void CosNaming::NamingContext:: rebind (const CosNaming::Name& n, CORBA::Object_ptr obj, CORBA::Environment &IT_pEnv) throw (CORBA::SystemException, CosNaming::NamingContext::NotFound, CosNaming::NamingContext::CannotProceed, CosNaming::NamingContext::InvalidName) {

        CORBA::EnvExcRaiser IT_raise;
        CORBA::Environment &IT_env = IT_raise.registerEnv (&IT_pEnv);
        if (IT_env || m_isNull) {
            return ;
        }
        CORBA::Request IT_r (this, "rebind",IT_env,1,0);
        if (!IT_r.isException (IT_env)) {

            n.encodeOp (IT_r);


            {
                CORBA::Object* IT_l0 = (CORBA::Object*) obj;
                IT_r << IT_l0;
            }
        }

        IT_r.invoke (CORBA::Flags(0),IT_env);
        if (!IT_r.isException (IT_env)) {

        }
        else if (CORBA::UserException:: _narrow (IT_env.exception ())) { 
            const char* IT_str = IT_env.exception_id (); 
            if (!strcmp (IT_str, CosNaming::NamingContext::ex_NotFound)) {
                CORBA::Exception *IT_ex =  new CosNaming::NamingContext::NotFound;
                ((CosNaming::NamingContext::NotFound*)IT_ex) -> decodeOp (IT_r);
                IT_env = IT_ex;
            }

           else
            if (!strcmp (IT_str, CosNaming::NamingContext::ex_CannotProceed)) {
                CORBA::Exception *IT_ex =  new CosNaming::NamingContext::CannotProceed;
                ((CosNaming::NamingContext::CannotProceed*)IT_ex) -> decodeOp (IT_r);
                IT_env = IT_ex;
            }

           else
            if (!strcmp (IT_str, CosNaming::NamingContext::ex_InvalidName)) {
                CORBA::Exception *IT_ex =  new CosNaming::NamingContext::InvalidName;
                ((CosNaming::NamingContext::InvalidName*)IT_ex) -> decodeOp (IT_r);
                IT_env = IT_ex;
            }
        } else if (CORBA::SystemException:: _narrow (IT_env.exception ())) {
        }
        if (IT_r.exceptionKind () == CORBA::SYSTEM_EXCEPTION) {
            IT_r.checkEnv (IT_env);
        }
    }

    void CosNaming::NamingContext:: bind_context (const CosNaming::Name& n, CosNaming::NamingContext_ptr nc, CORBA::Environment &IT_pEnv) throw (CORBA::SystemException, CosNaming::NamingContext::NotFound, CosNaming::NamingContext::CannotProceed, CosNaming::NamingContext::InvalidName, CosNaming::NamingContext::AlreadyBound) {

        CORBA::EnvExcRaiser IT_raise;
        CORBA::Environment &IT_env = IT_raise.registerEnv (&IT_pEnv);
        if (IT_env || m_isNull) {
            return ;
        }
        CORBA::Request IT_r (this, "bind_context",IT_env,1,0);
        if (!IT_r.isException (IT_env)) {

            n.encodeOp (IT_r);


            {
                CORBA::Object* IT_l0 = (CORBA::Object*) nc;
                IT_r << IT_l0;
            }
        }

        IT_r.invoke (CORBA::Flags(0),IT_env);
        if (!IT_r.isException (IT_env)) {

        }
        else if (CORBA::UserException:: _narrow (IT_env.exception ())) { 
            const char* IT_str = IT_env.exception_id (); 
            if (!strcmp (IT_str, CosNaming::NamingContext::ex_NotFound)) {
                CORBA::Exception *IT_ex =  new CosNaming::NamingContext::NotFound;
                ((CosNaming::NamingContext::NotFound*)IT_ex) -> decodeOp (IT_r);
                IT_env = IT_ex;
            }

           else
            if (!strcmp (IT_str, CosNaming::NamingContext::ex_CannotProceed)) {
                CORBA::Exception *IT_ex =  new CosNaming::NamingContext::CannotProceed;
                ((CosNaming::NamingContext::CannotProceed*)IT_ex) -> decodeOp (IT_r);
                IT_env = IT_ex;
            }

           else
            if (!strcmp (IT_str, CosNaming::NamingContext::ex_InvalidName)) {
                CORBA::Exception *IT_ex =  new CosNaming::NamingContext::InvalidName;
                ((CosNaming::NamingContext::InvalidName*)IT_ex) -> decodeOp (IT_r);
                IT_env = IT_ex;
            }

           else
            if (!strcmp (IT_str, CosNaming::NamingContext::ex_AlreadyBound)) {
                CORBA::Exception *IT_ex =  new CosNaming::NamingContext::AlreadyBound;
                ((CosNaming::NamingContext::AlreadyBound*)IT_ex) -> decodeOp (IT_r);
                IT_env = IT_ex;
            }
        } else if (CORBA::SystemException:: _narrow (IT_env.exception ())) {
        }
        if (IT_r.exceptionKind () == CORBA::SYSTEM_EXCEPTION) {
            IT_r.checkEnv (IT_env);
        }
    }

    void CosNaming::NamingContext:: rebind_context (const CosNaming::Name& n, CosNaming::NamingContext_ptr nc, CORBA::Environment &IT_pEnv) throw (CORBA::SystemException, CosNaming::NamingContext::NotFound, CosNaming::NamingContext::CannotProceed, CosNaming::NamingContext::InvalidName) {

        CORBA::EnvExcRaiser IT_raise;
        CORBA::Environment &IT_env = IT_raise.registerEnv (&IT_pEnv);
        if (IT_env || m_isNull) {
            return ;
        }
        CORBA::Request IT_r (this, "rebind_context",IT_env,1,0);
        if (!IT_r.isException (IT_env)) {

            n.encodeOp (IT_r);


            {
                CORBA::Object* IT_l0 = (CORBA::Object*) nc;
                IT_r << IT_l0;
            }
        }

        IT_r.invoke (CORBA::Flags(0),IT_env);
        if (!IT_r.isException (IT_env)) {

        }
        else if (CORBA::UserException:: _narrow (IT_env.exception ())) { 
            const char* IT_str = IT_env.exception_id (); 
            if (!strcmp (IT_str, CosNaming::NamingContext::ex_NotFound)) {
                CORBA::Exception *IT_ex =  new CosNaming::NamingContext::NotFound;
                ((CosNaming::NamingContext::NotFound*)IT_ex) -> decodeOp (IT_r);
                IT_env = IT_ex;
            }

           else
            if (!strcmp (IT_str, CosNaming::NamingContext::ex_CannotProceed)) {
                CORBA::Exception *IT_ex =  new CosNaming::NamingContext::CannotProceed;
                ((CosNaming::NamingContext::CannotProceed*)IT_ex) -> decodeOp (IT_r);
                IT_env = IT_ex;
            }

           else
            if (!strcmp (IT_str, CosNaming::NamingContext::ex_InvalidName)) {
                CORBA::Exception *IT_ex =  new CosNaming::NamingContext::InvalidName;
                ((CosNaming::NamingContext::InvalidName*)IT_ex) -> decodeOp (IT_r);
                IT_env = IT_ex;
            }
        } else if (CORBA::SystemException:: _narrow (IT_env.exception ())) {
        }
        if (IT_r.exceptionKind () == CORBA::SYSTEM_EXCEPTION) {
            IT_r.checkEnv (IT_env);
        }
    }

    CORBA::Object_ptr CosNaming::NamingContext:: resolve (const CosNaming::Name& n, CORBA::Environment &IT_pEnv) throw (CORBA::SystemException, CosNaming::NamingContext::NotFound, CosNaming::NamingContext::CannotProceed, CosNaming::NamingContext::InvalidName) {

        CORBA::EnvExcRaiser IT_raise;
        CORBA::Environment &IT_env = IT_raise.registerEnv (&IT_pEnv);
        if (IT_env || m_isNull) {
            return NULL;
        }
        CORBA::Request IT_r (this, "resolve",IT_env,1,0);
        if (!IT_r.isException (IT_env)) {

            n.encodeOp (IT_r);
        }

        IT_r.invoke (CORBA::Flags(0),IT_env);
        if (!IT_r.isException (IT_env)) {
            CORBA::Object_ptr IT_result;
            IT_result = (CORBA::ObjectRef) IT_r.decodeObjRef ("CORBA::Object");
            IT_r.checkEnv (IT_env);
            return IT_result;
        }
        else if (CORBA::UserException:: _narrow (IT_env.exception ())) { 
            const char* IT_str = IT_env.exception_id (); 
            if (!strcmp (IT_str, CosNaming::NamingContext::ex_NotFound)) {
                CORBA::Exception *IT_ex =  new CosNaming::NamingContext::NotFound;
                ((CosNaming::NamingContext::NotFound*)IT_ex) -> decodeOp (IT_r);
                IT_env = IT_ex;
            }

           else
            if (!strcmp (IT_str, CosNaming::NamingContext::ex_CannotProceed)) {
                CORBA::Exception *IT_ex =  new CosNaming::NamingContext::CannotProceed;
                ((CosNaming::NamingContext::CannotProceed*)IT_ex) -> decodeOp (IT_r);
                IT_env = IT_ex;
            }

           else
            if (!strcmp (IT_str, CosNaming::NamingContext::ex_InvalidName)) {
                CORBA::Exception *IT_ex =  new CosNaming::NamingContext::InvalidName;
                ((CosNaming::NamingContext::InvalidName*)IT_ex) -> decodeOp (IT_r);
                IT_env = IT_ex;
            }
        } else if (CORBA::SystemException:: _narrow (IT_env.exception ())) {
        }
        if (IT_r.exceptionKind () == CORBA::SYSTEM_EXCEPTION) {
            IT_r.checkEnv (IT_env);
        }
        return NULL;
    }

    void CosNaming::NamingContext:: unbind (const CosNaming::Name& n, CORBA::Environment &IT_pEnv) throw (CORBA::SystemException, CosNaming::NamingContext::NotFound, CosNaming::NamingContext::CannotProceed, CosNaming::NamingContext::InvalidName) {

        CORBA::EnvExcRaiser IT_raise;
        CORBA::Environment &IT_env = IT_raise.registerEnv (&IT_pEnv);
        if (IT_env || m_isNull) {
            return ;
        }
        CORBA::Request IT_r (this, "unbind",IT_env,1,0);
        if (!IT_r.isException (IT_env)) {

            n.encodeOp (IT_r);
        }

        IT_r.invoke (CORBA::Flags(0),IT_env);
        if (!IT_r.isException (IT_env)) {
        }
        else if (CORBA::UserException:: _narrow (IT_env.exception ())) { 
            const char* IT_str = IT_env.exception_id (); 
            if (!strcmp (IT_str, CosNaming::NamingContext::ex_NotFound)) {
                CORBA::Exception *IT_ex =  new CosNaming::NamingContext::NotFound;
                ((CosNaming::NamingContext::NotFound*)IT_ex) -> decodeOp (IT_r);
                IT_env = IT_ex;
            }

           else
            if (!strcmp (IT_str, CosNaming::NamingContext::ex_CannotProceed)) {
                CORBA::Exception *IT_ex =  new CosNaming::NamingContext::CannotProceed;
                ((CosNaming::NamingContext::CannotProceed*)IT_ex) -> decodeOp (IT_r);
                IT_env = IT_ex;
            }

           else
            if (!strcmp (IT_str, CosNaming::NamingContext::ex_InvalidName)) {
                CORBA::Exception *IT_ex =  new CosNaming::NamingContext::InvalidName;
                ((CosNaming::NamingContext::InvalidName*)IT_ex) -> decodeOp (IT_r);
                IT_env = IT_ex;
            }
        } else if (CORBA::SystemException:: _narrow (IT_env.exception ())) {
        }
        if (IT_r.exceptionKind () == CORBA::SYSTEM_EXCEPTION) {
            IT_r.checkEnv (IT_env);
        }
    }

    CosNaming::NamingContext_ptr CosNaming::NamingContext:: new_context (CORBA::Environment &IT_pEnv) throw (CORBA::SystemException) {

        CORBA::EnvExcRaiser IT_raise;
        CORBA::Environment &IT_env = IT_raise.registerEnv (&IT_pEnv);
        if (IT_env || m_isNull) {
            return new CosNaming::NamingContext;
        }
        CORBA::Request IT_r (this, "new_context",IT_env,1,0);

        IT_r.invoke (CORBA::Flags(0),IT_env);
        if (!IT_r.isException (IT_env)) {
            CosNaming::NamingContext_ptr IT_result;
            IT_result = (CosNaming::NamingContext*) IT_r.decodeObjRef (CosNaming_NamingContext_IR);
            IT_r.checkEnv (IT_env);
            return IT_result;
        }
        if (IT_r.exceptionKind () == CORBA::SYSTEM_EXCEPTION) {
            IT_r.checkEnv (IT_env);
        }
        return new CosNaming::NamingContext;
    }

    CosNaming::NamingContext_ptr CosNaming::NamingContext:: bind_new_context (const CosNaming::Name& n, CORBA::Environment &IT_pEnv) throw (CORBA::SystemException, CosNaming::NamingContext::NotFound, CosNaming::NamingContext::AlreadyBound, CosNaming::NamingContext::CannotProceed, CosNaming::NamingContext::InvalidName) {

        CORBA::EnvExcRaiser IT_raise;
        CORBA::Environment &IT_env = IT_raise.registerEnv (&IT_pEnv);
        if (IT_env || m_isNull) {
            return new CosNaming::NamingContext;
        }
        CORBA::Request IT_r (this, "bind_new_context",IT_env,1,0);
        if (!IT_r.isException (IT_env)) {

            n.encodeOp (IT_r);
        }

        IT_r.invoke (CORBA::Flags(0),IT_env);
        if (!IT_r.isException (IT_env)) {
            CosNaming::NamingContext_ptr IT_result;
            IT_result = (CosNaming::NamingContext*) IT_r.decodeObjRef (CosNaming_NamingContext_IR);
            IT_r.checkEnv (IT_env);
            return IT_result;
        }
        else if (CORBA::UserException:: _narrow (IT_env.exception ())) { 
            const char* IT_str = IT_env.exception_id (); 
            if (!strcmp (IT_str, CosNaming::NamingContext::ex_NotFound)) {
                CORBA::Exception *IT_ex =  new CosNaming::NamingContext::NotFound;
                ((CosNaming::NamingContext::NotFound*)IT_ex) -> decodeOp (IT_r);
                IT_env = IT_ex;
            }

           else
            if (!strcmp (IT_str, CosNaming::NamingContext::ex_AlreadyBound)) {
                CORBA::Exception *IT_ex =  new CosNaming::NamingContext::AlreadyBound;
                ((CosNaming::NamingContext::AlreadyBound*)IT_ex) -> decodeOp (IT_r);
                IT_env = IT_ex;
            }

           else
            if (!strcmp (IT_str, CosNaming::NamingContext::ex_CannotProceed)) {
                CORBA::Exception *IT_ex =  new CosNaming::NamingContext::CannotProceed;
                ((CosNaming::NamingContext::CannotProceed*)IT_ex) -> decodeOp (IT_r);
                IT_env = IT_ex;
            }

           else
            if (!strcmp (IT_str, CosNaming::NamingContext::ex_InvalidName)) {
                CORBA::Exception *IT_ex =  new CosNaming::NamingContext::InvalidName;
                ((CosNaming::NamingContext::InvalidName*)IT_ex) -> decodeOp (IT_r);
                IT_env = IT_ex;
            }
        } else if (CORBA::SystemException:: _narrow (IT_env.exception ())) {
        }
        if (IT_r.exceptionKind () == CORBA::SYSTEM_EXCEPTION) {
            IT_r.checkEnv (IT_env);
        }
        return new CosNaming::NamingContext;
    }

    void CosNaming::NamingContext:: destroy (CORBA::Environment &IT_pEnv) throw (CORBA::SystemException, CosNaming::NamingContext::NotEmpty) {

        CORBA::EnvExcRaiser IT_raise;
        CORBA::Environment &IT_env = IT_raise.registerEnv (&IT_pEnv);
        if (IT_env || m_isNull) {
            return ;
        }
        CORBA::Request IT_r (this, "destroy",IT_env,1,0);

        IT_r.invoke (CORBA::Flags(0),IT_env);
        if (!IT_r.isException (IT_env)) {
        }
        else if (CORBA::UserException:: _narrow (IT_env.exception ())) { 
            const char* IT_str = IT_env.exception_id (); 
            if (!strcmp (IT_str, CosNaming::NamingContext::ex_NotEmpty)) {
                CORBA::Exception *IT_ex =  new CosNaming::NamingContext::NotEmpty;
                ((CosNaming::NamingContext::NotEmpty*)IT_ex) -> decodeOp (IT_r);
                IT_env = IT_ex;
            }
        } else if (CORBA::SystemException:: _narrow (IT_env.exception ())) {
        }
        if (IT_r.exceptionKind () == CORBA::SYSTEM_EXCEPTION) {
            IT_r.checkEnv (IT_env);
        }
    }

    void CosNaming::NamingContext:: list (CORBA::ULong how_many, CosNaming::BindingList*& bl, CosNaming::BindingIterator_ptr& bi, CORBA::Environment &IT_pEnv) throw (CORBA::SystemException) {

        CORBA::EnvExcRaiser IT_raise;
        CORBA::Environment &IT_env = IT_raise.registerEnv (&IT_pEnv);
        if (IT_env || m_isNull) {
            return ;
        }
        CORBA::Request IT_r (this, "list",IT_env,1,0);
        if (!IT_r.isException (IT_env)) {

            IT_r << how_many;


        }

        IT_r.invoke (CORBA::Flags(0),IT_env);
        if (!IT_r.isException (IT_env)) {

            bl =  new CosNaming::BindingList;

            (*bl).decodeOp (IT_r);


            bi = (CosNaming::BindingIterator*) IT_r.decodeObjRef (CosNaming_BindingIterator_IR);
            IT_r.checkEnv (IT_env);
        }
        if (IT_r.exceptionKind () == CORBA::SYSTEM_EXCEPTION) {
            IT_r.checkEnv (IT_env);
        }
    }





#ifndef CosNaming_NamingContext_dispatch_impl

unsigned char CosNaming::NamingContext_dispatch::dispatch (CORBA::Request &IT_r, 
    unsigned char, void *) {
      IT_r.makeRuntimeException1 ("CosNaming::NamingContext");
      return 0;
}
 
#endif

CosNaming::BindingIterator::BindingIterator (char *IT_OR) {
      m_pptr = new BindingIterator_dispatch (IT_OR, this,(CORBA::Object*)this);
} 
CosNaming::BindingIterator::BindingIterator (ObjectReference *IT_OR) {
      m_pptr = new BindingIterator_dispatch (IT_OR, this,(CORBA::Object*)this);
} 

#ifndef CosNaming_BindingIteratorForwC
#define CosNaming_BindingIteratorForwC
CORBA::ObjectRef CosNaming::BindingIterator_getBase(void *IT_p){
    return (CosNaming::BindingIterator*) IT_p;}

void CosNaming::BindingIterator_release (CosNaming::BindingIterator_ptr IT_p, CORBA::Environment &IT_env) {
    CORBA::release(IT_p, IT_env);}

void CosNaming::BindingIterator_release (CosNaming::BindingIterator_ptr IT_p) {
    CosNaming::BindingIterator_release (IT_p, CORBA::IT_chooseDefaultEnv ()); }

CosNaming::BindingIterator_ptr CosNaming::BindingIterator_nil (CORBA::Environment &) {
    return CosNaming::BindingIterator:: _nil ();}

CosNaming::BindingIterator_ptr CosNaming::BindingIterator_nil () {
    return CosNaming::BindingIterator_nil (CORBA::IT_chooseDefaultEnv ());}

CosNaming::BindingIterator_ptr CosNaming::BindingIterator_duplicate (CosNaming::BindingIterator_ptr IT_p, CORBA::Environment &IT_env) {
    return (CosNaming::BindingIterator::_duplicate(IT_p, IT_env)); }
CosNaming::BindingIterator_ptr CosNaming::BindingIterator_duplicate (CosNaming::BindingIterator_ptr IT_p) {
    return CosNaming::BindingIterator_duplicate (IT_p, CORBA::IT_chooseDefaultEnv ()); }
#endif

CosNaming::BindingIterator_ptr CosNaming::BindingIterator::_duplicate(BindingIterator_ptr obj, CORBA::Environment& IT_pEnv) {
        CORBA::EnvExcRaiser IT_raise;
        CORBA::Environment &IT_env = IT_raise.registerEnv (&IT_pEnv);
       if (!obj) {
          return (obj);
       }
       BindingIterator_ptr IT_obj = obj->__duplicate (IT_env);
       return IT_obj;
}



CosNaming::BindingIterator* CosNaming::BindingIterator:: _bind (const char* IT_markerServer, const char* host,
		const CORBA::Context &IT_c, 
		CORBA::Environment &IT_env) {
       return (BindingIterator*)CORBA::Factory.New (IT_markerServer, IT_env, IT_c, host, 
		CosNaming_BindingIterator_IMPL, CosNaming_BindingIterator_IR);
}



CosNaming::BindingIterator* CosNaming::BindingIterator:: _bind (CORBA::Environment &IT_env) {
       return _bind (NULL,NULL,CORBA::Context(), IT_env); }


CosNaming::BindingIterator* CosNaming::BindingIterator:: _bind (const char* IT_markerServer, const char* host,
                CORBA::Environment &IT_env) {
       return _bind (IT_markerServer, host, CORBA::Context (), IT_env); }
CosNaming::BindingIterator* CosNaming::BindingIterator::_narrow (CORBA::Object* IT_obj, CORBA::Environment &IT_pEnv) {
        CORBA::EnvExcRaiser IT_raise;
        CORBA::Environment &IT_env = IT_raise.registerEnv (&IT_pEnv);
        if (CORBA::is_nil (IT_obj)) {
                return BindingIterator:: _nil();
        }

       CosNaming::BindingIterator* IT_p = (CosNaming::BindingIterator*)CORBA::Object::_castDown (IT_obj, CosNaming_BindingIterator_IR, IT_env);
        IT_p = IT_p ? IT_p->__duplicate(IT_env) : NULL;
        if ((IT_env) &&(!strcmp(IT_env.exception()->id(), "CORBA::StExcep::BAD_PARAM")) && (((CORBA::BAD_PARAM*)IT_env.exception())-> minor() == NARROW_FAILED)) {
                IT_env.exception_free();
        }
        return IT_p;
   }

const CORBA::TypeCode_ptr CosNaming::_tc_BindingIteratorRef = "\
O~CosNaming::BindingIterator\
"
;

void* CosNaming::BindingIteratorProxyFactoryClass::New (char *IT_OR, CORBA::Environment&) {
        return  new class BindingIterator(IT_OR);}

void* CosNaming::BindingIteratorProxyFactoryClass::New (ObjectReference *IT_OR, CORBA::Environment&) {
        return  new class BindingIterator(IT_OR);}

void* CosNaming::BindingIteratorProxyFactoryClass::New2 () {
        return  new class BindingIterator((char*)0);}

CORBA::Object* CosNaming::BindingIteratorProxyFactoryClass::NewObject (char *IT_OR, CORBA::Environment&) {
        return  new class BindingIterator(IT_OR);}

CORBA::Object* CosNaming::BindingIteratorProxyFactoryClass::NewObject (ObjectReference *IT_OR, CORBA::Environment&) {
        return  new class BindingIterator(IT_OR);}

CORBA::Object* CosNaming::BindingIteratorProxyFactoryClass::New2Object () {
        return  new class BindingIterator((char*)0);}

void* CosNaming::BindingIteratorProxyFactoryClass::IT_castUp (void *IT_p, char* IT_s, CORBA::Environment &IT_env) {
      void *IT_l;
      if (!CORBA::_interfaceCmp (IT_s,CosNaming_BindingIterator_IR, IT_env))
         return IT_p;
      else if (IT_l=CORBA::ObjectFactoryClass::IT_castUp((CORBA::Object*)((class CosNaming::BindingIterator*)IT_p),IT_s, IT_env))
         return IT_l;
      else return NULL;
    }


CORBA::PPTR* CosNaming::BindingIteratorProxyFactoryClass::pptr (void *IT_p) {
       return ((class CosNaming::BindingIterator*)IT_p)->_pptr ();}

void CosNaming::BindingIteratorProxyFactoryClass::baseInterfaces (_IDL_SEQUENCE_string& seq) {
      add (seq, CosNaming_BindingIterator_IR);
      CORBA::ObjectFactoryClass::baseInterfaces (seq);
}

CosNaming::BindingIteratorProxyFactoryClass CosNaming::BindingIteratorProxyFactory(1);

    CORBA::Boolean CosNaming::BindingIterator:: next_one (CosNaming::Binding*& b, CORBA::Environment &IT_pEnv) throw (CORBA::SystemException) {

        CORBA::EnvExcRaiser IT_raise;
        CORBA::Environment &IT_env = IT_raise.registerEnv (&IT_pEnv);
        if (IT_env || m_isNull) {
            return 0;
        }
        CORBA::Request IT_r (this, "next_one",IT_env,1,0);

        IT_r.invoke (CORBA::Flags(0),IT_env);
        if (!IT_r.isException (IT_env)) {
            b =  new CosNaming::Binding;

            (*b).decodeOp (IT_r);
            CORBA::Boolean IT_result;
            IT_r >> IT_result;
            IT_r.checkEnv (IT_env);
            return IT_result;
        }
        if (IT_r.exceptionKind () == CORBA::SYSTEM_EXCEPTION) {
            IT_r.checkEnv (IT_env);
        }
        return 0;
    }

    CORBA::Boolean CosNaming::BindingIterator:: next_n (CORBA::ULong how_many, CosNaming::BindingList*& bl, CORBA::Environment &IT_pEnv) throw (CORBA::SystemException) {

        CORBA::EnvExcRaiser IT_raise;
        CORBA::Environment &IT_env = IT_raise.registerEnv (&IT_pEnv);
        if (IT_env || m_isNull) {
            return 0;
        }
        CORBA::Request IT_r (this, "next_n",IT_env,1,0);
        if (!IT_r.isException (IT_env)) {

            IT_r << how_many;

        }

        IT_r.invoke (CORBA::Flags(0),IT_env);
        if (!IT_r.isException (IT_env)) {

            bl =  new CosNaming::BindingList;

            (*bl).decodeOp (IT_r);
            CORBA::Boolean IT_result;
            IT_r >> IT_result;
            IT_r.checkEnv (IT_env);
            return IT_result;
        }
        if (IT_r.exceptionKind () == CORBA::SYSTEM_EXCEPTION) {
            IT_r.checkEnv (IT_env);
        }
        return 0;
    }

    void CosNaming::BindingIterator:: destroy (CORBA::Environment &IT_pEnv) throw (CORBA::SystemException) {

        CORBA::EnvExcRaiser IT_raise;
        CORBA::Environment &IT_env = IT_raise.registerEnv (&IT_pEnv);
        if (IT_env || m_isNull) {
            return ;
        }
        CORBA::Request IT_r (this, "destroy",IT_env,1,0);

        IT_r.invoke (CORBA::Flags(0),IT_env);
        if (IT_r.exceptionKind () == CORBA::SYSTEM_EXCEPTION) {
            IT_r.checkEnv (IT_env);
        }
    }





#ifndef CosNaming_BindingIterator_dispatch_impl

unsigned char CosNaming::BindingIterator_dispatch::dispatch (CORBA::Request &IT_r, 
    unsigned char, void *) {
      IT_r.makeRuntimeException1 ("CosNaming::BindingIterator");
      return 0;
}
 
#endif


#ifndef _IDL_SEQUENCE_CosNaming_NameComponent_TC
#define _IDL_SEQUENCE_CosNaming_NameComponent_TC


const CORBA::TypeCode_ptr _tc__IDL_SEQUENCE_CosNaming_NameComponent = "\
S{R~CosNaming::NameComponent~id{0},kind{0}},0\
"
;

#endif


#ifndef _IDL_SEQUENCE_CosNaming_NameComponent_Ops
#define _IDL_SEQUENCE_CosNaming_NameComponent_Ops

_IDL_SEQUENCE_CosNaming_NameComponent:: ~_IDL_SEQUENCE_CosNaming_NameComponent() {
    if (_buffer && _release) {
        {
        }
        freebuf (_buffer);
    }
}

_IDL_SEQUENCE_CosNaming_NameComponent:: _IDL_SEQUENCE_CosNaming_NameComponent () {
    _release = 1;
    _length = 0;
    _maximum = 0;
    _buffer = NULL;
}


_IDL_SEQUENCE_CosNaming_NameComponent:: _IDL_SEQUENCE_CosNaming_NameComponent (CORBA::ULong max, CORBA::ULong length, CosNaming::NameComponent* data, CORBA::Boolean release) {
    _release = release;
    _length = length;
    _maximum = max;
    _buffer = data;
}

_IDL_SEQUENCE_CosNaming_NameComponent:: _IDL_SEQUENCE_CosNaming_NameComponent (const _IDL_SEQUENCE_CosNaming_NameComponent &IT_s) {
    _length = IT_s._length;
    _release = 1;
    CORBA::ULong IT_maximum = _maximum = IT_s._maximum;
    if (IT_maximum) {
        _buffer = allocbuf (IT_maximum);
        for (CORBA::ULong IT_i = 0; IT_i < _length; IT_i++) {
            _buffer[IT_i] = IT_s._buffer[IT_i];
        }
    } else
        _buffer = NULL;
}

_IDL_SEQUENCE_CosNaming_NameComponent:: _IDL_SEQUENCE_CosNaming_NameComponent (CORBA::ULong IT_size) {
    if (IT_size) {
        _buffer = allocbuf (IT_size);
    } else
        _buffer = NULL;
    _length = 0;
    _release = 1;
    _maximum = IT_size;
}

_IDL_SEQUENCE_CosNaming_NameComponent& _IDL_SEQUENCE_CosNaming_NameComponent:: operator= (const _IDL_SEQUENCE_CosNaming_NameComponent &IT_s){
    if (this == &IT_s) return *this;
    if (_buffer && _release) {
    }
    if (!_release || (IT_s._maximum > _maximum)) {
        if (_buffer && _release) {
            freebuf (_buffer);
        }
        _buffer = allocbuf (IT_s._maximum);
        _maximum = IT_s._maximum;
        _release = 1;
    }
    _length = IT_s._length;
    for (CORBA::ULong IT_j = 0; IT_j < _length; IT_j++) {
        _buffer[IT_j] = IT_s._buffer[IT_j];
    }
    return *this;
}

CORBA::ULong _IDL_SEQUENCE_CosNaming_NameComponent:: maximum () const
{
    return _maximum;
}

CORBA::ULong _IDL_SEQUENCE_CosNaming_NameComponent:: length () const
{
    return _length;
}

void _IDL_SEQUENCE_CosNaming_NameComponent:: length (CORBA::ULong len) {
    if (len > _maximum) {
        _IDL_SEQUENCE_CosNaming_NameComponent tempSeq (*this);
        if (_buffer && _release) {
            {
            }
            freebuf (_buffer);
        }
        _release = 1;
        _length = len;
        _maximum = len;
        _buffer = allocbuf(len);
        for (CORBA::ULong IT_i = 0; IT_i < tempSeq._length; IT_i++) {
            _buffer[IT_i] = tempSeq._buffer[IT_i];
        }
    } else if (len < _length) {
        if (_release) {
        }
        _length = len;
    } else {
        _length = len;
    }
}

CosNaming::NameComponent& _IDL_SEQUENCE_CosNaming_NameComponent:: operator [] (CORBA::ULong IT_i)  {
    if (IT_i >= _length)
        ;
    return _buffer [IT_i];
}

const CosNaming::NameComponent& _IDL_SEQUENCE_CosNaming_NameComponent:: operator [] (CORBA::ULong IT_i) const  {
    if (IT_i >= _length)
        ;
    return _buffer [IT_i];
}

CosNaming::NameComponent* _IDL_SEQUENCE_CosNaming_NameComponent:: allocbuf (CORBA::ULong nelems)
{
    return (new CosNaming::NameComponent[nelems]);
}

void _IDL_SEQUENCE_CosNaming_NameComponent:: freebuf (CosNaming::NameComponent* data)
{
    if (data) delete [] data;
}

void _IDL_SEQUENCE_CosNaming_NameComponent:: decodeInOutOp (CORBA::Request &IT_r) {
    CORBA::ULong IT_max;
    CORBA::ULong IT_length;
    IT_r >> IT_max;
    IT_r >> IT_length;
    unsigned char IT_codeSeqElems = IT_max ? 1 : 0;
    if (!_release || (IT_max > _maximum)) {
        if (_buffer && _release) {
            {
            }
            freebuf (_buffer);
        }
        _buffer = allocbuf (IT_max);
        _maximum = IT_max;
        _release = 1;
    }
    if (IT_length > IT_max)
        IT_r.makeRuntimeException4 ();
    else {
        for (CORBA::ULong IT_i = 0; IT_i < IT_length; IT_i++) {
            _buffer[IT_i].decodeInOutOp (IT_r);
        }
    }
    _length = IT_length;
}

void _IDL_SEQUENCE_CosNaming_NameComponent:: encodeOp (CORBA::Request &IT_r) const {
    IT_r << _maximum;
    IT_r << _length;
    unsigned char IT_codeSeqElems = _maximum ? 1 : 0;
    for (CORBA::ULong IT_i = 0; IT_i < _length; IT_i++) {
        _buffer[IT_i].encodeOp (IT_r);
    }
}

void _IDL_SEQUENCE_CosNaming_NameComponent:: decodeOp (CORBA::Request &IT_r) {
    IT_r >> _maximum;
    IT_r >> _length;
    unsigned char IT_codeSeqElems = _maximum ? 1 : 0;
    if (IT_codeSeqElems) {
        _buffer = allocbuf (_maximum);
        for (CORBA::ULong IT_i = 0; IT_i < _length; IT_i++) {
            _buffer[IT_i].decodeOp (IT_r);
        }
    }
    else
      _buffer = NULL;
}


CosNaming::NameComponent& _IDL_SEQUENCE_CosNaming_NameComponent_var:: operator[] (CORBA::ULong index) {
    return (*_ptr) [index];
}

const CosNaming::NameComponent& _IDL_SEQUENCE_CosNaming_NameComponent_var:: operator[] (CORBA::ULong index) const {
    return (*_ptr) [index];
}


#endif


#ifndef _IDL_SEQUENCE_CosNaming_Binding_TC
#define _IDL_SEQUENCE_CosNaming_Binding_TC


const CORBA::TypeCode_ptr _tc__IDL_SEQUENCE_CosNaming_Binding = "\
S{R~CosNaming::Binding~binding_name{S{R~CosNaming::NameComponent~id{0},kind{0}},0},binding_type{E~CosNaming::BindingType~nobject,ncontext}},0\
"
;

#endif


#ifndef _IDL_SEQUENCE_CosNaming_Binding_Ops
#define _IDL_SEQUENCE_CosNaming_Binding_Ops

_IDL_SEQUENCE_CosNaming_Binding:: ~_IDL_SEQUENCE_CosNaming_Binding() {
    if (_buffer && _release) {
        {
        }
        freebuf (_buffer);
    }
}

_IDL_SEQUENCE_CosNaming_Binding:: _IDL_SEQUENCE_CosNaming_Binding () {
    _release = 1;
    _length = 0;
    _maximum = 0;
    _buffer = NULL;
}


_IDL_SEQUENCE_CosNaming_Binding:: _IDL_SEQUENCE_CosNaming_Binding (CORBA::ULong max, CORBA::ULong length, CosNaming::Binding* data, CORBA::Boolean release) {
    _release = release;
    _length = length;
    _maximum = max;
    _buffer = data;
}

_IDL_SEQUENCE_CosNaming_Binding:: _IDL_SEQUENCE_CosNaming_Binding (const _IDL_SEQUENCE_CosNaming_Binding &IT_s) {
    _length = IT_s._length;
    _release = 1;
    CORBA::ULong IT_maximum = _maximum = IT_s._maximum;
    if (IT_maximum) {
        _buffer = allocbuf (IT_maximum);
        for (CORBA::ULong IT_i = 0; IT_i < _length; IT_i++) {
            _buffer[IT_i] = IT_s._buffer[IT_i];
        }
    } else
        _buffer = NULL;
}

_IDL_SEQUENCE_CosNaming_Binding:: _IDL_SEQUENCE_CosNaming_Binding (CORBA::ULong IT_size) {
    if (IT_size) {
        _buffer = allocbuf (IT_size);
    } else
        _buffer = NULL;
    _length = 0;
    _release = 1;
    _maximum = IT_size;
}

_IDL_SEQUENCE_CosNaming_Binding& _IDL_SEQUENCE_CosNaming_Binding:: operator= (const _IDL_SEQUENCE_CosNaming_Binding &IT_s){
    if (this == &IT_s) return *this;
    if (_buffer && _release) {
    }
    if (!_release || (IT_s._maximum > _maximum)) {
        if (_buffer && _release) {
            freebuf (_buffer);
        }
        _buffer = allocbuf (IT_s._maximum);
        _maximum = IT_s._maximum;
        _release = 1;
    }
    _length = IT_s._length;
    for (CORBA::ULong IT_j = 0; IT_j < _length; IT_j++) {
        _buffer[IT_j] = IT_s._buffer[IT_j];
    }
    return *this;
}

CORBA::ULong _IDL_SEQUENCE_CosNaming_Binding:: maximum () const
{
    return _maximum;
}

CORBA::ULong _IDL_SEQUENCE_CosNaming_Binding:: length () const
{
    return _length;
}

void _IDL_SEQUENCE_CosNaming_Binding:: length (CORBA::ULong len) {
    if (len > _maximum) {
        _IDL_SEQUENCE_CosNaming_Binding tempSeq (*this);
        if (_buffer && _release) {
            {
            }
            freebuf (_buffer);
        }
        _release = 1;
        _length = len;
        _maximum = len;
        _buffer = allocbuf(len);
        for (CORBA::ULong IT_i = 0; IT_i < tempSeq._length; IT_i++) {
            _buffer[IT_i] = tempSeq._buffer[IT_i];
        }
    } else if (len < _length) {
        if (_release) {
        }
        _length = len;
    } else {
        _length = len;
    }
}

CosNaming::Binding& _IDL_SEQUENCE_CosNaming_Binding:: operator [] (CORBA::ULong IT_i)  {
    if (IT_i >= _length)
        ;
    return _buffer [IT_i];
}

const CosNaming::Binding& _IDL_SEQUENCE_CosNaming_Binding:: operator [] (CORBA::ULong IT_i) const  {
    if (IT_i >= _length)
        ;
    return _buffer [IT_i];
}

CosNaming::Binding* _IDL_SEQUENCE_CosNaming_Binding:: allocbuf (CORBA::ULong nelems)
{
    return (new CosNaming::Binding[nelems]);
}

void _IDL_SEQUENCE_CosNaming_Binding:: freebuf (CosNaming::Binding* data)
{
    if (data) delete [] data;
}

void _IDL_SEQUENCE_CosNaming_Binding:: decodeInOutOp (CORBA::Request &IT_r) {
    CORBA::ULong IT_max;
    CORBA::ULong IT_length;
    IT_r >> IT_max;
    IT_r >> IT_length;
    unsigned char IT_codeSeqElems = IT_max ? 1 : 0;
    if (!_release || (IT_max > _maximum)) {
        if (_buffer && _release) {
            {
            }
            freebuf (_buffer);
        }
        _buffer = allocbuf (IT_max);
        _maximum = IT_max;
        _release = 1;
    }
    if (IT_length > IT_max)
        IT_r.makeRuntimeException4 ();
    else {
        for (CORBA::ULong IT_i = 0; IT_i < IT_length; IT_i++) {
            _buffer[IT_i].decodeInOutOp (IT_r);
        }
    }
    _length = IT_length;
}

void _IDL_SEQUENCE_CosNaming_Binding:: encodeOp (CORBA::Request &IT_r) const {
    IT_r << _maximum;
    IT_r << _length;
    unsigned char IT_codeSeqElems = _maximum ? 1 : 0;
    for (CORBA::ULong IT_i = 0; IT_i < _length; IT_i++) {
        _buffer[IT_i].encodeOp (IT_r);
    }
}

void _IDL_SEQUENCE_CosNaming_Binding:: decodeOp (CORBA::Request &IT_r) {
    IT_r >> _maximum;
    IT_r >> _length;
    unsigned char IT_codeSeqElems = _maximum ? 1 : 0;
    if (IT_codeSeqElems) {
        _buffer = allocbuf (_maximum);
        for (CORBA::ULong IT_i = 0; IT_i < _length; IT_i++) {
            _buffer[IT_i].decodeOp (IT_r);
        }
    }
    else
      _buffer = NULL;
}


CosNaming::Binding& _IDL_SEQUENCE_CosNaming_Binding_var:: operator[] (CORBA::ULong index) {
    return (*_ptr) [index];
}

const CosNaming::Binding& _IDL_SEQUENCE_CosNaming_Binding_var:: operator[] (CORBA::ULong index) const {
    return (*_ptr) [index];
}


#endif


void operator<<= (CORBA::any &IT_a, const CosNaming::NameComponent& IT_t) {
    void *IT_la;
    IT_la = new CosNaming::NameComponent(IT_t);
    IT_a.replace (CosNaming::_tc_NameComponent, IT_la, 1);
}

CORBA::Boolean operator>>= (const CORBA::any &IT_a, CosNaming::NameComponent*& IT_t) {
    if (!(IT_a.containsType(CosNaming::_tc_NameComponent))) {
        ;
        return 0;
    }
    IT_t = ((CosNaming::NameComponent*) IT_a.value());
    return 1;
}


void operator<<= (CORBA::any &IT_a, CosNaming::BindingIterator_ptr IT_t) {
    void *IT_la;
    CORBA::Object_ptr *IT_b = new CORBA::Object_ptr;
    (*IT_b) = CosNaming::BindingIterator_duplicate (IT_t);
    IT_la = IT_b;
    IT_a.replace (CosNaming::_tc_BindingIteratorRef, IT_la, 1);
}

CORBA::Boolean operator>>= (const CORBA::any &IT_a, CosNaming::BindingIterator_ptr& IT_t) {
    if (!(IT_a.containsType(CosNaming::_tc_BindingIteratorRef))) {
        return 0;
    }
    CORBA::Object_ptr IT_la;
    IT_la = *((CORBA::Object_ptr*) IT_a.value());
    IT_t = CosNaming::BindingIterator:: _narrow (IT_la);
    if (CORBA::is_nil(IT_t) && !CORBA::is_nil(IT_la))
        return 0;
    return 1;
}


void operator<<= (CORBA::any &IT_a, CosNaming::NamingContext::NotFoundReason IT_t) {
    void *IT_la;
    IT_la = new CosNaming::NamingContext::NotFoundReason(IT_t);
    IT_a.replace (CosNaming::NamingContext::_tc_NotFoundReason, IT_la, 1);
}

CORBA::Boolean operator>>= (const CORBA::any &IT_a, CosNaming::NamingContext::NotFoundReason& IT_t) {
    if (!(IT_a.containsType(CosNaming::NamingContext::_tc_NotFoundReason))) {
        return 0;
    }
    IT_t = *((CosNaming::NamingContext::NotFoundReason*) IT_a.value());
    return 1;
}


void operator<<= (CORBA::any &IT_a, const CosNaming::_IDL_SEQUENCE_CosNaming_Binding& IT_t) {
    void *IT_la;
    IT_la = new CosNaming::_IDL_SEQUENCE_CosNaming_Binding(IT_t);
    IT_a.replace (CosNaming::_tc__IDL_SEQUENCE_CosNaming_Binding, IT_la, 1);
}

CORBA::Boolean operator>>= (const CORBA::any &IT_a, CosNaming::_IDL_SEQUENCE_CosNaming_Binding*& IT_t) {
    if (!(IT_a.containsType(CosNaming::_tc__IDL_SEQUENCE_CosNaming_Binding))) {
        ;
        return 0;
    }
    IT_t = ((CosNaming::_IDL_SEQUENCE_CosNaming_Binding*) IT_a.value());
    return 1;
}


void operator<<= (CORBA::any &IT_a, const CosNaming::Binding& IT_t) {
    void *IT_la;
    IT_la = new CosNaming::Binding(IT_t);
    IT_a.replace (CosNaming::_tc_Binding, IT_la, 1);
}

CORBA::Boolean operator>>= (const CORBA::any &IT_a, CosNaming::Binding*& IT_t) {
    if (!(IT_a.containsType(CosNaming::_tc_Binding))) {
        ;
        return 0;
    }
    IT_t = ((CosNaming::Binding*) IT_a.value());
    return 1;
}


void operator<<= (CORBA::any &IT_a, CosNaming::NamingContext_ptr IT_t) {
    void *IT_la;
    CORBA::Object_ptr *IT_b = new CORBA::Object_ptr;
    (*IT_b) = CosNaming::NamingContext_duplicate (IT_t);
    IT_la = IT_b;
    IT_a.replace (CosNaming::_tc_NamingContextRef, IT_la, 1);
}

CORBA::Boolean operator>>= (const CORBA::any &IT_a, CosNaming::NamingContext_ptr& IT_t) {
    if (!(IT_a.containsType(CosNaming::_tc_NamingContextRef))) {
        return 0;
    }
    CORBA::Object_ptr IT_la;
    IT_la = *((CORBA::Object_ptr*) IT_a.value());
    IT_t = CosNaming::NamingContext:: _narrow (IT_la);
    if (CORBA::is_nil(IT_t) && !CORBA::is_nil(IT_la))
        return 0;
    return 1;
}


void operator<<= (CORBA::any &IT_a, CosNaming::BindingType IT_t) {
    void *IT_la;
    IT_la = new CosNaming::BindingType(IT_t);
    IT_a.replace (CosNaming::_tc_BindingType, IT_la, 1);
}

CORBA::Boolean operator>>= (const CORBA::any &IT_a, CosNaming::BindingType& IT_t) {
    if (!(IT_a.containsType(CosNaming::_tc_BindingType))) {
        return 0;
    }
    IT_t = *((CosNaming::BindingType*) IT_a.value());
    return 1;
}


void operator<<= (CORBA::any &IT_a, const CosNaming::_IDL_SEQUENCE_CosNaming_NameComponent& IT_t) {
    void *IT_la;
    IT_la = new CosNaming::_IDL_SEQUENCE_CosNaming_NameComponent(IT_t);
    IT_a.replace (CosNaming::_tc__IDL_SEQUENCE_CosNaming_NameComponent, IT_la, 1);
}

CORBA::Boolean operator>>= (const CORBA::any &IT_a, CosNaming::_IDL_SEQUENCE_CosNaming_NameComponent*& IT_t) {
    if (!(IT_a.containsType(CosNaming::_tc__IDL_SEQUENCE_CosNaming_NameComponent))) {
        ;
        return 0;
    }
    IT_t = ((CosNaming::_IDL_SEQUENCE_CosNaming_NameComponent*) IT_a.value());
    return 1;
}


void operator<<= (CORBA::any &IT_a, const _IDL_SEQUENCE_CosNaming_NameComponent& IT_t) {
    void *IT_la;
    IT_la = new _IDL_SEQUENCE_CosNaming_NameComponent(IT_t);
    IT_a.replace (_tc__IDL_SEQUENCE_CosNaming_NameComponent, IT_la, 1);
}

CORBA::Boolean operator>>= (const CORBA::any &IT_a, _IDL_SEQUENCE_CosNaming_NameComponent*& IT_t) {
    if (!(IT_a.containsType(_tc__IDL_SEQUENCE_CosNaming_NameComponent))) {
        ;
        return 0;
    }
    IT_t = ((_IDL_SEQUENCE_CosNaming_NameComponent*) IT_a.value());
    return 1;
}


void operator<<= (CORBA::any &IT_a, const _IDL_SEQUENCE_CosNaming_Binding& IT_t) {
    void *IT_la;
    IT_la = new _IDL_SEQUENCE_CosNaming_Binding(IT_t);
    IT_a.replace (_tc__IDL_SEQUENCE_CosNaming_Binding, IT_la, 1);
}

CORBA::Boolean operator>>= (const CORBA::any &IT_a, _IDL_SEQUENCE_CosNaming_Binding*& IT_t) {
    if (!(IT_a.containsType(_tc__IDL_SEQUENCE_CosNaming_Binding))) {
        ;
        return 0;
    }
    IT_t = ((_IDL_SEQUENCE_CosNaming_Binding*) IT_a.value());
    return 1;
}

