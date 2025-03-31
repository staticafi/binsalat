#include <test_input_flow/test_data.hpp>
#include <utility/assumptions.hpp>
#include <utility/invariants.hpp>
#include <istream>
#include <sstream>
#include <iterator>


bool TestData::on_input(std::uint8_t* const ptr, std::size_t const count)
{
    if (done())
        return false;

    Line const& line = lines_.at(current_line_);
    if (line.size() != count)
        return false;

    for (std::size_t i = 0ULL; i != count; ++i)
        ptr[i] = line.at(i);

    ++current_line_;
    num_bytes_read_ += count;

    return true;
}


void TestData::set_args(std::vector<std::string> const& args)
{
    argv_strings = args;
    for (std::string& s : argv_strings)
        argv_.push_back(s.data());
}


std::istream& operator>>(std::istream& istr, TestData& data)
{
    std::vector<std::string> args;
    std::string line;
    while (std::getline(istr, line))
    {
        if (line.empty())
            continue;

        if (line.starts_with("arg:"))
        {
            args.push_back({ line.begin() + 4UL, line.end() });
            continue;
        }

        std::vector<std::uint8_t> bytes;
        for (std::size_t idx = 0ULL; idx < line.size() && std::isalnum(line.at(idx)) && std::isalnum(line.at(idx + 1ULL)); idx += 2ULL)
        {
            std::stringstream sstr;
            sstr << std::hex << line.at(idx) << std::hex << line.at(idx + 1ULL);
            std::uint32_t value;
            sstr >> value;
            INVARIANT(value < 256U);
            bytes.push_back((std::uint8_t)value);
        }

        data.push_back_line(bytes);
    }

    if (!args.empty())
        data.set_args(args);

    return istr;
}
