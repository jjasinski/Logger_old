#include "logger/Logger.hpp"

namespace logger
{
  void Logger::setMessageService(MessageServicePtr aService)
  {
    if (aService)
    {
      messageService = aService;
    }
    else
    {
      messageService = std::make_shared< NullMessageService >();
    }
  }



  /**
  */

  Logger::Logger(const std::string aName)
    :
    name(aName),
    loggingLevel(MessageLevel::WARNING),
    messageService(std::make_shared< NullMessageService >())
  {
  }

}// end logger