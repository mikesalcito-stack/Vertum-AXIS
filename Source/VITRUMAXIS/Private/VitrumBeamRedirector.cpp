#include "VitrumBeamRedirector.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

AVitrumBeamRedirector::AVitrumBeamRedirector()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create redirector mesh
    RedirectorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RedirectorMesh"));
    RootComponent = RedirectorMesh;
    RedirectorMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

    // Create redirector effect
    RedirectorEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("RedirectorEffect"));
    RedirectorEffect->SetupAttachment(RedirectorMesh);

    // Add tags
    Tags.Add(TEXT("PuzzleElement"));
    Tags.Add(TEXT("Rotatable"));
}

void AVitrumBeamRedirector::BeginPlay()
{
    Super::BeginPlay();

    UpdateVisuals();
}

void AVitrumBeamRedirector::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Update RedirectBeam to handle mixing

void AVitrumBeamRedirector::RedirectBeam(FLinearColor IncomingColor, FVector IncomingDirection)
{
    if (RedirectorType == ERedirectorType::Mirror)
    {
        // Existing reflection logic
        FVector ReflectionDirection = FMath::GetReflectionVector(IncomingDirection, ReflectionNormal.GetSafeNormal());

        // Trace and propagate with same color
        FVector Start = GetActorLocation();
        FVector End = Start + (ReflectionDirection * 10000.0f);

        FHitResult HitResult;
        if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility))
        {
            if (AVitrumBeamRedirector* NextRedirector = Cast<AVitrumBeamRedirector>(HitResult.GetActor()))
            {
                NextRedirector->RedirectBeam(IncomingColor, ReflectionDirection);
            }
            else if (AVitrumBeamReceptor* Receptor = Cast<AVitrumBeamReceptor>(HitResult.GetActor()))
            {
                Receptor->ReceiveBeam(IncomingColor);
            }
        }

        // Visual
        if (RedirectorEffect)
        {
            RedirectorEffect->SetVectorParameter(TEXT("EndPoint"), HitResult.bBlockingHit ? HitResult.ImpactPoint : End);
            RedirectorEffect->SetLinearColorParameter(TEXT("BeamColor"), IncomingColor);
        }
    }
    else if (RedirectorType == ERedirectorType::Prism)
    {
        // Collect incoming color
        if (!IncomingColors.Contains(IncomingColor))
        {
            IncomingColors.Add(IncomingColor);
        }

        // Mix colors: simple average for now
        FLinearColor SumColor = FLinearColor::Black;
        for (FLinearColor Color : IncomingColors)
        {
            SumColor += Color;
        }
        MixedColor = SumColor / IncomingColors.Num();

        // Redirect mixed color in fixed direction or multiple? Assuming single output for simplicity
        FVector OutputDirection = GetActorForwardVector(); // Assuming prism has a fixed output direction

        // Trace and propagate mixed color
        FVector Start = GetActorLocation();
        FVector End = Start + (OutputDirection * 10000.0f);

        FHitResult HitResult;
        if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility))
        {
            if (AVitrumBeamRedirector* NextRedirector = Cast<AVitrumBeamRedirector>(HitResult.GetActor()))
            {
                NextRedirector->RedirectBeam(MixedColor, OutputDirection);
            }
            else if (AVitrumBeamReceptor* Receptor = Cast<AVitrumBeamReceptor>(HitResult.GetActor()))
            {
                Receptor->ReceiveBeam(MixedColor);
            }
        }

        // Visual for output
        if (RedirectorEffect)
        {
            RedirectorEffect->SetVectorParameter(TEXT("EndPoint"), HitResult.bBlockingHit ? HitResult.ImpactPoint : End);
            RedirectorEffect->SetLinearColorParameter(TEXT("BeamColor"), MixedColor);
        }
    }

    UpdateVisuals();
}

// In Tick or some reset mechanism, clear IncomingColors if no recent hits, but for simplicity, assume beams are continuous

void AVitrumBeamRedirector::UpdateVisuals()
{
    // Update based on type
    if (RedirectorEffect)
    {
        RedirectorEffect->SetFloatParameter(TEXT("Type"), RedirectorType == ERedirectorType::Mirror ? 0.0f : 1.0f);
    }
}
