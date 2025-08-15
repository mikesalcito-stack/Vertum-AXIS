#include "VitrumDialogueManager.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/TextRenderComponent.h"

AVitrumDialogueManager::AVitrumDialogueManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AVitrumDialogueManager::PlayDialogue(const FDialogueLine& Line)
{
    // Display text (assume HUD or text render)
    // For simplicity, use debug message or add to HUD
    GEngine->AddOnScreenDebugMessage(-1, Line.DisplayTime, FColor::White, Line.Text.ToString());

    if (Line.VoiceCue)
    {
        UGameplayStatics::PlaySound2D(GetWorld(), Line.VoiceCue);
    }
}

void AVitrumDialogueManager::PlayStorySequence(FName SequenceID)
{
    if (StorySequences.Contains(SequenceID))
    {
        const TArray<FDialogueLine>& Lines = StorySequences[SequenceID];
        float Delay = 0.0f;
        for (const FDialogueLine& Line : Lines)
        {
            FTimerHandle Handle;
            GetWorldTimerManager().SetTimer(Handle, [this, Line]() { PlayDialogue(Line); }, Delay, false);
            Delay += Line.DisplayTime + 1.0f; // Buffer between lines
        }
    }
}
