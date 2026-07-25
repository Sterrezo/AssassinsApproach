#include "Settings.h"
#include <SimpleIni.h>
#include <format>

Settings* Settings::GetSingleton() {
	static Settings singleton;
    return &singleton;
}

void Settings::Load() {
	CSimpleIniA ini;
    ini.SetUnicode();

	auto pluginName = SKSE::PluginDeclaration::GetSingleton()->GetName();
    auto path = std::format("Data/SKSE/Plugins/{}.ini", pluginName);

	ini.LoadFile(path.c_str());

	const char* sPerkFormID = ini.GetValue("Assassination Conditions", "sRequiredPerkFormID", "");
    uRequiredPerkFormID = static_cast<RE::FormID>(std::strtoul(sPerkFormID, nullptr, 16));
    ini.SetValue("Assassination Conditions", "sRequiredPerkFormID", sPerkFormID, "; FormID of the perk required to trigger assassinations, leave out the first two characters (for example BE126), leave empty to disable | Default: empty");

    sRequiredPerkPlugin = ini.GetValue("Assassination Conditions", "sRequiredPerkPlugin", "");
    ini.SetValue("Assassination Conditions", "sRequiredPerkPlugin", sRequiredPerkPlugin.c_str(), "; Plugin that the perk above comes from (for example Skyrim.esm), leave empty to disable | Default: empty");

	fAssassinationDistance = static_cast<float>(ini.GetDoubleValue("Assassination Conditions", "fAssassinationDistance", 180.000000));
	ini.SetDoubleValue("Assassination Conditions", "fAssassinationDistance", fAssassinationDistance, "; Range from which assassinations can be triggered | Default: 180.000000");

	fAssassinationHeightDifference = static_cast<float>(ini.GetDoubleValue("Assassination Conditions", "fAssassinationHeightDifference", 40.000000));
	ini.SetDoubleValue("Assassination Conditions", "fAssassinationHeightDifference", fAssassinationHeightDifference, "; Assassinations trigger if the difference in Z-axis is within this range | Default: 40.000000");

	iMaximumDetectionValue = static_cast<int>(ini.GetLongValue("Assassination Conditions", "iMaximumDetectionValue", -11));
	ini.SetLongValue("Assassination Conditions", "iMaximumDetectionValue", iMaximumDetectionValue, "; Prevent assassinations if the target's detection is higher than this value | Default: -11");

	bAllowSittingTargets = ini.GetBoolValue("Target Conditions", "bAllowSittingTargets", true);
    ini.SetBoolValue("Target Conditions", "bAllowSittingTargets", bAllowSittingTargets, "; Allow assassinations on targets that are currently sitting | Default: true");

	bAllowSleepingTargets = ini.GetBoolValue("Target Conditions", "bAllowSleepingTargets", false);
    ini.SetBoolValue("Target Conditions", "bAllowSleepingTargets", bAllowSleepingTargets, "; Allow assassinations on targets that are currently sleeping | Default: false");

	bAllowEssentialTargets = ini.GetBoolValue("Target Conditions", "bAllowEssentialTargets", false);
    ini.SetBoolValue("Target Conditions", "bAllowEssentialTargets", bAllowEssentialTargets, "; Allow assassinations on essential targets | Default: false");

	bAllowProtectedTargets = ini.GetBoolValue("Target Conditions", "bAllowProtectedTargets", true);
    ini.SetBoolValue("Target Conditions", "bAllowProtectedTargets", bAllowProtectedTargets, "; Allow assassinations on protected targets | Default: true");

	ini.SaveFile(path.c_str());
}