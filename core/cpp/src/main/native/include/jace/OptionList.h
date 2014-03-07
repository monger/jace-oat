#ifndef JACE_OPTION_LIST_H
#define JACE_OPTION_LIST_H

#include "jace/Namespace.h"
#include "jace/JNIException.h"

#include <jni.h>

#include <vector>
#include <string>

#include <boost/shared_ptr.hpp>

BEGIN_NAMESPACE(jace)


/**
 * A Jvm option. These are supplied to jace::createVm()
 * in an OptionList.
 *
 * @author Toby Reyelts
 */
class Option
{
public:
  virtual ~Option() {}

  /**
   * Returns the text string value of the option.
   * For example, "-Djava.class.path=c:\foo\myclasses"
   */
  virtual const std::string stringValue() const = 0;

  /**
   * Returns the extra data required for an option,
   * for example, a pointer to a vfprintf hook.
   * Returns NULL if no extra data is required.
   */
  virtual void* extraInfo() = 0;

  /**
   * Returns a dynamically allocated copy of the Option.
   */
  virtual Option* clone() const = 0;
};

/**
 * A List of Options. An OptionList is supplied to jace::createVm()
 * to specify the set of virtual machine options to be used in creating
 * a virtual machine.
 */
class OptionList
{
public:
  typedef ::boost::shared_ptr<Option> OptionPtr;

  /**
   * Creates a new empty OptionList.
   */
  OptionList();

  /**
   * Adds a clone of option to the list.
   */
  void push_back(const Option& option);

  /**
   * Returns the size of the list.
   */
  size_t size() const;

  /**
   * Returns an iterator to the beginning of the list.
   */
	std::vector<OptionPtr>::const_iterator begin() const;

  /**
   * Returns an iterator to the beginning of the list.
   */
  std::vector<OptionPtr>::const_iterator end() const;

  /**
   * Returns the OptionList as an array of JNI options.
   *
   * The caller must deallocate the options by making a call
   * to destroyJniOptions().
   */
  JavaVMOption* createJniOptions() const;

  /**
   * Releases the options created by a call to createJniOptions().
   */
  void destroyJniOptions(JavaVMOption* jniOptions) const;

private:
	/**
	 * Prevent assignment.
	 */
	OptionList& operator=(const OptionList& other);

  std::vector<OptionPtr> options;
};

/**
 * A specific type of Option, the SystemProperty tells the
 * virtual machine to set the named system property to the
 * specified value.
 */
class SystemProperty: public Option
{
public:
  /**
   * Creates a new SystemProperty with the specified name and value.
   *
   * @param name the property name (encoded using the default platform encoding)
   * @param value the property name (encoded using the default platform encoding)
   * @see jace::toPlatformEncoding(std::wstring)
   */
  SystemProperty(const std::string& name, const std::string& value);

	/**
	 * Copy constructor.
	 */
	SystemProperty(const SystemProperty& other);

	/**
   * Returns the name of the SystemProperty.
   */
  virtual const std::string name();

  /**
   * Returns the value of the SystemProperty.
   */
  virtual const std::string value();

  virtual const std::string stringValue() const;
  virtual void* extraInfo();
  virtual Option* clone() const;

private:
	/**
	 * Prevent assignment.
	 */
	SystemProperty& operator=(const SystemProperty& other);

  const std::string mName;
  const std::string mValue;
};


  //-------------------------------------------------------------------------
  // Predefined SystemProperty instances
  //-------------------------------------------------------------------------

  class Version: public SystemProperty
	{
		/**
		 * Creates a new "java.version" property with the specified value.
		 *
		 * @param value the property name (encoded using the default platform encoding)
		 * @see jace::toPlatformEncoding(std::wstring)
		 */
    public: Version(const std::string& value ): SystemProperty("java.version", value) {}
  };

  class Vendor: public SystemProperty
	{
		/**
		 * Creates a new "java.vendor" property with the specified value.
		 *
		 * @param value the property name (encoded using the default platform encoding)
		 * @see jace::toPlatformEncoding(std::wstring)
		 */
    public: Vendor(const std::string& value): SystemProperty("java.vendor", value) {}
  };

  class VendorUrl: public SystemProperty
	{
		/**
		 * Creates a new "java.vendor.url" property with the specified value.
		 *
		 * @param value the property name (encoded using the default platform encoding)
		 * @see jace::toPlatformEncoding(std::wstring)
		 */
    public: VendorUrl(const std::string& value): SystemProperty("java.vendor.url", value) {}
  };

  class Home: public SystemProperty
	{
		/**
		 * Creates a new "java.home" property with the specified value.
		 *
		 * @param value the property name (encoded using the default platform encoding)
		 * @see jace::toPlatformEncoding(std::wstring)
		 */
    public: Home(const std::string& value): SystemProperty("java.home", value) {}
  };

  class VmSpecificationVersion: public SystemProperty
	{
		/**
		 * Creates a new "java.vm.specification.version" property with the specified value.
		 *
		 * @param value the property name (encoded using the default platform encoding)
		 * @see jace::toPlatformEncoding(std::wstring)
		 */
    public: VmSpecificationVersion(const std::string& value): SystemProperty("java.vm.specification.version", value) {}
  };

  class VmSpecificationVendor: public SystemProperty
	{
		/**
		 * Creates a new "java.vm.specification.vendor" property with the specified value.
		 *
		 * @param value the property name (encoded using the default platform encoding)
		 * @see jace::toPlatformEncoding(std::wstring)
		 */
    public: VmSpecificationVendor(const std::string& value): SystemProperty("java.vm.specification.vendor", value) {}
  };

  class VmSpecificationName: public SystemProperty
	{
		/**
		 * Creates a new "java.vm.specification.name" property with the specified value.
		 *
		 * @param value the property name (encoded using the default platform encoding)
		 * @see jace::toPlatformEncoding(std::wstring)
		 */
    public: VmSpecificationName(const std::string& value): SystemProperty("java.vm.specification.name", value) {}
  };

  class VmVersion: public SystemProperty
	{
		/**
		 * Creates a new "java.vm.version" property with the specified value.
		 *
		 * @param value the property name (encoded using the default platform encoding)
		 * @see jace::toPlatformEncoding(std::wstring)
		 */
    public: VmVersion(const std::string& value): SystemProperty("java.vm.version", value) {}
  };

  class VmVendor: public SystemProperty
	{
		/**
		 * Creates a new "java.vm.vendor" property with the specified value.
		 *
		 * @param value the property name (encoded using the default platform encoding)
		 * @see jace::toPlatformEncoding(std::wstring)
		 */
    public: VmVendor(const std::string& value): SystemProperty("java.vm.vendor", value) {}
  };

  class VmName: public SystemProperty
	{
		/**
		 * Creates a new "java.vm.name" property with the specified value.
		 *
		 * @param value the property name (encoded using the default platform encoding)
		 * @see jace::toPlatformEncoding(std::wstring)
		 */
    public: VmName(const std::string& value): SystemProperty("java.vm.name", value) {}
  };

  class SpecificationVersion: public SystemProperty
	{
		/**
		 * Creates a new "java.specification.version" property with the specified value.
		 *
		 * @param value the property name (encoded using the default platform encoding)
		 * @see jace::toPlatformEncoding(std::wstring)
		 */
    public: SpecificationVersion(const std::string& value): SystemProperty("java.specification.version", value) {}
  };

  class SpecificationVendor: public SystemProperty
	{
		/**
		 * Creates a new "java.specification.vendor" property with the specified value.
		 *
		 * @param value the property name (encoded using the default platform encoding)
		 * @see jace::toPlatformEncoding(std::wstring)
		 */
    public: SpecificationVendor(const std::string& value): SystemProperty("java.specification.vendor", value) {}
  };

  class SpecificationName: public SystemProperty
	{
		/**
		 * Creates a new "java.specification.name" property with the specified value.
		 *
		 * @param value the property name (encoded using the default platform encoding)
		 * @see jace::toPlatformEncoding(std::wstring)
		 */
    public: SpecificationName(const std::string& value): SystemProperty("java.specification.name", value) {}
  };

  class ClassVersion: public SystemProperty
	{
		/**
		 * Creates a new "java.class.version" property with the specified value.
		 *
		 * @param value the property name (encoded using the default platform encoding)
		 * @see jace::toPlatformEncoding(std::wstring)
		 */
    public: ClassVersion(const std::string& value): SystemProperty("java.class.version", value) {}
  };

  class ClassPath: public SystemProperty
	{
		/**
		 * Creates a new "java.class.path" property with the specified value.
		 *
		 * @param value the property name (encoded using the default platform encoding)
		 * @see jace::toPlatformEncoding(std::wstring)
		 */
    public: ClassPath(const std::string& value): SystemProperty("java.class.path", value) {}
  };

  class LibraryPath: public SystemProperty
	{
		/**
		 * Creates a new "java.library.path" property with the specified value.
		 *
		 * @param value the property name (encoded using the default platform encoding)
		 * @see jace::toPlatformEncoding(std::wstring)
		 */
    public: LibraryPath(const std::string& value): SystemProperty("java.library.path", value) {}
  };

  class IoTmpDir: public SystemProperty
	{
		/**
		 * Creates a new "java.io.tmpdir" property with the specified value.
		 *
		 * @param value the property name (encoded using the default platform encoding)
		 * @see jace::toPlatformEncoding(std::wstring)
		 */
    public: IoTmpDir(const std::string& value): SystemProperty("java.io.tmpdir", value) {}
  };

  class Compiler: public SystemProperty
	{
		/**
		 * Creates a new "java.compiler" property with the specified value.
		 *
		 * @param value the property name (encoded using the default platform encoding)
		 * @see jace::toPlatformEncoding(std::wstring)
		 */
    public: Compiler(const std::string& value): SystemProperty("java.compiler", value) {}
  };

  class ExtDirs: public SystemProperty
	{
		/**
		 * Creates a new "java.ext.dirs" property with the specified value.
		 *
		 * @param value the property name (encoded using the default platform encoding)
		 * @see jace::toPlatformEncoding(std::wstring)
		 */
    public: ExtDirs(const std::string& value): SystemProperty("java.ext.dirs", value) {}
  };

  class OsName: public SystemProperty
	{
		/**
		 * Creates a new "java.os.name" property with the specified value.
		 *
		 * @param value the property name (encoded using the default platform encoding)
		 * @see jace::toPlatformEncoding(std::wstring)
		 */
    public: OsName(const std::string& value): SystemProperty("java.os.name", value) {}
  };

  class OsArch: public SystemProperty
	{
		/**
		 * Creates a new "java.os.arch" property with the specified value.
		 *
		 * @param value the property name (encoded using the default platform encoding)
		 * @see jace::toPlatformEncoding(std::wstring)
		 */
    public: OsArch(const std::string& value): SystemProperty("java.os.arch", value) {}
  };

  class OsVersion: public SystemProperty
	{
		/**
		 * Creates a new "java.os.version" property with the specified value.
		 *
		 * @param value the property name (encoded using the default platform encoding)
		 * @see jace::toPlatformEncoding(std::wstring)
		 */
    public: OsVersion(const std::string& value): SystemProperty("java.os.version", value) {}
  };

  class FileSeparator: public SystemProperty
	{
		/**
		 * Creates a new "file.separator" property with the specified value.
		 *
		 * @param value the property name (encoded using the default platform encoding)
		 * @see jace::toPlatformEncoding(std::wstring)
		 */
    public: FileSeparator(const std::string& value): SystemProperty("file.separator", value) {}
  };

  class PathSeparator: public SystemProperty
	{
		/**
		 * Creates a new "path.separator" property with the specified value.
		 *
		 * @param value the property name (encoded using the default platform encoding)
		 * @see jace::toPlatformEncoding(std::wstring)
		 */
    public: PathSeparator(const std::string& value): SystemProperty("path.separator", value) {}
  };

  class LineSeparator: public SystemProperty
	{
		/**
		 * Creates a new "line.separator" property with the specified value.
		 *
		 * @param value the property name (encoded using the default platform encoding)
		 * @see jace::toPlatformEncoding(std::wstring)
		 */
    public: LineSeparator(const std::string& value ): SystemProperty("line.separator", value) {}
  };

  class UserName: public SystemProperty
	{
		/**
		 * Creates a new "user.name" property with the specified value.
		 *
		 * @param value the property name (encoded using the default platform encoding)
		 * @see jace::toPlatformEncoding(std::wstring)
		 */
    public: UserName(const std::string& value): SystemProperty("user.name", value) {}
  };

  class UserHome: public SystemProperty
	{
		/**
		 * Creates a new "user.home" property with the specified value.
		 *
		 * @param value the property name (encoded using the default platform encoding)
		 * @see jace::toPlatformEncoding(std::wstring)
		 */
    public: UserHome(const std::string& value): SystemProperty("user.home", value) {}
  };

  class UserDir: public SystemProperty
	{
		/**
		 * Creates a new "user.dir" property with the specified value.
		 *
		 * @param value the property name (encoded using the default platform encoding)
		 * @see jace::toPlatformEncoding(std::wstring)
		 */
    public: UserDir(const std::string& value) : SystemProperty("user.dir", value) {}
  };


/**
 * A specific type of Option that tells the virtual machine to produce
 * verbose output of a specified type.
 *
 * Nonstandard verbose types must begin with a leading "X" character.
 *
 */
class Verbose : public Option {
public:
	/**
	 * The component that should output verbosely.
	 */
	enum ComponentType {
		/**
		 * The garbage collector.
		 */
		GC,
		/**
		 * The Java Native Interface.
		 */
		JNI,
		/**
		 * The ClassLoader.
		 */
		CLASS
	};

	/**
	 * Creates a new Verbose option.
	 *
	 * @param component the component type
	 */
  Verbose(ComponentType component);

	/**
	 * Copy constructor.
	 */
	Verbose(const Verbose& other);

  virtual const std::string stringValue() const;
  virtual void* extraInfo();
  virtual Option* clone() const;

private:
	/**
	 * Prevent assignment.
	 */
	Verbose& operator=(const Verbose& other);
	/**
	 * Converts a ComponentType to a string.
	 *
	 * @param component the component type
	 */
	std::string toString(ComponentType component) const;
  const ComponentType componentType;
};


/**
 * A specific type of Option that tells the virtual machine to load
 * a Java instrumentation agent.
 *
 */
class JavaAgent : public Option {
public:
	/**
	 * Creates a new JavaAgent.
	 *
	 * @param path the path to the JavaAgent (encoded using the default platform encoding)
	 * @see jace::toPlatformEncoding(std::wstring)
	 */
  JavaAgent(const std::string& path);

	/**
	 * Creates a new JavaAgent.
	 *
	 * @param path the path to the JavaAgent (encoded using the default platform encoding)
	 * @param options the agent options (encoded using the default platform encoding)
	 * @see jace::toPlatformEncoding(std::wstring)
	 */
	JavaAgent(const std::string& path, const std::string& options);

	/**
	 * Copy constructor.
	 */
	JavaAgent(const JavaAgent& other);

	/**
   * Returns the path of the Java agent.
   *
   */
  virtual const std::string path();

	/**
   * Returns the Java agent options.
   *
   */
  virtual const std::string options();
  virtual const std::string stringValue() const;
  virtual void* extraInfo();
  virtual Option* clone() const;

private:
	/**
	 * Prevent assignment.
	 */
	JavaAgent& operator=(const JavaAgent& other);
	/**
	 * Removes the leading and trailing whitespace from a string.
	 *
	 * @param text the input string
	 * @return the output string
	 */
	std::string trim(const std::string& text);

	const std::string mPath;
  const std::string mOptions;
};


/**
 * A virtual machine specific, custom option.
 *
 * You may specify any virtual machine specific option, for example,
 * "-Xmx=128M". All custom options must begin with a leading
 * "-X" or "_" string.
 *
 */
class CustomOption: public Option
{
public:
  /**
   * Creates a new custom option.
	 *
	 * @param value the option value (encoded using the default platform encoding)
	 * @see jace::toPlatformEncoding(std::wstring)
   */
  CustomOption(const std::string& value);

	/**
	 * Copy constructor.
	 */
	CustomOption(const CustomOption& other);

	virtual const std::string stringValue() const;
  virtual void* extraInfo();
  virtual Option* clone() const;

private:
	/**
	 * Prevent assignment.
	 */
	CustomOption& operator=(const CustomOption& other);

  const std::string value;
};


/**
 * A virtual machine option that lets you hook into a specific function.
 */
class Hook : public Option
{
};


/**
 * Lets you hook into the vfprintf() function.
 */
class VfprintfHook: public Hook
{
public:
  typedef int (*vfprintf_t)(FILE* fp, const char* format, va_list arg);

  VfprintfHook(vfprintf_t hook_);

  virtual const std::string stringValue() const;
  virtual void* extraInfo();
  virtual Option* clone() const;

private:
  vfprintf_t hook;
};

/**
 * Lets you hook into the exit() function.
 */
class ExitHook: public Hook
{
public:
  typedef void (*exit_t)(int status);

  ExitHook(exit_t _hook);

  virtual const std::string stringValue() const;
  virtual void* extraInfo();
  virtual Option* clone() const;

private:
  exit_t hook;
};


/**
 * Lets you hook into the abort() function.
 */
class AbortHook: public Hook
{
public:
  typedef void (*abort_t) (void);

  AbortHook(abort_t _hook);

  virtual const std::string stringValue() const;
  virtual void* extraInfo();
  virtual Option* clone() const;

private:
  abort_t hook;
};

END_NAMESPACE(jace)

#endif // JACE_OPTION_LIST_H

