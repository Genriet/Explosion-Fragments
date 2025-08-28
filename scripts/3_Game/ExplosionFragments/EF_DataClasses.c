static const int EFEFFECTSLIST = 9356293865186;
#ifdef DIAG_DEVELOPER
static const int EFDEBUGLIST = 23523036452352;
#endif

enum FragmentationPresetType
{
	SPHERE,
	OPTIMIZEDSPHERE,
	ELLIPSOID,
	CONE
};

class FragmentationPreset
{
	int Type;// 0 - SPHERE, 1 - OPTIMIZEDSPHERE, 2 - ELLIPSOID, 3 - CONE

	float EllipsoidPower;// How flat the ellipsoid is. 1 is a flat circle
	bool EllipsoidDirectional; // Makes it relative to the object's dimensions

	float ConeAngle;// Angle of the cone in degrees
	vector ConeVector; // Directional vector in object space

	void FragmentationPreset(int id = FragmentationPresetType.SPHERE, float power = 0.5, bool directional = false, float angle = 45, vector vec = vector.Forward)
	{
		Type = id;
		EllipsoidPower = power;
		EllipsoidDirectional = directional;
		ConeAngle = angle;
		ConeVector = vec;
	}
};

class EF_ImpactEffectData
{
	vector Pos;
	vector Rot;

	int ParticleId;
	int SoundId;

	float Distance;

	void EF_ImpactEffectData(vector pos, vector rot, int id, int sound, float distance)
	{
		Pos = pos;
		Rot = rot;
		ParticleId = id;
		SoundId = sound;
		Distance = distance;
	}
};

class EF_Data
{
	string explosiveClassname; // Classname of an explosive item or bullet

	ref FragmentationPreset fragmentationPreset;

	int fragmentAmount;// Amount of fragments per explosion
	float maxDistance;// Maximum distance for fragments

	float damageCoef;// Base multiplier for fragment damage
	bool useDamageCoefByDistance;// If enabled, fragment damage decreases with distance (distance coefficient from 1 to 0, based on distance, using Quart easing) + damage coefficient

	string ammoType;// Type of ammo used for fragments; defaults to Bullet_ExplosionFragment which is a copy of .45 ACP

	bool createParticles;// If particles should be created
	float minDistanceToSpawnParticles;// Minimum distance to spawn particles

	bool createSounds;// If sounds should be created
	float minDistanceToSpawnSounds;// Minimum distance to spawn sounds

	float offset; // Offset from the item's zero point or the point of contact in the case of projectiles. It's better to leave it at the default 
	int delay; // Explosion FX has a slight delay. This compensates for it (in milliseconds)

	void EF_Data(string name, FragmentationPreset preset, int fragAmount = 300,  float maxDist = 50, float dmgCoef = 1, bool distCoef = true, string ammo = "Bullet_ExplosionFragment",
		bool createP = true, float minDistP = 5, bool createS = true, float minDistS = 10, float offs = 0.15, int time = 100)
	{
		explosiveClassname = name;

		fragmentationPreset = preset;

		fragmentAmount = fragAmount;
		maxDistance = maxDist;

		damageCoef = dmgCoef;
		useDamageCoefByDistance = distCoef;

		ammoType = ammo;

		createParticles = createP;
		minDistanceToSpawnParticles = minDistP;

		createSounds = createS;
		minDistanceToSpawnSounds = minDistS;

		offset = offs;
		delay = time;
	}
};

class EF_DebugData
{
	vector begPos;
	ref array<vector> fragEndPos = new array<vector>;
	ref array<int> fragEndType = new array<int>;
	
	ref array<vector> opSphEndPos = new array<vector>;
	ref array<int> opSphEndType = new array<int>;
}