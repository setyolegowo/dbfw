//
// Database Permission class header
// 
// Created by  : Setyo Legowo (13511071@std.stei.itb.ac.id)
// Institution : Institut Teknologi Bandung
// License     : GPL v2 (http://www.gnu.org/licenses/gpl-2.0.html)
//

#ifndef DBFW_PERM_HPP
#define DBFW_PERM_HPP

#include <string.h>
#include <map>
#include <vector>

#include "buffer.hpp"

class DBPerm
{
public:
	bool error_result;
	std::map<std::string, unsigned char> mask_map;

    DBPerm();
    virtual ~DBPerm();

    bool addAttr(std::string& attr);
    bool checkout(std::string& subject, std::string& action, std::string& uri);
    bool oneCheckPermission(std::string& subject, std::string& action, std::string& uri);
    int getResult();

private:
    std::vector<std::string> attr_list;
    bool _connect(std::string& buff);
    bool _parsingResult(const char *, int size);

};

#endif // DBFW_PERM_HPP