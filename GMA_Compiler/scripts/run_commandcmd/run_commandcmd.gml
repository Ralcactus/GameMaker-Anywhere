function run_commandpowershell(_destination, _ps_block)
{
    var powershell = "C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe";
    var command = string_replace_all("Set-Location -LiteralPath '" + _destination + "';" + _ps_block, "\"", "\\\"");
	
    execute_shell(powershell, "-NoProfile -ExecutionPolicy Bypass -Command \"" + command + "\"");
}