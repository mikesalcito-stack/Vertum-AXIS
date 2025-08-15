#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VitrumAchievementManager.generated.h"

UCLASS()
class VITRUMAXIS_API AVitrumAchievementManager : public AActor
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Achievements")
    void UnlockAchievement(FName AchievementID);

    UFUNCTION(BlueprintCallable, Category = "Achievements")
    void CheckPerfectRun(float Efficiency);
};
