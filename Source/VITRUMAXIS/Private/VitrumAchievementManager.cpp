#include "VitrumAchievementManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

void AVitrumAchievementManager::UnlockAchievement(FName AchievementID)
{
    // Use platform-specific achievement system or save game
    // For now, just log the achievement
    UE_LOG(LogTemp, Log, TEXT("Achievement Unlocked: %s"), *AchievementID.ToString());
    
    // TODO: Implement proper save game system
    // UGameplayStatics::SaveGameToSlot(SaveGameObject, SlotName, UserIndex);
}

void AVitrumAchievementManager::CheckPerfectRun(float Efficiency)
{
    if (Efficiency > 0.9f) UnlockAchievement("PerfectFlow");
}
