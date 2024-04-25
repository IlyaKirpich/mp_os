
#include "../include/client_logger.h"

std::map<std::string, std::pair<std::ofstream, unsigned int>> client_logger::_counter = 
    std::map<std::string, std::pair<std::ofstream, unsigned int>>();

client_logger::client_logger(
    const std::map<std::string, std::set<logger::severity>> &streams, const std::string &format)
    : _format(format), _format_size(format.size())
{
    for (auto &[key, set] : streams) 
    {
        if (_counter.find(key) == _counter.end()) 
        {
            if (key != "/console"){
                _counter[key].first.open(key);
                if (_counter[key].first.is_open() == false)
                {
                    throw std::runtime_error("Error file" + key);
                }
            }
            _counter[key].second = 1;
        }
        else 
        {
            _counter[key].second++;
        }
        _streams[key] = set;
    }
}

client_logger::client_logger(
    client_logger const &other)
    : _format(other._format), _format_size(other._format_size), _streams(other._streams)
{
    for (auto &[key, set] : _streams)
    {
        _counter[key].second++;
    }
}

client_logger &client_logger::operator=(
    client_logger const &other)
{
    if (this == &other)
    {
        return *this;
    }
    close_unused_streams();
    _streams = other._streams;
    _format = other._format;
    _format_size = other._format_size;
    for (auto &[key, set] : _streams)
    {
        _counter[key].second++;
    }
    return *this;
}

client_logger::client_logger(
    client_logger &&other) noexcept
    :_streams(std::move(other._streams)), _format(std::move(other._format)), _format_size(std::move(other._format_size)){}

client_logger &client_logger::operator=(
    client_logger &&other) noexcept
{
    if (this == &other)
    {
        return *this;
    }
    close_unused_streams();
    _streams = std::move(other._streams);
    _format = std::move(other._format);
    _format_size = std::move(other._format_size);
    return *this;
}

client_logger::~client_logger() noexcept
{
    close_unused_streams();
}


void client_logger::close_unused_streams(){
    for (auto &[key, set] : _streams)
    {
        if (!--_counter[key].second)
        {
            _counter[key].first.close();
            _counter.erase(key);
        }
    }
}

void client_logger::format(
    std::string &to_format, std::string const &flag,
    std::string const &replace) const noexcept
{
    auto pos = 0;
    while ((pos = to_format.find(flag, pos)) != std::string::npos)
    {
        to_format.replace(pos, flag.size(), replace);
        pos += replace.size();
    }
}

logger const *client_logger::log(
    const std::string &text,
    logger::severity severity) const noexcept
{
    std::string datetime = current_datetime_to_string();
    auto separator = datetime.find(' ');
    std::string date = datetime.substr(0, separator);
    std::string time = datetime.substr(separator);

    for (auto &[key, set] : _streams) 
    {
        if (set.find(severity) == set.end()) 
        {
            continue;
        }
        
        std::string message = _format;
        format(message, "%d", date);
        format(message, "%t", time);
        format(message, "%s", severity_to_string(severity));
        format(message, "%m", text);

        if (_counter[key].first.is_open())
        {
            _counter[key].first << message;
        }
        else
        {
            std::cout << message;
        }
    }
    return this;
}