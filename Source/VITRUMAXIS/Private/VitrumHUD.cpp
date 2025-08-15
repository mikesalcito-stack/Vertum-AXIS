#include "VitrumHUD.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "VitrumGameMode.h"
#include "VitrumMarble.h"

AVitrumHUD::AVitrumHUD()
{
    // Set default values
    CurrentHUDWidget = nullptr;
    FlowHUDWidget = nullptr;
    SolveHUDWidget = nullptr;
    GameOverWidget = nullptr;
    PauseWidget = nullptr;
    GameMode = nullptr;
    PlayerMarble = nullptr;
}

void AVitrumHUD::BeginPlay()
{
    Super::BeginPlay();

    // Get references
    GameMode = Cast<AVitrumGameMode>(GetWorld()->GetAuthGameMode());
    PlayerMarble = Cast<AVitrumMarble>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

    // Create widgets
    if (FlowHUDWidgetClass)
    {
        FlowHUDWidget = CreateWidget<UUserWidget>(GetWorld(), FlowHUDWidgetClass);
    }

    if (SolveHUDWidgetClass)
    {
        SolveHUDWidget = CreateWidget<UUserWidget>(GetWorld(), SolveHUDWidgetClass);
    }

    if (GameOverWidgetClass)
    {
        GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
    }

    if (PauseWidgetClass)
    {
        PauseWidget = CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass);
    }

    // Bind to game mode events
    if (GameMode)
    {
        GameMode->OnGameStateChanged.AddDynamic(this, &AVitrumHUD::OnGameStateChangedInternal);
        GameMode->OnTimerUpdated.AddDynamic(this, &AVitrumHUD::OnTimerUpdatedInternal);
        GameMode->OnWorldRotationChanged.AddDynamic(this, &AVitrumHUD::OnWorldRotationChangedInternal);
    }

    // Show initial HUD
    ShowFlowHUD();
}

void AVitrumHUD::ShowFlowHUD()
{
    HideAllHUD();

    if (FlowHUDWidget)
    {
        FlowHUDWidget->AddToViewport();
        CurrentHUDWidget = FlowHUDWidget;
    }
}

void AVitrumHUD::ShowSolveHUD()
{
    HideAllHUD();

    if (SolveHUDWidget)
    {
        SolveHUDWidget->AddToViewport();
        CurrentHUDWidget = SolveHUDWidget;
    }
}

void AVitrumHUD::ShowGameOverHUD()
{
    HideAllHUD();

    if (GameOverWidget)
    {
        GameOverWidget->AddToViewport();
        CurrentHUDWidget = GameOverWidget;
    }
}

void AVitrumHUD::ShowPauseHUD()
{
    HideAllHUD();

    if (PauseWidget)
    {
        PauseWidget->AddToViewport();
        CurrentHUDWidget = PauseWidget;
    }
}

void AVitrumHUD::HideAllHUD()
{
    if (FlowHUDWidget && FlowHUDWidget->IsInViewport())
    {
        FlowHUDWidget->RemoveFromViewport();
    }

    if (SolveHUDWidget && SolveHUDWidget->IsInViewport())
    {
        SolveHUDWidget->RemoveFromViewport();
    }

    if (GameOverWidget && GameOverWidget->IsInViewport())
    {
        GameOverWidget->RemoveFromViewport();
    }

    if (PauseWidget && PauseWidget->IsInViewport())
    {
        PauseWidget->RemoveFromViewport();
    }

    CurrentHUDWidget = nullptr;
}

void AVitrumHUD::UpdateTimerDisplay(float TimeRemaining)
{
    // This would typically update a timer widget
    // For now, we'll just broadcast the event
    OnTimerUpdated.Broadcast(TimeRemaining);
}

void AVitrumHUD::UpdateIntegrityDisplay(float Integrity)
{
    // This would update the integrity display
    // Could be a health bar or visual indicator
}

void AVitrumHUD::UpdateEnergyDisplay(float Energy)
{
    // This would update the energy display
    // Could be an energy bar or meter
}

void AVitrumHUD::UpdateWorldRotationDisplay(EWorldRotation Rotation)
{
    // This would update the rotation indicator
    // Could be a compass or rotation cube icon
    OnWorldRotationChanged.Broadcast(Rotation);
}

void AVitrumHUD::OnGameStateChangedInternal(EGameState NewState)
{
    switch (NewState)
    {
        case EGameState::Flow:
            ShowFlowHUD();
            break;
        case EGameState::Solve:
            ShowSolveHUD();
            break;
        case EGameState::GameOver:
            ShowGameOverHUD();
            break;
        case EGameState::Paused:
            ShowPauseHUD();
            break;
    }

    OnGameStateChanged.Broadcast(NewState);
}

void AVitrumHUD::OnTimerUpdatedInternal(float TimeRemaining)
{
    UpdateTimerDisplay(TimeRemaining);
}

void AVitrumHUD::OnWorldRotationChangedInternal(EWorldRotation NewRotation)
{
    UpdateWorldRotationDisplay(NewRotation);
}
