#include <memory>
#include <string>
#include <map>

//#include "logger/CallInfo.hpp"
//#include "logger/Message.hpp"
//#include "logger/MessageService.hpp"
//#include "logger/Logger.hpp"
#include "logger/LoggerRegistry.hpp"

namespace logger {



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

  void flush()
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
    logger::registry().registerHandle(logger::makeLoggerRegistryHandle());
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