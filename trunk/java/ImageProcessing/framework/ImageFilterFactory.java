package imaging.framework;

import java.awt.image.*;
import imaging.filters.*;

public class ImageFilterFactory
{
  public ImageFilterFactory()
  {
  }

  public RotateFilter createRotateFilter()
  {
    return new RotateFilter(Math.PI/3);
  }

  public MeanFilter createMeanFilter()
  {
    return new MeanFilter();
  }

  public EmbossFilter createEmbossFilter()
  {
    return new EmbossFilter();
  }

  public SobelFilter createSobelFilter()
  {
    return new SobelFilter();
  }

  public SharpenFilter createSharpenFilter()
  {
    return new SharpenFilter();
  }
   
  public DarkenFilter createDarkenFilter()
  {
    return new DarkenFilter(50);
  }

  public DissolveFilter createDissolveFilter()
  {
    return new DissolveFilter(85);
  }

}
