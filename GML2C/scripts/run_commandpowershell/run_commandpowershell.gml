function sleep(ms) {
    var start = current_time;
    while (current_time - start < ms) {}
}

function run_commandpowershell(destination, command) {
    var REALCOMMAND;

    if (os_type == os_windows) {
        REALCOMMAND = "powershell.exe -NoProfile -ExecutionPolicy Bypass -Command \"Set-Location -Path '" + destination + "'; " + command + "\"";
    } else if (os_type == os_linux || os_type == os_macosx) {
        REALCOMMAND = "bash -lc \"cd '" + destination + "' && " + command + "\"";
    } else {
        show_message("OS is not supported! Issue in run_commandpowershell.gml");
        return -1;
    }

    show_debug_message("RUN: " + REALCOMMAND);
    var process = ProcessExecuteAsync(REALCOMMAND);

    while (!CompletionStatusFromExecutedProcess(process)) {
        sleep(10);
    }

    return process;
}