// sound_manager.cpp

#include "application.hpp"
#include "sound_manager.hpp"
#include "sound_context.hpp"
#include "memory_pool.hpp"

using namespace types;

namespace realware
{
    cSound::cSound(const std::string& id, cApplication* app, u32 source, u32 buffer) : cIdVecObject(id, app), _source(source), _buffer(buffer) {}

    cSound::~cSound()
    {
        if (_file != nullptr)
        {
            if (_format == eCategory::SOUND_FORMAT_WAV)
            {
                cMemoryPool* memoryPool = _app->GetMemoryPool();
                memoryPool->Free(_file->_data);
                _file->~sWAVStructure();
                memoryPool->Free(_file);
            }
        }
    }

    mSound::mSound(cApplication* app, iSoundContext* context) : _app(app), _context(context), _sounds(app, app->GetDesc()->_maxSoundCount)
    {
    }

    cSound* mSound::CreateSound(const std::string& id, const std::string& filename, eCategory format)
    {
        u32 source = 0;
        u32 buffer = 0;
        sWAVStructure* file = nullptr;
        _context->Create(filename, format, (const sWAVStructure**)&file, source, buffer);

        return _sounds.Add(id, source, buffer);
    }

    cSound* mSound::FindSound(const std::string& id)
    {
        return _sounds.Find(id);
    }

    void mSound::DestroySound(const std::string& id)
    {
        _sounds.Delete(id);
    }

    /*void mSound::Play(entity object, cScene* scene)
    {
        core::sCSound* sound = scene->Get<core::sCSound>(object);
        core::sCTransform* transform = scene->Get<core::sCTransform>(object);
        _context->SetPosition(sound->Sound, transform->Position);
        _context->Play(sound->Sound);
    }

    void mSound::Stop(core::entity object, core::cScene* scene)
    {
        core::sCSound* sound = scene->Get<core::sCSound>(object);
        _context->Stop(sound->Sound);
    }*/
}