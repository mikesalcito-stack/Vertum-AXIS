#include "VitrumMainMenuWidget.h"
#include "Kismet/GameplayStatics.h"

void UVitrumMainMenuWidget::StartGame()
{
    UGameplayStatics::OpenLevel(GetWorld(), FName("World1_Level1"));
}

void UVitrumMainMenuWidget::OpenSettings()
{
    // Open settings menu widget
}

void UVitrumMainMenuWidget::QuitGame()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
