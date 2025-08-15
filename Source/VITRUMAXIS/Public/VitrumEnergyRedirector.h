#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "VitrumEnergyRedirector.generated.h"

UCLASS()
class VITRUMAXIS_API AVitrumEnergyRedirector : public AActor
{
    GENERATED_BODY()

public:
    AVitrumEnergyRedirector();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Redirector")
    FVector OutputDirection = FVector(1.0f, 0.0f, 0.0f);

    UFUNCTION(BlueprintCallable, Category = "Redirector")
    void RedirectFlow(FVector IncomingDirection);
};
