#include <db/Catalog.h>
#include <db/TupleDesc.h>
#include <stdexcept>

using namespace db;

void Catalog::addTable(DbFile *file, const std::string &name, const std::string &pkeyField) {
    auto tb = Table(file,name,pkeyField);
    auto it = nameMap.find(name);
    if (nameMap.find(name) != nameMap.end()) {
        nameMap.erase(it);
    }

    nameMap.insert({name,tb});
    auto it1 = idMap.find(file->getId());
    if (it1 != idMap.end()) {
        idMap.erase(it1);
    }
    idMap.insert({file->getId(),tb});
}

int Catalog::getTableId(const std::string &name) const {
    auto it = nameMap.find(name);
    if (it != nameMap.end()) {
        return it->second.file->getId();
    }
    throw std::invalid_argument("Invalid table id");
}

const TupleDesc &Catalog::getTupleDesc(int tableid) const {
    auto it = idMap.find(tableid);
    if (it != idMap.end()) {
        return (it->second.file->getTupleDesc());
    }
    throw std::invalid_argument("Invalid table id");
}

DbFile *Catalog::getDatabaseFile(int tableid) const {
    auto it = idMap.find(tableid);
    if (it != idMap.end()) {
        return (it->second.file);
    }
    return nullptr;
}
std::string Catalog::getPrimaryKey(int tableid) const {
    auto it = idMap.find(tableid);
    if (it != idMap.end()) {
        return (it->second.pkeyField);
    }
    throw std::invalid_argument("Invalid table id");
}

std::string Catalog::getTableName(int tableid) const {
    auto it = idMap.find(tableid);
    if (it != idMap.end()) {
        return (it->second.name);
    }
    throw std::invalid_argument("Invalid table id");
}

void Catalog::clear() {
    nameMap.clear();
    idMap.clear();
}
