// $Id$

#include "Event_Comm.hh"

ACE_RCSID(libsrc, Event_CommC, "$Id$")

#ifndef Event_Comm_Notification_Ops
#define Event_Comm_Notification_Ops

void Event_Comm::Notification:: encodeOp (CORBA::Request &IT_r) const {
    IT_r.encodeStringOp (tag_);
}

void Event_Comm::Notification:: decodeOp (CORBA::Request &IT_r) {
    IT_r.decodeStringOp(tag_);
}

void Event_Comm::Notification:: decodeInOutOp (CORBA::Request &IT_r) {
    IT_r.decodeInOutStrOp(tag_, 0);
}

void* Event_Comm::Notification:: IT_anySupport (CORBA::Request &IT_r,
		void *& IT_v, void *IT_to, const CORBA::Flags& IT_f) {
    Event_Comm::Notification* IT_l = (Event_Comm::Notification*)IT_v;

    if (IT_f.isSetAll (CORBA::ARG_INOUT)) {
        if (!IT_l)
            IT_l = new Event_Comm::Notification();
        IT_l -> decodeInOutOp (IT_r);
        IT_v = IT_l;
    }
    else if (IT_f.isSet (CORBA::ARG_IN)) {
        IT_l -> encodeOp (IT_r);
    }
    else if (IT_f.isSet (CORBA::ARG_OUT)) {
        if (!IT_l)
            IT_l = new Event_Comm::Notification();
        IT_l -> decodeOp (IT_r);
        IT_v = IT_l;
    }
    else if (IT_f.isSet (CORBA::_ANY_ASSIGN)) {
       Event_Comm::Notification*IT_s = IT_to ? (Event_Comm::Notification*)IT_to : new Event_Comm::Notification;
       *IT_s = *IT_l;  return IT_s;
    }
    else if (IT_f.isSet (CORBA::_ANY_DELETE)) {
       if (IT_to) IT_l->Event_Comm::Notification::~Notification();
       else delete IT_l;
       return NULL;
    }
    else if (IT_f.isSet (CORBA::_ANY_SIZEOF)) {
       return (void*) (sizeof (Event_Comm::Notification));
    }
    else if (IT_f.isNil ()) {
        if (!IT_l)
            IT_l = new Event_Comm::Notification();
        IT_l -> decodeOp (IT_r);
        IT_v = IT_l;
    }
    return NULL;
}

const void *Event_Comm::Notification:: IT_fn =
CORBA::anyTable.record ("Event_Comm::Notification", &Event_Comm::Notification:: IT_anySupport);

Event_Comm::Notification &Event_Comm::Notification:: operator= (const Event_Comm::IONANC_Notification& IT_p) {
    this->operator= (*(Event_Comm::Notification*) &IT_p);
    return (*this);
}

Event_Comm::Notification:: operator Event_Comm::IONANC_Notification () {
    Event_Comm::IONANC_Notification tmp;
    memset (&tmp, 0, sizeof(tmp));
    ((Event_Comm::Notification *) &tmp)->operator= (*this);
    return tmp;
}

Event_Comm::Notification:: operator const Event_Comm::IONANC_Notification () const {
    Event_Comm::IONANC_Notification tmp;
    memset (&tmp, 0, sizeof(tmp));
    ((Event_Comm::Notification *) &tmp)->operator= (*this);
    return tmp;
}

Event_Comm::Notification::~Notification () {
    if (tag_) delete [] tag_;
}

Event_Comm::Notification:: Notification (const Event_Comm::Notification &IT_s) 
	 {
    if (IT_s.tag_) { 
    tag_=new char [strlen(IT_s.tag_)+1];
    strcpy (tag_, IT_s.tag_);
    }
    else {
    tag_ = NULL;
    }
}

Event_Comm::Notification:: Notification () {
    tag_ = NULL;
}

Event_Comm::Notification &Event_Comm::Notification:: operator= (const Event_Comm::Notification& IT_s) {
    if (this == &IT_s) return *this;
    if (tag_) delete [] tag_;
    if (IT_s.tag_) { 
    tag_=new char [strlen(IT_s.tag_)+1];
    strcpy (tag_, IT_s.tag_);
    }
    else {
    tag_ = NULL;
    }
    return *this;
}

Event_Comm::IONANC_Notification:: operator Event_Comm::Notification () {
    return (*((Event_Comm::Notification *) this));
}

Event_Comm::IONANC_Notification:: operator const Event_Comm::Notification () const {
    return (*((const Event_Comm::Notification *) this));
}


#endif
Event_Comm::Notification_Receiver::Notification_Receiver (char *IT_OR) {
      m_pptr = new Notification_Receiver_dispatch (IT_OR, this,(CORBA::Object*)this);
} 

#ifndef Event_Comm_Notification_ReceiverForwC
#define Event_Comm_Notification_ReceiverForwC
CORBA::ObjectRef Event_Comm::Notification_Receiver_getBase(void *IT_p){
    return (Event_Comm::Notification_Receiver*)IT_p;}

void Event_Comm::Notification_Receiver_release (void *IT_p, CORBA::Environment &IT_env) {
    ((Event_Comm::Notification_Receiver*)IT_p)->_release(IT_env);}

Event_Comm::Notification_Receiver* Event_Comm::Notification_Receiver_duplicate (void *IT_p, CORBA::Environment &IT_env) {
    return ((Event_Comm::Notification_Receiver*)IT_p)->_duplicate(IT_env); }
#endif



Event_Comm::Notification_Receiver* Event_Comm::Notification_Receiver:: _bind (const char* IT_markerServer, const char* host,
		const CORBA::Context &IT_c, 
		CORBA::Environment &IT_env) {
       Notification_Receiver*IT_p =
 (Notification_Receiver*)CORBA::Factory.New (IT_markerServer, IT_env, IT_c, host, 
		Event_Comm_Notification_Receiver_IMPL, Event_Comm_Notification_Receiver_IR);
       return IT_p ? IT_p->_duplicate () : NULL; }



Event_Comm::Notification_Receiver* Event_Comm::Notification_Receiver:: _bind (CORBA::Environment &IT_env) {
       return _bind (NULL,NULL,CORBA::Context(), IT_env); }


Event_Comm::Notification_Receiver* Event_Comm::Notification_Receiver:: _bind (const char* IT_markerServer, const char* host,
                CORBA::Environment &IT_env) {
       return _bind (IT_markerServer, host, CORBA::Context (), IT_env); }
Event_Comm::Notification_Receiver* Event_Comm::Notification_Receiver::_narrow (CORBA::Object* IT_obj, CORBA::Environment &IT_env) {
       Event_Comm::Notification_Receiver* IT_p = (Event_Comm::Notification_Receiver*)CORBA::Object::_castDown (IT_obj, Event_Comm_Notification_Receiver_IR, IT_env);
        return IT_p ? IT_p->_duplicate(IT_env) : NULL;
   }

void* Event_Comm::Notification_ReceiverProxyFactoryClass::New (char *IT_OR, CORBA::Environment&) {
        return  new Notification_Receiver(IT_OR);}

void* Event_Comm::Notification_ReceiverProxyFactoryClass::New2 () {
        return  new Notification_Receiver();}

void* Event_Comm::Notification_ReceiverProxyFactoryClass::IT_castUp (void *IT_p, char* IT_s) {
      void *IT_l;
      if (!CORBA::_interfaceCmp (IT_s,Event_Comm_Notification_Receiver_IR))
         return IT_p;
      else if (IT_l=CORBA::ObjectFactoryClass::IT_castUp((CORBA::Object*)((Event_Comm::Notification_Receiver*)IT_p),IT_s))
         return IT_l;
      else return NULL;
    }


CORBA::PPTR* Event_Comm::Notification_ReceiverProxyFactoryClass::pptr (void *IT_p) {
       return ((Event_Comm::Notification_Receiver*)IT_p)->_pptr ();}

void Event_Comm::Notification_ReceiverProxyFactoryClass::baseInterfaces (_IDL_SEQUENCE_string& seq) {
      add (seq, Event_Comm_Notification_Receiver_IR);
      CORBA::ObjectFactoryClass::baseInterfaces (seq);
}

    void Event_Comm::Notification_Receiver:: receive_notification(const Event_Comm::Notification& notification, CORBA::Environment &IT_env) {

        if (IT_env || m_isNull) return ;
        CORBA::Request IT_r (this, "receive_notification",IT_env,1,1);
        if (!IT_r.isException (IT_env)) {
            notification.encodeOp (IT_r);
        }

        IT_r.invoke (CORBA::Flags(CORBA::INV_NO_RESPONSE), IT_env);
    }

    void Event_Comm::Notification_Receiver:: disconnect(const char * reason, CORBA::Environment &IT_env) {

        if (IT_env || m_isNull) return ;
        CORBA::Request IT_r (this, "disconnect",IT_env,1,1);
        if (!IT_r.isException (IT_env)) {
            IT_r.encodeStringOp (reason);
        }

        IT_r.invoke (CORBA::Flags(CORBA::INV_NO_RESPONSE), IT_env);
    }


Event_Comm::Notification_ReceiverProxyFactoryClass Event_Comm::Notification_ReceiverProxyFactory(1);


#ifndef Event_Comm_Notification_Receiver_dispatch_impl

unsigned char Event_Comm::Notification_Receiver_dispatch::dispatch (CORBA::Request &IT_r, 
    unsigned char, void *) {
      IT_r.makeRuntimeException1 ("Event_Comm::Notification_Receiver");
      return 0;
}
 
#endif

Event_Comm::Notifier::Notifier (char *IT_OR) {
      m_pptr = new Notifier_dispatch (IT_OR, this,(CORBA::Object*)this);
} 

#ifndef Event_Comm_NotifierForwC
#define Event_Comm_NotifierForwC
CORBA::ObjectRef Event_Comm::Notifier_getBase(void *IT_p){
    return (Event_Comm::Notifier*)IT_p;}

void Event_Comm::Notifier_release (void *IT_p, CORBA::Environment &IT_env) {
    ((Event_Comm::Notifier*)IT_p)->_release(IT_env);}

Event_Comm::Notifier* Event_Comm::Notifier_duplicate (void *IT_p, CORBA::Environment &IT_env) {
    return ((Event_Comm::Notifier*)IT_p)->_duplicate(IT_env); }
#endif



Event_Comm::Notifier* Event_Comm::Notifier:: _bind (const char* IT_markerServer, const char* host,
		const CORBA::Context &IT_c, 
		CORBA::Environment &IT_env) {
       Notifier*IT_p =
 (Notifier*)CORBA::Factory.New (IT_markerServer, IT_env, IT_c, host, 
		Event_Comm_Notifier_IMPL, Event_Comm_Notifier_IR);
       return IT_p ? IT_p->_duplicate () : NULL; }



Event_Comm::Notifier* Event_Comm::Notifier:: _bind (CORBA::Environment &IT_env) {
       return _bind (NULL,NULL,CORBA::Context(), IT_env); }


Event_Comm::Notifier* Event_Comm::Notifier:: _bind (const char* IT_markerServer, const char* host,
                CORBA::Environment &IT_env) {
       return _bind (IT_markerServer, host, CORBA::Context (), IT_env); }
Event_Comm::Notifier* Event_Comm::Notifier::_narrow (CORBA::Object* IT_obj, CORBA::Environment &IT_env) {
       Event_Comm::Notifier* IT_p = (Event_Comm::Notifier*)CORBA::Object::_castDown (IT_obj, Event_Comm_Notifier_IR, IT_env);
        return IT_p ? IT_p->_duplicate(IT_env) : NULL;
   }

void* Event_Comm::NotifierProxyFactoryClass::New (char *IT_OR, CORBA::Environment&) {
        return  new Notifier(IT_OR);}

void* Event_Comm::NotifierProxyFactoryClass::New2 () {
        return  new Notifier();}

void* Event_Comm::NotifierProxyFactoryClass::IT_castUp (void *IT_p, char* IT_s) {
      void *IT_l;
      if (!CORBA::_interfaceCmp (IT_s,Event_Comm_Notifier_IR))
         return IT_p;
      else if (IT_l=CORBA::ObjectFactoryClass::IT_castUp((CORBA::Object*)((Event_Comm::Notifier*)IT_p),IT_s))
         return IT_l;
      else return NULL;
    }


CORBA::PPTR* Event_Comm::NotifierProxyFactoryClass::pptr (void *IT_p) {
       return ((Event_Comm::Notifier*)IT_p)->_pptr ();}

void Event_Comm::NotifierProxyFactoryClass::baseInterfaces (_IDL_SEQUENCE_string& seq) {
      add (seq, Event_Comm_Notifier_IR);
      CORBA::ObjectFactoryClass::baseInterfaces (seq);
}

    void Event_Comm::Notifier:: send_disconnect(const char * reason, CORBA::Environment &IT_env) {

        if (IT_env || m_isNull) return ;
        CORBA::Request IT_r (this, "send_disconnect",IT_env,1,1);
        if (!IT_r.isException (IT_env)) {
            IT_r.encodeStringOp (reason);
        }

        IT_r.invoke (CORBA::Flags(CORBA::INV_NO_RESPONSE), IT_env);
    }

    void Event_Comm::Notifier:: send_notification(const Event_Comm::Notification& notification, CORBA::Environment &IT_env) {

        if (IT_env || m_isNull) return ;
        CORBA::Request IT_r (this, "send_notification",IT_env,1,1);
        if (!IT_r.isException (IT_env)) {
            notification.encodeOp (IT_r);
        }

        IT_r.invoke (CORBA::Flags(CORBA::INV_NO_RESPONSE), IT_env);
    }

    void Event_Comm::Notifier:: subscribe(Event_Comm::Notification_Receiver* notification_receiver, const char * filtering_criteria, CORBA::Environment &IT_env) {

        if (IT_env || m_isNull) return ;
        CORBA::Request IT_r (this, "subscribe",IT_env,1,1);
        if (!IT_r.isException (IT_env)) {
            IT_r << (CORBA::Object*)notification_receiver;

            IT_r.encodeStringOp (filtering_criteria);
        }

        IT_r.invoke (CORBA::Flags(CORBA::INV_NO_RESPONSE), IT_env);
    }

    void Event_Comm::Notifier:: unsubscribe(Event_Comm::Notification_Receiver* notification_receiver, const char * filtering_criteria, CORBA::Environment &IT_env) {

        if (IT_env || m_isNull) return ;
        CORBA::Request IT_r (this, "unsubscribe",IT_env,1,1);
        if (!IT_r.isException (IT_env)) {
            IT_r << (CORBA::Object*)notification_receiver;

            IT_r.encodeStringOp (filtering_criteria);
        }

        IT_r.invoke (CORBA::Flags(CORBA::INV_NO_RESPONSE), IT_env);
    }


Event_Comm::NotifierProxyFactoryClass Event_Comm::NotifierProxyFactory(1);


#ifndef Event_Comm_Notifier_dispatch_impl

unsigned char Event_Comm::Notifier_dispatch::dispatch (CORBA::Request &IT_r, 
    unsigned char, void *) {
      IT_r.makeRuntimeException1 ("Event_Comm::Notifier");
      return 0;
}
 
#endif

