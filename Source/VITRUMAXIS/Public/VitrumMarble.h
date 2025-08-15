// Full VitrumMarble.h - compiled from history
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"
#include "VitrumGameMode.h"
#include "VITRUMAXIS.h"
#include "VitrumMarble.generated.h"

struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVitrumStateChanged, EVitrumState, NewState);

UENUM(BlueprintType)
enum class EMarbleState : uint8
{
    Normal UMETA(DisplayName = "Normal"),
    Damaged UMETA(DisplayName = "Damaged"),
    Critical UMETA(DisplayName = "Critical"),
    Shattered UMETA(DisplayName = "Shattered")
};

UCLASS(BlueprintType, Blueprintable)
class VITRUMAXIS_API AVitrumMarble : public APawn
{
    GENERATED_BODY()

public:
    AVitrumMarble();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
    // Core Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* CollisionSphere;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MarbleMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCameraComponent* Camera;

    // Physics Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
    float Mass = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
    float Friction = 0.3f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
    float Restitution = 0.7f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
    float MaxVelocity = 2000.0f;

    // Movement Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MovementForce = 5000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float JumpForce = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float AirControl = 0.5f;

    // Kinetic Drive Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinetic Drive")
    float DashForce = 8000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinetic Drive")
    float DashDuration = 0.3f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinetic Drive")
    float TractionFieldForce = 3000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinetic Drive")
    float EnergyRegenRate = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinetic Drive")
    float MaxEnergy = 100.0f;

    // Integrity System
    UPROPERTY(BlueprintReadOnly, Category = "Integrity")
    EVitrumState CurrentState;

    UPROPERTY(BlueprintReadOnly, Category = "Integrity")
    float Integrity = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Integrity")
    float MaxIntegrity = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Integrity")
    float DamageThreshold = 25.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Integrity")
    float CriticalThreshold = 10.0f;

    // Current State
    UPROPERTY(BlueprintReadOnly, Category = "State")
    float CurrentEnergy;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    bool bIsDashing;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    bool bIsGrounded;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    bool bTractionFieldActive;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    FVector LastCheckpoint;

    // Input Variables
    UPROPERTY(BlueprintReadOnly, Category = "Input")
    FVector MovementInput;

    UPROPERTY(BlueprintReadOnly, Category = "Input")
    bool bJumpPressed;

    UPROPERTY(BlueprintReadOnly, Category = "Input")
    bool bDashPressed;

    UPROPERTY(BlueprintReadOnly, Category = "Input")
    bool bTractionFieldPressed;

    // Audio
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundCue* RollSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundCue* JumpSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundCue* DashSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundCue* DamageSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundCue* ShatterSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundCue* HealSound;

    // Visual Effects
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UParticleSystem* DamageParticles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UParticleSystem* DashParticles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UParticleSystem* ShatterParticles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UParticleSystem* HealParticles;

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnVitrumStateChanged OnStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnWorldRotationChanged OnWorldRotationChanged;

    // Public Functions
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void Move(const FVector& Direction);

    void MoveFromInputAction(const FInputActionValue& Value);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void Jump();

    UFUNCTION(BlueprintCallable, Category = "Kinetic Drive")
    void Dash();

    UFUNCTION(BlueprintCallable, Category = "Kinetic Drive")
    void ActivateTractionField();

    UFUNCTION(BlueprintCallable, Category = "Kinetic Drive")
    void DeactivateTractionField();

    UFUNCTION(BlueprintCallable, Category = "Integrity")
    virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

    UFUNCTION(BlueprintCallable, Category = "Integrity")
    void Heal(float HealAmount);

    UFUNCTION(BlueprintCallable, Category = "Integrity")
    void Shatter();

    UFUNCTION(BlueprintCallable, Category = "Respawn")
    void Respawn();

    UFUNCTION(BlueprintCallable, Category = "Respawn")
    void SetCheckpoint(FVector Location);

    UFUNCTION(BlueprintCallable, Category = "State")
    void RepairIntegrity();

    UFUNCTION(BlueprintCallable, Category = "State")
    void ChangeState(EVitrumState NewState);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    FVector GetCurrentVelocity() const;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMaxSpeed() const { return MaxVelocity; }

    UFUNCTION(BlueprintCallable, Category = "Kinetic Drive")
    float GetCurrentCharge() const { return CurrentEnergy / MaxEnergy; }

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void ApplyMovementForce(const FVector& Direction, float Force);

protected:
    // Internal Functions
    void UpdatePhysics(float DeltaTime);
    void UpdateEnergy(float DeltaTime);
    void UpdateAudio();
    void UpdateVisualEffects();
    void ClampVelocity();
    void PlayRollSound();
    void StopRollSound();

    // Input Functions
    void MoveForward(float Value);
    void MoveRight(float Value);

private:
    // Internal State
    UPROPERTY()
    UAudioComponent* RollAudioComponent;

    UPROPERTY()
    UAudioComponent* EffectsAudioComponent;

    float DashTimer;
    float RollSoundTimer;
    FVector LastVelocity;
    bool bWasGrounded;
};