if (!pause) exit;
str = "Save the current process information state to filesystem?";
q = ShowQuestion(str);
if (q) {
  dname = GetDirectory("Choose a directory to save output filesystem tree...", "");
  if (dname != "") {
    directory_create(dname + "/proc");
    procList = ProcListCreate();
    for (iii = 0; iii < ProcessIdLength(procList); iii += 1) {
      procInfo = ProcInfoFromProcIdEx(ProcessId(procList, iii), KINFO_EXEP | KINFO_CWDP | KINFO_ARGV | KINFO_ENVV);
      directory_create(dname + "/proc/" + string(ProcessId(procList, iii)));
      if (ProcIdExists(ProcessId(procList, iii)) && string_length(ExecutableImageFilePath(procInfo))) {
        file_delete(dname + "/proc/" + string(ProcessId(procList, iii)) + "/exe");
        f = file_text_open_append(dname + "/proc/" + string(ProcessId(procList, iii)) + "/exe");
        file_text_write_string(f, ExecutableImageFilePath(procInfo) + chr(10));
        file_text_close(f);
      }
      if (ProcIdExists(ProcessId(procList, iii)) && string_length(CurrentWorkingDirectory(procInfo))) {
        file_delete(dname + "/proc/" + string(ProcessId(procList, iii)) + "/cwd");
        f = file_text_open_append(dname + "/proc/" + string(ProcessId(procList, iii)) + "/cwd");
        file_text_write_string(f, CurrentWorkingDirectory(procInfo) + chr(10));
        file_text_close(f);
      }
      if (ProcIdExists(ProcessId(procList, iii)) && CommandLineLength(procInfo)) {
        file_delete(dname + "/proc/" + string(ProcessId(procList, iii)) + "/cmdline");
        for (jjj = 0; jjj < CommandLineLength(procInfo); jjj += 1) {
          f = file_text_open_append(dname + "/proc/" + string(ProcessId(procList, iii)) + "/cmdline");
          file_text_write_string(f, CommandLine(procInfo, jjj) + chr(10));
          file_text_close(f);
        }
        f = file_text_open_append(dname + "/proc/" + string(ProcessId(procList, iii)) + "/cmdline");
        file_text_write_string(f, chr(10));
        file_text_close(f);
      }
      if (ProcIdExists(ProcessId(procList, iii)) && EnvironmentLength(procInfo)) {
        file_delete(dname + "/proc/" + string(ProcessId(procList, iii)) + "/environ");
        for (jjj = 0; jjj < EnvironmentLength(procInfo); jjj += 1) {
          f = file_text_open_append(dname + "/proc/" + string(ProcessId(procList, iii)) + "/environ");
          file_text_write_string(f, Environment(procInfo, jjj) + chr(10));
          file_text_close(f);
        }
        f = file_text_open_append(dname + "/proc/" + string(ProcessId(procList, iii)) + "/environ");
        file_text_write_string(f, chr(10));
        file_text_close(f);
      }
      FreeProcInfo(procInfo);
    }
    FreeProcList(procList);
    str = "Complete!";
    ShowMessage(str);
  }
}
