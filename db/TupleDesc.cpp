#include <db/TupleDesc.h>
#include <sstream>

using namespace db;

//
// TDItem
//

bool TDItem::operator==(const TDItem &other) const {
    if (this == &other) return true;
    return fieldType == other.fieldType && fieldName == other.fieldName;
}

std::size_t std::hash<TDItem>::operator()(const TDItem &r) const {
    auto h1 = std::hash<int>();
    auto h2 = std::hash<std::string>();
    return h1(r.fieldType) + h2(r.fieldName);
}

//
// TupleDesc
//
TupleDesc::TupleDesc(const std::vector<Types::Type> &types) : tdItems(){
    tdItems.reserve(types.size());

    for (auto &t : types) {
        tdItems.emplace_back(t, "");
    }
}

TupleDesc::TupleDesc(const std::vector<Types::Type> &types, const std::vector<std::string> &names) : tdItems() {
    tdItems.reserve(types.size());

    for (int i = 0; i < types.size(); ++i) {
        tdItems.emplace_back(types[i], names[i]);
    }
}

size_t TupleDesc::numFields() const {
    return tdItems.size();
}

std::string TupleDesc::getFieldName(size_t i) const {
    return tdItems[i].fieldName;
}

Types::Type TupleDesc::getFieldType(size_t i) const {
    return tdItems[i].fieldType;
}

int TupleDesc::fieldNameToIndex(const std::string &fieldName) const {
    for (int i = 0; i < tdItems.size(); i++) {
        if ( tdItems[i].fieldName == fieldName) {
            return i;
        }
    }
    throw std::invalid_argument("Invalid field name");
}

size_t TupleDesc::getSize() const {
    size_t size = 0;
    for (const TDItem &item : tdItems)
        size += getLen(item.fieldType); // only fieldType??
    return size;
}

TupleDesc TupleDesc::merge(const TupleDesc &td1, const TupleDesc &td2) {
    size_t total = td1.numFields() + td2.numFields();
    std::vector<Types::Type> types{};
    std::vector<std::string> fnames{};
    types.reserve(total);
    fnames.reserve(total);

    for (size_t i = 0; i < td1.numFields(); ++i) {
        types.emplace_back(td1.getFieldType(i));
        fnames.emplace_back(td1.getFieldName(i));
    }

    for (size_t i = 0; i < td2.numFields(); ++i) {
        types.emplace_back(td2.getFieldType(i));
        fnames.emplace_back(td2.getFieldName(i));
    }
    TupleDesc newDesc = {types, fnames};
    return std::move(newDesc);
}

std::string TupleDesc::to_string() const {
    std::stringstream ss = {};
    for (int i = 0; i < numFields(); i++)
    {
        ss << getFieldType(i) << "["  <<  i << "]";
        ss << "(" << getFieldName(i) <<")";
        if (i != numFields() - 1)
            ss << ", ";
    }
    return std::move(ss.str());
}

bool TupleDesc::operator==(const TupleDesc &other) const {
    if (this == &other) return true;
    if (numFields() == other.numFields() && getSize() == other.getSize()) {
        for (int i = 0; i < numFields(); ++i) {
            if (getFieldType(i) != other.getFieldType(i)) return false;
            if (getFieldName(i) != other.getFieldName(i)) return false;
        }
        return true;
    }
    return false;
}

TupleDesc::iterator TupleDesc::begin() const {
    return tdItems.cbegin();
}

TupleDesc::iterator TupleDesc::end() const {
    return tdItems.cend();
}

std::size_t std::hash<db::TupleDesc>::operator()(const db::TupleDesc &td) const {
    size_t h = 0;
    auto hf = std::hash<TDItem>();
    for (const auto & it : td) {
        h += hf(it);
    }
    return h;
}
