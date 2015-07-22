#include <memory>
#include <string>
#include <map>

/*
registry()->registerLogger("ModuleName");
Logger logger = registry()->getLogger("ModuleName");

logger.debug() // do nothing

namespace logger
Message ()
Registry
LoggerHolder/MessageService (createMessage, writeMessage, flush)
Logger

Logger.debug() -> MessageService.createMessage(), writeMessage



*/

//std::make_unique
//class Message;
//
//typedef std::unique_ptr< Message > MessagePtr;

#define LOGGER_CALL_INFO ::logger::CallInfo(__FUNCTION__, __FUNCSIG__, __FILE__, __LINE__)

namespace logger {

enum class MessageLevel
{
  FATAL,    /// fatal error - generally causes stop/crash of the application
  ERROR,    /// normal error - something is wrong
  WARNING,  /// warning - 
  INFO, 
  DEBUG,
  DEBUG_FINE,
  DEBUG_FINER,
  DEBUG_FINEST,
  TRACE,
  TRIVIAL
};

/** Definition of a function call
*/
struct CallInfo
{
  CallInfo(const char* aFunc, const char* aFunctionDecorated, const char* aFile, unsigned int aLine)
    :
    function(aFunc),
    functionDecorated(aFunctionDecorated),
    file(aFile),
    line(aLine)
  {
  }

  CallInfo(const CallInfo& info) = default;

  const char* function;
  const char* functionDecorated;
  const char* file;
  const unsigned int line;
};

class Message
{
public:
  Message(MessageLevel aLevel, const CallInfo& anInfo)
    :
    level(aLevel),
    info(anInfo)
  {
  }

  virtual ~Message() = default;

  const MessageLevel level;
  const CallInfo info;
  std::string content;
};

class MessageService
{
public:
  typedef std::unique_ptr< Message > MessagePtr;
  MessageService() = default;
  virtual ~MessageService() = default;
 
  virtual MessagePtr createMessage(MessageLevel aLevel, const CallInfo& anInfo) = 0;
  virtual void writeMessage(MessagePtr aMessage) = 0;
  virtual void flush() = 0;
};

//class MessageFactory
//{
//public:
//  virtual MessagePtr createMessage(...);
//};
//
//class MessageOutput
//{
//public:
//  void write(MessagePtr message);
//  void flush();
//};

class Logger
{
public:
  typedef std::shared_ptr< MessageService > MessageServicePtr;

  /**
  */
  Logger(const std::string aName/*, MessageServicePtr aMessageService*/)
    :
    name(aName),
    //messageService(aMessageService),
    loggingLevel(MessageLevel::WARNING)
  {
  }

  const std::string name;



  //void fatal();
  //void error();
  //void warning();
  //void info();
  void debug(const CallInfo& anInfo)
  {
    if (loggingLevel >= MessageLevel::DEBUG)
    {
      if (auto message = messageService->createMessage(MessageLevel::DEBUG, anInfo))
      {
        message->content = "";//TODO
        messageService->writeMessage(std::move(message));
      }
    }
  }
  //void trace();
  //void trivial();
  MessageLevel getLevel() const { return loggingLevel; }
  void setLevel(MessageLevel aLevel) { loggingLevel = aLevel; }

  MessageServicePtr getMessageService() { return messageService; }
  void setMessageService(MessageServicePtr aService) { messageService = aService; }
private:
  MessageLevel loggingLevel;
  MessageServicePtr messageService;

};

class RegistryHandle
{
public:
  typedef std::shared_ptr< Logger > LoggerPtr;

  RegistryHandle() = default;
  virtual ~RegistryHandle() = default;

  virtual LoggerPtr registerLogger(std::string aName) = 0;
  virtual LoggerPtr unregisterLogger(std::string aName) = 0;
  virtual LoggerPtr getLogger(std::string aName) = 0;
};

class LoggerRegistry
{
public:
  typedef std::shared_ptr< RegistryHandle > HandlePtr;
  LoggerRegistry() = default;

  void registerHandle(HandlePtr aHandle)
  {
    handle = aHandle;
  }

  HandlePtr unregisterHandle()
  {
    auto oldHandle = handle;
    handle = nullptr;
    return oldHandle;
  }

  HandlePtr operator->()
  {
    if (!handle)
    {
      throw std::logic_error("RegistryHandle not initialized");
    }
    return handle;
  }

private:
  HandlePtr handle;
};

class RegistryHandleImpl: public RegistryHandle
{
public:
  RegistryHandleImpl() = default;
  virtual ~RegistryHandleImpl() = default;

  virtual LoggerPtr registerLogger(std::string aName)
  {
    // TODO error support
    auto logger = std::make_shared< Logger >(aName);
    loggers[aName] = logger;
    return logger;
  }

  virtual LoggerPtr unregisterLogger(std::string aName)
  {
    // TODO error support
    auto logger = loggers[aName];
    loggers.erase(aName);
    return logger;
  }

  virtual LoggerPtr getLogger(std::string aName)
  {
    if (loggers.count(aName) == 0)
    {
      throw std::logic_error("Logger '" + aName + "' not found");
    }
    return loggers[aName];
  }

private:
  std::map< std::string, LoggerPtr > loggers;
};

LoggerRegistry& registry()
{
  static LoggerRegistry instance;
  return instance;
}

class NullMessageService : public MessageService
{
  // Inherited via MessageService
  virtual MessagePtr createMessage(MessageLevel aLevel, const CallInfo & anInfo) override
  {
    return nullptr;
  }

  virtual void writeMessage(MessagePtr aMessage) override
  {
  }

  virtual void flush() override
  {
  }

};

class StdOutMessageService : public MessageService
{
  // Inherited via MessageService
  virtual MessagePtr createMessage(MessageLevel aLevel, const CallInfo & anInfo) override
  {
    return std::make_unique< Message >(aLevel, anInfo);
  }

  virtual void writeMessage(MessagePtr aMessage) override
  {
    auto descr = aMessage->level >= MessageLevel::WARNING ? stderr : stdout;
    fprintf(descr, 
      "[%s] %s\n", 
      getMessageLevelDescription(aMessage->level), 
      aMessage->content.c_str());
  }

  virtual void flush() override
  {
    fflush(stderr);
    fflush(stdout);
  }

private:
  const char* getMessageLevelDescription(MessageLevel aLevel) const
  {
    switch (aLevel)
    {
    case MessageLevel::FATAL: return "FATAL ERROR";
    case MessageLevel::ERROR: return "ERROR";
    case MessageLevel::WARNING: return "WARNING";
    case MessageLevel::INFO: return "INFO";
    case MessageLevel::DEBUG:
    case MessageLevel::DEBUG_FINE:
    case MessageLevel::DEBUG_FINER:
    case MessageLevel::DEBUG_FINEST: return "DEBUG";
    case MessageLevel::TRACE: return "TRACE";
    case MessageLevel::TRIVIAL: return "TRIVIAL";
    };
    return "";
  }
};

}// end logger




void main()
{
  try
  {
    logger::registry().registerHandle(std::make_shared< logger::RegistryHandleImpl >());
    // initialize of the logger registry
    
    
    // configuration
    auto logger = logger::registry()->registerLogger("moduleName");
    logger->setLevel(logger::MessageLevel::DEBUG);
    
    auto nullMessageService = std::make_shared< logger::NullMessageService >();
    auto stdOutMessageService = std::make_shared< logger::StdOutMessageService >();
    logger->setMessageService(nullMessageService);
    logger->setMessageService(stdOutMessageService);

    // usage
    logger->debug(LOGGER_CALL_INFO);


    //auto logger2 = logger::registry()->getLogger("");

    //logger->debug(logger::);

    // free of the logger registry (better to do it)
    logger::registry().unregisterHandle();
  }
  catch (const std::exception& e)
  {
    printf("catch exception: %s\n", e.what());
  }
}