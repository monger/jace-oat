package jace.metaclass;

/**
 * Represents the meta-data for the java primitive, 'float'.
 *
 * @author Toby Reyelts
 * @author Gili Tzabari
 */
public class FloatClass extends PrimitiveMetaClass
{
  /**
   * Creates a new FloatClass.
   *
   * @param isProxy true if the object represents a proxy
   */
  public FloatClass(boolean isProxy)
  {
    super(isProxy);
  }

  @Override
  protected MetaClass newInstance(boolean isProxy)
  {
    return new FloatClass(isProxy);
  }

  public String getName()
  {
    return "JFloat";
  }

  @Override
  public boolean equals(Object obj)
  {
    return obj instanceof FloatClass;
  }

  @Override
  public int hashCode()
  {
    return getName().hashCode();
  }

  public String getJniType()
  {
    return "jfloat";
  }
}
