#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/date_time/posix_time/posix_time.hpp> // Include this header
#include <boost/log/utility/setup/common_attributes.hpp>
#include <string>

namespace logging = boost::log;
namespace keywords = boost::log::keywords;

class Logger {
public:
    Logger() {
        // Initialize Boost.Log
        logging::register_simple_formatter_factory<logging::trivial::severity_level, char>("Severity");

        boost::log::add_console_log(
                std::cout,
                boost::log::keywords::format = ("[%TimeStamp%] [%Severity%] %Message%")
        );
        boost::log::add_file_log(
                boost::log::keywords::file_name = "log_%N.log",
                boost::log::keywords::format = ("[%TimeStamp%] [%Severity%] %Message%")
        );

        boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::info);
        logging::add_common_attributes();
    }

    template <typename... Args>
    void logInfo(const Args&... args) {
        BOOST_LOG_TRIVIAL(info) << concatenate(args...);
        boost::log::core::get()->flush(); // Flush the log stream
    }

    template <typename... Args>
    void logError(const Args&... args) {
        BOOST_LOG_TRIVIAL(error) << concatenate(args...);
        boost::log::core::get()->flush(); // Flush the log stream
    }

    template <typename... Args>
    void logWarn(const Args&... args){
        BOOST_LOG_TRIVIAL(warning) << concatenate(args...);
        boost::log::core::get()->flush(); // Flush the log stream
    }

private:
    template <typename T>
    std::string concatenate(const T& value) {
        return std::to_string((double) value);
    }

    static std::string getTimeStamp() {
        const auto now = boost::posix_time::second_clock::local_time();
        return "[" + boost::posix_time::to_simple_string(now.time_of_day()) + "]\t";
    }

    std::string concatenate(const char* value) {
        return value;
    }

    template <typename T, typename... Args>
    std::string concatenate(const T& value, const Args&... args) {
        return concatenate(value) + " " + concatenate(args...);
    }
};