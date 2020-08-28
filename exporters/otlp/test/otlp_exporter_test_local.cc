#include "opentelemetry/sdk/trace/simple_processor.h"
#include "opentelemetry/sdk/trace/tracer_provider.h"
#include "opentelemetry/trace/provider.h"
#include "opentelemetry/exporters/otlp/otlp_exporter.h"
#include "opentelemetry/context/threadlocal_context.h"
#include "src/common/random.h"

#include <gtest/gtest.h>

OPENTELEMETRY_BEGIN_NAMESPACE
namespace exporter
{
namespace otlp
{

// Helper functions
trace::TraceId GenerateRandomTraceId()
{
  uint8_t trace_id_buf[trace::TraceId::kSize];
  opentelemetry::sdk::common::Random::GenerateRandomBuffer(trace_id_buf);
  return trace::TraceId(trace_id_buf);
}

trace::SpanId GenerateRandomSpanId()
{
  uint8_t span_id_buf[trace::SpanId::kSize];
  opentelemetry::sdk::common::Random::GenerateRandomBuffer(span_id_buf);
  return trace::SpanId(span_id_buf);
}

TEST(OtlpExporter, Export)
{
  auto exporter = std::unique_ptr<sdk::trace::SpanExporter>(new OtlpExporter);

  const int kNumSpans = 2;
  trace::TraceId trace_id = GenerateRandomTraceId();
  trace::SpanId last_span_id = GenerateRandomSpanId();
  std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now();
  int durations[2] = {1000, 700};

  for (int i=0; i < kNumSpans; i++) {
    auto rec = exporter->MakeRecordable();

    trace::SpanId span_id = GenerateRandomSpanId();
    rec->SetName("Span " + std::to_string(i));
    rec->SetIds(trace_id, span_id, last_span_id);

    rec->SetStartTime(core::SystemTimestamp(start_time));
    rec->SetDuration(std::chrono::milliseconds(durations[i]));

    rec->SetAttribute("str_attr", "str_value");
    rec->SetAttribute("int_attr", 234);
    rec->SetAttribute("bool_attr", true);
    // rec->SetAttribute("double_attr", 3.3);
    // rec->SetAttribute("array_attr", nostd::span<const bool>({true, false}));

    rec->SetStatus(trace::CanonicalCode::OK, "For demo");
    rec->AddEvent("Demo event", core::SystemTimestamp(start_time));
    rec->AddEvent("Demo event with attributes", start_time,
                trace::KeyValueIterableView<std::map<std::string, int>>({{"attr1", 6}, {"attr2", 8}}));
    rec->AddLink(trace::SpanContext(false, false));
    // rec->AddLink(trace::SpanContext(false, false),
    //             trace::KeyValueIterableView<std::map<std::string, int>>({{"attr1", 5}, {"attr2", 11}}));

    last_span_id = span_id;
    start_time = start_time + std::chrono::milliseconds(300);

    auto result = exporter->Export(nostd::span<std::unique_ptr<sdk::trace::Recordable>>(&rec, 1));
    EXPECT_EQ(sdk::trace::ExportResult::kSuccess, result);
  }
}
}  // namespace otlp
}  // namespace exporter
OPENTELEMETRY_END_NAMESPACE
