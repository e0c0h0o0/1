#include <db/HeapPageId.h>

using namespace db;

//
// HeapPageId
//

HeapPageId::HeapPageId(int tbId, int pgNo) : tableId(tbId), pageNo(pgNo) {
}

int HeapPageId::getTableId() const {
    return tableId;
}

int HeapPageId::pageNumber() const {
    return pageNo;
}

bool HeapPageId::operator==(const PageId &other) const {
    if (const auto *otherPageId = dynamic_cast<const HeapPageId *>(&other)) {
        return this->getTableId() == otherPageId->getTableId() && this->pageNumber() == otherPageId->pageNumber();
    }
    return false;
}
