#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VitrumPuzzleHintManager.generated.h"

UCLASS()
class VITRUMAXIS_API AVitrumPuzzleHintManager : public AActor
{
    GENERATED_BODY()

public:
    AVitrumPuzzleHintManager();

    UFUNCTION(BlueprintCallable, Category = "Hints")
    void ShowHint(AActor* Target, float Duration);

    UFUNCTION(BlueprintCallable, Category = "Hints")
    void ProvideHint(AActor* Target);

    // Properties for hint types (glow, audio)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hints")
	class UNiagaraSystem* HintEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hints")
	class USoundBase* HintSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hints")
	class UNiagaraSystem* BasicHintEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hints")
	class USoundBase* BasicHintSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hints")
	class UNiagaraSystem* DetailedHintEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hints")
	class USoundBase* DetailedHintSound;

private:
    int32 HintLevel;
};
