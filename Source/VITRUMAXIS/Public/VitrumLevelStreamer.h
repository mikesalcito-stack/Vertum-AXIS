#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelStreamingDynamic.h"
#include "VitrumLevelStreamer.generated.h"

UCLASS()
class VITRUMAXIS_API AVitrumLevelStreamer : public AActor
{
    GENERATED_BODY()

public:
    AVitrumLevelStreamer();

    UFUNCTION(BlueprintCallable, Category = "Streaming")
    void StreamLevel(FName LevelName, FVector Location, FRotator Rotation);
};
