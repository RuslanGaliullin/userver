#include <userver/components/loggable_component_base.hpp>

#include <userver/components/component.hpp>
#include <userver/logging/component.hpp>
#include <userver/tracing/component.hpp>
#include <userver/yaml_config/schema.hpp>

USERVER_NAMESPACE_BEGIN

namespace components {

LoggableComponentBase::LoggableComponentBase(
    const ComponentConfig&, const ComponentContext& component_context) {
  component_context.FindComponent<Logging>();
  component_context.FindComponent<Tracer>();
}

yaml_config::Schema LoggableComponentBase::GetStaticConfigSchema() {
  auto schema = RawComponentBase::GetStaticConfigSchema();
  schema.UpdateDescription(
      "Base class for all application components, it depends on "
      "components::Logger and optionally components::Tracer.");
  return schema;
}

}  // namespace components

USERVER_NAMESPACE_END
