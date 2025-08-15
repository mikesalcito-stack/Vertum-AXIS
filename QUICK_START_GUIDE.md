# VITRUM AXIS - Quick Start Guide
# Get Your AAA-Quality Action-Puzzler Running in Minutes

## 🚀 Opening the Project

### Step 1: Launch Unreal Engine 5.3
1. Open Epic Games Launcher
2. Go to Unreal Engine tab
3. Launch Unreal Engine 5.3
4. Wait for the engine to fully load

### Step 2: Open VITRUM AXIS Project
1. In Unreal Engine, click "Open Project"
2. Navigate to `C:\VITRUM_AXIS`
3. Select `VITRUM_AXIS.uproject`
4. Click "Open"
5. Wait for the project to compile and load

### Step 3: Verify Project Setup
- ✅ C++ code should compile automatically
- ✅ All C++ classes should be available in Blueprint
- ✅ Project should open to the default level

## 🎮 First Steps in the Editor

### Step 1: Set Up Default Classes
1. Go to **Edit → Project Settings**
2. Under **Maps & Modes**:
   - Set **Default GameMode** to `VitrumGameMode`
   - Set **Default Pawn Class** to `VitrumMarble`
3. Click **Apply**

### Step 2: Create Your First Level
1. In **Content Browser**, right-click → **New Level**
2. Choose **Empty Level**
3. Save as `W1_L1_Awakening`
4. Place in `Content/Maps/World1/`

### Step 3: Add Basic Geometry
1. Drag a **Cube** from **Place Actors** panel
2. Scale it to create a floor (e.g., 800x600x20)
3. Position at (0, 0, 0)
4. Apply material `M_Floor_Calibration` (create this material)

## 🎨 Creating Your First Assets

### Step 1: Create Survey Sphere Material
1. Right-click in **Content Browser** → **Material**
2. Name it `M_SurveySphere_Base`
3. Set **Base Color** to deep blue (0.1, 0.2, 0.8)
4. Set **Metallic** to 0.8
5. Set **Roughness** to 0.2
6. Add **Emissive** parameter for glow

### Step 2: Create Player Blueprint
1. Right-click in **Content Browser** → **Blueprint Class**
2. Choose **VitrumMarble** as parent
3. Name it `BP_VitrumMarble`
4. Add **Static Mesh Component** (sphere)
5. Apply `M_SurveySphere_Base` material
6. Add **Spring Arm** and **Camera** components

### Step 3: Create Game Mode Blueprint
1. Right-click in **Content Browser** → **Blueprint Class**
2. Choose **VitrumGameMode** as parent
3. Name it `BP_VitrumGameMode`
4. Set up basic game state management

## 🎯 Setting Up Gameplay

### Step 1: Add Player to Level
1. Drag `BP_VitrumMarble` into your level
2. Position at spawn point (0, 0, 100)
3. Set as **Player Start**

### Step 2: Add World Manager
1. Create `BP_VitrumWorldManager` Blueprint
2. Place in level
3. This handles perspective shifting

### Step 3: Test Basic Movement
1. Press **Play** in editor
2. Use **WASD** to move
3. Use **Space** to jump
4. Use **Q/E** to rotate world (after setup)

## 🎨 Creating Visual Assets

### Step 1: Environment Materials
Create these materials in `Content/Materials/`:
- `M_Floor_Calibration` - Dark gray with blue grid
- `M_Wall_Calibration` - Medium gray with panels
- `M_Platform_Calibration` - Light gray with glow

### Step 2: Gameplay Element Materials
- `M_Node_Base` - Neutral gray for alignment nodes
- `M_Node_Active` - Bright blue for active nodes
- `M_Hazard_AcidPool` - Toxic green for acid
- `M_Shard_Standard` - Blue crystal for shards

### Step 3: UI Materials
- `M_UI_Background` - Dark blue for UI
- `M_UI_Button` - Medium blue for buttons
- `M_UI_ProgressBar` - Progress bar material

## 🎵 Setting Up Audio

### Step 1: Create Audio Manager
1. Create `BP_AudioManager` Blueprint
2. Add **Audio Components** for different states
3. Set up music tracks for Flow/Solve states

### Step 2: Add Sound Effects
Create these audio assets:
- `SFX_Dash` - Dash ability sound
- `SFX_NodeActivation` - Node activation
- `SFX_ShardCollection` - Shard collection
- `SFX_WorldRotation` - Perspective shift

### Step 3: Add Music Tracks
- `Music_Flow` - High-energy for Flow state
- `Music_Solve` - Calm for Solve state
- `Music_Victory` - Victory music

## 🧩 Creating Puzzle Elements

### Step 1: Alignment Node Blueprint
1. Create `BP_VitrumAlignmentNode`
2. Add **Collision Sphere** and **Static Mesh**
3. Set up state management (Inactive → Active → Linked → Completed)
4. Add particle effects for activation

### Step 2: Hazard Blueprint
1. Create `BP_VitrumHazard`
2. Add different hazard types (AcidPool, Stalker, NullZone)
3. Set up damage system
4. Add visual effects

### Step 3: Artisan Shard Blueprint
1. Create `BP_VitrumArtisanShard`
2. Add floating animation
3. Set up collection system
4. Add particle effects

## 🗺️ Setting Up World Map

### Step 1: Create Map Widget
1. Create `BP_WorldMapHUD` User Widget
2. Add level node buttons
3. Set up connection lines
4. Add zoom and navigation

### Step 2: Level Manager Setup
1. Create `BP_VitrumLevelManager`
2. Set up level data from JSON
3. Handle level loading/unlocking
4. Track progress

## 🎮 Testing Your Game

### Step 1: Basic Functionality
1. Test player movement
2. Test world rotation
3. Test basic interactions
4. Verify audio works

### Step 2: Gameplay Testing
1. Test alignment nodes
2. Test hazard interactions
3. Test shard collection
4. Test level completion

### Step 3: Performance Testing
1. Check frame rate (target 60fps)
2. Monitor memory usage
3. Test on different settings
4. Optimize as needed

## 🚀 Next Steps

### Immediate Tasks
1. Create all 10 levels for World 1
2. Set up complete material library
3. Implement all Blueprint systems
4. Add particle effects

### Advanced Features
1. Implement save/load system
2. Add cooperative multiplayer
3. Create level editor tools
4. Add modding support

## 🛠️ Troubleshooting

### Common Issues
- **Project won't open**: Check engine version (5.3)
- **C++ won't compile**: Regenerate project files
- **Blueprints missing**: Check parent class references
- **Performance issues**: Optimize materials and particles

### Getting Help
- Check the detailed documentation in `README.md`
- Review the Blueprint setup guide
- Check material specifications
- Verify all file paths are correct

## 🎯 Success Checklist

Before moving to advanced features, ensure:
- ✅ Project opens and compiles
- ✅ Basic movement works
- ✅ World rotation functions
- ✅ Materials look correct
- ✅ Audio plays properly
- ✅ First level is playable
- ✅ UI elements display
- ✅ Performance is acceptable

---

**You're now ready to build the AAA-quality VITRUM AXIS experience!**

The foundation is complete - all C++ systems are implemented, the project structure is set up, and you have comprehensive guides for creating all the visual and audio assets. Start with the basic materials and Blueprints, then expand to create the full 10-level World 1 experience.

*Remember: The key to AAA quality is attention to detail, smooth performance, and polished user experience. Take your time to get each element right before moving to the next.*
