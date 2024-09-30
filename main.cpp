#include <windows.h>
#include <iostream>

bool FixDriveBlockList()
{
    const LPCWSTR kVulnerableDriverBlocklistPath = L"SYSTEM\\ControlSet001\\Control\\CI\\Config";
    const LPCWSTR kDeviceGuardPath = L"SYSTEM\\ControlSet001\\Control\\DeviceGuard";
    const LPCWSTR kScenariosPath = L"SYSTEM\\ControlSet001\\Control\\DeviceGuard\\Scenarios";
    const LPCWSTR kCredentialGuardPath = L"SYSTEM\\ControlSet001\\Control\\DeviceGuard\\Scenarios\\CredentialGuard";

    bool isChange = false;
    HKEY key;

    auto SetRegistryValueIfNotZero = [](HKEY key, LPCWSTR valueName, DWORD desiredValue = 0) -> bool {
        DWORD currentValue = 0;
        DWORD dataSize = sizeof(DWORD);
        if (RegQueryValueEx(key, valueName, nullptr, nullptr, reinterpret_cast<LPBYTE>(&currentValue), &dataSize) == ERROR_SUCCESS) {
            if (currentValue != desiredValue) {
                if (RegSetValueEx(key, valueName, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&desiredValue), sizeof(desiredValue)) == ERROR_SUCCESS) {
                    return true;
                }
                else {
                    std::wcerr << L"Failed to set value: " << valueName << std::endl;
                }
            }
        }
        else {
            std::wcerr << L"Failed to query value: " << valueName << std::endl;
        }
        return false;
        };

    // Disable Vulnerable Driver Blocklist
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, kVulnerableDriverBlocklistPath, 0, KEY_ALL_ACCESS, &key) == ERROR_SUCCESS) {
        isChange |= SetRegistryValueIfNotZero(key, L"VulnerableDriverBlocklistEnable", 0); // 0 to disable
        RegCloseKey(key);
    }
    else {
        std::wcerr << L"Failed to open registry key: " << kVulnerableDriverBlocklistPath << std::endl;
    }

    // Disable Device Guard
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, kDeviceGuardPath, 0, KEY_ALL_ACCESS, &key) == ERROR_SUCCESS) {
        isChange |= SetRegistryValueIfNotZero(key, L"EnableVirtualizationBasedSecurity", 0); // 0 to disable VBS
        isChange |= SetRegistryValueIfNotZero(key, L"RequireMicrosoftSignedBootChain", 0);  // 0 to disable signed boot chain requirement
        RegCloseKey(key);
    }
    else {
        std::wcerr << L"Failed to open registry key: " << kDeviceGuardPath << std::endl;
    }

    // Disable Hypervisor Enforced Code Integrity
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, kScenariosPath, 0, KEY_ALL_ACCESS, &key) == ERROR_SUCCESS) {
        isChange |= SetRegistryValueIfNotZero(key, L"HypervisorEnforcedCodeIntegrity", 0); // 0 to disable HVCI
        RegCloseKey(key);
    }
    else {
        std::wcerr << L"Failed to open registry key: " << kScenariosPath << std::endl;
    }

    // Disable Credential Guard
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, kCredentialGuardPath, 0, KEY_ALL_ACCESS, &key) == ERROR_SUCCESS) {
        isChange |= SetRegistryValueIfNotZero(key, L"Enabled", 0);  // 0 to disable Credential Guard
        RegCloseKey(key);
    }
    else {
        std::wcerr << L"Failed to open registry key: " << kCredentialGuardPath << std::endl;
    }

    if (isChange) {
        std::wcout << L"Settings have been prepared and will be restarted after 10 seconds" << std::endl;
        Sleep(10 * 1000);
        system("shutdown /r");
        return false;
    }
    return true;
}

int main()
{
    if (FixDriveBlockList()) {
        std::wcout << L"No changes were necessary." << std::endl;
    }
    else {
        std::wcout << L"System restart initiated to apply changes." << std::endl;
    }
    getchar();
    return 0;
}
