#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "VitrumSwitch.generated.h"

UENUM(BlueprintType)
enum class ESwitchType : uint8
{
    Toggle UMETA(DisplayName = "Toggle"),
    PressurePlate UMETA(DisplayName = "Pressure Plate")
};

UCLASS()
class VITRUMAXIS_API AVitrumSwitch : public AActor
{
    GENERATED_BODY()

public:
    AVitrumSwitch();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* SwitchMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* InteractionBox;

    // Switch Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch")
    ESwitchType SwitchType = ESwitchType::Toggle;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Switch")
    bool bIsActivated = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch")
    float RequiredWeight = 1.0f; // For pressure plates

    // Linked Actors
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch")
    TArray<AActor*> LinkedActors;

    // Functions
    UFUNCTION(BlueprintCallable, Category = "Switch")
    void Activate();

    UFUNCTION(BlueprintCallable, Category = "Switch")
    void Deactivate();

    UFUNCTION(BlueprintCallable, Category = "Switch")
    void Toggle();

    // Events
    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnActivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnDeactivated();

private:
    // Helper functions
    void UpdateVisuals();
    void NotifyLinkedActors(bool bActive);
};
