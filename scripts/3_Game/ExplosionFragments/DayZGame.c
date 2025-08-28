class ExplosionFragments_Config
{
	int EffectsPerRPC;
	ref array<ref EF_Data> Explosives = new array<ref EF_Data>();
	bool EnableDebug;
};

modded class DayZGame
{
	protected ref ExplosionFragments_Config m_ExplosionFragments_Config;
	protected ref array<string> m_EF_ExplosiveBullets = new array<string>;// Used in FirearmEffects to spawn fragments on explosive projectiles

	static ref ScriptInvoker EF_ExplosiveHitInvoker = new ScriptInvoker();// Used to send data from 3_Game to 4_World for explosive projectiles

#ifdef DIAG_DEVELOPER
	ref array<Shape> m_EF_DebugShapes = new array<Shape>;
#endif

	// Preparing stuff we need
	void DayZGame()
	{
		EF_LookUpTable.EF_SetUpTable();
		EF_SetupAmmoList();
	}

	ExplosionFragments_Config GetExplosionFragments_Config()
	{
		if (!m_ExplosionFragments_Config)
		{
			if (!FileExist("$profile:\\ExplosionFragments"))
			{
				MakeDirectory("$profile:\\ExplosionFragments");
			}
			if (!FileExist("$profile:\\ExplosionFragments\\ExplosionFragments.json"))
			{
				m_ExplosionFragments_Config = EF_CreateDefaultConfig();

				JsonFileLoader<ExplosionFragments_Config<ref EF_Data<ref FragmentationPreset>>>.JsonSaveFile("$profile:\\ExplosionFragments\\ExplosionFragments.json", m_ExplosionFragments_Config);
			}
			else
			{
				JsonFileLoader<ExplosionFragments_Config<ref EF_Data<ref FragmentationPreset>>>.JsonLoadFile("$profile:\\ExplosionFragments\\ExplosionFragments.json", m_ExplosionFragments_Config);

				if (!m_ExplosionFragments_Config)
				{
					m_ExplosionFragments_Config = EF_CreateDefaultConfig();
					ErrorEx("EXPLOSION FRAGMENTS JSON IS INVALID, USING DEFAULT ONE. REGENERATE JSON");
				}
			}
		}
		return m_ExplosionFragments_Config;
	}

	ExplosionFragments_Config EF_CreateDefaultConfig()
	{
		ExplosionFragments_Config l_ExplosionFragments_Config = new ExplosionFragments_Config();

		l_ExplosionFragments_Config.EffectsPerRPC = 50;
		l_ExplosionFragments_Config.EnableDebug = false;

		l_ExplosionFragments_Config.Explosives.Insert(new EF_Data("RGD5Grenade", new FragmentationPreset()));
		l_ExplosionFragments_Config.Explosives.Insert(new EF_Data("M67Grenade", new FragmentationPreset));
		l_ExplosionFragments_Config.Explosives.Insert(new EF_Data("LandMineTrap", new FragmentationPreset));
		l_ExplosionFragments_Config.Explosives.Insert(new EF_Data("ClaymoreMine", new FragmentationPreset(FragmentationPresetType.CONE, 0, false, 60, vector.Forward)));
		l_ExplosionFragments_Config.Explosives.Insert(new EF_Data("Bullet_40mm_Explosive", new FragmentationPreset));
		l_ExplosionFragments_Config.Explosives.Insert(new EF_Data("Ammo_40mm_Explosive", new FragmentationPreset));

		return l_ExplosionFragments_Config;
	}

	// Used in FirearmEffects to spawn fragments on explosive projectiles
	void EF_SetupAmmoList()
	{
		GetExplosionFragments_Config();
		array<ref EF_Data> Explosives = m_ExplosionFragments_Config.Explosives;
		for (int i = 0; i < Explosives.Count(); i++)
		{
			if (Explosives[i].explosiveClassname.Contains("Bullet_"))
			{
				m_EF_ExplosiveBullets.Insert(Explosives[i].explosiveClassname);
			}
		}
	}

	override void FirearmEffects(Object source, Object directHit, int componentIndex, string surface, vector pos, vector surfNormal, vector exitPos, vector inSpeed, vector outSpeed, bool isWater, bool deflected, string ammoType)
	{
		super.FirearmEffects(source, directHit, componentIndex, surface, pos, surfNormal, exitPos, inSpeed, outSpeed, isWater, deflected, ammoType);

		if (IsServer())
		{
			if (source && source.ShootsExplosiveAmmo() && !deflected && outSpeed == vector.Zero)
			{
				if (m_EF_ExplosiveBullets.Find(ammoType) > -1)
				{
					Object dummy = CreateObject("EF_DummyExplosiveAmmoImpactObject", pos);// Need something to call CreateFragments from
					dummy.SetOrientation(inSpeed.VectorToAngles()); // Aligns with projectile orientation  
					EF_ExplosiveHitInvoker.Invoke(ammoType, surfNormal); // Sends data to the object  
				}
			}
		}
	}

	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, target, rpc_type, ctx);

		switch (rpc_type)
		{
		case EFEFFECTSLIST:
		{
			Param1 <ref array<ref EF_ImpactEffectData>> list;
			if (!ctx.Read(list)) return;

			array<ref EF_ImpactEffectData> particleList = list.param1;

			// No need for sounds and particles beyond 200 meters  
			if (!IsServer() && (vector.Distance(GetPlayer().GetPosition(), particleList[0].Pos) < 200))
			{
				foreach(EF_ImpactEffectData data : particleList)
				{
					// Creating a wave of particles. 0.7 represents an average fragment speed of 700 m/s.  
					float delay = data.Distance / 0.7;
					GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(EF_SpawnFX, delay, false, data);
				}
			}
			break;
		}
#ifdef DIAG_DEVELOPER
		case EFDEBUGLIST:
		{
			Param1 <EF_DebugData> DebugData;
			if (!ctx.Read(DebugData)) return;
				
			EF_DebugData temp = DebugData.param1;
			EF_DrawDebugShapes(temp);
			break;
		}
#endif
		}
	}

	void EF_SpawnFX(EF_ImpactEffectData data)
	{
		// Spawning particles
		if (data.ParticleId > 0)
		{
			ParticleSource m_Particle = ParticleManager.GetInstance().CreateInWorld(data.ParticleId, data.Pos, data.Rot);
			m_Particle.SetParticleAutoDestroyFlags(ParticleAutoDestroyFlags.ON_STOP);
			m_Particle.Play();
		}

		// Spawning sounds
		if (data.SoundId > 0)
		{
			string sound_set = EF_LookUpTable.SoundSetTable.Get(data.SoundId);
			EffectSound m_Sound = SEffectManager.PlaySound(sound_set, data.Pos, 0.1, 0.1);
			m_Sound.SetAutodestroy(true);
		}
	}
	
#ifdef DIAG_DEVELOPER
	void EF_DrawDebugShapes(EF_DebugData data)
	{
		if (!IsServer())
		{
			Debug.CleanupDrawShapes(m_EF_DebugShapes);
			m_EF_DebugShapes.Clear();
			
			Shape shape;
			for (int i = 0; i < data.fragEndPos.Count(); i++)
			{
				shape = Debug.DrawLine(data.begPos, data.fragEndPos[i], data.fragEndType[i]);
				m_EF_DebugShapes.Insert(shape);
			}
			
			for (int j = 0; j < data.opSphEndPos.Count(); j++)
			{
				shape = Debug.DrawSphere(data.opSphEndPos[j], 0.025, data.opSphEndType[j]);
				m_EF_DebugShapes.Insert(shape);
			}
		}
	}
#endif
};