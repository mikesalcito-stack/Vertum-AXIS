#include "VitrumConductivityPuzzle.h"
#include "VitrumMarble.h"
#include "Kismet/GameplayStatics.h"

AVitrumConductivityPuzzle::AVitrumConductivityPuzzle()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AVitrumConductivityPuzzle::CheckCircuitCompletion(AVitrumMarble* Marble)
{
    if (Marble->CurrentState == EVitrumState::Metal)
    {
        // Check if marble is between two contacts (use overlap or distance)
        // If yes, set bIsComplete = true, activate linked elements
    }
}
