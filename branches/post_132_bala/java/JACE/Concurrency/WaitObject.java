package JACE.Concurrency;

import JACE.ASX.TimedWait;

class WaitObject extends TimedWait
{
  public WaitObject ()
  {
    super ();
  }

  public WaitObject (Object obj)
  {
    super (obj);
  }

  public WaitObject (boolean initialState)
  {
    condition_ = initialState;
  }

  public WaitObject (boolean initialState, Object obj)
  {
    super (obj);
    condition_ = initialState;
  }

  public boolean condition ()
  {
    return this.condition_;
  }
  
  public void condition (boolean c)
  {
    this.condition_ = c;
  }

  private boolean condition_ = false;
}
