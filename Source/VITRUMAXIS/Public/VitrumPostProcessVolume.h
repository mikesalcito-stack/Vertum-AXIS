#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "Components/PostProcessComponent.h"
#include "VitrumPostProcessVolume.generated.h"

UCLASS()
class VITRUMAXIS_API AVitrumPostProcessVolume : public AVolume
{
    GENERATED_BODY()

public:
    AVitrumPostProcessVolume();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PostProcess")
    UPostProcessComponent* PostProcessComponent;

    // Set world-specific settings (heat haze, refractions) in BP
};
