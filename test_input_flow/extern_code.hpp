#ifndef TEST_INPUT_FLOW_EXTERNAL_CODE_HPP_INCLUDED
#   define TEST_INPUT_FLOW_EXTERNAL_CODE_HPP_INCLUDED

#   include <test_input_flow/test_data.hpp>
#   include <sala/extern_code_cstd.hpp>
#   include <sala/input_flow.hpp>


struct ExternCode : public sala::ExternCodeCStd
{
    ExternCode(sala::ExecState* state, TestData* data, sala::InputFlow* input_flow);

    TestData& data() { return *data_; }

private:
    void read(std::size_t count);
    void test_equal();
    void test_comprises();
    void test_not_comprises();
    void test_comprises_join();
    void test_not_comprises_join();
    void test_none();

    void set_termination(std::string const& method, std::uint32_t id, std::uint32_t i);

    TestData* data_;
    sala::InputFlow* input_flow_;
};


#endif
