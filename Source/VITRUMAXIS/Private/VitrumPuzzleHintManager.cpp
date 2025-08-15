#include "VitrumPuzzleHintManager.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

AVitrumPuzzleHintManager::AVitrumPuzzleHintManager()
{
    PrimaryActorTick.bCanEverTick = false;
    HintLevel = 0;
}

void AVitrumPuzzleHintManager::ShowHint(AActor* Target, float Duration)
{
    if (Target && HintEffect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HintEffect, Target->GetActorLocation());
    }
    
    if (Target && HintSound)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), HintSound, Target->GetActorLocation());
    }
}

void AVitrumPuzzleHintManager::ProvideHint(AActor* Target)
{
    if (!Target) return;

    if (HintLevel == 0)
    {
        // Basic hint
        if (BasicHintEffect)
        {
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BasicHintEffect, Target->GetActorLocation());
        }
        if (BasicHintSound)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), BasicHintSound, Target->GetActorLocation());
        }
        HintLevel++;
    }
    else if (HintLevel == 1)
    {
        // Detailed hint
        if (DetailedHintEffect)
        {
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DetailedHintEffect, Target->GetActorLocation());
        }
        if (DetailedHintSound)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), DetailedHintSound, Target->GetActorLocation());
        }
        HintLevel++;
    }
}

// Add properties in .h if needed:
UPROPERTY(EditAnywhere) class UNiagaraSystem* BasicHintEffect;
UPROPERTY(EditAnywhere) class USoundCue* BasicHintSound;
UPROPERTY(EditAnywhere) class UNiagaraSystem* DetailedHintEffect;
UPROPERTY(EditAnywhere) class USoundCue* DetailedHintSound;
int32 HintLevel = 0;