package org.jace.metaclass;

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
	 * The package used to prefix all Jace proxies.
	 *
	 * @return the package used to prefix all Jace proxies
	 */
	public static TypeName getProxyPackage()
	{
		return TypeNameFactory.fromPath("jace/proxy");
	}
}
