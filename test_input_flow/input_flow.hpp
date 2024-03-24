#ifndef TEST_INPUT_FLOW_INPUT_FLOW_HPP_INCLUDED
#   define TEST_INPUT_FLOW_INPUT_FLOW_HPP_INCLUDED

#   include <sala/input_flow.hpp>
#   include <test_input_flow/test_data.hpp>


struct InputFlow : public sala::InputFlow
{
    InputFlow(sala::ExecState* state, TestData* data);

private:
    void register_external_functions();

    void start_input_flow(std::size_t const count);
    void test_equal();
    void test_comprises();
    void test_not_comprises();
    void test_comprises_join();
    void test_not_comprises_join();
    void test_none();

    void set_termination(std::string const& method, std::uint32_t id, std::uint32_t i);

    TestData* data_;
};


#endif
