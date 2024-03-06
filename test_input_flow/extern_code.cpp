#include <test_input_flow/extern_code.hpp>
#include <utility/timeprof.hpp>
#include <utility/assumptions.hpp>
#include <utility/invariants.hpp>
#include <utility/development.hpp>
#include <string>


ExternCode::ExternCode(sala::ExecState* const state, TestData* data, sala::InputFlow* input_flow)
    : sala::ExternCodeCStd{ state }
    , data_{ data }
    , input_flow_{ input_flow }
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
    register_code("__sala_testing_flow_equal", [this]() { this->test_equal(); });
    register_code("__sala_testing_flow_comprises", [this]() { this->test_comprises(); });
    register_code("__sala_testing_flow_not_comprises", [this]() { this->test_not_comprises(); });
    register_code("__sala_testing_flow_comprises_join", [this]() { this->test_comprises_join(); });
    register_code("__sala_testing_flow_not_comprises_join", [this]() { this->test_not_comprises_join(); });
    register_code("__sala_testing_flow_none", [this]() { this->test_none(); });
}


void ExternCode::read(std::size_t const count)
{
    std::size_t desc{ data_->num_bytes_read() };
    sala::MemPtr ptr{ parameters().front().as<sala::MemPtr>() };

    if (!data_->on_input(ptr, count))
    {
        state().set_stage(sala::ExecState::Stage::FINISHED);
        state().set_termination(
            sala::ExecState::Termination::ERROR,
            "test_input_flow[extern_code]",
            "Unexpected read w.r.t. test data at line " + std::to_string(data_->current_line())
            );
    }
    for (std::size_t i = 0ULL; i != count; ++i, ++desc)
        input_flow_->start(ptr + i, (sala::InputFlow::InputDescriptor)(desc + i));
}


void ExternCode::test_equal()
{
    std::uint32_t id{ parameters().front().as<std::uint32_t>() };
    sala::MemPtr tst{ parameters().at(1).as<sala::MemPtr>() };
    sala::MemPtr src{ parameters().at(2).as<sala::MemPtr>() };
    std::uint32_t count{ parameters().at(3).as<std::uint32_t>() };
    for (std::uint32_t i = 0; i != count; ++i)
        if (*input_flow_->read(tst + i) != *input_flow_->read(src + i))
        {
            set_termination("equal", id, i);
            return;
        }
}


void ExternCode::test_comprises()
{
    std::uint32_t id{ parameters().front().as<std::uint32_t>() };
    sala::MemPtr tst{ parameters().at(1).as<sala::MemPtr>() };
    sala::MemPtr src{ parameters().at(2).as<sala::MemPtr>() };
    std::uint32_t count{ parameters().at(3).as<std::uint32_t>() };
    for (std::uint32_t i = 0; i != count; ++i)
        if (!input_flow_->read(tst + i)->comprises(*input_flow_->read(src + i)))
        {
            set_termination("comprises", id, i);
            return;
        }
}


void ExternCode::test_not_comprises()
{
    std::uint32_t id{ parameters().front().as<std::uint32_t>() };
    sala::MemPtr tst{ parameters().at(1).as<sala::MemPtr>() };
    sala::MemPtr src{ parameters().at(2).as<sala::MemPtr>() };
    std::uint32_t count{ parameters().at(3).as<std::uint32_t>() };
    for (std::uint32_t i = 0; i != count; ++i)
        if (input_flow_->read(tst + i)->comprises(*input_flow_->read(src + i)))
        {
            set_termination("not comprises", id, i);
            return;
        }
}


void ExternCode::test_comprises_join()
{
    std::uint32_t id{ parameters().front().as<std::uint32_t>() };
    sala::MemPtr tst{ parameters().at(1).as<sala::MemPtr>() };
    std::uint32_t tst_count{ parameters().at(2).as<std::uint32_t>() };
    sala::MemPtr src{ parameters().at(3).as<sala::MemPtr>() };
    std::uint32_t src_count{ parameters().at(4).as<std::uint32_t>() };

    sala::InputFlow::FlowSet src_joined_flow{};
    for (std::uint32_t i = 0; i != src_count; ++i)
        src_joined_flow.join(*input_flow_->read(src + i));

    for (std::uint32_t i = 0; i != tst_count; ++i)
        if (!input_flow_->read(tst + i)->comprises(src_joined_flow))
        {
            set_termination("comprises_join", id, i);
            return;
        }
}


void ExternCode::test_not_comprises_join()
{
    std::uint32_t id{ parameters().front().as<std::uint32_t>() };
    sala::MemPtr tst{ parameters().at(1).as<sala::MemPtr>() };
    std::uint32_t tst_count{ parameters().at(2).as<std::uint32_t>() };
    sala::MemPtr src{ parameters().at(3).as<sala::MemPtr>() };
    std::uint32_t src_count{ parameters().at(4).as<std::uint32_t>() };

    sala::InputFlow::FlowSet src_joined_flow{};
    for (std::uint32_t i = 0; i != src_count; ++i)
        src_joined_flow.join(*input_flow_->read(src + i));

    for (std::uint32_t i = 0; i != tst_count; ++i)
        if (input_flow_->read(tst + i)->comprises(src_joined_flow))
        {
            set_termination("comprises_join", id, i);
            return;
        }
}


void ExternCode::test_none()
{
    std::uint32_t id{ parameters().front().as<std::uint32_t>() };
    sala::MemPtr tst{ parameters().at(1).as<sala::MemPtr>() };
    std::uint32_t count{ parameters().at(2).as<std::uint32_t>() };
    for (std::uint32_t i = 0; i != count; ++i)
        if (!input_flow_->read(tst + i)->empty())
        {
            set_termination("none", id, i);
            return;
        }
}


void ExternCode::set_termination(std::string const& method, std::uint32_t id, std::uint32_t i)
{
    state().set_stage(sala::ExecState::Stage::FINISHED);
    state().set_exit_code(id);
    state().set_termination(
        sala::ExecState::Termination::ERROR,
        "test_input_flow[extern_code]",
        "Input flow '" + method + "' check FAILED at source code line " + std::to_string(id) + " at byte index " + std::to_string(i) + "."
        );
}
