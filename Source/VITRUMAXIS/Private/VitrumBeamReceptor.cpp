#include "VitrumBeamReceptor.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"

AVitrumBeamReceptor::AVitrumBeamReceptor()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create receptor mesh
    ReceptorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ReceptorMesh"));
    RootComponent = ReceptorMesh;
    ReceptorMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

    // Create receptor effect
    ReceptorEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ReceptorEffect"));
    ReceptorEffect->SetupAttachment(ReceptorMesh);

    // Add tags
    Tags.Add(TEXT("PuzzleElement"));
    Tags.Add(TEXT("Rotatable"));
}

void AVitrumBeamReceptor::BeginPlay()
{
    Super::BeginPlay();

    UpdateVisuals();
}

void AVitrumBeamReceptor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AVitrumBeamReceptor::ReceiveBeam(FLinearColor IncomingColor)
{
    // Check if color matches
    if (IncomingColor.Equals(RequiredColor))
    {
        bIsActivated = true;
        UpdateVisuals();
        OnActivated();
    }
    else
    {
        Deactivate();
    }
}

void AVitrumBeamReceptor::Deactivate()
{
    bIsActivated = false;
    UpdateVisuals();
    OnDeactivated();
}

void AVitrumBeamReceptor::UpdateVisuals()
{
    if (ReceptorEffect)
    {
        ReceptorEffect->SetFloatParameter(TEXT("Activated"), bIsActivated ? 1.0f : 0.0f);
        ReceptorEffect->SetLinearColorParameter(TEXT("Color"), RequiredColor);
    }
}
