#pragma once

/// @file userver/storages/redis/component.hpp
/// @brief @copybrief components::Redis

#include <memory>
#include <string>
#include <unordered_map>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/storages/redis/impl/wait_connected_mode.hpp>
#include <userver/utils/statistics/storage.hpp>

#include <userver/testsuite/testsuite_support.hpp>

#include <userver/taxi_config/storage/component.hpp>

USERVER_NAMESPACE_BEGIN

namespace redis {
class Sentinel;
class ThreadPools;
}  // namespace redis

/// Components, clients and helpers for different databases and storages
namespace storages {
/// Redis client
namespace redis {
class Client;
class SubscribeClient;
class SubscribeClientImpl;
}  // namespace redis
}  // namespace storages

namespace components {

class StatisticsStorage;

// clang-format off

/// @ingroup userver_components
///
/// @brief Redis client component
///
/// Provides access to a redis cluster.
///
/// ## Static options:
/// Name | Description | Default value
/// ---- | ----------- | -------------
/// thread_pools.redis_thread_pool_size | thread count to serve Redis requests | -
/// thread_pools.sentinel_thread_pool_size | thread count to serve sentinel requests. | -
/// groups | array of redis clusters to work with excluding subscribers | -
/// groups.[].config_name | key name in secdist with options for this cluster | -
/// groups.[].db | name to refer to the cluster in components::Redis::GetClient() | -
/// groups.[].sharding_strategy | one of RedisCluster, KeyShardCrc32, KeyShardTaximeterCrc32 or KeyShardGpsStorageDriver | "KeyShardTaximeterCrc32"
/// subscribe_groups | array of redis clusters to work with in subscribe mode | -
/// subscribe_groups.[].config_name | key name in secdist with options for this cluster | -
/// subscribe_groups.[].db | name to refer to the cluster in components::Redis::GetSubscribeClient() | -
/// subscribe_groups.[].sharding_strategy | either RedisCluster or KeyShardTaximeterCrc32 | "KeyShardTaximeterCrc32"
///
/// ## Static configuration example:
///
/// ```
///    redis:
///        groups:
///          - config_name: taxi-tmp
///            db: taxi-tmp
///            sharding_strategy: "RedisCluster"
///          - config_name: taxi-tmp-pubsub
///            db: taxi-tmp-pubsub
///        subscribe_groups:
///          - config_name: taxi-tmp-pubsub
///            db: taxi-tmp-pubsub
///        thread_pools:
///            redis_thread_pool_size: 8
///            sentinel_thread_pool_size: 1
/// ```

// clang-format on
class Redis : public LoggableComponentBase {
 public:
  Redis(const ComponentConfig& config,
        const ComponentContext& component_context);

  ~Redis() override;

  static constexpr std::string_view kName = "redis";

  std::shared_ptr<storages::redis::Client> GetClient(
      const std::string& name,
      USERVER_NAMESPACE::redis::RedisWaitConnected wait_connected = {}) const;
  [[deprecated("use GetClient()")]] std::shared_ptr<redis::Sentinel> Client(
      const std::string& name) const;
  std::shared_ptr<storages::redis::SubscribeClient> GetSubscribeClient(
      const std::string& name,
      USERVER_NAMESPACE::redis::RedisWaitConnected wait_connected = {}) const;

 private:
  void OnConfigUpdate(const taxi_config::Snapshot& cfg);

  void Connect(const ComponentConfig& config,
               const ComponentContext& component_context,
               const testsuite::RedisControl& testsuite_redis_control);

  formats::json::Value ExtendStatisticsRedis(
      const utils::statistics::StatisticsRequest& /*request*/);

  formats::json::Value ExtendStatisticsRedisPubsub(
      const utils::statistics::StatisticsRequest& /*request*/);

  std::shared_ptr<redis::ThreadPools> thread_pools_;
  std::unordered_map<std::string, std::shared_ptr<redis::Sentinel>> sentinels_;
  std::unordered_map<std::string, std::shared_ptr<storages::redis::Client>>
      clients_;
  std::unordered_map<std::string,
                     std::shared_ptr<storages::redis::SubscribeClientImpl>>
      subscribe_clients_;

  taxi_config::Source config_;
  concurrent::AsyncEventSubscriberScope config_subscription_;

  components::StatisticsStorage& statistics_storage_;
  utils::statistics::Entry statistics_holder_;
  utils::statistics::Entry subscribe_statistics_holder_;
};

}  // namespace components

USERVER_NAMESPACE_END
