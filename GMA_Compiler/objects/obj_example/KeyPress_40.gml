if (pause) {
  list = ProcListCreate();
  size = ProcessIdLength(list); 
  i -= 1; if (i < 0) i = size - 1;
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
          cmd = CommandLine(info, 0);
      }
      FreeProcInfo(info);
    }
  }
  FreeProcList(list);
}