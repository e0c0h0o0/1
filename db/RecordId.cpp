#include <db/RecordId.h>
#include <stdexcept>

using namespace db;

//
// RecordId
//

//TODO pa1.4: implement
RecordId::RecordId(const PageId *pid, int tupleno): pageId(pid), tupleNo(tupleno)  {
}

bool RecordId::operator==(const RecordId &other) const {
    if (this == &other) return true;
    return this->pageId->getTableId() == other.pageId->getTableId() &&
    this->pageId->pageNumber() == other.pageId->pageNumber() &&
    this->tupleNo == other.tupleNo;
}

const PageId *RecordId::getPageId() const {
    return pageId;
}

int RecordId::getTupleno() const {
    return tupleNo;
}

//
// RecordId hash function
//

std::size_t std::hash<RecordId>::operator()(const RecordId &r) const {
    return std::hash<PageId>()(*r.getPageId()) ^ std::hash<int>()(r.getTupleno());
}
