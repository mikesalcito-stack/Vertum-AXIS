#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "VitrumInputConfig.generated.h"

UCLASS(BlueprintType, Blueprintable)
class VITRUMAXIS_API UVitrumInputConfig : public UDataAsset
{
    GENERATED_BODY()

public:
    // Input Mapping Context
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputMappingContext* DefaultMappingContext;

    // Movement Actions
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
    UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
    UInputAction* DashAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
    UInputAction* TractionFieldAction;

    // Perspective Shift Actions
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Perspective")
    UInputAction* RotateWorldLeftAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Perspective")
    UInputAction* RotateWorldRightAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Perspective")
    UInputAction* ResetRotationAction;

    // Game Actions
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
    UInputAction* PauseAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
    UInputAction* RestartAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
    UInputAction* UseSubroutineAction;

    // Camera Actions
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
    UInputAction* FreeViewAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
    UInputAction* ZoomInAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
    UInputAction* ZoomOutAction;

    // UI Actions
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    UInputAction* MenuAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    UInputAction* ConfirmAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    UInputAction* CancelAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    UInputAction* NavigateAction;

public:
    // Helper Functions
    UFUNCTION(BlueprintCallable, Category = "Input")
    void SetupInputMappings(class UEnhancedInputComponent* EnhancedInputComponent, class AVitrumMarble* PlayerPawn);

    UFUNCTION(BlueprintCallable, Category = "Input")
    void SetupInputContext(class UEnhancedInputLocalPlayerSubsystem* InputSubsystem);

    UFUNCTION(BlueprintCallable, Category = "Input")
    void RemoveInputContext(class UEnhancedInputLocalPlayerSubsystem* InputSubsystem);
};
