#include <stdint.h>
#include <stdbool.h>

#define __VERIFIER_nondet_char() __sala_testing_start_flow_with_s8()
#define __VERIFIER_nondet_short() __sala_testing_start_flow_with_s16()
#define __VERIFIER_nondet_int() __sala_testing_start_flow_with_s32()
#define __VERIFIER_nondet_long() __sala_testing_start_flow_with_s32()
#define __VERIFIER_nondet_longlong() __sala_testing_start_flow_with_s64()
#define __VERIFIER_nondet_uchar() __sala_testing_start_flow_with_u8()
#define __VERIFIER_nondet_ushort() __sala_testing_start_flow_with_u16()
#define __VERIFIER_nondet_uint() __sala_testing_start_flow_with_u32()
#define __VERIFIER_nondet_ulong() __sala_testing_start_flow_with_u32()
#define __VERIFIER_nondet_ulonglong() __sala_testing_start_flow_with_u64()
#define __VERIFIER_nondet_float() __sala_testing_start_flow_with_f32()
#define __VERIFIER_nondet_double() __sala_testing_start_flow_with_f64()

#define FLOW_EQUAL_EX(T, S, N) __sala_testing_flow_comprises(__LINE__, (uint8_t const*)(T), (uint8_t const*)&(S), N)
#define FLOW_COMPRISES_EX(T, S, N) __sala_testing_flow_comprises(__LINE__, (uint8_t const*)(T), (uint8_t const*)&(S), N)
#define FLOW_NOT_COMPRISES_EX(T, S, N) __sala_testing_flow_not_comprises(__LINE__, (uint8_t const*)(T), (uint8_t const*)(S), N)
#define FLOW_COMPRISES_JOIN_EX(T, M, S, N) __sala_testing_flow_comprises_join(__LINE__, (uint8_t const*)(T), M, (uint8_t const*)(S), N)
#define FLOW_NOT_COMPRISES_JOIN_EX(T, M, S, N) __sala_testing_flow_not_comprises_join(__LINE__, (uint8_t const*)(T), M, (uint8_t const*)(S), N)
#define FLOW_NONE_EX(T, N) __sala_testing_flow_none(__LINE__, (uint8_t const*)(T), N)

#define FLOW_EQUAL(V0, V1) FLOW_EQUAL_EX(&(V0), &(V1), sizeof(V0))
#define FLOW_COMPRISES(V0, V1) FLOW_COMPRISES_EX(&(V0), &(V1), sizeof(V0))
#define FLOW_NOT_COMPRISES(V0, V1) FLOW_NOT_COMPRISES_EX(&(V0), &(V1), sizeof(V0))
#define FLOW_COMPRISES_JOIN(V0, V1) FLOW_COMPRISES_JOIN_EX(&(V0), sizeof(V0), &(V1), sizeof(V1))
#define FLOW_NOT_COMPRISES_JOIN(V0, V1) FLOW_NOT_COMPRISES_JOIN_EX(&(V0), sizeof(V0), &(V1), sizeof(V1))
#define FLOW_NONE(V0) FLOW_NONE_EX(&(V0), sizeof(V0))

extern _Bool __sala_testing_start_flow_with_bool();
extern int8_t __sala_testing_start_flow_with_s8();
extern int16_t __sala_testing_start_flow_with_s16();
extern int32_t __sala_testing_start_flow_with_s32();
extern int64_t __sala_testing_start_flow_with_s64();
extern uint8_t __sala_testing_start_flow_with_u8();
extern uint16_t __sala_testing_start_flow_with_u16();
extern uint32_t __sala_testing_start_flow_with_u32();
extern uint64_t __sala_testing_start_flow_with_u64();
extern float __sala_testing_start_flow_with_f32();
extern double __sala_testing_start_flow_with_f64();

extern void __sala_testing_flow_equal(int32_t id, uint8_t const* tst, uint8_t const* src, uint32_t count);
extern void __sala_testing_flow_comprises(int32_t id, uint8_t const* tst, uint8_t const* src, uint32_t count);
extern void __sala_testing_flow_not_comprises(int32_t id, uint8_t const* tst, uint8_t const* src, uint32_t count);
extern void __sala_testing_flow_comprises_join(int32_t id, uint8_t const* tst, uint32_t tst_count, uint8_t const* src, uint32_t src_count);
extern void __sala_testing_flow_not_comprises_join(int32_t id, uint8_t const* tst, uint32_t tst_count, uint8_t const* src, uint32_t src_count);
extern void __sala_testing_flow_none(int32_t id, uint8_t const* tst, uint32_t count);
