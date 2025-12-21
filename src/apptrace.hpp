#ifndef __APPTRACE_HPP__
#define __APPTRACE_HPP__

#include <string>
#include <sstream>
#include <iostream>
#include <utility>

enum class TRACE_LEVEL {ERROR, WARNING, INFO, DEBUG};

class AppTrace
{
    public:
        inline static TRACE_LEVEL log_depth = TRACE_LEVEL::INFO;
        static void log(TRACE_LEVEL level, const std::string& message)
        {
            std::string outMessage = "";
            if(level <= AppTrace::log_depth)
            {
                switch(level)
                {
                    case TRACE_LEVEL::ERROR:
                        outMessage = "[ERROR] " + message;
                        break;
                    case TRACE_LEVEL::WARNING:
                        outMessage = "[WARNING] " + message;
                        break;
                    case TRACE_LEVEL::INFO:
                        outMessage = "[INFO] " + message;
                        break;
                    case TRACE_LEVEL::DEBUG:
                        outMessage = "[DEBUG] " + message;
                        break;
                }
                std::cout << outMessage << std::endl;
            }    
        }
};

#endif