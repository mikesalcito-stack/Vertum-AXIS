#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Engine/World.h"
#include "VitrumLevelManager.generated.h"

USTRUCT(BlueprintType)
struct FLevelData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    FName LevelID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    FString LevelName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    FVector2D MapPosition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    TArray<FName> ConnectedLevels;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    bool bIsUnlocked;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    bool bIsCompleted;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    float BestTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    int32 ArtisanShardsCollected;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    int32 TotalArtisanShards;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    TArray<FName> RequiredAbilities;

    FLevelData()
    {
        LevelID = NAME_None;
        LevelName = TEXT("");
        MapPosition = FVector2D::ZeroVector;
        bIsUnlocked = false;
        bIsCompleted = false;
        BestTime = 0.0f;
        ArtisanShardsCollected = 0;
        TotalArtisanShards = 0;
    }
};

USTRUCT(BlueprintType)
struct FWorldData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World")
    int32 WorldIndex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World")
    FString WorldName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World")
    FString WorldDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World")
    TArray<FLevelData> Levels;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World")
    bool bIsUnlocked;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World")
    bool bIsCompleted;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World")
    int32 TotalArtisanShards;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World")
    int32 CollectedArtisanShards;

    FWorldData()
    {
        WorldIndex = 0;
        WorldName = TEXT("");
        WorldDescription = TEXT("");
        bIsUnlocked = false;
        bIsCompleted = false;
        TotalArtisanShards = 0;
        CollectedArtisanShards = 0;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelCompleted, FName, LevelID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWorldCompleted, int32, WorldIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnArtisanShardCollected, int32, ShardIndex);

UCLASS(BlueprintType, Blueprintable)
class VITRUMAXIS_API AVitrumLevelManager : public AActor
{
    GENERATED_BODY()

public:
    AVitrumLevelManager();

protected:
    virtual void BeginPlay() override;

public:
    // Level Data
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    UDataTable* LevelDataTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    TArray<FWorldData> Worlds;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    int32 CurrentWorldIndex;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    FName CurrentLevelID;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    FLevelData CurrentLevelData;

    // Level Management
    UFUNCTION(BlueprintCallable, Category = "Level")
    void LoadLevel(FName LevelID);

    UFUNCTION(BlueprintCallable, Category = "Level")
    void CompleteCurrentLevel(float CompletionTime);

    UFUNCTION(BlueprintCallable, Category = "Level")
    void UnlockLevel(FName LevelID);

    UFUNCTION(BlueprintCallable, Category = "Level")
    bool IsLevelUnlocked(FName LevelID);

    UFUNCTION(BlueprintCallable, Category = "Level")
    bool IsLevelCompleted(FName LevelID);

    UFUNCTION(BlueprintCallable, Category = "Level")
    void CollectArtisanShard(int32 ShardIndex);

    UFUNCTION(BlueprintCallable, Category = "Level")
    TArray<FName> GetConnectedLevels(FName LevelID);

    UFUNCTION(BlueprintCallable, Category = "Level")
    FLevelData GetLevelData(FName LevelID);

    UFUNCTION(BlueprintCallable, Category = "Level")
    FWorldData GetCurrentWorldData();

    // World Management
    UFUNCTION(BlueprintCallable, Category = "World")
    void LoadWorld(int32 WorldIndex);

    UFUNCTION(BlueprintCallable, Category = "World")
    void UnlockWorld(int32 WorldIndex);

    UFUNCTION(BlueprintCallable, Category = "World")
    bool IsWorldUnlocked(int32 WorldIndex);

    UFUNCTION(BlueprintCallable, Category = "World")
    bool IsWorldCompleted(int32 WorldIndex);

    // Save/Load
    UFUNCTION(BlueprintCallable, Category = "Save")
    void SaveProgress();

    UFUNCTION(BlueprintCallable, Category = "Save")
    void LoadProgress();

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnLevelCompleted OnLevelCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnWorldCompleted OnWorldCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnArtisanShardCollected OnArtisanShardCollected;

protected:
    // Internal Functions
    void InitializeWorlds();
    void LoadLevelDataFromTable();
    FLevelData* FindLevelData(FName LevelID);
    FWorldData* FindWorldData(int32 WorldIndex);
    void UpdateWorldProgress();

private:
    // Internal State
    TMap<FName, FLevelData> LevelDataMap;
    bool bIsInitialized;
};
