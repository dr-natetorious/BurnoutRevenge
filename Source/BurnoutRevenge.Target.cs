using UnrealBuildTool;

public class BurnoutRevengeTarget : TargetRules
{
	public BurnoutRevengeTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("BurnoutRevenge");
	}
}
