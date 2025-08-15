#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "VitrumGameMode.h"
#include "VitrumHUD.generated.h"

UCLASS(BlueprintType, Blueprintable)
class VITRUMAXIS_API AVitrumHUD : public AHUD
{
    GENERATED_BODY()

public:
    AVitrumHUD();

protected:
    virtual void BeginPlay() override;

public:
    // UI Widgets
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> FlowHUDWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> SolveHUDWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> GameOverWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PauseWidgetClass;

    UPROPERTY(BlueprintReadOnly, Category = "UI")
    UUserWidget* CurrentHUDWidget;

    UPROPERTY(BlueprintReadOnly, Category = "UI")
    UUserWidget* FlowHUDWidget;

    UPROPERTY(BlueprintReadOnly, Category = "UI")
    UUserWidget* SolveHUDWidget;

    UPROPERTY(BlueprintReadOnly, Category = "UI")
    UUserWidget* GameOverWidget;

    UPROPERTY(BlueprintReadOnly, Category = "UI")
    UUserWidget* PauseWidget;

    // HUD Management
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ShowFlowHUD();

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ShowSolveHUD();

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ShowGameOverHUD();

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ShowPauseHUD();

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void HideAllHUD();

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateTimerDisplay(float TimeRemaining);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateIntegrityDisplay(float Integrity);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateEnergyDisplay(float Energy);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateWorldRotationDisplay(EWorldRotation Rotation);

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnGameStateChanged OnGameStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnTimerUpdated OnTimerUpdated;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnWorldRotationChanged OnWorldRotationChanged;

protected:
    // Internal Functions
    void OnGameStateChangedInternal(EGameState NewState);
    void OnTimerUpdatedInternal(float TimeRemaining);
    void OnWorldRotationChangedInternal(EWorldRotation NewRotation);

private:
    // References
    AVitrumGameMode* GameMode;
    AVitrumMarble* PlayerMarble;
};
