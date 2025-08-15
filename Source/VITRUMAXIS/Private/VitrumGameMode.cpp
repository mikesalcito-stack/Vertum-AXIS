#include "VitrumGameMode.h"
#include "Engine/World.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

AVitrumGameMode::AVitrumGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
    
    // Set default values
    CurrentGameState = EGameState::Flow;
    CurrentWorldRotation = EWorldRotation::North;
    TimeRemaining = 0.0f;
    bTimerActive = false;
    LevelDuration = 300.0f; // 5 minutes default
    
    CurrentWorldIndex = 1;
    CurrentLevelIndex = 1;
    
    // Create audio component
    CurrentMusicComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("MusicComponent"));
    RootComponent = CurrentMusicComponent;
}

void AVitrumGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    // Start with Flow state
    SetGameState(EGameState::Flow);
    
    // Start the level timer
    StartLevelTimer(LevelDuration);
}

void AVitrumGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    UpdateTimer(DeltaTime);
}

void AVitrumGameMode::SetGameState(EGameState NewState)
{
    if (CurrentGameState != NewState)
    {
        EGameState OldState = CurrentGameState;
        CurrentGameState = NewState;
        
        // Handle state-specific logic
        switch (NewState)
        {
            case EGameState::Flow:
                ResumeTimer();
                PlayStateMusic(EGameState::Flow);
                break;
                
            case EGameState::Solve:
                PauseTimer();
                PlayStateMusic(EGameState::Solve);
                break;
                
            case EGameState::Paused:
                PauseTimer();
                StopCurrentMusic();
                break;
                
            case EGameState::GameOver:
                PauseTimer();
                StopCurrentMusic();
                break;
        }
        
        // Broadcast the event
        OnGameStateChanged.Broadcast(NewState);
    }
}

void AVitrumGameMode::RotateWorldLeft()
{
    EWorldRotation NewRotation;
    
    switch (CurrentWorldRotation)
    {
        case EWorldRotation::North:
            NewRotation = EWorldRotation::West;
            break;
        case EWorldRotation::West:
            NewRotation = EWorldRotation::South;
            break;
        case EWorldRotation::South:
            NewRotation = EWorldRotation::East;
            break;
        case EWorldRotation::East:
            NewRotation = EWorldRotation::North;
            break;
    }
    
    CurrentWorldRotation = NewRotation;
    PlayRotationSound();
    OnWorldRotationChanged.Broadcast(NewRotation);
}

void AVitrumGameMode::RotateWorldRight()
{
    EWorldRotation NewRotation;
    
    switch (CurrentWorldRotation)
    {
        case EWorldRotation::North:
            NewRotation = EWorldRotation::East;
            break;
        case EWorldRotation::East:
            NewRotation = EWorldRotation::South;
            break;
        case EWorldRotation::South:
            NewRotation = EWorldRotation::West;
            break;
        case EWorldRotation::West:
            NewRotation = EWorldRotation::North;
            break;
    }
    
    CurrentWorldRotation = NewRotation;
    PlayRotationSound();
    OnWorldRotationChanged.Broadcast(NewRotation);
}

void AVitrumGameMode::StartLevelTimer(float Duration)
{
    LevelDuration = Duration;
    TimeRemaining = Duration;
    bTimerActive = true;
    OnTimerUpdated.Broadcast(TimeRemaining);
}

void AVitrumGameMode::PauseTimer()
{
    bTimerActive = false;
}

void AVitrumGameMode::ResumeTimer()
{
    bTimerActive = true;
}

void AVitrumGameMode::LoadNextLevel()
{
    CurrentLevelIndex++;
    
    // Check if we need to move to the next world
    if (CurrentLevelIndex > 12) // 12 levels per world
    {
        CurrentLevelIndex = 1;
        CurrentWorldIndex++;
    }
    
    // Restart the current level
    RestartLevel();
}

void AVitrumGameMode::RestartLevel()
{
    // Reset timer
    StartLevelTimer(LevelDuration);
    
    // Reset world rotation
    CurrentWorldRotation = EWorldRotation::North;
    OnWorldRotationChanged.Broadcast(CurrentWorldRotation);
    
    // Set back to Flow state
    SetGameState(EGameState::Flow);
}

void AVitrumGameMode::UpdateTimer(float DeltaTime)
{
    if (bTimerActive && CurrentGameState == EGameState::Flow)
    {
        TimeRemaining -= DeltaTime;
        
        if (TimeRemaining <= 0.0f)
        {
            TimeRemaining = 0.0f;
            SetGameState(EGameState::GameOver);
        }
        
        OnTimerUpdated.Broadcast(TimeRemaining);
    }
}

void AVitrumGameMode::PlayStateMusic(EGameState State)
{
    StopCurrentMusic();
    
    USoundCue* MusicToPlay = nullptr;
    
    switch (State)
    {
        case EGameState::Flow:
            MusicToPlay = FlowMusic;
            break;
        case EGameState::Solve:
            MusicToPlay = SolveMusic;
            break;
    }
    
    if (MusicToPlay && CurrentMusicComponent)
    {
        CurrentMusicComponent->SetSound(MusicToPlay);
        CurrentMusicComponent->Play();
    }
}

void AVitrumGameMode::StopCurrentMusic()
{
    if (CurrentMusicComponent)
    {
        CurrentMusicComponent->Stop();
    }
}

void AVitrumGameMode::PlayRotationSound()
{
    if (RotationSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, RotationSound, GetActorLocation());
    }
}