package jace.ant;

import jace.autoproxy.AutoProxy;
import jace.metaclass.TypeName;
import jace.metaclass.TypeNameFactory;
import jace.util.Util;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashSet;
import java.util.Set;
import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.DirectoryScanner;
import org.apache.tools.ant.Project;
import org.apache.tools.ant.Task;
import org.apache.tools.ant.types.DirSet;
import org.apache.tools.ant.types.Path;

/**
 * Generates C++ proxies for Java classes.
 *
 * Example:
 * &lt;GenerateCppProxies inputHeaders="input/include" inputSources="input/source" outputHeaders="output/include"
 * outputSource="output/source" exportSymbols="false" classpath="rt.jar"&gt;
 *   &lt;classpath&gt;
 *     &lt;pathelement location="classes"/&gt;
 *   &lt;/classpath&gt;
 *   &lt;dependency name="java.lang.String"/&gt;
 *   &lt;inputHeaders dir="input"&gt;
 *     &lt;include name="include1"&gt;
 *     &lt;include name="include2"&gt;
 *   &lt;/inputHeaders&gt;
 *   &lt;inputSources dir="input"&gt;
 *     &lt;include name="source1"&gt;
 *     &lt;include name="source2"&gt;
 *   &lt;/inputSources&gt;
 * &lt;/GenerateCppProxies&gt;
 *
 * @author Gili Tzbari
 */
public class GenerateCppProxiesTask extends Task
{
  private final Collection<File> inputHeaders = new ArrayList<File>();
  private final Collection<File> inputSources = new ArrayList<File>();
  private File outputHeaders;
  private File outputSources;
  private Path classpath = new Path(getProject());
  /**
   * Unused classes to generate proxies for (useful for libraries where the used classes are not known in advance).
   */
  private Set<Dependency> dependencies = new HashSet<Dependency>();
  /**
   * True if proxies should export their symbols (for DLLs/SOs).
   */
  private boolean exportSymbols;

  /**
   * Sets the directory containing the input header files.
   *
   * @param inputHeaders the directory containing the input header files
   */
  public void setInputHeaders(String inputHeaders)
  {
    this.inputHeaders.clear();
    for (String path: inputHeaders.split(File.pathSeparator))
      this.inputHeaders.add(new File(path));
  }

  /**
   * Sets the directory containing the input source files.
   *
   * @param inputSources the directory containing the input source files
   */
  public void setInputSources(String inputSources)
  {
    this.inputSources.clear();
    for (String path: inputSources.split(File.pathSeparator))
      this.inputSources.add(new File(path));
  }

  /**
   * Sets the directory containing the output header files.
   *
   * @param outputHeaders the directory containing the output header files
   */
  public void setOutputHeaders(File outputHeaders)
  {
    this.outputHeaders = outputHeaders;
  }

  /**
   * Sets the directory containing the output source files.
   *
   * @param outputSources the directory containing the output source files
   */
  public void setOutputSources(File outputSources)
  {
    this.outputSources = outputSources;
  }

  /**
   * Indicates if the proxy symbols should be exported (for generating DLLs/SOs).
   *
   * @param exportSymbols true if the proxy symbols should be exported
   */
  public void setExportSymbols(boolean exportSymbols)
  {
    this.exportSymbols = exportSymbols;
  }

  /**
   * Sets the Java classpath.
   *
   * @param classpath the Java classpath
   */
  public void setClasspath(Path classpath)
  {
    this.classpath = classpath;
  }

  @Override
  public void execute() throws BuildException
  {
    if (inputHeaders.isEmpty())
      throw new BuildException("must specify at least one inputHeaders directory", getLocation());
    if (inputSources.isEmpty())
      throw new BuildException("must specify at least one inputSources directory", getLocation());
    if (outputHeaders == null)
      throw new BuildException("outputHeaders must be set", getLocation());
    if (outputSources == null)
      throw new BuildException("outputSources must be set", getLocation());
    log(toString(), Project.MSG_DEBUG);
    Set<TypeName> extraDependencies = new HashSet<TypeName>();
    for (Dependency dependency: dependencies)
      extraDependencies.add(TypeNameFactory.fromIdentifier(dependency.getName()));
    AutoProxy autoProxy = new AutoProxy(inputHeaders, inputSources, outputHeaders, outputSources,
      Util.parseClasspath(classpath.toString()), true, extraDependencies, exportSymbols);
    try
    {
      autoProxy.generateProxies();
    }
    catch (IOException e)
    {
      throw new BuildException(e);
    }
  }

  /**
   * Adds a class that must have a C++ proxy generated.
   *
   * @param dependency a class that must have a C++ proxy generated
   */
  public void addConfiguredLibrary(Dependency dependency)
  {
    if (dependency.getName() == null)
      throw new BuildException("name must be set", getLocation());
    dependencies.add(dependency);
  }

  /**
   * Adds to the Java classpath.
   *
   * @param classpath the Java classpath
   */
  public void addConfiguredClasspath(Path classpath)
  {
    this.classpath.add(classpath);
  }

  /**
   * Adds input header directories.
   *
   * @param headers the input header directories
   */
  public void addConfiguredInputHeaders(DirSet headers)
  {
    DirectoryScanner scanner = headers.getDirectoryScanner(getProject());
    scanner.scan();
    for (String directory: scanner.getIncludedDirectories())
      this.inputHeaders.add(new File(scanner.getBasedir(), directory));
  }

  /**
   * Adds input source directories.
   *
   * @param sources the input source directories
   */
  public void addConfiguredInputSources(DirSet sources)
  {
    DirectoryScanner scanner = sources.getDirectoryScanner(getProject());
    scanner.scan();
    for (String directory: scanner.getIncludedDirectories())
      this.inputSources.add(new File(scanner.getBasedir(), directory));
  }

  @Override
  public String toString()
  {
    return getClass().getSimpleName() + "[inputHeaders=" + inputHeaders + ", inputSources=" + inputSources +
           ", outputHeader=" + outputHeaders + ", outputSources=" + outputSources + ", exportSymbols=" + exportSymbols +
           "]";
  }
}