// For conditions of distribution and use, see copyright notice in license.txt

#include "StableHeaders.h"
#include "OpenALAudioModule.h"
#include <Poco/ClassLibrary.h>
#include "Foundation.h"
#include "ComponentRegistrarInterface.h"
#include "EC_OpenALEntity.h"

namespace OpenALAudio
{
    OpenALAudioModule::OpenALAudioModule() : ModuleInterfaceImpl(type_static_)
	{
	}

    OpenALAudioModule::~OpenALAudioModule()
	{
	}

    //Virtual
    void OpenALAudioModule::Load()
	{
		using namespace OpenALAudio;

		LogInfo("Module " + Name() + " loaded.");
		DECLARE_MODULE_EC(EC_OpenALEntity);
	}

    //Virtual
    void OpenALAudioModule::Unload()
	{
		LogInfo("Module " + Name() + " unloaded.");
	}

	
    void OpenALAudioModule::PreInitialize()
	{
		sound_ = OpenALAudio::SoundPtr(new OpenALAudio::Sound(framework_));
	}
	
    //Virtual
    void OpenALAudioModule::Initialize()
    {
        sound_->Initialize();

		framework_->GetServiceManager()->RegisterService(Foundation::Service::ST_Sound, sound_);

        LogInfo("Module " + Name() + " initialized.");
    }

    void OpenALAudioModule::PostInitialize()
	{
	}

    void OpenALAudioModule::Uninitialize()
	{
		framework_->GetServiceManager()->UnregisterService(sound_);
		sound_.reset();

		LogInfo("Module " + Name() + " uninitialized.");
	}

    void OpenALAudioModule::Update()
	{
		sound_->Update();
	}
}

using namespace OpenALAudio;

POCO_BEGIN_MANIFEST(Foundation::ModuleInterface)
    POCO_EXPORT_CLASS(OpenALAudioModule)
POCO_END_MANIFEST
