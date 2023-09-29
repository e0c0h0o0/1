#include <db/HeapFile.h>
#include <db/TupleDesc.h>
#include <db/Page.h>
#include <db/PageId.h>
#include <db/HeapPage.h>
#include <stdexcept>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdio>
#include <unistd.h>

#include <filesystem>

using namespace db;

//
// HeapFile
//

//TODO pa1.5: implement
HeapFile::HeapFile(const char *fname, const TupleDesc &td) : file(fname), td(td) {
    std::filesystem::path file_path{fname};
    auto hf = std::hash<std::string>();
    id = (int)hf(file_path.string());
}

int HeapFile::getId() const {
    return id;
}

const TupleDesc &HeapFile::getTupleDesc() const {
    return td;
}

Page *HeapFile::readPage(const PageId &pid) {
    HeapPage *res;
    int pgsz = Database::getBufferPool().getPageSize();
    auto data = new uint8_t[pgsz];
    long offset = pid.pageNumber() * pgsz;

    int fd = open(file.c_str(), O_RDONLY);

    if (fd < 0 || pread(fd, data, pgsz, offset) <= 0) {
        goto error_read_page;
    }
    close(fd);

    res = new HeapPage(
            {pid.getTableId(), pid.pageNumber()}, data);
    delete[] data;
    return res;

    error_read_page:
    delete[] data;
    return nullptr;
}

int HeapFile::getNumPages() const {
    struct stat stbuf;
    int pgsz = Database::getBufferPool().getPageSize();
    int fd = open(file.c_str(), O_RDONLY);
    if (fd < 0 || fstat(fd, &stbuf) < 0) {
        return -1;
    };

    return (int)(stbuf.st_size + pgsz - 1)/pgsz;
}

HeapFileIterator HeapFile::begin() const {
    HeapPageId pid{id, 0};
    HeapPage *page = (HeapPage *)Database::getBufferPool().getPage({}, &pid);
    if (page) {
        return {this, 0, page->begin()};
    } else {
        return {this, 0, {0, nullptr}};
    }
}

HeapFileIterator HeapFile::end() const {
    return {this, getNumPages(), {0, nullptr}};
}



//
// HeapFileIterator
//
HeapFileIterator::HeapFileIterator(const HeapFile *f, int pgNo, HeapPageIterator pgIt):
        heapFile(f), pageNo(pgNo), pageIterator(pgIt) {

}

bool HeapFileIterator::operator!=(const HeapFileIterator &other) const {
    if (this == &other) return false;
    if (this->heapFile != other.heapFile || this->pageNo != other.pageNo) {
        return true;
    }
    return pageNo != heapFile->getNumPages() && pageIterator != other.pageIterator;
}

Tuple &HeapFileIterator::operator*() const {
    return *(pageIterator);
}

HeapFileIterator &HeapFileIterator::operator++() {
    ++pageIterator;
    HeapPageId pid = {heapFile->getId(), pageNo};
    HeapPage *curPage = (HeapPage *)Database::getBufferPool().getPage({}, &pid);

    while (curPage && !(pageIterator != curPage->end())) {
        pageNo += 1;
        pid = {heapFile->getId(), pageNo};
        curPage = (HeapPage *)Database::getBufferPool().getPage({}, &pid);
        pageIterator = {0, curPage};

    }
    return *this;
}
