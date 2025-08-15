# VITRUM AXIS - AAA Action-Puzzler Prototype

## 🎮 Game Overview

**VITRUM AXIS** is a premium AAA-quality action-puzzler that combines high-speed momentum-driven platforming with methodical logic-based puzzle-solving. Set in a mysterious digital realm called "The Manifold," players control a Survey Sphere through interconnected worlds, mastering the unique "Perspective Shift" mechanic to solve increasingly complex challenges.

### Core Concept
- **2.5D Isometric View** with fully 3D rotatable world
- **Perspective Shift** - 90-degree world rotations reveal hidden paths and solutions
- **Dual Gameplay States** - High-speed "Flow" and methodical "Solve" modes
- **Metroidvania Progression** - Unlock new abilities and explore interconnected worlds
- **Physics-Based Movement** - Momentum-driven platforming with realistic physics

## 🏗️ Technical Architecture

### Engine & Version
- **Unreal Engine 5.3** with latest features
- **C++ Core Systems** with Blueprint integration
- **Enhanced Input System** for responsive controls
- **Modular Gameplay Framework** for extensible systems

### Key Systems Implemented

#### 1. **Core Game Systems**
- `VitrumGameMode` - Game state management (Flow/Solve/Paused/GameOver)
- `VitrumMarble` - Player character with physics-based movement
- `VitrumWorldManager` - Perspective shift mechanics
- `VitrumHUD` - Dynamic UI system
- `VitrumInputConfig` - Enhanced input mapping

#### 2. **Puzzle & Gameplay Elements**
- `VitrumAlignmentNode` - Oxyd-style stone matching system
- `VitrumHazard` - Environmental threats (Acid pools, Stalkers, Null Zones)
- `VitrumArtisanShard` - Collectible progression system
- `VitrumLevelManager` - Metroidvania map system

#### 3. **Level Design & World Building**
- `VitrumLevelBlueprint` - Procedural level generation system
- `VitrumWorldMapWidget` - Interactive world map UI
- Modular level sections with interconnected design
- Dynamic lighting and atmosphere systems

## 🎯 Gameplay Mechanics

### Survey Sphere (Player Character)
- **Physics-Based Movement** with momentum control
- **Integrity System** - Visible damage states with cracks
- **Kinetic Drive Abilities**:
  - **Dash** - High-speed burst movement
  - **Traction Field** - Localized friction control
  - **Enhanced Abilities** - Unlocked through Artisan Shards

### Perspective Shift
- **90-Degree Rotations** of the entire world
- **Smooth Interpolation** with configurable speed
- **Reveals Hidden Paths** and puzzle solutions
- **Affects All Gameplay Elements** - physics, hazards, collectibles

### Game States
- **Flow State** - High-speed platforming with hazards
- **Solve State** - Methodical puzzle-solving
- **Paused State** - Menu and UI interaction
- **GameOver State** - Death and respawn handling

### Puzzle Elements
- **Alignment Nodes** - Match symbols to unlock paths
- **3D Sokoban** - Push blocks in 3D space
- **Volumetric Conduits** - Energy flow puzzles
- **Artisan Shards** - Collectible progression rewards

## 🌍 World Design - The Calibration Core

### World Overview
**World 1: The Calibration Core** serves as the tutorial and foundation world, introducing players to all core mechanics through a carefully crafted progression system.

### Level Structure (10 Levels)
1. **Awakening** (W1_L1) - Tutorial introduction
2. **First Steps** (W1_L2) - Basic movement and physics
3. **Momentum** (W1_L3) - Physics mastery with Artisan Shard
4. **The Shift** (W1_L4) - Introduction to perspective shifting
5. **Hidden Path** (W1_L5) - Secret areas requiring Dash ability
6. **Perspective Mastery** (W1_L6) - Advanced perspective puzzles
7. **The Forge** (W1_L7) - Hazard introduction with Traction Field
8. **Core Access** (W1_L8) - Final puzzle preparation
9. **The Convergence** (W1_L9) - Combined mechanics challenge
10. **The Calibration** (W1_L10) - World completion

### Metroidvania Map Features
- **Interconnected Levels** with multiple paths
- **Ability-Gated Areas** requiring specific unlocks
- **Artisan Shard Collection** (3 total in World 1)
- **Progressive Difficulty** with "Teach, Test, Twist" design
- **Secret Areas** and hidden collectibles

## 🎨 Visual & Audio Design

### Visual Style
- **Clean, Geometric Aesthetics** with stark lines
- **Dynamic Lighting** that responds to game state
- **Particle Effects** for all interactions
- **Material-Based Damage** visualization
- **Smooth Camera Transitions** and effects

### Audio System
- **Dynamic Music** that changes with game state
- **3D Spatial Audio** for environmental effects
- **Distance-Based Volume** for collectibles
- **State-Responsive Soundscapes**
- **Particle Audio Integration**

### AAA-Quality Features
- **Realistic Physics** with Chaos physics engine
- **Advanced Materials** with dynamic properties
- **Post-Processing Effects** for atmosphere
- **Optimized Performance** for smooth 60fps
- **Scalable Graphics** settings

## 🎮 Controls & Input

### Movement Controls
- **WASD** - Movement with momentum physics
- **Space** - Jump with variable height
- **Shift** - Kinetic Dash (energy cost)
- **Ctrl** - Traction Field activation

### Perspective Controls
- **Q/E** - Rotate world left/right
- **R** - Reset to North orientation
- **Mouse** - Free camera view (optional)

### Game Controls
- **P** - Pause game
- **Tab** - Toggle world map
- **Enter** - Confirm/Interact
- **Escape** - Menu/Cancel

## 🏆 Progression System

### Artisan Shards
- **Standard Shards** - Basic progression
- **Rare Shards** - Enhanced abilities
- **Legendary Shards** - Advanced powers
- **Master Shards** - Ultimate abilities

### Unlocked Abilities
- **Enhanced Dash** - Longer range, faster cooldown
- **Enhanced Traction Field** - Larger radius, stronger effect
- **Time Slow** - Master ability for precision

### World Progression
- **Level Completion** unlocks connected areas
- **Ability Requirements** gate advanced content
- **Artisan Shard Collection** tracks completion
- **Best Time Tracking** for speedrunning

## 🛠️ Development Setup

### Prerequisites
- Unreal Engine 5.6
- Visual Studio 2022 (for C++ development)
- Windows 10/11

### Building the Project
1. **Open Project**:
   ```
   Double-click VITRUM_AXIS.uproject
   ```

2. **Generate Project Files** (if needed):
   ```
   Right-click VITRUM_AXIS.uproject → Generate Visual Studio Project Files
   ```

3. **Build in Visual Studio**:
   ```
   Open VITRUM_AXIS.sln
   Build → Build Solution
   ```

4. **Run in Unreal Editor**:
   ```
   Open VITRUM_AXIS.uproject in Unreal Editor
   Set Default Game Mode to VitrumGameMode
   Set Default Pawn Class to VitrumMarble
   ```

### Project Structure
```
VITRUM_AXIS/
├── Source/VITRUMAXIS/
│   ├── Public/          # Header files
│   ├── Private/         # Implementation files
│   └── VITRUMAXIS.Build.cs
├── Content/             # Blueprints, assets, levels
├── Config/              # Engine configuration
└── VITRUM_AXIS.uproject
```

## 🎯 Level Design Philosophy

### "Teach, Test, Twist" Progression
1. **Teach** - Introduce new mechanics in safe environments
2. **Test** - Challenge players with basic applications
3. **Twist** - Combine mechanics in unexpected ways

### Design Principles
- **Accessibility First** - Clear visual language
- **Progressive Complexity** - Gradual difficulty increase
- **Multiple Solutions** - Encourage creative problem-solving
- **Rewarding Exploration** - Hidden areas and collectibles
- **Smooth Flow** - Seamless transitions between sections

### Technical Implementation
- **Modular Level Sections** for easy iteration
- **Data-Driven Design** with level blueprints
- **Procedural Generation** capabilities
- **Performance Optimization** for large levels
- **Scalable Architecture** for future worlds

## 🚀 Future Development

### Planned Features
- **7 Additional Worlds** (20+ hours total)
- **Cooperative Multiplayer** mode
- **Level Editor** tools
- **Modding Support** framework
- **DLC Content** expansion

### Technical Roadmap
- **Advanced AI** for dynamic hazards
- **Procedural Level Generation**
- **Advanced Physics** interactions
- **VR Support** for immersive experience
- **Cross-Platform** deployment

## 📊 Performance Targets

### Minimum Requirements
- **GPU**: GTX 1060 / RX 580
- **CPU**: Intel i5-8400 / AMD Ryzen 5 2600
- **RAM**: 16GB
- **Storage**: 50GB SSD
- **Target**: 60fps @ 1080p

### Recommended Requirements
- **GPU**: RTX 3070 / RX 6700 XT
- **CPU**: Intel i7-10700K / AMD Ryzen 7 3700X
- **RAM**: 32GB
- **Storage**: 100GB NVMe SSD
- **Target**: 60fps @ 1440p

## 🎨 Asset Creation Guidelines

### Visual Assets
- **Clean, Geometric** design language
- **PBR Materials** with realistic properties
- **Optimized Meshes** for performance
- **Consistent Scale** across all assets
- **Modular Design** for reusability

### Audio Assets
- **Ambient Soundscapes** for atmosphere
- **Interactive Audio** for gameplay feedback
- **Dynamic Music** system integration
- **Spatial Audio** for 3D positioning
- **Compression Optimization** for file size

## 🔧 Debugging & Testing

### Debug Features
- **Visual Debugging** for physics and gameplay
- **Performance Profiling** tools
- **State Visualization** for game modes
- **Input Debugging** for control issues
- **Memory Profiling** for optimization

### Testing Framework
- **Automated Testing** for core systems
- **Performance Benchmarking** tools
- **Playability Testing** protocols
- **Cross-Platform** compatibility testing
- **User Experience** evaluation metrics

---

## 🎮 Ready to Play!

This AAA-quality prototype provides a complete foundation for VITRUM AXIS, featuring:

✅ **Complete Core Systems** - All major gameplay mechanics implemented  
✅ **Full World 1** - 10 interconnected levels with metroidvania progression  
✅ **AAA-Quality Assets** - Professional-grade visual and audio systems  
✅ **Scalable Architecture** - Ready for expansion to 8 worlds  
✅ **Performance Optimized** - Smooth 60fps gameplay  
✅ **Modular Design** - Easy to extend and modify  

**Open `VITRUM_AXIS.uproject` in Unreal Engine 5.6 to start playing!**

---

*VITRUM AXIS - Where perspective is everything, and momentum is your ally.*
