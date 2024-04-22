#ifndef TEST_INTERPRETATION_TEST_DATA_HPP_INCLUDED
#   define TEST_INTERPRETATION_TEST_DATA_HPP_INCLUDED

#   include <sala/exec_state.hpp>
#   include <iosfwd>
#   include <vector>
#   include <cstdint>


struct TestData
{
    struct Line
    {
        bool in{ false };
        std::uint32_t id{ 0U };
        std::vector<std::uint8_t> bytes{};
    };

    struct Terminator
    {
        std::uint32_t id{ 0U };
        sala::ExecState::Termination termination{ sala::ExecState::Termination::UNKNOWN };
        int exit_code{ 0 };
    };

    bool done() const { return current_line_ == lines_.size(); }
    std::vector<Line> const& lines() const { return lines_; }
    std::size_t current_line() const { return current_line_; }

    bool on_input(std::uint32_t id, std::uint8_t* ptr, std::size_t count);
    bool on_output(std::uint32_t id, std::uint8_t const* ptr, std::size_t count);
    bool on_hit(std::uint32_t id) { return on_output(id, nullptr, 0ULL); }
    bool on_termination(std::uint32_t id, sala::ExecState::Termination termination, int exit_code);

    void push_back_line(Line const& line) { lines_.push_back(line); }
    void set_termination(std::uint32_t id, sala::ExecState::Termination termination, int exit_code);

private:
    std::vector<Line> lines_{};
    std::size_t current_line_{ 0ULL };
    Terminator terminator_{};
};


std::istream& operator>>(std::istream& istr, TestData& data);


#endif
