#ifdef SERVER
modded class Ammunition_Base
{
	protected ref EF_Data m_EF_Data;
	protected bool m_EF_UseFragments;
	protected bool m_EF_AlreadyExploded = false;
	ref ExplosionFragmentsComponent m_EF_Component = new ExplosionFragmentsComponent;

	override void EEInit()
	{
		super.EEInit();
		EF_SetupFragments();
	}

	void EF_SetupFragments()
	{
		if (GetDayZGame().GetExplosionFragments_Config())
		{
			array<ref EF_Data> Explosives = GetDayZGame().GetExplosionFragments_Config().Explosives;
			for (int i = 0; i < Explosives.Count(); i++)
			{
				if (GetType() == Explosives[i].explosiveClassname)
				{
					m_EF_Data = Explosives[i];
					m_EF_UseFragments = true;
					break;
				}
				m_EF_UseFragments = false;
			}
		}
	}

	override void OnActivatedByItem(notnull ItemBase item)
	{
		super.OnActivatedByItem(item);

		if (m_EF_UseFragments && !m_EF_AlreadyExploded)
		{
			// Explosion FX has some delay, compensate for this with m_EF_Data.delay
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(m_EF_Component.CreateFragments, m_EF_Data.delay, false, item, m_EF_Data, false, vector.Zero);
			m_EF_AlreadyExploded = true;
		}
	}

	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);

		if (m_EF_UseFragments && !m_EF_AlreadyExploded)
		{
			// Explosion FX has some delay, compensate for this with m_EF_Data.delay
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(m_EF_Component.CreateFragments, m_EF_Data.delay, false, this, m_EF_Data, false, vector.Zero);
			m_EF_AlreadyExploded = true;
		}
	}
};#endif