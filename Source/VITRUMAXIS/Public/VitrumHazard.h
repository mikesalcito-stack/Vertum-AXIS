#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"
#include "VitrumHazard.generated.h"

UENUM(BlueprintType)
enum class EHazardType : uint8
{
    AcidPool UMETA(DisplayName = "Acid Pool"),
    Stalker UMETA(DisplayName = "Stalker"),
    NullZone UMETA(DisplayName = "Null Zone")
};

UENUM(BlueprintType)
enum class EHazardState : uint8
{
    Inactive UMETA(DisplayName = "Inactive"),
    Active UMETA(DisplayName = "Active"),
    Triggered UMETA(DisplayName = "Triggered")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHazardTriggered, AVitrumHazard*, Hazard);

UCLASS(BlueprintType, Blueprintable)
class VITRUMAXIS_API AVitrumHazard : public AActor
{
    GENERATED_BODY()

public:
    AVitrumHazard();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

public:
    // Core Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* RootScene;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* HazardMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* TriggerSphere;

    // Hazard Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
    EHazardType HazardType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
    EHazardState HazardState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
    float DamageAmount = 25.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
    float DamageInterval = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
    bool bIsLethal = false;

    // Movement Properties (for Stalkers)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MovementSpeed = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float DetectionRadius = 500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float ChaseRadius = 200.0f;

    // Visual Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    FLinearColor AcidColor = FLinearColor(0.2f, 0.8f, 0.2f, 0.8f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    FLinearColor StalkerColor = FLinearColor(0.1f, 0.1f, 0.1f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    FLinearColor NullZoneColor = FLinearColor(0.8f, 0.0f, 0.8f, 0.6f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    float PulseSpeed = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    float PulseIntensity = 0.5f;

    // Audio
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundCue* TriggerSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundCue* DamageSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundCue* AmbientSound;

    // Visual Effects
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UParticleSystem* TriggerParticles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UParticleSystem* DamageParticles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UParticleSystem* AmbientParticles;

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnHazardTriggered OnHazardTriggered;

    // Public Functions
    UFUNCTION(BlueprintCallable, Category = "Hazard")
    void ActivateHazard();

    UFUNCTION(BlueprintCallable, Category = "Hazard")
    void DeactivateHazard();

    UFUNCTION(BlueprintCallable, Category = "Hazard")
    void TriggerHazard();

    UFUNCTION(BlueprintPure, Category = "Hazard")
    bool IsActive() const { return HazardState == EHazardState::Active; }

    UFUNCTION(BlueprintPure, Category = "Hazard")
    bool IsLethal() const { return bIsLethal; }

protected:
    // Internal Functions
    void UpdateVisuals(float DeltaTime);
    void UpdateMovement(float DeltaTime);
    void UpdatePulse(float DeltaTime);
    void PlayTriggerEffects();
    void PlayDamageEffects();
    void PlayAmbientEffects();
    void UpdateMaterialColor(FLinearColor Color);
    void MoveTowardsPlayer(float DeltaTime);

private:
    // Internal State
    UPROPERTY()
    UMaterialInstanceDynamic* HazardMaterial;

    float DamageTimer;
    float PulseTimer;
    float CurrentPulseIntensity;
    bool bIsPulsing;
    FVector InitialLocation;
    class AVitrumMarble* TargetPlayer;
};
