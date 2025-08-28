class CfgMods
{
	class ExplosionFragments
	{
		type = "mod";
		dependencies[] = { "Game", "World" };
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = { "ExplosionFragments/scripts/3_Game" };
			};
			class worldScriptModule
			{
				value = "";
				files[] = { "ExplosionFragments/scripts/4_World" };
			};
		};
	};
};
class CfgPatches
{
	class ExplosionFragments
	{
		requiredAddons[] = { "DZ_Scripts", "DZ_Data" }; // Decided to add this because Mikero complains when you don't have it. However, they are always loaded first, so you don't actually need them.
	};
};

class CfgVehicles
{
	class HouseNoDestruct;
	class EF_DummyExplosiveAmmoImpactObject : HouseNoDestruct
	{
		scope = 1;
		model = "ExplosionFragments\empty.p3d";// Important for some reason
	};
};

// Couldn't find sound shaders for bullet impact, so made them myself.
class CfgSoundShaders
{
	class EF_soundDefault_SoundShader
	{
		samples[] = { {"dz\sounds\weapons\hits\bullet\soft_ground_1",1},{"dz\sounds\weapons\hits\bullet\soft_ground_2",1},{"dz\sounds\weapons\hits\bullet\soft_ground_3",1},{"dz\sounds\weapons\hits\bullet\soft_ground_4",1},{"dz\sounds\weapons\hits\bullet\soft_ground_5",1},{"dz\sounds\weapons\hits\bullet\soft_ground_6",1},{"dz\sounds\weapons\hits\bullet\soft_ground_7",1},{"dz\sounds\weapons\hits\bullet\soft_ground_8",1} };
		volume = 0.6;
		frequency = 1;
		range = 50;
	};
	class EF_soundGroundSoft_SoundShader : EF_soundDefault_SoundShader
	{
		samples[] = { {"dz\sounds\weapons\hits\bullet\soft_ground_1",1},{"dz\sounds\weapons\hits\bullet\soft_ground_2",1},{"dz\sounds\weapons\hits\bullet\soft_ground_3",1},{"dz\sounds\weapons\hits\bullet\soft_ground_4",1},{"dz\sounds\weapons\hits\bullet\soft_ground_5",1},{"dz\sounds\weapons\hits\bullet\soft_ground_6",1},{"dz\sounds\weapons\hits\bullet\soft_ground_7",1},{"dz\sounds\weapons\hits\bullet\soft_ground_8",1} };
		volume = 0.6;
		frequency = 1;
		range = 50;
	};
	class EF_soundGroundHard_SoundShader
	{
		samples[] = { {"dz\sounds\weapons\hits\bullet\hard_ground_1",1},{"dz\sounds\weapons\hits\bullet\hard_ground_2",1},{"dz\sounds\weapons\hits\bullet\hard_ground_3",1},{"dz\sounds\weapons\hits\bullet\hard_ground_4",1},{"dz\sounds\weapons\hits\bullet\hard_ground_5",1},{"dz\sounds\weapons\hits\bullet\hard_ground_6",1},{"dz\sounds\weapons\hits\bullet\hard_ground_7",1},{"dz\sounds\weapons\hits\bullet\hard_ground_8",1} };
		volume = 0.6;
		frequency = 1;
		range = 60;
	};
	class EF_soundMetal_SoundShader
	{
		samples[] = { {"dz\sounds\weapons\hits\bullet\metal_1",1},{"dz\sounds\weapons\hits\bullet\metal_2",1},{"dz\sounds\weapons\hits\bullet\metal_3",1},{"dz\sounds\weapons\hits\bullet\metal_4",1},{"dz\sounds\weapons\hits\bullet\metal_5",1},{"dz\sounds\weapons\hits\bullet\metal_6",1},{"dz\sounds\weapons\hits\bullet\metal_7",1},{"dz\sounds\weapons\hits\bullet\metal_8",1} };
		volume = 0.6;
		frequency = 1;
		range = 80;
	};
	class EF_soundGlass_SoundShader
	{
		samples[] = { {"dz\sounds\weapons\hits\bullet\glass_1",1},{"dz\sounds\weapons\hits\bullet\glass_2",1},{"dz\sounds\weapons\hits\bullet\glass_3",1},{"dz\sounds\weapons\hits\bullet\glass_4",1},{"dz\sounds\weapons\hits\bullet\glass_5",1},{"dz\sounds\weapons\hits\bullet\glass_6",1},{"dz\sounds\weapons\hits\bullet\glass_7",1},{"dz\sounds\weapons\hits\bullet\glass_8",1} };
		volume = 0.6;
		frequency = 1;
		range = 70;
	};
	class EF_soundGlassArmored_SoundShader
	{
		samples[] = { {"dz\sounds\weapons\hits\bullet\glass_arm_1",1},{"dz\sounds\weapons\hits\bullet\glass_arm_2",1},{"dz\sounds\weapons\hits\bullet\glass_arm_3",1},{"dz\sounds\weapons\hits\bullet\glass_arm_4",1},{"dz\sounds\weapons\hits\bullet\glass_arm_5",1},{"dz\sounds\weapons\hits\bullet\glass_arm_6",1},{"dz\sounds\weapons\hits\bullet\glass_arm_7",1},{"dz\sounds\weapons\hits\bullet\glass_arm_8",1} };
		volume = 0.6;
		frequency = 1;
		range = 70;
	};
	class EF_soundVehiclePlate_SoundShader
	{
		samples[] = { {"dz\sounds\weapons\hits\bullet\metal_plate_1",1},{"dz\sounds\weapons\hits\bullet\metal_plate_2",1},{"dz\sounds\weapons\hits\bullet\metal_plate_3",1},{"dz\sounds\weapons\hits\bullet\metal_plate_4",1},{"dz\sounds\weapons\hits\bullet\metal_plate_5",1},{"dz\sounds\weapons\hits\bullet\metal_plate_6",1},{"dz\sounds\weapons\hits\bullet\metal_plate_7",1},{"dz\sounds\weapons\hits\bullet\metal_plate_8",1} };
		volume = 0.6;
		frequency = 1;
		range = 80;
	};
	class EF_soundWood_SoundShader
	{
		samples[] = { {"dz\sounds\weapons\hits\bullet\wood_1",1},{"dz\sounds\weapons\hits\bullet\wood_2",1},{"dz\sounds\weapons\hits\bullet\wood_3",1},{"dz\sounds\weapons\hits\bullet\wood_4",1},{"dz\sounds\weapons\hits\bullet\wood_5",1},{"dz\sounds\weapons\hits\bullet\wood_6",1},{"dz\sounds\weapons\hits\bullet\wood_7",1},{"dz\sounds\weapons\hits\bullet\wood_8",1} };
		volume = 0.7;
		frequency = 1;
		range = 60;
	};
	class EF_soundHitBody_SoundShader
	{
		samples[] = { {"dz\sounds\weapons\hits\bullet\body_1",1},{"dz\sounds\weapons\hits\bullet\body_2",1},{"dz\sounds\weapons\hits\bullet\body_3",1},{"dz\sounds\weapons\hits\bullet\body_4",1},{"dz\sounds\weapons\hits\bullet\body_5",1},{"dz\sounds\weapons\hits\bullet\body_6",1},{"dz\sounds\weapons\hits\bullet\body_7",1},{"dz\sounds\weapons\hits\bullet\body_8",1} };
		volume = 1;
		frequency = 1;
		range = 20;
	};
	class EF_soundHitBuilding_SoundShader
	{
		samples[] = { {"dz\sounds\weapons\hits\bullet\building_1",1},{"dz\sounds\weapons\hits\bullet\building_2",1},{"dz\sounds\weapons\hits\bullet\building_3",1},{"dz\sounds\weapons\hits\bullet\building_4",1},{"dz\sounds\weapons\hits\bullet\building_5",1},{"dz\sounds\weapons\hits\bullet\building_6",1},{"dz\sounds\weapons\hits\bullet\building_7",1},{"dz\sounds\weapons\hits\bullet\building_8",1} };
		volume = 0.6;
		frequency = 1;
		range = 60;
	};
	class EF_soundHitFoliage_SoundShader
	{
		samples[] = { {"dz\sounds\weapons\hits\bullet\foliage_1",1},{"dz\sounds\weapons\hits\bullet\foliage_2",1},{"dz\sounds\weapons\hits\bullet\foliage_3",1},{"dz\sounds\weapons\hits\bullet\foliage_4",1},{"dz\sounds\weapons\hits\bullet\foliage_5",1},{"dz\sounds\weapons\hits\bullet\foliage_6",1},{"dz\sounds\weapons\hits\bullet\foliage_7",1},{"dz\sounds\weapons\hits\bullet\foliage_8",1} };
		volume = 1;
		frequency = 1;
		range = 40;
	};
	class EF_soundPlastic_SoundShader
	{
		samples[] = { {"dz\sounds\weapons\hits\bullet\plastic_1",1},{"dz\sounds\weapons\hits\bullet\plastic_2",1},{"dz\sounds\weapons\hits\bullet\plastic_3",1},{"dz\sounds\weapons\hits\bullet\plastic_4",1},{"dz\sounds\weapons\hits\bullet\plastic_5",1},{"dz\sounds\weapons\hits\bullet\plastic_6",1},{"dz\sounds\weapons\hits\bullet\plastic_7",1},{"dz\sounds\weapons\hits\bullet\plastic_8",1} };
		volume = 0.6;
		frequency = 1;
		range = 60;
	};
	class EF_soundConcrete_SoundShader
	{
		samples[] = { {"dz\sounds\weapons\hits\bullet\concrete_1",1},{"dz\sounds\weapons\hits\bullet\concrete_2",1},{"dz\sounds\weapons\hits\bullet\concrete_3",1},{"dz\sounds\weapons\hits\bullet\concrete_4",1},{"dz\sounds\weapons\hits\bullet\concrete_5",1},{"dz\sounds\weapons\hits\bullet\concrete_6",1},{"dz\sounds\weapons\hits\bullet\concrete_7",1},{"dz\sounds\weapons\hits\bullet\concrete_8",1} };
		volume = 0.6;
		frequency = 1;
		range = 40;
	};
	class EF_soundRubber_SoundShader
	{
		samples[] = { {"dz\sounds\weapons\hits\bullet\tyre_1",1},{"dz\sounds\weapons\hits\bullet\tyre_2",1},{"dz\sounds\weapons\hits\bullet\tyre_3",1},{"dz\sounds\weapons\hits\bullet\tyre_4",1},{"dz\sounds\weapons\hits\bullet\tyre_5",1},{"dz\sounds\weapons\hits\bullet\tyre_6",1},{"dz\sounds\weapons\hits\bullet\tyre_7",1},{"dz\sounds\weapons\hits\bullet\tyre_8",1} };
		volume = 0.6;
		frequency = 1;
		range = 40;
	};
	class EF_soundWater_SoundShader
	{
		samples[] = { {"dz\sounds\weapons\hits\bullet\water_1",1},{"dz\sounds\weapons\hits\bullet\water_2",1},{"dz\sounds\weapons\hits\bullet\water_3",1},{"dz\sounds\weapons\hits\bullet\water_4",1},{"dz\sounds\weapons\hits\bullet\water_5",1},{"dz\sounds\weapons\hits\bullet\water_6",1},{"dz\sounds\weapons\hits\bullet\water_7",1},{"dz\sounds\weapons\hits\bullet\water_8",1} };
		volume = 0.6;
		frequency = 1;
		range = 50;
	};
	class EF_soundSnow_SoundShader
	{
		samples[] = { {"dz\sounds\weapons\hits\bullet\Snow_01",1},{"dz\sounds\weapons\hits\bullet\Snow_02",1},{"dz\sounds\weapons\hits\bullet\Snow_03",1},{"dz\sounds\weapons\hits\bullet\Snow_04",1},{"dz\sounds\weapons\hits\bullet\Snow_05",1},{"dz\sounds\weapons\hits\bullet\Snow_06",1},{"dz\sounds\weapons\hits\bullet\Snow_07",1},{"dz\sounds\weapons\hits\bullet\Snow_08",1} };
		volume = 0.6;
		frequency = 1;
		range = 40;
	};
	class EF_soundIce_SoundShader
	{
		samples[] = { {"dz\sounds\weapons\hits\bullet\Ice_01",1},{"dz\sounds\weapons\hits\bullet\Ice_02",1},{"dz\sounds\weapons\hits\bullet\Ice_03",1},{"dz\sounds\weapons\hits\bullet\Ice_04",1},{"dz\sounds\weapons\hits\bullet\Ice_05",1},{"dz\sounds\weapons\hits\bullet\Ice_06",1},{"dz\sounds\weapons\hits\bullet\Ice_07",1},{"dz\sounds\weapons\hits\bullet\Ice_08",1} };
		volume = 0.6;
		frequency = 1;
		range = 60;
	};
};

class CfgSoundSets
{
	class EF_ImpactBase_SoundSet
	{
		sound3DProcessingType = "smallItem3DProcessingType";
		volumeCurve = "characterAttenuationCurve";
		spatial = 1;
		doppler = 0;
		loop = 0;
	};
	class EF_soundDefault_SoundSet : EF_ImpactBase_SoundSet
	{
		soundShaders[] = { "EF_soundDefault_SoundShader" };
	};
	class EF_soundGroundSoft_SoundSet : EF_ImpactBase_SoundSet
	{
		soundShaders[] = { "EF_soundGroundSoft_SoundShader" };
	};
	class EF_soundGroundHard_SoundSet : EF_ImpactBase_SoundSet
	{
		soundShaders[] = { "EF_soundGroundHard_SoundShader" };
	};
	class EF_soundMetal_SoundSet : EF_ImpactBase_SoundSet
	{
		soundShaders[] = { "EF_soundMetal_SoundShader" };
	};
	class EF_soundGlass_SoundSet : EF_ImpactBase_SoundSet
	{
		soundShaders[] = { "EF_soundGlass_SoundShader" };
	};
	class EF_soundGlassArmored_SoundSet : EF_ImpactBase_SoundSet
	{
		soundShaders[] = { "EF_soundGlassArmored_SoundShader" };
	};
	class EF_soundVehiclePlate_SoundSet : EF_ImpactBase_SoundSet
	{
		soundShaders[] = { "EF_soundVehiclePlate_SoundShader" };
	};
	class EF_soundWood_SoundSet : EF_ImpactBase_SoundSet
	{
		soundShaders[] = { "EF_soundWood_SoundShader" };
	};
	class EF_soundHitBody_SoundSet : EF_ImpactBase_SoundSet
	{
		soundShaders[] = { "EF_soundHitBody_SoundShader" };
	};
	class EF_soundHitBuilding_SoundSet : EF_ImpactBase_SoundSet
	{
		soundShaders[] = { "EF_soundHitBuilding_SoundShader" };
	};
	class EF_soundHitFoliage_SoundSet : EF_ImpactBase_SoundSet
	{
		soundShaders[] = { "EF_soundHitFoliage_SoundShader" };
	};
	class EF_soundPlastic_SoundSet : EF_ImpactBase_SoundSet
	{
		soundShaders[] = { "EF_soundPlastic_SoundShader" };
	};
	class EF_soundConcrete_SoundSet : EF_ImpactBase_SoundSet
	{
		soundShaders[] = { "EF_soundConcrete_SoundShader" };
	};
	class EF_soundRubber_SoundSet : EF_ImpactBase_SoundSet
	{
		soundShaders[] = { "EF_soundRubber_SoundShader" };
	};
	class EF_soundWater_SoundSet : EF_ImpactBase_SoundSet
	{
		soundShaders[] = { "EF_soundWater_SoundShader" };
	};
	class EF_soundSnow_SoundSet : EF_ImpactBase_SoundSet
	{
		soundShaders[] = { "EF_soundSnow_SoundShader" };
	};
	class EF_soundIce_SoundSet : EF_ImpactBase_SoundSet
	{
		soundShaders[] = { "EF_soundIce_SoundShader" };
	};
};

class CfgAmmo
{
    class Bullet_45ACP;
    class Bullet_ExplosionFragment : Bullet_45ACP {};
};