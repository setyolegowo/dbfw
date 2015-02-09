//
// Database Firewall configuration class implementation.
// 
// Created by  : Setyo Legowo (13511071@std.stei.itb.ac.id)
// Institution : Institut Teknologi Bandung
// License     : GPL v2 (http://www.gnu.org/licenses/gpl-2.0.html)
//

#include "config.hpp"

DBFWConfig* DBFWConfig::_obj = NULL;

DBFWConfig::DBFWConfig() {
    init();
}

DBFWConfig::~DBFWConfig() {
    // TODO Log destruction
}

DBFWConfig* DBFWConfig::getInstance() {
    if (_obj == NULL) {
        _obj = new DBFWConfig();
    }
    return _obj;
}

void DBFWConfig::free() {
    if(_obj != NULL) {
        _obj->close_db();
        delete _obj;
        _obj = NULL;
    }
}

void DBFWConfig::init() {
}

bool DBFWConfi::load(std::string & path) {
}

bool DBFWConfi::load_db() {
}

