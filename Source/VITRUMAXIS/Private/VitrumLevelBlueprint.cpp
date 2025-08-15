#include "VitrumLevelBlueprint.h"
#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/StaticMesh.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Engine/PointLight.h"
#include "Engine/PostProcessVolume.h"
#include "Components/StaticMeshComponent.h"
#include "Components/LightComponent.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Materials/MaterialInterface.h"
#include "Kismet/GameplayStatics.h"
#include "VitrumAlignmentNode.h"
#include "VitrumHazard.h"
#include "VitrumArtisanShard.h"
#include "VitrumMarble.h"
#include "VitrumWorldManager.h"

UVitrumLevelBlueprint::UVitrumLevelBlueprint()
{
    // Set default values
    LevelID = NAME_None;
    LevelName = TEXT("Default Level");
    LevelType = ELevelType::Tutorial;
    LevelDescription = TEXT("A default level template.");
    TargetCompletionTime = 60.0f;
    DifficultyRating = 1;

    // Gameplay defaults
    RequiredAlignmentNodes = 2;
    TotalArtisanShards = 0;
    bHasHazards = false;
    bHasPuzzles = false;
    bRequiresPerspectiveShift = false;

    // Audio defaults
    MusicVolume = 0.7f;
    VictorySoundVolume = 1.0f;
}

void UVitrumLevelBlueprint::GenerateLevel(UWorld* World)
{
    if (!World)
    {
        return;
    }

    // Spawn level elements
    SpawnGeometry(World);
    SpawnLighting(World);
    SpawnGameplayElements(World);
    SpawnAudio(World);
    SpawnParticles(World);
}

FVector UVitrumLevelBlueprint::GetPlayerSpawnPosition() const
{
    if (Sections.Num() > 0)
    {
        return Sections[0].PlayerSpawnPosition;
    }
    return FVector::ZeroVector;
}

FVector UVitrumLevelBlueprint::GetLevelExitPosition() const
{
    if (Sections.Num() > 0)
    {
        return Sections[0].LevelExitPosition;
    }
    return FVector::ZeroVector;
}

TArray<FVector> UVitrumLevelBlueprint::GetAlignmentNodePositions() const
{
    TArray<FVector> AllPositions;
    
    for (const FLevelSection& Section : Sections)
    {
        AllPositions.Append(Section.AlignmentNodePositions);
    }
    
    return AllPositions;
}

TArray<FVector> UVitrumLevelBlueprint::GetHazardPositions() const
{
    TArray<FVector> AllPositions;
    
    for (const FLevelSection& Section : Sections)
    {
        AllPositions.Append(Section.HazardPositions);
    }
    
    return AllPositions;
}

TArray<FVector> UVitrumLevelBlueprint::GetArtisanShardPositions() const
{
    TArray<FVector> AllPositions;
    
    for (const FLevelSection& Section : Sections)
    {
        AllPositions.Append(Section.ArtisanShardPositions);
    }
    
    return AllPositions;
}

TArray<FVector> UVitrumLevelBlueprint::GetCheckpointPositions() const
{
    TArray<FVector> AllPositions;
    
    for (const FLevelSection& Section : Sections)
    {
        AllPositions.Append(Section.CheckpointPositions);
    }
    
    return AllPositions;
}

bool UVitrumLevelBlueprint::IsFlowLevel() const
{
    return LevelType == ELevelType::Flow;
}

bool UVitrumLevelBlueprint::IsSolveLevel() const
{
    return LevelType == ELevelType::Solve;
}

bool UVitrumLevelBlueprint::IsTutorialLevel() const
{
    return LevelType == ELevelType::Tutorial;
}

void UVitrumLevelBlueprint::SpawnGeometry(UWorld* World)
{
    // Spawn floor geometry
    if (Geometry.FloorMesh)
    {
        for (const FLevelSection& Section : Sections)
        {
            AStaticMeshActor* FloorActor = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Section.SectionLocation, Section.SectionRotation);
            if (FloorActor)
            {
                UStaticMeshComponent* MeshComponent = FloorActor->GetStaticMeshComponent();
                if (MeshComponent)
                {
                    MeshComponent->SetStaticMesh(Geometry.FloorMesh);
                    MeshComponent->SetWorldScale3D(Section.SectionScale);
                    
                    if (FloorMaterial)
                    {
                        MeshComponent->SetMaterial(0, FloorMaterial);
                    }
                }
            }
        }
    }

    // Spawn walls
    if (Geometry.WallMesh)
    {
        // Create walls around the level perimeter
        for (const FLevelSection& Section : Sections)
        {
            FVector SectionSize = Section.SectionScale * 1000.0f; // Assuming 1000 units per section
            
            // North wall
            FVector NorthWallPos = Section.SectionLocation + FVector(0, SectionSize.Y * 0.5f, 200);
            AStaticMeshActor* NorthWall = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), NorthWallPos, FRotator::ZeroRotator);
            if (NorthWall && NorthWall->GetStaticMeshComponent())
            {
                NorthWall->GetStaticMeshComponent()->SetStaticMesh(Geometry.WallMesh);
                NorthWall->GetStaticMeshComponent()->SetWorldScale3D(FVector(SectionSize.X / 100.0f, 1.0f, 4.0f));
                if (WallMaterial)
                {
                    NorthWall->GetStaticMeshComponent()->SetMaterial(0, WallMaterial);
                }
            }

            // South wall
            FVector SouthWallPos = Section.SectionLocation + FVector(0, -SectionSize.Y * 0.5f, 200);
            AStaticMeshActor* SouthWall = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), SouthWallPos, FRotator::ZeroRotator);
            if (SouthWall && SouthWall->GetStaticMeshComponent())
            {
                SouthWall->GetStaticMeshComponent()->SetStaticMesh(Geometry.WallMesh);
                SouthWall->GetStaticMeshComponent()->SetWorldScale3D(FVector(SectionSize.X / 100.0f, 1.0f, 4.0f));
                if (WallMaterial)
                {
                    SouthWall->GetStaticMeshComponent()->SetMaterial(0, WallMaterial);
                }
            }

            // East wall
            FVector EastWallPos = Section.SectionLocation + FVector(SectionSize.X * 0.5f, 0, 200);
            AStaticMeshActor* EastWall = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), EastWallPos, FRotator(0, 90, 0));
            if (EastWall && EastWall->GetStaticMeshComponent())
            {
                EastWall->GetStaticMeshComponent()->SetStaticMesh(Geometry.WallMesh);
                EastWall->GetStaticMeshComponent()->SetWorldScale3D(FVector(SectionSize.Y / 100.0f, 1.0f, 4.0f));
                if (WallMaterial)
                {
                    EastWall->GetStaticMeshComponent()->SetMaterial(0, WallMaterial);
                }
            }

            // West wall
            FVector WestWallPos = Section.SectionLocation + FVector(-SectionSize.X * 0.5f, 0, 200);
            AStaticMeshActor* WestWall = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), WestWallPos, FRotator(0, 90, 0));
            if (WestWall && WestWall->GetStaticMeshComponent())
            {
                WestWall->GetStaticMeshComponent()->SetStaticMesh(Geometry.WallMesh);
                WestWall->GetStaticMeshComponent()->SetWorldScale3D(FVector(SectionSize.Y / 100.0f, 1.0f, 4.0f));
                if (WallMaterial)
                {
                    WestWall->GetStaticMeshComponent()->SetMaterial(0, WallMaterial);
                }
            }
        }
    }

    // Spawn platforms and other geometry
    if (Geometry.PlatformMesh)
    {
        for (const FLevelSection& Section : Sections)
        {
            // Add some platforms for variety
            for (int32 i = 0; i < 3; i++)
            {
                FVector PlatformPos = Section.SectionLocation + FVector(FMath::RandRange(-400, 400), FMath::RandRange(-400, 400), 100 + i * 50);
                AStaticMeshActor* Platform = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), PlatformPos, FRotator::ZeroRotator);
                if (Platform && Platform->GetStaticMeshComponent())
                {
                    Platform->GetStaticMeshComponent()->SetStaticMesh(Geometry.PlatformMesh);
                    Platform->GetStaticMeshComponent()->SetWorldScale3D(FVector(2.0f, 2.0f, 0.5f));
                    if (PlatformMaterial)
                    {
                        Platform->GetStaticMeshComponent()->SetMaterial(0, PlatformMaterial);
                    }
                }
            }
        }
    }
}

void UVitrumLevelBlueprint::SpawnLighting(UWorld* World)
{
    // Spawn directional light
    ADirectionalLight* DirectionalLight = World->SpawnActor<ADirectionalLight>(ADirectionalLight::StaticClass(), FVector(0, 0, 1000), FRotator(-45, -45, 0));
    if (DirectionalLight)
    {
        DirectionalLight->GetLightComponent()->SetLightColor(Atmosphere.SkyLightColor);
        DirectionalLight->GetLightComponent()->SetIntensity(Atmosphere.SkyLightIntensity);
    }

    // Spawn sky light
    ASkyLight* SkyLight = World->SpawnActor<ASkyLight>(ASkyLight::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
    if (SkyLight)
    {
        SkyLight->GetLightComponent()->SetLightColor(Atmosphere.AmbientLightColor);
        SkyLight->GetLightComponent()->SetIntensity(Atmosphere.AmbientLightIntensity);
    }

    // Spawn point lights
    for (int32 i = 0; i < Atmosphere.PointLightPositions.Num(); i++)
    {
        if (i < Atmosphere.PointLightColors.Num() && i < Atmosphere.PointLightIntensities.Num())
        {
            APointLight* PointLight = World->SpawnActor<APointLight>(APointLight::StaticClass(), Atmosphere.PointLightPositions[i], FRotator::ZeroRotator);
            if (PointLight)
            {
                PointLight->GetLightComponent()->SetLightColor(Atmosphere.PointLightColors[i]);
                PointLight->GetLightComponent()->SetIntensity(Atmosphere.PointLightIntensities[i]);
            }
        }
    }
}

void UVitrumLevelBlueprint::SpawnGameplayElements(UWorld* World)
{
    // Spawn alignment nodes
    TArray<FVector> NodePositions = GetAlignmentNodePositions();
    for (int32 i = 0; i < NodePositions.Num(); i++)
    {
        AVitrumAlignmentNode* Node = World->SpawnActor<AVitrumAlignmentNode>(AVitrumAlignmentNode::StaticClass(), NodePositions[i], FRotator::ZeroRotator);
        if (Node)
        {
            // Set up node properties based on level type
            if (IsSolveLevel())
            {
                Node->SetNodeState(ENodeState::Inactive);
            }
            else
            {
                Node->SetNodeState(ENodeState::Active);
            }
        }
    }

    // Spawn hazards
    if (bHasHazards)
    {
        TArray<FVector> HazardPositions = GetHazardPositions();
        for (int32 i = 0; i < HazardPositions.Num(); i++)
        {
            AVitrumHazard* Hazard = World->SpawnActor<AVitrumHazard>(AVitrumHazard::StaticClass(), HazardPositions[i], FRotator::ZeroRotator);
            if (Hazard)
            {
                // Randomly assign hazard types
                EHazardType HazardTypes[] = { EHazardType::AcidPool, EHazardType::Stalker, EHazardType::NullZone };
                Hazard->SetHazardType(HazardTypes[FMath::RandRange(0, 2)]);
                Hazard->ActivateHazard();
            }
        }
    }

    // Spawn artisan shards
    TArray<FVector> ShardPositions = GetArtisanShardPositions();
    for (int32 i = 0; i < ShardPositions.Num(); i++)
    {
        AVitrumArtisanShard* Shard = World->SpawnActor<AVitrumArtisanShard>(AVitrumArtisanShard::StaticClass(), ShardPositions[i], FRotator::ZeroRotator);
        if (Shard)
        {
            Shard->ShardIndex = i;
            // Randomly assign shard types
            EShardType ShardTypes[] = { EShardType::Standard, EShardType::Rare, EShardType::Legendary };
            Shard->ShardType = ShardTypes[FMath::RandRange(0, 2)];
        }
    }

    // Spawn world manager
    AVitrumWorldManager* WorldManager = World->SpawnActor<AVitrumWorldManager>(AVitrumWorldManager::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
    if (WorldManager)
    {
        // Configure world manager based on level type
        if (bRequiresPerspectiveShift)
        {
            WorldManager->SetRotationSpeed(90.0f);
            WorldManager->SetRotationDuration(1.0f);
        }
    }
}

void UVitrumLevelBlueprint::SpawnAudio(UWorld* World)
{
    // Spawn background music
    if (BackgroundMusic)
    {
        UAudioComponent* MusicComponent = UGameplayStatics::SpawnSound2D(World, BackgroundMusic);
        if (MusicComponent)
        {
            MusicComponent->SetVolumeMultiplier(MusicVolume);
        }
    }

    // Spawn ambient sound
    if (Atmosphere.AmbientSound)
    {
        UAudioComponent* AmbientComponent = UGameplayStatics::SpawnSound2D(World, Atmosphere.AmbientSound);
        if (AmbientComponent)
        {
            AmbientComponent->SetVolumeMultiplier(Atmosphere.AmbientSoundVolume);
        }
    }
}

void UVitrumLevelBlueprint::SpawnParticles(UWorld* World)
{
    // Spawn ambient particles
    if (Atmosphere.AmbientParticles)
    {
        for (const FLevelSection& Section : Sections)
        {
            UParticleSystemComponent* ParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(World, Atmosphere.AmbientParticles, Section.SectionLocation, FRotator::ZeroRotator, FVector(1.0f, 1.0f, 1.0f), true);
            if (ParticleComponent)
            {
                ParticleComponent->SetAutoActivate(true);
            }
        }
    }
}
