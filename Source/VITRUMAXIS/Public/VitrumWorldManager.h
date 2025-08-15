#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/SceneComponent.h"
#include "VitrumGameMode.h"
#include "VitrumWorldManager.generated.h"

UCLASS(BlueprintType, Blueprintable)
class VITRUMAXIS_API AVitrumWorldManager : public AActor
{
    GENERATED_BODY()

public:
    AVitrumWorldManager();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

public:
    // Core Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* WorldRoot;

    // Rotation Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
    float RotationSpeed = 90.0f; // Degrees per second

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
    float RotationDuration = 0.5f; // Seconds to complete rotation

    UPROPERTY(BlueprintReadOnly, Category = "Rotation")
    bool bIsRotating;

    UPROPERTY(BlueprintReadOnly, Category = "Rotation")
    EWorldRotation TargetRotation;

    UPROPERTY(BlueprintReadOnly, Category = "Rotation")
    EWorldRotation CurrentRotation;

    // Rotation Functions
    UFUNCTION(BlueprintCallable, Category = "Rotation")
    void RotateWorldLeft();

    UFUNCTION(BlueprintCallable, Category = "Rotation")
    void RotateWorldRight();

    UFUNCTION(BlueprintCallable, Category = "Rotation")
    void SetWorldRotation(EWorldRotation NewRotation);

    UFUNCTION(BlueprintPure, Category = "Rotation")
    EWorldRotation GetCurrentRotation() const { return CurrentRotation; }

    UFUNCTION(BlueprintPure, Category = "Rotation")
    bool IsRotating() const { return bIsRotating; }

    // World Management
    UFUNCTION(BlueprintCallable, Category = "World")
    void RegisterRotatableActor(AActor* Actor);

    UFUNCTION(BlueprintCallable, Category = "World")
    void UnregisterRotatableActor(AActor* Actor);

    UFUNCTION(BlueprintCallable, Category = "World")
    void ApplyRotationToActor(AActor* Actor, EWorldRotation Rotation);

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnWorldRotationChanged OnRotationChanged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnWorldRotationChanged OnRotationComplete;

protected:
    // Internal Functions
    void UpdateRotation(float DeltaTime);
    void StartRotation(EWorldRotation NewTarget);
    void CompleteRotation();
    FRotator GetRotationForDirection(EWorldRotation Direction);
    void ApplyRotationToAllActors(EWorldRotation Rotation);

private:
    // Internal State
    UPROPERTY()
    TArray<AActor*> RotatableActors;

    float RotationTimer;
    FRotator RotationStart;
    FRotator EndRotation;
    AVitrumGameMode* GameMode;
};
