#include "VitrumEnergyReceptor.h"
#include "Components/StaticMeshComponent.h"

AVitrumEnergyReceptor::AVitrumEnergyReceptor()
{
    UStaticMeshComponent* Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    Tags.Add(TEXT("PuzzleElement"));
    Tags.Add(TEXT("Rotatable"));
}

void AVitrumEnergyReceptor::ReceiveFlow()
{
    bIsPowered = true;
    OnPowered(); // Trigger linked events, e.g., open door
}
