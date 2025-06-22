# Kill Animation System - Integration Guide
## TIAN0517 - Jy技術團隊 Implementation

### 🎯 Quick Start Guide

#### 1. Database Setup
Execute the SQL script to create required tables:
```sql
-- Run this in your game database
EXEC sp_executesql N'
-- Content from Database/KillAnimation.sql
-- Creates: KillAnimation, KillAnimationLog, KillAnimationCommands tables
'
```

#### 2. Test GM Commands
Login as GM (Level 3+) and test the animations:

```
!killanim_sword     - Golden sword slash (2.5s)
!killanim_pierce    - Silver blade pierce (1.8s) 
!killanim_explode   - Orange explosion (3.0s)
!killanim_ice       - Blue ice shatter (2.2s)
!killanim_flame     - Red flame burn (2.8s)
!killanim_lightning - Purple lightning (2.0s)
!killanim_test      - Test all animations in circle
!killanim_random    - Random animation
```

#### 3. Integration Points

**A. Combat System Integration:**
```cpp
// In your combat/damage processing code:
void OnPlayerKillEnemy(GLChar* pKiller, GLChar* pTarget) 
{
    // Determine animation based on weapon/skill
    EMKILL_ANIMATION_TYPE emType = DetermineKillAnimation(pKiller);
    
    // Play the kill animation
    GLKillAnimation::GetInstance().ProcessKillAnimation(pKiller, pTarget, emType);
}
```

**B. Skill System Integration:**
```cpp
// In skill execution code:
if (IsKillingBlow(pTarget)) 
{
    EMKILL_ANIMATION_TYPE emType = GetSkillKillAnimation(skillId);
    GLKillAnimation::GetInstance().ProcessKillAnimation(pCaster, pTarget, emType);
}
```

### 🏗️ Build Integration

#### Visual Studio Project Files
Add these files to your project:

**Engine Library (enginelib.vcproj):**
- `DxEffect/KillAnimation/KillAnimSystem.cpp`
- `DxEffect/KillAnimation/KillAnimSystem.h`

**Server Logic (RanLogicServer.vcproj):**
- `KillAnimation/GLKillAnimation.cpp`
- `KillAnimation/GLKillAnimation.h`

#### Include Paths
Ensure these directories are in your include paths:
- `Source/enginelib/DxEffect/KillAnimation/`
- `Source/RanLogicServer/KillAnimation/`

### 🎨 Asset Integration

#### Texture Files (DDS Format)
Replace placeholder files in `DxEffect/KillAnimation/Textures/`:
- `sword_slash.dds` - Gold blade sweep animation frames
- `pierce_fatal.dds` - Silver pierce effect frames
- `explosion_blast.dds` - Orange explosion frames
- `ice_shatter.dds` - Blue ice crystal break frames
- `flame_burn.dds` - Red flame effect frames
- `lightning_strike.dds` - Purple lightning frames

#### Sound Files (WAV Format)
Replace placeholder files in `DxEffect/KillAnimation/Sounds/`:
- `sword_slash.wav` - Sharp blade cutting sound
- `pierce_fatal.wav` - Metal piercing sound
- `explosion_blast.wav` - Explosion blast sound
- `ice_shatter.wav` - Ice breaking sound
- `flame_burn.wav` - Fire crackling sound
- `lightning_strike.wav` - Thunder strike sound

### 🔧 Configuration

#### Animation Parameters
Modify in `KillAnimSystem.cpp` InitializeAnimationData():
```cpp
// Adjust duration, colors, scale per animation type
data.fDuration = 2.5f;           // Animation length
data.dwColor = D3DCOLOR_XRGB(255, 215, 0);  // Effect color
data.fScale = 1.2f;              // Size multiplier
```

#### Performance Tuning
```cpp
// In DxKillAnimationManager
static const int MAX_CONCURRENT_ANIMATIONS = 10;  // Adjust as needed
static const float CLEANUP_INTERVAL = 1.0f;       // Dead animation cleanup
```

### 📊 Database Schema

#### Main Tables Created:
1. **KillAnimation** - Animation configurations
2. **KillAnimationLog** - Usage tracking  
3. **KillAnimationCommands** - GM command mappings

#### Query Examples:
```sql
-- Get most used animation
SELECT TOP 1 AnimationName, COUNT(*) as UseCount 
FROM V_KillAnimationStats 
ORDER BY UseCount DESC

-- Check animation configuration
SELECT * FROM KillAnimation WHERE AnimationID = 12001
```

### 🚀 Deployment Checklist

- [ ] Database schema deployed
- [ ] Server code compiled and tested
- [ ] GM commands verified working
- [ ] Texture files replaced with actual assets
- [ ] Sound files replaced with actual assets
- [ ] Performance tested with multiple concurrent animations
- [ ] Integration with combat system complete

### 🐛 Troubleshooting

#### Common Issues:

**1. GM Commands Not Working:**
- Check user has GM Level 3+
- Verify server restart after deployment
- Check InstanceSystemFieldGMCommand.cpp compilation

**2. Animations Not Visible:**
- Verify texture files exist and are valid DDS
- Check DirectX device initialization
- Ensure DxKillAnimationManager::InitDeviceObjects called

**3. Performance Issues:**
- Monitor concurrent animation count
- Increase cleanup frequency if needed
- Check texture memory usage

#### Debug Commands:
```cpp
// Add to GM commands for debugging
notifyField("Kill anim system status: %d active animations", 
    DxKillAnimationManager::GetInstance().GetActiveCount());
```

### 📈 Future Enhancements

1. **Particle Effects**: Replace simple scaling with particle systems
2. **Animation Editor**: Visual tool for tweaking parameters
3. **Weapon-Specific**: Different animations per weapon type
4. **Chain Animations**: Multiple kills trigger special effects
5. **Player Customization**: Let players choose preferred effects

### 📞 Support

For technical issues or questions:
- Contact: TIAN0517
- Team: Jy技術團隊
- Repository: Check latest commits for updates

---
**Status: ✅ Ready for Production**  
**Version: 1.0**  
**Compatibility: RanOnline DirectX 9**