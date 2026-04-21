using UnrealBuildTool;

public class BurnoutRevenge : ModuleRules
{
	public BurnoutRevenge(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"ChaosVehicles",
			"PhysicsCore"
		});
	}
}
