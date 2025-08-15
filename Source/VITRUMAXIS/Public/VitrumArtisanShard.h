#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "VitrumArtisanShard.generated.h"

UENUM(BlueprintType)
enum class EShardType : uint8
{
    Standard     UMETA(DisplayName = "Standard"),
    Rare         UMETA(DisplayName = "Rare"),
    Legendary    UMETA(DisplayName = "Legendary"),
    Master       UMETA(DisplayName = "Master")
};

UENUM(BlueprintType)
enum class EShardState : uint8
{
    Idle         UMETA(DisplayName = "Idle"),
    Collected    UMETA(DisplayName = "Collected"),
    Consumed     UMETA(DisplayName = "Consumed")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShardCollected, int32, ShardIndex);

UCLASS(BlueprintType, Blueprintable)
class VITRUMAXIS_API AVitrumArtisanShard : public AActor
{
    GENERATED_BODY()

public:
    AVitrumArtisanShard();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

public:
    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* CollisionSphere;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* ShardMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UParticleSystemComponent* IdleParticles;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UParticleSystemComponent* CollectionParticles;

    // Shard Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shard")
    int32 ShardIndex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shard")
    EShardType ShardType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shard")
    EShardState ShardState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shard")
    FString ShardName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shard")
    FString ShardDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shard")
    int32 ShardValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shard")
    TArray<FName> UnlockedAbilities;

    // Visual Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    float RotationSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    float BobSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    float BobHeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    FLinearColor ShardColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    float PulseSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    float PulseIntensity;

    // Audio
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundCue* IdleSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundCue* CollectionSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    float IdleSoundVolume;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    float CollectionSoundVolume;

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnShardCollected OnShardCollected;

    // Public Functions
    UFUNCTION(BlueprintCallable, Category = "Shard")
    void CollectShard();

    UFUNCTION(BlueprintCallable, Category = "Shard")
    void ConsumeShard();

    UFUNCTION(BlueprintCallable, Category = "Shard")
    bool IsCollected() const;

    UFUNCTION(BlueprintCallable, Category = "Shard")
    bool IsConsumed() const;

    UFUNCTION(BlueprintCallable, Category = "Shard")
    void ResetShard();

    UFUNCTION(BlueprintCallable, Category = "Shard")
    FString GetShardName() const;

    UFUNCTION(BlueprintCallable, Category = "Shard")
    FString GetShardDescription() const;

    UFUNCTION(BlueprintCallable, Category = "Shard")
    int32 GetShardValue() const;

    UFUNCTION(BlueprintCallable, Category = "Shard")
    TArray<FName> GetUnlockedAbilities() const;

protected:
    // Internal Functions
    void UpdateVisuals(float DeltaTime);
    void UpdateAudio();
    void PlayCollectionEffects();
    void UpdateMaterialProperties();
    void StartIdleSound();
    void StopIdleSound();
    void InitializeShardProperties();

    // Overlap Events
    UFUNCTION()
    void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
    // Internal State
    float InitialZ;
    float CurrentRotation;
    float CurrentBob;
    float CurrentPulse;
    UAudioComponent* IdleAudioComponent;
    UMaterialInstanceDynamic* ShardMaterial;
    bool bIsInitialized;
};
