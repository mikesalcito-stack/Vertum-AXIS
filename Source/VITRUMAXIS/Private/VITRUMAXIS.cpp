#include "VITRUMAXIS.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FVITRUMAXISModule, VITRUMAXIS, "VITRUMAXIS");

void FVITRUMAXISModule::StartupModule()
{
	// Initialize VITRUM AXIS module
	UE_LOG(LogTemp, Log, TEXT("VITRUM AXIS Module Started"));
}

void FVITRUMAXISModule::ShutdownModule()
{
	// Cleanup VITRUM AXIS module
	UE_LOG(LogTemp, Log, TEXT("VITRUM AXIS Module Shutdown"));
}
