#pragma once

/// @file userver/server/handlers/inspect_requests.hpp
/// @brief @copybrief server::handlers::InspectRequests

#include <userver/components/manager.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>

USERVER_NAMESPACE_BEGIN

namespace server {

class RequestsView;

namespace handlers {
// clang-format off

/// @ingroup userver_components userver_http_handlers
///
/// @brief Handler that returns information about all in-flight requests.
///
/// The component has no service configuration except the
/// @ref userver_http_handlers "common handler options".
///
/// ## Static configuration example:
///
/// @snippet components/common_server_component_list_test.cpp  Sample handler inspect requests component config
///
/// ## Scheme
/// Provide an optional query parameter `body` to get the bodies of all the
/// in-flight requests.

// clang-format on
class InspectRequests final : public HttpHandlerJsonBase {
 public:
  InspectRequests(const components::ComponentConfig& config,
                  const components::ComponentContext& component_context);

  static constexpr std::string_view kName = "handler-inspect-requests";

  formats::json::Value HandleRequestJsonThrow(
      const http::HttpRequest& request,
      const formats::json::Value& request_json,
      request::RequestContext& context) const override;

 private:
  RequestsView& view_;
};

}  // namespace handlers
}  // namespace server

USERVER_NAMESPACE_END
