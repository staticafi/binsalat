#include <test_input_flow/program_info.hpp>
#include <test_input_flow/program_options.hpp>
#include <test_input_flow/extern_code.hpp>
#include <test_input_flow/input_flow.hpp>
#include <test_input_flow/test_data.hpp>
#include <sala/program.hpp>
#include <sala/exec_state.hpp>
#include <sala/interpreter.hpp>
#include <sala/sanitizer.hpp>
#include <sala/streaming.hpp>
#include <utility/timeprof.hpp>
#include <iostream>
#include <fstream>
#include <filesystem>

int run(int argc, char* argv[])
{
    TMPROF_BLOCK();

    if (get_program_options()->has("help"))
    {
        std::cout << get_program_options() << std::endl;
        return -1;
    }
    if (get_program_options()->has("version"))
    {
        std::cout << get_program_options()->value("version") << std::endl;
        return -2;
    }
    if (!get_program_options()->has("input"))
    {
        std::cout << "The input file was not specified." << std::endl;
        return -3;
    }
    if (!get_program_options()->has("data"))
    {
        std::cout << "The I/O test data file was not specified." << std::endl;
        return -4;
    }

    sala::Program P;
    {
        std::filesystem::path input_json_file{ get_program_options()->value("input") };
        std::ifstream istr(input_json_file.c_str(), std::ios_base::binary);
        istr >> P;

        if (P.num_cpu_bits() != 64U && P.num_cpu_bits() != 32U)
        {
            std::cout << "Only 64-bit and 32-bit platforms are supported. The program is for " << P.num_cpu_bits() << "-bit platform." << std::endl;
            return -5;
        }
        if (P.system().find("linux") == std::string::npos)
        {
            std::cout << "Only Linux platform is supported. The program is for " << P.system() << " platform." << std::endl;
            return -6;
        }
    }

    TestData D;
    {
        std::filesystem::path data_txt_file{ get_program_options()->value("data") };
        std::ifstream istr(data_txt_file.c_str(), std::ios_base::binary);
        istr >> D;
    }

    sala::ExecState state{ &P, D.argc(), D.argv() };
    sala::Sanitizer sanitizer{ &state };
    ExternCode externals{ &state, &sanitizer, &D };
    InputFlow input_flow{ &state, &D };
    sala::Interpreter interpreter{ &state, &externals, { &sanitizer, &input_flow } };

    while (!interpreter.done())
        interpreter.step();

    return D.done() ? state.exit_code() : 1 + (int)D.current_line();
}
