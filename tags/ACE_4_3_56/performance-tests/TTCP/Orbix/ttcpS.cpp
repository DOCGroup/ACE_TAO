
// $Id$

#include "ttcp.hh"


#define ttcp_sequence_dispatch_impl

unsigned char ttcp_sequence_dispatch::dispatch (CORBA::Request &IT_r, 
 unsigned char IT_isTarget, void *IT_pp) {
    if (!IT_pp)
       IT_pp = m_obj;
    const char *IT_s = IT_r.getOperation ();
    if (!strcmp(IT_s,"send")) {
        long IT_result;
        CORBA::Environment IT_env (IT_r);
        CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
        if (!IT_r.tcAssert ("\
Ro~send~+ttcp_seq{S{c},0},>{l},N{}\
"))
            return 1;
        ttcp_sequence::my_sequence ttcp_seq;
        ttcp_seq.decodeOp (IT_r);

        if (IT_f && !IT_r.isException (IT_env))
        	IT_f->inRequestPostM (IT_r, IT_env);
        if (!IT_r.isException (IT_env))
        	IT_result = ((ttcp_sequence*)IT_pp)->send ( ttcp_seq, IT_env);


        if (!IT_r.isException (IT_env)) {
            if (!IT_r.convertToReply ("\
l\
", IT_env)) return 1;
            IT_r << IT_result;
            }

        else IT_r.makeSystemException (IT_env);
        return 1;
    }

    else if (!strcmp(IT_s,"start_timer")) {
        CORBA::Environment IT_env (IT_r);
        CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
        if (!IT_r.tcAssert ("\
Ro~start_timer~>{v},O{}\
"))
            return 1;

        if (IT_f && !IT_r.isException (IT_env))
        	IT_f->inRequestPostM (IT_r, IT_env);
        if (!IT_r.isException (IT_env))
        ((ttcp_sequence*)IT_pp)->start_timer (IT_env);

        IT_r.replyNoResults (CORBA::Flags(CORBA::INV_NO_RESPONSE),IT_env);
        return 1;
    }

    else if (!strcmp(IT_s,"stop_timer")) {
        CORBA::Environment IT_env (IT_r);
        CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
        if (!IT_r.tcAssert ("\
Ro~stop_timer~>{v},O{}\
"))
            return 1;

        if (IT_f && !IT_r.isException (IT_env))
        	IT_f->inRequestPostM (IT_r, IT_env);
        if (!IT_r.isException (IT_env))
        ((ttcp_sequence*)IT_pp)->stop_timer (IT_env);

        IT_r.replyNoResults (CORBA::Flags(CORBA::INV_NO_RESPONSE),IT_env);
        return 1;
    }

    else if (IT_isTarget)
        IT_r.makeRuntimeException2 ();

    return 0;
}

#define ttcp_string_dispatch_impl

unsigned char ttcp_string_dispatch::dispatch (CORBA::Request &IT_r, 
 unsigned char IT_isTarget, void *IT_pp) {
    if (!IT_pp)
       IT_pp = m_obj;
    const char *IT_s = IT_r.getOperation ();
    if (!strcmp(IT_s,"send")) {
        long IT_result;
        CORBA::Environment IT_env (IT_r);
        CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
        if (!IT_r.tcAssert ("\
Ro~send~+ttcp_str{0},>{l},N{}\
"))
            return 1;
        char * ttcp_str;
        IT_r.decodeStringOp(ttcp_str);

        if (IT_f && !IT_r.isException (IT_env))
        	IT_f->inRequestPostM (IT_r, IT_env);
        if (!IT_r.isException (IT_env))
        	IT_result = ((ttcp_string*)IT_pp)->send ( ttcp_str, IT_env);

        delete [] ttcp_str;

        if (!IT_r.isException (IT_env)) {
            if (!IT_r.convertToReply ("\
l\
", IT_env)) return 1;
            IT_r << IT_result;
            }

        else IT_r.makeSystemException (IT_env);
        return 1;
    }

    else if (!strcmp(IT_s,"start_timer")) {
        CORBA::Environment IT_env (IT_r);
        CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
        if (!IT_r.tcAssert ("\
Ro~start_timer~>{v},O{}\
"))
            return 1;

        if (IT_f && !IT_r.isException (IT_env))
        	IT_f->inRequestPostM (IT_r, IT_env);
        if (!IT_r.isException (IT_env))
        ((ttcp_string*)IT_pp)->start_timer (IT_env);

        IT_r.replyNoResults (CORBA::Flags(CORBA::INV_NO_RESPONSE),IT_env);
        return 1;
    }

    else if (!strcmp(IT_s,"stop_timer")) {
        CORBA::Environment IT_env (IT_r);
        CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
        if (!IT_r.tcAssert ("\
Ro~stop_timer~>{v},O{}\
"))
            return 1;

        if (IT_f && !IT_r.isException (IT_env))
        	IT_f->inRequestPostM (IT_r, IT_env);
        if (!IT_r.isException (IT_env))
        ((ttcp_string*)IT_pp)->stop_timer (IT_env);

        IT_r.replyNoResults (CORBA::Flags(CORBA::INV_NO_RESPONSE),IT_env);
        return 1;
    }

    else if (IT_isTarget)
        IT_r.makeRuntimeException2 ();

    return 0;
}

#include "ttcpC.cpp"

