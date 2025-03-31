#include <test_interpretation/test_data.hpp>
#include <utility/assumptions.hpp>
#include <utility/invariants.hpp>
#include <istream>
#include <sstream>
#include <iterator>


bool TestData::on_input(std::uint32_t const id, std::uint8_t* const ptr, std::size_t const count)
{
    if (done())
        return false;

    Line const& line = lines_.at(current_line_);
    if (!line.in)
        return false;
    if (line.id != id)
        return false;
    if (line.bytes.size() != count)
        return false;

    for (std::size_t i = 0ULL; i != count; ++i)
        ptr[i] = line.bytes.at(i);

    ++current_line_;

    return true;
}


bool TestData::on_output(std::uint32_t const id, std::uint8_t const* const ptr, std::size_t const count)
{
    if (done())
        return false;

    Line const& line = lines_.at(current_line_);
    if (line.in)
        return false;
    if (line.id != id)
        return false;
    if (line.bytes.size() != count)
        return false;

    for (std::size_t i = 0ULL; i != count; ++i)
        if (line.bytes.at(i) != ptr[i])
            return false;

    ++current_line_;

    return true;
}


bool TestData::on_termination(std::uint32_t const id, sala::ExecState::Termination const termination, int const exit_code)
{
    return terminator_.id == id && terminator_.termination == termination && terminator_.exit_code == exit_code;
}


void TestData::set_args(std::vector<std::string> const& args)
{
    argv_strings = args;
    for (std::string& s : argv_strings)
        argv_.push_back(s.data());
}


void TestData::set_termination(std::uint32_t const id, sala::ExecState::Termination const termination, int const exit_code)
{
    terminator_ = { id, termination, exit_code };
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

        std::size_t idx{ 0ULL };

        char type = line.at(idx);

        for (++idx; idx < line.size() && !std::isalnum(line.at(idx)); ++idx);

        std::uint32_t id;
        {
            std::stringstream sstr;
            for ( ; idx < line.size() && std::isalnum(line.at(idx)); ++idx)
                sstr << line.at(idx);
            sstr >> id;
        }

        for (++idx; idx < line.size() && !std::isalnum(line.at(idx)); ++idx);

        std::vector<std::uint8_t> bytes;
        for ( ; idx < line.size() && std::isalnum(line.at(idx)) && std::isalnum(line.at(idx + 1ULL)); idx += 2ULL)
        {
            std::stringstream sstr;
            sstr << std::hex << line.at(idx) << std::hex << line.at(idx + 1ULL);
            std::uint32_t value;
            sstr >> value;
            INVARIANT(value < 256U);
            bytes.push_back((std::uint8_t)value);
        }

        struct local
        {
            static int bytes_to_int(std::vector<std::uint8_t> const& bytes)
            {
                switch (bytes.size())
                {
                case 1ULL: return (int)*(std::int8_t const*)bytes.data();
                case 2ULL: return (int)*(std::int16_t const*)bytes.data();
                case 4ULL: return (int)*(std::int32_t const*)bytes.data();
                case 8ULL: return (int)*(std::int64_t const*)bytes.data();
                default: UNREACHABLE(); return 0;
                }
            }
        };

        switch (type)
        {
            case 'i': data.push_back_line({ true, id, bytes }); break;
            case 'o': data.push_back_line({ false, id, bytes }); break;
            case 'n': data.set_termination(id, sala::ExecState::Termination::NORMAL, local::bytes_to_int(bytes)); break;
            case 'e': data.set_termination(id, sala::ExecState::Termination::ERROR, local::bytes_to_int(bytes)); break;
            case 'c': data.set_termination(id, sala::ExecState::Termination::CRASH, local::bytes_to_int(bytes)); break;
            default: UNREACHABLE(); break;
        }
    }

    if (!args.empty())
        data.set_args(args);

    return istr;
}
