/*
 *  Copyright (c) 2014, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include <boost/thread.hpp>

#include <osquery/core.h>
#include <osquery/scheduler.h>

#include "osquery/core/watcher.h"

const std::string kWatcherWorkerName = "osqueryd: worker";

int main(int argc, char* argv[]) {
  osquery::Initializer runner(argc, argv, osquery::OSQUERY_TOOL_DAEMON);

  if (!runner.isWorker()) {
    runner.initDaemon();
  }

  if (!osquery::FLAGS_disable_watchdog) {
    // When a watcher is used, the current watcher will fork into a worker.
    runner.initWorkerWatcher(kWatcherWorkerName);
  }

  // Start osquery work.
  runner.start();

  // Begin the schedule runloop.
  boost::thread scheduler_thread(osquery::initializeScheduler);
  scheduler_thread.join();

  // Finally shutdown.
  runner.shutdown();

  return 0;
}
