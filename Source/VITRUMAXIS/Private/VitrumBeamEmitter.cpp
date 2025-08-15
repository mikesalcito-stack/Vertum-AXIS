#include "VitrumBeamEmitter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "VitrumBeamRedirector.h"
#include "VitrumBeamReceptor.h"

AVitrumBeamEmitter::AVitrumBeamEmitter()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create emitter mesh
    EmitterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EmitterMesh"));
    RootComponent = EmitterMesh;
    EmitterMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // Create beam effect
    BeamEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BeamEffect"));
    BeamEffect->SetupAttachment(EmitterMesh);

    // Add tags
    Tags.Add(TEXT("PuzzleElement"));
    Tags.Add(TEXT("Rotatable"));
}

void AVitrumBeamEmitter::BeginPlay()
{
    Super::BeginPlay();

    UpdateBeam();
}

void AVitrumBeamEmitter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AVitrumBeamEmitter::ActivateEmitter()
{
    bIsActive = true;
    UpdateBeam();
    OnBeamActivated();
}

void AVitrumBeamEmitter::DeactivateEmitter()
{
    bIsActive = false;
    UpdateBeam();
    OnBeamDeactivated();
}

void AVitrumBeamEmitter::UpdateBeam()
{
    if (BeamEffect)
    {
        BeamEffect->SetFloatParameter(TEXT("Active"), bIsActive ? 1.0f : 0.0f);
        BeamEffect->SetLinearColorParameter(TEXT("BeamColor"), BeamColor);
    }

    if (bIsActive)
    {
        TraceBeam();
    }
}

void AVitrumBeamEmitter::TraceBeam()
{
    FVector Start = GetActorLocation();
    FVector End = Start + (BeamDirection.GetSafeNormal() * BeamLength);

    FHitResult HitResult;
    ECollisionChannel TraceChannel = ECC_Visibility; // Or custom channel

    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, TraceChannel))
    {
        // Check if hit a redirector or receptor
        if (AVitrumBeamRedirector* Redirector = Cast<AVitrumBeamRedirector>(HitResult.GetActor()))
        {
            // Redirect the beam
            Redirector->RedirectBeam(BeamColor, HitResult.ImpactNormal);
        }
        else if (AVitrumBeamReceptor* Receptor = Cast<AVitrumBeamReceptor>(HitResult.GetActor()))
        {
            // Activate receptor
            Receptor->ReceiveBeam(BeamColor);
        }

        // Update beam end point for visual
        if (BeamEffect)
        {
            BeamEffect->SetVectorParameter(TEXT("EndPoint"), HitResult.ImpactPoint);
        }
    }
    else
    {
        // Beam goes to max length
        if (BeamEffect)
        {
            BeamEffect->SetVectorParameter(TEXT("EndPoint"), End);
        }
    }
}
