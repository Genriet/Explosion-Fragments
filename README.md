# Explosion Fragments — Realistic Grenade Fragmentation for DayZ

Explosion Fragments adds configurable fragmentation to DayZ explosives. Instead of relying solely on the game's standard area damage, explosions generate individual fragment raycasts that can hit characters, objects, and surfaces.

Fragmentation behavior can be customized per explosive through JSON configuration, including fragment count, distribution pattern, maximum distance, damage scaling, and visual and audio effects.

## Features

- **Fragment-based explosions:** Generate individual fragments that can hit characters, objects, and surfaces.
- **Configurable fragmentation patterns:** Support spherical, ellipsoidal, and conical fragment distributions.
- **Server-side hit detection:** Fragment raycasts are calculated server-side and processed instantly.
- **Client-side effects:** Fragment particles and sounds spawn in waves from the explosion center to simulate fragment travel time.
- **Distance-based damage scaling:** Optionally reduce fragment damage based on distance from the explosion.
- **Configurable audiovisual effects:** Control particle and sound generation, including minimum distances for spawning effects.

The mod enhances explosions without changing the base damage values of grenades.

## Showcase

[![Watch Explosion Fragments showcase](https://img.youtube.com/vi/qrGWElOxBRI/hqdefault.jpg)](https://youtu.be/qrGWElOxBRI)

## Configuration

Explosion Fragments uses JSON configuration to define fragmentation behavior for each explosive.

Example configuration:

```json
{
  "EffectsPerRPC": 50,
  "Explosives": [
    {
      "explosiveClassname": "RGD5Grenade",
      "fragmentationPreset": {
        "Type": 0,
        "EllipsoidPower": 0.5,
        "EllipsoidDirectional": 0,
        "ConeAngle": 45.0,
        "ConeVector": [0.0, 0.0, 1.0]
      },
      "fragmentAmount": 300,
      "maxDistance": 50.0,
      "damageCoef": 1.0,
      "useDamageCoefByDistance": 1,
      "ammoType": "Bullet_ExplosionFragment",
      "createParticles": 1,
      "minDistanceToSpawnParticles": 5.0,
      "createSounds": 1,
      "minDistanceToSpawnSounds": 10.0,
      "offset": 0.15,
      "delay": 100
    }
  ],
  "EnableDebug": 0
}
```

### Configuration options

| Option | Description |
|---|---|
| `EffectsPerRPC` | Number of effects sent per RPC batch to help manage client load. |
| `explosiveClassname` | Class name of the explosive item, ammunition, trap, or projectile. |
| `fragmentationPreset` | Defines the fragment distribution pattern and its parameters. |
| `fragmentAmount` | Number of fragments generated per explosion. |
| `maxDistance` | Maximum fragment raycast distance. |
| `damageCoef` | Base multiplier for fragment damage. |
| `useDamageCoefByDistance` | Enables distance-based damage reduction. |
| `ammoType` | Ammunition type used to determine fragment damage behavior. |
| `createParticles` | Enables or disables fragment particle effects. |
| `minDistanceToSpawnParticles` | Minimum distance threshold for spawning particle effects. |
| `createSounds` | Enables or disables fragment sounds. |
| `minDistanceToSpawnSounds` | Minimum distance threshold for spawning sound effects. |
| `offset` | Offset from the explosive's origin or the projectile's contact point. |
| `delay` | Timing adjustment in milliseconds to compensate for explosion-effect delays. |
| `EnableDebug` | Enables configuration logging and, in DayZ Diag, debug visualization. |

## Fragmentation Patterns

The fragmentation preset controls how fragments are distributed around an explosion.

| Type | Pattern |
|---|---|
| `0` — `SPHERE` | Distributes fragments in a spherical pattern. |
| `1` — `OPTIMIZEDSPHERE` | Redistributes fragments away from nearest surface improving fragment density. |
| `2` — `ELLIPSOID` | Produces an ellipsoidal distribution, with configurable shape and orientation. |
| `3` — `CONE` | Directs fragments into a cone defined by an angle and direction vector. |

The optimized spherical preset checks the surrounding surface and falls back to the standard spherical pattern when the conditions for optimization are not met.

## Debugging

Set `EnableDebug` to enable additional diagnostics.

- **Script logs:** Configuration information is printed to the script log.
- **DayZ Diag:** Debug lines and surface-check visualization are drawn to help inspect fragment behavior.

### Fragment debug colors

| Color | Meaning |
|---|---|
| White | Fragment did not hit anything. |
| Green | Fragment hit something and both particles and sound are allowed. |
| Yellow | Fragment hit something and only particles are allowed. |
| Blue | Fragment hit something and only sound is allowed. |
| Red | Fragment hit something, but neither particles nor sound are allowed. |

For the optimized spherical preset, diagnostic spheres indicate surface detection:

- **Green sphere:** Surface detected.
- **Red sphere:** Surface not detected.

If more than six spheres detect a surface, the optimized preset is used. Otherwise, the standard spherical pattern is used, such as when an explosion occurs in mid-air or away from a solid surface.

## Known Issues and Limitations

- **No fragment penetration:** Fragments use raycasts rather than physical projectiles, so they cannot penetrate surfaces or objects.
- **Sound queuing:** A large number of fragment sounds can be queued and play with a delay. Increasing `minDistanceToSpawnSounds` can help reduce the number of sounds generated.
- **Performance optimization:** High fragment counts and large numbers of simultaneous effects can affect performance; optimization remains ongoing.
- **Visual timing:** Fragment hit detection occurs before all client-side particles and sounds necessarily reach their corresponding positions. A character may therefore be hit shortly before the visual effects arrive.

## For Modders
- **Workshop:** [Explosion Fragments](https://steamcommunity.com/sharedfiles/filedetails/?id=3441651115)

The mod supports custom fragmentation settings for compatible explosives, ammunition, traps, and projectiles.
Use the JSON configuration to define supported class names, fragment patterns, damage behavior, and audiovisual settings for custom content.
