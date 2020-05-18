//
// Copyright (C) 2004-2006 Maciej Sobczak, Stephen Hutton
// MySQL backend copyright (C) 2006 Pawel Aleksander Fedorynski
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#define SOCI_MYSQL_SOURCE
#include <ciso646>
#include "soci/backend-loader.h"
#include "soci/mysql/soci-mysql.h"

#ifdef _MSC_VER
#pragma warning(disable : 4355)
#endif

using namespace soci;
using namespace soci::details;

// concrete factory for MySQL concrete strategies
mysql_session_backend* mysql_backend_factory::make_session(
    connection_parameters const& parameters) const {
  std::unique_lock<std::mutex> lk(mtx_);
  std::thread::id tid = std::this_thread::get_id();
  auto result = initedThreads_.insert(tid);
  if (result.second) { // inserted.
    mysql_thread_init();
  }

  return new mysql_session_backend(parameters);
}

mysql_backend_factory const soci::mysql;

extern "C" {

// for dynamic backend loading
SOCI_MYSQL_DECL backend_factory const* factory_mysql() { return &soci::mysql; }

SOCI_MYSQL_DECL void register_factory_mysql() {
  mysql_library_init(0, NULL, NULL);
  soci::dynamic_backends::register_backend("mysql", soci::mysql);
}

}  // extern "C"
