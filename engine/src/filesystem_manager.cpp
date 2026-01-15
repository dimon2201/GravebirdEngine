// filesystem_manager.cpp

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include "application.hpp"
#include "context.hpp"
#include "filesystem_manager.hpp"
#include "memory_pool.hpp"

using namespace types;

namespace realware
{
    cDataFile::cDataFile(cContext* context) : cFactoryObject(context) {}

    cDataFile::~cDataFile()
    {
        _context->Destroy<cDataBuffer>(_data);
    }

    void cDataFile::Open(const std::string& path, types::boolean isText)
    {
        std::ifstream inputFile(path, std::ios::binary);

        inputFile.seekg(0, std::ios::end);
        const usize byteSize = inputFile.tellg();
        inputFile.seekg(0, std::ios::beg);
        const usize databyteSize = byteSize + (isText == K_TRUE ? 1 : 0);

        auto memoryPoolBuffer = _context->GetMemoryPool<cDataBuffer>();
        auto memoryPoolFile = _context->GetMemoryPool<cDataFile>();

        cDataBuffer* data = memoryPoolBuffer->Allocate();
        memset(data, 0, databyteSize);
        inputFile.read((char*)&data[0], byteSize);

        cDataFile* pFile = memoryPoolFile->Allocate();
        cDataFile* file = new (pFile) cDataFile;

        file->_data = data;
        file->_dataByteSize = databyteSize;

        return file;
    }

    cFileSystem::cFileSystem(cContext* context) : iObject(context) {}

    cDataFile* cFileSystem::CreateDataFile(const std::string& filepath, types::boolean isText)
    {
        std::ifstream inputFile(filepath, std::ios::binary);
        
        inputFile.seekg(0, std::ios::end);
        const usize byteSize = inputFile.tellg();
        inputFile.seekg(0, std::ios::beg);
        const usize databyteSize = byteSize + (isText == K_TRUE ? 1 : 0);
        
        auto memoryPoolBuffer = _context->GetMemoryPool<cDataBuffer>();
        auto memoryPoolFile = _context->GetMemoryPool<cDataFile>();

        cDataBuffer* data = memoryPoolBuffer->Allocate();
        memset(data, 0, databyteSize);
        inputFile.read((char*)&data[0], byteSize);

        cDataFile* pFile = memoryPoolFile->Allocate();
        cDataFile* file = new (pFile) cDataFile;

        file->_data = data;
        file->_dataByteSize = databyteSize;

        return file;
    }

    void cFileSystem::DestroyDataFile(cDataFile* file)
    {
        void* fileData = file->_data;

        if (fileData == nullptr || file->_dataByteSize == 0)
            return;

        GetApplication()->GetMemoryPool()->Free(fileData);
    }
}