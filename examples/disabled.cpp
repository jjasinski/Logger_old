#include <iostream>
#include <chrono>

#include "Logger.hpp"


std::shared_ptr< logger::Logger > getLogger()
{
  static auto& logger = logger::registry()->getLogger("");
  return logger;
}

#define LOG_DEBUG(...) { getLogger()->debug(LOGGER_CALL_INFO, __VA_ARGS__);}

int main()
{
  // check logger performance for 'NULL configuration'
  logger::registry().registerHandle(logger::makeLoggerRegistryHandle());
  auto logger = logger::registry()->registerLogger("");
  logger->setMessageService(std::make_shared< logger::NullMessageService >());
  logger->setLevel(logger::MessageLevel::DEBUG);

  auto start = std::chrono::high_resolution_clock::now();
  const auto COUNT = 100000000; // a hundred million
  for (auto i = 0; i < COUNT; ++i)
  {
    LOG_DEBUG("line: %d", i);
  }
  
  auto end = std::chrono::high_resolution_clock::now();

  auto duration = end - start;
  std::chrono::duration<float> seconds = duration;// std::chrono::duration_cast<std::chrono::seconds>(duration);

  auto msPerLine = std::chrono::duration_cast<std::chrono::nanoseconds>(duration) / COUNT;
  printf("work time: %f sec [%lld ns * %d]\n", seconds.count(), msPerLine.count(), COUNT);

  return 0;
}