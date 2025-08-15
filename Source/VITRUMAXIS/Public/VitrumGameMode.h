#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/World.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "VitrumGameMode.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
    Flow UMETA(DisplayName = "Flow"),
    Solve UMETA(DisplayName = "Solve"),
    Paused UMETA(DisplayName = "Paused"),
    GameOver UMETA(DisplayName = "GameOver")
};

UENUM(BlueprintType)
enum class EWorldRotation : uint8
{
    North UMETA(DisplayName = "North"),
    East UMETA(DisplayName = "East"),
    South UMETA(DisplayName = "South"),
    West UMETA(DisplayName = "West")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameStateChanged, EGameState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWorldRotationChanged, EWorldRotation, NewRotation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimerUpdated, float, TimeRemaining);

UCLASS(BlueprintType, Blueprintable)
class VITRUMAXIS_API AVitrumGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AVitrumGameMode();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

public:
    // Game State Management
    UFUNCTION(BlueprintCallable, Category = "Game State")
    void SetGameState(EGameState NewState);

    UFUNCTION(BlueprintPure, Category = "Game State")
    EGameState GetCurrentGameState() const { return CurrentGameState; }

    // World Rotation Management
    UFUNCTION(BlueprintCallable, Category = "World Rotation")
    void RotateWorldLeft();

    UFUNCTION(BlueprintCallable, Category = "World Rotation")
    void RotateWorldRight();

    UFUNCTION(BlueprintPure, Category = "World Rotation")
    EWorldRotation GetCurrentWorldRotation() const { return CurrentWorldRotation; }

    // Timer Management
    UFUNCTION(BlueprintCallable, Category = "Timer")
    void StartLevelTimer(float Duration);

    UFUNCTION(BlueprintCallable, Category = "Timer")
    void PauseTimer();

    UFUNCTION(BlueprintCallable, Category = "Timer")
    void ResumeTimer();

    UFUNCTION(BlueprintPure, Category = "Timer")
    float GetTimeRemaining() const { return TimeRemaining; }

    // Level Management
    UFUNCTION(BlueprintCallable, Category = "Level")
    void LoadNextLevel();

    UFUNCTION(BlueprintCallable, Category = "Level")
    void RestartLevel();

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnGameStateChanged OnGameStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnWorldRotationChanged OnWorldRotationChanged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnTimerUpdated OnTimerUpdated;

protected:
    // Game State
    UPROPERTY(BlueprintReadOnly, Category = "Game State")
    EGameState CurrentGameState;

    // World Rotation
    UPROPERTY(BlueprintReadOnly, Category = "World Rotation")
    EWorldRotation CurrentWorldRotation;

    // Timer
    UPROPERTY(BlueprintReadOnly, Category = "Timer")
    float TimeRemaining;

    UPROPERTY(BlueprintReadOnly, Category = "Timer")
    bool bTimerActive;

    UPROPERTY(BlueprintReadOnly, Category = "Timer")
    float LevelDuration;

    // Audio
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundCue* FlowMusic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundCue* SolveMusic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundCue* RotationSound;

    UPROPERTY(BlueprintReadOnly, Category = "Audio")
    UAudioComponent* CurrentMusicComponent;

    // Level Data
    UPROPERTY(BlueprintReadOnly, Category = "Level")
    int32 CurrentWorldIndex;

    UPROPERTY(BlueprintReadOnly, Category = "Level")
    int32 CurrentLevelIndex;

private:
    void UpdateTimer(float DeltaTime);
    void PlayStateMusic(EGameState State);
    void StopCurrentMusic();
    void PlayRotationSound();
};
