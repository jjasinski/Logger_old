#include <iostream>
#include <chrono>

#include "Logger.hpp"
#include "logger\MessageService.hpp"

namespace logger
{
  class StdOutMessageService : public MessageService
  {
  public:
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

std::shared_ptr< logger::Logger > getLogger()
{
  static auto& logger = logger::registry()->getLogger("");
  return logger;
}

#define LOG_DEBUG(...) { getLogger()->debug(LOGGER_CALL_INFO, __VA_ARGS__); }
#define LOG_ERROR(...) { getLogger()->error(LOGGER_CALL_INFO, __VA_ARGS__); }
int main()
{
  // check logger performance for 'NULL configuration'
  logger::registry().registerHandle(logger::makeLoggerRegistryHandle());
  auto logger = logger::registry()->registerLogger("");
  //logger->setMessageService(std::make_shared< logger::NullMessageService >());
  auto messageService = std::make_shared< logger::StdOutMessageService >();
  logger->setMessageService(messageService);
  logger->setLevel(logger::MessageLevel::DEBUG);

  auto start = std::chrono::high_resolution_clock::now();
  const auto COUNT = 100; // a hundred million
  for (auto i = 0; i < COUNT; ++i)
  {
    LOG_DEBUG("debug message no: %d", i);
    LOG_ERROR("error message no: %d", i);
  }
  messageService->flush();
  auto end = std::chrono::high_resolution_clock::now();

  auto duration = end - start;
  std::chrono::duration<float> seconds = duration;// std::chrono::duration_cast<std::chrono::seconds>(duration);

  auto msPerLine = std::chrono::duration_cast<std::chrono::nanoseconds>(duration) / COUNT;
  printf("work time: %f sec [%lld ns * %d]\n", seconds.count(), msPerLine.count(), COUNT);

  return 0;
}