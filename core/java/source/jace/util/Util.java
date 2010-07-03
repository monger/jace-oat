package jace.util;

import java.io.File;
import java.io.IOException;
import java.io.StringReader;
import java.io.Writer;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Util
{
  private static final String newLine = System.getProperty("line.separator");

  /**
   * Generates a header comment with the given message.
   *
   * Instead of doing a nicer, yet more complicated word-parsing algorithm,
   * this function supports new lines, by parsing any '\n' characters passed
   * in the given message.
   *
   * @param output the output writer
   * @param message the comment body
   * @throws IOException if an error occurs while writing
   */
  public static void generateComment(Writer output, String message) throws IOException
  {
    output.write("/**" + newLine);

    StringReader reader = new StringReader(message);
    StringBuilder line = new StringBuilder();
    while (true)
    {
      output.write(" * ");
      line.setLength(0);
      while (true)
      {
        int i = reader.read();
        if (i == -1)
        {
          output.write(newLine);
          output.write(" */" + newLine);
          return;
        }

        char c = (char) i;
        output.write(c);
        line.append(c);

        // if we've encountered a newLine, we need to start a new line
        if (line.toString().endsWith(newLine))
          break;
      }
    }
  }

  /**
   * Parses a String classpath to a {@code List<File>} format.
   *
   * @param classPath the String representation of the classpath
   * @return the {@code List<File>} making up the classpath
   * @throws IllegalArgumentException if classPath is null
   */
  public static List<File> parseClasspath(String classPath) throws IllegalArgumentException
  {
    if (classPath == null)
      throw new IllegalArgumentException("classPath may not be null");
    List<File> result = new ArrayList<File>();
    List<String> classPathArray = Arrays.asList(classPath.split(File.pathSeparator));
    for (String path: classPathArray)
    {
      if (path.contains("*") || path.contains("?"))
      {
        path = path.replace(File.separator, "/");
        int index = path.lastIndexOf("/");
        String directory;
        String filename;
        if (index == -1)
        {
          directory = ".";
          filename = path;
        }
        else
        {
          directory = path.substring(0, index);
          filename = path.substring(index + "/".length());
        }
        if (directory.contains("*") || directory.contains("?"))
        {
          throw new IllegalArgumentException("classpath directories may not contain wildcards");
        }
        WildcardFileFilter filter = new WildcardFileFilter(filename);
        File[] files = new File(directory).listFiles(filter);
        for (File file: files)
          result.add(file);
      }
      else
        result.add(new File(path));
    }
    return result;
  }

  /**
   * Inserts indents at the beginning of each line of text.
   *
   * @param text the text to indent
   * @param indentSize the number of space characters to insert
   * @return the indented text
   * @throws IllegalArgumentException if text is null
   */
  public static String indent(String text, int indentSize) throws IllegalArgumentException
  {
    if (text == null)
      throw new IllegalArgumentException("text may not be null");
    // insert an indent after all newlines except the last one
    StringBuilder indentBuilder = new StringBuilder(indentSize);
    for (int i = 0; i < indentSize; ++i)
      indentBuilder.append(" ");
    String indent = indentBuilder.toString();
    // insert an indent after all newlines except the last one
    return indent + text.replaceAll(Pattern.quote(newLine) + "(?!$)", Matcher.quoteReplacement(newLine)
                                                                      + "  ");
  }
}
