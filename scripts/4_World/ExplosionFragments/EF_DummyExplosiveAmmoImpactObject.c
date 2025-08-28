#ifdef SERVER
class EF_DummyExplosiveAmmoImpactObject extends House
{
	protected ref Timer lifeTimer;
	protected float life = 3;
	ref ExplosionFragmentsComponent m_EF_Component = new ExplosionFragmentsComponent;

	void EF_DummyExplosiveAmmoImpactObject()
	{
		GetDayZGame().EF_ExplosiveHitInvoker.Insert(SetAmmoAndStartTimer);
	}

	void SetAmmoAndStartTimer(string ammoType, vector surfNormal)
	{
		if (GetDayZGame().GetExplosionFragments_Config())
		{
			array<ref EF_Data> Explosives = GetDayZGame().GetExplosionFragments_Config().Explosives;
			for (int i = 0; i < Explosives.Count(); i++)
			{
				if (ammoType == Explosives[i].explosiveClassname)
				{
					EF_Data data = Explosives[i];
					break;
				}
			}

			if (data)
			{
				// Explosion FX has some delay, compensate for this with data.delay
				GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(m_EF_Component.CreateFragments, data.delay, false, this, data, true, surfNormal);
			}
		}
		lifeTimer = new Timer;
		lifeTimer.Run(life, this, "DeleteSafe");
	}
};
#endif