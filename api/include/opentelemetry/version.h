#pragma once

#include "opentelemetry/detail/preprocessor.h"

#define OPENTELEMETRY_ABI_VERSION_NO 0
#define OPENTELEMETRY_VERSION "0.0.1"
#define OPENTELEMETRY_ABI_VERSION OPENTELEMETRY_STRINGIFY(OPENTELEMETRY_ABI_VERSION_NO)

// clang-format off
#define OPENTELEMETRY_BEGIN_NAMESPACE \
  namespace opentelemetry { inline namespace OPENTELEMETRY_CONCAT(v, OPENTELEMETRY_ABI_VERSION_NO) {

#define OPENTELEMETRY_END_NAMESPACE \
  }}
// clang-format on
