#include <db/SeqScan.h>
#include <db/TransactionId.h>
#include <db/TupleDesc.h>

using namespace db;

SeqScan::SeqScan(TransactionId *tid, int tbid, const std::string &tbalias):
    transactionId(tid),
    tableid(tbid),
    tableAlias(tbalias){
    dbFile = Database::getCatalog().getDatabaseFile(tbid);
}

std::string SeqScan::getTableName() const {
    return Database::getCatalog().getTableName(tableid);
}

std::string SeqScan::getAlias() const {
    return tableAlias;
}

void SeqScan::reset(int tabid, const std::string &tableAlias) {
    this->tableid = tabid;
    this->tableAlias = tableAlias;
}

const TupleDesc &SeqScan::getTupleDesc() const {
    return Database::getCatalog().getTupleDesc(tableid);
}

SeqScan::iterator SeqScan::begin() const {
    return {this, ((HeapFile*)dbFile)->begin()};
}

SeqScan::iterator SeqScan::end() const {
    int pgs = ((HeapFile*)dbFile)->getNumPages();
    return {this, ((HeapFile*)dbFile)->end()};
}

//
// SeqScanIterator
//

SeqScanIterator::SeqScanIterator(const void *ss, HeapFileIterator it): seqScan(ss), heapFileIterator(it) {

}

bool SeqScanIterator::operator!=(const SeqScanIterator &other) const {
    if (this == &other) return false;
    return this->seqScan != other.seqScan || (this->heapFileIterator != other.heapFileIterator);
}

SeqScanIterator &SeqScanIterator::operator++() {
    ++heapFileIterator;
    return *this;
}

const Tuple &SeqScanIterator::operator*() const {
    return *heapFileIterator;
}
