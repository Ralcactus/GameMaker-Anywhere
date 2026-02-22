process = 0; pause = false; i = 0; list = ProcListCreate();
size = ProcessIdLength(list);
if (size) {
  if (i < size) {
    procId = ProcessId(list, i);
    info = ProcInfoFromProcIdEx(procId, KINFO_PPID | KINFO_EXEP | KINFO_CWDP | KINFO_ARGV);
    ppid = ParentProcessId(info);
    exe = ExecutableImageFilePath(info);
    cwd = CurrentWorkingDirectory(info);
    ii = 0; cmd = "";
    cmdsize = CommandLineLength(info);
    if (cmdsize) {
      for (ii = 0; ii < cmdsize; ii += 1) {
        tmp = string_replace_all(CommandLine(info, ii), @'\', @'\\');
        cmd += @'"' + string_replace_all(tmp, @'"', @'\"') + @'" ';
      }
      cmd = string_copy(cmd, 0, string_length(cmd) - 1);
    }
    FreeProcInfo(info);
  }
}
FreeProcList(list);
alarm[0] = 10; 
alarm[1] = 10; 
