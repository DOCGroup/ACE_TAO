// $Id$

#include "common.h"

class mySFP_Callback: public SFP_Callback
{
  public:
  mySFP_Callback (CORBA_ORB_ptr orb)
    :orb_ (orb)
    {
    }
  // This is called for both active and passive start.
  virtual int start_failed (void) 
    {
      return  0;
    }

  // This is a callback for both active and passive stream
  // establshment.
  virtual int stream_established (void)
    {
      return 0;
    }  

  virtual void end_stream (void)
    {
      this->orb_->shutdown ();
    }

  virtual int receive_frame (ACE_Message_Block *frame)
    {
      ACE_DEBUG ((LM_DEBUG,"mySFP_Callback:receive_frame\n"));
      ACE_Message_Block *block = frame;
      while (block != 0)
        {
          char *buf =block->rd_ptr ();
          ACE_DEBUG ((LM_DEBUG,"length of buf = %d\n",block->length ()));
          for (u_int i=0;i<block->length ();i++)
            ACE_DEBUG ((LM_DEBUG,"%c ",buf[i]));
          ACE_DEBUG ((LM_DEBUG,"\n"));
          block = block->cont ();
        }
      return 0;
    }
    private:
      CORBA_ORB_ptr orb_;
};

int
main (int argc, char **argv)
{
  TAO_ORB_Manager orb_manager;
  
  orb_manager.init (argc,argv);

  ACE_Time_Value timeout1 (5),timeout2 (50);

  mySFP_Callback callback (orb_manager.orb ());
  TAO_SFP sfp (orb_manager.orb (),
               TAO_ORB_Core_instance ()->reactor (),
               timeout1,
               timeout2,
               &callback);

  int result;
  // passive start.
  result = sfp.start_stream (server_addr,10);
  if (result != 0)
    return 1;
  
  result = orb_manager.run ();
  if (result == 0)
    ACE_DEBUG ((LM_DEBUG,"ORB run timed out\n"));
  else if (result == -1)
    ACE_DEBUG ((LM_DEBUG,"ORB run error\n"));
  return 0;
}
