#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H

#include <logger.h>
#include "client_logger_builder.h"
#include <map>
#include <set>

class client_logger final:
    public logger
{

public:

    client_logger(
        client_logger const &other);

    client_logger &operator=(
        client_logger const &other);

    client_logger(
        client_logger &&other) noexcept;

    client_logger &operator=(
        client_logger &&other) noexcept;

    ~client_logger() noexcept final;

public:

    [[nodiscard]] logger const *log(
        const std::string &message,
        logger::severity severity) const noexcept override;

friend class client_logger_builder;

private:
    client_logger(
        const std::map<std::string, std::set<logger::severity>> &streams, const std::string &format);

    void format(
        std::string &to_format, std::string const &flag,
        std::string const &replace) const noexcept;

private:
    static std::map<std::string, std::pair<std::ofstream, unsigned int>> _counter;

    std::map<std::string, std::set<logger::severity>> _streams;

    std::string _format;
    size_t _format_size;
    void close_unused_streams();
};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H