#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>


static FILE* open_file()
{
    return fopen("./test_builder_for_interpretation.txt", "a");
}


static void save_test_line(char const type, int32_t const id, uint8_t const* const ptr, uint32_t const n)
{
    FILE* const f = open_file();
    fputc(type, f);
    fprintf(f, " %u ", id);
    for (uint32_t i = 0; i != n; ++i)
        fprintf(f, "%02x", ptr[i]);
    fputc('\n', f);
    fclose(f);
}


void __sala_testing_terminate(int32_t id, char const type, int32_t value)
{
    save_test_line(type, id, (uint8_t const*)&value, sizeof(value));
}


void __sala_testing_loc_hit(int32_t id)
{
    save_test_line('o', id, NULL, 0U);
}


_Bool __sala_testing_read_bool(int32_t id)
{
    _Bool value;
    save_test_line('i', id, (uint8_t const*)&value, sizeof(value));
    return value;
}


int8_t __sala_testing_read_s8(int32_t id)
{
    int8_t value;
    save_test_line('i', id, (uint8_t const*)&value, sizeof(value));
    return value;
}


int16_t __sala_testing_read_s16(int32_t id)
{
    int16_t value;
    save_test_line('i', id, (uint8_t const*)&value, sizeof(value));
    return value;
}


int32_t __sala_testing_read_s32(int32_t id)
{
    int32_t value;
    save_test_line('i', id, (uint8_t const*)&value, sizeof(value));
    return value;
}


int64_t __sala_testing_read_s64(int32_t id)
{
    int64_t value;
    save_test_line('i', id, (uint8_t const*)&value, sizeof(value));
    return value;
}


uint8_t __sala_testing_read_u8(int32_t id)
{
    uint8_t value;
    save_test_line('i', id, (uint8_t const*)&value, sizeof(value));
    return value;
}


uint16_t __sala_testing_read_u16(int32_t id)
{
    uint16_t value;
    save_test_line('i', id, (uint8_t const*)&value, sizeof(value));
    return value;
}


uint32_t __sala_testing_read_u32(int32_t id)
{
    uint32_t value;
    save_test_line('i', id, (uint8_t const*)&value, sizeof(value));
    return value;
}


uint64_t __sala_testing_read_u64(int32_t id)
{
    uint64_t value;
    save_test_line('i', id, (uint8_t const*)&value, sizeof(value));
    return value;
}


float __sala_testing_read_f32(int32_t id)
{
    float value;
    save_test_line('i', id, (uint8_t const*)&value, sizeof(value));
    return value;
}


double __sala_testing_read_f64(int32_t id)
{
    double value;
    save_test_line('i', id, (uint8_t const*)&value, sizeof(value));
    return value;
}


void __sala_testing_write_bool(int32_t id, _Bool value)
{
    save_test_line('o', id, (uint8_t const*)&value, sizeof(value));
}


void __sala_testing_write_s8(int32_t id, int8_t value)
{
    save_test_line('o', id, (uint8_t const*)&value, sizeof(value));
}


void __sala_testing_write_s16(int32_t id, int16_t value)
{
    save_test_line('o', id, (uint8_t const*)&value, sizeof(value));
}


void __sala_testing_write_s32(int32_t id, int32_t value)
{
    save_test_line('o', id, (uint8_t const*)&value, sizeof(value));
}


void __sala_testing_write_s64(int32_t id, int64_t value)
{
    save_test_line('o', id, (uint8_t const*)&value, sizeof(value));
}


void __sala_testing_write_u8(int32_t id, uint8_t value)
{
    save_test_line('o', id, (uint8_t const*)&value, sizeof(value));
}


void __sala_testing_write_u16(int32_t id, uint16_t value)
{
    save_test_line('o', id, (uint8_t const*)&value, sizeof(value));
}


void __sala_testing_write_u32(int32_t id, uint32_t value)
{
    save_test_line('o', id, (uint8_t const*)&value, sizeof(value));
}


void __sala_testing_write_u64(int32_t id, uint64_t value)
{
    save_test_line('o', id, (uint8_t const*)&value, sizeof(value));
}


void __sala_testing_write_f32(int32_t id, float value)
{
    save_test_line('o', id, (uint8_t const*)&value, sizeof(value));
}


void __sala_testing_write_f64(int32_t id, double value)
{
    save_test_line('o', id, (uint8_t const*)&value, sizeof(value));
}


void __sala_testing_write_args(int32_t argc, ...)
{
    if (argc < 1)
        return;
    FILE* const f = open_file();
    va_list args;
    va_start(args, argc);
    for (int i = 0; i < argc; ++i)
    {
        fputs("arg:", f);
        fputs(va_arg(args, char*), f);
        fputc('\n', f);
    }
    fclose(f);
}
