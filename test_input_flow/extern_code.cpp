#include <test_input_flow/extern_code.hpp>
#include <utility/timeprof.hpp>
#include <utility/assumptions.hpp>
#include <utility/invariants.hpp>
#include <utility/development.hpp>


ExternCode::ExternCode(sala::ExecState* const state, sala::Sanitizer* const sanitizer, TestData* data)
    : sala::ExternCodeCStd{ state, sanitizer }
    , data_{ data }
{
    register_code("__sala_testing_start_flow_with_bool", [this]() { this->read(sizeof(bool)); });
    register_code("__sala_testing_start_flow_with_s8", [this]() { this->read(sizeof(std::int8_t)); });
    register_code("__sala_testing_start_flow_with_s16", [this]() { this->read(sizeof(std::int16_t)); });
    register_code("__sala_testing_start_flow_with_s32", [this]() { this->read(sizeof(std::int32_t)); });
    register_code("__sala_testing_start_flow_with_s64", [this]() { this->read(sizeof(std::int64_t)); });
    register_code("__sala_testing_start_flow_with_u8", [this]() { this->read(sizeof(std::uint8_t)); });
    register_code("__sala_testing_start_flow_with_u16", [this]() { this->read(sizeof(std::uint16_t)); });
    register_code("__sala_testing_start_flow_with_u32", [this]() { this->read(sizeof(std::uint32_t)); });
    register_code("__sala_testing_start_flow_with_u64", [this]() { this->read(sizeof(std::uint64_t)); });
    register_code("__sala_testing_start_flow_with_f32", [this]() { this->read(sizeof(float)); });
    register_code("__sala_testing_start_flow_with_f64", [this]() { this->read(sizeof(double)); });
}


void ExternCode::read(std::size_t const count)
{
    sala::MemPtr ptr{ parameters().front().read<sala::MemPtr>() };
    if (!data_->on_input(ptr, count))
    {
        state().set_stage(sala::ExecState::Stage::FINISHED);
        state().set_termination(
            sala::ExecState::Termination::ERROR,
            "test_input_flow[ExternCode]",
            "Unexpected read w.r.t. test data at line " + std::to_string(data_->current_line())
            );
    }
}
