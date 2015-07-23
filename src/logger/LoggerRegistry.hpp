#pragma once

//------------------------------------------

// INCLUDES
#include <cassert>
#include "logger/Logger.hpp"


//------------------------------------------
// BEGIN OF CLASS DECLARATION

namespace logger
{

  class LoggerRegistryHandle
  {
  public:
    typedef std::shared_ptr< Logger > LoggerPtr;

    LoggerRegistryHandle() = default;
    virtual ~LoggerRegistryHandle() = default;

    virtual LoggerPtr registerLogger(std::string aName) = 0;
    virtual LoggerPtr unregisterLogger(std::string aName) = 0;
    virtual LoggerPtr getLogger(std::string aName) = 0;
  };

  /**
   */
  class LoggerRegistry
  {
  private:

    /** Null Object Pattern implementation of RegistryHandle
    */
    class NullHandle : public LoggerRegistryHandle
    {
    public:
      // Inherited via RegistryHandle
      virtual LoggerPtr registerLogger(std::string aName) override
      {
        throw std::logic_error("Null object is called");
      }
      virtual LoggerPtr unregisterLogger(std::string aName) override
      {
        throw std::logic_error("Null object is called");
      }
      virtual LoggerPtr getLogger(std::string aName) override
      {
        throw std::logic_error("Null object is called");
      }
    };
  public:
    typedef std::shared_ptr< LoggerRegistryHandle > HandlePtr;

    LoggerRegistry();

    void registerHandle(HandlePtr aHandle);

    HandlePtr unregisterHandle();

    HandlePtr operator->()
    {
      assert(handle && "RegistryHandle is not initialized");
      return handle;
    }

  private:
    HandlePtr handle;

    HandlePtr makeNullHandle() const
    {
      return std::make_shared< NullHandle >();
    }
  };

  /** Accessor to the global LoggerRegistry object
   */
  inline LoggerRegistry& registry()
  {
    static LoggerRegistry instance;
    return instance;
  }

  std::shared_ptr< LoggerRegistryHandle > makeLoggerRegistryHandle();


}// end logger

//------------------------------------------