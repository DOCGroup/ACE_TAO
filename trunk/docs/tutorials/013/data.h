
// $Id$

#ifndef DATA_H
#define DATA_H

#include "ace/Message_Block.h"
#include "ace/Synch.h"

class Data_Base
{
public:
    Data_Base (void)
            : state_(0)
        {
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x Data_Base ctor\n", (void *) this));
			++self_;
			++instance_count_;
        }
    virtual ~ Data_Base (void)
        {
			if( --self_ < 0 )
				foo();
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x Data_Base dtor (%d) (%d)\n", (void *) this, self_.value(), --instance_count_));
        }

	void foo(void)
	{
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x Data_Base FOO\n", (void *) this ));
	}

    void who_am_i (void)
        {
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x Data_Base instance\n", (void *) this));
        }

    virtual void what_am_i (void)
        {
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x I am a Data_Base object\n", (void*)this));
        }

    virtual int process(void)
        {
            switch( ++state_ )
            {
                case 1:
                    ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x Stage One\n",(void*)this));
                    break;
                case 2:
                    ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x Stage Two\n",(void*)this));
                    break;
                case 3:
                    ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x Stage Three\n",(void*)this));
                    break;
            }
            return(0);
        }

protected:
    ACE_Atomic_Op<ACE_Mutex,int> state_;
    ACE_Atomic_Op<ACE_Mutex,int> self_;
    static ACE_Atomic_Op<ACE_Mutex,int> instance_count_;
};

class Data : public Data_Base
{
public:
    Data (void)
            : message_ (-1)
        {
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x Data ctor\n", (void *) this));
        }

    Data (int message)
            : message_ (message)
        {
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x Data ctor for message %d\n", (void *) this, message_));
        }
    virtual ~ Data (void)
        {
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x Data dtor\n", (void *) this));
        }

    void what_am_i (void)
        {
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x I am a Data object for message %d\n", (void*)this, message_));
        }

protected:
    int message_;

};

class Data_Block : public ACE_Data_Block
{
public:
    typedef ACE_Data_Block inherited;
	typedef ACE_Lock_Adapter<ACE_Mutex> lock_t;

    Data_Block( Data_Base * _data )
              : inherited(0,ACE_Message_Block::MB_DATA,0,0,new lock_t(),0,0)	// 4.5.39
//            : inherited(0,ACE_Message_Block::MB_DATA,0,0,0,0)		// 4.4.32 -- OK
//            : inherited()	// 4.4.32 -- OK , 4.5.39 -- SEGV
        ,data_(_data)
        {
    		ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x Data_Block ctor for 0x%x\n", (void *) this, (void*)data_));
        }
    ~Data_Block(void)
        {
    		ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x Data_Block dtor for 0x%x\n", (void *) this, (void*)data_));
            delete data_;
        }

    Data_Base * data(void)
        {
            return this->data_;
        }
    
protected:
    Data_Base * data_;
};

class Message_Block : public ACE_Message_Block
{
public:
    typedef ACE_Message_Block inherited;
    
    Message_Block( void )
            : inherited( new Data_Block( new Data_Base() ) )
        {
    		ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x Message_Block ctor for shutdown\n", (void *) this ));
			this->msg_type( MB_HANGUP );
        }
    Message_Block( Data_Base * _data )
            : inherited( new Data_Block(_data) )
        {
    		ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x Message_Block ctor for 0x%x\n", (void *) this, (void*)_data));
        }

    ~Message_Block( void )
        {
    		ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x Message_Block dtor\n", (void *) this ));
        }

	Data_Base * data(void)
		{
            return ((Data_Block*)this->data_block())->data();
        }
};
            
#endif
