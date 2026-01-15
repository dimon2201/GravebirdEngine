// buffer.cpp

#pragma once

#include "buffer.hpp"

namespace realware
{
    cDataBuffer::cDataBuffer(cContext* context) : cFactoryObject(context) {}

    cDataBuffer::~cDataBuffer()
    {
    }

    void cDataBuffer::Create(void* data, types::usize byteSize)
    {
    }
}