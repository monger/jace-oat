package jace.metaclass;

/**
 * Jace constants.
 *
 * @author Gili Tzabari
 */
public class JaceConstants
{
  /**
   * Prevent construction.
   */
  private JaceConstants()
  {
  }

  /**
   * The package used to prefix all Jace peers.
   *
   * @return the package used to prefix all Jace peers
   */
  public static TypeName getPeerPackage()
  {
    return TypeNameFactory.fromPath("jace/peer");
  }

  /**
   * The package used to prefix all Jace proxies.
   *
   * @return the package used to prefix all Jace proxies
   */
  public static TypeName getProxyPackage()
  {
    return TypeNameFactory.fromPath("jace/proxy");
  }
}
