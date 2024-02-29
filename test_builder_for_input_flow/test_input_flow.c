#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>


static void save_test_line(uint8_t const* const ptr, uint32_t const n)
{
    FILE* const f = fopen("./test_builder_for_input_flow.txt", "a");
    for (uint32_t i = 0; i != n; ++i)
        fprintf(f, "%02x", ptr[i]);
    fputc('\n', f);
    fclose(f);
}


_Bool __sala_testing_start_flow_with_bool(int32_t id)
{
    _Bool value;
    save_test_line((uint8_t const*)&value, sizeof(value));
    return value;
}


int8_t __sala_testing_start_flow_with_s8(int32_t id)
{
    int8_t value;
    save_test_line((uint8_t const*)&value, sizeof(value));
    return value;
}


int16_t __sala_testing_start_flow_with_s16(int32_t id)
{
    int16_t value;
    save_test_line((uint8_t const*)&value, sizeof(value));
    return value;
}


int32_t __sala_testing_start_flow_with_s32(int32_t id)
{
    int32_t value;
    save_test_line((uint8_t const*)&value, sizeof(value));
    return value;
}


int64_t __sala_testing_start_flow_with_s64(int32_t id)
{
    int64_t value;
    save_test_line((uint8_t const*)&value, sizeof(value));
    return value;
}


uint8_t __sala_testing_start_flow_with_u8(int32_t id)
{
    uint8_t value;
    save_test_line((uint8_t const*)&value, sizeof(value));
    return value;
}


uint16_t __sala_testing_start_flow_with_u16(int32_t id)
{
    uint16_t value;
    save_test_line((uint8_t const*)&value, sizeof(value));
    return value;
}


uint32_t __sala_testing_start_flow_with_u32(int32_t id)
{
    uint32_t value;
    save_test_line((uint8_t const*)&value, sizeof(value));
    return value;
}


uint64_t __sala_testing_start_flow_with_u64(int32_t id)
{
    uint64_t value;
    save_test_line((uint8_t const*)&value, sizeof(value));
    return value;
}


float __sala_testing_start_flow_with_f32(int32_t id)
{
    float value;
    save_test_line((uint8_t const*)&value, sizeof(value));
    return value;
}


double __sala_testing_start_flow_with_f64(int32_t id)
{
    double value;
    save_test_line((uint8_t const*)&value, sizeof(value));
    return value;
}


void __sala_testing_flow_equal(int32_t id, uint8_t const* tst, uint8_t const* src, uint32_t count)
{
    // Nothing to do.
}


void __sala_testing_flow_comprises(int32_t id, uint8_t const* tst, uint8_t const* src, uint32_t count)
{
    // Nothing to do.
}


void __sala_testing_flow_not_comprises(int32_t id, uint8_t const* tst, uint8_t const* src, uint32_t count)
{
    // Nothing to do.
}


void __sala_testing_flow_comprises_join(int32_t id, uint8_t const* tst, uint32_t tst_count, uint8_t const* src, uint32_t src_count)
{
    // Nothing to do.
}


void __sala_testing_flow_not_comprises_join(int32_t id, uint8_t const* tst, uint32_t tst_count, uint8_t const* src, uint32_t src_count)
{
    // Nothing to do.
}


void __sala_testing_flow_none(int32_t id, uint8_t const* tst, uint32_t count)
{
    // Nothing to do.
}
