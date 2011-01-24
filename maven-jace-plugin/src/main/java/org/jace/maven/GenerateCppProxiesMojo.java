package org.jace.maven;

import com.google.common.collect.Lists;
import com.google.common.collect.Sets;
import edu.umd.cs.findbugs.annotations.SuppressWarnings;
import org.apache.maven.plugin.AbstractMojo;
import org.apache.maven.plugin.MojoExecutionException;

import java.io.File;
import java.io.IOException;
import java.util.List;
import java.util.Set;
import org.apache.maven.plugin.MojoFailureException;
import org.jace.metaclass.TypeName;
import org.jace.metaclass.TypeNameFactory;
import org.jace.proxy.AutoProxy;
import org.jace.proxy.ClassPath;
import org.jace.proxy.ProxyGenerator.AccessibilityType;

/**
 * Generates a C++ proxies.
 *
 * @goal generate-cpp-proxies
 * @phase generate-cpp-proxies
 * @author Gili Tzabari
 */
public class GenerateCppProxiesMojo
	extends AbstractMojo
{
	/**
	 * The directory of the input header files.
	 *
	 * @parameter
	 * @required
	 */
	@SuppressWarnings("UWF_UNWRITTEN_FIELD")
	private List<File> inputHeaders;
	/**
	 * The directory of the input source files.
	 *
	 * @parameter
	 * @required
	 */
	@SuppressWarnings("UWF_UNWRITTEN_FIELD")
	private List<File> inputSources;
	/**
	 * The directory of the output header files.
	 *
	 * @parameter
	 * @required
	 */
	@SuppressWarnings("UWF_UNWRITTEN_FIELD")
	private File outputHeaders;
	/**
	 * The directory of the output source files.
	 *
	 * @parameter
	 * @required
	 */
	@SuppressWarnings("UWF_UNWRITTEN_FIELD")
	private File outputSources;
	/**
	 * The search path for Java classes referenced by C++ files.
	 *
	 * @parameter
	 * @required
	 */
	private List<File> classPath;
	/**
	 * Indicates the method accessibility to expose.
	 *
	 * Acceptable values include: PUBLIC, PROTECTED, PACKAGE or PRIVATE.
	 * For example, a value of PROTECTED indicates that public or protected
	 * methods should be generated.
	 *
	 * @parameter default-value="PUBLIC"
	 */
	private AccessibilityType accessibility;
	/**
	 * Indicates if the proxy symbols should be exported (for generating DLLs/SOs).
	 *
	 * @parameter default-value="false"
	 */
	private boolean exportSymbols;
	/**
	 * A list of fully-qualified class names that must be exported.
	 *
	 * When generating C++ proxies for a Java library, there is no way of
	 * knowing which classes will be referenced by 3rd-party code. This feature
	 * enables developers to export C++ proxies for Java classes even if they are
	 * not referenced at the time the generator is run.
	 *
	 * @parameter
	 */
	private List<String> forcedClasses = Lists.newArrayList();

	@Override
	@SuppressWarnings("NP_UNWRITTEN_FIELD")
	public void execute()
		throws MojoExecutionException, MojoFailureException
	{
		Set<TypeName> extraDependencies = Sets.newHashSetWithExpectedSize(forcedClasses.size());
		for (String forcedClass: forcedClasses)
			extraDependencies.add(TypeNameFactory.fromIdentifier(forcedClass));
		AutoProxy.Builder autoProxy = new AutoProxy.Builder(inputHeaders, inputSources, outputHeaders,
			outputSources, new ClassPath(classPath.toString())).accessibility(accessibility).
			minimizeDependencies(true).exportSymbols(exportSymbols);
		for (TypeName dependency: extraDependencies)
			autoProxy.extraDependency(dependency);
		try
		{
			autoProxy.generateProxies();
		}
		catch (IOException e)
		{
			throw new MojoExecutionException("", e);
		}
		catch (ClassNotFoundException e)
		{
			throw new MojoExecutionException("", e);
		}
	}
}
