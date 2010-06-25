package jace.util;

import java.util.Collection;
import java.util.Iterator;

/**
 * A utility class that returns the String representation of a collection.
 *
 * @param <T> the list type
 * @author Toby Reyelts
 */
public class DelimitedCollection<T>
{
  private Collection<T> collection;

  /**
   * Returns the String representation of an Object.
   *
   * @param <T> the object type
   * @author Toby Reyelts
   */
  public interface Stringifier<T>
  {
    /**
     * Returns the String representation of the object.
     *
     * @param t the object
     * @return the String representation
     */
    public String toString(T t);
  }
  /**
   * Returns Object.toString().
   *
   * @author Toby Reyelts
   */
  private static Stringifier<Object> defaultStringifier = new Stringifier<Object>()
  {
    public String toString(Object obj)
    {
      return obj.toString();
    }
  };

  /**
   * Creates a new DelimitedCollection.
   *
   * @param c the collection
   */
  public DelimitedCollection(Collection<T> c)
  {
    collection = c;
  }

  /**
   * Returns the String representation of the collection.
   *
   * @param separator the separator string
   * @return the String representation of the collection
   */
  public String toString(String separator)
  {
    @SuppressWarnings("unchecked")
    Stringifier<T> stringifier = (Stringifier<T>) defaultStringifier;
    return toString(stringifier, separator);
  }

  /**
   * Returns the String representation of the collection.
   *
   * @param stringifier generates the String representation of the collection elements
   * @param separator the separator string
   * @return the String representation of the collection
   */
  public String toString(Stringifier<T> stringifier, String separator)
  {
    StringBuilder sb = new StringBuilder();
    for (Iterator<T> it = collection.iterator(); it.hasNext();)
    {
      sb.append(stringifier.toString(it.next()));

      if (!it.hasNext())
        return sb.toString();
      sb.append(separator);
    }
    return sb.toString();
  }
}