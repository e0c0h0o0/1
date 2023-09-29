#include <db/Tuple.h>
#include <sstream>

using namespace db;

//
// Tuple
//

Tuple::Tuple(const TupleDesc &td, RecordId *rid) :
desc(td), rid(rid), fields(td.numFields(), nullptr){
}

const TupleDesc &Tuple::getTupleDesc() const {
    return desc;
}

const RecordId *Tuple::getRecordId() const {
    return rid;
}

void Tuple::setRecordId(const RecordId *id) {
    this->rid = id;
}

const Field &Tuple::getField(int i) const {
    return *(fields.at(i));
}

void Tuple::setField(int i, const Field *f) {
    fields[i] = f;
}

Tuple::iterator Tuple::begin() const {
    return fields.cbegin();
}

Tuple::iterator Tuple::end() const {
    return fields.cend();
}

std::string Tuple::to_string() const {
    std::stringstream ss{};
    for (int i = 0; i < fields.size(); i++)
        ss << fields[i]->to_string() << "\t";
    return std::move(ss.str());
}
