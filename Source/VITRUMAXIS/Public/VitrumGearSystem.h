#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "VitrumGearSystem.generated.h"

UCLASS()
class VITRUMAXIS_API AVitrumGearSystem : public AActor
{
    GENERATED_BODY()

public:
    AVitrumGearSystem();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TArray<UStaticMeshComponent*> Gears;

    // Gear Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gears")
    float RotationSpeed = 30.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gears")
    bool bIsRotating = false;

    // Functions
    UFUNCTION(BlueprintCallable, Category = "Gears")
    void StartRotation();

    UFUNCTION(BlueprintCallable, Category = "Gears")
    void StopRotation();

    UFUNCTION(BlueprintCallable, Category = "Gears")
    void UpdateGearPositions(FRotator WorldRotation);

    // Events
    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnRotationStarted();

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnRotationStopped();
};
