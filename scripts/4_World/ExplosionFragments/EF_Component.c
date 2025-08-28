#ifdef SERVER
// The place where all the magic happens
// I gotta start adding weird shit like Inedia does
class ExplosionFragmentsComponent
{
	//CONFIGURATION
	Object m_EF_Object; // Doesn't like ref for some reason.  
	ref EF_Data m_EF_Data;
	bool m_EF_IsBullet;
	vector m_EF_SurfNormal;
	int m_EF_EffectsPerRPC;

	//GENERATED VARIABLES
	vector m_EF_BegPos;
	vector m_EF_BegOri;
	bool m_UseOptimizedSphere;

	ref array<ref EF_ImpactEffectData> m_EF_EffectsList = new array<ref EF_ImpactEffectData>;

#ifdef DIAG_DEVELOPER
	ref EF_DebugData m_EF_DebugData = new EF_DebugData;
#endif

	void CreateFragments(Object object, EF_Data preset, bool bullet, vector normal)
	{
		//CONFIGURATION
		m_EF_Object = object;
		m_EF_Data = preset;
		m_EF_IsBullet = bullet;
		m_EF_SurfNormal = normal.Normalized();
		m_EF_EffectsPerRPC = GetDayZGame().GetExplosionFragments_Config().EffectsPerRPC;

		//ERROR DETECTION
		if (!GetGame().ConfigIsExisting("CfgAmmo " + m_EF_Data.ammoType))
		{
			ErrorEx("ExplosionFragments: " + m_EF_Object.GetType() + " AmmoType " + m_EF_Data.ammoType + " is misspelled or doesn't exist, changing to default");
			m_EF_Data.ammoType = "Bullet_ExplosionFragment";
		}
		if (m_EF_Data.fragmentationPreset.Type > EnumTools.GetEnumSize(FragmentationPresetType) - 1)
		{
			ErrorEx("ExplosionFragments: " + m_EF_Object.GetType() + " FragmentationPreset " + m_EF_Data.fragmentationPreset.Type.ToString() + " is out of enum bounds, changing to default");
			m_EF_Data.fragmentationPreset.Type = FragmentationPresetType.SPHERE;
		}

		//GENERATED VARIABLES
		m_EF_BegPos = CorrectedBegPos();
		m_EF_BegOri = m_EF_Object.GetOrientation();
		m_UseOptimizedSphere = OptimizedSphere_SurfaceCheck();

		StartFragmentRaycasts();

		if (m_EF_Data.createParticles || m_EF_Data.createSounds)
		{
			SendEffectsRPC();
		}

		//DEBUG
		if (GetDayZGame().GetExplosionFragments_Config().EnableDebug)
		{
			Debug_PrintLog();
#ifdef DIAG_DEVELOPER
			m_EF_DebugData.begPos = m_EF_BegPos;
			SendDebugRPC();
#endif
		}
	}

	protected vector CorrectedBegPos()
	{
		vector begPos;
		// Surface oriented offset for projectiles
		if (m_EF_IsBullet)
		{
			begPos = m_EF_Object.GetPosition() + (m_EF_SurfNormal * m_EF_Data.offset);
		}
		// Vertical offset for items
		else
		{
			begPos = m_EF_Object.GetPosition() + Vector(0, m_EF_Data.offset, 0);
		}
		return begPos;
	}

	protected bool OptimizedSphere_SurfaceCheck()
	{
		if (m_EF_Data.fragmentationPreset.Type != FragmentationPresetType.OPTIMIZEDSPHERE)
		{
			return false;
		}

		array<vector> endPoints = new array<vector>;

		float distance = 0.75;// Distance for surface check raycasts

		float angle45 = 45.0 * Math.DEG2RAD;

		if (m_EF_IsBullet)
		{
			// Calculating end points for raycasts, don't really undestand what's going on here, vector math credit goes to ChatGPT
			vector right = m_EF_SurfNormal.Perpend();
			vector forward = m_EF_SurfNormal * right;

			right = right.Normalized();
			forward = forward.Normalized();

			float heightOffset = distance * Math.Sin(angle45);
			float horizontalOffset = distance * Math.Cos(angle45);

			endPoints.Insert(m_EF_BegPos - m_EF_SurfNormal * distance);

			endPoints.Insert(m_EF_BegPos + forward * horizontalOffset - m_EF_SurfNormal * heightOffset);
			endPoints.Insert(m_EF_BegPos - forward * horizontalOffset - m_EF_SurfNormal * heightOffset);
			endPoints.Insert(m_EF_BegPos + right * horizontalOffset - m_EF_SurfNormal * heightOffset);
			endPoints.Insert(m_EF_BegPos - right * horizontalOffset - m_EF_SurfNormal * heightOffset);

			endPoints.Insert(m_EF_BegPos + (forward + right).Normalized() * horizontalOffset - m_EF_SurfNormal * heightOffset);
			endPoints.Insert(m_EF_BegPos + (forward - right).Normalized() * horizontalOffset - m_EF_SurfNormal * heightOffset);
			endPoints.Insert(m_EF_BegPos + (-forward + right).Normalized() * horizontalOffset - m_EF_SurfNormal * heightOffset);
			endPoints.Insert(m_EF_BegPos + (-forward - right).Normalized() * horizontalOffset - m_EF_SurfNormal * heightOffset);
		}
		else
		{
			endPoints.Insert(m_EF_BegPos + Vector(0, -distance, 0));

			endPoints.Insert(m_EF_BegPos + Vector(distance, -distance * Math.Tan(angle45), 0));
			endPoints.Insert(m_EF_BegPos + Vector(-distance, -distance * Math.Tan(angle45), 0));
			endPoints.Insert(m_EF_BegPos + Vector(0, -distance * Math.Tan(angle45), distance));
			endPoints.Insert(m_EF_BegPos + Vector(0, -distance * Math.Tan(angle45), -distance));

			endPoints.Insert(m_EF_BegPos + Vector(distance, -distance * Math.Tan(angle45), distance));
			endPoints.Insert(m_EF_BegPos + Vector(distance, -distance * Math.Tan(angle45), -distance));
			endPoints.Insert(m_EF_BegPos + Vector(-distance, -distance * Math.Tan(angle45), distance));
			endPoints.Insert(m_EF_BegPos + Vector(-distance, -distance * Math.Tan(angle45), -distance));
		}


		// Perfoming raycasts
		int surfaceHits = 0;
		foreach(vector endPos : endPoints)
		{
			RaycastRVParams params = new RaycastRVParams(m_EF_BegPos, endPos, m_EF_Object, 0.0);
			params.type = ObjIntersectIFire;

			array<ref RaycastRVResult> results = new array<ref RaycastRVResult>;

			if (DayZPhysics.RaycastRVProxy(params, results))
			{
				surfaceHits++;
#ifdef DIAG_DEVELOPER
				m_EF_DebugData.opSphEndPos.Insert(results[0].pos);//Diag sphere endpoint at the hit position
				m_EF_DebugData.opSphEndType.Insert(COLOR_GREEN);// Diag sphere is green, indicating a hit 
				continue;
#endif
			}
#ifdef DIAG_DEVELOPER
			m_EF_DebugData.opSphEndPos.Insert(endPos);//Diag sphere endpoint at the end position
			m_EF_DebugData.opSphEndType.Insert(COLOR_RED);// Diag sphere is red, indicating no hit
#endif
		}

		// More than 6 means close enough to the surface to use optimized preset
		if (surfaceHits > 6)
		{
			return true;
		}
		return false;
	}

	protected void StartFragmentRaycasts()
	{
		for (int i = 0; i < m_EF_Data.fragmentAmount; i++)
		{
			vector endPos = GenerateEndPos();
			RaycastRVParams params = new RaycastRVParams(m_EF_BegPos, endPos, m_EF_Object, 0.0);
			params.type = ObjIntersectIFire;

			array<ref RaycastRVResult> results = new array<ref RaycastRVResult>;

			if (DayZPhysics.RaycastRVProxy(params, results))
			{
				vector hitPosition = results[0].pos;
				float hitDistance = vector.Distance(m_EF_BegPos, hitPosition);
				vector hitNormal = results[0].dir;

				int hitParticleId = HandleParticle(results[0], hitDistance);
				int hitSoundId = HandleSound(results[0], hitDistance);

				m_EF_EffectsList.Insert(new EF_ImpactEffectData(hitPosition, hitNormal, hitParticleId, hitSoundId, hitDistance));

				HandleEntityDamage(results[0], hitPosition, hitDistance);
				
#ifdef DIAG_DEVELOPER
				m_EF_DebugData.fragEndPos.Insert(hitPosition);//Diag line endpoint at the hit position
				if (hitParticleId > 0 && hitSoundId > 0)
				{
					m_EF_DebugData.fragEndType.Insert(COLOR_GREEN);// Diag line is green, create particle and sound
				}
				else if (hitParticleId > 0)
				{
					m_EF_DebugData.fragEndType.Insert(COLOR_YELLOW);// Diag line is yellow, create particle
				}
				else if (hitSoundId > 0)
				{
					m_EF_DebugData.fragEndType.Insert(COLOR_BLUE);// Diag line is blue, create sound
				}
				else if (hitParticleId == 0 && hitSoundId == 0)
				{
					m_EF_DebugData.fragEndType.Insert(COLOR_RED);// Diag line is red, no sound and particle
				}
				continue;
#endif
			}
#ifdef DIAG_DEVELOPER
			m_EF_DebugData.fragEndPos.Insert(endPos);//Diag line endpoint at the end position
			m_EF_DebugData.fragEndType.Insert(COLOR_WHITE);// Diag line is white, indicating no hit
#endif
		}
	}

	// Different types of fragmentation
	protected vector GenerateEndPos()
	{
		vector randDir;
		switch (m_EF_Data.fragmentationPreset.Type)
		{
		case FragmentationPresetType.SPHERE:
			randDir = SpherePreset();
			break;

		case FragmentationPresetType.OPTIMIZEDSPHERE:
			// Use optimized sphere only when surface check returned true
			if (m_UseOptimizedSphere)
			{
				randDir = OptimizedSpherePreset();
			}
			else
			{
				randDir = SpherePreset();
			}
			break;

		case FragmentationPresetType.ELLIPSOID:
			randDir = EllipsoidPreset(m_EF_Data.fragmentationPreset.EllipsoidPower, m_EF_Data.fragmentationPreset.EllipsoidDirectional);
			break;

		case FragmentationPresetType.CONE:
			randDir = ConePreset(m_EF_Data.fragmentationPreset.ConeAngle, m_EF_BegOri.AnglesToVector(), m_EF_Data.fragmentationPreset.ConeVector);
			break;
		}

		vector tempEndPos = m_EF_BegPos + randDir;
		vector endPos = m_EF_BegPos + ((tempEndPos - m_EF_BegPos).Normalized() * m_EF_Data.maxDistance);
		return endPos;
	}

	protected vector SpherePreset()
	{
		float x = Math.RandomFloatInclusive(-1, 1);
		float y = Math.RandomFloatInclusive(-1, 1);
		float z = Math.RandomFloatInclusive(-1, 1);

		return Vector(x, y, z);
	}

	protected vector OptimizedSpherePreset()
	{
		return ConePreset(120, m_EF_SurfNormal, vector.Forward);
	}

	protected vector EllipsoidPreset( float EllipsoidPower, bool EllipsoidDirectional)
	{
		EllipsoidPower = 1 - EllipsoidPower;

		float x = Math.RandomFloatInclusive(-1, 1);
		float y = Math.RandomFloatInclusive(-1, 1) * EllipsoidPower;
		float z = Math.RandomFloatInclusive(-1, 1);

		vector localDir = Vector(x, y, z);

		// Align with parent object
		if (EllipsoidDirectional)
		{
			vector rotationMatrix[4];
			Math3D.YawPitchRollMatrix(m_EF_BegOri, rotationMatrix);

			localDir = localDir.Multiply3(rotationMatrix);
		}

		return localDir;
	}

	// Now I understand what's going on here (spent two days debugging it), but credit still goes to my bud ChatGPT
	protected vector ConePreset(float ConeAngle, vector BaseVector, vector ConeVector)
	{
		float radAngle = Math.DEG2RAD * ConeAngle;

		float theta = Math.RandomFloatInclusive(0, radAngle);
		float phi = Math.RandomFloatInclusive(0, Math.PI2);

		float x = Math.Sin(theta) * Math.Cos(phi);
		float y = Math.Sin(theta) * Math.Sin(phi);
		float z = Math.Cos(theta);

		vector localDir = Vector(x, y, z);
		
		if (BaseVector == vector.Zero)
		{
			ConeVector = vector.Up;
		}
		
		vector upVector = vector.Up;
        if (Math.AbsFloat(ConeVector[0]) < 0.0001 && Math.AbsFloat(ConeVector[2]) < 0.0001)
        	upVector = vector.Forward;

        vector transform[3];
        Math3D.DirectionAndUpMatrix(ConeVector, upVector, transform);
		localDir = localDir.Multiply3(transform);
		
		if (BaseVector == vector.Zero)
		{
			return localDir;
		}
		
		upVector = vector.Up;
        if (Math.AbsFloat(BaseVector[0]) < 0.0001 && Math.AbsFloat(BaseVector[2]) < 0.0001)
        	upVector = vector.Forward;
		
		Math3D.DirectionAndUpMatrix(BaseVector, upVector, transform);
        localDir = localDir.Multiply3(transform);
		
		return localDir;
	}

	protected int HandleParticle(RaycastRVResult result, float hitDistance)
	{
		int hitImpactId = 0;
		if (m_EF_Data.createParticles)
		{
			if (hitDistance > m_EF_Data.minDistanceToSpawnParticles)
			{
				// Lookup table to get particle Id
				if (result.surface.GetImpact() == "")
				{
					hitImpactId = EF_LookUpTable.ParticleIdTable.Get("Hit_Water");
				}
				else
				{
					hitImpactId = EF_LookUpTable.ParticleIdTable.Get(result.surface.GetImpact());
				}
			}
		}
		return hitImpactId;
	}

	//I don't like how it's done, but I can't think of a better way until GetSoundHit() is added https://feedback.bistudio.com/T189758
	protected int HandleSound(RaycastRVResult result, float hitDistance)
	{
		int hitSoundId = 0;
		if (m_EF_Data.createSounds)
		{
			if (hitDistance > m_EF_Data.minDistanceToSpawnSounds)
			{
				string hitSound = result.surface.GetEntryName();
				// .bisurf lookup table
				if (hitSound.Contains(".bisurf"))
				{
					int toTrim = 25;
					// Immersive Armor support https://steamcommunity.com/sharedfiles/filedetails/?id=3403371340
					if (hitSound.Contains("IA_"))
					{
						toTrim = 27;
					}
					hitSound = hitSound.Substring(toTrim, hitSound.Length() - toTrim);
					if (EF_LookUpTable.BisurfSoundTable.Get(hitSound))
					{
						hitSound = EF_LookUpTable.BisurfSoundTable.Get(hitSound);
					}
					else
					{
						hitSound = "hard_ground";
					}
				}
				else
				{
					hitSound = result.surface.GetName();
					// config lookup table
					if (EF_LookUpTable.ConfigSoundTable.Get(hitSound))
					{
						hitSound = EF_LookUpTable.ConfigSoundTable.Get(hitSound);
					}
					else
					{
						hitSound = "hard_ground";
					}
				}
				if (result.surface.GetImpact() == "")
				{
					hitSoundId = EF_LookUpTable.SoundIdTable.Get("water");
				}
				else
				{
					hitSoundId = EF_LookUpTable.SoundIdTable.Get(hitSound);
				}
			}
		}
		return hitSoundId;
	}

	// Damage EntityAI if hit something
	protected void HandleEntityDamage(RaycastRVResult result, vector hitPosition, float hitDistance)
	{
		EntityAI hitObject = EntityAI.Cast(result.obj);

		if (!hitObject)
		{
			return;
		}

		//Damage zone and hit position in model space
		int compID = result.component;
		string dmgZone = hitObject.GetDamageZoneNameByComponentIndex(result.component);
		vector MSPos = hitObject.WorldToModel(hitPosition);

		// For some reason, calling ProcessDirectDamage on Building causes access violation crash
		if (!hitObject.IsInherited(Building))
		{
			if (m_EF_Data.useDamageCoefByDistance)
			{
				float dynamicDamageCoef = Easing.EaseInQuart(1 - (hitDistance / m_EF_Data.maxDistance));
				hitObject.ProcessDirectDamage(DamageType.FIRE_ARM, hitObject, dmgZone, m_EF_Data.ammoType, MSPos, dynamicDamageCoef + m_EF_Data.damageCoef);
			}
			else
			{
				hitObject.ProcessDirectDamage(DamageType.FIRE_ARM, hitObject, dmgZone, m_EF_Data.ammoType, MSPos, m_EF_Data.damageCoef);
			}
		}
	}

	protected void SendEffectsRPC()
	{
		// Sending RPC in batches to minimize client lag
		array<ref EF_ImpactEffectData> listToSend = new array<ref EF_ImpactEffectData>;

		for (int i = 0; i < m_EF_EffectsList.Count(); i += m_EF_EffectsPerRPC)
		{
			listToSend.Clear();

			for (int j = 0; j < m_EF_EffectsPerRPC && (i + j) < m_EF_EffectsList.Count(); j++)
			{
				listToSend.Insert(m_EF_EffectsList[i + j]);
			}

			auto list = new Param1<ref array<ref EF_ImpactEffectData>>(listToSend);
			GetGame().RPCSingleParam(null, EFEFFECTSLIST, list, false);
		}
	}

	protected void Debug_PrintLog()
	{
		Print("ExplosionFragments: Object: " + m_EF_Object.GetType());

		Print("ExplosionFragments: FragmentationPreset: Type: " + EnumTools.EnumToString(FragmentationPresetType, m_EF_Data.fragmentationPreset.Type));
		Print("ExplosionFragments: FragmentationPreset: EllipsoidPower: " + m_EF_Data.fragmentationPreset.EllipsoidPower.ToString());
		Print("ExplosionFragments: FragmentationPreset: EllipsoidDirectional: " + m_EF_Data.fragmentationPreset.EllipsoidDirectional.ToString());
		Print("ExplosionFragments: FragmentationPreset: ConeAngle: " + m_EF_Data.fragmentationPreset.ConeAngle.ToString());
		Print("ExplosionFragments: FragmentationPreset: ConeVector: " + m_EF_Data.fragmentationPreset.ConeVector.ToString());

		Print("ExplosionFragments: FragmentAmount: " + m_EF_Data.fragmentAmount.ToString());
		Print("ExplosionFragments: MaxDistance: " + m_EF_Data.maxDistance.ToString());

		Print("ExplosionFragments: DamageCoef: " + m_EF_Data.damageCoef.ToString());
		Print("ExplosionFragments: useDamageCoefByDistance: " + m_EF_Data.useDamageCoefByDistance.ToString());

		Print("ExplosionFragments: AmmoType: " + m_EF_Data.ammoType);

		Print("ExplosionFragments: CreateParticles: " + m_EF_Data.createParticles.ToString());
		Print("ExplosionFragments: MinDistanceToSpawnParticles: " + m_EF_Data.minDistanceToSpawnParticles.ToString());

		Print("ExplosionFragments: CreateSounds: " + m_EF_Data.createSounds.ToString());
		Print("ExplosionFragments: MinDistanceToSpawnSounds: " + m_EF_Data.minDistanceToSpawnSounds.ToString());

		Print("ExplosionFragments: Delay: " + m_EF_Data.delay.ToString());

		Print("ExplosionFragments: IsBullet: " + m_EF_IsBullet.ToString());

		Print("ExplosionFragments: SurfaceNormal: " + m_EF_SurfNormal.ToString());

		Print("ExplosionFragments: BegPos: " + m_EF_BegPos.ToString());
		Print("ExplosionFragments: BegOri: " + m_EF_BegOri.ToString());

		if (m_EF_Data.fragmentationPreset.Type == FragmentationPresetType.OPTIMIZEDSPHERE)
		{
			Print("ExplosionFragments: UseOptimizedSphere: " + m_UseOptimizedSphere.ToString());
		}
	}
#ifdef DIAG_DEVELOPER
	protected void SendDebugRPC()
	{
		auto list = new Param1<EF_DebugData>(m_EF_DebugData);
		GetGame().RPCSingleParam(null, EFDEBUGLIST, list, false);
	}
#endif
};
#endif