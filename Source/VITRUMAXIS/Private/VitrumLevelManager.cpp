#include "VitrumLevelManager.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "VitrumGameMode.h"

AVitrumLevelManager::AVitrumLevelManager()
{
    PrimaryActorTick.bCanEverTick = false;

    // Set default values
    CurrentWorldIndex = 1;
    CurrentLevelID = NAME_None;
    bIsInitialized = false;
}

void AVitrumLevelManager::BeginPlay()
{
    Super::BeginPlay();

    InitializeWorlds();
    LoadLevelDataFromTable();
    LoadProgress();
}

void AVitrumLevelManager::InitializeWorlds()
{
    // Initialize World 1: The Calibration Core
    FWorldData World1;
    World1.WorldIndex = 1;
    World1.WorldName = TEXT("The Calibration Core");
    World1.WorldDescription = TEXT("A world of clean lines, stark geometry, and raw energy. The foundational layer of the Manifold.");
    World1.bIsUnlocked = true;
    World1.bIsCompleted = false;
    World1.TotalArtisanShards = 3;
    World1.CollectedArtisanShards = 0;

    // Create levels for World 1
    FLevelData Level1_1;
    Level1_1.LevelID = FName(TEXT("W1_L1"));
    Level1_1.LevelName = TEXT("Awakening");
    Level1_1.MapPosition = FVector2D(0.0f, 0.0f);
    Level1_1.ConnectedLevels = { FName(TEXT("W1_L2")) };
    Level1_1.bIsUnlocked = true;
    Level1_1.bIsCompleted = false;
    Level1_1.TotalArtisanShards = 0;
    Level1_1.RequiredAbilities = {};

    FLevelData Level1_2;
    Level1_2.LevelID = FName(TEXT("W1_L2"));
    Level1_2.LevelName = TEXT("First Steps");
    Level1_2.MapPosition = FVector2D(200.0f, 0.0f);
    Level1_2.ConnectedLevels = { FName(TEXT("W1_L1")), FName(TEXT("W1_L3")) };
    Level1_2.bIsUnlocked = false;
    Level1_2.bIsCompleted = false;
    Level1_2.TotalArtisanShards = 0;
    Level1_2.RequiredAbilities = {};

    FLevelData Level1_3;
    Level1_3.LevelID = FName(TEXT("W1_L3"));
    Level1_3.LevelName = TEXT("Momentum");
    Level1_3.MapPosition = FVector2D(400.0f, 0.0f);
    Level1_3.ConnectedLevels = { FName(TEXT("W1_L2")), FName(TEXT("W1_L4")), FName(TEXT("W1_L5")) };
    Level1_3.bIsUnlocked = false;
    Level1_3.bIsCompleted = false;
    Level1_3.TotalArtisanShards = 1;
    Level1_3.RequiredAbilities = {};

    FLevelData Level1_4;
    Level1_4.LevelID = FName(TEXT("W1_L4"));
    Level1_4.LevelName = TEXT("The Shift");
    Level1_4.MapPosition = FVector2D(400.0f, 200.0f);
    Level1_4.ConnectedLevels = { FName(TEXT("W1_L3")), FName(TEXT("W1_L6")) };
    Level1_4.bIsUnlocked = false;
    Level1_4.bIsCompleted = false;
    Level1_4.TotalArtisanShards = 0;
    Level1_4.RequiredAbilities = {};

    FLevelData Level1_5;
    Level1_5.LevelID = FName(TEXT("W1_L5"));
    Level1_5.LevelName = TEXT("Hidden Path");
    Level1_5.MapPosition = FVector2D(600.0f, 0.0f);
    Level1_5.ConnectedLevels = { FName(TEXT("W1_L3")), FName(TEXT("W1_L7")) };
    Level1_5.bIsUnlocked = false;
    Level1_5.bIsCompleted = false;
    Level1_5.TotalArtisanShards = 1;
    Level1_5.RequiredAbilities = { FName(TEXT("Dash")) };

    FLevelData Level1_6;
    Level1_6.LevelID = FName(TEXT("W1_L6"));
    Level1_6.LevelName = TEXT("Perspective Mastery");
    Level1_6.MapPosition = FVector2D(400.0f, 400.0f);
    Level1_6.ConnectedLevels = { FName(TEXT("W1_L4")), FName(TEXT("W1_L8")) };
    Level1_6.bIsUnlocked = false;
    Level1_6.bIsCompleted = false;
    Level1_6.TotalArtisanShards = 0;
    Level1_6.RequiredAbilities = {};

    FLevelData Level1_7;
    Level1_7.LevelID = FName(TEXT("W1_L7"));
    Level1_7.LevelName = TEXT("The Forge");
    Level1_7.MapPosition = FVector2D(800.0f, 0.0f);
    Level1_7.ConnectedLevels = { FName(TEXT("W1_L5")), FName(TEXT("W1_L9")) };
    Level1_7.bIsUnlocked = false;
    Level1_7.bIsCompleted = false;
    Level1_7.TotalArtisanShards = 1;
    Level1_7.RequiredAbilities = { FName(TEXT("TractionField")) };

    FLevelData Level1_8;
    Level1_8.LevelID = FName(TEXT("W1_L8"));
    Level1_8.LevelName = TEXT("Core Access");
    Level1_8.MapPosition = FVector2D(400.0f, 600.0f);
    Level1_8.ConnectedLevels = { FName(TEXT("W1_L6")), FName(TEXT("W1_L10")) };
    Level1_8.bIsUnlocked = false;
    Level1_8.bIsCompleted = false;
    Level1_8.TotalArtisanShards = 0;
    Level1_8.RequiredAbilities = {};

    FLevelData Level1_9;
    Level1_9.LevelID = FName(TEXT("W1_L9"));
    Level1_9.LevelName = TEXT("The Convergence");
    Level1_9.MapPosition = FVector2D(1000.0f, 0.0f);
    Level1_9.ConnectedLevels = { FName(TEXT("W1_L7")), FName(TEXT("W1_L10")) };
    Level1_9.bIsUnlocked = false;
    Level1_9.bIsCompleted = false;
    Level1_9.TotalArtisanShards = 0;
    Level1_9.RequiredAbilities = { FName(TEXT("Dash")), FName(TEXT("TractionField")) };

    FLevelData Level1_10;
    Level1_10.LevelID = FName(TEXT("W1_L10"));
    Level1_10.LevelName = TEXT("The Calibration");
    Level1_10.MapPosition = FVector2D(600.0f, 600.0f);
    Level1_10.ConnectedLevels = { FName(TEXT("W1_L8")), FName(TEXT("W1_L9")) };
    Level1_10.bIsUnlocked = false;
    Level1_10.bIsCompleted = false;
    Level1_10.TotalArtisanShards = 0;
    Level1_10.RequiredAbilities = {};

    // Add levels to world
    World1.Levels = { Level1_1, Level1_2, Level1_3, Level1_4, Level1_5, Level1_6, Level1_7, Level1_8, Level1_9, Level1_10 };

    // Add world to worlds array
    Worlds.Add(World1);

    bIsInitialized = true;
}

void AVitrumLevelManager::LoadLevelDataFromTable()
{
    if (LevelDataTable)
    {
        TArray<FName> RowNames = LevelDataTable->GetRowNames();
        for (FName RowName : RowNames)
        {
            FLevelData* LevelData = LevelDataTable->FindRow<FLevelData>(RowName, TEXT(""));
            if (LevelData)
            {
                LevelDataMap.Add(RowName, *LevelData);
            }
        }
    }
}

void AVitrumLevelManager::LoadLevel(FName LevelID)
{
    FLevelData* LevelData = FindLevelData(LevelID);
    if (LevelData && LevelData->bIsUnlocked)
    {
        CurrentLevelID = LevelID;
        CurrentLevelData = *LevelData;

        // Load the level map
        FString LevelMapName = FString::Printf(TEXT("/Game/Maps/World%d/%s"), CurrentWorldIndex, *LevelID.ToString());
        UGameplayStatics::OpenLevel(this, FName(*LevelMapName));
    }
}

void AVitrumLevelManager::CompleteCurrentLevel(float CompletionTime)
{
    FLevelData* LevelData = FindLevelData(CurrentLevelID);
    if (LevelData)
    {
        LevelData->bIsCompleted = true;
        if (CompletionTime < LevelData->BestTime || LevelData->BestTime == 0.0f)
        {
            LevelData->BestTime = CompletionTime;
        }

        // Unlock connected levels
        for (FName ConnectedLevelID : LevelData->ConnectedLevels)
        {
            UnlockLevel(ConnectedLevelID);
        }

        // Update world progress
        UpdateWorldProgress();

        // Broadcast event
        OnLevelCompleted.Broadcast(CurrentLevelID);

        // Check if world is completed
        FWorldData* WorldData = FindWorldData(CurrentWorldIndex);
        if (WorldData && WorldData->bIsCompleted)
        {
            OnWorldCompleted.Broadcast(CurrentWorldIndex);
        }
    }
}

void AVitrumLevelManager::UnlockLevel(FName LevelID)
{
    FLevelData* LevelData = FindLevelData(LevelID);
    if (LevelData)
    {
        LevelData->bIsUnlocked = true;
    }
}

bool AVitrumLevelManager::IsLevelUnlocked(FName LevelID)
{
    FLevelData* LevelData = FindLevelData(LevelID);
    return LevelData ? LevelData->bIsUnlocked : false;
}

bool AVitrumLevelManager::IsLevelCompleted(FName LevelID)
{
    FLevelData* LevelData = FindLevelData(LevelID);
    return LevelData ? LevelData->bIsCompleted : false;
}

void AVitrumLevelManager::CollectArtisanShard(int32 ShardIndex)
{
    FLevelData* LevelData = FindLevelData(CurrentLevelID);
    if (LevelData)
    {
        LevelData->ArtisanShardsCollected++;
        
        FWorldData* WorldData = FindWorldData(CurrentWorldIndex);
        if (WorldData)
        {
            WorldData->CollectedArtisanShards++;
        }

        OnArtisanShardCollected.Broadcast(ShardIndex);
    }
}

TArray<FName> AVitrumLevelManager::GetConnectedLevels(FName LevelID)
{
    FLevelData* LevelData = FindLevelData(LevelID);
    return LevelData ? LevelData->ConnectedLevels : TArray<FName>();
}

FLevelData AVitrumLevelManager::GetLevelData(FName LevelID)
{
    FLevelData* LevelData = FindLevelData(LevelID);
    return LevelData ? *LevelData : FLevelData();
}

FWorldData AVitrumLevelManager::GetCurrentWorldData()
{
    FWorldData* WorldData = FindWorldData(CurrentWorldIndex);
    return WorldData ? *WorldData : FWorldData();
}

void AVitrumLevelManager::LoadWorld(int32 WorldIndex)
{
    FWorldData* WorldData = FindWorldData(WorldIndex);
    if (WorldData && WorldData->bIsUnlocked)
    {
        CurrentWorldIndex = WorldIndex;
        
        // Load the first unlocked level
        for (FLevelData& Level : WorldData->Levels)
        {
            if (Level.bIsUnlocked)
            {
                LoadLevel(Level.LevelID);
                break;
            }
        }
    }
}

void AVitrumLevelManager::UnlockWorld(int32 WorldIndex)
{
    FWorldData* WorldData = FindWorldData(WorldIndex);
    if (WorldData)
    {
        WorldData->bIsUnlocked = true;
    }
}

bool AVitrumLevelManager::IsWorldUnlocked(int32 WorldIndex)
{
    FWorldData* WorldData = FindWorldData(WorldIndex);
    return WorldData ? WorldData->bIsUnlocked : false;
}

bool AVitrumLevelManager::IsWorldCompleted(int32 WorldIndex)
{
    FWorldData* WorldData = FindWorldData(WorldIndex);
    return WorldData ? WorldData->bIsCompleted : false;
}

void AVitrumLevelManager::SaveProgress()
{
    // TODO: Implement save system
    // This would save the current state to a save file
}

void AVitrumLevelManager::LoadProgress()
{
    // TODO: Implement load system
    // This would load the saved state from a save file
}

FLevelData* AVitrumLevelManager::FindLevelData(FName LevelID)
{
    // First check the data table
    if (LevelDataTable)
    {
        return LevelDataTable->FindRow<FLevelData>(LevelID, TEXT(""));
    }

    // Then check the worlds array
    for (FWorldData& World : Worlds)
    {
        for (FLevelData& Level : World.Levels)
        {
            if (Level.LevelID == LevelID)
            {
                return &Level;
            }
        }
    }

    return nullptr;
}

FWorldData* AVitrumLevelManager::FindWorldData(int32 WorldIndex)
{
    for (FWorldData& World : Worlds)
    {
        if (World.WorldIndex == WorldIndex)
        {
            return &World;
        }
    }
    return nullptr;
}

void AVitrumLevelManager::UpdateWorldProgress()
{
    FWorldData* WorldData = FindWorldData(CurrentWorldIndex);
    if (WorldData)
    {
        int32 CompletedLevels = 0;
        int32 TotalLevels = WorldData->Levels.Num();

        for (FLevelData& Level : WorldData->Levels)
        {
            if (Level.bIsCompleted)
            {
                CompletedLevels++;
            }
        }

        // World is completed if all levels are completed
        WorldData->bIsCompleted = (CompletedLevels >= TotalLevels);
    }
}
