// $Id$

#include "cubit.h"

ACE_RCSID(client, cubitC, "$Id$")

Cubit::Cubit (char *IT_OR) {
      m_pptr = new Cubit_dispatch (IT_OR, this,(CORBA::Object*)this);
} 
Cubit::Cubit (ObjectReference *IT_OR) {
      m_pptr = new Cubit_dispatch (IT_OR, this,(CORBA::Object*)this);
} 

#ifndef CubitForwC
#define CubitForwC
CORBA::ObjectRef Cubit_getBase(void *IT_p){
    return (Cubit*) IT_p;}

void Cubit_release (Cubit_ptr IT_p, CORBA::Environment &IT_env) {
    CORBA::release(IT_p, IT_env);}

void Cubit_release (Cubit_ptr IT_p) {
    Cubit_release (IT_p, CORBA::IT_chooseDefaultEnv ()); }

Cubit_ptr Cubit_nil (CORBA::Environment &) {
    return Cubit:: _nil ();}

Cubit_ptr Cubit_nil () {
    return Cubit_nil (CORBA::IT_chooseDefaultEnv ());}

Cubit_ptr Cubit_duplicate (Cubit_ptr IT_p, CORBA::Environment &IT_env) {
    return (Cubit::_duplicate(IT_p, IT_env)); }
Cubit_ptr Cubit_duplicate (Cubit_ptr IT_p) {
    return Cubit_duplicate (IT_p, CORBA::IT_chooseDefaultEnv ()); }
#endif

Cubit_ptr Cubit::_duplicate(Cubit_ptr obj, CORBA::Environment& IT_pEnv) {
        CORBA::EnvExcRaiser IT_raise;
        CORBA::Environment &IT_env = IT_raise.registerEnv (&IT_pEnv);
       if (!obj) {
          return (obj);
       }
       Cubit_ptr IT_obj = obj->__duplicate (IT_env);
       return IT_obj;
}



Cubit* Cubit:: _bind (const char* IT_markerServer, const char* host,
		const CORBA::Context &IT_c, 
		CORBA::Environment &IT_env) {
       return (Cubit*)CORBA::Factory.New (IT_markerServer, IT_env, IT_c, host, 
		Cubit_IMPL, Cubit_IR);
}



Cubit* Cubit:: _bind (CORBA::Environment &IT_env) {
       return _bind (NULL,NULL,CORBA::Context(), IT_env); }


Cubit* Cubit:: _bind (const char* IT_markerServer, const char* host,
                CORBA::Environment &IT_env) {
       return _bind (IT_markerServer, host, CORBA::Context (), IT_env); }
Cubit* Cubit::_narrow (CORBA::Object* IT_obj, CORBA::Environment &IT_pEnv) {
        CORBA::EnvExcRaiser IT_raise;
        CORBA::Environment &IT_env = IT_raise.registerEnv (&IT_pEnv);
        if (CORBA::is_nil (IT_obj)) {
                return Cubit:: _nil();
        }

       Cubit* IT_p = (Cubit*)CORBA::Object::_castDown (IT_obj, Cubit_IR, IT_env);
        IT_p = IT_p ? IT_p->__duplicate(IT_env) : NULL;
        if ((IT_env) &&(!strcmp(IT_env.exception()->id(), "CORBA::StExcep::BAD_PARAM")) && (((CORBA::BAD_PARAM*)IT_env.exception())-> minor() == NARROW_FAILED)) {
                IT_env.exception_free();
        }
        return IT_p;
   }


#ifndef Cubit_Many_Ops
#define Cubit_Many_Ops

void Cubit::Many:: encodeOp (CORBA::Request &IT_r) const {

    IT_r.insertOctet (o);

    IT_r << l;

    IT_r << s;
}

void Cubit::Many:: decodeOp (CORBA::Request &IT_r) {

    IT_r.extractOctet (o);

    IT_r >> l;

    IT_r >> s;
}

void Cubit::Many:: decodeInOutOp (CORBA::Request &IT_r) {

    IT_r.extractOctet (o);

    IT_r >> l;

    IT_r >> s;
}

Cubit::Many ::~Many () {
}

Cubit::Many:: Many (const Cubit::Many &IT_s) 
	 {
    o = IT_s.o;
    l = IT_s.l;
    s = IT_s.s;
}

Cubit::Many:: Many () {
}

Cubit::Many &Cubit::Many:: operator= (const Cubit::Many& IT_s) {
    if (this == &IT_s) return *this;
    o = IT_s.o;
    l = IT_s.l;
    s = IT_s.s;
    return *this;
}


#endif


#ifndef Cubit_oneof_Ops
#define Cubit_oneof_Ops
void Cubit::oneof:: encodeOp (CORBA::Request &IT_r) const {
    if (!isSet) return;
    {
    CORBA::ULong IT_0 = (CORBA::ULong) __d;
    IT_r << IT_0;
    }
    switch (__d) {
        case Cubit::e_0th: 
        IT_r.insertOctet (_o_);
        break;
        case Cubit::e_1st: 
        IT_r << _s_;
        break;
        case Cubit::e_2nd: 
        IT_r << _l_;
        break;
        case Cubit::e_3rd: 
        default: 
        (*_cm_).encodeOp (IT_r);
        break;
    }

}

void Cubit::oneof:: decodeOp (CORBA::Request &IT_r) {
    isSet = 0;
    {
    CORBA::ULong IT_1;
    IT_r >> IT_1;
    __d = Cubit::discrim(IT_1);
    }
    switch (__d) {
        case Cubit::e_0th: 
        if (!isSet) {
            isSet = 1;
        }
        IT_r.extractOctet (_o_);
        break;
        case Cubit::e_1st: 
        if (!isSet) {
            isSet = 1;
        }
        IT_r >> _s_;
        break;
        case Cubit::e_2nd: 
        if (!isSet) {
            isSet = 1;
        }
        IT_r >> _l_;
        break;
        case Cubit::e_3rd: 
        default: 
        if (!isSet) {
            _cm_ = new Cubit::Many;
            isSet = 1;
        }
        (*_cm_).decodeOp (IT_r);
        break;
    }

}
void Cubit::oneof:: decodeInOutOp (CORBA::Request &IT_r) {
    Cubit::discrim IT_d;
    {
    CORBA::ULong IT_2;
    IT_r >> IT_2;
    IT_d = Cubit::discrim(IT_2);
    }
    if (IT_d == __d)
        switch (__d) {
            case Cubit::e_0th: 
            if (!isSet) {
                isSet = 1;
            }
            IT_r.extractOctet (_o_);
            break;
            case Cubit::e_1st: 
            if (!isSet) {
                isSet = 1;
            }
            IT_r >> _s_;
            break;
            case Cubit::e_2nd: 
            if (!isSet) {
                isSet = 1;
            }
            IT_r >> _l_;
            break;
            case Cubit::e_3rd: 
            default: 
            if (!isSet) {
                _cm_ = new Cubit::Many;
                isSet = 1;
            }
            (*_cm_).decodeInOutOp (IT_r);
            break;
        }

    else {
        this->oneof:: ~oneof();
        memset(this, 0, sizeof(*this));
        __d = IT_d;
        switch (__d) {
            case Cubit::e_0th: 
            if (!isSet) {
                isSet = 1;
            }
            IT_r.extractOctet (_o_);
            break;
            case Cubit::e_1st: 
            if (!isSet) {
                isSet = 1;
            }
            IT_r >> _s_;
            break;
            case Cubit::e_2nd: 
            if (!isSet) {
                isSet = 1;
            }
            IT_r >> _l_;
            break;
            case Cubit::e_3rd: 
            default: 
            if (!isSet) {
                _cm_ = new Cubit::Many;
                isSet = 1;
            }
            (*_cm_).decodeOp (IT_r);
            break;
        }
        isSet = 1;
    }

}

Cubit::oneof:: oneof() { 
    memset(this, 0, sizeof(*this));
    isSet = 0;
}

Cubit::oneof ::~oneof() {
    if (!isSet) return;
    switch (__d) { 
        case Cubit::e_0th:         break;
        case Cubit::e_1st:         break;
        case Cubit::e_2nd:         break;
        case Cubit::e_3rd: 
        default:         if (_cm_) delete _cm_;
        break;
    }

    isSet = 0;
}

Cubit::oneof:: oneof (const Cubit::oneof &IT_s) {
    isSet = 0;
    memset(this, 0, sizeof(*this));
    __d = IT_s.__d;
    switch (__d) {
        case Cubit::e_0th:         isSet = 1;
        _o_ = IT_s._o_;
        break;
        case Cubit::e_1st:         isSet = 1;
        _s_ = IT_s._s_;
        break;
        case Cubit::e_2nd:         isSet = 1;
        _l_ = IT_s._l_;
        break;
        case Cubit::e_3rd: 
        default: 
        if (!isSet) {
            _cm_ = new Cubit::Many;
            isSet = 1;
        }
        *(_cm_) = *(IT_s._cm_);
        break;
    }
    isSet = 1;
}

Cubit::oneof& Cubit::oneof:: operator= (const Cubit::oneof & IT_s) {
    if (this == &IT_s) return *this;
    if (isSet && (__d != IT_s.__d))
    {
        this-> oneof::~oneof();
        memset(this, 0, sizeof(*this));
    }

    __d = IT_s.__d;
    switch (__d) {
        case Cubit::e_0th:         isSet = 1;
        _o_ = IT_s._o_;
        break;
        case Cubit::e_1st:         isSet = 1;
        _s_ = IT_s._s_;
        break;
        case Cubit::e_2nd:         isSet = 1;
        _l_ = IT_s._l_;
        break;
        case Cubit::e_3rd: 
        default: 
        if (!isSet) {
            _cm_ = new Cubit::Many;
            isSet = 1;
        }
        *(_cm_) = *(IT_s._cm_);
        break;
    }
    isSet = 1;
    return *this;
}


#endif

void* CubitProxyFactoryClass::New (char *IT_OR, CORBA::Environment&) {
        return  new class Cubit(IT_OR);}

void* CubitProxyFactoryClass::New (ObjectReference *IT_OR, CORBA::Environment&) {
        return  new class Cubit(IT_OR);}

void* CubitProxyFactoryClass::New2 () {
        return  new class Cubit((char*)0);}

CORBA::Object* CubitProxyFactoryClass::NewObject (char *IT_OR, CORBA::Environment&) {
        return  new class Cubit(IT_OR);}

CORBA::Object* CubitProxyFactoryClass::NewObject (ObjectReference *IT_OR, CORBA::Environment&) {
        return  new class Cubit(IT_OR);}

CORBA::Object* CubitProxyFactoryClass::New2Object () {
        return  new class Cubit((char*)0);}

void* CubitProxyFactoryClass::IT_castUp (void *IT_p, char* IT_s, CORBA::Environment &IT_env) {
      void *IT_l;
      if (!CORBA::_interfaceCmp (IT_s,Cubit_IR, IT_env))
         return IT_p;
      else if (IT_l=CORBA::ObjectFactoryClass::IT_castUp((CORBA::Object*)((class Cubit*)IT_p),IT_s, IT_env))
         return IT_l;
      else return NULL;
    }


CORBA::PPTR* CubitProxyFactoryClass::pptr (void *IT_p) {
       return ((class Cubit*)IT_p)->_pptr ();}

void CubitProxyFactoryClass::baseInterfaces (_IDL_SEQUENCE_string& seq) {
      add (seq, Cubit_IR);
      CORBA::ObjectFactoryClass::baseInterfaces (seq);
}

CubitProxyFactoryClass CubitProxyFactory(1);

CORBA::Octet Cubit:: cube_octet (CORBA::Octet o, CORBA::Environment &IT_pEnv) throw (CORBA::SystemException) {

    CORBA::EnvExcRaiser IT_raise;
    CORBA::Environment &IT_env = IT_raise.registerEnv (&IT_pEnv);
    if (IT_env || m_isNull) {
        return 0;
    }
    CORBA::Request IT_r (this, "cube_octet",IT_env,1,0);
    if (!IT_r.isException (IT_env)) {

        IT_r.insertOctet (o);
    }

    IT_r.invoke (CORBA::Flags(0),IT_env);
    if (!IT_r.isException (IT_env)) {
        CORBA::Octet IT_result;
        IT_r.extractOctet (IT_result);
        IT_r.checkEnv (IT_env);
        return IT_result;
    }
    if (IT_r.exceptionKind () == CORBA::SYSTEM_EXCEPTION) {
        IT_r.checkEnv (IT_env);
    }
    return 0;
}

CORBA::Short Cubit:: cube_short (CORBA::Short s, CORBA::Environment &IT_pEnv) throw (CORBA::SystemException) {

    CORBA::EnvExcRaiser IT_raise;
    CORBA::Environment &IT_env = IT_raise.registerEnv (&IT_pEnv);
    if (IT_env || m_isNull) {
        return 0;
    }
    CORBA::Request IT_r (this, "cube_short",IT_env,1,0);
    if (!IT_r.isException (IT_env)) {

        IT_r << s;
    }

    IT_r.invoke (CORBA::Flags(0),IT_env);
    if (!IT_r.isException (IT_env)) {
        CORBA::Short IT_result;
        IT_r >> IT_result;
        IT_r.checkEnv (IT_env);
        return IT_result;
    }
    if (IT_r.exceptionKind () == CORBA::SYSTEM_EXCEPTION) {
        IT_r.checkEnv (IT_env);
    }
    return 0;
}

CORBA::Long Cubit:: cube_long (CORBA::Long l, CORBA::Environment &IT_pEnv) throw (CORBA::SystemException) {

    CORBA::EnvExcRaiser IT_raise;
    CORBA::Environment &IT_env = IT_raise.registerEnv (&IT_pEnv);
    if (IT_env || m_isNull) {
        return 0;
    }
    CORBA::Request IT_r (this, "cube_long",IT_env,1,0);
    if (!IT_r.isException (IT_env)) {

        IT_r << l;
    }

    IT_r.invoke (CORBA::Flags(0),IT_env);
    if (!IT_r.isException (IT_env)) {
        CORBA::Long IT_result;
        IT_r >> IT_result;
        IT_r.checkEnv (IT_env);
        return IT_result;
    }
    if (IT_r.exceptionKind () == CORBA::SYSTEM_EXCEPTION) {
        IT_r.checkEnv (IT_env);
    }
    return 0;
}

Cubit::Many Cubit:: cube_struct (const Cubit::Many& values, CORBA::Environment &IT_pEnv) throw (CORBA::SystemException) {

    CORBA::EnvExcRaiser IT_raise;
    CORBA::Environment &IT_env = IT_raise.registerEnv (&IT_pEnv);
    if (IT_env || m_isNull) {
        { 
            Cubit::Many IT_l0;
            return IT_l0;
        }
    }
    CORBA::Request IT_r (this, "cube_struct",IT_env,1,0);
    if (!IT_r.isException (IT_env)) {

        values.encodeOp (IT_r);
    }

    IT_r.invoke (CORBA::Flags(0),IT_env);
    if (!IT_r.isException (IT_env)) {
        Cubit::Many IT_result;
        IT_result.decodeOp (IT_r);
        IT_r.checkEnv (IT_env);
        return IT_result;
    }
    if (IT_r.exceptionKind () == CORBA::SYSTEM_EXCEPTION) {
        IT_r.checkEnv (IT_env);
    }
    { 
        Cubit::Many IT_l1;
        return IT_l1;
    }
}

Cubit::oneof Cubit:: cube_union (const Cubit::oneof& values, CORBA::Environment &IT_pEnv) throw (CORBA::SystemException) {

    CORBA::EnvExcRaiser IT_raise;
    CORBA::Environment &IT_env = IT_raise.registerEnv (&IT_pEnv);
    if (IT_env || m_isNull) {
        { 
            Cubit::oneof IT_l0;
            return IT_l0;
        }
    }
    CORBA::Request IT_r (this, "cube_union",IT_env,1,0);
    if (!IT_r.isException (IT_env)) {

        values.encodeOp (IT_r);
    }

    IT_r.invoke (CORBA::Flags(0),IT_env);
    if (!IT_r.isException (IT_env)) {
        Cubit::oneof IT_result;
        IT_result.decodeOp (IT_r);
        IT_r.checkEnv (IT_env);
        return IT_result;
    }
    if (IT_r.exceptionKind () == CORBA::SYSTEM_EXCEPTION) {
        IT_r.checkEnv (IT_env);
    }
    { 
        Cubit::oneof IT_l1;
        return IT_l1;
    }
}

void Cubit:: please_exit (CORBA::Environment &IT_pEnv) throw (CORBA::SystemException) {

    CORBA::EnvExcRaiser IT_raise;
    CORBA::Environment &IT_env = IT_raise.registerEnv (&IT_pEnv);
    if (IT_env || m_isNull) {
        return ;
    }
    CORBA::Request IT_r (this, "please_exit",IT_env,1,1);

    IT_r.invoke (CORBA::Flags(CORBA::INV_NO_RESPONSE), IT_env);
    if (IT_r.exceptionKind () == CORBA::SYSTEM_EXCEPTION) {
        IT_r.checkEnv (IT_env);
    }
}





#ifndef Cubit_dispatch_impl

unsigned char Cubit_dispatch::dispatch (CORBA::Request &IT_r, 
    unsigned char, void *) {
      IT_r.makeRuntimeException1 ("Cubit");
      return 0;
}
 
#endif

