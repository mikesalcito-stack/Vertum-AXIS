#include "VitrumPostProcessVolume.h"
#include "Engine/PostProcessVolume.h"
#include "Components/PostProcessComponent.h"

AVitrumPostProcessVolume::AVitrumPostProcessVolume()
{
    PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcess"));
    PostProcessComponent->bUnbound = false; // Bound to volume
    // In constructor, set default settings

    // PostProcessComponent->Settings.AddBlendable(HeatDistortionMaterial, 1.0f); // Example - commented out for now
}
