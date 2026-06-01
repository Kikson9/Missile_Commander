# Missile Commander — C++ Project Plan

### Course Project | Raylib Game Improvement

### Estimated Duration: 3–4 Weeks

---

## Overview

**Base game:** Missile Commander — from the raylib-games classics collection
**Source:** https://github.com/raysan5/raylib-games/tree/master/classics/src
**Original file:** `missile_commander.c` (written in C)
**Project file:** `missile_commander.cpp` (converted to C++ — see section below)

The plan is to first convert the original C file to C++, then fully understand
the existing codebase, then add 9 new features that transform it from a
bare-bones tech demo into a polished, complete game. Every feature is chosen
because it directly addresses something missing from the original code — not
features added on top, but improvements that fill real gaps.

---

## Why Missile Commander

The original raylib version already has the hardest part working:

- Mouse-click interceptor firing system
- Growing/shrinking explosion radius collision
- Missiles that target buildings and launchers
- 6 distinct object types (Missile, Interceptor, Explosion, Launcher,
  Building, ground)

What it is completely missing:

- No score system (no variable exists anywhere in the code)
- No wave system (missiles spawn forever at the same speed)
- No progression, no structure, no game loop
- Plain white background, no visual polish
- No persistence (nothing saved between sessions)

This makes it an ideal project base — the core mechanic works, but everything
that makes it a real game needs to be built.

---

## Step 0 — Converting C to C++

The original file is written in C (C99). The course requires C++. This
conversion is NOT a big deal — C and C++ are closely related and most valid
C code is already valid C++ code. No logic needs to be rewritten. The
conversion is a series of small, specific fixes that took us about 30 minutes.

### Why it is easy

C++ is a superset of C. Almost everything that compiles in C also compiles
in C++. The changes below are the only ones that actually matter for this
specific file.

### The 5 conversion steps

STEP 1 — Rename the file

---

missile_commander.c → missile_commander.cpp

That one rename tells the compiler to treat the file as C++.
Do this first, then compile and read the errors. There will be very few.

STEP 2 — Remove the stdbool.h include

---

The original file has:
#include <stdbool.h>

In C, this include is needed to use 'bool', 'true', and 'false'.
In C++, bool is a built-in type — the include is no longer needed.

Remove that line. bool, true, and false will still work exactly the same.

STEP 3 — Fix struct type usage (the main C vs C++ difference)

---

In C, when you define a struct you must write the word 'struct' every time
you use it as a type — unless a typedef exists.

The original file uses 'typedef struct' for all its types (Missile,
Interceptor, Explosion, Launcher, Building) which already handles this.
So in practice there are likely 0 places to fix here. But if the compiler
complains about a struct type not being found, this is why — just remove
the 'struct' keyword before it.

Before (C style): struct Missile missile[MAX_MISSILES];
After (C++ style): Missile missile[MAX_MISSILES];

STEP 4 — Fix C-style void\* casts (if any appear)

---

C allows implicit casting from void\* (e.g. from malloc) to any pointer.
C++ requires an explicit cast.

This file uses raylib memory functions rather than raw malloc, so this
probably will not appear. But if the compiler says something like
"cannot convert void* to SomeType*", fix it like this:

Before: SomeType *p = malloc(sizeof(SomeType));
After: SomeType *p = (SomeType\*)malloc(sizeof(SomeType));

In practice this file uses fixed-size arrays everywhere so malloc does
not appear at all — this step is included just in case.

STEP 5 — Variable declarations (nothing to change, but good to know)

---

C99 requires all variables to be declared at the top of a function block.
C++ lets you declare variables anywhere — right next to where you use them.

The original code already follows C99 style so nothing breaks. But for all
NEW code added during this project, you can declare variables right where
they are first used. This is standard C++ practice.

Example of what is now allowed when writing new features:
for (int i = 0; i < MAX_MISSILES; i++) { ... }
// 'i' is declared inside the loop — not allowed in strict C89

      float dist = Vector2Distance(a, b);
      // can declare dist right where it is needed, mid-function

### Summary — what changes vs what stays the same

Changes (the entire conversion): - File renamed from .c to .cpp (1 action) - #include <stdbool.h> removed (1 line deleted) - Any bare 'struct X' type usage updated if needed (likely 0 places)

Stays exactly the same: - All raylib function calls (DrawText, CheckCollision*, Vector2*, etc.) - All structs and their fields - All game logic in UpdateGame(), DrawGame(), InitGame() - All #define constants and MAX\_ values - The entire main() game loop - stdio.h file I/O (fopen, fprintf, fscanf) - math.h functions (sinf, cosf, sqrtf, etc.)

Raylib's raylib.h header is written to be fully compatible with both C
and C++, so zero changes are needed for any raylib calls.

### C++ features that will be used when adding new features

enum class WaveState { PLAYING, WAVE_CLEAR, WAVE_INCOMING };
// Scoped enums — cleaner than C-style unscoped enums

bool isMirv = false;
// bool is native in C++, no include needed

for (int i = 0; i < MAX_PARTICLES; i++) { ... }
// Variables declared inside loop scope

No classes, inheritance, templates, or STL containers are required.
The project stays close to C-style procedural code — just compiled as
C++ with C++ features used where they make things cleaner.

---

## The 9 Features

### Visual Features (transform how it looks)

## DONE

**Feature 1 — Black space background + starfield**

- Replace ClearBackground(RAYWHITE) with ClearBackground(BLACK)
- Add a Vector2 stars[150] array, initialized once in InitGame()
- Draw each star as a 1px dot in DrawGame()
- Estimated code: ~15 lines
- Impact: single biggest visual transformation in the project

## DONE

**Feature 2 — Score system + HUD**

- Add a global int score = 0
- Hook into the existing missile-kill collision check to award points
- Draw score, wave number, and building-count icons in DrawGame()
- Estimated code: ~25 lines
- Impact: game now has feedback and reason to play well

**Feature 7 — Particle debris on missile destruction**

- New Particle struct: position, velocity, alpha, size, active
- Spawn 8 particles from missile position on kill, with random angles
- Each frame: move by velocity, fade alpha, deactivate at 0
- Estimated code: ~50 lines
- Impact: every missile kill feels satisfying and responsive

**Feature 8 — City destruction screen flash**

- Global int screenFlash = 0
- On building destroyed: set screenFlash = 8, spawn red particles
- In DrawGame: full-screen Fade(RED, screenFlash \* 0.04f), decrement
- Estimated code: ~10 lines
- Impact: losing a city feels dramatic and weighty

### Gameplay Features (turn it into a real game)

**Feature 3 — Wave system with escalating difficulty**

- New globals: wave, missilesThisWave, missilesLaunched, missilesDestroyed,
  missileSpeed
- Missile spawn stops after missilesThisWave are launched
- Wave ends when all missiles are accounted for (destroyed or hit buildings)
- Each wave: +4 missiles, +0.3f speed
- New waveState enum: PLAYING / WAVE_CLEAR / WAVE_INCOMING
- Estimated code: ~40 lines
- Impact: gives the game its core loop and structure

**Feature 4 — Wave announcement screen**

- During WAVE_CLEAR: centered "WAVE CLEARED" text with Fade() pulse effect
- Show bonus points earned (per surviving building)
- Switch to WAVE_INCOMING: "WAVE 3 — INCOMING" in red for 1 second
- Reuses the Fade() function already present in the codebase
- Estimated code: ~30 lines
- Impact: creates tension, rhythm, and breathing room between waves

**Feature 5 — High score with file persistence**

- SaveHighScore(int score): fopen("highscore.txt", "w") + fprintf
- LoadHighScore(): fopen("highscore.txt", "r") + fscanf
- Called on game start (load) and game over (save if new best)
- stdio.h is already included in the original file
- Estimated code: ~25 lines
- Impact: demonstrates file I/O, gives players a reason to replay

### Advanced Features (technical depth)

**Feature 6 — MIRVs (splitting missiles)**

- Add bool isMirv and bool hasSplit fields to the existing Missile struct
- From wave 3 onwards, ~25% of missiles flagged as MIRVs on spawn
- When a MIRV reaches screenHeight/2: deactivate it, spawn 3 child missiles
  fanning toward different buildings
- MIRVs drawn in orange so players can identify and prioritize them
- Estimated code: ~60 lines
- Impact: most visually dramatic feature, requires new strategy from player

**Feature 9 — Smart bombs that dodge explosions**

- Add bool isSmart field to Missile struct
- Enabled from wave 5 onwards (~15% of missiles)
- Each frame: loop active explosions, if one is within threat range push the
  missile's speed vector away using Vector2Normalize + Vector2Subtract
- Smart bombs drawn in cyan/white for visual distinction
- Estimated code: ~45 lines
- Impact: most technically impressive feature, demonstrates vector math and
  basic AI behaviour

---

## 4-Week Schedule

### Week 1 — Convert + understand the code + first visual wins (Days 0–7)

Goal: convert to C++, understand every line of the original, make first
visible improvements.
End state: game compiles as C++, looks completely different, codebase
fully understood.

Day 0 C to C++ conversion (approx. 30 minutes). 1. Copy missile_commander.c, rename copy to missile_commander.cpp 2. Open the .cpp file, delete the line: #include <stdbool.h> 3. Compile. Read any errors — there will be very few. 4. Fix each error using the conversion guide in Step 0 above. 5. Game should run identically to the original once it compiles. 6. Keep the original .c file untouched as a reference point.
Also update your IDE or build system to compile .cpp if needed.

Day 1–2 Download and run the original .c version first. Play it for 20
minutes. Write down in plain words: how missiles spawn, what
triggers an explosion, how buildings get destroyed, what ends
the game. Then switch to working from the .cpp version.

Day 3 Read every struct in the file. For each field, write what it does.
Understand that the 'active' bool is the on/off switch for
everything in this codebase.

Day 4 Trace the full game loop: main() → InitGame() → UpdateGame() →
DrawGame(). Find where missiles spawn, move, collide, and where
buildings lose their active state. Draw this on paper.

Day 5 Implement Feature 1: Black background + starfield.
Compile and compare before vs after.

Day 6–7 Implement Feature 2: Score system + HUD.

Week 1 milestone: file compiles cleanly as C++. Game looks completely
different. Every line of the original is understood.

---

### Week 2 — Build the real game loop (Days 8–14)

Goal: install the missing game loop so it functions as a proper game.
End state: complete, playable game with structure, score, and persistence.

Day 8–9 Implement Feature 3 (part 1): Wave system variables.
Add wave globals. Modify missile spawn to respect wave count.
Make missile speed use the missileSpeed variable.

Day 10 Implement Feature 3 (part 2): Wave transition logic.
Add waveState enum. Handle WAVE_CLEAR → WAVE_INCOMING → PLAYING
transitions. Award bonus points per surviving building.

Day 11 Implement Feature 4: Wave announcement screen.
Pulsing "WAVE CLEARED" and "WAVE X — INCOMING" messages using
Fade() already in the codebase.

Day 12–13 Implement Feature 5: High score file persistence.
SaveHighScore() and LoadHighScore() using stdio.h (already
included in the original).

Day 14 Test and fix. Play through 5 full waves. Verify score saves and
reloads. Stable codebase before moving to week 3 is critical.

Week 2 milestone: fully functional game with wave progression, score,
and persistent high score. Strong enough to submit as-is if needed.

---

### Week 3 — Add the wow factor (Days 15–21)

Goal: visual drama and enemy variety that make the demo unforgettable.
End state: project is complete and highly impressive.

Day 15–17 Implement Feature 6: MIRVs.
Extend Missile struct with isMirv and hasSplit bools.
Splitting logic at screenHeight/2. Fan 3 child missiles to
different targets. Draw MIRVs in orange.

Day 18 Implement Feature 7: Particle debris.
New Particle struct + pool array. Spawn on missile kill.
Per-frame movement, alpha fade, deactivation.

Day 19 Implement Feature 8: City destruction flash.
screenFlash variable + full-screen red Fade() overlay.
Red particle burst from destroyed building position.

Day 20–21 Integration testing. Verify all features interact correctly.
Do MIRVs trigger particles? Does flash trigger for child missile
hits? Does score handle all missile types? Fix bugs.

Week 3 milestone: project is complete. All 8 features working together.
Week 4 smart bombs are a bonus on top of an already strong submission.

---

### Week 4 — Advanced feature + polish + presentation (Days 22–28)

Goal: add the most technically impressive feature, polish everything,
rehearse the presentation.

Day 22–24 Implement Feature 9: Smart bombs.
Add isSmart bool to Missile. Threat detection loop over active
explosions. Velocity steering using Vector2Normalize and
Vector2Subtract. Cyan/white visual distinction.

Day 25 Polish pass. Consistent naming, comments, no debug output.
Review HUD readability. Tune wave difficulty curve.
Verify game over and restart work cleanly.

Day 26–27 Prepare presentation. Build before/after demo script.
Practice explaining: "In the original code, line X does Y.
I added Z here because of W." Print original source to show
what existed before changes.

Day 28 Final run-through. Fix last bugs. Freeze the build.
A stable, well-rehearsed demo beats one last feature that might
break something.

Week 4 milestone: fully polished, all 9 features, rehearsed and ready.

---

## Safety Net

The project is complete and presentable at the end of Week 3 with 8 features.
Smart bombs (Feature 9) are a genuine bonus. If Week 4 runs short on time,
skipping smart bombs still results in an impressive, complete submission.

Feature priority if time becomes tight:
Must have (Week 1–2): Features 1, 2, 3, 4, 5
Strong addition (Week 3): Features 6, 7, 8
Bonus (Week 4): Feature 9

---

## C++ Concepts Demonstrated

Struct extension ........... MIRVs, particles, smart bombs
(adding fields to existing structs)
Array management ........... Particle pool, starfield
(fixed-size arrays, active/inactive patterns)
Enums + state machines ..... Wave system
(PLAYING / WAVE_CLEAR / WAVE_INCOMING states)
File I/O ................... High score persistence
(fopen, fprintf, fscanf from stdio.h)
Vector math ................ Smart bomb steering
(Vector2Normalize, Vector2Subtract, dot product)
Render layering ............ HUD, screen flash, particle overlay
(draw order, Fade() alpha blending)
Game loop architecture ..... Understanding and extending an existing loop
(InitGame / UpdateGame / DrawGame pattern)

---

## Presentation Structure (5–10 minutes)

1. Run the original game first.
   Show the teacher what exists: working core mechanic but no score,
   no waves, white background, missiles spawn forever.

2. Show the code that was understood before any changes.
   Point to the missile struct, the explosion collision check, the building
   deactivation line. Demonstrate understanding of the original architecture.

3. Run the improved version.
   Let the visual difference speak for itself before explaining anything.

4. Walk through each feature with the code.
   For each: show the specific lines added and where they hook into
   the existing codebase.

5. Summarise the C++ concepts covered.
   Connect each feature to a concept: structs, arrays, enums, file I/O,
   vector math, state machines.

Key point to communicate:
"I did not add features on top of code I didn't understand. I first read
every struct, traced the full game loop, and mapped out every interaction.
Then I planned each feature around the existing architecture before writing
a single line."

---

## Estimated Code Added

Conversion (C to C++) .............. ~0 new lines (3 small fixes)
Feature 1 (Starfield) ............ ~15 lines
Feature 2 (Score + HUD) .......... ~25 lines
Feature 3 (Wave system) ........... ~40 lines
Feature 4 (Wave screen) ........... ~30 lines
Feature 5 (File I/O) .............. ~25 lines
Feature 6 (MIRVs) ................. ~60 lines
Feature 7 (Particles) ............. ~50 lines
Feature 8 (City flash) ............ ~10 lines
Feature 9 (Smart bombs) ........... ~45 lines

---

Total new code ..................... ~300 lines

Original file size: ~565 lines
Final file size (estimated): ~865 lines

---

## Resources

Raylib games repo: https://github.com/raysan5/raylib-games
Raylib cheatsheet: https://www.raylib.com/cheatsheet/cheatsheet.html
Raylib examples: https://www.raylib.com/examples.html
