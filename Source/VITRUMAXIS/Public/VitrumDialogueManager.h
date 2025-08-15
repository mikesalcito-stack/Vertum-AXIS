#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "Sound/SoundCue.h"
#include "VitrumDialogueManager.generated.h"

USTRUCT(BlueprintType)
struct FDialogueLine
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Text;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USoundCue* VoiceCue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DisplayTime = 5.0f;
};

USTRUCT(BlueprintType)
struct FDialogueSequence
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDialogueLine> Lines;
};

UCLASS()
class VITRUMAXIS_API AVitrumDialogueManager : public AActor
{
    GENERATED_BODY()

public:
    AVitrumDialogueManager();

    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void PlayDialogue(const FDialogueLine& Line);

    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void PlayStorySequence(FName SequenceID); // For level-specific stories

    // Map of sequences (e.g., "Awakening" -> array of lines)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    TMap<FName, FDialogueSequence> StorySequences;
};
