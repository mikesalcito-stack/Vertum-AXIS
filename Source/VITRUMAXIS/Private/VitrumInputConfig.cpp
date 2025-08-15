#include "VitrumInputConfig.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "VitrumMarble.h"
#include "VitrumGameMode.h"
#include "VitrumWorldManager.h"

void UVitrumInputConfig::SetupInputMappings(UEnhancedInputComponent* EnhancedInputComponent, AVitrumMarble* PlayerPawn)
{
    if (!EnhancedInputComponent || !PlayerPawn)
    {
        return;
    }

    // Movement Actions
    if (MoveAction)
    {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, PlayerPawn, &AVitrumMarble::MoveFromInputAction);
    }

    if (JumpAction)
    {
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, PlayerPawn, &AVitrumMarble::Jump);
    }

    if (DashAction)
    {
        EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, PlayerPawn, &AVitrumMarble::Dash);
    }

    if (TractionFieldAction)
    {
        EnhancedInputComponent->BindAction(TractionFieldAction, ETriggerEvent::Started, PlayerPawn, &AVitrumMarble::ActivateTractionField);
        EnhancedInputComponent->BindAction(TractionFieldAction, ETriggerEvent::Completed, PlayerPawn, &AVitrumMarble::DeactivateTractionField);
    }

    // Perspective Shift Actions
    if (RotateWorldLeftAction)
    {
        EnhancedInputComponent->BindAction(RotateWorldLeftAction, ETriggerEvent::Started, [this, PlayerPawn]()
        {
            if (AVitrumGameMode* GameMode = Cast<AVitrumGameMode>(PlayerPawn->GetWorld()->GetAuthGameMode()))
            {
                GameMode->RotateWorldLeft();
            }
        });
    }

    if (RotateWorldRightAction)
    {
        EnhancedInputComponent->BindAction(RotateWorldRightAction, ETriggerEvent::Started, [this, PlayerPawn]()
        {
            if (AVitrumGameMode* GameMode = Cast<AVitrumGameMode>(PlayerPawn->GetWorld()->GetAuthGameMode()))
            {
                GameMode->RotateWorldRight();
            }
        });
    }

    if (ResetRotationAction)
    {
        EnhancedInputComponent->BindAction(ResetRotationAction, ETriggerEvent::Started, [this, PlayerPawn]()
        {
            // Find world manager and reset rotation
            TArray<AActor*> FoundActors;
            UGameplayStatics::GetAllActorsOfClass(PlayerPawn->GetWorld(), AVitrumWorldManager::StaticClass(), FoundActors);
            
            if (FoundActors.Num() > 0)
            {
                if (AVitrumWorldManager* WorldManager = Cast<AVitrumWorldManager>(FoundActors[0]))
                {
                    WorldManager->SetWorldRotation(EWorldRotation::North);
                }
            }
        });
    }

    // Game Actions
    if (PauseAction)
    {
        EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, [this, PlayerPawn]()
        {
            if (AVitrumGameMode* GameMode = Cast<AVitrumGameMode>(PlayerPawn->GetWorld()->GetAuthGameMode()))
            {
                GameMode->SetGameState(EGameState::Paused);
            }
        });
    }

    if (RestartAction)
    {
        EnhancedInputComponent->BindAction(RestartAction, ETriggerEvent::Started, [this, PlayerPawn]()
        {
            if (AVitrumGameMode* GameMode = Cast<AVitrumGameMode>(PlayerPawn->GetWorld()->GetAuthGameMode()))
            {
                GameMode->RestartLevel();
            }
        });
    }

    if (UseSubroutineAction)
    {
        EnhancedInputComponent->BindAction(UseSubroutineAction, ETriggerEvent::Started, [this, PlayerPawn]()
        {
            // TODO: Implement subroutine usage
            // This would be handled by the player pawn or a separate inventory system
        });
    }

    // Camera Actions
    if (FreeViewAction)
    {
        EnhancedInputComponent->BindAction(FreeViewAction, ETriggerEvent::Triggered, [this, PlayerPawn](const FInputActionValue& Value)
        {
            // TODO: Implement free camera view
            // This would allow the player to look around without moving
        });
    }

    if (ZoomInAction)
    {
        EnhancedInputComponent->BindAction(ZoomInAction, ETriggerEvent::Started, [this, PlayerPawn]()
        {
            // TODO: Implement camera zoom in
            if (PlayerPawn->SpringArm)
            {
                float CurrentLength = PlayerPawn->SpringArm->TargetArmLength;
                PlayerPawn->SpringArm->TargetArmLength = FMath::Max(200.0f, CurrentLength - 100.0f);
            }
        });
    }

    if (ZoomOutAction)
    {
        EnhancedInputComponent->BindAction(ZoomOutAction, ETriggerEvent::Started, [this, PlayerPawn]()
        {
            // TODO: Implement camera zoom out
            if (PlayerPawn->SpringArm)
            {
                float CurrentLength = PlayerPawn->SpringArm->TargetArmLength;
                PlayerPawn->SpringArm->TargetArmLength = FMath::Min(1200.0f, CurrentLength + 100.0f);
            }
        });
    }

    // UI Actions
    if (MenuAction)
    {
        EnhancedInputComponent->BindAction(MenuAction, ETriggerEvent::Started, [this, PlayerPawn]()
        {
            // TODO: Implement menu toggle
            // This would show/hide the pause menu or main menu
        });
    }

    if (ConfirmAction)
    {
        EnhancedInputComponent->BindAction(ConfirmAction, ETriggerEvent::Started, [this, PlayerPawn]()
        {
            // TODO: Implement confirm action for UI
        });
    }

    if (CancelAction)
    {
        EnhancedInputComponent->BindAction(CancelAction, ETriggerEvent::Started, [this, PlayerPawn]()
        {
            // TODO: Implement cancel action for UI
        });
    }

    if (NavigateAction)
    {
        EnhancedInputComponent->BindAction(NavigateAction, ETriggerEvent::Triggered, [this, PlayerPawn](const FInputActionValue& Value)
        {
            // TODO: Implement UI navigation
            // This would handle menu navigation with thumbstick/d-pad
        });
    }
}

void UVitrumInputConfig::SetupInputContext(UEnhancedInputLocalPlayerSubsystem* InputSubsystem)
{
    if (InputSubsystem && DefaultMappingContext)
    {
        InputSubsystem->AddMappingContext(DefaultMappingContext, 0);
    }
}

void UVitrumInputConfig::RemoveInputContext(UEnhancedInputLocalPlayerSubsystem* InputSubsystem)
{
    if (InputSubsystem && DefaultMappingContext)
    {
        InputSubsystem->RemoveMappingContext(DefaultMappingContext);
    }
}
