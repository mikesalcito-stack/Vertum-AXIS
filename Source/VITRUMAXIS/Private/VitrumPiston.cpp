#include "VitrumPiston.h"
#include "Components/StaticMeshComponent.h"

AVitrumPiston::AVitrumPiston()
{
    PrimaryActorTick.bCanEverTick = true;

    UStaticMeshComponent* PistonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PistonMesh"));
    RootComponent = PistonMesh;

    Tags.Add(TEXT("PuzzleElement"));
    Tags.Add(TEXT("Rotatable"));
}

void AVitrumPiston::Extend()
{
    bIsExtended = true;
    // Use timeline or tick to animate extension
}

void AVitrumPiston::Retract()
{
    bIsExtended = false;
    // Animate retraction
}
