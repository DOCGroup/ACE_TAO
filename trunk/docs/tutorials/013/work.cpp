
// $Id$

#include "work.h"

/*
   Initialize the state to zero 
 */
Unit_Of_Work::Unit_Of_Work (void)
        : state_ (0)
{
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x Unit_Of_Work ctor\n", (void *) this));
}

Unit_Of_Work::~Unit_Of_Work (void)
{
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x Unit_Of_Work dtor\n", (void *) this));
}

/*
   Display our instance value 
 */
void Unit_Of_Work::who_am_i (void)
{
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x Unit_Of_Work instance\n", (void *) this));
}

/*
   Dispay our type name 
 */
void Unit_Of_Work::what_am_i (void)
{
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x I am a Unit_Of_Work object\n", (void *) this));
}

/*
   Return failure.  You should always derive from Unit_Of_Work... 
 */
int Unit_Of_Work::process (void)
{
    return -1;
}

/*
   ditto 
 */
int Unit_Of_Work::fini (void)
{
    return -1;
}

/*
   Default constructor has no "message number" 
 */
Work::Work (void)
        :message_ (-1)
{
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x Work ctor\n", (void *) this));
}

/*
   The useful constructor remembers which message it is and will tell you if
   you ask. 
 */
Work::Work (int message)
        : message_ (message)
{
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x Work ctor for message %d\n", (void *) this, message_));
}

Work::~Work (void)
{
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x Work dtor\n", (void *) this));
}

/*
   This objects type name is different from the baseclass 
 */
void Work::what_am_i (void)
{
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x I am a Work object for message %d\n", (void *) this, message_));
}

/*
   A very simple state machine that just walks through three stages. If it is
   called more than that, it will tell you not to bother. 
 */
int Work::process (void)
{
    switch (++state_)
    {
        case 1:
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x Stage One\n", (void *) this));
            break;
        case 2:
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x Stage Two\n", (void *) this));
            break;
        case 3:
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x Stage Three\n", (void *) this));
            break;
        default:
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x No work to do in state %d\n",
                        (void *) this, state_.value ()));
            break;
    }
    return (0);
}

/*
   If you don't have enough subtasks in the chain then the state machine won't
   progress to the end.  The fini() hook will allow us to  recover from that by 
   executing the remaining states in the final task of the chain. 
 */
int Work::fini (void)
{
    while (state_.value () < 3)
    {
        ACE_DEBUG ((LM_DEBUG, "(%P|%t) 0x%x Work::fini() state %d\n", (void *) this,state_.value()));
        if (this->process () == -1)
        {
            ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "process"), -1);
        }
    }
    return (0);
}
