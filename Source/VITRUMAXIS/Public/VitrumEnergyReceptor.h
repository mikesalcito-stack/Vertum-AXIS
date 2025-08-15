#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "VitrumEnergyReceptor.generated.h"

UCLASS()
class VITRUMAXIS_API AVitrumEnergyReceptor : public AActor
{
    GENERATED_BODY()

public:
    AVitrumEnergyReceptor();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Receptor")
    bool bIsPowered = false;

    UFUNCTION(BlueprintCallable, Category = "Receptor")
    void ReceiveFlow();

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnPowered();
};
