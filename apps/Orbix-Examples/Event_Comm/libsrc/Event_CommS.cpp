// $Id$

#include "Event_Comm.hh"

ACE_RCSID(libsrc, Event_CommS, "$Id$")

#define Event_Comm_Notification_Receiver_dispatch_impl

unsigned char Event_Comm::Notification_Receiver_dispatch::dispatch (CORBA::Request &IT_r, 
 unsigned char IT_isTarget, void *IT_pp) {
    if (!IT_pp)
       IT_pp = m_obj;
    const char *IT_s = IT_r.getOperation ();
    if (!strcmp(IT_s,"receive_notification")) {
            CORBA::Environment IT_env (IT_r);
            CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
            if (!IT_r.tcAssert ("\
Ro~receive_notification~+notification{R~Event_Comm::Notification~tag_{0}},>{v},O{}\
"))
                return 1;
            Event_Comm::Notification notification;
            notification.decodeOp (IT_r);

            if (IT_f && !IT_r.isException (IT_env))
            	IT_f->inRequestPostM (IT_r, IT_env);
            if (!IT_r.isException (IT_env))
            ((Event_Comm::Notification_Receiver*)IT_pp)->receive_notification ( notification, IT_env);

            IT_r.replyNoResults (CORBA::Flags(CORBA::INV_NO_RESPONSE),IT_env);
            return 1;
    }

    else if (!strcmp(IT_s,"disconnect")) {
            CORBA::Environment IT_env (IT_r);
            CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
            if (!IT_r.tcAssert ("\
Ro~disconnect~+reason{0},>{v},O{}\
"))
                return 1;
            char * reason;
            IT_r.decodeStringOp(reason);

            if (IT_f && !IT_r.isException (IT_env))
            	IT_f->inRequestPostM (IT_r, IT_env);
            if (!IT_r.isException (IT_env))
            ((Event_Comm::Notification_Receiver*)IT_pp)->disconnect ( reason, IT_env);

            delete [] reason;
            IT_r.replyNoResults (CORBA::Flags(CORBA::INV_NO_RESPONSE),IT_env);
            return 1;
    }

    else if (IT_isTarget)
            IT_r.makeRuntimeException2 ();

    return 0;
}

#define Event_Comm_Notifier_dispatch_impl

unsigned char Event_Comm::Notifier_dispatch::dispatch (CORBA::Request &IT_r, 
 unsigned char IT_isTarget, void *IT_pp) {
    if (!IT_pp)
       IT_pp = m_obj;
    const char *IT_s = IT_r.getOperation ();
    if (!strcmp(IT_s,"send_disconnect")) {
            CORBA::Environment IT_env (IT_r);
            CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
            if (!IT_r.tcAssert ("\
Ro~send_disconnect~+reason{0},>{v},O{}\
"))
                return 1;
            char * reason;
            IT_r.decodeStringOp(reason);

            if (IT_f && !IT_r.isException (IT_env))
            	IT_f->inRequestPostM (IT_r, IT_env);
            if (!IT_r.isException (IT_env))
            ((Event_Comm::Notifier*)IT_pp)->send_disconnect ( reason, IT_env);

            delete [] reason;
            IT_r.replyNoResults (CORBA::Flags(CORBA::INV_NO_RESPONSE),IT_env);
            return 1;
    }

    else if (!strcmp(IT_s,"send_notification")) {
            CORBA::Environment IT_env (IT_r);
            CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
            if (!IT_r.tcAssert ("\
Ro~send_notification~+notification{R~Event_Comm::Notification~tag_{0}},>{v},O{}\
"))
                return 1;
            Event_Comm::Notification notification;
            notification.decodeOp (IT_r);

            if (IT_f && !IT_r.isException (IT_env))
            	IT_f->inRequestPostM (IT_r, IT_env);
            if (!IT_r.isException (IT_env))
            ((Event_Comm::Notifier*)IT_pp)->send_notification ( notification, IT_env);

            IT_r.replyNoResults (CORBA::Flags(CORBA::INV_NO_RESPONSE),IT_env);
            return 1;
    }

    else if (!strcmp(IT_s,"subscribe")) {
            CORBA::Environment IT_env (IT_r);
            CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
            if (!IT_r.tcAssert ("\
Ro~subscribe~+notification_receiver{O~Event_Comm::Notification_Receiver},+filtering_criteria{0},>{v},O{}\
"))
                return 1;
            Event_Comm::Notification_Receiver* notification_receiver;
            notification_receiver = (Event_Comm::Notification_Receiver*) IT_r.decodeObjRef (Event_Comm_Notification_Receiver_IR);
            if (notification_receiver) notification_receiver->_duplicate ();

            char * filtering_criteria;
            IT_r.decodeStringOp(filtering_criteria);

            if (IT_f && !IT_r.isException (IT_env))
            	IT_f->inRequestPostM (IT_r, IT_env);
            if (!IT_r.isException (IT_env))
            ((Event_Comm::Notifier*)IT_pp)->subscribe ( notification_receiver,  filtering_criteria, IT_env);

            if (notification_receiver) notification_receiver->_release ();

            delete [] filtering_criteria;
            IT_r.replyNoResults (CORBA::Flags(CORBA::INV_NO_RESPONSE),IT_env);

            return 1;
    }

    else if (!strcmp(IT_s,"unsubscribe")) {
            CORBA::Environment IT_env (IT_r);
            CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
            if (!IT_r.tcAssert ("\
Ro~unsubscribe~+notification_receiver{O~Event_Comm::Notification_Receiver},+filtering_criteria{0},>{v},O{}\
"))
                return 1;
            Event_Comm::Notification_Receiver* notification_receiver;
            notification_receiver = (Event_Comm::Notification_Receiver*) IT_r.decodeObjRef (Event_Comm_Notification_Receiver_IR);
            if (notification_receiver) notification_receiver->_duplicate ();

            char * filtering_criteria;
            IT_r.decodeStringOp(filtering_criteria);

            if (IT_f && !IT_r.isException (IT_env))
            	IT_f->inRequestPostM (IT_r, IT_env);
            if (!IT_r.isException (IT_env))
            ((Event_Comm::Notifier*)IT_pp)->unsubscribe ( notification_receiver,  filtering_criteria, IT_env);

            if (notification_receiver) notification_receiver->_release ();

            delete [] filtering_criteria;
            IT_r.replyNoResults (CORBA::Flags(CORBA::INV_NO_RESPONSE),IT_env);

            return 1;
    }

    else if (IT_isTarget)
            IT_r.makeRuntimeException2 ();

    return 0;
}

#include "Event_CommC.cpp"

