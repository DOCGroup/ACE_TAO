// $Id$

#include "common.h"
#include "ace/Event_Handler.h"

char *data = "Hello how are you";
int fragment = 0;

class frame_handler: public ACE_Event_Handler
{
  public:

  frame_handler (TAO_SFP *sfp,
                 CORBA_ORB_ptr orb,
                 ACE_Reactor *reactor)
    :sfp_ (sfp),
     orb_ (orb)
    {
      reactor->schedule_timer (this,
                               0,
                               20);
    }

  // Called when timer expires.  
  int handle_timeout (const ACE_Time_Value &tv,
                      const void *arg = 0)
    {
       ACE_DEBUG ((LM_DEBUG,"frame_handler:handle_timeout\n"));
       if (fragment)
         {
           size_t mb_len = SFP_MAX_PACKET_SIZE*2;
           ACE_DEBUG ((LM_DEBUG,"message block size = %d\n",mb_len));
           ACE_Message_Block mb (mb_len);
           int i =0;
           char c = 'a';
           for (char *ptr = mb.rd_ptr ();i<2;i++,c++,ptr+=SFP_MAX_PACKET_SIZE)
             ACE_OS::memset (ptr,c,SFP_MAX_PACKET_SIZE);
           
           mb.wr_ptr (mb_len);
           int result = this->sfp_->send_frame (&mb);
           if (result != -1)
             ACE_DEBUG ((LM_DEBUG,"simple_frame sent\n"));
           else
             ACE_DEBUG ((LM_DEBUG,"simple_frame send failed\n"));
         }
       else
         {
           ACE_Message_Block mb (data,
                                 ACE_OS::strlen (data));
           mb.wr_ptr (ACE_OS::strlen (data));
           int result = this->sfp_->send_frame (&mb);
           if (result != -1)
             ACE_DEBUG ((LM_DEBUG,"simple_frame sent\n"));
           else
             ACE_DEBUG ((LM_DEBUG,"simple_frame send failed\n"));
         }
       this->sfp_->end_stream ();
       this->orb_->shutdown ();
       return 0;
    }

private:
  TAO_SFP *sfp_;
  CORBA_ORB_ptr orb_;
};

int
main (int argc, char **argv)
{
  TAO_ORB_Manager orb_manager;
  
  orb_manager.init (argc,argv);

  if (argc > 1)
    if (ACE_OS::strcmp (argv[1],"-f") == 0)
      fragment = 1;
    
  ACE_Time_Value timeout1 (5),timeout2 (50);

  TAO_SFP sfp (orb_manager.orb (),
           TAO_ORB_Core_instance ()->reactor (),
           timeout1,
           timeout2,
           0);

  int result;

  result = sfp.start_stream (server_addr);
  if (result != 0)
    ACE_ERROR_RETURN ((LM_ERROR,"sfp start failed\n"),1);

  frame_handler handler (&sfp,
                         orb_manager.orb (),
                         TAO_ORB_Core_instance ()->reactor ());

  result = orb_manager.run ();
  if (result == 0)
    ACE_DEBUG ((LM_DEBUG,"ORB run timed out\n"));
  else if (result == -1)
    ACE_DEBUG ((LM_DEBUG,"ORB run error\n"));
  return 0;
}
