#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VitrumAcidTimer.generated.h"

UCLASS()
class VITRUMAXIS_API AVitrumAcidTimer : public AActor
{
    GENERATED_BODY()

public:
    AVitrumAcidTimer();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Acid")
    float AcidDamageInterval = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Acid")
    float AcidDamageAmount = 10.0f;

    UFUNCTION(BlueprintCallable, Category = "Acid")
    void StartAcidTimer(AVitrumMarble* Marble);

    UFUNCTION(BlueprintCallable, Category = "Acid")
    void StopAcidTimer();

private:
    FTimerHandle AcidTimerHandle;
};
