#include "logger/LoggerRegistry.hpp"

#include <map>
#include <mutex>
#include <algorithm>

namespace logger
{
  LoggerRegistry::LoggerRegistry()
    :
    handle(makeNullHandle())
  {
  }
  
  void LoggerRegistry::registerHandle(HandlePtr aHandle)
  {
    if (aHandle)
    {
      handle = aHandle;
    }
    else
    {
      handle = makeNullHandle();
    }
  }

  LoggerRegistry::HandlePtr LoggerRegistry::unregisterHandle()
  {
    auto oldHandle = handle;
    handle = makeNullHandle();
    return oldHandle;
  }

  class LoggerRegistryHandleImpl : public LoggerRegistryHandle
  {
  public:
    // Inherited via LoggerRegistryHandle
    virtual LoggerPtr registerLogger(std::string aName) override
    {
      // TODO error support
      auto logger = std::make_shared< Logger >(aName);
      
      std::lock_guard< std::mutex > lock(mutex);
      loggers[aName] = logger;
      
      return logger;
    }
    virtual LoggerPtr unregisterLogger(std::string aName) override
    {
      // TODO error support
      std::lock_guard< std::mutex > lock(mutex);
      auto logger = loggers[aName];
      loggers.erase(aName);
      return logger;
    }
    virtual LoggerPtr getLogger(std::string aName) override
    {
      // TODO can be a little optimized
      std::lock_guard< std::mutex > lock(mutex);
      if (loggers.count(aName) == 0)
      {
        throw std::logic_error("Logger for name '" + aName + "' doesn't found");
      }
      return loggers[aName];
    }
  private:
    std::mutex mutex;
    std::map< std::string, LoggerPtr > loggers;
  };

  std::shared_ptr<LoggerRegistryHandle> makeLoggerRegistryHandle()
  {
    return std::make_shared<LoggerRegistryHandleImpl>();
  }

}// end logger