# VITRUM AXIS - Debugging Guide
# Fixing Crash Issues and Compilation Problems

## 🚨 **CRITICAL ISSUES IDENTIFIED AND FIXED**

### 1. **Missing Function Declaration** ✅ FIXED
**Problem**: `InitializeShardProperties()` was implemented in `.cpp` but not declared in `.h`
**Fix**: Added function declaration to `VitrumArtisanShard.h`
**Status**: ✅ RESOLVED

### 2. **Potential Null Pointer Issues**
**Problem**: Several classes don't check for null pointers before accessing components
**Risk**: Runtime crashes when components aren't properly initialized

### 3. **Missing Error Handling**
**Problem**: Functions don't validate inputs or check for valid states
**Risk**: Undefined behavior and crashes

## 🔧 **IMMEDIATE FIXES NEEDED**

### Fix 1: Add Null Pointer Checks to VitrumMarble
```cpp
// In VitrumMarble::BeginPlay()
if (CollisionSphere)
{
    // ... existing code
}
else
{
    UE_LOG(LogTemp, Error, TEXT("CollisionSphere is null in VitrumMarble"));
    return;
}
```

### Fix 2: Add Error Handling to VitrumGameMode
```cpp
// In VitrumGameMode::SetGameState()
if (NewState == CurrentGameState)
{
    return; // No change needed
}
```

### Fix 3: Add Validation to VitrumWorldManager
```cpp
// In VitrumWorldManager::BeginPlay()
if (!WorldRoot)
{
    UE_LOG(LogTemp, Error, TEXT("WorldRoot is null in VitrumWorldManager"));
    return;
}
```

## 🎯 **STEP-BY-STEP DEBUGGING PROCESS**

### Step 1: Verify Project Structure
1. **Check Engine Version**: Ensure using Unreal Engine 5.3
2. **Verify .uproject file**: Should be 600B+ (not 1KB)
3. **Check C++ compilation**: All files should compile without errors

### Step 2: Test Basic Compilation
1. **Open Visual Studio**: Open `VITRUM_AXIS.sln`
2. **Build Solution**: Check for compilation errors
3. **Fix any errors**: Address missing includes or declarations

### Step 3: Test in Unreal Editor
1. **Open Project**: Double-click `VITRUM_AXIS.uproject`
2. **Check for warnings**: Look for missing assets or references
3. **Test basic functionality**: Try to create a simple level

### Step 4: Isolate Crash Source
1. **Comment out problematic classes**: Start with `VitrumArtisanShard`
2. **Test incrementally**: Add classes back one by one
3. **Check logs**: Look for error messages in Output Log

## 🛠️ **COMMON CRASH CAUSES**

### 1. **Missing Asset References**
- **Symptoms**: "Failed to load" errors
- **Fix**: Create placeholder assets or remove references

### 2. **Invalid Component Setup**
- **Symptoms**: "Component is null" errors
- **Fix**: Add null checks before component access

### 3. **Memory Access Violations**
- **Symptoms**: Random crashes, access violations
- **Fix**: Validate pointers before dereferencing

### 4. **Infinite Loops**
- **Symptoms**: Editor hangs, high CPU usage
- **Fix**: Check Tick functions and event loops

## 🔍 **DEBUGGING TOOLS**

### 1. **Unreal Editor Logs**
- **Window → Developer Tools → Output Log**
- **Look for**: Error, Warning, and Log messages
- **Filter by**: Your project name

### 2. **Visual Studio Debugger**
- **Attach to UnrealEditor.exe**
- **Set breakpoints**: In problematic functions
- **Check call stack**: When crashes occur

### 3. **Unreal Insights**
- **Enable profiling**: In project settings
- **Monitor performance**: Identify bottlenecks

## 📋 **CHECKLIST FOR STABLE BUILD**

### ✅ **Pre-Compilation Checks**
- [ ] All header files have proper include guards
- [ ] All function declarations match implementations
- [ ] No circular dependencies
- [ ] All required modules are included in Build.cs

### ✅ **Runtime Checks**
- [ ] All components are properly initialized
- [ ] Null pointer checks are in place
- [ ] Error handling for edge cases
- [ ] Memory management is correct

### ✅ **Asset Checks**
- [ ] All referenced assets exist
- [ ] Material references are valid
- [ ] Blueprint parent classes are correct
- [ ] Input mappings are properly set up

## 🚀 **QUICK FIXES FOR IMMEDIATE TESTING**

### 1. **Disable Problematic Features**
```cpp
// In VitrumArtisanShard::BeginPlay()
// Comment out problematic initialization
// InitializeShardProperties(); // TEMPORARILY DISABLED
```

### 2. **Add Safety Checks**
```cpp
// In all classes, add null checks
if (!SomeComponent)
{
    UE_LOG(LogTemp, Warning, TEXT("Component is null"));
    return;
}
```

### 3. **Simplify Constructor Logic**
```cpp
// Move complex initialization to BeginPlay
// Keep constructors simple
```

## 📞 **GETTING HELP**

### 1. **Check Error Messages**
- Copy exact error text
- Note the line numbers
- Include call stack if available

### 2. **Provide Context**
- What were you doing when it crashed?
- Which classes were involved?
- What assets were being loaded?

### 3. **Test Incrementally**
- Start with minimal setup
- Add features one by one
- Test after each addition

## 🎯 **SUCCESS INDICATORS**

### ✅ **Project Opens Successfully**
- No compilation errors
- No missing asset warnings
- Editor loads without crashes

### ✅ **Basic Functionality Works**
- Can create new levels
- Can place actors
- Can run in Play mode

### ✅ **Core Systems Function**
- Player movement works
- Basic interactions work
- No runtime crashes

---

**Remember**: Start simple, test often, and fix issues one at a time. The foundation is solid - we just need to identify and fix the specific crash causes.
