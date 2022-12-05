#pragma once

// operating system platform
#if defined(_WIN64) || defined(_WIN32)
#	define FLORAL_PLATFORM_WINDOWS
#	if defined(_GAMING_XBOX_SCARLETT)
#		define FLORAL_PLATFORM_HAS_DX12
#		define FLORAL_PLATFORM_XBOX
#		define FLORAL_PLATFORM_XBOX_SCARLETT
#	elif defined(_GAMING_XBOX_XBOXONE)
#		define FLORAL_PLATFORM_HAS_DX12
#		define FLORAL_PLATFORM_XBOX
#		define FLORAL_PLATFORM_XBOX_ONE
#	else
#		define FLORAL_PLATFORM_HAS_DX12
#		define FLORAL_PLATFORM_HAS_GL
#		define FLORAL_PLATFORM_HAS_VULKAN
#	endif
#elif defined(__ANDROID__) || defined(__linux__)
#	define FLORAL_PLATFORM_LINUX
#	define FLORAL_PLATFORM_HAS_VULKAN
#	if defined(__ANDROID__)
#		define FLORAL_PLATFORM_ANDROID
#		define FLORAL_PLATFORM_HAS_GLES
#	else
#		define FLORAL_PLATFORM_HAS_GL
#	endif
#else
// TODO
#endif

// cpu
#if defined(__aarch64__) || defined(__arm__)
#	define FLORAL_CPU_ARM
#elif defined(__x86_64__) || defined(__i386__)
#	define FLORAL_CPU_INTEL
#else
// TODO
#endif

// cpu architecture
#if defined(__x86_64__) || defined(__aarch64__) || defined(_M_X64)
#	define FLORAL_ARCH_64BIT
#elif defined(__i386__) || defined(__arm__) || defined(_M_IX86)
#	define FLORAL_ARCH_32BIT
#else
// TODO
#endif

// inlining
#define FLORAL_INLINE inline