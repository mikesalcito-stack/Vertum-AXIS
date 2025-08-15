#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VitrumMainMenuWidget.generated.h"

UCLASS()
class VITRUMAXIS_API UVitrumMainMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Menu")
    void StartGame();

    UFUNCTION(BlueprintCallable, Category = "Menu")
    void OpenSettings();

    UFUNCTION(BlueprintCallable, Category = "Menu")
    void QuitGame();

    // Buttons and text would be bound in Blueprint
};
