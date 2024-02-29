#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define READ(T) __sala_testing_read_##T(__LINE__)
#define WRITE(T,V) __sala_testing_write_##T(__LINE__, V)
#define HIT() __sala_testing_loc_hit(__LINE__)
#define RET(V) return __sala_testing_terminate(__LINE__, 'n', (int32_t)(V))

#define EXIT(V) do { __sala_testing_terminate(__LINE__, 'n', (int32_t)(V)); exit(V); } while(0)
#define ABORT() do { __sala_testing_terminate(__LINE__, 'e', 0); abort(); } while(0)
#define __ASSERT_FAIL(A,B,C,D) do { __sala_testing_terminate(__LINE__, 'e', 0); __assert_fail(A,B,C,D); } while(0)
#define CRASH(V) __sala_testing_terminate(__LINE__, 'c', V)
#define CRASHX(V) do { CRASH(V); abort(); } while(0)

#define __VERIFIER_nondet_char() READ(s8)
#define __VERIFIER_nondet_short() READ(s16)
#define __VERIFIER_nondet_int() READ(s32)
#define __VERIFIER_nondet_long() READ(s32)
#define __VERIFIER_nondet_longlong() READ(s64)
#define __VERIFIER_nondet_uchar() READ(u8)
#define __VERIFIER_nondet_ushort() READ(u16)
#define __VERIFIER_nondet_uint() READ(u32)
#define __VERIFIER_nondet_ulong() READ(u32)
#define __VERIFIER_nondet_ulonglong() READ(u64)
#define __VERIFIER_nondet_float() READ(f32)
#define __VERIFIER_nondet_double() READ(f64)

extern int __sala_testing_terminate(int32_t id, char const type, int32_t value);

extern void __sala_testing_loc_hit(int32_t id);

extern _Bool __sala_testing_read_bool(int32_t id);
extern int8_t __sala_testing_read_s8(int32_t id);
extern int16_t __sala_testing_read_s16(int32_t id);
extern int32_t __sala_testing_read_s32(int32_t id);
extern int64_t __sala_testing_read_s64(int32_t id);
extern uint8_t __sala_testing_read_u8(int32_t id);
extern uint16_t __sala_testing_read_u16(int32_t id);
extern uint32_t __sala_testing_read_u32(int32_t id);
extern uint64_t __sala_testing_read_u64(int32_t id);
extern float __sala_testing_read_f32(int32_t id);
extern double __sala_testing_read_f64(int32_t id);

extern void __sala_testing_write_bool(int32_t id, _Bool value);
extern void __sala_testing_write_s8(int32_t id, int8_t value);
extern void __sala_testing_write_s16(int32_t id, int16_t value);
extern void __sala_testing_write_s32(int32_t id, int32_t value);
extern void __sala_testing_write_s64(int32_t id, int64_t value);
extern void __sala_testing_write_u8(int32_t id, uint8_t value);
extern void __sala_testing_write_u16(int32_t id, uint16_t value);
extern void __sala_testing_write_u32(int32_t id, uint32_t value);
extern void __sala_testing_write_u64(int32_t id, uint64_t value);
extern void __sala_testing_write_f32(int32_t id, float value);
extern void __sala_testing_write_f64(int32_t id, double value);
