#include <test_interpretation/extern_code.hpp>
#include <utility/timeprof.hpp>
#include <utility/assumptions.hpp>
#include <utility/invariants.hpp>
#include <utility/development.hpp>
#include <string>
#include <cstring>


ExternCode::ExternCode(sala::ExecState* const state, TestData* data)
    : sala::ExternCodeCStd{ state }
    , data_{ data }
{
    register_code("__sala_testing_loc_hit", [this]() {
        if (!this->data_->on_hit(parameters().back().read<std::uint32_t>()))
        {
            this->state().set_stage(sala::ExecState::Stage::FINISHED);
            this->state().set_termination(
                sala::ExecState::Termination::ERROR,
                "test_interpreter[extern_code]",
                "Unexpected hit w.r.t. test data at line " + std::to_string(data_->current_line()),
                this->get_call_instruction()
                );
        }
    });
    register_code("__sala_testing_main_return", [this]() {
        std::memcpy(parameters().front().read<sala::MemPtr>(), parameters().back().start(), parameters().back().count());
        this->state().set_termination(
            sala::ExecState::Termination::NORMAL,
            "test_interpreter[extern_code]",
            "Returning from main.",
            this->get_call_instruction()
            );
    });
    register_code("__sala_testing_crash", [this]() {
        this->state().set_stage(sala::ExecState::Stage::FINISHED);
        this->state().set_exit_code(parameters().back().read<std::uint32_t>());
        this->state().set_termination(
            sala::ExecState::Termination::CRASH,
            "test_interpreter[extern_code]",
            "Crashing the program.",
            this->get_call_instruction()
            );
    });
    register_code("__sala_testing_read_bool", [this]() { this->read(sizeof(bool)); });
    register_code("__sala_testing_read_s8", [this]() { this->read(sizeof(std::int8_t)); });
    register_code("__sala_testing_read_s16", [this]() { this->read(sizeof(std::int16_t)); });
    register_code("__sala_testing_read_s32", [this]() { this->read(sizeof(std::int32_t)); });
    register_code("__sala_testing_read_s64", [this]() { this->read(sizeof(std::int64_t)); });
    register_code("__sala_testing_read_u8", [this]() { this->read(sizeof(std::uint8_t)); });
    register_code("__sala_testing_read_u16", [this]() { this->read(sizeof(std::uint16_t)); });
    register_code("__sala_testing_read_u32", [this]() { this->read(sizeof(std::uint32_t)); });
    register_code("__sala_testing_read_u64", [this]() { this->read(sizeof(std::uint64_t)); });
    register_code("__sala_testing_read_f32", [this]() { this->read(sizeof(float)); });
    register_code("__sala_testing_read_f64", [this]() { this->read(sizeof(double)); });
    register_code("__sala_testing_write_bool", [this]() { this->write(sizeof(bool)); });
    register_code("__sala_testing_write_s8", [this]() { this->write(sizeof(std::int8_t)); });
    register_code("__sala_testing_write_s16", [this]() { this->write(sizeof(std::int16_t)); });
    register_code("__sala_testing_write_s32", [this]() { this->write(sizeof(std::int32_t)); });
    register_code("__sala_testing_write_s64", [this]() { this->write(sizeof(std::int64_t)); });
    register_code("__sala_testing_write_u8", [this]() { this->write(sizeof(std::uint8_t)); });
    register_code("__sala_testing_write_u16", [this]() { this->write(sizeof(std::uint16_t)); });
    register_code("__sala_testing_write_u32", [this]() { this->write(sizeof(std::uint32_t)); });
    register_code("__sala_testing_write_u64", [this]() { this->write(sizeof(std::uint64_t)); });
    register_code("__sala_testing_write_f32", [this]() { this->write(sizeof(float)); });
    register_code("__sala_testing_write_f64", [this]() { this->write(sizeof(double)); });
}


void ExternCode::read(std::size_t const count)
{
    if (!data_->on_input(
            parameters().back().read<std::uint32_t>(),
            parameters().front().read<sala::MemPtr>(),
            count))
    {
        state().set_stage(sala::ExecState::Stage::FINISHED);
        state().set_termination(
            sala::ExecState::Termination::ERROR,
            "test_interpreter[extern_code]",
            "Unexpected read w.r.t. test data at line " + std::to_string(data_->current_line()),
            this->get_call_instruction()
            );
    }
}


void ExternCode::write(std::size_t const count)
{
    if (!data_->on_output(
            parameters().front().read<std::uint32_t>(),
            parameters().back().start(),
            count))
    {
        state().set_stage(sala::ExecState::Stage::FINISHED);
        state().set_termination(
            sala::ExecState::Termination::ERROR,
            "test_interpreter[extern_code]",
            "Unexpected write w.r.t. test data at line " + std::to_string(data_->current_line()),
            this->get_call_instruction()
            );
    }
}
