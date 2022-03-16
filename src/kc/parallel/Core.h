#pragma once

#define CL_HPP_ENABLE_EXCEPTIONS

#ifndef CL_HPP_TARGET_OPENCL_VERSION
#define CL_HPP_TARGET_OPENCL_VERSION 210
#endif

#include <CL/cl2.hpp>

#include "kc/core/ErrorBase.hpp"

DEFINE_ERROR(ParallelError);
