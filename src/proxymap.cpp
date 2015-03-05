//
// Database Firewall functions used to manage a number of proxy objects.
//
// Copyright (c) 2015 Setyo Legowo <13511071@std.stei.itb.ac.id>
// License: GPL v2 (http://www.gnu.org/licenses/gpl.html)
//

#include "proxymap.hpp"

#include "dbfw.hpp"
// #include "mysql/mysql_con.hpp"
// #include "pgsql/pgsql_con.hpp"

#include <stdio.h>
#include <map>

bool proxyMapInit()
{}
bool proxyMapReload()
{}
bool proxyMapClose()
{}
void wrapServer(struct ev_loop *loop, ev_io *w_, int revents)
{}
void wrapProxy(struct ev_loop *loop, ev_io *w_, int revents)
{}
void wrapBackend(struct ev_loop *loop, ev_io *w_, int revents)
{}
