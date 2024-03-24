#include <test_input_flow/input_flow.hpp>
#include <utility/timeprof.hpp>
#include <utility/assumptions.hpp>
#include <utility/invariants.hpp>
#include <utility/development.hpp>
#include <string>


InputFlow::InputFlow(sala::ExecState* const state, TestData* data)
    : sala::InputFlow{ state }
    , data_{ data }
{
    REGISTER_EXTERN_FUNCTION_PROCESSOR(__sala_testing_start_flow_with_bool, this->start_input_flow(sizeof(bool)) );
    REGISTER_EXTERN_FUNCTION_PROCESSOR(__sala_testing_start_flow_with_s8, this->start_input_flow(sizeof(std::int8_t)) );
    REGISTER_EXTERN_FUNCTION_PROCESSOR(__sala_testing_start_flow_with_s16, this->start_input_flow(sizeof(std::int16_t)) );
    REGISTER_EXTERN_FUNCTION_PROCESSOR(__sala_testing_start_flow_with_s32, this->start_input_flow(sizeof(std::int32_t)) );
    REGISTER_EXTERN_FUNCTION_PROCESSOR(__sala_testing_start_flow_with_s64, this->start_input_flow(sizeof(std::int64_t)) );
    REGISTER_EXTERN_FUNCTION_PROCESSOR(__sala_testing_start_flow_with_u8, this->start_input_flow(sizeof(std::uint8_t)) );
    REGISTER_EXTERN_FUNCTION_PROCESSOR(__sala_testing_start_flow_with_u16, this->start_input_flow(sizeof(std::uint16_t)) );
    REGISTER_EXTERN_FUNCTION_PROCESSOR(__sala_testing_start_flow_with_u32, this->start_input_flow(sizeof(std::uint32_t)) );
    REGISTER_EXTERN_FUNCTION_PROCESSOR(__sala_testing_start_flow_with_u64, this->start_input_flow(sizeof(std::uint64_t)) );
    REGISTER_EXTERN_FUNCTION_PROCESSOR(__sala_testing_start_flow_with_f32, this->start_input_flow(sizeof(float)) );
    REGISTER_EXTERN_FUNCTION_PROCESSOR(__sala_testing_start_flow_with_f64, this->start_input_flow(sizeof(double)) );
    REGISTER_EXTERN_FUNCTION_PROCESSOR(__sala_testing_flow_equal, this->test_equal() );
    REGISTER_EXTERN_FUNCTION_PROCESSOR(__sala_testing_flow_comprises, this->test_comprises() );
    REGISTER_EXTERN_FUNCTION_PROCESSOR(__sala_testing_flow_not_comprises, this->test_not_comprises() );
    REGISTER_EXTERN_FUNCTION_PROCESSOR(__sala_testing_flow_comprises_join, this->test_comprises_join() );
    REGISTER_EXTERN_FUNCTION_PROCESSOR(__sala_testing_flow_not_comprises_join, this->test_not_comprises_join() );
    REGISTER_EXTERN_FUNCTION_PROCESSOR(__sala_testing_flow_none, this->test_none() );
}


void InputFlow::start_input_flow(std::size_t const count)
{
    std::size_t desc{ data_->num_bytes_read() - count };
    sala::MemPtr ptr{ parameters().front().as<sala::MemPtr>() };
    for (std::size_t i = 0ULL; i != count; ++i, ++desc)
        start(ptr + i, (sala::InputFlow::InputDescriptor)desc);
}


void InputFlow::test_equal()
{
    std::uint32_t id{ parameters().front().as<std::uint32_t>() };
    sala::MemPtr tst{ parameters().at(1).as<sala::MemPtr>() };
    sala::MemPtr src{ parameters().at(2).as<sala::MemPtr>() };
    std::uint32_t count{ parameters().at(3).as<std::uint32_t>() };
    for (std::uint32_t i = 0; i != count; ++i)
        if (*read(tst + i) != *read(src + i))
        {
            set_termination("equal", id, i);
            return;
        }
}


void InputFlow::test_comprises()
{
    std::uint32_t id{ parameters().front().as<std::uint32_t>() };
    sala::MemPtr tst{ parameters().at(1).as<sala::MemPtr>() };
    sala::MemPtr src{ parameters().at(2).as<sala::MemPtr>() };
    std::uint32_t count{ parameters().at(3).as<std::uint32_t>() };
    for (std::uint32_t i = 0; i != count; ++i)
        if (!read(tst + i)->comprises(*read(src + i)))
        {
            set_termination("comprises", id, i);
            return;
        }
}


void InputFlow::test_not_comprises()
{
    std::uint32_t id{ parameters().front().as<std::uint32_t>() };
    sala::MemPtr tst{ parameters().at(1).as<sala::MemPtr>() };
    sala::MemPtr src{ parameters().at(2).as<sala::MemPtr>() };
    std::uint32_t count{ parameters().at(3).as<std::uint32_t>() };
    for (std::uint32_t i = 0; i != count; ++i)
        if (read(tst + i)->comprises(*read(src + i)))
        {
            set_termination("not comprises", id, i);
            return;
        }
}


void InputFlow::test_comprises_join()
{
    std::uint32_t id{ parameters().front().as<std::uint32_t>() };
    sala::MemPtr tst{ parameters().at(1).as<sala::MemPtr>() };
    std::uint32_t tst_count{ parameters().at(2).as<std::uint32_t>() };
    sala::MemPtr src{ parameters().at(3).as<sala::MemPtr>() };
    std::uint32_t src_count{ parameters().at(4).as<std::uint32_t>() };

    sala::InputFlow::FlowSet src_joined_flow{};
    for (std::uint32_t i = 0; i != src_count; ++i)
        src_joined_flow.join(*read(src + i));

    for (std::uint32_t i = 0; i != tst_count; ++i)
        if (!read(tst + i)->comprises(src_joined_flow))
        {
            set_termination("comprises_join", id, i);
            return;
        }
}


void InputFlow::test_not_comprises_join()
{
    std::uint32_t id{ parameters().front().as<std::uint32_t>() };
    sala::MemPtr tst{ parameters().at(1).as<sala::MemPtr>() };
    std::uint32_t tst_count{ parameters().at(2).as<std::uint32_t>() };
    sala::MemPtr src{ parameters().at(3).as<sala::MemPtr>() };
    std::uint32_t src_count{ parameters().at(4).as<std::uint32_t>() };

    sala::InputFlow::FlowSet src_joined_flow{};
    for (std::uint32_t i = 0; i != src_count; ++i)
        src_joined_flow.join(*read(src + i));

    for (std::uint32_t i = 0; i != tst_count; ++i)
        if (read(tst + i)->comprises(src_joined_flow))
        {
            set_termination("comprises_join", id, i);
            return;
        }
}


void InputFlow::test_none()
{
    std::uint32_t id{ parameters().front().as<std::uint32_t>() };
    sala::MemPtr tst{ parameters().at(1).as<sala::MemPtr>() };
    std::uint32_t count{ parameters().at(2).as<std::uint32_t>() };
    for (std::uint32_t i = 0; i != count; ++i)
        if (!read(tst + i)->empty())
        {
            set_termination("none", id, i);
            return;
        }
}


void InputFlow::set_termination(std::string const& method, std::uint32_t id, std::uint32_t i)
{
    state().set_stage(sala::ExecState::Stage::FINISHED);
    state().set_exit_code(id);
    state().set_termination(
        sala::ExecState::Termination::ERROR,
        "test_input_flow[InputFlow]",
        "Input flow '" + method + "' check FAILED at source code line " + std::to_string(id) + " at byte index " + std::to_string(i) + "."
        );
}
