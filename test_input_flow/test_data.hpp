#ifndef TEST_INPUT_FLOW_TEST_DATA_HPP_INCLUDED
#   define TEST_INPUT_FLOW_TEST_DATA_HPP_INCLUDED

#   include <iosfwd>
#   include <vector>
#   include <string>
#   include <cstdint>


struct TestData
{
    using Line = std::vector<std::uint8_t>;

    std::int32_t argc() const { return (std::int32_t)argv_.size(); }
    char** argv() const { return argv_.empty() ? nullptr : (char**)argv_.data(); }

    bool done() const { return current_line_ == lines_.size(); }
    std::vector<Line> const& lines() const { return lines_; }
    std::size_t current_line() const { return current_line_; }
    std::size_t num_bytes_read() const { return num_bytes_read_; }

    bool on_input(std::uint8_t* ptr, std::size_t count);

    void set_args(std::vector<std::string> const& args);
    void push_back_line(Line const& line) { lines_.push_back(line); }

private:
    std::vector<char*> argv_{};
    std::vector<std::string> argv_strings{};
    std::vector<Line> lines_{};
    std::size_t current_line_{ 0ULL };
    std::size_t num_bytes_read_{ 0ULL };
};


std::istream& operator>>(std::istream& istr, TestData& data);


#endif
