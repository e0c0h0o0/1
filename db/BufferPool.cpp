#include <db/BufferPool.h>
#include <db/Database.h>

using namespace db;

BufferPool::BufferPool(int numPages) : pageMap() {

}

Page *BufferPool::getPage(const TransactionId &tid, PageId *pid) {
    Page *ret = nullptr;
    auto it1 = pageMap.find(pid->getTableId());
    if (it1 == pageMap.end()) {
        pageMap[pid->getTableId()] = {};
        it1 = pageMap.find(pid->getTableId());
    }
    auto &pgs = it1->second;
    auto it2 = pgs.find(pid->pageNumber());
    if (it2 == pgs.end()) {
        DbFile *file = Database::getCatalog().getDatabaseFile(pid->getTableId());
        if (file) {
            ret = file->readPage(*pid);
            if (ret) pgs.insert({pid->pageNumber(), ret});
        }
    } else {
        ret = it2->second;
    }
    return ret;

}

void BufferPool::evictPage() {
    // do nothing for now
}
