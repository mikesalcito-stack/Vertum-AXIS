#include "VitrumLevelStreamer.h"
#include "Engine/LevelStreamingDynamic.h"

AVitrumLevelStreamer::AVitrumLevelStreamer()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AVitrumLevelStreamer::StreamLevel(FName LevelName, FVector Location, FRotator Rotation)
{
    FSoftObjectPath LevelPath(FString::Printf(TEXT("/Game/Levels/%s"), *LevelName.ToString()));
    bool bSuccess = false;
    ULevelStreamingDynamic* LoadedLevel = ULevelStreamingDynamic::LoadLevelInstance(GetWorld(), LevelPath.ToString(), Location, Rotation, bSuccess);
}

UFUNCTION(BlueprintCallable, Category = "Streaming")
void UnloadLevel(ULevelStreaming* Level)
{
    if (Level) Level->SetIsRequestingUnloadAndRemoval(true);
}
