// $Id$

ACE_INLINE void POA_CORBA::DIIPollable::is_ready_skel (
    CORBA::ServerRequest &req,
    void *obj,
    void *context,
    CORBA::Environment &env
  )
{
  POA_CORBA::Pollable_ptr impl = (POA_CORBA::DIIPollable_ptr) obj;
  POA_CORBA::Pollable::is_ready_skel (
      req,
      (POA_CORBA::Pollable_ptr) impl,
      context,
       env
    );
}

ACE_INLINE void POA_CORBA::DIIPollable::create_pollable_set_skel (
    CORBA::ServerRequest &req,
    void *obj,
    void *context,
    CORBA::Environment &env
  )
{
  POA_CORBA::Pollable_ptr impl = (POA_CORBA::DIIPollable_ptr) obj;
  POA_CORBA::Pollable::create_pollable_set_skel (
      req,
      (POA_CORBA::Pollable_ptr) impl,
      context,
       env
    );
}
