// $Id$
 
#include "ace/Reactor.h"
#include "ace/Thread.h"
#include "ace/Message_Queue.h"

typedef ACE_Message_Queue<ACE_MT_SYNCH> Message_Queue;

////////////////////////////////////////////////////////////////////////////////

class IPCQueue : public Message_Queue
{
protected:
  ACE_Event_Handler * event_handler_;
  ACE_Reactor * reactor_;
public:
  IPCQueue( )
  {
    event_handler_ = NULL;
    reactor_ = NULL;
  }

  void configure( ACE_Reactor * reactor, ACE_Event_Handler * handler )
  {
    reactor_ = reactor;
    event_handler_ = handler;
  }

  virtual int notify(void)
  {
    if( reactor_ && event_handler_ )
      this->reactor_->notify( event_handler_, ACE_Event_Handler::WRITE_MASK);

    return 0;
  }

  int dequeue( ACE_Message_Block * & message_block )
  {
    ACE_Time_Value timeout(ACE_OS::time(0)+1,0);

    return dequeue_head(message_block,&timeout) ;
  }
};

////////////////////////////////////////////////////////////////////////////////

class FoobarBase
{
protected:
	IPCQueue             data_queue_;
	ACE_Thread_Manager & thr_mgr_;

public:
	FoobarBase( ACE_Thread_Manager & _thr_mgr ) : data_queue_ (), thr_mgr_ (_thr_mgr)
	{
	}

	ACE_Thread_Manager & thr_mgr(void)
	{
		return thr_mgr_;
	}

	IPCQueue & queue()
	{
		return data_queue_;
	}
};

////////////////////////////////////////////////////////////////////////////////

class Foobar : public FoobarBase
{
public:
	Foobar( ACE_Thread_Manager & _thr_mgr ) : FoobarBase(_thr_mgr)
	{
	}

	void produce_data( char * buf )
	{
		ACE_Message_Block *message_block = 
			new ACE_Message_Block( strlen(buf), ACE_Message_Block::MB_DATA, 0, buf );

		data_queue_.enqueue_tail(message_block);

		return;
	}

	void consume_data( char * buf )
	{

		if( buf )
			cerr << ACE_Thread::self() << " Consuming (" << buf << ") " << (void *)buf << endl;

		return;
	}
};

////////////////////////////////////////////////////////////////////////////////

template <class OBJ>
class FoobarHandler : public ACE_Event_Handler
{
protected:
	OBJ * foobar_;

public:
	FoobarHandler(OBJ * _foobar, ACE_Reactor * reactor)
	{
		foobar_ = _foobar;

		reactor->register_handler( this, ACE_Event_Handler::READ_MASK );
		this->reactor(reactor);

		foobar_->queue().configure( reactor, this );
	}

	virtual int handle_timeout (const ACE_Time_Value & tv, const void *arg = 0)
	{
		cerr << ACE_Thread::self() << " Timeout" << endl;

		this->reactor()->notify( this, ACE_Event_Handler::WRITE_MASK);

		return 0;
	}

	virtual int handle_output (ACE_HANDLE)
	{
		ACE_Message_Block * block = NULL;

		if( foobar_->queue().dequeue(block) != -1 )
		{
			foobar_->consume_data( block->base() );
			delete block;
		}
		else
		{
			cerr << ACE_Thread::self() << " Nothing in queue" << endl;
		}

		return 0;
	}
};

////////////////////////////////////////////////////////////////////////////////

static void *thread_func(void * arg1)
{
	Foobar * foobar = (Foobar *)arg1;

	ACE_Thread_Control thread_control ( & foobar->thr_mgr() );

	ACE_Reactor reactor;

	FoobarHandler<Foobar> handler(foobar,&reactor);

	if (reactor.schedule_timer (&handler, 0, ACE_Time_Value (1, 0), ACE_Time_Value (0, 3500000)) == -1)
		ACE_ERROR_RETURN ((LM_ERROR, "ACE_Reactor::schedule_timer"), -1);

	for( int i = 0 ; i < 50 ; ++i )
	{
		cerr << ACE_Thread::self() << " " << i << endl;
		reactor.handle_events( ACE_Time_Value(5,0 ) );
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////

main()
{
	ACE_Thread_Manager thr_mgr;

	cerr << ACE_Thread::self() << " Main Thread" << endl;

	Foobar foobar(thr_mgr);

	if (thr_mgr.spawn(ACE_THR_FUNC (thread_func),&foobar,THR_NEW_LWP | THR_DETACHED) == -1)
	      ACE_ERROR_RETURN((LM_ERROR,"%p\n","spawing client thread"),1);

	sleep(1);

	for( int i = 0 ; i < 10 ; ++i )
	{
		char buf[64];
		sprintf(buf,"This is iteration %d",i);

		foobar.produce_data(buf);

		sleep(2);
	}

	thr_mgr.wait();

	exit(0);
}

////////////////////////////////////////////////////////////////////////////////

