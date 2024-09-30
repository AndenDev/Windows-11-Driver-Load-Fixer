# Windows Security Feature Disabler

This project provides a script that disables various Windows security features by modifying specific registry values. The features disabled by this script include Credential Guard, Device Guard, Hypervisor Enforced Code Integrity (HVCI), and the Vulnerable Driver Blocklist.

## Features

- **Disable Credential Guard**: Turns off Credential Guard by modifying the `Enabled` registry keys under `CredentialGuard`.
- **Disable Device Guard**: Disables Virtualization-Based Security and removes the requirement for a Microsoft-signed boot chain.
- **Disable Hypervisor Enforced Code Integrity (HVCI)**: Prevents Hypervisor Enforced Code Integrity from running by setting the related registry key to `0`.
- **Disable Vulnerable Driver Blocklist**: Turns off the vulnerable driver blocklist by modifying its registry settings.
