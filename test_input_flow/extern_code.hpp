#ifndef TEST_INPUT_FLOW_EXTERNAL_CODE_HPP_INCLUDED
#   define TEST_INPUT_FLOW_EXTERNAL_CODE_HPP_INCLUDED

#   include <test_input_flow/test_data.hpp>
#   include <sala/extern_code_cstd.hpp>


struct ExternCode : public sala::ExternCodeCStd
{
    ExternCode(sala::ExecState* state, sala::Sanitizer* const sanitizer, TestData* data);

    TestData& data() { return *data_; }

private:
    void read(std::size_t count);

    TestData* data_;
};


#endif
