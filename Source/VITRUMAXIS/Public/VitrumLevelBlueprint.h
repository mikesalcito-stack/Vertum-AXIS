#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/World.h"
#include "VitrumLevelBlueprint.generated.h"

USTRUCT(BlueprintType)
struct FLevelSection
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Section")
    FString SectionName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Section")
    FVector SectionLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Section")
    FRotator SectionRotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Section")
    FVector SectionScale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Section")
    TArray<FVector> AlignmentNodePositions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Section")
    TArray<FVector> HazardPositions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Section")
    TArray<FVector> ArtisanShardPositions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Section")
    TArray<FVector> CheckpointPositions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Section")
    FVector PlayerSpawnPosition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Section")
    FVector LevelExitPosition;

    FLevelSection()
    {
        SectionName = TEXT("");
        SectionLocation = FVector::ZeroVector;
        SectionRotation = FRotator::ZeroRotator;
        SectionScale = FVector(1.0f, 1.0f, 1.0f);
    }
};

USTRUCT(BlueprintType)
struct FLevelGeometry
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Geometry")
    UStaticMesh* FloorMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Geometry")
    UStaticMesh* WallMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Geometry")
    UStaticMesh* CeilingMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Geometry")
    UStaticMesh* PlatformMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Geometry")
    UStaticMesh* RampMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Geometry")
    UStaticMesh* BridgeMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Geometry")
    UStaticMesh* PillarMesh;

    FLevelGeometry()
    {
        FloorMesh = nullptr;
        WallMesh = nullptr;
        CeilingMesh = nullptr;
        PlatformMesh = nullptr;
        RampMesh = nullptr;
        BridgeMesh = nullptr;
        PillarMesh = nullptr;
    }
};

USTRUCT(BlueprintType)
struct FLevelAtmosphere
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atmosphere")
    FLinearColor AmbientLightColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atmosphere")
    float AmbientLightIntensity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atmosphere")
    FLinearColor SkyLightColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atmosphere")
    float SkyLightIntensity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atmosphere")
    TArray<FVector> PointLightPositions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atmosphere")
    TArray<FLinearColor> PointLightColors;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atmosphere")
    TArray<float> PointLightIntensities;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atmosphere")
    UParticleSystem* AmbientParticles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atmosphere")
    USoundCue* AmbientSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atmosphere")
    float AmbientSoundVolume;

    FLevelAtmosphere()
    {
        AmbientLightColor = FLinearColor(0.1f, 0.1f, 0.2f, 1.0f);
        AmbientLightIntensity = 0.3f;
        SkyLightColor = FLinearColor(0.5f, 0.5f, 0.8f, 1.0f);
        SkyLightIntensity = 1.0f;
        AmbientSoundVolume = 0.5f;
    }
};

UENUM(BlueprintType)
enum class ELevelType : uint8
{
    Tutorial      UMETA(DisplayName = "Tutorial"),
    Flow          UMETA(DisplayName = "Flow"),
    Solve         UMETA(DisplayName = "Solve"),
    Boss          UMETA(DisplayName = "Boss"),
    Hub           UMETA(DisplayName = "Hub")
};

UCLASS(BlueprintType, Blueprintable)
class VITRUMAXIS_API UVitrumLevelBlueprint : public UDataAsset
{
    GENERATED_BODY()

public:
    UVitrumLevelBlueprint();

    // Level Information
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    FName LevelID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    FString LevelName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    ELevelType LevelType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    FString LevelDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    float TargetCompletionTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    int32 DifficultyRating;

    // Level Structure
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Structure")
    TArray<FLevelSection> Sections;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Structure")
    FLevelGeometry Geometry;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Structure")
    FLevelAtmosphere Atmosphere;

    // Gameplay Elements
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    int32 RequiredAlignmentNodes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    int32 TotalArtisanShards;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    TArray<FName> RequiredAbilities;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    bool bHasHazards;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    bool bHasPuzzles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    bool bRequiresPerspectiveShift;

    // Visual Assets
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    UMaterialInterface* FloorMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    UMaterialInterface* WallMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    UMaterialInterface* CeilingMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    UMaterialInterface* PlatformMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    UTexture2D* LevelThumbnail;

    // Audio
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundCue* BackgroundMusic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    float MusicVolume;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundCue* VictorySound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    float VictorySoundVolume;

    // Public Functions
    UFUNCTION(BlueprintCallable, Category = "Level")
    void GenerateLevel(UWorld* World);

    UFUNCTION(BlueprintCallable, Category = "Level")
    FVector GetPlayerSpawnPosition() const;

    UFUNCTION(BlueprintCallable, Category = "Level")
    FVector GetLevelExitPosition() const;

    UFUNCTION(BlueprintCallable, Category = "Level")
    TArray<FVector> GetAlignmentNodePositions() const;

    UFUNCTION(BlueprintCallable, Category = "Level")
    TArray<FVector> GetHazardPositions() const;

    UFUNCTION(BlueprintCallable, Category = "Level")
    TArray<FVector> GetArtisanShardPositions() const;

    UFUNCTION(BlueprintCallable, Category = "Level")
    TArray<FVector> GetCheckpointPositions() const;

    UFUNCTION(BlueprintCallable, Category = "Level")
    bool IsFlowLevel() const;

    UFUNCTION(BlueprintCallable, Category = "Level")
    bool IsSolveLevel() const;

    UFUNCTION(BlueprintCallable, Category = "Level")
    bool IsTutorialLevel() const;

protected:
    // Internal Functions
    void SpawnGeometry(UWorld* World);
    void SpawnLighting(UWorld* World);
    void SpawnGameplayElements(UWorld* World);
    void SpawnAudio(UWorld* World);
    void SpawnParticles(UWorld* World);
};
