
// $Id$

#ifndef PROTOCOL_TASK_H
#define PROTOCOL_TASK_H

#include "ace/Task.h"

class Protocol_Task : public ACE_Task<ACE_MT_SYNCH>
{
public:

    typedef ACE_Task<ACE_MT_SYNCH> inherited;
    
    Protocol_Task( int _thr_count );
    ~Protocol_Task(void);

    virtual int open(void *arg);
    virtual int close(u_long flags);

    virtual int put(ACE_Message_Block *message,
                    ACE_Time_Value *timeout);

    virtual int svc(void);

protected:

    int process(ACE_Message_Block * message, ACE_Time_Value *timeout);
    
    int is_active(void)
        {
            return this->thr_count() != 0;
        }

    virtual int send(ACE_Message_Block *message,
                     ACE_Time_Value *timeout)
        {
            return -1;
        }
    
    virtual int recv(ACE_Message_Block * message,
                     ACE_Time_Value *timeout)
        {
            return -1;
        }

private:
    int desired_thr_count_;
};

#endif // PROTOCOL_TASK_H
