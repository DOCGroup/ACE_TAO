// $Id$

#ifndef DATA_H
#define DATA_H

class DataBase
{
public:
  DataBase (void)
  {
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) DataBase ctor 0x%x\n",
                (void *) this));
  }
  virtual ~DataBase (void)
  {
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) DataBase dtor 0x%x\n",
                (void *) this));
  }

  void who_am_i (void)
  {
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) DataBase instance 0x%x\n",
                (void *) this));
  }

  virtual void what_am_i (void)
  {
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) I am a DataBase object\n"));
  }

};

class Data : public DataBase
{
public:
  Data (void)
    : message_ (-1)
  {
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) Data ctor 0x%x\n",
                (void *) this));
  }

  Data (int message)
    : message_ (message)
  {
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) Data ctor 0x%x for message %d\n",
                (void *) this, message_));
  }
  virtual ~Data (void)
  {
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) Data dtor 0x%x\n",
                (void *) this));
  }

  void what_am_i (void)
  {
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) I am a Data object for message %d\n",
                message_));
  }

protected:
  int message_;
};

#endif /* DATA_H */
