// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_RESOLUTION_PROXY_CONFIG_SERVICE_MAC_H_
#define NET_PROXY_RESOLUTION_PROXY_CONFIG_SERVICE_MAC_H_

#include <memory>

#include "base/compiler_specific.h"
#include "base/macros.h"
#include "base/memory/ref_counted.h"
#include "base/observer_list.h"
#include "net/base/network_config_watcher_mac.h"
#include "net/proxy_resolution/proxy_config_service.h"
#include "net/proxy_resolution/proxy_config_with_annotation.h"

namespace base {
class SequencedTaskRunner;
}  // namespace base

namespace net {

class ProxyConfigServiceMac : public ProxyConfigService {
 public:
  // Constructs a ProxyConfigService that watches the Mac OS system settings.
  // This instance is expected to be operated and deleted on
  // |sequenced_task_runner| (however it may be constructed elsewhere).
  explicit ProxyConfigServiceMac(
      const scoped_refptr<base::SequencedTaskRunner>& sequenced_task_runner,
      const NetworkTrafficAnnotationTag& traffic_annotation);

  ProxyConfigServiceMac(const ProxyConfigServiceMac&) = delete;
  ProxyConfigServiceMac& operator=(const ProxyConfigServiceMac&) = delete;

  ~ProxyConfigServiceMac() override;

 public:
  // ProxyConfigService implementation:
  void AddObserver(Observer* observer) override;
  void RemoveObserver(Observer* observer) override;
  ConfigAvailability GetLatestProxyConfig(
      ProxyConfigWithAnnotation* config) override;

 private:
  class Helper;

  // Forwarder just exists to keep the NetworkConfigWatcherMac API out of
  // ProxyConfigServiceMac's public API.
  class Forwarder : public NetworkConfigWatcherMac::Delegate {
   public:
    explicit Forwarder(ProxyConfigServiceMac* proxy_config_service)
        : proxy_config_service_(proxy_config_service) {}

    Forwarder(const Forwarder&) = delete;
    Forwarder& operator=(const Forwarder&) = delete;

    // NetworkConfigWatcherMac::Delegate implementation:
    void StartReachabilityNotifications() override {}
    void SetDynamicStoreNotificationKeys(SCDynamicStoreRef store) override;
    void OnNetworkConfigChange(CFArrayRef changed_keys) override;

   private:
    ProxyConfigServiceMac* const proxy_config_service_;
  };

  // Methods directly called by the NetworkConfigWatcherMac::Delegate:
  void SetDynamicStoreNotificationKeys(SCDynamicStoreRef store);
  void OnNetworkConfigChange(CFArrayRef changed_keys);

  // Called when the proxy configuration has changed, to notify the observers.
  void OnProxyConfigChanged(const ProxyConfigWithAnnotation& new_config);

  Forwarder forwarder_;
  std::unique_ptr<const NetworkConfigWatcherMac> config_watcher_;

  base::ObserverList<Observer>::Unchecked observers_;

  // Holds the last system proxy settings that we fetched.
  bool has_fetched_config_;
  ProxyConfigWithAnnotation last_config_fetched_;

  scoped_refptr<Helper> helper_;

  // The task runner that |this| will be operated on.
  const scoped_refptr<base::SequencedTaskRunner> sequenced_task_runner_;

  const NetworkTrafficAnnotationTag traffic_annotation_;
};

}  // namespace net

#endif  // NET_PROXY_RESOLUTION_PROXY_CONFIG_SERVICE_MAC_H_
