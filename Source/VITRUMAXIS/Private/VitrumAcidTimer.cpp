#include "VitrumAcidTimer.h"
#include "VitrumMarble.h"
#include "TimerManager.h"

AVitrumAcidTimer::AVitrumAcidTimer()
{
    PrimaryActorTick.bCanEverTick = false;
}

UPROPERTY(EditAnywhere)
UCurveFloat* DamageCurve;

UPROPERTY(EditAnywhere)
USoundBase* WarningSound;

void AVitrumAcidTimer::StartAcidTimer(AVitrumMarble* Marble)
{
    float Elapsed = 0.0f;
    GetWorldTimerManager().SetTimer(AcidTimerHandle, [this, Marble, &Elapsed]() {
        Elapsed += AcidDamageInterval;
        float ScaledDamage = AcidDamageAmount * (DamageCurve ? DamageCurve->GetFloatValue(Elapsed) : 1.0f);
        if (Marble) 
        {
            Marble->TakeDamage(ScaledDamage);
            // Warning if high damage
            if (ScaledDamage > 15.0f) UGameplayStatics::PlaySoundAtLocation(GetWorld(), WarningSound, Marble->GetActorLocation());
        }
    }, AcidDamageInterval, true);
}

void AVitrumAcidTimer::StopAcidTimer()
{
    GetWorldTimerManager().ClearTimer(AcidTimerHandle);
}
