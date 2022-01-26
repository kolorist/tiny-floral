#pragma once

#include "configs.h"
#include "stdaliases.h"
#include "misc.h"

namespace floral
{
// ----------------------------------------------------------------------------

struct rng_context
{
    static constexpr u64 k_PCG32_default_state      = 0x853c49e6748fea9bull;
    static constexpr u64 k_PCG32_default_stream     = 0xda3e39cb94b95bdbull;
    static constexpr u64 k_PCG32_mult               = 0x5851f42d4c957f2dull;

    static constexpr f32 k_f32_one_minus_epsilon    = 0.99999994f;
    static constexpr f64 k_f64_one_minus_epsilon    = 0.99999999999999989;

    u64                                         state;
    u64                                         inc;
};

FLORAL_INLINE const u32 get_u32(rng_context* i_ctx)
{
    u64 oldState = i_ctx->state;
    i_ctx->state = oldState * rng_context::k_PCG32_mult + i_ctx->inc;
    u32 xorshifted = (u32)(((oldState >> 18u) ^ oldState) >> 27u);
    u32 rot = (u32)(oldState >> 59u);
    return (xorshifted >> rot) | (xorshifted << ((~rot + 1u) & 31));
}

FLORAL_INLINE const u32 get_u32(rng_context* i_ctx, u32 i_modulo)
{
    u32 threshold = (~i_modulo + 1u) % i_modulo;
    while (true)
    {
        u32 r = get_u32(i_ctx);
        if (r >= threshold)
        {
            return r % i_modulo;
        }
    }
}

FLORAL_INLINE const f32 get_f32(rng_context* i_ctx)
{
    return min(rng_context::k_f32_one_minus_epsilon, f32(get_u32(i_ctx) * 2.3283064365386963e-10f));
}

FLORAL_INLINE const f64 get_f64(rng_context* i_ctx)
{
    return min(rng_context::k_f64_one_minus_epsilon, f64(get_u32(i_ctx) * 2.3283064365386963e-10f));
}

FLORAL_INLINE rng_context create_rng(u64 i_sequenceIndex)
{
    rng_context rngContext;

    rngContext.state = 0u;
    rngContext.inc = (i_sequenceIndex << 1u) | 1u;
    get_u32(&rngContext);
    rngContext.state += rng_context::k_PCG32_default_state;
    get_u32(&rngContext);

    return rngContext;
}

// ----------------------------------------------------------------------------
}
