
// @(#)loggerS.cpp	1.1	10/18/96

#include "logger.hh"


#define logger_dispatch_impl

unsigned char logger_dispatch::dispatch (CORBA::Request &IT_r, 
 unsigned char IT_isTarget, void *IT_pp) {
    if (!IT_pp)
       IT_pp = m_obj;
    const char *IT_s = IT_r.getOperation ();
    if (!strcmp(IT_s,"log")) {
        CORBA::Environment IT_env (IT_r);
        CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
        if (!IT_r.tcAssert ("\
Ro~log~+log_rec{R~logger::Log_Record~type{E~logger::Log_Priority~LM_MESSAGE,LM_DEBUG,LM_WARNING,LM_ERROR,LM_EMERG},time{l},app_id{l},host_addr{l},msg_data{S{c},0}},>{v},O{}\
"))
            return 1;
        logger::Log_Record log_rec;
        log_rec.decodeOp (IT_r);

        if (IT_f && !IT_r.isException (IT_env))
        	IT_f->inRequestPostM (IT_r, IT_env);
        if (!IT_r.isException (IT_env))
        ((logger*)IT_pp)->log ( log_rec, IT_env);

        IT_r.replyNoResults (CORBA::Flags(CORBA::INV_NO_RESPONSE),IT_env);
        return 1;
    }

    else if (!strcmp (IT_s,"_get_verbose")) {
        char verbose;
        CORBA::Environment IT_env (IT_r);
        CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
        if (!IT_r.tcAssert ("\
Ro~_get_verbose~>{c},N{}\
"))
            return 1;
        if (IT_f)
        	IT_f->inRequestPostM (IT_r, IT_env);
        if (!IT_r.isException (IT_env))
            verbose = ((logger*)IT_pp)->verbose(IT_env);

        if (!IT_r.isException (IT_env)) {
            if (!IT_r.convertToReply ("\
c\
", IT_env)) return 1;
            IT_r << verbose;
        }
        else IT_r.makeSystemException (IT_env);

        return 1;
  }
    else if (!strcmp (IT_s,"_set_verbose")) {
        CORBA::Environment IT_env (IT_r);
        CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
        if (IT_r.tcAssert ("\
Ro~_set_verbose~+{c},>{v},N{}\
")) {
            char verbose;
            IT_r >> verbose;
            if (IT_f && !IT_r.isException (IT_env))
            	IT_f->inRequestPostM (IT_r, IT_env);
            if (!IT_r.isException (IT_env))
            	((logger*)IT_pp)->verbose(verbose, IT_env);
        }
        IT_r.replyNoResults (IT_env);
        return 1;
    }

    else if (IT_isTarget)
        IT_r.makeRuntimeException2 ();

    return 0;
}

#define profile_logger_dispatch_impl

unsigned char profile_logger_dispatch::dispatch (CORBA::Request &IT_r, 
 unsigned char IT_isTarget, void *IT_pp) {
    if (!IT_pp)
       IT_pp = m_obj;
    const char *IT_s = IT_r.getOperation ();
    if (!strcmp(IT_s,"start_timer")) {
        CORBA::Environment IT_env (IT_r);
        CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
        if (!IT_r.tcAssert ("\
Ro~start_timer~>{v},N{}\
"))
            return 1;

        if (IT_f && !IT_r.isException (IT_env))
        	IT_f->inRequestPostM (IT_r, IT_env);
        if (!IT_r.isException (IT_env))
        ((profile_logger*)IT_pp)->start_timer (IT_env);

        IT_r.replyNoResults (IT_env);
        return 1;
    }

    else if (!strcmp(IT_s,"stop_timer")) {
        CORBA::Environment IT_env (IT_r);
        CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
        if (!IT_r.tcAssert ("\
Ro~stop_timer~-et{R~profile_logger::Elapsed_Time~real_time{d},user_time{d},system_time{d}},>{v},N{}\
"))
            return 1;
        profile_logger::Elapsed_Time et;

        if (IT_f && !IT_r.isException (IT_env))
        	IT_f->inRequestPostM (IT_r, IT_env);
        if (!IT_r.isException (IT_env))
        ((profile_logger*)IT_pp)->stop_timer ( et, IT_env);


        if (!IT_r.isException (IT_env)) {
            if (!IT_r.convertToReply ("\
v\
", IT_env)) return 1;
            et.encodeOp (IT_r);
            }

        else IT_r.makeSystemException (IT_env);
        return 1;
    }

    else if (logger_dispatch::dispatch (IT_r, 0,
         (logger*)((profile_logger*)IT_pp))) {
   return 1;
    }

    else if (IT_isTarget)
        IT_r.makeRuntimeException2 ();

    return 0;
}

#include "loggerC.cpp"

