#include "VitrumEnergyRedirector.h"
#include "Kismet/GameplayStatics.h"
#include "VitrumEnergyReceptor.h"

AVitrumEnergyRedirector::AVitrumEnergyRedirector()
{
    PrimaryActorTick.bCanEverTick = false;

    UStaticMeshComponent* Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    Tags.Add(TEXT("PuzzleElement"));
    Tags.Add(TEXT("Rotatable"));
}

void AVitrumEnergyRedirector::RedirectFlow(FVector IncomingDirection)
{
    // Simple redirection to output direction
    FVector Start = GetActorLocation();
    FVector End = Start + OutputDirection * 10000.0f;

    FHitResult Hit;
    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility))
    {
        if (AVitrumEnergyRedirector* Next = Cast<AVitrumEnergyRedirector>(Hit.GetActor()))
        {
            Next->RedirectFlow(OutputDirection);
        }
        else if (AVitrumEnergyReceptor* Receptor = Cast<AVitrumEnergyReceptor>(Hit.GetActor()))
        {
            Receptor->ReceiveFlow();
        }
    }
}
