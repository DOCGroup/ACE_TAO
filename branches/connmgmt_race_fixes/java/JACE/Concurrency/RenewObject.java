package JACE.Concurrency;

import JACE.ASX.TimedWait;

class RenewObject extends TimedWait
{
  public RenewObject (int maxYieldTo)
  {
    yieldTo_ = maxYieldTo;
  }

  public boolean condition ()
  {
    return yieldTo_ <= 0;
  }

  public void decrementYieldTo()
  {
    this.yieldTo_--;
  }

  public int yieldTo ()
  {
    return this.yieldTo_;
  }
  
  public RenewObject min (RenewObject other)
  {
    if (other.yieldTo_ < this.yieldTo_)
      return other;
    else
      return this;
  }

  private int yieldTo_;
}
