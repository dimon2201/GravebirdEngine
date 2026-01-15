// filesystem_manager.hpp

#pragma once

#include "object.hpp"
#include "types.hpp"

namespace realware
{
    class cDataBuffer;

    class cDataFile : public cFactoryObject
    {
    public:
        cDataFile(cContext* context);
        virtual ~cDataFile() override;

        void Open(const std::string& path, types::boolean isText);

    private:
        cDataBuffer* _data = nullptr;
    };

    class cFileSystem : public iObject
    {
    public:
        explicit cFileSystem(cContext* context);
        ~cFileSystem() = default;

        cDataFile* CreateDataFile(const std::string& filepath, types::boolean isText);
        void DestroyDataFile(cDataFile* buffer);
    };
}