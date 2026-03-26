function run_commandpowershell(destination, command)
{	
    var REALCOMMAND = "powershell.exe -NoProfile -ExecutionPolicy Bypass -Command \"Set-Location -Path '" + destination + "'; " + command + "\"";
   
   return ProcessExecuteAsync(REALCOMMAND);
}